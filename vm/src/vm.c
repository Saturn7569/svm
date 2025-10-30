#include <stdio.h>
#include <stdlib.h>

#include "vm.h"
#include "stack.h"
#include "utils.h"

uint VarSpace_init(struct VarSpace* vs, size_t size) {
    vs->vars = (uint*)malloc(sizeof(uint) * size);
    if (!vs->vars) {
        return 1;
    }

    vs->SPACE = size;
    return 0;
}

uint VarSpace_extend(struct VarSpace* vs, size_t size) {
    size_t newSize = vs->SPACE + size;

    vs->vars = (uint*)realloc(vs->vars, newSize);
    if (!vs->vars) {
        return 1;
    }

    vs->SPACE = newSize;
    return 0;
}

void VarSpace_destroy(struct VarSpace* vs) {
    free(vs->vars);
}

uint32_t VM_init(VM* vm, uint8_t* program, size_t program_size) {
    if (Stack_init(&vm->stack, STACK_SIZE) != 0) {
        goto Stack_fail;
    }
    if (Stack_init(&vm->callStack, STACK_SIZE) != 0) {
        goto CallStack_fail;
    }
    if (VarSpace_init(&vm->vars, VARS_SIZE) != 0) {
        goto VarSpace_fail;
    }

    vm->code = program;
    vm->programSize = program_size;
    vm->pc = 0;

    vm->errorLevel = 0;
    vm->vmRunning = 1;

    return 0;

    //VarSpace_destroy(&vm->vars);
VarSpace_fail:
    Stack_destroy(&vm->stack);
CallStack_fail:
    Stack_destroy(&vm->callStack);
Stack_fail:
    return 1;
}

uint32_t VM_next(VM* vm) {
    if (vm->pc >= vm->programSize) {
        return ERR_VM_EOF;
    }

    Opcode o = vm->code[vm->pc];

    #ifdef DEBUG
    printf("PC: %d\tO: %#x (%d)\tSP: %d\n", vm->pc, o, o, vm->stack.ptr);
    #endif

    // Yandere sim moment
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

        case MVAR: {
            if (VarSpace_extend(&vm->vars, 1) != 0) {
                vm->vmRunning = 0;
                return ERR_OPCODE_EXEC;
            }

            #ifdef DEBUG
            printf("Allocated var space, new space size: %d\n", vm->vars.SPACE);
            #endif

            vm->pc++;
            break;
        }
        case STORE: {
            uint32_t addr = read_u32(vm);
            if (vm->errorLevel > 0) {
                vm->vmRunning = 0;
                return ERR_OPCODE_EXEC;
            }

            if (addr >= vm->vars.SPACE) {
                vm->vmRunning = 0;
                return ERR_INVALID_SPACE;
            }

            uint val = pop_val(vm);
            if (vm->errorLevel > 0) {
                vm->vmRunning = 0;
                return ERR_STACK_UNDERFLOW;
            }

            vm->vars.vars[addr] = val;

            #ifdef DEBUG
            printf("Stored value %d (%#x) to variable address %d (%#x)\n", val, val, addr, addr);
            #endif

            vm->pc++;
            break;
        }
        case LOAD: {
            uint32_t addr = read_u32(vm);
            if (vm->errorLevel > 0) {
                vm->vmRunning = 0;
                return ERR_OPCODE_EXEC;
            }

            if (addr >= vm->vars.SPACE) {
                vm->vmRunning = 0;
                return ERR_INVALID_SPACE;
            }

            uint val = vm->vars.vars[addr];

            if (Stack_push(&vm->stack, val) != 0) {
                vm->vmRunning = 0;
                return ERR_OPCODE_EXEC;
            }

            #ifdef DEBUG
            printf("Loaded value from variable address %d (%#x) -> %d (%#x)\n", addr, addr, val, val);
            #endif

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

            #ifdef DEBUG
            printf("Jumping from %#x (%d) to %#x (%d)\n", vm->pc, vm->pc, val, val);
            #endif
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

            #ifdef DEBUG
            printf("Jumping from %#x (%d) to %#x (%d)\n", vm->pc, vm->pc, val, val);
            #endif
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

            #ifdef DEBUG
            printf("Jumping from %#x (%d) to %#x (%d)\n", vm->pc, vm->pc, val, val);
            #endif
            vm->pc = val;
            break;
        }

        case CALL: {
            if (Stack_push(&vm->callStack, vm->pc + 4) != 0) {
                vm->vmRunning = 0;
                return ERR_OPCODE_EXEC;
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

            #ifdef DEBUG
            printf("Jumping from %#x (%d) to %#x (%d)\n", vm->pc, vm->pc, val, val);
            #endif
            vm->pc = val;
            break;
        }
        case RET: {
            long temp = Stack_pop(&vm->callStack);
            if (temp < 0) {
                vm->vmRunning = 0;
                return ERR_OPCODE_EXEC;
            }
            uint val = (uint)temp;
            if (vm->errorLevel != 0) {
                vm->vmRunning = 0;
                return ERR_INVALID_OPERANDS;
            }

            if (val >= vm->programSize) {
                vm->vmRunning = 0;
                return ERR_OPCODE_EXEC;
            }

            #ifdef DEBUG
            printf("Returning from %#x (%d) to %#x (%d)\n", vm->pc, vm->pc, val, val);
            #endif
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
