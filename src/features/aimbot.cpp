#include "features/aimbot.h"

#include "sdk/interfaces.h"
#include "sdk/classes/C_BasePlayer.h"
#include "sdk/classes/CUserCmd.h"

#include "features/adaptivedifficulty.h"

#include<cmath>
#include <iostream>

#define HEAD_BONE_ID 8
#define DEGREES_IN_RADIAN 57.295779513082f

namespace AimBot
{

	void clamp(QAngle& angle)
	{
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

	Vector GetBonePosition(C_BaseEntity* pEntity, int boneId)
	{
		matrix3x4a_t mMatrixArray[128];
		pEntity->SetupBones(mMatrixArray, 128, 256, 0);
		matrix3x4a_t mHitboxMatrix = mMatrixArray[boneId];
		return Vector(mHitboxMatrix.m_flMatVal[0][3], mHitboxMatrix.m_flMatVal[1][3], mHitboxMatrix.m_flMatVal[2][3]);
	}

	QAngle AimTo(Vector target)
	{
		Vector playerHead = GetBonePosition(*g_LocalPlayer, HEAD_BONE_ID);

		Vector posDiff = Vector(
			playerHead.x - target.x,
			playerHead.y - target.y,
			playerHead.z - target.z
		);

		// use radian trig to find new angle
		QAngle newAngle = QAngle(
			asinf(posDiff.z / sqrt(pow(posDiff.x, 2) + pow(posDiff.y, 2) + pow(posDiff.z, 2))) * DEGREES_IN_RADIAN,
			atanf(posDiff.y / posDiff.x) * DEGREES_IN_RADIAN,
			0.0f
		);

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
		if (!isAimbotAndNoRecoil) {
			return;
		}

		// when we shoot
		if (cmd->buttons & IN_ATTACK)
		{
			auto localPlayer = *g_LocalPlayer;

			double closestDiff = 999999999.f;

			QAngle nearestEntityAim;
			bool botFound = FALSE;

			auto highestIndex = g_EntityList->GetHighestEntityIndex();

			// find the entity shortest distance away
			for (int i = 0; i <= highestIndex; i++) {

				if (g_EntityList->GetClientEntity(i) == NULL) {
					continue;
				}

				C_BaseEntity* entity = (C_BaseEntity*) g_EntityList->GetClientEntity(i);

				// Only aim at players
				if (entity->GetClientClass()->m_ClassId != CCSPlayer)
				{
					continue;
				}

				// if it's on the same team as player don't bother
				int teamNum = entity->m_iTeamNum();
				if (localPlayer->m_iTeamNum() == teamNum || teamNum == 0) {
					continue;
				}

				// if it's dead then don't bother
				if (((C_BasePlayer*) entity)->IsAlive() == false) {
					continue;
				}

				// select entity closest to the view angle of the player
				Vector entityHead = GetBonePosition(entity, HEAD_BONE_ID);
				QAngle viewAngles = cmd->viewangles;
				QAngle newViewAngles = AimTo(entityHead);


				double diff = sqrt(
					pow(newViewAngles.x - viewAngles.x, 2) +
					pow(newViewAngles.y - viewAngles.y, 2)
				);

				if (diff < closestDiff) {
					closestDiff = diff;
					nearestEntityAim = newViewAngles;
					botFound = TRUE;
				}
			}

			if (botFound == FALSE) {
				return;
			}

			// set player view angle to aim to nearest found
			cmd->viewangles = nearestEntityAim;
		}
	}
}
