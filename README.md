# BrainWash

Remeber this really awesome language called [BrainFuck](https://en.wikipedia.org/wiki/Brainfuck)? Well, now you can make real CLI apps with it (not really).

## Features v0.1.0 (in progress)

Compared to the default BF:
- No `.,` instructions. That's too easy;
- New syscall operator `%`;
- New jump to pointer `*` and `&` operators;
- Tape addres, argc, argv are avaliable on the tape.

## WHY???

Well, I really liked the idea of [systemf](https://github.com/ajyoon/systemf), but I thought that compilers are much cooler than interpreters and I wanted to have access to CLI arguments and ENV.

## More in details

### Intial tape setup

Each cell on tape is byte-sized. Addition and substraction to the cell value is wrapping. Amount of cells restricted by OS (it's literally stack). For the same reason zeroing is not guaranteed.

Before your BW programs starts there is a little setup:
1. First ptr_size cells contain address of tape (this cell to be precise) (`$rsp - 8` in x64);
2. Second ptr_size cells contain argc (`$rsp` in x64);
3. Third ptr_size cells contain ptr to argv (`$rsp + 8` in x64).

Note that strings on the stack stored as bytes, while pointers are little endian (big-endian and little-endian crossplatform is not supported)

### Avaliable instructions

- default BF instructions `+-<>[]`;
- syscall instruction `%<syscall_name>` which takes next ptr_size * syscall_argc cells and makes a syscall;
- `*` instruction moves current cell ptr to another register and changes current cell ptr to that what lies on tape (ptr-sized);
- `&` instruction moves to current cell ptr from register that is used by `*`.

### Implemented optimizations

- squash repeated instructions.

TODO: [those optimizations](http://calmerthanyouare.org/2015/01/07/optimizing-brainfuck.html)

### Supported platforms

- linux-x64

TODO: win64, arm64

# Usage

```
./bw run <bw file>
```

## Compile

Based cmake project only C99 compiler required:
```sh
mkdir -p build
cd build
cmake ..
make # If you are using linux
```
