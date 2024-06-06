# BrainWash

Remeber this really awesome language called [BrainFuck](https://en.wikipedia.org/wiki/Brainfuck)? Well, now you can make real CLI apps with it (not really).

## Features

- No `.,` instructions. That's too easy;
- New syscall operator `%`;
- Actual address of tape on tape (such a pain honestly).

## How to syscall?

Well, you can look up [examples](examples/README.md). But in general you have 8 cells (x64) for syscall id and then 0-6 arguments. Each argument takes 8 cells as well. When all arguments are set just use `%N` op on a starting cell of syscall id (N is the number of arguments).

## WHY???

Well, I really liked the idea of [systemf](https://github.com/ajyoon/systemf), but I thought that compilers are much cooler than interpreters and I wanted to have access to CLI arguments and ENV.

## How to compile

Currently there is only one way to compile this atrocity. With any C99 compiler, nasm and any linker:
```sh
clang -std=c99 bw.c -Wall -o bw
./bw examples/hw.bw > hw.asm
nasm -f elf64 hw.asm -o hw.o
ld hw.o -o hw
```
