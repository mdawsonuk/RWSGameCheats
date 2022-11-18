#pragma once

#include "C_PlayerResource.h"
#include "sdk/netvars.h"

class C_CS_PlayerResource : public C_PlayerResource
{
public:
	NETVAR_ARRAY(m_iKills, int, "DT_CSPlayerResource", "m_iKills");
	NETVAR_ARRAY(m_iAssists, int, "DT_CSPlayerResource", "m_iAssists");
	NETVAR_ARRAY(m_iDeaths, int, "DT_CSPlayerResource", "m_iDeaths");
};