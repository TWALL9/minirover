#ifdef __cplusplus
extern "C" {
#endif

#include "utils/math.h"

int32_t rescale_range(float x, float x_min, float x_max, float y_min, float y_max)
{
    float percentage = ((x - x_min))/(x_max - x_min);
    return (int32_t) (percentage * (y_max - y_min) + y_min);
}

#ifdef __cplusplus
}
#endif