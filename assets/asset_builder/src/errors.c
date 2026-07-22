#include "errors.h"

#include <stdio.h>

void print_error(const char *content)
{
    printf("\033[31merror\033[m: %s\n", content);
}

void print_error_description(error_t error)
{
    switch(error)
    {
      case NO_EXTENSION:
        print_error("file does not have an extension."); break;

      case EXTENSION_UNSUPPORTED:
        print_error("extension not yet supported."); break;

      case OPEN_FAILED:
        print_error("failed to open file."); break;

      case READ_FAILED:
        print_error("failed to read the input file."); break;

      case WRITE_FAILED:
        print_error("failed to write to output file."); break;

      case BAD_SIGNATURE:
        print_error("file not valid."); break;

      case MEMORY_ALLOC_FAILED:
        print_error("memory allocation failed."); break;

      case PNG_BIT_DEPTH_UNSUPPORTED:
        print_error("png bit depth not supported."); break;

      case PNG_COLOR_TYPE_UNSUPPORTED:
        print_error("png color type not supported."); break;

      case NO_ERROR:
        return;

      default:
        print_error("unknown error"); break;
    }
}