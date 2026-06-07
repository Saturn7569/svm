#ifndef STACK_H
#define STACK_H

#include <stdio.h>

#include "types.h"

struct Stack {
    size_t STACK_SIZE; // The size of the stack in memory (in bytes)
    size_t LENGTH; // The actual length of the stack (how many bytes are currently pushed and under the pointer)
    size_t ptr; // Pointer to the top most value on the stack

    byte* stack;
};

// Init function
VoidRet Stack_init(struct Stack* st, size_t al_size);

// Destroy function
void Stack_destroy(struct Stack* st);

// Push function for every value size
VoidRet Stack_pushb(struct Stack* st, byte val);
VoidRet Stack_pushw(struct Stack* st, word val);
VoidRet Stack_pushd(struct Stack* st, dword val);
VoidRet Stack_pushq(struct Stack* st, qword val);

// Pop function for every value size (returns the value popped)
ByteRet Stack_popb(struct Stack* st);
WordRet Stack_popw(struct Stack* st);
DWordRet Stack_popd(struct Stack* st);
QWordRet Stack_popq(struct Stack* st);

// Dup function to duplicate top value
VoidRet Stack_dup(struct Stack* st, byte bytes);
// Bytes sets how many of the top bytes to duplicate so I don't have to
// write a different function for every value

#endif