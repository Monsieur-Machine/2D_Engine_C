#ifndef MAP_H
#define MAP_H

#include <stdio.h>
#include <stdint.h>

int build_map(const char *input_file_name, FILE *output_file, uint32_t *asset_size);

#endif // MAP_H