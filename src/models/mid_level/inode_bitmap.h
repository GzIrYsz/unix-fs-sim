/**
 * @file inode_bitmap.h
 * @brief This file contains the CRUD operations available on an inode bitmap.
 * @author Thomas REMY
 * @version 1.0.0
 * @date 03-29-2024
 */

#pragma once

#include <stdint.h>

#include "../../ufs.priv.h"

/**
 * @brief Creates a new inode bitmap on disk.
 * @param p The partition where to create de bitmap.
 * @return 0 if everything went well, -1 otherwise.
 */
int create_inodebitmap(partition_t p);

/**
 * @brief Reads the inode bitmap and store it in memory.
 * @param p The partition.
 * @return 0 if everything went well, -1 otherwise.
 */
int read_inodebitmap(partition_t p);

/**
 * @brief Writes the inode bitmap on the disk.
 * @param p The partition.
 * @return 0 if everything went well, -1 otherwise.
 */
int update_inodebitmap(partition_t p);

/**
 * @brief Deletes the inode bitmap stored on disk.
 * @param p The partition.
 * @return 0 if everything went well, -1 otherwise.
 */
int delete_inodebitmap(partition_t p);

/**
 * @brief Finds the next free inode and returns its index.
 * @param p The partition.
 * @return The index of the next free inode or -1 if an error occurs or there is no more free inode.
 */
uint32_t next_free_inode(partition_t p);