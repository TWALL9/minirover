#include "string.h"
#include "utils/debug.h"

void DebugBase::set_level(LogLevel_t new_level) 
{
    _level = new_level;
}

LogLevel_t DebugBase::get_level(void)
{
    return _level;
}

void DebugBase::set_handle(const char * handle)
{
    if (strlen(handle) <= 8)
    {
        strcpy(_handle, handle);
    }
}

uint8_t DebugBase::format_log(LogLevel_t level, char * buf, const char * fmt, ...) 
{
    static const char * level_strings[] = 
    {
        "[DEBUG]: ", "[INFO]: ", "[WARN]: ", "[ERROR]: "
    };

    uint8_t len = 0;

    if (this->_level >= level && _level != LOG_LEVEL_OFF)
    {
        va_list args;
        va_start(args, fmt);
        strcpy(buf, _handle);
        strcpy(buf, level_strings[level]);
        
        uint8_t remaining_size = MAX_DEBUG_LEN - strlen(buf);
        len = strlen(buf) + vsnprintf(&buf[strlen(buf)], remaining_size, fmt, args);
        len = len > MAX_DEBUG_LEN ? MAX_DEBUG_LEN : len;

        va_end(args);
    }

    return len;
}

SerialDebug::SerialDebug(uint32_t usart_base, uint32_t baud)
{
    _usart_base = usart_base;
    usart_setup(usart_base, baud, 8, USART_STOPBITS_1, USART_MODE_TX, USART_PARITY_NONE, USART_FLOWCONTROL_NONE);
}
