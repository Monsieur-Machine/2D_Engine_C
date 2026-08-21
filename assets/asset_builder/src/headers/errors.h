#ifndef ERRORS_H
#define ERRORS_H

#include <stdio.h>
#include <errno.h>
#include <assert.h>

#ifndef DEBUG
#define DEBUG 0
#endif

#if DEBUG > 0
#define debug_printf(...) \
    do { \
        fprintf(stderr, "\033[32mINFO\033[m: " __VA_ARGS__); \
        fputc('\n', stderr); \
    } while (0)

#define return_failure_int \
    do { \
        fprintf(stderr, "\tin %s:%d:%s()\n", __FILE__, __LINE__, __func__); \
        return EXIT_FAILURE; \
    } while (0);

#define return_failure_ptr \
    do { \
        fprintf(stderr, "\tin %s:%d:%s()\n", __FILE__, __LINE__, __func__); \
        return NULL; \
    } while (0);
#else
#define debug_printf(...)

#define return_failure_int return EXIT_FAILURE;
#define return_failure_ptr return NULL;
#endif

#define warning_printf(...) \
    do { \
        fprintf(stderr, "\033[35mWARNING\033[m: " __VA_ARGS__); \
        fputc('\n', stderr); \
    } while (0)

#define error_printf(...) \
    do { \
        fprintf(stderr, "\033[31mERROR\033[m: %s:%d:%s(): ", __FILE__, __LINE__, __func__); \
        fprintf(stderr, __VA_ARGS__); \
        fputc('\n', stderr); \
    } while (0)

#endif // ERRORS_H