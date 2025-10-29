#ifndef VM_H
#define VM_H

#include <inttypes.h>

#include "stack.h"

#define STACK_SIZE 16
#define VARS_SIZE 16

#define ERR_VM_NOERR 0
#define ERR_VM_EOF 1
#define ERR_INVALID_OPCODE 2
#define ERR_INVALID_OPERANDS 3
#define ERR_OPCODE_EXEC 4
#define ERR_INVALID_SPACE 5
#define ERR_STACK_UNDERFLOW 6

struct VarSpace {
    size_t SPACE;
    uint* vars;
};

uint VarSpace_init(struct VarSpace*, size_t);
void VarSpace_destroy(struct VarSpace*);
uint VarSpace_extend(struct VarSpace*, size_t);

typedef struct {
    struct Stack stack;
    struct Stack callStack;

    struct VarSpace vars;

    uint8_t* code;
    size_t programSize;
    size_t pc;

    uint32_t errorLevel;

    uint8_t vmRunning;
} VM;

uint32_t VM_init(VM*, uint8_t*, size_t);

uint32_t VM_next(VM*);

#endif