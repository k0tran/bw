/// 2024 Sorochan Ilya

#include "lexer.h"
#include "syscalls.h"
#include "common.h"

#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

/// Parses one token. Returns None if all tokens are exausted
parseTokenResult parseToken(const char *code, size_t start, size_t size) {
    for (size_t i = start; i < size; ++i)
        switch (code[i]) {
            case '+': return TokenValue(TokenPlus(), ++i);
            case '-': return TokenValue(TokenMinus(), ++i);
            case '<': return TokenValue(TokenLeft(), ++i);
            case '>': return TokenValue(TokenRight(), ++i);
            case '[': return TokenValue(TokenLoopStart(), ++i);
            case ']': return TokenValue(TokenLoopEnd(), ++i);
            case '%': {
                // Calculate syscall name length
                size_t j = ++i;
                while (j < size && isalpha(code[j])) ++j;
                size_t size = j - i;

                if (size == 0)
                    return TokenError(format("Syscall name not provided\n"));

                // Check if there is such syscall
                for (j = 0; j < SYSCALL_TABLE_SIZE; ++j)
                    if (!strncmp(SYSCALL_TABLE[j].name, code + i, size))
                        break;

                if (j == SYSCALL_TABLE_SIZE)
                    return TokenError(format("Unrecognized syscall '%.*s'\n", size, code + i));

                return TokenValue(TokenSyscall(j), i + size);
            }
            case '*': return TokenValue(TokenJump(), ++i);
            case '&': return TokenValue(TokenRet(), ++i);
            default: {} // Let loop go
        }
    return TokenEof();
}

TokenizeResult tokenize(const char *code, size_t size) {
    Token *token_list = malloc(size * sizeof(Token));
    size_t i = 0;
    size_t start = 0;
    bool eof = false;

    while (!eof) {
        parseTokenResult ptr = parseToken(code, start, size);
        match (ptr) {
            of(TokenValue, token, shift) {
                start = *shift;
                token_list[i] = *token;
                i++;
            }
            of(TokenError, msg) return TokenizeError(*msg);
            of(TokenEof) eof = true;
        }
    }

    return TokenizeList(token_list, i);
}
