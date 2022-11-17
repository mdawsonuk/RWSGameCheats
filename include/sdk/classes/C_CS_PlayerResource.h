#pragma once

#include "C_PlayerResource.h"
#include "sdk/netvars.h"

class C_CS_PlayerResource : public C_PlayerResource
{
public:
	C_CS_PlayerResource();
	virtual			~C_CS_PlayerResource();

	virtual	void	OnDataChanged(DataUpdateType_t updateType);

	virtual const char* GetPlayerName(int index) override;

	NETVAR_ARRAY(m_iKills, int, "DT_CSPlayerResource", "m_iKills");
	NETVAR_ARRAY(m_iAssists, int, "DT_CSPlayerResource", "m_iAssists");
	NETVAR_ARRAY(m_iDeaths, int, "DT_CSPlayerResource", "m_iDeaths");

protected:

	virtual void	UpdatePlayerName(int slot) override;
};

C_CS_PlayerResource* GetCSResources(void);
