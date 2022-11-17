#include "hooks.h"

#include "sdk/interfaces.h"
#include "sdk/classes/C_BasePlayer.h"
#include "sdk/classes/Vector.h"

#include "features/aimbot.h"
#include "features/norecoil.h"
#include "features/bhop.h"
#include "features/glow.h"
#include "features/noflash.h"
#include "features/bhop.h"
#include "features/chams.h"
#include "features/skinchanger.h"

namespace Hooks
{
	void* clientDllGadget;
	void* engineDllGadget;

	std::shared_ptr<VMTHook> clientHooks;
	std::shared_ptr<VMTHook> clientModeHooks;
	std::shared_ptr<VMTHook> modelRenderHooks;
	std::shared_ptr<VMTHook> modelInfoHooks;

	bool __stdcall hkCreateMove(float flInputSampleTime, CUserCmd* cmd)
	{
		static auto ogCreateMove = clientModeHooks->GetOriginalFn(24);

		auto res = Utils::SpoofStdCall<bool>(ogCreateMove, clientDllGadget, flInputSampleTime, cmd);

		// TODO: Do anything in CreateMove here (aimbot, bhop, etc)
		BHop::OnCreateMove(cmd);
		AimBot::OnCreateMove(cmd);
		NoRecoil::OnCreateMove(cmd);

		return res;
	}
	
	bool __fastcall hkDrawModelExecute(IVModelRender* _this, void* edx, IMatRenderContext* pRenderContext, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld)
	{
		static auto ogDrawModelExecute = Hooks::modelRenderHooks->GetOriginalFn(21);
		
		if (g_ModelRender->IsForcedMaterialOverride())
			return Utils::SpoofThisCall<bool>(ogDrawModelExecute, Hooks::engineDllGadget, _this, pRenderContext, &state, &pInfo, pCustomBoneToWorld);

		// Override any necessary materials
		Chams::OnDrawModelExecute(_this, pRenderContext, state, pInfo, pCustomBoneToWorld);

		// After overriding the materials, call the original DrawModelExecute to render the overriden model
		auto res = Utils::SpoofThisCall<bool>(ogDrawModelExecute, Hooks::engineDllGadget, _this, pRenderContext, &state, &pInfo, pCustomBoneToWorld);

		// Then reset the overriden material and return the draw call return value
		g_ModelRender->ForcedMaterialOverride(nullptr);
		return res;
	}

	void __fastcall hkFrameStageNotify(void* _this, void* edx, ClientFrameStage_t curStage)
	{
		static auto ogFrameStageNotify = Hooks::clientHooks->GetOriginalFn(37);

		switch (curStage)
		{
		case FRAME_NET_UPDATE_POSTDATAUPDATE_START:

			SkinChanger::OnFramePostDataUpdateStart();

			return Utils::SpoofFastCall(ogFrameStageNotify, clientDllGadget, _this, edx, curStage);

		case FRAME_RENDER_START:

			Glow::OnFrameStageNotify();
			NoFlash::CheckForFlash();

			return Utils::SpoofFastCall(ogFrameStageNotify, clientDllGadget, _this, edx, curStage);

		default:

			return Utils::SpoofFastCall(ogFrameStageNotify, clientDllGadget, _this, edx, curStage);
		}

	}

	bool SetupHooks()
	{
		// Find gadgets in required dlls
		clientDllGadget = Utils::GetRetAddrSpoofGadget("client.dll");
		if (!clientDllGadget)
		{
			return false;
		}

		engineDllGadget = Utils::GetRetAddrSpoofGadget("engine.dll");
		if (!engineDllGadget)
		{
			return false;
		}

		// Setup VMTHook objects for every vtable we want to hook functions in
		clientModeHooks = std::shared_ptr<VMTHook>(new VMTHook(reinterpret_cast<void**>(g_ClientMode)));
		clientModeHooks->Hook(24, hkCreateMove);
		// More ClientMode hooks here...
		clientModeHooks->EnableHooks();

		clientHooks = std::shared_ptr<VMTHook>(new VMTHook(reinterpret_cast<void**>(g_ClientDLL)));
		clientHooks->Hook(37, hkFrameStageNotify);
		clientHooks->EnableHooks();

		modelRenderHooks = std::shared_ptr<VMTHook>(new VMTHook(reinterpret_cast<void**>(g_ModelRender)));
		modelRenderHooks->Hook(21, hkDrawModelExecute);
		modelRenderHooks->EnableHooks();

		// Netvar recv proxy hooks
		SkinChanger::ogRecvProxy_nModelIndex = Netvars::HookRecvProxy("DT_BaseViewModel", "m_nModelIndex", SkinChanger::RecvProxy_nModelIndex);
		SkinChanger::ogRecvProxy_nSequence = Netvars::HookRecvProxy("DT_BaseViewModel", "m_nSequence", SkinChanger::RecvProxy_nSequence);

		return true;
	}

	void CleanupHooks()
	{
		// Call DisableHooks on every VMTHook object
		if (clientModeHooks)
		{
			clientModeHooks->DisableHooks();
		}
		if (clientHooks)
		{
			clientHooks->DisableHooks();
		}
		if (modelRenderHooks)
		{
			modelRenderHooks->DisableHooks();
		}
	}


}


