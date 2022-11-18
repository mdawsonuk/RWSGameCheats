#include "features/noflash.h"

#include "sdk/interfaces.h"
#include "sdk/classes/C_BasePlayer.h"
#include "features/settings.h"

namespace NoFlash
{

	void CheckForFlash()
	{
		auto localPlayer = *g_LocalPlayer;

		if (!localPlayer) {
			return;
		}

		if (Settings::isNoFlash)
		{
			if (localPlayer->m_flFlashDuration() > 0.0f)
			{
				localPlayer->m_flFlashDuration() = 0.0f;
			}
		}
	}
}
