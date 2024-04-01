/**
 * @file ufs.priv.h
 * @brief This file contains the private declarations used for the UNIX File System Simulation.
 * @author Thomas REMY
 * @version 0.1.0
 * @date 03-24-2024
 */

#pragma once

#include <stdint.h>

#include "unix_fs_sim/ufs.h"

#define MAX_OPENED_FILES 64

typedef struct {
    int fd;
    super_bloc_t super_bloc;
    uint8_t *data_bitmap;
    uint8_t *inode_bitmap;
    file_t *opened_files[MAX_OPENED_FILES];
    uint16_t nb_opened_files;
    dir_entry_t* directory;
} partition_t;

/**
 * @brief Initialize a partition_t variable.
 * @param fd The file descriptor of the opened partition.
 * @param super_bloc The super block of the opened partition.
 * @return A partition struct representing the partition.
 */
partition_t init_partition(int fd, super_bloc_t super_bloc);