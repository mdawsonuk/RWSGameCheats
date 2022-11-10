#include "features/glow.h"

#include "sdk/interfaces.h"
#include "sdk/classes/C_BasePlayer.h"
#include "sdk/classes/CUserCmd.h"

namespace Glow
{

	void OnFrameStageNotify()
	{

        auto glowManager = *g_GlowManager;

        //printf_s("m_Size: %d\n", glowManager->m_GlowObjectDefinitions.m_Size);

        for (int i = 0; i < glowManager->m_GlowObjectDefinitions.m_Size; i++) {
            GlowObjectDefinition_t& glowObject = glowManager->m_GlowObjectDefinitions[i];

            //printf_s("m_nNextFreeSlot: %d\n", glowObject.m_nNextFreeSlot);

            //             THIS VVVVVV is always -1 but should by -2 to use
            if (glowObject.m_nNextFreeSlot != ENTRY_IN_USE)
                continue;

            C_BaseEntity* entity = glowObject.m_pEntity;

            //printf_s("Entity Pos: (%.2f, %.2f, %.2f)\n", entity->m_vecOrigin().x, entity->m_vecOrigin().y, entity->m_vecOrigin().z);

            // for now, just glow everything green

            //printf_s("m_vGlowColor: (%.2f, %.2f, %.2f)\n", glowObject.m_vGlowColor.x, glowObject.m_vGlowColor.y, glowObject.m_vGlowColor.z);
            glowObject.m_vGlowColor = Vector(0.0f, 1.0f, 0.0f);
            //printf_s("m_flGlowAlpha: %.2f\n", glowObject.m_flGlowAlpha);
            glowObject.m_flGlowAlpha = 0.5f;
            glowObject.m_bRenderWhenOccluded = true;
            //printf_s("m_flBloomAmount: %.2f\n\n\n", glowObject.m_flGlowAlphaMax);
            glowObject.m_flGlowAlphaMax = 1.0f;

        }
	}
}
