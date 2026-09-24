#ifndef DISPLAY_H
#define DISPLAY_H

#include "texture.h"
#include "vector2d.h"

#define C_BLACK 0,   0,   0
#define C_RED   255, 0,   0
#define C_GREEN 0,   255, 0
#define C_BLUE  0,   0,   255
#define C_WHITE 255, 255, 255

#define DISPLAY_WIDTH 720
#define DISPLAY_HEIGHT 480

typedef struct display_t {
} display_t;

int display_init(void);
void display_exit(void);

int display_clear(void);
int display_update(void);

int display_draw_texture(texture_t *texture, const rect_t *src_rect, const rect_t *dst_rect, bool flip);
int display_draw_frect(const frect_t *frect, unsigned int r, unsigned int g, unsigned int b, unsigned int a);

#endif // DISPLAY_H