/**
 * @file data_bitmap.c
 * @brief This file contains the implementation of the CRUD operations available on a data bitmap.
 * @author Pierre FRANCK-PAPUCHON
 * @version 1.0.0
 * @date 03-29-2024
 */

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include "logging/logging.h"

#include "data_bitmap.h"


extern logger_t *logger;

int create_databitmap(partition_t p){
    off_t bitmap_pos = (int) p.super_bloc.block_size;

    if (lseek(p.fd, bitmap_pos, SEEK_SET) == -1){
        logger->error("An error occurred when trying to move the head.");
        return -1;
    }

    uint8_t* bitmap = (uint8_t*) malloc(p.super_bloc.nb_data * sizeof(uint8_t));
    if (bitmap == NULL){
        logger->error("An error occured when trying to allocate your bitmap");
        return -1;
    }

    if (write(p.fd, bitmap, p.super_bloc.nb_data) == -1){
        logger->error("An error occurred when trying to create the data bitmap.");
        free(bitmap);
        return -1;
    }

    free(bitmap);
    logger->info("Data bitmap created");
    return 0;
}

int read_databitmap(partition_t p){
    off_t bitmap_pos = (int) p.super_bloc.block_size;
    if (lseek(p.fd, bitmap_pos, SEEK_SET) == -1){
        logger->error("An error occurred when trying to move the head.");
        return -1;
    }

    if (read(p.fd, p.data_bitmap, p.super_bloc. nb_data) == -1) {
        logger->error("An error occurred when trying to read the data bitmap.");
        return -1;
    }

    logger->trace("Data bitmap read");
    return 0;
}

int update_databitmap(partition_t p){
    off_t bitmap_pos = (int) p.super_bloc.block_size;
    if (lseek(p.fd, bitmap_pos, SEEK_SET) == -1){
        logger->error("An error occurred when trying to move the head.");
        return -1;
    }

    if (write(p.fd, p.data_bitmap, p.super_bloc. nb_data) == -1) {
        logger->error("An error occurred when trying to update the data bitmap.");
        return -1;
    }

    logger->trace("Data bitmap update");
    return 0;

}

int delete_databitmap(partition_t p){
    off_t bitmap_pos = (int) p.super_bloc.block_size;
    if (lseek(p.fd, bitmap_pos, SEEK_SET) == -1){
        logger->error("An error occurred when trying to move the head.");
        return -1;
    }

    uint8_t* bitmap = (uint8_t*) malloc(p.super_bloc.nb_data * sizeof(uint8_t ));

    if (write(p.fd, bitmap, p.super_bloc.nb_data) == -1) {
        logger->error("An error occurred when trying to delete the inode bitmap.");
        free(bitmap);
        return -1;
    }
    free(p.data_bitmap);
    logger->info("Data bitmap deleted");
    return 0;
}

uint32_t next_free_data(partition_t p){
    if (p.super_bloc.nb_data_free <= 0){
        logger->warn("No more free data");
        return 0;
    }

    uint32_t i = 0;
    while (p.data_bitmap[i]) i++;
    return i;
}