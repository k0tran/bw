// 2024 Sorochan Ilya

#pragma once

typedef enum ASTNodeType {
    AST_NODE_PLUS,
    AST_NODE_MINUS,
    AST_NODE_LSHIFT,
    AST_NODE_RSHIFT,
    AST_NODE_LOOP,
    AST_NODE_SYSCALL,
    AST_NODE_RET,
    AST_NODE_JUMP,
} ASTNodeType;
