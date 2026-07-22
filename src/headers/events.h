#ifndef EVENTS_H
#define EVENTS_H

#include <keycodes.h>
#include <vector2d.h>

typedef struct events_t {
    vector2d_t cursor_pos;
    bool keys[MAX_KEYCODES];
} events_t;

#endif // EVENTS_H