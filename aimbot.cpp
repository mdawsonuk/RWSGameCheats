#include "../aimbot.h"

#include "sdk/interfaces.h"
#include "sdk/classes/C_BasePlayer.h"
#include "sdk/classes/CUserCmd.h"

#include<cmath>

namespace AimBot
{

	void clamp(QAngle& angle) {
		if (angle.x > 89.0)
			angle.x = 89.0;

		if (angle.x < -89.0)
			angle.x = -89.0;

		if (angle.y > 180.0)
			angle.y = 180.0;

		if (angle.y < -180.0)
			angle.y = -180.0;
	}

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

	QAngle AimTo(Vector target) {
		auto localPlayer = *g_LocalPlayer;

		auto& playerPos = localPlayer->m_vecOrigin();

		Vector posDiff = *(new Vector(
			playerPos.x - target.x,
			playerPos.y - target.y,
			playerPos.z - target.z
		));


		double hyp = sqrt(pow(posDiff.x, 2) + pow(posDiff.y, 2));

		// use radian trig to find new angle
		QAngle newAngle = *(new QAngle(
			(float)(atan((posDiff.z, hyp) * 57.295779513082f)),
			(float)(atan(posDiff.y / posDiff.x) * 57.295779513082f),
			0.0f
		));

		if (posDiff.x >= 0.f) {
			newAngle.y += 180.0f;
		}

		// make angles -90<x<90 and -180<y<180
		normalise(newAngle);
		clamp(newAngle);

		return newAngle;
	}

	void OnCreateMove(CUserCmd* cmd)
	{
		// when we shoot
		if (cmd->buttons & IN_ATTACK)
		{
			auto localPlayer = *g_LocalPlayer;

			auto& playerPos = localPlayer->m_vecOrigin();

			auto& playerTeamNum = localPlayer->m_iTeamNum();

			IClientEntityList* entityList = g_EntityList;
		
			float closest_diff = 999999999.f;

			QAngle* nearestEntityAim = NULL;

			auto highestIndex = entityList->GetHighestEntityIndex();

			// find the entity shortest distance away
			for (int i = 0; i <= highestIndex; i++) {
				auto entity = entityList->GetClientEntity(i);

				if (entity == NULL) {
					continue;
				}

				Vector entityPos = entity->GetAbsOrigin();

				// if it's on the same team as player don't bother
				int teamNum = ((C_BaseEntity*)(entity))->m_iTeamNum();
				if (playerTeamNum == teamNum || teamNum == 0) {
					continue;
				}

				// if it's dead then don't bother
				int health = ((C_BasePlayer*)(entity))->m_iHealth();
				if (health <= 0 || health > 100) {
					continue;
				}

				// select entity closest to the view angle of the player
				QAngle viewAngles = cmd->viewangles;
				QAngle newViewAngles = AimTo(entityPos);

				float diff = sqrt(
					pow(newViewAngles.x - viewAngles.x, 2) +
					pow(newViewAngles.y - viewAngles.y, 2)
				);

				if (diff < closest_diff) {
					nearestEntityAim = &newViewAngles;
				}
			}

			if (nearestEntityAim == NULL) {
				return;
			}

			// set player view angle to aim to nearest found
			cmd->viewangles = *nearestEntityAim;
		}
	}
}
