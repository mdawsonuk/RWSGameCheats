#pragma once

#include <vadefs.h>

typedef unsigned char uint8;
typedef unsigned char BYTE;
typedef unsigned char byte;

typedef __int32 int32;
typedef unsigned __int32 uint32;
typedef __int64 int64;
typedef unsigned __int64 uint64;
typedef void* (*CreateInterfaceFn)(const char* name, int* returnCode);
typedef int qboolean;
typedef uintptr_t uintp;