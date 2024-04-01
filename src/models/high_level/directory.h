/**
 * @file directory.h
 * @brief
 * @author Thomas REMY
 * @author Pierre FRANCK-PAPUCHON
 * @version 0.1.0
 * @date 03-28-2024
 */

#include "../low_level/block.h"
#pragma once


int create_directory(partition_t p);

int read_directory(partition_t p);

int update_directory(partition_t p);

int delete_directory(partition_t p);

int insertion_entry(partition_t p, dir_entry_t dir);

int delete_entry(partition_t p, dir_entry_t dir);