#ifndef EVENTS_H
#define EVENTS_H

#include <keycodes.h>
#include <vector2d.h>

typedef struct events_t {
    bool keys[MAX_KEYCODES];
    vector2d_t cursor_pos;
} events_t;

#endif // EVENTS_H