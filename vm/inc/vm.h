#ifndef VM_H
#define VM_H

#include <inttypes.h>

#include "stack.h"

#define STACK_SIZE 16
#define LOCALS_SIZE 256

#define ERR_VM_NOERR 0
#define ERR_VM_EOF 1
#define ERR_INVALID_OPCODE 2
#define ERR_INVALID_OPERANDS 3
#define ERR_OPCODE_EXEC 4

typedef struct {
    struct Stack stack;

    uint8_t* code;
    size_t programSize;
    size_t pc;

    uint32_t errorLevel;

    uint8_t vmRunning;
} VM;

uint32_t VM_init(VM*, uint8_t*, size_t);

uint32_t VM_next(VM*);

#endif