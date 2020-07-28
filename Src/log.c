#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "log.h"
#include "usart.h"
#include "ble.h"

#define MAX_DEBUG_LEN 64

#define ANSI_COLOUR_RED "\x1b[31m"
#define ANSI_COLOUR_YELLOW "\x1b[33m"
#define ANSI_COLOUR_CYAN "\x1b[36m"
#define ANSI_COLOUR_GREEN "\x1b[32m"
#define ANSI_COLOUR_RESET "\x1b[0m"

static struct
{
    LogLevel_t level;
    LogOutput_t output;
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

static void log_SendToOutput(char * buf, uint16_t length);

void log_Init(void)
{
    memset(&log, 0, sizeof(log));
}

void log_SetLevel(LogLevel_t level)
{
    log.level = level;
}

void log_SetOutput(LogOutput_t output)
{
    log.output = output;
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
        log_SendToOutput(buf, strlen(buf));
    }
    else
    {
        Error_Handler();
    }
    
    va_end(args);
}

static void log_SendToOutput(char * buf, uint16_t length)
{
    char * newLine = "\n\r";

    switch (log.output)
    {
        case (UART_TTL):
        {
            USART2_PrintBuffer((uint8_t *)buf, strlen(buf));
            USART2_PrintBuffer((uint8_t *)newLine, strlen(newLine));
            break;
        }
        case (BLUETOOTH):
        {
            ble_Transmit(buf, length);
            ble_Transmit(newLine, strlen(newLine));
            break;
        }
        default: { break; }
    }
}

// TODO: Add queue to add debugs to, so that they may be dumped out the uart when ready (or over AT protocol)
