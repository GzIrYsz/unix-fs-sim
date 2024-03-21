/**
* @file ufs.c
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/file.h>

#include "logging/logging.h"
#include "unix_fs_sim/ufs.h"

extern logger_t *logger;

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
    if ((fd = open(path, O_WRONLY | O_CREAT | O_EXCL)) == -1) {
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
}