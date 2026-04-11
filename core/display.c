#include "display.h"

#include <SDL2/SDL.h>

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

void display_init(void)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow("2D_Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, DISPLAY_WIDTH, DISPLAY_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void display_update(void)
{

}

void display_clear(void)
{
    SDL_SetRenderDrawColor(renderer, COLOR_WHITE);
    SDL_RenderClear(renderer);
}

void display_subimage()
{

}

void display_exit_void(void)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
