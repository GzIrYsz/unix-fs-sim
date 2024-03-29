/**
 * @file block.h
 * @brief This file contains the CRUD operations available on a block.
 * @author Thomas REMY
 * @version 1.0.0
 * @date 03-28-2024
 */

#pragma once

#include "unix_fs_sim/ufs.h"

#include "../../ufs.priv.h"

/**
 * @brief Creates a block at the specified location.
 * @param p The partition where to create the block.
 * @param i The index of the block you want to create.
 * @return 0 if everything went well, -1 otherwise.
 */
int create_block(partition_t p, uint32_t i);

/**
 * @brief Reads the data located at the specified block.
 * @param p The partition where to read the data.
 * @param buf The buffer where to store the data.
 * @param i The index of the block to read.
 * @return 0 if everything went well, -1 otherwise.
 */
int read_block(partition_t p, void *buf, uint32_t i);

/**
 * @brief Writes new data on the bloc located at the specified index.
 * @param p The partition where to write the data.
 * @param buf The buffer where the data is stored (should be block_size wide).
 * @param data_length The length of the data.
 * @param i The index of the block to update.
 * @param offset The were to write the data in the specific bloc.
 * @return 0 if everything went well, -1 otherwise.
 */
int update_bloc(partition_t p, void *buf, uint32_t data_length, uint32_t i, uint32_t offset);

/**
 * @brief Writes new data on the bloc located at the specified index.
 * @param p The partition where to write the data.
 * @param buf The buffer where the data is stored (should be block_size wide).
 * @param i The index of the block to update.
 * @return 0 if everything went well, -1 otherwise.
 */
int write_bloc(partition_t p, const void *buf, uint32_t i);

/**
 * @brief Deletes the bloc located at the specified index.
 * @param p The partition where to delete the block.
 * @param i The index of the block to delete.
 * @return 0 if everything went well, -1 otherwise.
 */
int delete_block(partition_t p, uint32_t i);