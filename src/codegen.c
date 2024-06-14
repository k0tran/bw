/// 2024 Sorochan Ilya

#include "codegen.h"

#include "syscalls.h"

const char *HEADER = \
"global _start\n"
"_start:\n"
"    mov rbx, rsp\n"
"    sub rbx, 8\n"
"    mov [rbx], rbx\n"
"    mov rcx, [rsp]\n"
"    mov [rbx - 8], rcx\n"
"    lea rcx, [rsp + 8]\n"
"    mov [rbx - 16], rcx\n\n"
"main:\n";

const char *LOOP_TEMPLATE_START = \
"\n"
"    jmp loop_%1$zu_condition\n"
"loop_%1$zu:\n";

const char *LOOP_TEMPLATE_END = \
"\n"
"loop_%1$zu_condition:\n"
"    cmp byte [rbx], 0\n"
"    jne loop_%1$zu\n\n";

size_t dumpCode(ASTNode *head, FILE *out, size_t loop_id) {
    if (loop_id == 0)
        fputs(HEADER, out);

    while (head != NULL) {
        match(head->type) {
            of(ASTNodeAdd, x)    fprintf(out, "    add byte [rbx], %zu\n", *x);
            of(ASTNodeSub, x)    fprintf(out, "    sub byte [rbx], %zu\n", *x);
            of(ASTNodeLeft, x)   fprintf(out, "    add rbx, %zu\n", *x);
            of(ASTNodeRight, x)  fprintf(out, "    sub rbx, %zu\n", *x);
            of(ASTNodeLoop, loop_code) {
                size_t current_loop = loop_id;

                fprintf(out, LOOP_TEMPLATE_START, current_loop);

                loop_id = dumpCode(*loop_code, out, loop_id + 1);

                fprintf(out, LOOP_TEMPLATE_END, current_loop);
            }
            of(ASTNodeSyscall, syscall_id) {
                SyscallInfo si = SYSCALL_TABLE[*syscall_id];
                fprintf(out, "\n    ; %s\n    mov rax, %zu\n", si.name, si.x64);
                if (si.argc >= 1)
                    fputs("    mov rdi, [rbx]\n", out);
                if (si.argc >= 2)
                    fputs("    mov rsi, [rbx - 8]\n", out);
                if (si.argc >= 3)
                    fputs("    mov rdx, [rbx - 16]\n", out);
                if (si.argc >= 4)
                    fputs("    mov r10, [rbx - 24]\n", out);
                if (si.argc >= 5)
                    fputs("    mov r8, [rbx - 32]\n", out);
                if (si.argc >= 6)
                    fputs("    mov r9, [rbx - 40]\n", out);
                fputs("    syscall\n", out);
            }
            of(ASTNodeRetval) fputs("    mov [rbx], rax\n", out);
            of(ASTNodeJump) fputs("    mov rcx, rbx\n    mov rbx, [rbx]\n", out);
            of(ASTNodeRet) fputs("    mov rbx, rcx\n", out);
        }
        head = head->next;
    }

    return loop_id;
}
