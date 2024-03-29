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
 * @brief Creates a bitmap for data
 * @param p The partition where to create your databitmap
 * @return 0 if your databitmap got allocated, -1 if you got an error on allocation
 */
int create_databitmap(partition_t p);

/**
 * @brief Look the accessibility of your data
 * @param p The partition where you get your databitmap
 * @param i The data_block you want to look
 * @return 0 if the data is unused, or 1 if he used, -1 otherwise
 */
int accessibility_databitmap(partition_t p, unsigned int i);

/**
 * @brief Update your the accessibility of your data in the bitmap
 * @param p The partition where you get your databitmap
 * @param i The data_block you want to look
 * @return 0 if he could update, -1 otherwise
 */
int update_databitmap(partition_t p, unsigned int i);

/**
 * @brief Delete your databitmap
 * @param p The partition where you get your databitmap
 * @param i The data_block you want to delete
 * @return 0 if you deleted it, -1 otherwise
 */
int delete_databitmap(partition_t p, unsigned int i);