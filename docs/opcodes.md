# instruction descriptions

### `MOV` - MOVe
asm: `MOV <src> <dst>`

copies `src` onto `dst`.

### `ADD` - ADD to register
asm: `ADD <src> <reg> <out>`

adds `src` to the register pointed to by `reg` and stores in `out`.

### `SUB` - SUBtract from register
asm: `SUB <src> <reg> <out>`

subtracts `src` from the register pointed to by `reg` and stores in `out`.

### `MUL` - MULtiply with register
asm: `MUL <src> <reg> <out>`

multiplies `src` with the register pointed to by `reg` and stores in `out`.

### `DIV` - DIVide by register
asm: `DIV <src> <reg> <out>`

divides `src` by the register pointed to by `reg` and stores in `out`.
throws an exception if reg is 0.

### `REM` - get REMainder of division with register
asm: `REM <src> <reg> <out>`

gets the remainder of the division between `src` and `reg`. stores in `out`.

## logic operations

### `AND` - bitwise AND
asm: `AND <src> <reg> <out>`

calculates the bitwise AND between `src` and `reg` and stores in `out`.

### `OR` - bitwise OR
asm: `OR <src> <reg> <out>`

calculates the bitwise OR between `src` and `reg` and stores in `out`.

### `XOR` - bitwise XOR
asm: `XOR <src> <reg> <out>`

calculates the bitwise XOR between `src` and `reg` and stores in `out`.

### `NOT` - bitwise NOT
asm: `NOT <reg> <out>`

calculates the bitwise negation of `reg` and stores it in `out`.

## branching

### `JMP` - unconditional JuMP
asm: `JMP <loc>`

jumps unconditionally to `loc`.
throws an exception if the dest. addr. mode is **register**.

### `JEQ` - Jump if EQual
asm: `JEQ <loc>`

jumps to `loc` if the `zero` flag is set.
throws an exception if the dest. addr. mode is **register**.

### `JIN` - Jump If Negative
asm: `JIN <loc>`

jumps to `loc` if the `negative` flag is set.
throws an exception if the dest. addr. mode is **register**.

### `JIO` - Jump If Overflow
asm: `JIO <loc>`

jumps to `loc` if the `overflow` flag is set.
throws an exception if the dest. addr. mode is **register**.

### `XFL` - eXchange FLags
asm `XFL <reg>`

exchanges the flags register with the contents of register `reg`.

## other

### `DOT` - Device OuT
asm: `DOT <src> <dev>`

sends a byte or a word to the device connected in port `dev`.

### `DIN` - Device IN
asm: `DIN <dev> <out>`

takes a byte or a word from device `dev` and places it in `out`.
throws an exception if the dest. addr. mode is **immediate**.

### `BRK` - BReaK
asm: `BRK`

halts the VM.
