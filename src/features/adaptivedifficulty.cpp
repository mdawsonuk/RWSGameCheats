#include "features/adaptivedifficulty.h"
#include "features/settings.h"

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
		if (!Settings::isAdaptive) {
			// Ensure we don't retain negative "cheats"
			Settings::isNegativeBhop = false;
			Settings::isPermFlash = false;
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

		if (!csPlayerResource) {
			return;
		}

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

		bool lastIsBhop = Settings::isBhop;
		bool lastIsNegativeBhop = Settings::isNegativeBhop;
		bool lastIsChams = Settings::isChams;
		bool lastIsGlow = Settings::isGlow;
		bool lastIsNoFlash = Settings::isNoFlash;
		bool lastIsPermFlash = Settings::isPermFlash;
		bool lastIsAimbotAndNoRecoil = Settings::isAimbotAndNoRecoil;
		
		Settings::isPermFlash = currentPlayerScore >= YES_FLASH_THRESHOLD;
#ifdef _DEBUG
		if (Settings::isPermFlash != lastIsPermFlash)
		{
			printf_s("Perm Flash changed: %d\n", Settings::isPermFlash);
		}
#endif

		Settings::isNegativeBhop = currentPlayerScore >= NO_JUMP_THRESHOLD;
#ifdef _DEBUG
		if (Settings::isNegativeBhop != lastIsNegativeBhop)
		{
			printf_s("No Jump changed: %d\n", Settings::isNegativeBhop);
		}
#endif

		Settings::isBhop = currentPlayerScore <= BHOP_THRESHOLD;
#ifdef _DEBUG
		if (Settings::isBhop != lastIsBhop)
		{
			printf_s("Bhop changed: %d\n", Settings::isBhop);
		}
#endif

		Settings::isChams = currentPlayerScore <= CHAMS_THRESHOLD;
#ifdef _DEBUG
		if (Settings::isChams != lastIsChams)
		{
			printf_s("Chams changed: %d\n", Settings::isChams);
		}
#endif

		Settings::isNoFlash = currentPlayerScore <= NO_FLASH_THRESHOLD;
#ifdef _DEBUG
		if (Settings::isNoFlash != lastIsNoFlash)
		{
			printf_s("No flash changed: %d\n", Settings::isNoFlash);
		}
#endif

		Settings::isGlow = currentPlayerScore <= GLOW_THRESHOLD;
#ifdef _DEBUG
		if (Settings::isGlow != lastIsGlow)
		{
			printf_s("Glow changed: %d\n", Settings::isNoFlash);
		}
#endif

		Settings::isAimbotAndNoRecoil = currentPlayerScore <= AIMBOT_THRESHOLD;
#ifdef _DEBUG
		if (Settings::isAimbotAndNoRecoil != lastIsAimbotAndNoRecoil)
		{
			printf_s("Aimbot/No recoil changed: %d\n", Settings::isNoFlash);
		}
#endif
	}
}