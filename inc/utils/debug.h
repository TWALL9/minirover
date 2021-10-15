#ifndef DEBUG_H
#define DEBUG_H

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include "log.h"
#include "peripherals/usart.h"

class DebugBase 
{
    public:
        virtual void log(LogLevel_t level, const char * fmt, ...) = 0;
        void set_level(LogLevel_t new_level);
        LogLevel_t get_level(void);
        void set_handle(const char * handle);
        virtual ~DebugBase(void) {};
    protected:
        uint8_t format_log(LogLevel_t level, char * buf, const char * fmt, ...);
        LogLevel_t _level = LOG_LEVEL_DEBUG;
        char _handle[8] = "";
};

class SerialDebug : DebugBase 
{
    public:
        SerialDebug(uint32_t usart_base, uint32_t baud);
    private:
        uint32_t _usart_base = USART2;

};

#endif /* DEBUG_H */
