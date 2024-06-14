/// 2024 Sorochan Ilya
/// This module is used to translate universal AST into set of instructions

#ifndef CODEGEN_H
#define CODEGEN_H

#include "ast.h"
#include <stdio.h>

// TODO: make Arch param
size_t dumpCode(ASTNode *head, FILE *out, size_t loop_id);

#endif
