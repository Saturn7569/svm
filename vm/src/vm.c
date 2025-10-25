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
    printf("PC: %d\tO: %#x\tSP: %d\n", vm->pc, o, vm->stack.ptr);
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

        case ADD: {
            uint* ab = pop_ab(vm);
            if (vm->errorLevel != 0) {
                vm->vmRunning = 0;
                return ERR_OPCODE_EXEC;
            }

            uint res = ab[0] + ab[1];

            if (Stack_push(&vm->stack, res) != 0) {
                vm->vmRunning = 0;
                return ERR_OPCODE_EXEC;
            }

            vm->pc++;
            break;
        }
        case SUB: {
            uint* ab = pop_ab(vm);
            if (vm->errorLevel != 0) {
                vm->vmRunning = 0;
                return ERR_OPCODE_EXEC;
            }

            uint res = ab[0] - ab[1];

            if (Stack_push(&vm->stack, res) != 0) {
                vm->vmRunning = 0;
                return ERR_OPCODE_EXEC;
            }

            vm->pc++;
            break;
        }
        case MUL: {
            uint* ab = pop_ab(vm);
            if (vm->errorLevel != 0) {
                vm->vmRunning = 0;
                return ERR_OPCODE_EXEC;
            }

            uint res = ab[0] * ab[1];

            if (Stack_push(&vm->stack, res) != 0) {
                vm->vmRunning = 0;
                return ERR_OPCODE_EXEC;
            }

            vm->pc++;
            break;
        }
        case MOD: {
            uint* ab = pop_ab(vm);
            if (vm->errorLevel != 0) {
                vm->vmRunning = 0;
                return ERR_OPCODE_EXEC;
            }

            uint res = ab[0] % ab[1];

            if (Stack_push(&vm->stack, res) != 0) {
                vm->vmRunning = 0;
                return ERR_OPCODE_EXEC;
            }

            vm->pc++;
            break;
        }

        case JMP: {
            uint val = read_u32(vm);
            if (vm->errorLevel != 0) {
                vm->vmRunning = 0;
                return ERR_INVALID_OPERANDS;
            }

            if (val >= vm->programSize) {
                vm->vmRunning = 0;
                return ERR_OPCODE_EXEC;
            }

            vm->pc = val;
            break;
        }
        case JZ: {
            long temp = Stack_pop(&vm->stack);
            if (temp < 0) {
                vm->vmRunning = 0;
                return ERR_OPCODE_EXEC;
            }
            uint check = (uint)temp;

            if (check != 0) {
                vm->pc += 5;
                break;
            }

            uint val = read_u32(vm);
            if (vm->errorLevel != 0) {
                vm->vmRunning = 0;
                return ERR_INVALID_OPERANDS;
            }

            if (val >= vm->programSize) {
                vm->vmRunning = 0;
                return ERR_OPCODE_EXEC;
            }

            vm->pc = val;
            break;
        }
        case JNZ: {
            long temp = Stack_pop(&vm->stack);
            if (temp < 0) {
                vm->vmRunning = 0;
                return ERR_OPCODE_EXEC;
            }
            uint check = (uint)temp;

            if (check == 0) {
                vm->pc += 5;
                break;
            }

            uint val = read_u32(vm);
            if (vm->errorLevel != 0) {
                vm->vmRunning = 0;
                return ERR_INVALID_OPERANDS;
            }

            if (val >= vm->programSize) {
                vm->vmRunning = 0;
                return ERR_OPCODE_EXEC;
            }

            vm->pc = val;
            break;
        }

        case EQ: {
            uint* ab = pop_ab(vm);
            if (vm->errorLevel != 0) {
                vm->vmRunning = 0;
                return ERR_OPCODE_EXEC;
            }

            if (Stack_push(&vm->stack, ab[0] == ab[1] ? 0 : 1) != 0) {
                vm->vmRunning = 0;
                return ERR_OPCODE_EXEC;
            }

            vm->pc++;
            break;
        }
        case NEQ: {
            uint* ab = pop_ab(vm);
            if (vm->errorLevel != 0) {
                vm->vmRunning = 0;
                return ERR_OPCODE_EXEC;
            }

            if (Stack_push(&vm->stack, ab[0] != ab[1] ? 0 : 1) != 0) {
                vm->vmRunning = 0;
                return ERR_OPCODE_EXEC;
            }

            vm->pc++;
            break;
        }
        case LT: {
            uint* ab = pop_ab(vm);
            if (vm->errorLevel != 0) {
                vm->vmRunning = 0;
                return ERR_OPCODE_EXEC;
            }

            if (Stack_push(&vm->stack, ab[0] < ab[1] ? 0 : 1) != 0) {
                vm->vmRunning = 0;
                return ERR_OPCODE_EXEC;
            }

            vm->pc++;
            break;
        }
        case GT: {
            uint* ab = pop_ab(vm);
            if (vm->errorLevel != 0) {
                vm->vmRunning = 0;
                return ERR_OPCODE_EXEC;
            }

            if (Stack_push(&vm->stack, ab[0] > ab[1] ? 0 : 1) != 0) {
                vm->vmRunning = 0;
                return ERR_OPCODE_EXEC;
            }

            vm->pc++;
            break;
        }
        case LTE: {
            uint* ab = pop_ab(vm);
            if (vm->errorLevel != 0) {
                vm->vmRunning = 0;
                return ERR_OPCODE_EXEC;
            }

            if (Stack_push(&vm->stack, ab[0] <= ab[1] ? 0 : 1) != 0) {
                vm->vmRunning = 0;
                return ERR_OPCODE_EXEC;
            }

            vm->pc++;
            break;
        }
        case GTE: {
            uint* ab = pop_ab(vm);
            if (vm->errorLevel != 0) {
                vm->vmRunning = 0;
                return ERR_OPCODE_EXEC;
            }

            if (Stack_push(&vm->stack, ab[0] >= ab[1] ? 0 : 1) != 0) {
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