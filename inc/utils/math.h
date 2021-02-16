#ifndef MATH_H
#define MATH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

int32_t rescale_range(float x, float x_min, float x_max, float y_min, float y_max);

#ifdef __cplusplus
}
#endif

#endif /* MATH_H */