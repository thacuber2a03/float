// Copyright (c) 2024 @thacuber2a03
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef FLOAT_VM_H
#define FLOAT_VM_H

#include <stdint.h>

#ifndef FORCE_INLINE
#	if defined(__GNUC__) || defined(__clang__)
#		define FORCE_INLINE __attribute((always_inline))
#	elif defined(_MSC_VER)
#		define FORCE_INLINE __forceinline
#	else
#		define FORCE_INLINE inline
#	endif
#endif

#define FVM_DUMP_ON_EXCEPTION

typedef struct float_VM float_VM;

typedef enum {
	FVM_OP_BRK = 0, FVM_OP_MOV,
	FVM_OP_ADD, FVM_OP_SUB, FVM_OP_MUL, FVM_OP_DIV, FVM_OP_REM,
	FVM_OP_AND, FVM_OP_OR, FVM_OP_XOR, FVM_OP_NOT,
	FVM_OP_JMP, FVM_OP_JEQ, FVM_OP_JIN, FVM_OP_JIO,
	FVM_OP_XFL, FVM_OP_DOT, FVM_OP_DIN,
} float_Opcode;

typedef enum {
	FVM_ADDR_MEMORY    = 0x0,
	FVM_ADDR_IMMEDIATE = 0x1,
	// FVM_ADDR_REGISTER  = 0x2,
	// FVM_ADDR_INDIRECT  = 0x3,
} float_AddrMode;

typedef enum {
	FVM_FLAG_ZERO     = (1 << 0),
	FVM_FLAG_NEGATIVE = (1 << 1),
	FVM_FLAG_OVERFLOW = (1 << 2),
} float_VMFlags;

typedef enum {
	FVM_STEP_OK = 0,
	FVM_STEP_HALT,
	FVM_STEP_DIV_ZERO,
	FVM_STEP_BAD_ADDRMODE,
} float_StepResult;

typedef void (*float_ExceptHandler)(float_VM *vm, float_StepResult except,
                                       uint16_t *loc, uint16_t extraInfo);

struct float_VM {
	union {
		struct { uint16_t A, B, C, D; };
		uint16_t arr[4];
	} registers;

	uint8_t flags;
	uint16_t *pc, *sp, *memory;
	uint16_t *rom; // if unset, will execute program from memory

	float_ExceptHandler handler;
};

void fvm_init(float_VM *vm);
void fvm_destroy(float_VM *vm);

static FORCE_INLINE void fvm_load_rom(float_VM *vm, uint16_t *rom)
{
	vm->rom = rom;
	vm->pc = vm->rom;
}

static FORCE_INLINE void fvm_set_pc(float_VM *vm, uint16_t pc) { vm->pc = vm->memory + pc; }
static FORCE_INLINE void fvm_set_sp(float_VM *vm, uint16_t sp) { vm->sp = vm->memory + sp; }

float_StepResult fvm_step(float_VM* vm);

float_StepResult fvm_except(float_VM *vm, float_StepResult except,
			    uint16_t *loc, uint16_t extraInfo);

#endif // FLOAT_VM
