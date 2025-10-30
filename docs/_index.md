# SVM DOCS

Welcome to the svm documentation. It is currently very minimal and does not include much content. To be extended in the future.

## What is SVM?

SVM is a java-like virtual machine to execute simple code.

## Why?

SVM lets you run code on any machine without having to recompile for specific platforms.

## Structure

An SVM program is structured as simple bytecode. The bytecode consists of an opcode (the instruction name if you will) and some optional operands. Each opcode can have a different ammount of operands that will get decoded by the virtual machine.

Every operand is a number. Since the code is broken up into individual bytes, any number with more than 8 bits should be encoded in **little endian** (least significant byte *first*).

[Here](opcodes.md) is the list of every opcode and how it works

## Ir compiler

SVM comes with a python script that can compile assembly-like code to bytecode (see [ir compiler](ir_compiler.md) docs).