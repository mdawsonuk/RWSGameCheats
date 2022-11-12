#pragma once

#include "sdk/classes/C_BaseEntity.h"
#include "sdk/interfaces/IGameResources.h"

class Color;

class C_PlayerResource : public C_BaseEntity, public IGameResources
{
public:
	C_PlayerResource();
	virtual			~C_PlayerResource();

public: // IGameResources intreface

	// Team data access 
	virtual int		GetTeamScore(int index);
	virtual const char* GetTeamName(int index);
	virtual const Color& GetTeamColor(int index);

	// Player data access
	virtual bool	IsConnected(int index);
	virtual bool	IsAlive(int index);
	virtual bool	IsFakePlayer(int index);
	virtual bool	IsLocalPlayer(int index);
	virtual bool	IsHLTV(int index);

	virtual const char* GetPlayerName(int index);
	virtual int		GetPing(int index);
	//	virtual int		GetPacketloss( int index );
	virtual int		GetKills(int index);
	virtual int		GetAssists(int index);
	virtual int		GetDeaths(int index);
	virtual int		GetTeam(int index);
	virtual int		GetPendingTeam(int index);
	virtual int		GetFrags(int index);
	virtual int		GetHealth(int index);
	virtual int		GetCoachingTeam(int index);

	virtual void	ClientThink();
	virtual	void	OnDataChanged(DataUpdateType_t updateType);
	virtual void	DeviceLost(void);
	virtual void	DeviceReset(void* pDevice, void* pPresentParameters, void* pHWnd);
	virtual void	ScreenSizeChanged(int width, int height) { }
	virtual void	TeamChanged(void) { }

protected:
	virtual void	UpdatePlayerName(int slot);

public:
	// Data for each player that's propagated to all clients
	// Stored in individual arrays so they can be sent down via datatables
	int		m_iPing[MAX_PLAYERS + 1];
	int		m_iKills[MAX_PLAYERS + 1];
	int		m_iAssists[MAX_PLAYERS + 1];
	int		m_iDeaths[MAX_PLAYERS + 1];
	bool	m_bConnected[MAX_PLAYERS + 1];
	int		m_iTeam[MAX_PLAYERS + 1];
	int		m_iPendingTeam[MAX_PLAYERS + 1];
	bool	m_bAlive[MAX_PLAYERS + 1];
	int		m_iHealth[MAX_PLAYERS + 1];
};

