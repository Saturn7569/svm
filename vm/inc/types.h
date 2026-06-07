#ifndef TYPES_H
#define TYPES_H

// Unsigned types for qol
typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned int dword;
typedef unsigned long qword;

// === RETURN STRUCTS ===
// Return structs consist of 2 values: the error value (0 if no error, >0 if error) and the return value
typedef struct VoidRet { byte err; };

typedef struct ByteRet { byte err; byte ret; };
typedef struct WordRet { byte err; word ret; };
typedef struct DWordRet { byte err; dword ret; };
typedef struct QWordRet { byte err; qword ret; };

#endif