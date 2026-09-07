#include "vector2d.h"

void frect_to_rect(const frect_t *frect, rect_t *rect)
{
    rect->x = (int)frect->x;
    rect->y = (int)frect->y;
    rect->w = (int)frect->w;
    rect->h = (int)frect->h;
}

inline float lerp(float a, float b, float s)
{
    return a * (1 - s) + b * s;
}

inline float lerp_time(float a, float b, float s, float t)
{
    return lerp(a, b, 1 - powf(1 - s, t * 60.0f));
}