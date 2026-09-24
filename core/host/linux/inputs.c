#include "inputs.h"

#include <SDL3/SDL_events.h>

inline int poll_event(input_event_t *event)
{
    SDL_Event sdl_event;
    int ret = SDL_PollEvent(&sdl_event);

    event->type = sdl_event.type;
    event->key = sdl_event.key.scancode;

    return ret;
}
