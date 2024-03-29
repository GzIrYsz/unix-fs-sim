/**
 * @file ufs.priv.h
 * @brief This file contains the private declarations used for the UNIX File System Simulation.
 * @author Thomas REMY
 * @version 0.1.0
 * @date 03-24-2024
 */

#pragma once

#include "unix_fs_sim/ufs.h"

typedef struct {
    int fd;
    super_bloc_t super_bloc;
    uint32_t *block_bitmap;
    uint32_t *inode_bitmap;
} partition_t;

int write_block();
int write_super_block();
int write_
int write_inode_table();
int write_inode();