#ifndef MAP_H
#define MAP_H

#include "asset_file.h"

int parse_map_for_header(const char *input_file_name, asset_header_def_t *asset_header_def);
int parse_map_for_content_and_write(const char *input_file_name, FILE *output_file);

#endif // MAP_H