#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

#include "fload.h"

struct Program loadFile(FILE* f) {
    char buf[F_READ_BUFSIZE];

    size_t codeSize = 0;

    size_t codeBufSize = F_READ_BUFSIZE;
    uint8_t* code = (uint8_t*)malloc(codeBufSize);
    if (!code) {
        return (struct Program){
            (uint8_t*)NULL,
            (size_t)0,
        };
    }

    size_t bytesRead;
    size_t totalByteSize;

    for (;;) {
        bytesRead = fread(buf, 1, F_READ_BUFSIZE, f);
        if (bytesRead == 0) {
            // EOF
            break;
        }

        totalByteSize = codeSize + bytesRead;
        if (totalByteSize > codeBufSize) {
            codeBufSize += F_READ_BUFSIZE;
            code = (uint8_t*)realloc(code, codeBufSize);
            if (!code) {
                return (struct Program){
                    (uint8_t*)NULL,
                    (size_t)0,
                };
            }
        }

        memcpy(code + codeBufSize, &buf, bytesRead);

        codeSize += bytesRead;
    }

    return (struct Program){
        code,
        codeSize,
    };
}