#include "features/adaptivedifficulty.h"

#include "sdk/interfaces.h"
#include "sdk/netvars.h"
#include "sdk/classes/C_BasePlayer.h"

#include <stdio.h>
#include <typeinfo>

namespace AdaptiveDifficulty
{
	int lastLocalPlayerIndex = -1;

	int lastPlayerKills   = 0;
	int lastPlayerAssists = 0;
	int lastPlayerDeaths  = 0;

	void AdaptDifficulty()
	{
		int localPlayerIndex = g_EngineClient->GetLocalPlayer();

		if (localPlayerIndex != lastLocalPlayerIndex) {
			printf_s("Local Player Index: %d\n", localPlayerIndex);
			lastLocalPlayerIndex = localPlayerIndex;
		}

		auto csPlayerResource = *g_CSPlayerResource;

		int playerKills = csPlayerResource->m_iKills()[localPlayerIndex];
		int playerAssists = csPlayerResource->m_iAssists()[localPlayerIndex];
		int playerDeaths = csPlayerResource->m_iDeaths()[localPlayerIndex];

		if (playerKills != lastPlayerKills) {
			printf_s("Local Player Kills: %d\n", playerKills);
			lastPlayerKills = playerKills;
		}

		if (playerAssists != lastPlayerAssists) {
			printf_s("Local Player Assists: %d\n", playerAssists);
			lastPlayerAssists = playerAssists;
		}

		if (playerDeaths != lastPlayerDeaths) {
			printf_s("Local Player Deaths: %d\n", playerDeaths);
			lastPlayerDeaths = playerDeaths;
		}
	}
}