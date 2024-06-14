/// 2024 Sorochan Ilya
/// This module is for parsing and ensuring syntax

#ifndef LEXER_H
#define LEXER_H

#include <datatype99.h>
#include <stddef.h>

datatype(
    Token,
    (TokenPlus),
    (TokenMinus),
    (TokenLeft),
    (TokenRight),
    (TokenLoopStart),
    (TokenLoopEnd),
    (TokenSyscall, size_t),
    (TokenRetval),
    (TokenJump),
    (TokenRet)
);

datatype(
    parseTokenResult,
    (TokenValue, Token, size_t),
    (TokenError, char *),
    (TokenEof)
);

/// Parses one token. Returns None if all tokens are exausted
parseTokenResult parseToken(const char *code, size_t start, size_t size);

datatype(
    TokenizeResult,
    (TokenizeList, Token *, size_t),
    (TokenizeError, char *)
);

/// Parse input into tokens
TokenizeResult tokenize(const char *code, size_t size);

#endif
