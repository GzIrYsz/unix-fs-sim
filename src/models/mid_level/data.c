/**
 * @file data.c
 * @brief This file contains the implementation of the CRUD operations available on data.
 * @author Thomas REMY
 * @version 1.0.0
 * @date 03-31-2024
 */

#include <math.h>
#include <stdint.h>
#include <unistd.h>

#include "logging/logging.h"

#include "data.h"

logger_t  *logger;

off_t get_data_offset(partition_t *p, uint32_t i) {
    double block_size = p->super_bloc.block_size;
    double nb_data = p->super_bloc.nb_data;
    double nb_inodes = p->super_bloc.nb_inodes;
    return (off_t) (1 + ceil(nb_data / block_size) + ceil(nb_inodes / block_size) + ceil((nb_inodes * sizeof(inode_t)) / block_size) + i) * block_size;
}

int create_data(partition_t *p, uint32_t i) {
    if (i > p->super_bloc.nb_data) {
        logger->error("You are trying to create data beyond the accepted range.");
        return -1;
    }

    if (p->data_bitmap[i] == 1) {
        logger->error("You are trying to create data that already exists.");
        return -1;
    }

    p->data_bitmap[i] = 1;
    p->super_bloc.nb_data_free--;

    logger->trace("Data created.");
    return 0;
}

int read_data(partition_t *p, uint8_t *data, uint32_t i) {
    if (i > p->super_bloc.nb_data) {
        logger->error("You are trying to read data beyond the accepted range.");
        return -1;
    }

    if (p->data_bitmap[i] == 0) {
        logger->error("You are trying to read data that does not exists.");
        return -1;
    }

    off_t data_pos = get_data_offset(p, i);
    if (lseek(p->fd, data_pos, SEEK_SET) == -1) {
        logger->error("An error occurred when trying to move the head.");
        return -1;
    }
    if (read(p->fd, data, p->super_bloc.block_size) == -1) {
        logger->error("An error occurred when trying to read data.");
        return -1;
    }

    logger->trace("Data read.");
    return 0;
}

int update_data(partition_t *p, const uint8_t *data, uint32_t i) {
    if (i > p->super_bloc.nb_data) {
        logger->error("You are trying to update data beyond the accepted range.");
        return -1;
    }

    if (p->data_bitmap[i] == 0) {
        logger->error("You are trying to update data that does not exists.");
        return -1;
    }

    off_t data_pos = get_data_offset(p, i);
    if (lseek(p->fd, data_pos, SEEK_SET) == -1) {
        logger->error("An error occurred when trying to move the head.");
        return -1;
    }
    if (write(p->fd, data, p->super_bloc.block_size) == -1) {
        logger->error("An error occurred when trying to update data.");
        return -1;
    }

    logger->trace("Data updated.");
    return 0;
}

int delete_data(partition_t *p, uint32_t i) {
    if (i > p->super_bloc.nb_data) {
        logger->error("You are trying to delete data beyond the accepted range.");
        return -1;
    }

    if (p->data_bitmap[i] == 0) {
        logger->error("You are trying to delete data that does not exists.");
        return -1;
    }

    p->data_bitmap[i] = 0;
    p->super_bloc.nb_data_free++;

    logger->trace("Data deleted.");
    return 0;
}