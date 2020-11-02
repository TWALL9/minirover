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
    LOG_DEBUG = 0,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_FATAL
} LogLevel_t;

void log_Init(uint32_t usart_periph);
void log_SetEnable(bool enable);
void log_SetLevel(LogLevel_t level);

#define DEBUG(...) log_Log(LOG_DEBUG, __VA_ARGS__)
#define INFO(...)  log_Log(LOG_INFO, __VA_ARGS__)
#define WARN(...)  log_Log(LOG_WARN, __VA_ARGS__)
#define ERROR(...) log_Log(LOG_ERROR, __VA_ARGS__)
#define FATAL(...) log_Log(LOG_FATAL, __VA_ARGS__)

void log_Log(LogLevel_t level, const char * fmt, ...);

#ifdef __cplusplus
}
#endif

#endif /* LOG_H */
