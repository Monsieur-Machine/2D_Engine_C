#include "errors.h"

#include <stdio.h>

void print_error(const char *content)
{
    printf("\033[31merror\033[m: %s\n", content);
}
