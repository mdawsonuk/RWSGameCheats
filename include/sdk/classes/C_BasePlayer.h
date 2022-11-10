#pragma once

#include "C_BaseEntity.h"
#include "sdk\netvars.h"

enum MoveType_t
{
	MOVETYPE_NONE = 0,	// never moves
	MOVETYPE_ISOMETRIC,			// For players -- in TF2 commander view, etc.
	MOVETYPE_WALK,				// Player only - moving on the ground
	MOVETYPE_STEP,				// gravity, special edge handling -- monsters use this
	MOVETYPE_FLY,				// No gravity, but still collides with stuff
	MOVETYPE_FLYGRAVITY,		// flies through the air + is affected by gravity
	MOVETYPE_VPHYSICS,			// uses VPHYSICS for simulation
	MOVETYPE_PUSH,				// no clip to world, push and crush
	MOVETYPE_NOCLIP,			// No gravity, no collisions, still do velocity/avelocity
	MOVETYPE_LADDER,			// Used by players only when going onto a ladder
	MOVETYPE_OBSERVER,			// Observer movement, depends on player's observer mode
	MOVETYPE_CUSTOM,			// Allows the entity to describe its own physics

	// should always be defined as the last item in the list
	MOVETYPE_LAST = MOVETYPE_CUSTOM,

	MOVETYPE_MAX_BITS = 4
};

#define	LIFE_ALIVE				0 // alive
#define	LIFE_DYING				1 // playing death animation or still falling off of a ledge waiting to hit ground
#define	LIFE_DEAD				2 // dead. lying still.
#define LIFE_RESPAWNABLE		3
#define LIFE_DISCARDBODY		4

#define MAX_WEAPONS 64

// There is actually a long inheritance chain between C_BasePlayer and C_BaseEntity
// but for what we need to do, this is enough (I also don't think we care about the custom materials...)

class C_BaseCombatWeapon;

class C_BasePlayer : public C_BaseEntity /*, public CCustomMaterialOwner*/
{
public:
	
	NETVAR(m_fFlags, int, "DT_BasePlayer", "m_fFlags");
	NETVAR(m_lifeState, int, "DT_BasePlayer", "m_lifeState");

	NETVAR(m_iMoveState, MoveType_t, "DT_CSPlayer", "m_iMoveState");
	NETVAR(m_iHealth, int, "DT_CSPlayer", "m_iHealth");

	NETVAR(m_hActiveWeapon, CBaseHandle, "DT_BaseCombatCharacter", "m_hActiveWeapon");
	NETVAR_ARRAY(m_hMyWeapons, CBaseHandle, "DT_BaseCombatCharacter", "m_hMyWeapons");

	bool IsAlive()
	{
		return m_lifeState() == LIFE_ALIVE;
	}

	C_BaseCombatWeapon* GetActiveWeapon()
	{
		return reinterpret_cast<C_BaseCombatWeapon*>(g_EntityList->GetClientEntityFromHandle(m_hActiveWeapon()));
	}
};