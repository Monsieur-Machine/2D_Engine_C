#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

int file_read(void *ptr, size_t size, size_t nb, FILE *file);

int file_write(const void *ptr, size_t size, size_t nb, FILE *file);

#endif // UTILS_H