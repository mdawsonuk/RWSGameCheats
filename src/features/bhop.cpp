#include "features/bhop.h"

#include "sdk/interfaces.h"
#include "sdk/classes/C_BasePlayer.h"
#include "sdk/classes/CUserCmd.h"
#include "features/adaptivedifficulty.h"

namespace BHop
{

	void OnCreateMove(CUserCmd* cmd)
	{
		if (!isBhop) {
			return;
		}

		auto localPlayer = *g_LocalPlayer;

		// Don't want to jump when going up a ladder or in noclip
		switch (localPlayer->m_iMoveState())
		{
		case MOVETYPE_NOCLIP:
		case MOVETYPE_LADDER:
			return;
		default:
			break;
		}

		if (cmd->buttons & IN_JUMP)
		{
			if (localPlayer->m_fFlags() & FL_ONGROUND)
			{
				// We're on the ground so let's jump!
				cmd->buttons |= IN_JUMP;
			}
			else
			{
				// We're in the air, clear the jump button
				cmd->buttons &= ~IN_JUMP;

				// TODO: We could also do auto strafing here
			}
		}

	}
}
