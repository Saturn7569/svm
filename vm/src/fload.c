#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "fload.h"

struct Program loadFile(FILE* f) {
    char buf[F_READ_BUFSIZE];
    size_t codeSize;

    size_t codeBufSize = F_READ_BUFSIZE;
    uint8_t* code = (uint8_t*)malloc(F_READ_BUFSIZE);
}