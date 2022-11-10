#pragma once

#include "sdk/interfaces.h"
#include "sdk/classes/C_BaseEntity.h"
#include "sdk/classes/C_BasePlayer.h"
#include "sdk/netvars.h"

class C_PredictedViewModel : public C_BaseEntity
{
public:

	NETVAR(m_hOwner, CBaseHandle, "DT_PredictedViewModel", "m_hOwner");

	C_BasePlayer* GetOwner()
	{
		return reinterpret_cast<C_BasePlayer*>(g_EntityList->GetClientEntityFromHandle(m_hOwner()));
	}

};