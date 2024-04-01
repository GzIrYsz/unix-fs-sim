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