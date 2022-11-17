#include "features/chams.h"

#include "hooks.h"
#include "sdk/interfaces/IMaterial.h"
#include "sdk/classes/C_BasePlayer.h"
#include "sdk/classes/C_PredictedViewModel.h"
#include "features/adaptivedifficulty.h"

namespace Chams
{
	IMaterial* matFlat = nullptr;
	IMaterial* matReg = nullptr;

	void GetMaterials()
	{
		matFlat = g_MaterialSystem->FindMaterial("debug/debugdrawflat", nullptr);
		matReg = g_MaterialSystem->FindMaterial("debug/debugambientcube", nullptr);
	}

	void OverrideMaterial(bool ignorez, float color[3])
	{
		matReg->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, ignorez);
		matReg->AlphaModulate(1.f);
		matReg->ColorModulate(color[0], color[1], color[2]);
		
		g_ModelRender->ForcedMaterialOverride(matReg);
	}

	void OverridePlayer(IVModelRender* _this, IMatRenderContext* pRenderContext, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld)
	{
		static auto ogDrawModelExecute = Hooks::modelRenderHooks->GetOriginalFn(21);

		auto localPlayer = *g_LocalPlayer;
		auto entity = reinterpret_cast<C_BasePlayer*>(g_EntityList->GetClientEntity(pInfo.entity_index));
		
		// Don't do anything if the entity isn't valid, it's a dead player, or they're on our team
		if (!entity || !entity->IsAlive() || entity->m_iTeamNum() == localPlayer->m_iTeamNum())
		{
			return;
		}

		float visibleColor[3] = { 220.f / 255.f, 140.f / 255.f, 20.f / 255.f };
		float occludedColor[3] = { 220.f / 255.f, 50.f / 255.f, 50.f / 255.f };

		// Override the material with an ignorez material with our color that will be visible through walls
		OverrideMaterial(true, occludedColor);

		// Draw the model with the ignorez material
		Utils::SpoofThisCall<bool>(ogDrawModelExecute, Hooks::engineDllGadget, _this, pRenderContext, &state, &pInfo, pCustomBoneToWorld);

		// Override the material with a normal material with the color that will not be visible through walls
		OverrideMaterial(false, visibleColor);

		// We don't draw the model here because it will be drawn and reset in the DrawModelExecute hook handler
	}

	void OverrideWeapon(const ModelRenderInfo_t& pInfo)
	{
		float color[3] = { 0.f / 255.f, 153.f / 255.f, 255.f / 255.f };
		OverrideMaterial(false, color);
	}

	void OnDrawModelExecute(IVModelRender* _this, IMatRenderContext* pRenderContext, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld)
	{
		if (!isChams) {
			return;
		}
		// Get the materials that we will be using if we haven't already
		static bool materialsFound = false;
		if (!materialsFound)
		{
			GetMaterials();

			materialsFound = true;
		}

		// Check the name of the model we are currently drawing
		auto model = pInfo.pModel;
		if (!model)
		{
			return;
		}

		auto modelName = g_ModelInfoClient->GetModelName(model);

		// If the model being rendered doesn't have an entity index, it can't be a player
		//		so check for weapons, arms etc here
		if (pInfo.entity_index == -1)
		{
			// Weapons
			if (modelName[15] == 'w')
			{
				OverrideWeapon(pInfo);
			}
			// Local player arms
			else if (modelName[15] == 'v')
			{
				// TODO: Override arms if we want?
			}

		}
		// Entity index is valid, it's either an equipped weapon or a player model
		else
		{ 
			// Is the model name "models/players/..."
			if (modelName[7] == 'p')
			{
				OverridePlayer(_this, pRenderContext, state, pInfo, pCustomBoneToWorld);
			}
			// It's a weapon
			else
			{
				auto viewModel = reinterpret_cast<C_PredictedViewModel*>(g_EntityList->GetClientEntity(pInfo.entity_index));
				if (viewModel && viewModel->GetOwner() == *g_LocalPlayer)
				{
					return;
				}

				OverrideWeapon(pInfo);
			}
		}
	}
}