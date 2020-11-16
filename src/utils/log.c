#include "log.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <libopencm3/stm32/usart.h>

#define MAX_DEBUG_LEN 64

static struct
{
    uint32_t periph;
    LogLevel_t level;
    bool enable;
}log_context;

const char * level_strings[] =
{
    "[DEBUG]: ", "[INFO]: ", "[WARN]: ", "[ERROR]: ", "[FATAL]: "
};

void log_init(uint32_t usart_periph)
{
    memset(&log_context, 0, sizeof(log_context));
    log_context.periph = usart_periph;
}

void log_set_level(LogLevel_t level)
{
    log_context.level = level;
}

void log_set_enable(bool enable)
{
    log_context.enable = enable;
}

void log_log(LogLevel_t level, const char *fmt, ...)
{
    if (level < log_context.level || log_context.enable == false)
    {
        return;
    }

    char buf[MAX_DEBUG_LEN] = {0};

    strcpy(buf, level_strings[level]);
    
    uint8_t remaining_size = MAX_DEBUG_LEN - strlen(buf);

    va_list args;
    va_start(args, fmt);

    uint8_t len = vsnprintf(&buf[strlen(buf)], remaining_size, fmt, args);

    if (len > 0)
    {
        for (uint8_t i = 0; i < len; i ++)
        {
            usart_send(log_context.periph, buf[i]);
        }
        usart_send(log_context.periph, '\r');
        usart_send(log_context.periph, '\n');
            
    }

    va_end(args);
}

// TODO: Add queue to add debugs to, so that they may be dumped out the uart when ready (or over AT protocol)
