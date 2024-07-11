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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define signature "float"
#define dietext (signature " fatal error: ")

static void die(const char *msg)
{
    size_t l = strlen(msg);

    char *buf = malloc(sizeof(dietext) + l + 1);
    memcpy(buf, dietext, sizeof(dietext));
    memcpy(buf + sizeof dietext, msg, l);
    buf[sizeof dietext + l + 1] = 0;

    perror(dietext);
    free(buf);

    exit(-1);
}

static uint8_t read_byte(FILE* fp)
{
    uint8_t c;
    if (fread(&c, sizeof c, 1, fp) != 1)
        die("couldn't read next byte from file");
    return c;
}

static uint16_t get_number(FILE* fp, int word)
{
    uint8_t c = read_byte(fp);
    if (!word) return c;
    return c << 8 | read_byte(fp);
}

static void interpret(FILE* fp)
{
    union { struct { uint16_t A, B, C, D; }; uint16_t arr[4]; } regs;

    uint8_t mem[0xffff+1];
    memset(mem, 0, sizeof mem);

    for (;;)
    {
#define IMM_MASK (1 << 0)
#define PTR_MASK (1 << 1)
#define WRD_MASK (1 << 2)
#define get_num() get_number(fp, wrd)
#define qual_num(n) (reg ? regs.arr[n&3] : imm ? n : mem[n])
#define reg ptr

        uint8_t raw = read_byte(fp);

        uint8_t imm = raw & IMM_MASK,
             ptr = raw & PTR_MASK,
             wrd = raw & WRD_MASK;

        uint8_t ins = (raw & ~(IMM_MASK | PTR_MASK | WRD_MASK)) >> 3;

        switch (ins)
        {
            case 0x00: // BRK
                return;

            // FIXME: lotta repeated code :(
            case 0x01: { // MOV
                uint16_t num = get_num();
                uint16_t addr = mem[get_num()];
                mem[addr] = qual_num(num);
            } break;

            case 0x02: { // MRG
                uint16_t a = get_num();
                uint16_t *b = &regs.arr[get_num()&3];
                *b = qual_num(a);
            } break;

            case 0x03: { // ADD
                uint16_t a = get_num();
                uint16_t *b = &regs.arr[get_num()&3];
                *b += qual_num(a);
            }

            case 0x04: { // SUB
                uint16_t a = get_num();
                uint16_t *b = &regs.arr[get_num()&3];
                *b -= qual_num(a);
            }

            case 0x05: { // MUL
                uint16_t a = get_num();
                uint16_t *b = &regs.arr[get_num()&3];
                *b *= qual_num(a);
            }

            case 0x06: { // DIV
                uint16_t a = get_num();
                uint16_t *b = &regs.arr[get_num()&3];
                // TODO: throw div zero exception
                // throw any exception, actually
                *b /= qual_num(a);
            }
            
            case 0x07: { // AND
                uint16_t a = get_num();
                uint16_t *b = &regs.arr[get_num()&3];
                *b &= qual_num(a);
            }

            case 0x08: { // OR
                uint16_t a = get_num();
                uint16_t *b = &regs.arr[get_num()&3];
                *b |= qual_num(a);
            }

            case 0x09: { // XOR
                uint16_t a = get_num();
                uint16_t *b = &regs.arr[get_num()&3];
                *b ^= qual_num(a);
            }

            case 0x0a: { // NOT
                uint16_t *b = &regs.arr[get_num()&3];
                *b = ~*b;
            }

            default:
                fprintf(stderr, "unreachable\n");
                exit(-1);
                break;
        }

#undef IMM_MASK
#undef PTR_MASK
#undef WRD_MASK
#undef get_num
#undef qual_num
#undef reg
    }
}

static void makeTestFile()
{
    FILE* fp = fopen("test.rom", "w+b");
    if (!fp) die("couldn't emit test file");

    uint8_t code[] = {
        0x09, 0x01, 0x00, // MOV $0x01  0x00
        0x0b, 0x01, 0x00, // MOV $0x01, [0x00]
        0x00,             // BRK
    };

    if (!fwrite(code, 1, sizeof code, fp))
    {
        fclose(fp);
        die("couldn't write to test file");
    }

    fclose(fp);
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "usage: " signature " <file.rom>\n");
        return -1;
    }
    
    if (*argv[1] == '-')
    {
        makeTestFile();
        fprintf(stderr, "test file made\n");
        return 0;
    }

    FILE* fp = fopen(argv[1], "rb");
    if (!fp) die("couldn't open rom file");

    interpret(fp);

    return 0;
}

// vim: ts=4 sw=4 et
