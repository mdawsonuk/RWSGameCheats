#pragma once

// All of our thresholds for which cheats/disabilities to enable

#define YES_FLASH_THRESHOLD 7

#define NO_JUMP_THRESHOLD 5

#define INVISIBLE_ENEMIES_THRESHOLD 3

#define BHOP_THRESHOLD 2

#define CHAMS_THRESHOLD 0

#define GLOW_THRESHOLD -1

#define NO_FLASH_THRESHOLD -2

#define AIMBOT_THRESHOLD -5

namespace AdaptiveDifficulty
{
	void AdaptDifficulty();
}