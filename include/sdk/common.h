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

template <class T, class I = int>
class CUtlMemory {
public:
    T& operator[](int i) {
        return m_pMemory[i];
    };

    T* m_pMemory;
    int m_nAllocationCount;
    int m_nGrowSize;
};

template<class T, class A = CUtlMemory<T>>
class CUtlVector {
public:
    typedef A CAllocator;

    T& operator[](int i) {
        return m_Memory[i];
    };

    CAllocator m_Memory;
    int m_Size;
    T* m_pElements;
};
template<class T, int nAlignment = 16>
class CUtlMemoryAligned;