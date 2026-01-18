#pragma once

#ifndef UTILS_H

#define UTILS_H

#define FILE_READ_BUFSIZE 512

#include <inttypes.h>

#include "vm.h"
#include "stack.h"

typedef enum {

    NOP = 0x00,

    POPB = 0x01,
    POPW = 0x02,
    POPD = 0x03,
    POPQ = 0x04,
    DUPB = 0x01,
    DUPW = 0x02,
    DUPD = 0x03,
    DUPQ = 0x04,


    BCONST = 0x10,
    WCONST = 0x11,
    DCONST = 0x12,
    QCONST = 0x13,


    MVAR = 0x20,
    STORE = 0x21,
    LOAD = 0x22,


    BADD = 0x30,
    WADD = 0x31,
    DADD = 0x32,
    QADD = 0x33,

    BSUB = 0x34,
    WSUB = 0x35,
    DSUB = 0x36,
    QSUB = 0x37,

    BMUL = 0x38,
    WMUL = 0x39,
    DMUL = 0x3A,
    QMUL = 0x3B,

    MOD = 0x34,


    JMP = 0x40,
    JZ = 0x41,
    JNZ = 0x42,
    CALL = 0x43,
    RET = 0x44,


    BEQ = 0x50,
    WEQ = 0x51,
    DEQ = 0x52,
    QEQ = 0x53,

    BNEQ = 0x54,
    WNEQ = 0x55,
    DNEQ = 0x56,
    QNEQ = 0x57,

    BLT = 0x58,
    WLT = 0x59,
    DLT = 0x5A,
    QLT = 0x5B,

    BGT = 0x5C,
    WGT = 0x5D,
    DGT = 0x5E,
    QGT = 0x5F,

    BGTE = 0x60,
    WGTE = 0x61,
    DGTE = 0x62,
    QGTE = 0x63,

    BLTE = 0x64,
    WLTE = 0x65,
    DLTE = 0x66,
    QLTE = 0x67,


    HLT = 0x70,


    DPRINT = 0xF0,
    DCHAR = 0xF1

} Opcode;

static uint8_t read_u8(VM* vm) {
    if (vm->programSize - vm->pc < 1) {
        vm->errorLevel = 1;
        return 0;
    }

    uint8_t val = vm->code[vm->pc + 1];

    return val;
}

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

static uint* pop_ab(VM* vm) {
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

static uint pop_val(VM* vm) {
    long temp = Stack_pop(&vm->stack);
    if (temp < 0) {
        vm->errorLevel = 1;
        return 0;
    }

    return (uint)temp;
}

#endif