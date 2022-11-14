#include "features/difficulty.h"

#include "sdk/interfaces.h"
#include "sdk/netvars.h"
#include "sdk/classes/C_BasePlayer.h"

#include <stdio.h>
#include <typeinfo>

namespace Difficulty
{
	void OnRoundStart() {

		int localPlayerIndex = g_EngineClient->GetLocalPlayer();
		printf_s("Local Player Index: %d\n", localPlayerIndex);

		auto csPlayerResource = *g_CSPlayerResource;

		printf_s("Local Player Kills: %d\n", csPlayerResource->m_iKills()[localPlayerIndex]);
		printf_s("Local Player Assists: %d\n", csPlayerResource->m_iAssists()[localPlayerIndex]);
		printf_s("Local Player Deaths: %d\n", csPlayerResource->m_iDeaths()[localPlayerIndex]);
	}
}