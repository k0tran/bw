/// 2024 Sorochan Ilya
/// This module contains functions userful for CLI and file processing

#ifndef ARGS_H
#define ARGS_H

#include <datatype99.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct {
    char *code;
    size_t code_size;
} CompileArgs;

datatype(
    CLIResult,
    (CLIError, char *),
    (CLIPrint, char *),
    (CLICompile, CompileArgs, bool),
    (CLIRepl)
);

/// On malloc errors this function exits
CLIResult parseCLI(int argc, char **argv);

#endif
