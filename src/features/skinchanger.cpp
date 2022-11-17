#include "features/skinchanger.h"

#include <random>
#include <map>

#include "sdk/classes/C_BasePlayer.h"
#include "sdk/classes/C_BaseCombatWeapon.h"

#define SEQ_DEFAULT_DRAW			0
#define SEQ_DEFAULT_IDLE1			1
#define SEQ_DEFAULT_IDLE2			2
#define SEQ_DEFAULT_LIGHT_MISS1		3
#define SEQ_DEFAULT_LIGHT_MISS2		4
#define SEQ_DEFAULT_LIGHT_HIT1		5
#define SEQ_DEFAULT_LIGHT_HIT2		6
#define SEQ_DEFAULT_LIGHT_BSTAB1	7
#define SEQ_DEFAULT_LIGHT_BSTAB2	8
#define SEQ_DEFAULT_HEAVY_MISS1		9
#define SEQ_DEFAULT_HEAVY_HIT1		10
#define SEQ_DEFAULT_HEAVY_BSTAB1	11
#define SEQ_DEFAULT_LOOKAT01		12

#define SEQ_BUTTERFLY_DRAW1			0
#define SEQ_BUTTERFLY_DRAW2			1
#define SEQ_BUTTERFLY_LOOKAT01		13
#define SEQ_BUTTERFLY_LOOKAT03		15

enum EEconItemQuality
{
	AE_UNDEFINED = -1,

	AE_NORMAL = 0,
	AE_GENUINE = 1,
	AE_VINTAGE,
	AE_UNUSUAL,
	AE_UNIQUE,
	AE_COMMUNITY,
	AE_DEVELOPER,
	AE_SELFMADE,
	AE_CUSTOMIZED,
	AE_STRANGE,
	AE_COMPLETED,
	AE_HAUNTED,
	AE_TOURNAMENT,
	AE_FAVORED,

	AE_MAX_TYPES,
};

enum ItemDefinitionIndex : short
{
	WEAPON_INVALID = -1,
	WEAPON_DEAGLE = 1,
	WEAPON_ELITE,
	WEAPON_FIVESEVEN,
	WEAPON_GLOCK,
	WEAPON_AK47 = 7,
	WEAPON_AUG,
	WEAPON_AWP,
	WEAPON_FAMAS,
	WEAPON_G3SG1,
	WEAPON_GALILAR = 13,
	WEAPON_M249,
	WEAPON_M4A1 = 16,
	WEAPON_MAC10,
	WEAPON_P90 = 19,
	WEAPON_MP5 = 23,
	WEAPON_UMP45,
	WEAPON_XM1014,
	WEAPON_BIZON,
	WEAPON_MAG7,
	WEAPON_NEGEV,
	WEAPON_SAWEDOFF,
	WEAPON_TEC9,
	WEAPON_TASER,
	WEAPON_HKP2000,
	WEAPON_MP7,
	WEAPON_MP9,
	WEAPON_NOVA,
	WEAPON_P250,
	WEAPON_SHIELD,
	WEAPON_SCAR20,
	WEAPON_SG556,
	WEAPON_SSG08,
	WEAPON_KNIFEGG,
	WEAPON_KNIFE,
	WEAPON_FLASHBANG,
	WEAPON_HEGRENADE,
	WEAPON_SMOKEGRENADE,
	WEAPON_MOLOTOV,
	WEAPON_DECOY,
	WEAPON_INCGRENADE,
	WEAPON_C4,
	WEAPON_HEALTHSHOT = 57,
	WEAPON_KNIFE_T = 59,
	WEAPON_M4A1_SILENCER,
	WEAPON_USP_SILENCER,
	WEAPON_CZ75A = 63,
	WEAPON_REVOLVER,
	WEAPON_TAGRENADE = 68,
	WEAPON_FISTS,
	WEAPON_BREACHCHARGE,
	WEAPON_TABLET = 72,
	WEAPON_MELEE = 74,
	WEAPON_AXE,
	WEAPON_HAMMER,
	WEAPON_SPANNER = 78,
	WEAPON_KNIFE_GHOST = 80,
	WEAPON_FIREBOMB,
	WEAPON_DIVERSION,
	WEAPON_FRAG_GRENADE,
	WEAPON_SNOWBALL,
	WEAPON_BUMPMINE,
	WEAPON_BAYONET = 500,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT,
	WEAPON_KNIFE_KARAMBIT,
	WEAPON_KNIFE_M9_BAYONET,
	WEAPON_KNIFE_TACTICAL,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY,
	WEAPON_KNIFE_PUSH,
	WEAPON_KNIFE_URSUS = 519,
	WEAPON_KNIFE_GYPSY_JACKKNIFE,
	WEAPON_KNIFE_STILETTO = 522,
	WEAPON_KNIFE_WIDOWMAKER,
	GLOVE_STUDDED_BLOODHOUND = 5027,
	GLOVE_T_SIDE = 5028,
	GLOVE_CT_SIDE = 5029,
	GLOVE_SPORTY = 5030,
	GLOVE_SLICK = 5031,
	GLOVE_LEATHER_WRAP = 5032,
	GLOVE_MOTORCYCLE = 5033,
	GLOVE_SPECIALIST = 5034,
	GLOVE_HYDRA = 5035
};

struct SkinChange
{
	ItemDefinitionIndex itemIndex = WEAPON_INVALID; // Used for changing knife models
	float itemWear = 0.00001f;	// Float val 0 - 1 for setting wear
	int seed = 0; // Seed for pattern based skins
	int quality = AE_NORMAL; // Set weapon prefix
	int paintKit = 0; // Skin paintkit id
	int statTrakKills = -1; // Number of stattrak kills (TODO: Currently stattrak will show user unknown - look into this)
};

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