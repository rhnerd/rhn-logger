// rhn_logger.h
#ifndef RHN_LOGGER_H
#define RHN_LOGGER_H

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

// --- Log levels ---
#define LOG_LEVEL_DEBUG 0
#define LOG_LEVEL_INFO  1
#define LOG_LEVEL_WARN  2
#define LOG_LEVEL_ERROR 3
#define LOG_LEVEL_NONE  4

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_DEBUG
#endif

// --- ANSI colors for stderr ---
#define COLOR_RESET   "\x1b[0m"
#define COLOR_RED     "\x1b[31m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_BLUE    "\x1b[34m"

// --- Global file pointer (NULL = no file logging) ---
static FILE *log_file = NULL;

// --- Enable logging to a file ---
// mode = "w" (write - overwrite), "a" (append)
static inline void LOG_FILE(const char *filename, const char *mode) {
    if (filename && *filename) {
        if (!mode || !*mode) mode = "w"; // default = overwrite
        log_file = fopen(filename, mode);
        if (!log_file) {
            fprintf(stderr,
                COLOR_YELLOW "[WARN ] Could not open log file '%s', logging only to stderr%s\n",
                filename, COLOR_RESET);
        }
    } else {
        log_file = NULL; // no file, stderr only
    }
}

// --- Close log file ---
static inline void LOG_FILE_CLOSE(void) {
    if (log_file) {
        fclose(log_file);
        log_file = NULL;
    }
}

#if LOG_LEVEL == LOG_LEVEL_NONE

#define LOG(level, fmt, ...) do { } while (0)

#else

// --- Main LOG macro ---
#define LOG(level, fmt, ...) do { \
    if ((level) >= LOG_LEVEL) { \
        const char *color, *name; \
        switch (level) { \
            case LOG_LEVEL_DEBUG: color = COLOR_BLUE;   name = "DEBUG"; break; \
            case LOG_LEVEL_INFO:  color = COLOR_GREEN;  name = "INFO "; break; \
            case LOG_LEVEL_WARN:  color = COLOR_YELLOW; name = "WARN "; break; \
            case LOG_LEVEL_ERROR: color = COLOR_RED;    name = "ERROR"; break; \
            default:              color = "";           name = "LOG  "; break; \
        } \
        time_t now = time(NULL); \
        char buf[20]; \
        strftime(buf, sizeof buf, "%H:%M:%S", localtime(&now)); \
        /* stderr (colored) */ \
        fprintf(stderr, "%s[%s] %-5s " fmt "%s\n", \
                color, buf, name, ##__VA_ARGS__, COLOR_RESET); \
        /* file (plain) */ \
        if (log_file) { \
            fprintf(log_file, "[%s] %-5s " fmt "\n", \
                    buf, name, ##__VA_ARGS__); \
            fflush(log_file); \
        } \
    } \
} while (0)

#endif // LOG_LEVEL_NONE

// --- Aliases ---
#define LOG_DEBUG(fmt, ...) LOG(LOG_LEVEL_DEBUG, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)  LOG(LOG_LEVEL_INFO,  fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...)  LOG(LOG_LEVEL_WARN,  fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) LOG(LOG_LEVEL_ERROR, fmt, ##__VA_ARGS__)

#endif // LOG_H
