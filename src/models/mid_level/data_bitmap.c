/**
 * @file data_bitmap.c
 * @brief This file contains the implementation of the CRUD operations available on a data bitmap.
 * @author Pierre FRANCK-PAPUCHON
 * @version 1.0.0
 * @date 03-29-2024
 */

#include <stdlib.h>

#include "logging/logging.h"

#include "data_bitmap.h"


extern logger_t *logger;

int create_databitmap(partition_t p){
    uint32_t* data_bitmap;
    uint32_t nb_data = p.super_bloc.nb_data;
    data_bitmap = (uint32_t *) malloc(nb_data * sizeof(uint32_t));

    if (data_bitmap == NULL){
        logger->error("You got on error on your alloction of your blockbitmap.");
        return -1;
    }

    for (int i = 0; i < nb_data; i++) {
        data_bitmap[i] = 0;
    }

    p.data_bitmap = data_bitmap;
    logger->trace("Blockbitmap created");

    return 0;
}

int accessibility_databitmap(partition_t p, unsigned int i){
    uint32_t nb_data = p.super_bloc.nb_data;
    uint32_t* data_bitemap = p.data_bitmap;
    if (i > nb_data){
        logger->error("You are trying to read block beyond the partition.");
        return -1;
    }

    if(data_bitemap[i] != 0){
        logger->trace("This data is already use");
        return 1;
    }
    logger->trace("You can use this data");
    return 0;
}

int update_databitmap(partition_t p, unsigned int i){
    uint32_t nb_data = p.super_bloc.nb_data;
    uint32_t* data_bitemap = p.data_bitmap;

    if (i > nb_data){
        logger->error("You are trying to create a block beyond the partition.");
        return -1;
    }

    if (data_bitemap[i] == 0){
        data_bitemap[i] = 1;
        p.super_bloc.nb_data_free--;
        logger->trace("This databloc is now define as used");
        return 0;
    }
    data_bitemap[i] = 0;
    p.super_bloc.nb_data_free++;
    logger->trace("This databloc is now define as unused");
    p.data_bitmap = data_bitemap;
    return 0;
}

int delete_databitmap(partition_t p, unsigned int i){
    uint32_t nb_data = p.super_bloc.nb_data;
    uint32_t* data_bitemap = p.data_bitmap;

    if (i > nb_data){
        logger->error("You are trying to create a block beyond the partition.");
        return -1;
    }

    p.data_bitmap = NULL;
    free(p.data_bitmap);
    logger->trace("Your databitmap got deleted");
    return 0;
}