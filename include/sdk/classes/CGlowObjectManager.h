#pragma once
#include "../common.h"
#include "Vector.h"

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