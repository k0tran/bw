/// 2024 Sorochan Ilya

#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stddef.h>

typedef struct {
    char *name;
    size_t argc;
    size_t x64;
} SyscallInfo;

extern const SyscallInfo SYSCALL_TABLE[];

extern const size_t SYSCALL_TABLE_SIZE;

#endif
