#include "hooks.h"

#include "sdk/interfaces.h"
#include "features/bhop.h"
#include "features/noflash.h"

#include "sdk/classes/C_BasePlayer.h"
#include "sdk/classes/Vector.h"
#include "features/aimbot.h"
#include "features/norecoil.h"
#include "features/noflash.h"

namespace Hooks
{
	void* clientDllGadget;

	VMTHook clientModeHooks;


	// TODO: Add hook handlers here:

	bool __stdcall hkCreateMove(float flInputSampleTime, CUserCmd* cmd)
	{
		static auto ogCreateMove = clientModeHooks.GetOriginalFn(24);

		auto res = Utils::SpoofStdCall<bool>(ogCreateMove, clientDllGadget, flInputSampleTime, cmd);

		// TODO: Do anything in CreateMove here (aimbot, bhop, etc)
		BHop::OnCreateMove(cmd);
		NoFlash::CheckForFlash();
		AimBot::OnCreateMove(cmd);
		NoRecoil::OnCreateMove(cmd);

		//auto localPlayer = *g_LocalPlayer;

		//auto& pos = localPlayer->m_vecOrigin();
		//printf_s("Local Player Pos: (%.2f, %.2f, %.2f)\n", pos.x, pos.y, pos.z);

		return res;
	}
	


	bool SetupHooks()
	{
		// Find gadgets in required dlls
		//		Can probably just use client.dll gadget for all since it's a whitelisted dll?
		clientDllGadget = Utils::GetRetAddrSpoofGadget("client.dll");
		if (!clientDllGadget)
		{
			return false;
		}


		// Setup VMTHook objects for every vtable we want to hook functions in
		clientModeHooks = VMTHook(reinterpret_cast<void**>(g_ClientMode));
		clientModeHooks.Hook(24, hkCreateMove);
		// More ClientMode hooks here...
		clientModeHooks.EnableHooks();


		return true;
	}

	void CleanupHooks()
	{
		// Call DisableHooks on every VMTHook object
		clientModeHooks.DisableHooks();
	}


}


