/**
 * @file logging.c
 * @brief The implementation of the logging library.
 * @author Thomas REMY
 * @version 1.0.0
 * @date 03-14-2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "logging/logging.h"

#include "ansiColors.h"
#include "exits.h"
#include "logging.priv.h"

extern Logger *logger;

char* getLevelName(LogLevel level) {
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

char* getLevelColor(LogLevel level) {
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

void initLogger(LoggerConfig config) {
    logger = malloc(sizeof(Logger));
    logger->config = config;
    logger->trace = logTrace;
    logger->debug = logDebug;
    logger->info = logInfo;
    logger->warn = logWarn;
    logger->error = logError;
    logger->fatal = logFatal;
}

Logger* getLogger() {
    if (logger != NULL)
        return logger;
    else
        return NULL;
}

void myLog(LogLevel level, char *msg) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    if (!logger->config.coloredStdout) {
        char *logData = malloc(logger->config.lineMaxLength * sizeof(char));
        sprintf(logData, "%d-%d-%d %d:%d:%d [%s] %s\n",
                tm.tm_mday,
                tm.tm_mon,
                tm.tm_year + 1900,
                tm.tm_hour,
                tm.tm_min,
                tm.tm_sec,
                getLevelName(level),
                msg);

        if (logger->config.logToStdout && (level >= logger->config.stdoutMinLevel))
            fputs(logData, stdout);

        if (logger->config.logToFile && (level >= logger->config.fileMinLevel)) {
            FILE *fptr;
            fptr = fopen(logger->config.filePath, "a");

            if (fptr == NULL) {
                perror("An error occurred when trying to open a file");
                exit(ERR_FOPEN);
            }

            fputs(logData, fptr);
            fclose(fptr);
        }
        free(logData);
    } else {
        if (logger->config.logToStdout && (level >= logger->config.stdoutMinLevel)) {
            printf("%d-%d-%d %d:%d:%d [%s%s%s] %s\n",
                    tm.tm_mday,
                    tm.tm_mon,
                    tm.tm_year + 1900,
                    tm.tm_hour,
                    tm.tm_min,
                    tm.tm_sec,
                    getLevelColor(level),
                    getLevelName(level),
                    RESET,
                    msg);
        }

        if (logger->config.logToFile && (level >= logger->config.fileMinLevel)) {
            FILE *fptr;
            fptr = fopen(logger->config.filePath, "a");

            if (fptr == NULL) {
                perror("An error occurred when trying to open a file");
                exit(ERR_FOPEN);
            }

            char *logData = malloc(logger->config.lineMaxLength * sizeof(char));
            sprintf(logData, "%d-%d-%d %d:%d:%d [%s] %s\n",
                    tm.tm_mday,
                    tm.tm_mon,
                    tm.tm_year + 1900,
                    tm.tm_hour,
                    tm.tm_min,
                    tm.tm_sec,
                    getLevelName(level),
                    msg);

            fputs(logData, fptr);
            free(logData);
            fclose(fptr);
        }
    }
}

void logTrace(char *msg) {
    myLog(TRACE, msg);
}

void logDebug(char *msg) {
    myLog(DEBUG, msg);
}

void logInfo(char *msg) {
    myLog(INFO, msg);
}

void logWarn(char *msg) {
    myLog(WARN, msg);
}

void logError(char *msg) {
    myLog(ERROR, msg);
}

void logFatal(char *msg) {
    myLog(FATAL, msg);
}