#ifndef IMAGE_H
#define IMAGE_H

#include <stdio.h>
#include <stdint.h>

int build_image(const char *input_file_name, FILE *output_file, uint32_t *asset_size);

#endif // IMAGE_H