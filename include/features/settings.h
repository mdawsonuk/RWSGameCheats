#pragma once

enum EDifficultyModifiers
{
	NONE,
	YES_FLASH,
	INVISIBLE_ENEMIES,
	CHAMS,
	NO_FLASH,
	BHOP,
	NO_JUMP,
	AIMBOT,
};

namespace Settings
{
	extern EDifficultyModifiers difficultyModifiers;

	extern bool isBhop;
	extern bool isNegativeBhop;
	extern bool isChams;
	extern bool isGlow;
	extern bool isNoFlash;
	extern bool isPermFlash;
	extern bool isAimbotAndNoRecoil;
	extern bool isAdaptive;
}
