#ifndef CAMERA_H
#define CAMERA_H

#include "vector2d.h"

typedef struct camera_t {
    rect_t view_rect;
    fvector2d_t *tracking_pos;
    vector2d_t tracking_center_offset;
} camera_t;

void camera_init(void);

void camera_update(void);

#endif // CAMERA_H