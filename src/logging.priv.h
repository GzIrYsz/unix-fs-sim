/**
 * @file logging.priv.h
 * @brief This file contains the private declarations used for the logging library.
 * @author Thomas REMY
 * @version 1.0.0
 * @date 03-14-2024
 */

#pragma once

#include "logging/logging.h"

/**
 * @brief Returns a string representation of a logging level.
 * @param level The logging level.
 * @return The string representation of the given logging level.
 */
char* getLevelName(LogLevel level);

/**
 * @brief Returns a color corresponding to a given logging level.
 * @param level The logging level.
 * @return The color representation of the logging level.
 *
 * The colors are :
 * - TRACE -> BLUE
 * - DEBUG -> MAGENTA
 * - INFO -> GREEN
 * - WARN -> YELLOW
 * - ERROR -> RED
 * - FATAL -> WHITE (on a RED background)
 */
char* getLevelColor(LogLevel level);

/**
 * @brief Logs a message at a given level.
 * @param level The level of the log message.
 * @param msg The actual message to log.
 *
 * This function logs to the standard output and/or to a file depending on the configuration.
 */
void myLog(LogLevel level, char *msg);

/**
 * @brief Logs a message on the trace level.
 * @param msg The message to log.
 * @see myLog
 */
void logTrace(char *msg);

/**
 * @brief Logs a message on the debug level.
 * @param msg The message to log.
 * @see myLog
 */
void logDebug(char *msg);

/**
 * @brief Logs a message on the info level.
 * @param msg The message to log.
 * @see myLog
 */
void logInfo(char *msg);

/**
 * @brief Logs a message on the warn level.
 * @param msg The message to log.
 * @see myLog
 */
void logWarn(char *msg);

/**
 * @brief Logs a message on the error level.
 * @param msg The message to log.
 * @see myLog
 */
void logError(char *msg);

/**
 * @brief Logs a message on the fatal level.
 * @param msg The message to log.
 * @see myLog
 */
void logFatal(char *msg);