// 2024 Sorochan Ilya
// Here in BW we don't have functions
// So AST is really an array of instructions (except for loop one)

#ifndef AST_H
#define AST_H

#include "lexer.h"

datatype(
    ASTNodeType,
    (ASTNodeAdd, size_t),
    (ASTNodeSub, size_t),
    (ASTNodeLeft, size_t),
    (ASTNodeRight, size_t),
    (ASTNodeLoop, void *), // this must point to ASTNode
    (ASTNodeSyscall, size_t),
    (ASTNodeRetval),
    (ASTNodeJump),
    (ASTNodeRet)
);

typedef struct ASTNode {
    ASTNodeType type;
    struct ASTnode *next;
} ASTNode;

datatype(
    parseASTResult,
    (ASTTree, ASTNode *),
    (ASTError, char *)
);

parseASTResult parseAST(const Token *tokens, size_t size);

datatype(
    parseASTNodeResult,
    (ASTNodeValue, ASTNode *, size_t),
    (ASTNodeError, char *)
);

parseASTNodeResult parseASTNode(const Token *tokens, size_t start, size_t size);

#endif
