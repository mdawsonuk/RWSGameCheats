#include "features/noflash.h"

#include "sdk/interfaces.h"
#include "sdk/classes/C_BasePlayer.h"

namespace NoFlash
{

	void CheckForFlash()
	{
		// TODO: Have a GUI for enabling/disabling cheats

		auto localPlayer = *g_LocalPlayer;

		if (localPlayer->m_flFlashDuration() > 0.f)
		{
			localPlayer->m_flFlashDuration() = 0.f;
		}
	}
}
