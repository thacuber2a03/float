# Balloon assembly syntax
(working name)

> [!TODO]
> should I introduce labels before opcode format?

the common format for instructions is of the form `op [arg (, arg)*]`.

`op` is a case-insensitive mnemonic as specified by [docs/opcodes](docs/opcodes.md).

each `arg` depends on the instruction, but can be any of:

- `#x` for immediate addressing
- `$x` or just `x` for memory addressing
- `[x]` for indirect addressing
- one of the registers `A`, `B`, `C` and `D` for register addressing

`x` can be a decimal number, a hexadecimal number with prefix `0x`, a binary number with prefix `0b` or a valid label name.

a `#x` by itself will output the value of the number, opcode or label directly into the source.
