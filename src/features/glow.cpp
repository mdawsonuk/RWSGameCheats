#include "features/glow.h"

#include "sdk/interfaces.h"
#include "sdk/classes/C_BasePlayer.h"
#include "sdk/classes/CUserCmd.h"
#include "gui/guiControl.h"

namespace Glow
{

	void OnFrameStageNotify()
	{
		if (!isGlow) {
			return;
		}

		for (int i = 0; i < g_GlowManager->m_GlowObjectDefinitions.m_Size; i++) {
			GlowObjectDefinition_t& glowObject = g_GlowManager->m_GlowObjectDefinitions[i];

			if (glowObject.m_nNextFreeSlot != ENTRY_IN_USE)
				continue;

			auto localPlayer = *g_LocalPlayer;

			if (!localPlayer)
				continue;

			C_BaseEntity* entity = (C_BaseEntity*)glowObject.m_pEntity;

			if (entity->m_iTeamNum() == localPlayer->m_iTeamNum()) {
				glowObject.m_vGlowColor.x = 0.f;
				glowObject.m_vGlowColor.y = 1.f;
				glowObject.m_vGlowColor.z = 0.f;
			}
			else if (entity->m_iTeamNum() != 0) {
				glowObject.m_vGlowColor.x = 1.f;
				glowObject.m_vGlowColor.y = 0.f;
				glowObject.m_vGlowColor.z = 0.f;
			}
			else {
				glowObject.m_vGlowColor.x = 0.f;
				glowObject.m_vGlowColor.y = 0.f;
				glowObject.m_vGlowColor.z = 1.f;
			}

			glowObject.m_flAlpha = 1.f;
			glowObject.m_bRenderWhenOccluded = true;
			glowObject.m_bRenderWhenUnoccluded = true;
			glowObject.m_bFullBloomRender = false;
		}
	}
}
