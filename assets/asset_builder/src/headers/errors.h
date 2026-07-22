#ifndef ERROR_H
#define ERROR_H

typedef enum error_t {
    NO_ERROR,
    NO_EXTENSION,
    EXTENSION_UNSUPPORTED,
    OPEN_FAILED,
    READ_FAILED,
    WRITE_FAILED,
    BAD_SIGNATURE,
    MEMORY_ALLOC_FAILED,

    // File-format specific errors
    PNG_BIT_DEPTH_UNSUPPORTED,
    PNG_COLOR_TYPE_UNSUPPORTED
} error_t;

void print_error(const char *content);

void print_error_description(error_t error);

#endif // ERROR_H