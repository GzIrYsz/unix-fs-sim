/**
 * @file inode.c
 * @brief
 * @author Thomas REMY
 * @author Pierre FRANCK-PAPUCHON
 * @version 0.1.0
 * @date 03-28-2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <string.h>

#include "logging/logging.h"
#include "data_bitmap.h"
#include "../low_level/block.h"
#include "inode.h"
extern logger_t* logger;

off_t get_offset_inode(partition_t p, uint32_t i){
    uint32_t nb_data = p.super_bloc.nb_data;
    uint32_t block_size = p.super_bloc.block_size;
    uint32_t nb_inodes = p.super_bloc.nb_inodes;
    return (int) (ceil(nb_data/block_size)+ceil(nb_inodes/block_size)+1)*block_size + i * sizeof(inode_t);
}

int create_inode(partition_t p, uint32_t i){
    if (i > p.super_bloc.nb_blocks) {
        logger->error("You are trying to create an inode beyond the memory for all inode.");
        return -1;
    }

    p.inode_bitmap[i] = 1;
    logger->trace("Inode created");
    return 0;
}

int read_inode(partition_t p, inode_t* inode, uint32_t i){
    if (p.inode_bitmap[i] == 0){
        logger->warn("Your inode is not open !");
        return -1;
    }

    off_t inode_pos = get_offset_inode(p, i);
    if (lseek(p.fd, inode_pos, SEEK_SET) == -1){
        logger->error("An error occurred when trying to move the head.");
        return -1;
    }

    if (read(p.fd, &inode->memory_size_data, 4) == -1){
        logger->error("An error occurred when trying to read the inode size data.");
        return -1;
    }
    if(read(p.fd, &inode->last_modification, 4) == -1){
        logger->error("An error occurred when trying to read the inode last modification");
        return -1;
    }
    if(read(p.fd, &inode->last_access, 4) == -1){
        logger->error("An error occurred when trying to read the inode last access");
        return -1;
    }
    for (int j = 0; j < 8; j++) {
        if(read(p.fd, inode->data_blocks+i, 8) == -1){
            logger->error("An error occurred when trying to read one of your inode data block");
            return -1;
        }
    }

    logger->trace("Inode read");
    return 0;
}

int update_inode(partition_t p, inode_t inode, uint32_t i){
    if (p.inode_bitmap[i] == 0){
        logger->warn("Your inode is not open");
        return -1;
    }

    off_t inode_pos = get_offset_inode(p, i);

    if(write(p.fd, &inode.memory_size_data, 8) == -1){
        logger->error("An error occurred when trying to update the size of inode data");
        return -1;
    }
    if(write(p.fd, &inode.last_modification, 8) == -1){
        logger->error("An error occurred when trying to update the size of inode last modification");
        return -1;
    }
    if(write(p.fd, &inode.last_access, 8) == -1){
        logger->error("An error occurred when trying to update the size of inode last access");
        return -1;
    }
    for (int j = 0; j < 8; j++) {
        if(write(p.fd, &inode.data_blocks+i, 8) == -1){
            logger->error("An error occurred when trying to read one of your inode data block");
            return -1;
        }
    }

    logger->trace("Inode update");
    return 0;
}

int delete_inode(partition_t p, uint32_t i){
    if (i > p.super_bloc.nb_blocks){
        logger->error("You are trying to create an inode beyond the memory for all inode.");
        return -1;
    }

    p.inode_bitmap[i] = 0;
    logger->trace("Inode deleted");
    return 0;
}
