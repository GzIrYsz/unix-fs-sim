/**
 * @file block_bitmap.h
 * @brief
 * @author Thomas REMY
 * @author Pierre FRANCK-PAPUCHON
 * @version 0.1.0
 * @date 03-28-2024
 */

#pragma once

#include "unix_fs_sim/ufs.h"

#include "../../ufs.priv.h"

/**
 * @brief Creates a new data bitmap on disk.
 * @param p The partition where to create your databitmap.
 * @return 0 if everything went well, -1 otherwise.
 */
int create_databitmap(partition_t *p);

/**
 * @brief Reads the data bitmap and store it in memory.
 * @param p The partition.
 * @return 0 if everything went well, -1 otherwise.
 */
int read_databitmap(partition_t *p);

/**
 * @brief Writes the data bitmap on the disk.
 * @param p The partition.
 * @return 0 if everything went well, -1 otherwise.
 */
int update_databitmap(partition_t *p);

/**
 * @brief Delete the inode bitmap stored on disk.
 * @param p The partition.
 * @return 0 if everything went well, -1 otherwise.
 */
int delete_databitmap(partition_t *p);

/**
 * @brief Finds the next free data and returns its index.
 * @param p The partition.
 * @return The index of the next free data or -1 if an error occurs or there is no more free data.
 */
uint32_t next_free_data(partition_t *p);
