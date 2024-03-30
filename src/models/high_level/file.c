/**
 * @file file.c
 * @brief
 * @author Thomas REMY
 * @author Pierre FRANCK-PAPUCHON
 * @version 0.1.0
 * @date 03-28-2024
 */

#include <stdlib.h>
#include <time.h>

#include "logging/logging.h"

#include "../mid_level/inode.h"
#include "../mid_level/inode_bitmap.h"

#include "file.h"

extern logger_t *logger;

int create_file(char *name, partition_t p) {
    if (p.super_bloc.nb_inodes_free <= 0 || p.super_bloc.nb_data_free <= 0) {
        logger->warn("No more inode or data block free.");
        return -1;
    }

    uint32_t i;
    if ((i = next_free_inode(p)) == -1) {
        logger->error("An error occurred when trying to find a free inode.");
        return -1;
    }

    time_t now = time(NULL);
    inode_t inode = {
            .memory_size_data = 0,
            .last_modification = now,
            .last_access = now,
            .data_blocks = (data_t*) malloc(NB_DATA_BLOCKS_INODE * sizeof(data_t))
    };

    if (create_inode(p, i) == -1) {
        logger->error("An error occurred when trying to create an inode.");
        return -1;
    }

    // TODO: Chercher block de données, le mettre dans l'inode + créer une directory entry avec le nom du fichier et l inode

    if (update_inode(p, inode, i) == -1) {
        logger->error("An error occurred when trying to update an inode.");
        return -1;
    }

    logger->info("File created.");
    return 0;
}