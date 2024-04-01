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

#include "../high_level/directory.h"
#include "../mid_level/inode.h"
#include "../mid_level/data_bitmap.h"
#include "../mid_level/inode_bitmap.h"

#include "file.h"

extern logger_t *logger;

uint32_t create_file(char *name, partition_t p) {
    if (p.super_bloc.nb_inodes_free <= 0 || p.super_bloc.nb_data_free <= 0) {
        logger->warn("No more inode or data block free.");
        return -1;
    }

    uint32_t i;
    if ((i = next_free_inode(p)) == (p.super_bloc.nb_inodes + 1)) {
        logger->error("An error occurred when trying to find a free inode.");
        return -1;
    }

    time_t now = time(NULL);
    inode_t inode = {
            .memory_size_data = 0,
            .last_modification = now,
            .last_access = now
    };

    if (create_inode(p, i) == -1) {
        logger->error("An error occurred when trying to create an inode.");
        return -1;
    }

    if ((inode.data_blocks[0] = next_free_data(p)) == 0) {
        logger->error("An error occurred when trying to find a free data block.");
        return -1;
    }

    if (update_inode(p, inode, i) == -1) {
        logger->error("An error occurred when trying to update an inode.");
        return -1;
    }

    dir_entry_t dir_entry;
    dir_entry.name = name;
    dir_entry.inode = i;
    if (insertion_entry(p, dir_entry) == -1) {
        logger->error("An error occurred when trying to create a directory entry for the file.");
        return -1;
    }
    if (update_directory(p) == -1) {
        logger->error("An error occurred when trying to update the directory.");
        return -1;
    }

    logger->info("File created.");
    return i;
}