#pragma once

#ifndef UTILS_H

#define UTILS_H

#define FILE_READ_BUFSIZE 512

#include <inttypes.h>

#include "vm.h"
#include "stack.h"

typedef enum {

    NOP = 0x00,
    POP = 0x01,
    DUP = 0x02,

    ICONST = 0x10,

    MVAR = 0x20,
    STORE = 0x21,
    LOAD = 0x22,

    ADD = 0x30,
    SUB = 0x31,
    MUL = 0x32,
    DIV = 0x33,
    MOD = 0x34,

    JMP = 0x40,
    JZ = 0x41,
    JNZ = 0x42,
    CALL = 0x43,
    RET = 0x44,

    EQ = 0x50,
    NEQ = 0x51,
    LT = 0x52,
    GT = 0x53,
    GTE = 0x54,
    LTE = 0x55,

    HLT = 0x60,


    DPRINT = 0xF0,
    DCHAR = 0xF1,

} Opcode;

static uint16_t read_u16(VM* vm) {
    if (vm->programSize - vm->pc < 2) {
        vm->errorLevel = 1;
        return 0;
    }

    uint16_t val = vm->code[vm->pc + 1] | (vm->code[vm->pc + 2] << 8);

    vm->pc += 2;

    return val;
}

static uint32_t read_u32(VM* vm) {
    if (vm->programSize - vm->pc < 4) {
        vm->errorLevel = 1;
        return 0;
    }

    uint32_t val = vm->code[vm->pc + 1] | (vm->code[vm->pc + 2] << 8) | (vm->code[vm->pc + 3] << 16) | (vm->code[vm->pc + 4] << 24);

    vm->pc += 4;

    return val;
}

static uint32_t* pop_ab(VM* vm) {
    static uint ab[2];
    long temp;

    temp = Stack_pop(&vm->stack);
    if (temp < 0) {
        vm->errorLevel = 2;
        return ab;
    }
    ab[1] = (uint)temp;

    temp = Stack_pop(&vm->stack);
    if (temp < 0) {
        vm->errorLevel = 2;
        return ab;
    }
    ab[0] = (uint)temp;

    return ab;
}

#endif