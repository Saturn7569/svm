#ifndef FLOAD_H
#define FLOAD_H

#include <stdio.h>
#include <inttypes.h>

#define F_READ_BUFSIZE 512

struct Program {
    uint8_t* code;

    size_t PROGRAM_SIZE;
};

struct Program loadFile(FILE*);

#endif