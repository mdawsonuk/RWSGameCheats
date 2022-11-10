#pragma once
#include "../common.h"
#include "Vector.h"

template <class T, class I>
class CUtlMemory {
public:
    T& operator[](int i) {
        return m_pMemory[i];
    };

    T* m_pMemory;
    int m_nAllocationCount;
    int m_nGrowSize;
};
    
template<class T, class A>
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

struct GlowObjectDefinition_t {
    C_BaseEntity* m_pEntity;
    Vector m_vGlowColor;
    float m_flGlowAlpha;

    bool m_bGlowAlphaCappedByRenderAlpha;
    float m_flGlowAlphaFunctionOfMaxVelocity;
    float m_flGlowAlphaMax;
    float m_flGlowPulseOverdrive;
    bool m_bRenderWhenOccluded;
    bool m_bRenderWhenUnoccluded;
    bool m_bFullBloomRender;
    int m_nFullBloomStencilTestValue; // only render full bloom objects if stencil is equal to this value (value of -1 implies no stencil test)
    int m_nRenderStyle;
    int m_nSplitScreenSlot;

    // Linked list of free slots
    int m_nNextFreeSlot;
};

#define END_OF_FREE_LIST -1
#define ENTRY_IN_USE -2

class CGlowObjectManager {
public:
    CUtlVector<GlowObjectDefinition_t> m_GlowObjectDefinitions;
    int m_nFirstFreeSlot;
};