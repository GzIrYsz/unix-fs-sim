/**
 * @file file.h
 * @brief
 * @author Thomas REMY
 * @author Pierre FRANCK-PAPUCHON
 * @version 0.1.0
 * @date 03-28-2024
 */

#pragma once

#include "../../ufs.priv.h"

/**
 * @brief Creates an empty file in the partition.
 * @param name The name of the file (should be unique).
 * @param p The partition.
 * @return The inode if everything went well, -1 otherwise.
 */
uint32_t create_file(char *name, partition_t p);

int read_file(char *name, partition_t p); // ?

int write_file(char *name, partition_t p); // ?

int delete_file(char *name, partition_t p);