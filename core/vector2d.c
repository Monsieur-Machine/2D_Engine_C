#include "vector2d.h"

__attribute__((const)) float fabsf(float x)
{
    union {
        float f;
        uint32_t i;
    } u = {x};

    u.i &= 0x7FFFFFFF;
    return u.f;
}

bool has_intersection(const rect_t * A, const rect_t * B)
{
    float Amin, Amax, Bmin, Bmax;

    if (!A || !B)
    {
        return false;
    }

    /* Special cases for empty rects */
    if (rect_empty(A) || rect_empty(B))
    {
        return false;
    }

    /* Horizontal intersection */
    Amin = A->x;
    Amax = Amin + A->w;
    Bmin = B->x;
    Bmax = Bmin + B->w;
    if (Bmin > Amin)
        Amin = Bmin;
    if (Bmax < Amax)
        Amax = Bmax;
    if (Amax <= Amin)
        return false;

    /* Vertical intersection */
    Amin = A->y;
    Amax = Amin + A->h;
    Bmin = B->y;
    Bmax = Bmin + B->h;
    if (Bmin > Amin)
        Amin = Bmin;
    if (Bmax < Amax)
        Amax = Bmax;
    if (Amax <= Amin)
        return false;

    return true;
}

bool intersect_rect(const rect_t * A, const rect_t * B, rect_t * result)
{
    if (!A || !B || !result)
    {
        // TODO error message
        return false;
    }

    /* Special cases for empty rects */
    if (rect_empty(A) || rect_empty(B))
    {
        return false;
    }

    float Amin, Amax, Bmin, Bmax;

    /* Horizontal intersection */
    Amin = A->x;
    Amax = Amin + A->w;
    Bmin = B->x;
    Bmax = Bmin + B->w;
    if (Bmin > Amin)
        Amin = Bmin;
    result->x = Amin;
    if (Bmax < Amax)
        Amax = Bmax;
    result->w = Amax - Amin;

    /* Vertical intersection */
    Amin = A->y;
    Amax = Amin + A->h;
    Bmin = B->y;
    Bmax = Bmin + B->h;
    if (Bmin > Amin)
        Amin = Bmin;
    result->y = Amin;
    if (Bmax < Amax)
        Amax = Bmax;
    result->h = Amax - Amin;

    return !rect_empty(result);
}