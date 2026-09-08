#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include "errors.h"

inline int file_read(void *ptr, size_t size, size_t nb, FILE *file)
{
    if(fread(ptr, size, nb, file) != nb)
    {
        error_printf("Failed to read output file : %d", errno);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

inline int file_write(const void *ptr, size_t size, size_t nb, FILE *file)
{
    if(fwrite(ptr, size, nb, file) != nb)
    {
        error_printf("Failed to write to output file : %d", errno);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
