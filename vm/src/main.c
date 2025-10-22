#include <stdio.h>
#include <inttypes.h>

#include "stack.h"
#include "vm.h"
#include "utils.h"

int main() {
    VM vm;
    uint8_t example[] = {
        0x10, 0x05, 0x00, 0x00, 0x00, // ICONST 0x5
        0xF0, // DPRINT
        0x10, 0x43, 0x00, 0x00, 0x00, // ICONST 0x43 (6 7)
        0xF0, // DPRINT
        0x01, // POP
        0xF0, // DPRINT
        0x02,
        0xF0, // DPRINT
    };

    VM_init(&vm, example, 16);

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