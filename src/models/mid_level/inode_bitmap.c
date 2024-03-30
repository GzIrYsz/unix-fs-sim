/**
 * @file inode_bitmap.c
 * @brief This file contains the implementations of the CRUD operations available on an inode bitmap.
 * @author Thomas REMY
 * @version 1.0.0
 * @date 03-29-2024
 */

#include <math.h>
#include <stdlib.h>
#include <unistd.h>

#include "logging/logging.h"

#include "inode_bitmap.h"

extern logger_t *logger;

int create_inodebitmap(partition_t p) {
    off_t bitmap_pos = (int) (ceil(p.super_bloc.nb_data / p.super_bloc.block_size) + 1) * p.super_bloc.block_size;
    if (lseek(p.fd, bitmap_pos, SEEK_SET) == -1) {
        logger->error("An error occurred when trying to move the head.");
        return -1;
    }

    uint8_t *bitmap = (uint8_t*) malloc(p.super_bloc.nb_inodes * sizeof(uint8_t));
    if (write(p.fd, bitmap, p.super_bloc.nb_inodes) == -1) {
        logger->error("An error occurred when trying to create the inode bitmap.");
        free(bitmap);
        return -1;
    }
    free(bitmap);
    logger->info("Inode bitmap created.");
    return 0;
}

int read_inodebitmap(partition_t p) {
    off_t bitmap_pos = (int) (ceil(p.super_bloc.nb_data / p.super_bloc.block_size) + 1) * p.super_bloc.block_size;
    if (lseek(p.fd, bitmap_pos, SEEK_SET) == -1) {
        logger->error("An error occurred when trying to move the head.");
        return -1;
    }

    if (read(p.fd, p.inode_bitmap, p.super_bloc.nb_inodes) == -1) {
        logger->error("An error occurred when trying to read the inode bitmap.");
        return -1;
    }
    logger->trace("Inode bitmap read.");
    return 0;
}

int update_inodebitmap(partition_t p) {
    off_t bitmap_pos = (int) (ceil(p.super_bloc.nb_data / p.super_bloc.block_size) + 1) * p.super_bloc.block_size;
    if (lseek(p.fd, bitmap_pos, SEEK_SET) == -1) {
        logger->error("An error occurred when trying to move the head.");
        return -1;
    }

    if (write(p.fd, p.inode_bitmap, p.super_bloc.nb_inodes) == -1) {
        logger->error("An error occurred when trying to update the inode bitmap.");
        return -1;
    }
    logger->trace("Inode bitmap updated.");
    return 0;
}

int delete_inodebitmap(partition_t p) {
    off_t bitmap_pos = (int) (ceil(p.super_bloc.nb_data / p.super_bloc.block_size) + 1) * p.super_bloc.block_size;
    if (lseek(p.fd, bitmap_pos, SEEK_SET) == -1) {
        logger->error("An error occurred when trying to move the head.");
        return -1;
    }

    uint8_t *bitmap = (uint8_t*) malloc(p.super_bloc.nb_inodes * sizeof(uint8_t));
    if (write(p.fd, bitmap, p.super_bloc.nb_inodes) == -1) {
        logger->error("An error occurred when trying to delete the inode bitmap.");
        free(bitmap);
        return -1;
    }
    free(bitmap);
    logger->info("Inode bitmap deleted.");
    return 0;
}

uint32_t next_free_inode(partition_t p) {
    if (p.super_bloc.nb_inodes_free <= 0) {
        logger->warn("No more free inode.");
        return -1;
    }

    uint32_t i = 0;
    while (p.inode_bitmap[i]) i++;
    return i;
}