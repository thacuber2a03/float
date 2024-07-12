# encoding

## description
all instructions are decoded from one short.
each instruction can have a variable amount of parameters.

## format
```
| f e d c b a 9 8 7 6 5 4 3 2 1 0 |
| s s d d o o o o o o o o o o o o |
```
where
- `s` is the source addressing mode
- `d` is the destination addressing mode
- `o` is the bits that make up the opcode

## modes
|   name    |               description               | implementation |
| :-------: | :-------------------------------------: | :------------: |
|  memory   | operand represents an address in memory |   `mem[op]`    |
| register  |      operand represents a register      |   `regs[op]`   |
| immediate |       operand is taken literally        |      `op`      |
| indirect  | operand refers to an address in memory  | `mem[mem[op]]` |
