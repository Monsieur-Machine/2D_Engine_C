#ifndef TEXTURE_H
#define TEXTURE_H

#include <gint/display.h>

#define TEXTURE_TRANSPARENT_COLOR 0x0001

typedef bopti_image_t texture_t;

#define get_width(T) T->width
#define get_height(T) T->height

texture_t *load_texture(void);
void destroy_texture(texture_t *texture);

#endif // TEXTURE_H