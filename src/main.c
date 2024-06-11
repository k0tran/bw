/// 2024 Sorochan Ilya

#include "args.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    Command c = parseCLI(argc, argv);

    match(c) {
        of(Error, msg) {
            fputs(*msg, stderr);
            free(*msg);
            return 1;
        }
        of(Print, msg) {
            puts(*msg);
            free(*msg);
            return 0;
        }
        of(Compile, args, run) printf("Compile code: %s\nRun after compile: %d\n", args->code, *run);
        of(Repl) puts("TODO: REPL");
    }

    return 0;
}
