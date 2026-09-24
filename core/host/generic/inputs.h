#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>
#include <stdbool.h>
#include "keycodes.h"

typedef struct input_event_t {
    uint32_t type;
    int key;
} input_event_t;

int poll_event(input_event_t *event);

typedef struct inputs_t {
    bool keys[MAX_KEYCODES];
} inputs_t;

#endif // KEYBOARD_H