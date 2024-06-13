/// 2024 Sorochan Ilya

#include "args.h"
#include "common.h"

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

const char *HELP_STRING =
"Usage: %s [command] [options]\n\n"
"Commands:\n\n"
"    build      Compile .bw code\n"
"    run        Build and run executable\n"
"    repl       Run bw in interactive mode\n"
"    version    Print bw version\n"
"    help       Print this help message\n";

const char *ERROR_STRING =
"Incorrect CLI arguments\n"
"Try '%s help' for more information\n";

CLIResult parseCLI(int argc, char **argv) {
    if (argc < 2 || !strcmp("help", argv[1]))
        return CLIPrint(format(HELP_STRING, argv[0]));
    if (!strcmp("version", argv[1]))
        return CLIPrint(format("Version: %s\n", VERSION));
    if (!strcmp("repl", argv[1]))
        return CLIRepl();

    bool run = !strcmp("run", argv[1]);
    if (run || !strcmp("build", argv[1])) {
        if (argc < 3)
            return CLIError(format("Please provide at least one file for building\n"));

        FILE *file = fopen(argv[2], "rb");
        if (!file)
            return CLIError(format("Error while opening file %s\n", argv[2]));

        fseek(file, 0, SEEK_END);
        long size = ftell(file);
        fseek(file, 0, SEEK_SET);
        if (size < 0) {
            fclose(file);
            return CLIError(format("Error while getting contens size of file %s\n", argv[2]));
        }

        assert(sizeof(size_t) >= sizeof(long)); // Check that casting is valid
        CompileArgs a = {NULL, size};

        a.code = malloc(a.code_size + 1);
        // if malloc fail we are screwed
        if (!a.code) {
            fclose(file);
            fputs("parseCLI: out of memory\n", stderr);
            exit(1);
        }

        if (fread(a.code, 1, a.code_size, file) != a.code_size) {
            free(a.code);
            fclose(file);
            return CLIError(format("Error while reading contents of file %s\n", argv[2]));
        }
        a.code[a.code_size] = '\0';

        fclose(file);

        return CLICompile(a, run);
    }

    return CLIError(format(ERROR_STRING, argv[0]));
}

#undef STR
