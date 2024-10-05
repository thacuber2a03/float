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

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "fvm.h"

#ifdef FVM_DUMP_ON_EXCEPTION
#include <stdio.h>
#endif

enum {
	VM_SRC_ADDR_MODE_MASK  = 0xc000, // 0b1100000000000000
	VM_DST_ADDR_MODE_MASK  = 0x3000, // 0b0011000000000000
	VM_INST_MASK           = 0x0fff, // 0b0000111111111111

	VM_SRC_ADDR_MODE_SHIFT = 14,
	VM_DST_ADDR_MODE_SHIFT = 12,
};

#define errorPrefix "float fatal error: "

void fvm_init(float_VM *vm)
{
	memset(vm, 0, sizeof *vm);

	vm->memory = malloc(UINT16_MAX * sizeof *vm->memory);
	assert(vm->memory && errorPrefix "vm->memory allocation failed");

	fvm_set_pc(vm, 0);
	fvm_set_sp(vm, UINT16_MAX);
}

void fvm_destroy(float_VM *vm)
{
	if (vm->memory)
	{
		free(vm->memory);
		vm->memory = NULL;
	}

	memset(vm, 0, sizeof *vm);
}

static uint16_t *get_register(float_VM *vm, uint8_t i, int assign)
{
	switch (i)
	{
	case 4: {
		uint16_t *sp = vm->sp;
		if (assign) vm->sp--;
		return sp;
	}
	case 5: return (uint16_t*)&vm->flags;
	default: return &vm->registers.arr[i & 3];
	}
}

static uint16_t *addr_mode_loc(float_VM *vm,
                               float_AddrMode am, uint16_t *s, int assign)
{
	switch (*s)
	{
	case FVM_ADDR_MEMORY:    return &vm->memory[*s];
	case FVM_ADDR_IMMEDIATE: return s;
	case FVM_ADDR_REGISTER:  return get_register(vm, *s, assign);
	case FVM_ADDR_INDIRECT:  return &vm->memory[vm->memory[*s]];
	default:
		assert(0 && "unreachable");
		break;
	}
}

#define nextShort(vm) (*(vm)->pc++)

float_StepResult fvm_step(float_VM* vm)
{
	uint16_t *oldPc = vm->pc;
	uint16_t raw = nextShort(vm);
	uint16_t ins = raw & VM_INST_MASK;

	// quick exit
	if (ins == FVM_OP_BRK) return FVM_STEP_HALT;

	float_AddrMode srcAMode = (raw & VM_SRC_ADDR_MODE_MASK) >> VM_SRC_ADDR_MODE_SHIFT;
	float_AddrMode dstAMode = (raw & VM_DST_ADDR_MODE_MASK) >> VM_DST_ADDR_MODE_SHIFT;

	uint16_t a, b, out;

	switch (ins)
	{
	case FVM_OP_MOV:
		a = nextShort(vm), b = nextShort(vm);

		if (dstAMode == FVM_ADDR_IMMEDIATE)
			return fvm_except(vm, FVM_STEP_BAD_ADDRMODE, oldPc, ins);

		*addr_mode_loc(vm, srcAMode, &b, 1) = *addr_mode_loc(vm, dstAMode, &a, 0);
		break;

	case FVM_OP_ADD:
		
		break;

	default:
		assert(0 && "unreachable");
		break;
	}

	return FVM_STEP_OK;
}

float_StepResult fvm_except(float_VM *vm, float_StepResult except,
			    uint16_t *loc, uint16_t extraInfo)
{
	if (vm->handler!= NULL)
	{
		vm->handler(vm, except, loc, extraInfo);
	}

#ifdef FVM_DUMP_ON_EXCEPTION
	fprintf(stderr, "vm(%p) exception:\n", vm);
	switch (except) {
	case FVM_STEP_OK: fprintf(stderr, "no exception"); break;
	case FVM_STEP_HALT: fprintf(stderr, "machine halt"); break;
	case FVM_STEP_DIV_ZERO:
		fprintf(stderr, "division by zero");
		break;
	case FVM_STEP_BAD_ADDRMODE:
		fprintf(stderr, "bad addressing mode");
		break;
	}
	fprintf(stderr, "\nat 0x%04x", (uint16_t)(vm->pc - vm->memory));
	fprintf(stderr, "\nregisters:\nA\t0x%04x\tB\t0x%04x\n", vm->registers.A, vm->registers.B);
	fprintf(stderr, "C\t0x%04x\tD\t0x%04x\n", vm->registers.C, vm->registers.D);
#endif

	return except;
}
