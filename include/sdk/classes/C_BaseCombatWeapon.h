#pragma once

#include "sdk/netvars.h"

class C_BaseCombatWeapon : public C_BaseEntity
{
public:

	NETVAR(m_hOwner, CBaseHandle, "DT_BaseCombatWeapon", "m_hOwner");
	NETVAR(m_iViewModelIndex, int, "DT_BaseCombatWeapon", "m_iViewModelIndex");
	NETVAR(m_iWorldDroppedModelIndex, int, "DT_BaseCombatWeapon", "m_iWorldDroppedModelIndex");
	NETVAR(m_iWorldModelIndex, int, "DT_BaseCombatWeapon", "m_iWorldModelIndex");

	NETVAR(m_flFallbackWear, float, "DT_EconEntity", "m_flFallbackWear");
	NETVAR(m_nFallbackPaintKit, int, "DT_EconEntity", "m_nFallbackPaintKit");
	NETVAR(m_nFallbackSeed, int, "DT_EconEntity", "m_nFallbackSeed");
	NETVAR(m_nFallbackStatTrak, int, "DT_EconEntity", "m_nFallbackStatTrak");
	NETVAR(m_iEntityQuality, int, "DT_EconEntity", "m_iEntityQuality");
	NETVAR(m_iItemIDHigh, int, "DT_EconEntity", "m_iItemIDHigh");
	NETVAR(m_iItemIDLow, int, "DT_EconEntity", "m_iItemIDLow");
	NETVAR(m_iItemDefinitionIndex, int, "DT_EconEntity", "m_iItemDefinitionIndex");

	C_BasePlayer* GetOwner()
	{
		return reinterpret_cast<C_BasePlayer*>(g_EntityList->GetClientEntityFromHandle(m_hOwner()));
	}

};