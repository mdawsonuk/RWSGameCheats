#pragma once

#include "sdk/netvars.h"

class C_BaseCombatWeapon : public C_BaseEntity
{
public:

	NETVAR(m_hOwner, CBaseHandle, "DT_BaseCombatWeapon", "m_hOwner");

};