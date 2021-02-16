#include "utils/log.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "peripherals/usart.h"

#define MAX_DEBUG_LEN 64

static struct
{
    LogLevel_t level;
}log_context;

const char * level_strings[] =
{
    "[DEBUG]: ", "[INFO]: ", "[WARN]: ", "[ERROR]: "
};

void log_init(void)
{
    memset(&log_context, 0, sizeof(log_context));
    log_set_level(LOG_LEVEL_DEBUG);
}

void log_set_level(LogLevel_t level)
{
    log_context.level = level;
}

void log_log(LogLevel_t level, const char *fmt, ...)
{
    char buf[MAX_DEBUG_LEN] = {'0'};

    if (level >= log_context.level && log_context.level != LOG_LEVEL_OFF)
    {
        va_list args;
        va_start(args, fmt);

        strcpy(buf, level_strings[level]);
        
        uint8_t remaining_size = MAX_DEBUG_LEN - strlen(buf);
        uint8_t len = strlen(buf) + vsnprintf(&buf[strlen(buf)], remaining_size, fmt, args);
        len = len > MAX_DEBUG_LEN ? MAX_DEBUG_LEN : len;

        if (len > 0)
        {
            usart_send_buf(buf, len);
            usart_send_buf("\r\n", 2);
        }

        va_end(args);
    }
}

// TODO: Add queue to add debugs to, so that they may be dumped out the uart when ready (or over AT protocol)
