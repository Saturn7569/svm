#ifndef STACK_H
#define STACK_H

#include <stdio.h>

typedef unsigned int uint;

struct Stack {
    size_t STACK_SIZE;
    size_t VAL_SIZE;
    size_t ptr;

    uint* stack;
};

const uint Stack_init(struct Stack*, size_t);

const uint Stack_push(struct Stack*, uint);
const long Stack_pop(struct Stack*);

const long Stack_dup(struct Stack*);

#endif