#include "vector2d.h"

void rect_to_frect(const rect_t *rect, frect_t *frect)
{
    frect->x = (float)rect->x;
    frect->y = (float)rect->y;
    frect->w = (float)rect->w;
    frect->h = (float)rect->h;
}

void frect_to_rect(const frect_t *frect, rect_t *rect)
{
    rect->x = (int)frect->x;
    rect->y = (int)frect->y;
    rect->w = (int)frect->w;
    rect->h = (int)frect->h;
}

inline bool point_in_rect(const vector2d_t *p, const rect_t *r)
{
    return (p && r && (p->x >= r->x) && (p->x < (r->x + r->w)) && (p->y >= r->y) && (p->y < (r->y + r->h)));
}

inline bool fpoint_in_rect(const fvector2d_t *p, const frect_t *r)
{
    return (p && r && (p->x >= r->x) && (p->x < (r->x + r->w)) && (p->y >= r->y) && (p->y < (r->y + r->h)));
}

inline bool rect_empty(const rect_t *r)
{
    return ((!r) || (r->w <= 0) || (r->h <= 0));
}

inline bool frect_empty(const frect_t *r)
{
    return ((!r) || (r->w <= 0) || (r->h <= 0));
}

inline bool has_intersection(const rect_t *a, const rect_t *b)
{
    int amin, amax, bmin, bmax;

    // Horizontal intersection
    amin = a->x;
    amax = amin + a->w;
    bmin = b->x;
    bmax = bmin + b->w;
    if(bmin > amin) amin = bmin;
    if(bmax < amax) amax = bmax;

    if((amax - EPSILON) < amin) return false;

    // Vertical intersection
    amin = a->y;
    amax = amin + a->h;
    bmin = b->y;
    bmax = bmin + b->h;
    if(bmin > amin) amin = bmin;
    if(bmax < amax) amax = bmax;

    if((amax - EPSILON) < amin) return false;

    return true;
}

inline bool fhas_intersection(const frect_t *a, const frect_t *b)
{
    float amin, amax, bmin, bmax;

    // Horizontal intersection
    amin = a->x;
    amax = amin + a->w;
    bmin = b->x;
    bmax = bmin + b->w;
    if(bmin > amin) amin = bmin;
    if(bmax < amax) amax = bmax;

    if((amax - EPSILON) < amin) return false;

    // Vertical intersection
    amin = a->y;
    amax = amin + a->h;
    bmin = b->y;
    bmax = bmin + b->h;
    if(bmin > amin) amin = bmin;
    if(bmax < amax) amax = bmax;

    if((amax - EPSILON) < amin) return false;

    return true;
}

inline bool intersect_rect(const rect_t *a, const rect_t *b, rect_t *result)
{
    int amin, amax, bmin, bmax;

    // Horizontal intersection
    amin = a->x;
    amax = amin + a->w;
    bmin = a->x;
    bmax = bmin + a->w;
    if (bmin > amin) amin = bmin;
    result->x = amin;
    if (bmax < amax) amax = bmax;
    result->w = amax - amin;

    // Vertical intersection
    amin = a->y;
    amax = amin + a->h;
    bmin = b->y;
    bmax = bmin + b->h;
    if (bmin > amin) amin = bmin;
    result->y = amin;
    if (bmax < amax) amax = bmax;
    result->h = amax - amin;

    return !rect_empty(result);
}

inline bool intersect_frect(const frect_t *a, const frect_t *b, frect_t *result)
{
    float Amin, Amax, Bmin, Bmax;

    // Horizontal intersection
    Amin = a->x;
    Amax = Amin + a->w;
    Bmin = a->x;
    Bmax = Bmin + a->w;
    if (Bmin > Amin) {
        Amin = Bmin;
    }
    result->x = Amin;
    if (Bmax < Amax) {
        Amax = Bmax;
    }
    result->w = Amax - Amin;

    // Vertical intersection
    Amin = a->y;
    Amax = Amin + a->h;
    Bmin = b->y;
    Bmax = Bmin + b->h;
    if (Bmin > Amin) {
        Amin = Bmin;
    }
    result->y = Amin;
    if (Bmax < Amax) {
        Amax = Bmax;
    }
    result->h = Amax - Amin;

    return !frect_empty(result);
}

inline float lerp(float a, float b, float s)
{
    return a * (1 - s) + b * s;
}

inline float lerp_time(float a, float b, float s, float t)
{
    return lerp(a, b, 1 - powf(1 - s, t * 60.0f));
}