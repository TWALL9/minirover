#ifndef DEBUG_H
#define DEBUG_H

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include "peripherals/usart.h"
#include "drivers/bluetooth.h"

typedef enum 
{
    LOG_LEVEL_DEBUG = 0,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_OFF
} LogLevel_t;

#define HANDLE_LEN 8

class DebugBase 
{
    public:
        virtual void debug(LogLevel_t level, const char * fmt, ...) = 0;
        void set_level(LogLevel_t new_level);
        LogLevel_t get_level(void);
        void set_handle(const char * handle);
        virtual ~DebugBase(void) {};
    protected:
        uint8_t format_log(LogLevel_t level, char * buf, const char * fmt, ...);
        LogLevel_t _level = LOG_LEVEL_DEBUG;
        char _handle[HANDLE_LEN] = "";
};

// TODO: Add mutex for shared peripheral!
class SerialDebug : public DebugBase 
{
    public:
        SerialDebug(uint32_t usart_base, uint32_t baud);
        void debug(LogLevel_t level, const char * fmt, ...);
    private:
        uint32_t _usart_base = USART2;

};

class BtDebug : public DebugBase
{
    public:
        BtDebug(HC06 bt);
        void debug(LogLevel_t level, const char * fmt, ...);
    private:
        HC06 _bt;
};

#endif /* DEBUG_H */
