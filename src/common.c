#include "common.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

char *format(const char *format, ...) {
    va_list args;

    // Estimate the size of the result
    va_start(args, format);
    int size = vsnprintf(NULL, 0, format, args);
    va_end(args);

    // Check that formatted string is correct
    if (size < 0) {
        fputs("format: vsnprintf error\n", stderr);
        exit(1);
    }

    // Allocate with check
    char *result = malloc(size + 1);
    if (!result){
        fputs("format: out of memory\n", stderr);
        exit(1);
    }

    // Format string
    va_start(args, format);
    vsnprintf(result, size + 1, format, args);
    va_end(args);

    return result;
}
