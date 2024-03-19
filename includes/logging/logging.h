/**
 * @file logging.h
 * @brief The public API of the logging library.
 * @author Thomas REMY
 * @version 1.0.1
 * @date 03-19-2024
 */

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <sys/syslimits.h>

/**
 * @brief A pointer representing the function use to log at a single level.
 */
typedef void (*log_action_t)(char *msg);

/**
 * @enum log_level_e
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
} log_level_e;

/**
 * @struct logger_config_t logging.h
 * @brief This struct contains the config for the logger.
 * @var line_max_length The maximum number of characters that can be displayed on a single line.
 * @var log_to_stdout If the logging should be displayed on the standard output.
 * @var stdout_min_level The minimum logging level to display on the standard output.
 * @var colored_stdout If the outputs should be displayed with colors.
 * @var log_to_file If the logs should be saved in a log file.
 * @var file_min_level The minimum logging level to store in the log file.
 * @var file_path[PATH_MAX] A string containing the path of the log file.
 */
typedef struct {
    size_t line_max_length;
    bool log_to_stdout;
    log_level_e stdout_min_level;
    bool colored_stdout;
    bool log_to_file;
    log_level_e file_min_level;
    char file_path[PATH_MAX];
} logger_config_t;

/**
 * @struct logger_t logging.h
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
    logger_config_t config;
    log_action_t trace;
    log_action_t debug;
    log_action_t info;
    log_action_t warn;
    log_action_t error;
    log_action_t fatal;
} logger_t;

/**
 * @brief Initializes the logger with the given configuration.
 * @param config The logger configuration.
 */
void init_logger(logger_config_t config);

/**
 * @brief Returns the logger.
 * @return The logger.
 */
logger_t* get_logger();