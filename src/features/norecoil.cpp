#include "features/norecoil.h"

#include "sdk/interfaces.h"
#include "sdk/classes/C_BasePlayer.h"
#include "sdk/classes/CUserCmd.h"

#include<cmath>
#include <iostream>

namespace NoRecoil
{
	void normalise(QAngle& angle)
	{
		while (angle.x > 89.f)
			angle.x -= 180.f;

		while (angle.x < -89.f)
			angle.x += 180.f;

		while (angle.y > 180.f)
			angle.y -= 360.f;

		while (angle.y < -180.f)
			angle.y += 360.f;
	}

	void OnCreateMove(CUserCmd* cmd)
	{
		// when we shoot
		if (cmd->buttons & IN_ATTACK)
		{
			auto localPlayer = *g_LocalPlayer;

			Vector oldPunch = { 0, 0, 0 };
			int oldShotCount = 0;

			auto& playerPos = localPlayer->m_vecOrigin();

			int shotCount = localPlayer->m_iShotsFired();

			if (shotCount >= 1) {
#if DEBUG
				printf_s("Shot count: %d\n", shotCount);
#endif
				auto& playerPunchAngle = localPlayer->m_aimPunchAngle();
#if DEBUG
				printf_s("Punch X: %f | Punch Y: %f\n", playerPunchAngle.x, playerPunchAngle.y);
#endif
				QAngle viewAngles = cmd->viewangles;

				Vector rcsAngle{};
				rcsAngle.x = viewAngles.x + (oldPunch.x - playerPunchAngle.x * 2.f);
				rcsAngle.y = viewAngles.y + (oldPunch.y - playerPunchAngle.y * 2.f);

				normalise(viewAngles);

				oldPunch.y = playerPunchAngle.y * 2.f;
				oldPunch.x = playerPunchAngle.x * 2.f;
				viewAngles.x = rcsAngle.x;
				viewAngles.y = rcsAngle.y;
				cmd->viewangles = viewAngles;
				oldShotCount = shotCount;
			}
			else {
				oldPunch = { 0, 0, 0 };
				oldShotCount = 0;
			}
		}
	}
}
