#include "hooks.h"

#include "sdk/interfaces.h"
#include "sdk/classes/C_BasePlayer.h"
#include "sdk/classes/Vector.h"

#include "features/bhop.h"
#include "features/chams.h"


namespace Hooks
{
	void* clientDllGadget;
	void* engineDllGadget;

	std::shared_ptr<VMTHook> clientModeHooks;
	std::shared_ptr<VMTHook> modelRenderHooks;

	// TODO: Add hook handlers here:

	bool __stdcall hkCreateMove(float flInputSampleTime, CUserCmd* cmd)
	{
		static auto ogCreateMove = clientModeHooks->GetOriginalFn(24);

		auto res = Utils::SpoofStdCall<bool>(ogCreateMove, clientDllGadget, flInputSampleTime, cmd);

		BHop::OnCreateMove(cmd);

		return res;
	}
	
	bool __fastcall hkDrawModelExecute(IVModelRender* _this, void* edx, IMatRenderContext* pRenderContext, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld)
	{
		static auto ogDrawModelExecute = Hooks::modelRenderHooks->GetOriginalFn(21);
		
		// Override any necessary materials
		Chams::OnDrawModelExecute(_this, pRenderContext, state, pInfo, pCustomBoneToWorld);

		// After overriding the materials, call the original DrawModelExecute to render the overriden model
		auto res = Utils::SpoofThisCall<bool>(ogDrawModelExecute, Hooks::engineDllGadget, _this, pRenderContext, &state, &pInfo, pCustomBoneToWorld);

		// Then reset the overriden material and return the draw call return value
		g_ModelRender->ForcedMaterialOverride(nullptr);
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

		engineDllGadget = Utils::GetRetAddrSpoofGadget("engine.dll");

		// Setup VMTHook objects for every vtable we want to hook functions in
		clientModeHooks = std::shared_ptr<VMTHook>(new VMTHook(reinterpret_cast<void**>(g_ClientMode)));
		clientModeHooks->Hook(24, hkCreateMove);
		// More ClientMode hooks here...
		clientModeHooks->EnableHooks();

		modelRenderHooks = std::shared_ptr<VMTHook>(new VMTHook(reinterpret_cast<void**>(g_ModelRender)));
		modelRenderHooks->Hook(21, hkDrawModelExecute);
		modelRenderHooks->EnableHooks();

		return true;
	}

	void CleanupHooks()
	{
		// Call DisableHooks on every VMTHook object
		//		TODO: Is this even necessary? Will the dtors be called automatically when unloading?
		if (clientModeHooks)
		{
			clientModeHooks->DisableHooks();
		}
		if (modelRenderHooks)
		{
			modelRenderHooks->DisableHooks();
		}
	}


}


