#ifndef __COMMON_ALLOC_H
#define __COMMON_ALLOC_H

/* Alloc.h -- Memory allocation functions
2008-03-13
Igor Pavlov
Public domain */

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

void *MyAlloc(size_t size);
void MyFree(void *address);

#ifdef _WIN32

void SetLargePageSize();

void *MidAlloc(size_t size);
void MidFree(void *address);
void *BigAlloc(size_t size);
void BigFree(void *address);

#else

#define MidAlloc(size) MyAlloc(size)
#define MidFree(address) MyFree(address)
#define BigAlloc(size) MyAlloc(size)
#define BigFree(address) MyFree(address)

#endif

#ifdef __cplusplus
}
#endif

#endif
