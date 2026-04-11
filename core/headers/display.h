#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL_render.h>

#define COLOR_WHITE 255, 255, 255, 255

#define DISPLAY_WIDTH 396
#define DISPLAY_HEIGHT 224

extern SDL_Window *window;
extern SDL_Renderer *renderer;

void display_init(void);

void display_update(void);

void display_clear(void);

void display_subimage();

void display_exit(void);

#endif // DISPLAY_H