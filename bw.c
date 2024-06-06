// 2024 Sorochan Ilya
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

const char *PREFIX_CODE = \
"global _start\n\n"
"section .text\n"
"_start:\n"
"    mov rbx, rsp\n"
"    sub rbx, 8\n"
"    mov [rbx], rbx ; And store it on tape\n\n";

const char *BRACKET_TEMPLATE_START = \
"\n"
"    cmp byte [rbx], 0\n"
"    je bw_label_%1$d_end\n"
"bw_label_%1$d_start:\n";

const char *BRACKET_TEMPLATE_END = \
"\n"
"    cmp byte [rbx], 0\n"
"    jne bw_label_%1$d_start\n"
"bw_label_%1$d_end:\n";

typedef struct Slice {
    char *data;
    size_t size;
} Slice;

Slice Slice_new(char *data, size_t size) {
    Slice s = {data, size};
    return s;
}

Slice Slice_empty() {
    return Slice_new(NULL, 0);
}

bool Slice_isEmpty(Slice s) {
    return s.data == NULL || s.size == 0;
}

// Make a slice that do not owns the memory
Slice Slice_subslice(Slice s, size_t start, size_t end) {
    if (s.size < start || s.size < end)
        return Slice_empty();
    s.data += start;
    s.size = end - start;
    return s;
}

void Slice_free(Slice *s) {
    if (s == NULL || Slice_isEmpty(*s))
        return;
    free(s->data);
    s->data = NULL;
    s->size = 0;
}

Slice readAlloc(const char *filename) {
    FILE *input = fopen(filename, "r");
    if (input == NULL)
        return Slice_empty();

    fseek(input, 0L, SEEK_END);
    size_t size = ftell(input);
    fseek(input, 0L, SEEK_SET);

    char *code = malloc(sizeof(char) * size);
    unsigned long read_amount = fread(code, sizeof(char), size, input);
    fclose(input);

    if (read_amount != size) {
        free(code);
        return Slice_empty();
    }

    return Slice_new(code, size);
}

int compile(FILE *output, Slice code, int loop) {
    size_t cnt, j;

    for (size_t i = 0; i < code.size; ++i) {
        switch (code.data[i]) {
            case '+':
                fputs("    inc byte [rbx]\n", output);
                break;
            case '-':
                fputs("    dec byte [rbx]\n", output);
                break;
            case '<':
                fputs("    dec rbx\n", output);
                break;
            case '>':
                fputs("    inc rbx\n", output);
                break;
            case '[':
                // Find matching ']'
                cnt = 0;
                for (j = i + 1; j < code.size; ++j)
                    if (code.data[j] == '[')
                        cnt++;
                    else if (code.data[j] == ']') {
                        if (cnt == 0)
                            break;
                        else
                            cnt--;
                    }

                // Check if ']' matching is found
                if (j >= code.size) {
                    fputs("Error while searching for matching ']'\n", stderr);
                    return -1;
                }

                // Compile loop
                cnt = loop;
                fprintf(output, BRACKET_TEMPLATE_START, cnt);
                loop = compile(output, Slice_subslice(code, i + 1, j), loop + 1);
                if (loop == -1)
                    return loop;
                fprintf(output, BRACKET_TEMPLATE_END, cnt);

                // i will be inc'ed further by the loop'
                i = j;

                break;
            case ']':
                fputs("Found unmatching ']'\n", stderr);
                return -1;
            case '%':
                i += 1;
                cnt = code.data[i] - '0';
                if (cnt > 6) {
                    fprintf(stderr, "Unknown syscall number %zu\n", cnt);
                    return -1;
                }

                // Write syscall
                fputs("\n    mov rax, [rbx]\n", output);
                if (cnt >= 1)
                    fputs("    mov rdi, [rbx + 8]\n", output);
                if (cnt >= 2)
                    fputs("    mov rsi, [rbx + 16]\n", output);
                if (cnt >= 3)
                    fputs("    mov rdx, [rbx + 24]\n", output);
                if (cnt >= 4)
                    fputs("    mov r10, [rbx + 32]\n", output);
                if (cnt >= 5)
                    fputs("    mov r8, [rbx + 40]\n", output);
                if (cnt >= 6)
                    fputs("    mov r9, [rbx + 48]\n", output);
                fputs("    syscall\n", output);

                break;
            default:
                break;
        }
    }

    return loop;
}

int main(int argc, const char **argv) {
    if (argc != 2)
        printf("Usage: %s FILE\n", argv[0]);

    Slice code = readAlloc(argv[1]);
    if (Slice_isEmpty(code))
        return 1;

    fprintf(stdout, "%s", PREFIX_CODE);
    compile(stdout, code, 0);

    Slice_free(&code);

    return 0;
}
