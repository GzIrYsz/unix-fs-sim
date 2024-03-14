/**
 * @file logging.h
 * @brief The public API of the logging library.
 * @author Thomas REMY
 * @version 1.0.0
 * @date 03-14-2024
 */

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <sys/syslimits.h>

/**
 * @brief A pointer representing the function use to log at a single level.
 */
typedef void (*logAction)(char *msg);

/**
 * @enum LogLevel
 * @brief This enum is about the different levels of logging.
 *
 * The 6 levels are (from high to low):
 * - TRACE
 * - DEBUG
 * - INFO
 * - WARN
 * - ERROR
 * - FATAL
 */
typedef enum {
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL
} LogLevel;

/**
 * @struct LoggerConfig logging.h
 * @brief This struct contains the config for the logger.
 * @var lineMaxLength The maximum number of characters that can be displayed on a single line.
 * @var logToStdout If the logging should be displayed on the standard output.
 * @var stdoutMinLevel The minimum logging level to display on the standard output.
 * @var coloredStdout If the outputs should be displayed with colors.
 * @var logToFile If the logs should be saved in a log file.
 * @var fileMinLevel The minimum logging level to store in the log file.
 * @var filePath[PATH_MAX] A string containing the path of the log file.
 */
typedef struct {
    size_t lineMaxLength;
    bool logToStdout;
    LogLevel stdoutMinLevel;
    bool coloredStdout;
    bool logToFile;
    LogLevel fileMinLevel;
    char filePath[PATH_MAX];
} LoggerConfig;

/**
 * @struct Logger logging.h
 * @brief This struct represents the logger itself.
 * @var config The configuration of this logger.
 * @var trace The function to log at the trace level.
 * @var debug The function to log at the debug level.
 * @var info The function to log at the info level.
 * @var warn The function to log at the warn level
 * @var error The function to log at the error level.
 * @var fatal The function to log at the fatal level.
 */
typedef struct {
    LoggerConfig config;
    logAction trace;
    logAction debug;
    logAction info;
    logAction warn;
    logAction error;
    logAction fatal;
} Logger;

/**
 * @brief Initializes the logger with the given configuration.
 * @param config The logger configuration.
 */
void initLogger(LoggerConfig config);

/**
 * @brief Returns the logger.
 * @return The logger.
 */
Logger* getLogger();