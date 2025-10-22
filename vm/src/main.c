#include <stdio.h>

#include "stack.h"

int main() {
    struct Stack test;

    if (Stack_init(&test, 8) != 0) {
        perror("Failed to initialize the stack");
        return 1;
    }

    if (Stack_push(&test, 5) != 0) {
        perror("Failed to push the number to the stack");
        Stack_destroy(&test);
        return 1;
    }
    if (Stack_push(&test, 69) != 0) {
        perror("Failed to push the number to the stack");
        Stack_destroy(&test);
        return 1;
    }

    printf("Number on the stack: %d\n", Stack_pop(&test));

    printf("Duplicated value: %d\n", Stack_dup(&test));

    Stack_destroy(&test);

    return 0;
}