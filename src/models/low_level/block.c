/**
 * @file block.c
 * @brief
 * @author Thomas REMY
 * @author Pierre FRANCK-PAPUCHON
 * @version 0.1.0
 * @date 03-28-2024
 */

#include <fcntl.h>
#include <unistd.h>

#include "logging/logging.h"

#include "block.h"

extern logger_t *logger;

// TODO: Change inputs to take a fd, not a path.
// TODO: When writing, do not use the size of a block, it may read beyond the actual buffer (or precise the buffer should have a length = to the block size).

int create_block(partition_stat_t p, unsigned int i) {
    if (i > p.super_bloc.nb_blocks) {
        logger->error("You are trying to create a block beyond the partition.");
        return -1;
    }
    logger->trace("Block created.");
    return 0;
}

int read_block(partition_stat_t p, void *buf, unsigned int i) {
    if (i > p.super_bloc.nb_blocks) {
        logger->error("You are trying to read a block beyond the partition.");
        return -1;
    }

    int fd;
    if ((fd = open(p.path, O_RDONLY)) == -1) {
        logger->error("An error occurred when trying to open the file.");
        return -1;
    }
    if (lseek(fd, (off_t) (i * p.super_bloc.block_size) - 1, SEEK_SET) == -1) {
        logger->error("An error occurred when trying to move the head.");
        return -1;
    }
    if (read(fd, buf, p.super_bloc.block_size) == -1) {
        logger->error("An error occurred when trying to read the file.");
        return -1;
    }
    if (close(fd) == -1) {
        logger->error("An error occurred when trying to close the file.");
        return -1;
    }
    logger->trace("Block read.");
    return 0;
}

int update_block(partition_stat_t p, const void *buf, unsigned int i) {
    if (i > p.super_bloc.nb_blocks) {
        logger->error("You are trying to update a block beyond the partition.");
        return -1;
    }

    int fd;
    if ((fd = open(p.path, O_WRONLY)) == -1) {
        logger->error("An error occurred when trying to open the file.");
        return -1;
    }
    if (lseek(fd, (off_t) (i * p.super_bloc.block_size) - 1, SEEK_SET) == -1) {
        logger->error("An error occurred when trying to move the head.");
        return -1;
    }
    if (write(fd, buf, p.super_bloc.block_size) == -1) {
        logger->error("An error occurred when trying to update the file.");
        return -1;
    }
    if (close(fd) == -1) {
        logger->error("An error occurred when trying to close the file.");
        return -1;
    }
    logger->trace("Block updated.");
    return 0;
}

int delete_block(partition_stat_t p, unsigned int i) {
    logger->trace("Block deleted.");
    return 0;
}