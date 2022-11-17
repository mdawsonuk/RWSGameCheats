#include "features/noflash.h"

#include "sdk/interfaces.h"
#include "sdk/classes/C_BasePlayer.h"
#include "features/adaptivedifficulty.h"

namespace NoFlash
{

	void CheckForFlash()
	{
		if (!isNoFlash) {
			return;
		}

		auto localPlayer = *g_LocalPlayer;

		if (!localPlayer) {
			return;
		}

		if (localPlayer->m_flFlashDuration() > 0.f)
		{
			localPlayer->m_flFlashDuration() = 0.f;
		}
	}
}
