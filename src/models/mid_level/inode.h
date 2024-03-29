/**
 * @file inode.h
 * @brief This file contains the CRUD operations available on an inode.
 * @author Thomas REMY
 * @version 1.0.0
 * @date 03-29-2024
 */

#pragma once

#include "unix_fs_sim/ufs.h"

#include "../../ufs.priv.h"

/**
 * @brief Creates an inode at the specified location.
 * @param p The partition to use.
 * @param i The index of the inode you want to create.
 * @return 0 if everything went well, -1 otherwise.
 */
int create_inode(partition_t p, uint32_t i);

/**
 * @brief Reads an inode located at the specified location.
 * @param p The partition to use.
 * @param inode The inode variable where to store the data.
 * @param i The index of inode to read.
 * @return 0 if everything went well, -1 otherwise.
 */
int read_inode(partition_t p, inode_t *inode, uint32_t i);

/**
 * @brief Writes or updates the inode located at the specified index.
 * @param p The partition to use.
 * @param inode The inode to store on the disk.
 * @param i The index of the inode to update.
 * @return 0 if everything went well, -1 otherwise.
 */
int update_inode(partition_t p, inode_t inode, uint32_t i);

/**
 * @brief Deletes the inode located at the specified index.
 * @param p The partition to use.
 * @param i The index of the inode to delete.
 * @return 0 if everything went well, -1 otherwise.
 */
int delete_inode(partition_t p, uint32_t i);