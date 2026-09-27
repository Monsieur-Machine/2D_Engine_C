#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL3/SDL_render.h>

#define TEXTURE_TRANSPARENT_COLOR 0x0001

typedef SDL_Texture texture_t;

#define texture_get_width(T) T->w
#define texture_get_height(T) T->h

texture_t *load_texture(void);
void destroy_texture(texture_t *texture);

#endif // TEXTURE_H