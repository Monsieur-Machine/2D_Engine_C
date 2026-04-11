#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <SDL2/SDL_events.h>

typedef SDL_Event event_t;

inline int pollevent(event_t *event)
{
    return SDL_PollEvent(event);
}

#endif // KEYBOARD_H