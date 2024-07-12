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

#define errorPrefix "float fatal error: "

enum {
	VM_ADDR_MODE_MASK = 0xc0, // 0b11000000
	VM_WORD_FLAG_MASK = 0x20, // 0b00100000
	VM_INST_MASK      = 0x1f, // 0b00011111
};

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

float_StepResult fvm_step(float_VM* vm)
{
	uint8_t raw = *vm->pc++;

	int word = raw & VM_WORD_FLAG_MASK;
	float_AddrMode addrMode = raw & VM_ADDR_MODE_MASK;

	switch (raw & VM_INST_MASK)
	{
	case FVM_OP_BRK: return FVM_STEP_HALT;

	default:
		assert(0 && "unreachable");
		break;
	}

	return FVM_STEP_OK;
}
