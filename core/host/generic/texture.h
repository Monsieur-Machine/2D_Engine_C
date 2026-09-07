#ifndef TEXTURE_H
#define TEXTURE_H

#define TEXTURE_TRANSPARENT_COLOR 0x0001

typedef struct texture_t texture_t;

#define get_width(T) 1
#define get_height(T) 1

texture_t *load_texture(void);
void destroy_texture(texture_t *texture);

#endif // TEXTURE_H