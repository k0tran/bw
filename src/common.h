/// 2024 Sorochan Ilya
/// Functions for error handling

#ifndef COMMON_H
#define COMMON_H

#ifndef VERSION
#error "Please define version"
#endif

/// Format string using heap
/// If something goes wrong this function exits
char *format(const char *format, ...);

#endif
