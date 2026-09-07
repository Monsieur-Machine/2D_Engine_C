#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <SDL3/SDL_events.h>
#include "keycodes.h"

#define INPUT_EVENT_KEY_UP SDL_EVENT_KEY_UP
#define INPUT_EVENT_KEY_DOWN SDL_EVENT_KEY_DOWN
#define INPUT_EVENT_QUIT SDL_EVENT_QUIT

typedef struct input_event_t {
    uint32_t type;
    SDL_Scancode key;
} input_event_t;

int poll_event(input_event_t *event);

typedef struct inputs_t {
    bool keys[MAX_KEYCODES];
} inputs_t;

#endif // KEYBOARD_H