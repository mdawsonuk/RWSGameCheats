#include "../aimbot.h"

#include "sdk/interfaces.h"
#include "sdk/classes/C_BasePlayer.h"
#include "sdk/classes/CUserCmd.h"

#include<cmath>
#include <iostream>
using namespace std;

#define HEAD_BONE_ID ((DWORD) 8)


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



		// use radian trig to find new angle
		QAngle newAngle = *(new QAngle(
			(float)(asinf(posDiff.z / sqrt(pow(posDiff.x, 2) + pow(posDiff.y, 2) + pow(posDiff.z, 2))) * 57.295779513082f),
			(float)(atanf(posDiff.y / posDiff.x) * 57.295779513082f),
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

			QAngle nearestEntityAim;
			QAngle* nearestEntityAimPointer = &nearestEntityAim;
			bool botFound = FALSE;

			auto highestIndex = entityList->GetHighestEntityIndex();

			// find the entity shortest distance away
			for (int i = 0; i <= highestIndex; i++) {
				auto entity = entityList->GetClientEntity(i);

				if (entity == NULL) {
					continue;
				}

				//-------------------------turn into head position


				Vector entityPos = entity->GetAbsOrigin();

				/*
				uintptr_t boneMatrix = RPM<uintptr_t>(entity + 0x26A8);

				uintptr_t boneMatrix = *reinterpret_cast<uintptr_t*>((C_BaseEntity*)(entity)) + 0x2698;

				// 0x26A8  or  0x2698 

				uintptr_t boneMatrix = *reinterpret_cast<int*>(entity + 0x26A8);
				
				Vector entityPos = { *reinterpret_cast<float*>(boneMatrix + 0x30 * 8 + 0x0C),
				*reinterpret_cast<float*>(boneMatrix + 0x30 * 8 + 0x1C),
				*reinterpret_cast<float*>(boneMatrix + 0x30 * 8 + 0x2C) };
				
				struct boneMatrix_t {
					::byte pad3[12];
					float x;
					::byte pad1[12];
					float y;
					::byte pad2[12];
					float z;
				};
				
				boneMatrix_t skull = *reinterpret_cast<boneMatrix_t*>(boneMatrix + (sizeof(boneMatrix) * 8 ));
				Vector entityPos = Vector(skull.x, skull.y, skull.z);
				*/


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
					closest_diff = diff;
					nearestEntityAim = newViewAngles;
					botFound = TRUE;
				}
			}

			if (botFound == FALSE) {
				return;
			}

			// set player view angle to aim to nearest found
			cmd->viewangles = *nearestEntityAimPointer;

			Vector oldPunch = { 0, 0, 0 };
			int oldShotCount = 0;

			auto& playerPunchAngle = localPlayer->m_aimPunchAngle();

		}
	}
}
