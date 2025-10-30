# Opcodes

Here is a table of every opcode and what it does:

## Basic opcodes
| Name | Opcode | Operands | Notes |
|----------|----------|----------|----------|
| `NOP` | 0x00 | None | No operation |
| `POP` | 0x01 | None | Pops a value from the stack |
| `DUP` | 0x02 | None | Duplicates the top stack value |
| `ICONST` | 0x10 | `uint32 val` | Loads an integer constant (`val`) to the stack |

## Variables
| Name | Opcode | Operands | Notes |
|----------|----------|----------|----------|
| `MVAR` | 0x20 | None | Allocates space for one variable |
| `STORE` | 0x21 | `uint32 addr` | Pops a value and stores it to a variable at `addr` |
| `LOAD` | 0x22 | `uint32 addr` | Loads a variable from `addr` and pushes it to the stack |

## Arithmetic
| Name | Opcode | Operands | Notes |
|----------|----------|----------|----------|
| `ADD` | 0x30 | None | Pops 2 values, adds them and pushes them (the first operand goes **first** to the stack) |
| `SUB` | 0x31 | None | Pops 2 values, subtracts them and pushes them (the first operand goes **first** to the stack) |
| `MUL` | 0x32 | None | Pops 2 values, multiplies them and pushes them (the first operand goes **first** to the stack) |
| `MOD` | 0x34 | None | Pops 2 values and pushes their modulo (the first operand goes **first** to the stack) |

## Code Jumping
| Name | Opcode | Operands | Notes |
|----------|----------|----------|----------|
| `JMP` | 0x40 | `uint32 addr` | Jumps to the address `addr` |
| `JZ` | 0x41 | `uint32 addr` | Pops a value and jumps to the address `addr` if the value is 0 |
| `JNZ` | 0x42 | `uint32 addr` | Pops a value and jumps to the address `addr` if the value is not 0 |
| `CALL` | 0x43 | `uint32 addr` | Jumps to the address `addr` and saves the call address |
| `RET` | 0x44 | `uint32 addr` | Returns from a call (`CALL` instruction) |

## Logic
| Name | Opcode | Operands | Notes |
|----------|----------|----------|----------|
| `EQ` | 0x50 | None | Pops 2 values and pushes their comparison (pushes 0 if the 2 values are equal) |
| `NEQ` | 0x51 | None | Pops 2 values and pushes their comparison (pushes 0 if the 2 values are not equal) |
| `LT` | 0x52 | None | Pops 2 values and pushes their comparison (pushes 0 if the first value is less than the second one) |
| `GT` | 0x53 | None | Pops 2 values and pushes their comparison (pushes 0 if the first value is greater than the second one) |
| `GTE` | 0x54 | None | Pops 2 values and pushes their comparison (pushes 0 if the first value is greater or equal to the second one) |
| `LTE` | 0x55 | None | Pops 2 values and pushes their comparison (pushes 0 if the first value is less than or equal to the second one) |

## Miscellaneous
| Name | Opcode | Operands | Notes |
|----------|----------|----------|----------|
| `HLT` | 0x60 | None | Ends the program and exits |

## Debug
| Name | Opcode | Operands | Notes |
|----------|----------|----------|----------|
| `DPRINT` | 0xF0 | None | Prints debug info on the top stack value |
| `DCHAR` | 0xF1 | None | Prints the top stack value as a character |