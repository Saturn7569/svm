#include "vm.h"
#include "stack.h"
#include "utils.h"

uint32_t VM_init(VM* vm, uint8_t* program, size_t program_size) {
    if (Stack_init(&vm->stack, STACK_SIZE) != 0) {
        return 1;
    }

    vm->code = program;
    vm->programSize = program_size;
    vm->pc = 0;

    vm->errorLevel = 0;
    vm->vmRunning = 1;

    return 0;
}

uint32_t VM_next(VM* vm) {
    if (vm->pc >= vm->programSize) {
        return ERR_VM_EOF;
    }

    Opcode o = vm->code[vm->pc];
    //printf("PC: %d\tO: %#x\tSP: %d\n", vm->pc, o, vm->stack.ptr);
    switch (o) {
        case NOP:
            vm->pc++;
            break;

        case POP: {
            if (Stack_pop(&vm->stack) < 0) {
                vm->vmRunning = 0;
                return ERR_OPCODE_EXEC;
            }
            vm->pc++;
            break;
        }

        case DUP: {
            if (Stack_dup(&vm->stack) < 0) {
                vm->vmRunning = 0;
                return ERR_OPCODE_EXEC;
            }
            vm->pc++;
            break;
        }

        case ICONST: {
            uint val = read_u32(vm);
            if (vm->errorLevel != 0) {
                vm->vmRunning = 0;
                return ERR_INVALID_OPERANDS;
            }
            if (Stack_push(&vm->stack, val) != 0) {
                vm->vmRunning = 0;
                return ERR_OPCODE_EXEC;
            }
            vm->pc++;
            break;
        }

        case DPRINT: {
            if (vm->stack.ptr > 0) {
                printf("Stack value at %d: %d (%#x)\n", vm->stack.ptr, vm->stack.stack[vm->stack.ptr - 1], vm->stack.stack[vm->stack.ptr - 1]);
            } else {
                puts("SP is 0");
            }
            vm->pc++;
            break;
        }

        case DCHAR: {
            if (vm->stack.ptr > 0) {
                printf("%c", vm->stack.stack[vm->stack.ptr - 1]);
            }
            vm->pc++;
            break;
        }

        case HLT: {
            vm->vmRunning = 0;
            break;
        }

        default:
            vm->vmRunning = 0;
            return ERR_INVALID_OPCODE;
    }

    return ERR_VM_NOERR;
}