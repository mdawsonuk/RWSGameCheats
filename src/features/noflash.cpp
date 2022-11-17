#include "features/noflash.h"

#include "sdk/interfaces.h"
#include "sdk/classes/C_BasePlayer.h"
#include "features/adaptivedifficulty.h"

namespace NoFlash
{

	void CheckForFlash()
	{
		auto localPlayer = *g_LocalPlayer;

		if (!localPlayer) {
			return;
		}

		if (isNoFlash)
		{
			if (localPlayer->m_flFlashDuration() > 0.0f)
			{
				localPlayer->m_flFlashDuration() = 0.0f;
			}
		}
		else if (isPermFlash)
		{
			if (localPlayer->m_flFlashDuration() != 1.0f)
			{
				localPlayer->m_flFlashDuration() = 1.0f;
			}
		}
	}
}
