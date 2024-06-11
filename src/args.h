/// 2024 Sorochan Ilya
/// This module contains functions userful for CLI and file processing

#ifndef ARGS_H
#define ARGS_H

#include <datatype99.h>
#include <stddef.h>
#include <stdbool.h>

/// Format string using heap
/// If something goes wrong this function exits
char *format(const char *format, ...);

typedef struct {
    char *code;
    size_t code_size;
} CompileArgs;

datatype(
    Command,
    (Error, char *),
    (Print, char *),
    (Compile, CompileArgs, bool),
    (Repl)
);

/// On malloc errors this function exits
Command parseCLI(int argc, char **argv);

#endif
