#include "log.h"
#include "usart.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#define MAX_DEBUG_LEN 64

#define ANSI_COLOUR_RED "\x1b[31m"
#define ANSI_COLOUR_YELLOW "\x1b[33m"
#define ANSI_COLOUR_CYAN "\x1b[36m"
#define ANSI_COLOUR_GREEN "\x1b[32m"
#define ANSI_COLOUR_RESET "\x1b[0m"

static struct
{
    LogLevel_t level;
    bool enable;
    bool colours;
}log;

const char * levelStrings[] =
{
    "[DEBUG]: ", "[INFO]: ", "[WARN]: ", "[ERROR]: ", "[FATAL]: "
};

const char * levelColours[] =
{
    ANSI_COLOUR_GREEN, ANSI_COLOUR_CYAN, ANSI_COLOUR_YELLOW, ANSI_COLOUR_RED, ANSI_COLOUR_RED
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

void log_SetColours(bool enable)
{
    log.colours = enable;
}

void log_Log(LogLevel_t level, const char *fmt, ...)
{
    if (level < log.level || log.enable == false)
    {
        return;
    }

    char buf[MAX_DEBUG_LEN] = {0};

    if (log.colours)
    {
        strcpy(buf, levelColours[level]);
        strcat(buf, levelStrings[level]);
        strcat(buf, ANSI_COLOUR_RESET);
    }
    else
    {
        strcpy(buf, levelStrings[level]);
    }
    
    uint8_t remainingSize = MAX_DEBUG_LEN - strlen(buf);

    va_list args;
    va_start(args, fmt);

    uint8_t debugLen = vsnprintf(&buf[strlen(buf)], remainingSize, fmt, args);

    if (debugLen > 0)
    {
        USART2_PrintBuffer((uint8_t *)buf, strlen(buf));
        const char * newLine = "\n\r";
        USART2_PrintBuffer((uint8_t *)newLine, strlen(newLine));
    }
    else
    {
        Error_Handler();
    }
    
    va_end(args);
}

// TODO: Add queue to add debugs to, so that they may be dumped out the uart when ready (or over AT protocol)
