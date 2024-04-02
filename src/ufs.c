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
#include <string.h>
#include <sys/file.h>
#include <unistd.h>

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

partition_t *p_mounted;

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

    partition_t *p = (partition_t*) malloc(sizeof(partition_t));

    p->fd = fd;
    p->super_bloc = super_bloc;

    if (lseek(fd, 0, SEEK_SET) == -1) {
        logger->error("An error occurred when trying to move the head.");
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

    for (int i = 0; i < (int) ceil((double) (p->super_bloc.nb_inodes * sizeof(dir_entry_t)) / (double) p->super_bloc.block_size); ++i) {
        if (create_data(p, i) == -1) {
            logger->error("An error occurred when trying to create root dir data.");
            return -1;
        }
    }

    if (update_databitmap(p) == -1) {
        logger->error("An error occurred when trying to update the data bitmap.");
        return -1;
    }

    if (update_bloc(p, &p->super_bloc, sizeof(super_bloc_t), 0, 0) == -1) {
        logger->error("An error occurred when trying to write the superblock to the partition.");
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

int mount(char *path) {
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

    super_bloc_t super_bloc;
    if (lseek(fd, 0, SEEK_SET) == -1) {
        logger->error("An error occurred when trying to move the head.");
        return -1;
    }
    if (read(fd, &super_bloc.magic_number, sizeof(uint32_t)) == -1) {
        logger->error("An error occurred when trying to read the magic number.");
        return -1;
    }
    if (read(fd, &super_bloc.block_size, sizeof(uint32_t)) == -1) {
        logger->error("An error occurred when trying to read the bloc size.");
        return -1;
    }
    if (read(fd, &super_bloc.nb_blocks, sizeof(uint32_t)) == -1) {
        logger->error("An error occurred when trying to read the number of blocks.");
        return -1;
    }
    if (read(fd, &super_bloc.nb_data, sizeof(uint32_t)) == -1) {
        logger->error("An error occurred when trying to read the number of data blocks.");
        return -1;
    }
    if (read(fd, &super_bloc.nb_data_free, sizeof(uint32_t)) == -1) {
        logger->error("An error occurred when trying to read the number of free data blocks.");
        return -1;
    }
    if (read(fd, &super_bloc.nb_inodes, sizeof(uint32_t)) == -1) {
        logger->error("An error occurred when trying to read the number of inodes.");
        return -1;
    }
    if (read(fd, &super_bloc.nb_inodes_free, sizeof(uint32_t)) == -1) {
        logger->error("An error occurred when trying to read the number of free inodes.");
        return -1;
    }
    if (read(fd, &super_bloc.nb_inode_blocks, sizeof(uint32_t)) == -1) {
        logger->error("An error occurred when trying to read the number of inode blocks.");
        return -1;
    }

    partition_t *p = (partition_t*) malloc(sizeof(partition_t));
    p->fd = fd;
    p->super_bloc = super_bloc;
    p->nb_opened_files = 0;
    p->data_bitmap = (uint8_t*) malloc(super_bloc.nb_data * sizeof(uint8_t));
    p->inode_bitmap = (uint8_t*) malloc(super_bloc.nb_inodes * sizeof(uint8_t));
    p->directory = (dir_entry_t*) malloc(super_bloc.nb_inodes * sizeof(dir_entry_t));
    read_databitmap(p);
    read_inodebitmap(p);
    read_directory(p);

    p_mounted = p;
    logger->info("Partition mounted.");
    return 0;
}

file_t* my_open(char *file_name) {
    dir_entry_t *main_dir = p_mounted->directory;

    int i = 0;
    char buf[100];
    sprintf(buf, "strcmp : %d", strcmp(file_name, main_dir[i].name));
    logger->debug(buf);
    while (!strcmp(file_name, main_dir[i].name) && i < (p_mounted->super_bloc.nb_inodes - p_mounted->super_bloc.nb_inodes_free)) {
        i++;
    }

    file_t *f = (file_t*) malloc(sizeof(file_t));
    if (i < (p_mounted->super_bloc.nb_inodes - p_mounted->super_bloc.nb_inodes_free)) {
        f->inode = main_dir[i].inode;
    }

    if ((f->inode = create_file(file_name, p_mounted)) == -1) {
        logger->error("An error occurred when trying to create the file.");
        return NULL;
    }

    strcpy(f->name, file_name);
    f->offset = 0;
    p_mounted->opened_files[p_mounted->nb_opened_files++] = f;
    logger->info("File opened.");
    return f;
}

int my_write(file_t *f, void *buffer, int nb_bytes) {
    inode_t i;
    read_inode(p_mounted, &i, f->inode);
    uint32_t start_size = i.memory_size_data;
    int write_pos = floor((double) f->offset / (double) p_mounted->super_bloc.block_size);
    if (write_pos >= NB_DATA_BLOCKS_INODE) {
        logger->error("Max size reached. Impossible to write here.");
        return -1;
    }

    off_t write_offset = i.memory_size_data - (write_pos * p_mounted->super_bloc.block_size);
    off_t block_offset = get_data_offset(p_mounted, i.data_blocks[write_pos]);

    if (update_bloc(p_mounted, buffer, nb_bytes, block_offset / p_mounted->super_bloc.block_size, f->offset) == -1) {
        logger->error("An error occurred when trying to write to the file.");
        return -1;
    }

    if (nb_bytes <= 0) {
        i.memory_size_data += nb_bytes;
        update_inode(p_mounted, i, f->inode);
        return nb_bytes;
    }

    size_t nb_blocks_to_write = floor((double) nb_bytes / (double) p_mounted->super_bloc.block_size);
    for (int j = 0; j < nb_blocks_to_write; ++j) {
        uint32_t new_data_block;
        if ((new_data_block = next_free_data(p_mounted)) == -1) {
            logger->error("An error occurred when trying to find a new free data block.");
            return -1;
        }
        if (create_data(p_mounted, new_data_block) == -1) {
            logger->error("An error occurred when trying to create data.");
        }
        i.data_blocks[write_pos + j] = new_data_block;
        update_inode(p_mounted, i, f->inode);

        if (j < nb_blocks_to_write - 1) {
            if (update_data(p_mounted, (uint8_t*) buffer, new_data_block) == -1) {
                logger->error("An error occurred when trying to write data in a new block.");
                return -1;
            }
        } else {
            if (update_bloc(p_mounted, buffer, nb_bytes, new_data_block, 0) == -1) {
                logger->error("An error occurred when trying to write data in a new block.");
                return i.memory_size_data - start_size;
            }
        }
    }
    i.memory_size_data += nb_bytes;
    update_inode(p_mounted, i, f->inode);
    logger->info("Data written.");
    f->offset += nb_bytes;
    return nb_bytes;;
}

int my_read(file_t *f, void *buffer, int nb_bytes) {
    inode_t i;
    read_inode(p_mounted, &i, f->inode);

    int real_nbytes_to_read = nb_bytes > i.memory_size_data ? i.memory_size_data : nb_bytes;

    int nb_data_blocks = (int) ceil((double) real_nbytes_to_read / (double) p_mounted->super_bloc.block_size);
    for (int j = 0; j < real_nbytes_to_read; j++) {
    }
}

void my_seek(file_t *f, int offset, int base) {
    inode_t i;
    switch (base) {
        case SEEK_SET:
            f->offset = offset;
            break;
        case SEEK_CUR:
            f->offset += offset;
            break;
        case SEEK_END:
            read_inode(p_mounted, &i, f->inode);
            f->offset = i.memory_size_data - offset;
        default:
            logger->error("Base unrecognized.");
    }
}

size_t size(file_t *f) {
    inode_t i;
    read_inode(p_mounted, &i, f->inode);

    return i.memory_size_data;
}