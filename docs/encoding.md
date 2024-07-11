# encoding

## description
all instructions are decoded from one byte.
each instruction can have a variable amount of parameters.

## format
```
| 7 6 | 5 | 4 3 2 1 0 |
| a a | w | o o o o o |
```
where
- `a` is the addressing mode
- `w` is the "word" flag, i.e. whether the instruction works on shorts
- `o` is the bits that actually make up the opcode

## modes
|   name    |               description               | implementation |
| :-------: | :-------------------------------------: | :------------: |
|  memory   | operand represents an address in memory |   `mem[op]`    |
| register  |      operand represents a register      |   `regs[op]`   |
| immediate |       operand is taken literally        |      `op`      |
| indirect  | operand refers to an address in memory  | `mem[mem[op]]` |
