/**
 * @file exits.h
 * @brief The exit codes used by the unix_fs_sim and logging libraries.
 * @author Thomas REMY
 * @version 0.1.0
 * @date 03-14-2024
 *
 * To not overlap the codes used by UNIX systemd's codes, the range used in this file is from 8 to 63.
 */

#pragma once

/**
 * @def ERR_FOPEN The exit code when an error occurred when trying to open a file.
 */
#define ERR_FOPEN 8

/**
 * @def ERR_MALLOC The exit code when an error occurred when trying to allocate memory.
 */
#define ERR_MALLOC 9

/**
 * @def ERR_FORK The exit code when an error occurred during a fork.
 */
#define ERR_FORK 10

/**
 * @def ERR_MKPART The exit code when an error occurred when trying to create a partition.
 */
#define ERR_MKPART 11

/**
 * @def ERR_MKFS The exit code when an error occurred when trying to format a partition.
 */
#define ERR_MKFS 12

/**
 * @def ERR_MOUNT The exit code when an error occurred when trying to mount a partition.
 */
#define ERR_MOUNT 13

/**
 * @def ERR_OPEN The exit code when an error occurred when trying to open a file.
 */
#define ERR_OPEN 14

/**
 * @def ERR_READ The exit code when an error occurred when trying to read a file.
 */
#define ERR_READ 15

/**
 * @def ERR_WRITE The exit code when an error occurred when trying to write in a file.
 */
#define ERR_WRITE 16

/**
 * @def ERR_CLOSE The exit code when an error occurred when trying to close a file.
 */
#define ERR_CLOSE 17

/**
 * @def ERR_UMOUNT The exit code when an error occurred when trying to unmount a partition.
 */
#define ERR_UMOUNT 18

/**
 * @def ERR_USAGE The exit code when an error occurred when trying to view the usage of a fs.
 */
#define ERR_USAGE 19