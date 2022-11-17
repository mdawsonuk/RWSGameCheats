#include "features/adaptivedifficulty.h"

#include "sdk/interfaces.h"
#include "sdk/netvars.h"
#include "sdk/classes/C_BasePlayer.h"

#include <stdio.h>
#include <typeinfo>

EDifficultyModifiers difficultyModifiers = NONE;

namespace AdaptiveDifficulty
{
	int lastLocalPlayerIndex = -1;

	int lastPlayerKills   = 0;
	int lastPlayerAssists = 0;
	int lastPlayerDeaths  = 0;

	void AdaptDifficulty()
	{
		if (!isAdaptive) {
			// Ensure we don't retain negative "cheats"
			isNegativeBhop = false;
			isPermFlash = false;
			return;
		}

		int localPlayerIndex = g_EngineClient->GetLocalPlayer();

		if (localPlayerIndex != lastLocalPlayerIndex) {
#ifdef _DEBUG
			printf_s("Local Player Index: %d\n", localPlayerIndex);
#endif
			lastLocalPlayerIndex = localPlayerIndex;
		}

		auto csPlayerResource = *g_CSPlayerResource;

		int playerKills = csPlayerResource->m_iKills()[localPlayerIndex];
		int playerAssists = csPlayerResource->m_iAssists()[localPlayerIndex];
		int playerDeaths = csPlayerResource->m_iDeaths()[localPlayerIndex];

		if (playerKills != lastPlayerKills) {
#ifdef _DEBUG
			printf_s("Local Player Kills: %d\n", playerKills);
#endif
			lastPlayerKills = playerKills;
		}

		if (playerAssists != lastPlayerAssists) {
#ifdef _DEBUG
			printf_s("Local Player Assists: %d\n", playerAssists);
#endif
			lastPlayerAssists = playerAssists;
		}

		if (playerDeaths != lastPlayerDeaths) {
#ifdef _DEBUG
			printf_s("Local Player Deaths: %d\n", playerDeaths);
#endif
			lastPlayerDeaths = playerDeaths;
		}

		float currentPlayerScore = playerKills + playerAssists * 0.5f - playerDeaths * 2.5f;

		bool lastIsBhop = isBhop;
		bool lastIsNegativeBhop = isNegativeBhop;
		bool lastIsChams = isChams;
		bool lastIsGlow = isGlow;
		bool lastIsNoFlash = isNoFlash;
		bool lastIsPermFlash = isPermFlash;
		bool lastIsAimbotAndNoRecoil = isAimbotAndNoRecoil;
		
		isPermFlash = currentPlayerScore >= YES_FLASH_THRESHOLD;
#ifdef _DEBUG
		if (isPermFlash != lastIsPermFlash)
		{
			printf_s("Perm Flash changed: %d\n", isPermFlash);
		}
#endif

		isNegativeBhop = currentPlayerScore >= NO_JUMP_THRESHOLD;
		
#ifdef _DEBUG
		if (isNegativeBhop != lastIsNegativeBhop)
		{
			printf_s("No Jump changed: %d\n", isPermFlash);
		}
#endif

		isChams = currentPlayerScore <= CHAMS_THRESHOLD;
#ifdef _DEBUG
		if (isChams != lastIsChams)
		{
			printf_s("Chams changed: %d\n", isChams);
		}
#endif

		isNoFlash = currentPlayerScore <= NO_FLASH_THRESHOLD;
#ifdef _DEBUG
		if (isNoFlash != lastIsNoFlash)
		{
			printf_s("No flash changed: %d\n", isNoFlash);
		}
#endif

		isGlow = currentPlayerScore <= GLOW_THRESHOLD;
#ifdef _DEBUG
		if (isGlow != lastIsGlow)
		{
			printf_s("Glow changed: %d\n", isNoFlash);
		}
#endif

		isAimbotAndNoRecoil = currentPlayerScore <= AIMBOT_THRESHOLD;
#ifdef _DEBUG
		if (isAimbotAndNoRecoil != lastIsAimbotAndNoRecoil)
		{
			printf_s("Aimbot/No recoil changed: %d\n", isNoFlash);
		}
#endif
	}
}