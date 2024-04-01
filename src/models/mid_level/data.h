/**
 * @file data.h
 * @brief This file contains the CRUD operations available on data.
 * @author Thomas REMY
 * @version 1.0.0
 * @date 03-31-2024
 */

#pragma once

#include <unistd.h>

#include "unix_fs_sim/ufs.h"

#include "../../ufs.priv.h"

off_t get_data_offset(partition_t p, uint32_t i);

/**
 * @brief Creates data at the specified location.
 * @param p The partition to use.
 * @param i The index where you want to create the data.
 * @return 0 if everything went well, -1 otherwise.
 */
int create_data(partition_t p, uint32_t i);

/**
 * @brief Reads data located at the specified index.
 * @param p The partition to use.
 * @param data Where to store the data.
 * @param i The index of the data.
 * @return 0 if everything went well, -1 otherwise.
 */
int read_data(partition_t p, uint8_t *data, uint32_t i);

/**
 * @brief Writes or updates the data located at the specified index.
 * @param p The partition to use.
 * @param data The data to store.
 * @param i The index where to store the data.
 * @return 0 if everything went well, -1 otherwise.
 */
int update_data(partition_t p, const uint8_t *data, uint32_t i);

/**
 * @brief Deletes the data located at the specified index.
 * @param p The partition to use.
 * @param i The index of the data to delete.
 * @return 0 if everything went well, -1 otherwise.
 */
int delete_data(partition_t p, uint32_t i);