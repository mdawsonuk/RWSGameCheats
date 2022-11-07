#pragma once

#include <vadefs.h>

typedef unsigned char uint8;
typedef unsigned char BYTE;
typedef unsigned char byte;
typedef unsigned int uint;

typedef __int32 int32;
typedef unsigned __int32 uint32;
typedef __int64 int64;
typedef unsigned __int64 uint64;
typedef void* (*CreateInterfaceFn)(const char* name, int* returnCode);
typedef int qboolean;
typedef uintptr_t uintp;

// TODO: Do we ever need the actual definitions of these?
template<class T, class I = int>
class CUtlMemory;
template<class T, class A = CUtlMemory<T>>
class CUtlVector;
template<class T, int nAlignment = 16>
class CUtlMemoryAligned;