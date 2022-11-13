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
    int m_nNextFreeSlot;
    IClientEntity* m_pEntity;
    Vector m_vGlowColor;
    float m_flAlpha;
    bool m_bGlowAlphaCappedByRenderAlpha;
    float m_flGlowAlphaFunctionOfMaxVelocity;
    float m_flGlowAlphaMax;
    float m_flGlowPulseOverdrive;
    bool m_bRenderWhenOccluded;
    bool m_bRenderWhenUnoccluded;
    bool m_bFullBloomRender;
    int m_nFullBloomStencilTestValue;
    int m_nGlowStyle;
    int m_nSplitScreenSlot;
};

#define END_OF_FREE_LIST -1
#define ENTRY_IN_USE -2

class CGlowObjectManager {
public:
    CUtlVector<GlowObjectDefinition_t> m_GlowObjectDefinitions;
    int m_nFirstFreeSlot;
};