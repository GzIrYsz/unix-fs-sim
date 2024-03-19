/**
 * @file logging.c
 * @brief The implementation of the logging library.
 * @author Thomas REMY
 * @version 1.0.1
 * @date 03-19-2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "logging/logging.h"

#include "ansiColors.h"
#include "exits.h"
#include "logging.priv.h"

extern logger_t *logger;

char* get_level_name(log_level_e level) {
    switch (level) {
        case TRACE:
            return "TRACE";
        case DEBUG:
            return "DEBUG";
        case INFO:
            return "INFO";
        case WARN:
            return "WARN";
        case ERROR:
            return "ERROR";
        case FATAL:
            return "FATAL";
    }
}

char* get_level_color(log_level_e level) {
    switch (level) {
        case TRACE:
            return BLU;
        case DEBUG:
            return MAG;
        case INFO:
            return GRN;
        case WARN:
            return YEL;
        case ERROR:
            return RED;
        case FATAL:
            return REDHB;
    }
}

void init_logger(logger_config_t config) {
    logger = malloc(sizeof(logger_t));
    logger->config = config;
    logger->trace = log_trace;
    logger->debug = log_debug;
    logger->info = log_info;
    logger->warn = log_warn;
    logger->error = log_error;
    logger->fatal = log_fatal;
}

logger_t* get_logger() {
    if (logger != NULL)
        return logger;
    else
        return NULL;
}

void my_log(log_level_e level, char *msg) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    if (!logger->config.colored_stdout) {
        char *log_data = malloc(logger->config.line_max_length * sizeof(char));
        sprintf(log_data, "%d-%d-%d %d:%d:%d [%s] %s\n",
                tm.tm_mday,
                tm.tm_mon,
                tm.tm_year + 1900,
                tm.tm_hour,
                tm.tm_min,
                tm.tm_sec,
                get_level_name(level),
                msg);

        if (logger->config.log_to_stdout && (level >= logger->config.stdout_min_level))
            fputs(log_data, stdout);

        if (logger->config.log_to_file && (level >= logger->config.file_min_level)) {
            FILE *fptr;
            fptr = fopen(logger->config.file_path, "a");

            if (fptr == NULL) {
                perror("An error occurred when trying to open a file");
                exit(ERR_FOPEN);
            }

            fputs(log_data, fptr);
            fclose(fptr);
        }
        free(log_data);
    } else {
        if (logger->config.log_to_stdout && (level >= logger->config.stdout_min_level)) {
            printf("%d-%d-%d %d:%d:%d [%s%s%s] %s\n",
                   tm.tm_mday,
                   tm.tm_mon,
                    tm.tm_year + 1900,
                   tm.tm_hour,
                   tm.tm_min,
                   tm.tm_sec,
                   get_level_color(level),
                   get_level_name(level),
                   RESET,
                   msg);
        }

        if (logger->config.log_to_file && (level >= logger->config.file_min_level)) {
            FILE *fptr;
            fptr = fopen(logger->config.file_path, "a");

            if (fptr == NULL) {
                perror("An error occurred when trying to open a file");
                exit(ERR_FOPEN);
            }

            char *logData = malloc(logger->config.line_max_length * sizeof(char));
            sprintf(logData, "%d-%d-%d %d:%d:%d [%s] %s\n",
                    tm.tm_mday,
                    tm.tm_mon,
                    tm.tm_year + 1900,
                    tm.tm_hour,
                    tm.tm_min,
                    tm.tm_sec,
                    get_level_name(level),
                    msg);

            fputs(logData, fptr);
            free(logData);
            fclose(fptr);
        }
    }
}

void log_trace(char *msg) {
    my_log(TRACE, msg);
}

void log_debug(char *msg) {
    my_log(DEBUG, msg);
}

void log_info(char *msg) {
    my_log(INFO, msg);
}

void log_warn(char *msg) {
    my_log(WARN, msg);
}

void log_error(char *msg) {
    my_log(ERROR, msg);
}

void log_fatal(char *msg) {
    my_log(FATAL, msg);
}