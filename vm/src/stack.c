#include "stack.h"

#include <stdio.h>
#include <stdlib.h>

uint Stack_init(struct Stack* st, size_t mal_elements) {
    if (mal_elements < 1)
        return 1;

    st->STACK_SIZE = mal_elements;
    st->VAL_SIZE = sizeof(uint);

    st->ptr = 0;

    const size_t mallocSize = mal_elements * st->VAL_SIZE;

    st->stack = (uint*)malloc(mallocSize);
    if (!st->stack) {
        return 1;
    }

    return 0;
}

void Stack_destroy(struct Stack* st) {
    free(st->stack);
}

uint Stack_push(struct Stack* st, uint val) {
    if (st->STACK_SIZE <= st->ptr) {
        st->STACK_SIZE *= 2;

        st->stack = (uint*)realloc(st->stack, st->STACK_SIZE * st->VAL_SIZE);
        if (!st->stack) {
            return 1;
        }
    }

    st->stack[st->ptr] = val;
    st->ptr++;

    return 0;
}

long Stack_pop(struct Stack* st) {
    if (st->ptr == 0) {
        return -1;
    }

    st->ptr--;

    return (long)st->stack[st->ptr];
}

long Stack_dup(struct Stack* st) {
    if (st->ptr == 0) {
        return -1;
    }

    uint val = st->stack[st->ptr - 1];

    if (Stack_push(st, val) != 0) {
        return -1;
    }

    return val;
}