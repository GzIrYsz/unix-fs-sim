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

/**
 * @brief Creates a directory.
 * @param p The partition to use.
 * @return 0 if everything went well, -1 otherwise.
 */
int create_directory(partition_t *p);

/**
 * @brief Reads a directory.
 * @param p The partition to use.
 * @return 0 if everything went well, -1 otherwise.
 */
int read_directory(partition_t *p);

/**
 * @brief Update a directory.
 * @param p The partition to use.
 * @return 0 if everything went well, -1 otherwise
 */
int update_directory(partition_t *p);

/**
 * @brief Delete the directory.
 * @param p The partition to use.
 * @return 0 if everything went well, -1 otherwise.
 */
int delete_directory(partition_t *p);

/**
 * @briedf Insertion of a directory entry in the directory
 * @param p The partition to use.
 * @param dir the directory to insert.
 * @return 0 if everything went well, -1 otherwise.
 */
int insertion_entry(partition_t *p, dir_entry_t dir);

/**
 * @brief Delete a specific directory entry in the directory
 * @param p The partition to use.
 * @param dir The directory to delete.
 * @return 0 if everything went well, -1 otherwise.
 */
int delete_entry(partition_t *p, dir_entry_t dir);