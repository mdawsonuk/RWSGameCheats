#pragma once

// All of our thresholds for which cheats/disabilities to enable

#define YES_FLASH_THRESHOLD 7

#define NO_JUMP_THRESHOLD 5

#define INVISIBLE_ENEMIES_THRESHOLD 3

#define CHAMS_THRESHOLD 0

#define GLOW_THRESHOLD -1

#define NO_FLASH_THRESHOLD -2

#define AIMBOT_THRESHOLD -5

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

extern EDifficultyModifiers difficultyModifiers;

extern bool isBhop;
extern bool isNegativeBhop;
extern bool isChams;
extern bool isGlow;
extern bool isNoFlash;
extern bool isPermFlash;
extern bool isAimbotAndNoRecoil;
extern bool isAdaptive;

namespace AdaptiveDifficulty
{
	void AdaptDifficulty();
}