/// 2024 Sorochan Ilya

#include "ast.h"
#include "common.h"
#include "datatype99.h"

#include <stdlib.h>

/// Special macro for parseASTNode
#define stackToken(TOKEN, NODE_TYPE) {             \
            size_t i = start + 1;                  \
            while (i < size) {                     \
                match(tokens[i]) {                 \
                    of(TOKEN) i++;                 \
                    otherwise goto TOKEN##_break;  \
                }                                  \
            }                                      \
            TOKEN##_break:;                        \
            n->type = NODE_TYPE(i - start);        \
        }

parseASTNodeResult parseASTNode(const Token *tokens, size_t start, size_t size) {
    ASTNode *n = malloc(sizeof(ASTNode));
    if (!n)
        return ASTNodeError(format("Out of memory")); // TODO: change that bc it's stupid'
    n->next = NULL;

    size_t amount;
    match (tokens[start]) {
        of(TokenPlus) stackToken(TokenPlus, ASTNodeAdd);
        of(TokenMinus) stackToken(TokenMinus, ASTNodeSub);
        of(TokenLeft) stackToken(TokenLeft, ASTNodeLeft);
        of(TokenRight) stackToken(TokenRight, ASTNodeRight);
        of(TokenLoopStart) {
            // find matching TokenLoopEnd
            size_t i = start + 1;
            int counter = 1;
            while (counter) { // TODO: bounds check
                match(tokens[i]) {
                    of(TokenLoopStart) counter += 1;
                    of(TokenLoopEnd) counter -= 1;
                    otherwise {} // Skip everything else
                }
                i++;
            }

            // Once found build recursevily
            parseASTNodeResult r = parseASTNode(tokens, start + 1, i);
            match (r) {
                of(ASTNodeValue, value) n->type = ASTNodeLoop(*value);
                of(ASTNodeError, err) return r;
            }
            start = i;
        }
        of(TokenLoopEnd) return ASTNodeError(format("Token loop end found (should not be)"));
        of(TokenSyscall, syscall_id) n->type = ASTNodeSyscall(*syscall_id);
        of(TokenJump) n->type = ASTNodeJump();
        of(TokenRet) n->type = ASTNodeRet();
    }
    return ASTNodeValue(n, start + 1);
}

parseASTResult parseAST(const Token *tokens, size_t size) {
    size_t i = 0;
    ASTNode dummy = {};
    ASTNode *last = &dummy;
    while (i < size) {
        parseASTNodeResult r = parseASTNode(tokens, i, size);
        match (r) {
            of(ASTNodeValue, node, shift) {
                last->next = *node;
                last = *node;
                i = *shift;
            }
            of(ASTNodeError, msg) return ASTError(*msg);
        }
    }
    return ASTTree(dummy.next);
}