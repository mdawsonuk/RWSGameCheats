#include "features/skinchanger.h"

#include <random>
#include <map>

#include "sdk/classes/C_BasePlayer.h"
#include "sdk/classes/C_BaseCombatWeapon.h"

std::map<ItemDefinitionIndex, SkinChange> skinChangeDefinitions;

namespace SkinChanger
{
	int ct_default = -1;
	int t_default = -1;
	int butterfly = -1;

	void GetModelIndices()
	{
		ct_default = g_ModelInfoClient->GetModelIndex("models/weapons/v_knife_default_ct.mdl");
		t_default = g_ModelInfoClient->GetModelIndex("models/weapons/v_knife_default_t.mdl");

		// TODO: Get other knife models here
		butterfly = g_ModelInfoClient->GetModelIndex("models/weapons/v_knife_butterfly.mdl");
	}

	void OnFramePostDataUpdateStart()
	{
		if (ct_default == -1)
		{
			GetModelIndices();
		}

		// If player is alive,
		auto localPlayer = *g_LocalPlayer;
		if (!localPlayer || !localPlayer->IsAlive())
		{
			return;
		}

		CBaseHandle* weapons = localPlayer->m_hMyWeapons();

		for (int i = 0; i < MAX_WEAPONS; i++)
		{
			auto weapon = reinterpret_cast<C_BaseCombatWeapon*>(g_EntityList->GetClientEntityFromHandle(weapons[i]));
			if (!weapon)
			{
				break;
			}

			auto weaponIndex = static_cast<ItemDefinitionIndex>(weapon->m_iItemDefinitionIndex());
			if (weapon->GetClientClass()->m_ClassId == CKnife)
			{
				if (skinChangeDefinitions.find(weaponIndex) == skinChangeDefinitions.end())
				{
					continue;
				}
				auto& skinChange = skinChangeDefinitions.at(weaponIndex);

				// Set item id high to -1 so that fallback settings are used
				weapon->m_iItemIDHigh() = -1;

				// Set all models correctly so that animations aren't bugged
				weapon->m_iItemDefinitionIndex() = skinChange.itemIndex;
				weapon->m_nModelIndex() = butterfly;
				weapon->m_iViewModelIndex() = butterfly;
				weapon->m_iWorldModelIndex() = butterfly + 1;
				weapon->m_iWorldDroppedModelIndex() = butterfly + 2;

				// Set all skin settings
				weapon->m_flFallbackWear() = skinChange.itemWear;
				weapon->m_nFallbackSeed() = skinChange.seed;
				weapon->m_iEntityQuality() = skinChange.quality;
				weapon->m_nFallbackPaintKit() = skinChange.paintKit;
				weapon->m_nFallbackStatTrak() = skinChange.statTrakKills;
			}
			else
			{
				if (skinChangeDefinitions.find(weaponIndex) == skinChangeDefinitions.end())
				{
					continue;
				}
				auto& skinChange = skinChangeDefinitions.at(weaponIndex);

				// Set item id high to -1 so that fallback settings are used
				weapon->m_iItemIDHigh() = -1;

				// Set all skin settings
				weapon->m_flFallbackWear() = skinChange.itemWear;
				weapon->m_nFallbackSeed() = skinChange.seed;
				weapon->m_iEntityQuality() = skinChange.quality;
				weapon->m_nFallbackPaintKit() = skinChange.paintKit;
				weapon->m_nFallbackStatTrak() = skinChange.statTrakKills;
			}
		}
	}

	RecvProxyFn ogRecvProxy_nModelIndex = nullptr;
	void RecvProxy_nModelIndex(const CRecvProxyData* pData, void* pStruct, void* pOut)
	{
		ogRecvProxy_nModelIndex(pData, pStruct, pOut);

		// TODO: Add ability to change this within gui

		int* out = reinterpret_cast<int*>(pOut);
		if (*out == ct_default || *out == t_default)
		{
			*out = butterfly;
		}
	}

	int RandSequence(int min, int max)
	{
		int n = max - min + 1;
		int r = RAND_MAX % n;
		int x;
		do
		{
			x = rand();
		} while (x >= RAND_MAX - r);
		return min + x % n;
	}

	RecvProxyFn ogRecvProxy_nSequence = nullptr;
	void RecvProxy_nSequence(const CRecvProxyData* pData, void* pStruct, void* pOut)
	{
		// Check that the local player is alive
		auto localPlayer = *g_LocalPlayer;
		if (!localPlayer || !localPlayer->IsAlive())
		{
			return ogRecvProxy_nSequence(pData, pStruct, pOut);
		}

		// Check that we are holding a knife
		auto weapon = localPlayer->GetActiveWeapon();
		if (!weapon || weapon->GetClientClass()->m_ClassId != CKnife)
		{
			return ogRecvProxy_nSequence(pData, pStruct, pOut);
		}

		// TODO: If we add ability to change knife type, switch on knife type
		int m_nSequence = pData->m_Value.m_Int;
		switch (m_nSequence)
		{
		case SEQ_DEFAULT_DRAW:
			// Butterfly knife has two possible draw animations
			m_nSequence = RandSequence(SEQ_BUTTERFLY_DRAW1, SEQ_BUTTERFLY_DRAW2);
			break;
		case SEQ_DEFAULT_LOOKAT01:
			// Butterfly knife has three possible inspect animations
			m_nSequence = RandSequence(SEQ_BUTTERFLY_LOOKAT01, SEQ_BUTTERFLY_LOOKAT03);
			break;
		default:
			// Because of the additional draw animation, every sequence is shifted by 1
			m_nSequence++;
			break;
		}

		// Set the correct sequence after removing the constness of pData
		auto data = const_cast<CRecvProxyData*>(pData);
		data->m_Value.m_Int = m_nSequence;

		ogRecvProxy_nSequence(pData, pStruct, pOut);
	}

	void DumpModelIndices()
	{
		for (int i = 0; i < 1000; i++)
		{
			auto model = g_ModelInfoClient->GetModel(i);
			if (!model)
			{
				continue;
			}

			auto name = g_ModelInfoClient->GetModelName(model);
			printf_s("%04d: %s\n", i, name);
		}
	}

	void LoadSkinChanges()
	{
		// TODO: Load and save to a file

		// For now will just use some static skin changes to show the feature off
		
		// Change knife to a marble fade butterfly knife
		auto def = SkinChange();
		def.itemIndex = WEAPON_KNIFE_BUTTERFLY;
		def.quality = AE_UNUSUAL;
		def.seed = 9; // Max red-tip
		def.paintKit = 413; // Marble fade

		skinChangeDefinitions.emplace(WEAPON_KNIFE, def);
		skinChangeDefinitions.emplace(WEAPON_KNIFE_T, def);

		// AK-47 fire serpent
		def = SkinChange();
		def.paintKit = 180;

		skinChangeDefinitions.emplace(WEAPON_AK47, def);

		// Souvenir AWP Dragon Lore
		def = SkinChange();
		def.quality = AE_TOURNAMENT;
		def.paintKit = 344;

		skinChangeDefinitions.emplace(WEAPON_AWP, def);

		// M4A4 Howl
		def = SkinChange();
		def.paintKit = 309;

		skinChangeDefinitions.emplace(WEAPON_M4A1, def);

		// M4A1-S Welcome to the Jungle
		def = SkinChange();
		def.quality = AE_TOURNAMENT;
		def.paintKit = 1001;

		skinChangeDefinitions.emplace(WEAPON_M4A1_SILENCER, def);
	}

}