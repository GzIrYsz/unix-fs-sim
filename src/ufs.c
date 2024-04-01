/**
* @file ufs.c
* @brief
* @author Thomas REMY
* @author Pierre FRANCK-PAPUCHON
* @version 0.1.0
* @date 03-28-2024
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/file.h>

#include "logging/logging.h"
#include "unix_fs_sim/ufs.h"

#include "ufs.priv.h"
#include "models/high_level/directory.h"
#include "models/high_level/file.h"
#include "models/low_level/block.h"
#include "models/mid_level/data.h"
#include "models/mid_level/data_bitmap.h"
#include "models/mid_level/inode.h"
#include "models/mid_level/inode_bitmap.h"

extern logger_t *logger;

partition_t init_partition(int fd, super_bloc_t super_bloc) {
    partition_t p;

    p.fd = fd;
    p.super_bloc = super_bloc;
    p.nb_opened_files = 0;

    return p;
}

int mkpart(char *path, size_t size, size_unit_t unit) {
    if (access(path, F_OK) == 0) {
        logger->error("This file already exists. Please choose another one.");
        return -1;
    }

    switch (unit) {
        case KB:
            size = 1000 * size;
            break;
        case MB:
            size = 1000000 * size;
            break;
        case GB:
            size = 1000000000 * size;
            break;
    }

    int fd;
    if ((fd = open(path, O_WRONLY | O_CREAT | O_EXCL, 0600)) == -1) {
        logger->error("An error occurred when trying to open the file.");
        return -1;
    }
    if (lseek(fd, (off_t) size - 1, SEEK_SET) == -1) {
        logger->error("An error occurred when trying to move the head.");
        return -1;
    }
    if (write(fd, "", 1) == -1) {
        logger->error("An error occurred when trying to write the partition.");
        return -1;
    }
    if (close(fd) == -1) {
        logger->error("An error occurred when trying to close the file.");
        return -1;
    }
    logger->info("Partition created.");
    return 0;
}

int mkfs(char *path, block_size_t block_size, uint8_t nb_inodes) {
    if (access(path, F_OK) != 0) {
        char log_buf[1024];
        sprintf(log_buf, "This partition does not exists: %s", path);
        logger->error(log_buf);
        return -1;
    }

    int fd;
    if ((fd = open(path, O_RDWR)) == -1) {
        logger->error("An error occurred when trying to open the partition.");
        return -1;
    }

    super_bloc_t super_bloc = {
            .magic_number = MAGIC_NUMBER,
            .block_size = block_size,
            .nb_blocks = (uint32_t) floor((double)lseek(fd, 0, SEEK_END) / (double) block_size)
    };
    super_bloc.nb_inode_blocks = (uint32_t) ceil((double) super_bloc.nb_blocks * 0.10); // TODO : Implémenter le formatage avec un nombre d'inodes dynamique
    super_bloc.nb_inodes = super_bloc.nb_inode_blocks * sizeof(inode_t);
    super_bloc.nb_inodes_free = super_bloc.nb_inodes;
    uint32_t nb_data_total = super_bloc.nb_blocks - 1 - (uint32_t) ceil((double) super_bloc.nb_inodes / (double) super_bloc.block_size) - super_bloc.nb_inode_blocks;
    super_bloc.nb_data = nb_data_total - (uint32_t) ceil((double) nb_data_total / (double) super_bloc.block_size);
    super_bloc.nb_data_free = super_bloc.nb_data;

    partition_t p = {
            .fd = fd,
            .super_bloc = super_bloc
    };

    if (lseek(fd, 0, SEEK_SET) == -1) {
        logger->error("An error occurred when trying to move the head.");
        return -1;
    }

    if (update_bloc(p, &super_bloc, sizeof(super_bloc_t), 0, 0) == -1) {
        logger->error("An error occurred when trying to write the superblock to the partition.");
        return -1;
    }

    if (create_databitmap(p) == -1) {
        logger->error("An error occurred when trying to create the data bitmap.");
        return -1;
    }

    if (create_inodebitmap(p) == -1) {
        logger->error("An error occurred when trying to create the inode bitmap.");
        return -1;
    }
    if (close(fd) == -1) {
        logger->error("An error occurred when trying to close the partition.");
        return -1;
    }
    logger->info("Filesystem created.");
    return 0;
}

int my_format(char *partition_name) {
    return mkfs(partition_name, LARGE, 10);
}