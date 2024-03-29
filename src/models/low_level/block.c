/**
 * @file block.c
 * @brief
 * @author Thomas REMY
 * @version 0.1.0
 * @date 03-28-2024
 */

#include <fcntl.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>

#include "logging/logging.h"

#include "../../exits.h"

#include "block.h"

extern logger_t *logger;

int create_block(partition_t p, uint32_t i) {
    if (i > p.super_bloc.nb_blocks) {
        logger->error("You are trying to create a block beyond the partition.");
        return -1;
    }
    logger->trace("Block created.");
    return 0;
}

int read_block(partition_t p, void *buf, uint32_t i) {
    if (i > p.super_bloc.nb_blocks) {
        logger->error("You are trying to read a block beyond the partition.");
        return -1;
    }

    if (lseek(p.fd, (off_t) (i * p.super_bloc.block_size) - 1, SEEK_SET) == -1) {
        logger->error("An error occurred when trying to move the head.");
        return -1;
    }
    if (read(p.fd, buf, p.super_bloc.block_size) == -1) {
        logger->error("An error occurred when trying to read the file.");
        return -1;
    }
    logger->trace("Block read.");
    return 0;
}

int update_bloc(partition_t p, void *buf, uint32_t data_length, uint32_t i, uint32_t offset) {
    if (i > p.super_bloc.nb_blocks) {
        logger->error("You are trying to update a block beyond the partition.");
        return -1;
    }

    if (lseek(p.fd, (off_t) (i * p.super_bloc.block_size) + offset - 1, SEEK_SET) == -1) {
        logger->error("An error occurred when trying to move the head.");
        return -1;
    }
    if (write(p.fd, buf, data_length) == -1) {
        logger->error("An error occurred when trying to update the block.");
        return -1;
    }
    logger->trace("Block updated.");
    return 0;
}

int write_bloc(partition_t p, const void *buf, uint32_t i) {
    if (i > p.super_bloc.nb_blocks) {
        logger->error("You are trying to write a block beyond the partition.");
        return -1;
    }

    if (lseek(p.fd, (off_t) (i * p.super_bloc.block_size) - 1, SEEK_SET) == -1) {
        logger->error("An error occurred when trying to move the head.");
        return -1;
    }
    if (write(p.fd, buf, p.super_bloc.block_size) == -1) {
        logger->error("An error occurred when trying to write the block.");
        return -1;
    }
    logger->trace("Block written.");
    return 0;
}

int delete_block(partition_t p, uint32_t i) {
    if (i > p.super_bloc.nb_blocks) {
        logger->error("You are trying to delete a block beyond the partition.");
        return -1;
    }

    if (lseek(p.fd, (off_t) (i * p.super_bloc.block_size) - 1, SEEK_SET) == -1) {
        logger->error("An error occurred when trying to move the head.");
        return -1;
    }

    char *buf;
    if ((buf = (char*) malloc(p.super_bloc.block_size * sizeof(char))) == NULL) {
        logger->error("An error occurred when trying to allocate memory.");
        exit(ERR_MALLOC);
    }
    memset(buf, 0, p.super_bloc.block_size);
    if (write(p.fd, buf, p.super_bloc.block_size) == -1) {
        logger->error("An error occurred when trying to delete the block.");
        return -1;
    }

    logger->trace("Block deleted.");
    return 0;
}