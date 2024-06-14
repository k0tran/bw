/// 2024 Sorochan Ilya

#include "syscalls.h"

const SyscallInfo SYSCALL_TABLE[] = {
    {"read", 3, 0},
    {"write", 3, 1},
    {"open", 3, 2},
    {"close", 3, 3},
    {"exit", 1, 60}
};

const size_t SYSCALL_TABLE_SIZE = sizeof(SYSCALL_TABLE) / sizeof(SyscallInfo);
