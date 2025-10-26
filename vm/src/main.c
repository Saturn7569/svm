#include <stdio.h>
#include <inttypes.h>

#include "stack.h"
#include "vm.h"
#include "utils.h"
#include "fload.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: %s <file>\n", argv[0]);
        return 1;
    }

    FILE* f = fopen(argv[1], "r+");
    if (!f) {
        perror("Error while opening file");
        return 1;
    }

    struct Program code = loadFile(f);
    if (!code.code) {
        printf("Error while loading file %s\n", argv[1]);
        fclose(f);
        return 1;
    }

    #ifdef DEBUG
    printf("Loaded code of size %o at %p\n", code.PROGRAM_SIZE, code.code);
    for (int i = 0; i < code.PROGRAM_SIZE; i++) {
        printf("%#x(%o) ", code.code[i], code.code[i]);
    }
    printf("\n");
    #endif

    fclose(f);

    VM vm;

    VM_init(&vm, code.code, code.PROGRAM_SIZE);

    while (vm.vmRunning > 0) {
        switch (VM_next(&vm)) {
            case 0:
                break;
            case ERR_VM_EOF:
                vm.vmRunning = 0;
                break;
            case ERR_INVALID_OPCODE:
                puts("Invalid opcode reached");
                break;
            case ERR_INVALID_OPERANDS:
                puts("Invalid operands parsed");
                break;
            case ERR_OPCODE_EXEC:
                puts("Error while executing opcodes");
                break;
            default:
                puts("An error occured.");
                break;
        }
    }

    return 0;
}