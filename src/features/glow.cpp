#include "../glow.h"

#include "sdk/interfaces.h"
#include "sdk/classes/C_BasePlayer.h"
#include "sdk/classes/CUserCmd.h"

namespace Glow
{

	void OnCreateGlow()
	{
		auto localPlayer = *g_LocalPlayer;

		int localTeam = ((C_BaseEntity*)(localPlayer))->m_iTeamNum();

		IClientEntityList* entityList = g_EntityList;

		auto highestIndex = entityList->NumberOfEntities(true);

		for (int i = 0; i <= highestIndex; i++) {
			auto entity = entityList->GetClientEntity(i);

			if (entity == NULL) {
				continue;
			}

			int health = ((C_BasePlayer*)(entity))->m_iHealth();
			if (health <= 0 || health > 100) {
				continue;
			}

			int teamNum = ((C_BaseEntity*)(entity))->m_iTeamNum();
			if (localTeam == teamNum || teamNum == 0) {
				if (localTeam == teamNum) {
					//green
				}
				else {
					continue;
				}
			}
			else {
				//red
			}
		}
	}
}
