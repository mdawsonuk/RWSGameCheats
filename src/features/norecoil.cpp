#include "features/norecoil.h"

#include "sdk/interfaces.h"
#include "sdk/classes/C_BasePlayer.h"
#include "sdk/classes/CUserCmd.h"
#include "features/settings.h"

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
		if (!Settings::isAimbotAndNoRecoil) {
			return;
		}

		// when we shoot
		if (cmd->buttons & IN_ATTACK)
		{
			auto localPlayer = *g_LocalPlayer;

			Vector oldPunch = { 0, 0, 0 };
			int oldShotCount = 0;

			auto& playerPos = localPlayer->m_vecOrigin();

			int shotCount = localPlayer->m_iShotsFired();

			if (shotCount >= 1) {
				auto& playerPunchAngle = localPlayer->m_aimPunchAngle();
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
