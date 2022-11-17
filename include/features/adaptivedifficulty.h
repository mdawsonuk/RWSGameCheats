#pragma once

// All of our thresholds for which cheats/disabilities to enable

#define YES_FLASH_THRESHOLD 10

#define INVISIBLE_ENEMIES_THRESHOLD 5

#define CHAMS_THRESHOLD 0

#define NO_FLASH_THRESHOLD -5

#define AIMBOT_THRESHOLD = -10

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

namespace AdaptiveDifficulty
{
	void AdaptDifficulty();
}