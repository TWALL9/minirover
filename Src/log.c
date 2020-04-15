#include "log.h"
#include "usart.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#define MAX_DEBUG_LEN 64

static struct
{
    LogLevel_t level;
    bool enable;
}log;

const char * levelStrings[] =
{
    "[DEBUG]: ", "[INFO]: ", "[WARN]: ", "[ERROR]: ", "[FATAL]: "
};

void log_Init(void)
{
    memset(&log, 0, sizeof(log));
}

void log_SetLevel(LogLevel_t level)
{
    log.level = level;
}

void log_SetEnable(bool enable)
{
    log.enable = enable;
}

void log_Log(LogLevel_t level, const char *fmt, ...)
{
    if (level < log.level || log.enable == false)
    {
        return;
    }

    char buf[MAX_DEBUG_LEN] = {0};
    strcpy(buf, levelStrings[level]);
    va_list args;
    va_start(args, fmt);

    uint8_t debugLen = vsnprintf(buf, MAX_DEBUG_LEN, fmt, args);
    debugLen > 64? debugLen = 64 : debugLen;

    if (debugLen > 0)
    {
        USART2_PrintBuffer((uint8_t *)buf, strlen(buf));
    }
    else
    {
        Error_Handler();
    }
    
    va_end(args);
}

// TODO: Add queue to add debugs to, so that they may be dumped out the uart when ready (or over AT protocol)