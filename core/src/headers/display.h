#ifndef DISPLAY_H
#define DISPLAY_H

#include "SDL2/SDL_render.h"

#define C_WHITE 255, 255, 255, 255

void dclear(void)
{
    SDL_SetRenderDrawColor(renderer, C_WHITE);
    SDL_RenderClear(renderer);
}

#endif // DISPLAY_H