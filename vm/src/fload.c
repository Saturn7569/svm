#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

#include "fload.h"

struct Program loadFile(FILE* f) {
    char buf[F_READ_BUFSIZE];
    size_t codeSize = 0;
    size_t codeBufSize = F_READ_BUFSIZE;
    uint8_t* code = malloc(codeBufSize);
    if (!code)
        return (struct Program){ NULL, 0 };

    for (;;) {
        size_t bytesRead = fread(buf, 1, F_READ_BUFSIZE, f);
        if (bytesRead == 0)
            break;

        size_t totalByteSize = codeSize + bytesRead;
        if (totalByteSize > codeBufSize) {
            while (totalByteSize > codeBufSize)
                codeBufSize *= 2;

            uint8_t* tmp = realloc(code, codeBufSize);
            if (!tmp) {
                free(code);
                return (struct Program){ NULL, 0 };
            }
            code = tmp;
        }

        memcpy(code + codeSize, buf, bytesRead);
        codeSize += bytesRead;
    }

    return (struct Program){ code, codeSize };
}