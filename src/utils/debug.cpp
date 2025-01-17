#include "string.h"
#include "utils/debug.h"

#define MAX_DEBUG_LEN 64

static const char * level_strings[] = 
{
    "[DEBUG]: ", "[INFO]: ", "[WARN]: ", "[ERROR]: "
};

void DebugBase::set_level(LogLevel_t new_level) 
{
    this->_level = new_level;
}

LogLevel_t DebugBase::get_level(void)
{
    return this->_level;
}

void DebugBase::set_handle(const char * handle)
{
    if (strlen(handle) <= HANDLE_LEN)
    {
        strcpy(this->_handle, handle);
    }
}

uint8_t DebugBase::format_log(LogLevel_t level, char * buf, const char * fmt, ...) 
{
    uint8_t len = 0;

    if (this->_level >= level && this->_level != LOG_LEVEL_OFF)
    {
        va_list args;
        va_start(args, fmt);
        strcpy(buf, this->_handle);
        strcpy(buf, level_strings[level]);
        
        uint8_t remaining_size = MAX_DEBUG_LEN - strlen(buf);
        len = strlen(buf) + vsnprintf(&buf[strlen(buf)], remaining_size, fmt, args);
        len = len > MAX_DEBUG_LEN ? MAX_DEBUG_LEN : len;

        va_end(args);
    }

    return len;
}

SerialDebug::SerialDebug(uint32_t usart_base, uint32_t baud)
:
    _usart_base(usart_base)
{
    usart_setup(usart_base, baud, 8, USART_STOPBITS_1, USART_MODE_TX, USART_PARITY_NONE, USART_FLOWCONTROL_NONE);
}

void SerialDebug::debug(LogLevel_t level, const char * fmt, ...)
{
    char buf[MAX_DEBUG_LEN] = {'0'};
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

        if (is_usart_base(this->_usart_base))
        {
            usart_send_buf(this->_usart_base, buf, len);
        }
    }
}

BtDebug::BtDebug(HC06 bt)
:
    _bt(bt)
{

}

void BtDebug::debug(LogLevel_t level, const char * fmt, ...)
{
    char buf[MAX_DEBUG_LEN] = {'0'};
    uint8_t len = 0;

    if (this->_level >= level && this->_level != LOG_LEVEL_OFF)
    {
        va_list args;
        va_start(args, fmt);
        strcpy(buf, this->_handle);
        strcpy(buf, level_strings[level]);
        
        uint8_t remaining_size = MAX_DEBUG_LEN - strlen(buf);
        len = strlen(buf) + vsnprintf(&buf[strlen(buf)], remaining_size, fmt, args);
        len = len > MAX_DEBUG_LEN ? MAX_DEBUG_LEN : len;

        va_end(args);

        this->_bt.write_blocking(buf, len);
    }
}
