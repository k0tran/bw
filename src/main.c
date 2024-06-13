/// 2024 Sorochan Ilya

#include "args.h"
#include "ast.h"
#include "lexer.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    CLIResult c = parseCLI(argc, argv);

    match(c) {
        of(CLIError, msg) {
            fputs(*msg, stderr);
            free(*msg);
            return 1;
        }
        of(CLIPrint, msg) {
            puts(*msg);
            free(*msg);
            return 0;
        }
        of(CLICompile, args, run) {
            TokenizeResult tr = tokenize(args->code, args->code_size);
            free(args->code);

            match (tr) {
                of(TokenizeList, tokens, size) {

                    parseASTResult par = parseAST(*tokens, *size);
                    free(*tokens);

                    match(par) {
                        of(ASTTree, value) {
                            // TODO codegen
                        }
                        of(ASTError, msg) {
                            puts(*msg);
                            free(*msg);
                            return 1;
                        }
                    }
                }
                of(TokenizeError, msg) {
                    puts(*msg);
                    free(*msg);
                    return 1;
                }
            }
        }
        of(CLIRepl) puts("TODO: REPL");
    }

    return 0;
}
