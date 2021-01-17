#ifndef LOG_H
#define LOG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

typedef enum 
{
    LOG_LEVEL_DEBUG = 0,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_OFF
} LogLevel_t;

void log_init(void);
void log_set_level(LogLevel_t level);

#define DEBUG(...) log(LOG_LEVEL_DEBUG, __VA_ARGS__)
#define INFO(...)  log(LOG_LEVEL_INFO, __VA_ARGS__)
#define WARN(...)  log(LOG_LEVEL_WARN, __VA_ARGS__)
#define ERROR(...) log(LOG_LEVEL_ERROR, __VA_ARGS__)

void log(LogLevel_t level, const char * fmt, ...);

#ifdef __cplusplus
}
#endif

#endif /* LOG_H */
