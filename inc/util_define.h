#ifndef UTIL_DEFINE_H
#define UTIL_DEFINE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct 
{
    uint32_t port;
    uint16_t pins; // Note, can be set to multiple pins
} gpio_reference_t;

#ifdef __cplusplus
}
#endif


#endif /* UTIL_DEFINE_H */