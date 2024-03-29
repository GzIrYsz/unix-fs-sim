/**
 * @file ufs.priv.h
 * @brief This file contains the private declarations used for the UNIX File System Simulation.
 * @author Thomas REMY
 * @version 0.1.0
 * @date 03-24-2024
 */

#pragma once

#include "unix_fs_sim/ufs.h"

#define MAX_OPENED_FILES 64

/**
 * @struct opened_file_t ufs.priv.h
 * @brief Represents an opened file.
 * @var inode The inode of the file.
 * @var offset The position of the read/write head.
 */
typedef struct {
    uint32_t inode;
    uint32_t offset;
} opened_file_t;

typedef struct {
    int fd;
    super_bloc_t super_bloc;
    uint32_t *block_bitmap;
    uint32_t *inode_bitmap;
    opened_file_t opened_files[MAX_OPENED_FILES];
    uint16_t nb_opened_files;
} partition_t;

/**
 * @brief Initialize a partition_t variable.
 * @param fd The file descriptor of the opened partition.
 * @param super_bloc The super block of the opened partition.
 * @return A partition struct representing the partition.
 */
partition_t init_partition(int fd, super_bloc_t super_bloc);

int write_block();
int write_super_block();
int write_
int write_inode_table();
int write_inode();