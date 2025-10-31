# IR compiler

The ir compiler is a python script that converts assembly-like code to bytecode instructions.

## How it works

You write a file (ending in `.sir`) with the ir code and then run the script like so:

```
python ir.py <file.sir>
```

This will generate a `.svm` file with the compiled bytecode.

## How to write SVM assembly

The compiler works like an assembler. It counts the file size and generates the proper bytecode. You can use the names from the [opcodes table](opcodes.md).

Here is an example that prints "hi" to the console:

```sir
; Create comments with the ; character

; We will print to the console using the built-in DCHAR instruction

ICONST 'h' ; Notice I can enter characters and the compiler will convert them to numbers correctly
DCHAR ; Print the letter

ICONST 'i'
DCHAR

ICONST 0x0a ; The newline character (10 in ascii)
DCHAR

; End the program
HLT
```

### Labels

The SVM IR compiler supports labels.

At compile-time it will figure out the actual label position in the bytecode and insert it properly so you don't have to count bytes manually

Here is an example that counts to 10

```sir
; Count to 10
; We will use the DPRINT instruction to see the top number on the stack

; Load the value
ICONST 0

; Define the loop label
::loop
; Push 1 and add it to the counter on the stack
ICONST 1
ADD

; The stack now contains the counter with 1 added

DPRINT

; Duplicate the value so we don't lose it
DUP

; Compare it to see if it is 10
ICONST 10
EQ

; If the value is not 0 (its not 10) then jump back to the loop
JNZ ::loop

HLT
```

### Macros

Macros are so far very minimal, they can only store integers that will be autofilled during compile-time.

Here is an example with macros:

```sir
; Define a macro
#define count 10

; Push the number
ICONST @count ; The macro will expand to the number 10
DPRINT ; Will print 10
```