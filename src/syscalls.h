/// 2024 Sorochan Ilya

#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stddef.h>

typedef struct {
    char *name;
    size_t x64;
} SyscallInfo;

const SyscallInfo SYSCALL_TABLE[] = {
    {"read", 0},
    {"write", 1},
    {"open", 2},
    {"close", 3},
    {"exit", 60}
};

const size_t SYSCALL_TABLE_SIZE = sizeof(SYSCALL_TABLE) / sizeof(SyscallInfo);

#endif
