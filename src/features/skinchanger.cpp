#include "features/skinchanger.h"

#include <random>

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


enum ClassIDs : int
{
	CAI_BaseNPC = 0,
	CAK47,
	CBaseAnimating,
	CBaseAnimatingOverlay,
	CBaseAttributableItem,
	CBaseButton,
	CBaseCombatCharacter,
	CBaseCombatWeapon,
	CBaseCSGrenade,
	CBaseCSGrenadeProjectile,
	CBaseDoor,
	CBaseEntity,
	CBaseFlex,
	CBaseGrenade,
	CBaseParticleEntity,
	CBasePlayer,
	CBasePropDoor,
	CBaseTeamObjectiveResource,
	CBaseTempEntity,
	CBaseToggle,
	CBaseTrigger,
	CBaseViewModel,
	CBaseVPhysicsTrigger,
	CBaseWeaponWorldModel,
	CBeam,
	CBeamSpotlight,
	CBoneFollower,
	CBRC4Target,
	CBreachCharge,
	CBreachChargeProjectile,
	CBreakableProp,
	CBreakableSurface,
	CBumpMine,
	CBumpMineProjectile,
	CC4,
	CCascadeLight,
	CChicken,
	CColorCorrection,
	CColorCorrectionVolume,
	CCSGameRulesProxy,
	CCSPlayer,
	CCSPlayerResource,
	CCSRagdoll,
	CCSTeam,
	CDangerZone,
	CDangerZoneController,
	CDEagle,
	CDecoyGrenade,
	CDecoyProjectile,
	CDrone,
	CDronegun,
	CDynamicLight,
	CDynamicProp,
	CEconEntity,
	CEconWearable,
	CEmbers,
	CEntityDissolve,
	CEntityFlame,
	CEntityFreezing,
	CEntityParticleTrail,
	CEnvAmbientLight,
	CEnvDetailController,
	CEnvDOFController,
	CEnvGasCanister,
	CEnvParticleScript,
	CEnvProjectedTexture,
	CEnvQuadraticBeam,
	CEnvScreenEffect,
	CEnvScreenOverlay,
	CEnvTonemapController,
	CEnvWind,
	CFEPlayerDecal,
	CFireCrackerBlast,
	CFireSmoke,
	CFireTrail,
	CFish,
	CFists,
	CFlashbang,
	CFogController,
	CFootstepControl,
	CFunc_Dust,
	CFunc_LOD,
	CFuncAreaPortalWindow,
	CFuncBrush,
	CFuncConveyor,
	CFuncLadder,
	CFuncMonitor,
	CFuncMoveLinear,
	CFuncOccluder,
	CFuncReflectiveGlass,
	CFuncRotating,
	CFuncSmokeVolume,
	CFuncTrackTrain,
	CGameRulesProxy,
	CGrassBurn,
	CHandleTest,
	CHEGrenade,
	CHostage,
	CHostageCarriableProp,
	CIncendiaryGrenade,
	CInferno,
	CInfoLadderDismount,
	CInfoMapRegion,
	CInfoOverlayAccessor,
	CItem_Healthshot,
	CItemCash,
	CItemDogtags,
	CKnife,
	CKnifeGG,
	CLightGlow,
	CMapVetoPickController,
	CMaterialModifyControl,
	CMelee,
	CMolotovGrenade,
	CMolotovProjectile,
	CMovieDisplay,
	CParadropChopper,
	CParticleFire,
	CParticlePerformanceMonitor,
	CParticleSystem,
	CPhysBox,
	CPhysBoxMultiplayer,
	CPhysicsProp,
	CPhysicsPropMultiplayer,
	CPhysMagnet,
	CPhysPropAmmoBox,
	CPhysPropLootCrate,
	CPhysPropRadarJammer,
	CPhysPropWeaponUpgrade,
	CPlantedC4,
	CPlasma,
	CPlayerPing,
	CPlayerResource,
	CPointCamera,
	CPointCommentaryNode,
	CPointWorldText,
	CPoseController,
	CPostProcessController,
	CPrecipitation,
	CPrecipitationBlocker,
	CPredictedViewModel,
	CProp_Hallucination,
	CPropCounter,
	CPropDoorRotating,
	CPropJeep,
	CPropVehicleDriveable,
	CRagdollManager,
	CRagdollProp,
	CRagdollPropAttached,
	CRopeKeyframe,
	CSCAR17,
	CSceneEntity,
	CSensorGrenade,
	CSensorGrenadeProjectile,
	CShadowControl,
	CSlideshowDisplay,
	CSmokeGrenade,
	CSmokeGrenadeProjectile,
	CSmokeStack,
	CSnowball,
	CSnowballPile,
	CSnowballProjectile,
	CSpatialEntity,
	CSpotlightEnd,
	CSprite,
	CSpriteOriented,
	CSpriteTrail,
	CStatueProp,
	CSteamJet,
	CSun,
	CSunlightShadowControl,
	CSurvivalSpawnChopper,
	CTablet,
	CTeam,
	CTeamplayRoundBasedRulesProxy,
	CTEArmorRicochet,
	CTEBaseBeam,
	CTEBeamEntPoint,
	CTEBeamEnts,
	CTEBeamFollow,
	CTEBeamLaser,
	CTEBeamPoints,
	CTEBeamRing,
	CTEBeamRingPoint,
	CTEBeamSpline,
	CTEBloodSprite,
	CTEBloodStream,
	CTEBreakModel,
	CTEBSPDecal,
	CTEBubbles,
	CTEBubbleTrail,
	CTEClientProjectile,
	CTEDecal,
	CTEDust,
	CTEDynamicLight,
	CTEEffectDispatch,
	CTEEnergySplash,
	CTEExplosion,
	CTEFireBullets,
	CTEFizz,
	CTEFootprintDecal,
	CTEFoundryHelpers,
	CTEGaussExplosion,
	CTEGlowSprite,
	CTEImpact,
	CTEKillPlayerAttachments,
	CTELargeFunnel,
	CTEMetalSparks,
	CTEMuzzleFlash,
	CTEParticleSystem,
	CTEPhysicsProp,
	CTEPlantBomb,
	CTEPlayerAnimEvent,
	CTEPlayerDecal,
	CTEProjectedDecal,
	CTERadioIcon,
	CTEShatterSurface,
	CTEShowLine,
	CTesla,
	CTESmoke,
	CTESparks,
	CTESprite,
	CTESpriteSpray,
	CTest_ProxyToggle_Networkable,
	CTestTraceline,
	CTEWorldDecal,
	CTriggerPlayerMovement,
	CTriggerSoundOperator,
	CVGuiScreen,
	CVoteController,
	CWaterBullet,
	CWaterLODControl,
	CWeaponAug,
	CWeaponAWP,
	CWeaponBaseItem,
	CWeaponBizon,
	CWeaponCSBase,
	CWeaponCSBaseGun,
	CWeaponCycler,
	CWeaponElite,
	CWeaponFamas,
	CWeaponFiveSeven,
	CWeaponG3SG1,
	CWeaponGalil,
	CWeaponGalilAR,
	CWeaponGlock,
	CWeaponHKP2000,
	CWeaponM249,
	CWeaponM3,
	CWeaponM4A1,
	CWeaponMAC10,
	CWeaponMag7,
	CWeaponMP5Navy,
	CWeaponMP7,
	CWeaponMP9,
	CWeaponNegev,
	CWeaponNOVA,
	CWeaponP228,
	CWeaponP250,
	CWeaponP90,
	CWeaponSawedoff,
	CWeaponSCAR20,
	CWeaponScout,
	CWeaponSG550,
	CWeaponSG552,
	CWeaponSG556,
	CWeaponShield,
	CWeaponSSG08,
	CWeaponTaser,
	CWeaponTec9,
	CWeaponTMP,
	CWeaponUMP45,
	CWeaponUSP,
	CWeaponXM1014,
	CWeaponZoneRepulsor,
	CWorld,
	CWorldVguiText,
	DustTrail,
	MovieExplosion,
	ParticleSmokeGrenade,
	RocketTrail,
	SmokeTrail,
	SporeExplosion,
	SporeTrail,
};

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

namespace SkinChanger
{
	static int ct_default = -1;
	static int t_default = -1;
	static int butterfly = -1;

	void GetModelIndices()
	{
		ct_default = g_ModelInfoClient->GetModelIndex("models/weapons/v_knife_default_ct.mdl");
		t_default = g_ModelInfoClient->GetModelIndex("models/weapons/v_knife_default_t.mdl");

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

		CBaseHandle* weapons = &localPlayer->m_hMyWeapons();

		for (int i = 0; i < MAX_WEAPONS; i++)
		{
			auto weapon = reinterpret_cast<C_BaseCombatWeapon*>(g_EntityList->GetClientEntityFromHandle(weapons[i]));
			if (!weapon)
			{
				break;
			}

			// Set item id high to -1 so that fallback settings are used
			weapon->m_iItemIDHigh() = -1;

			// Set some generic fallback settings
			weapon->m_flFallbackWear() = 0.000000001f;
			weapon->m_nFallbackSeed() = 0;
			weapon->m_nFallbackStatTrak() = 6969;

			// TODO: Have an array for each weapon econ index and define paint kits, wear etc for all
			switch (weapon->GetClientClass()->m_ClassId)
			{
			case CKnife:

				// Set the weapon index to the correct knife
				weapon->m_iItemDefinitionIndex() = 515;

				// Set all models correctly so that animations aren't bugged
				weapon->m_nModelIndex() = butterfly;
				weapon->m_iViewModelIndex() = butterfly;
				weapon->m_iWorldModelIndex() = butterfly + 1;
				weapon->m_iWorldDroppedModelIndex() = butterfly + 2;

				// Set the skin
				weapon->m_iEntityQuality() = AE_UNUSUAL;
				weapon->m_nFallbackPaintKit() = 38;

				break;
			case CAK47:
				weapon->m_nFallbackPaintKit() = 180;
				break;
			case CWeaponAWP:
				weapon->m_nFallbackPaintKit() = 344;
				weapon->m_iEntityQuality() = AE_TOURNAMENT;
				break;
			case CWeaponM4A1:
				weapon->m_nFallbackPaintKit() = 309;
				weapon->m_iEntityQuality() = AE_TOURNAMENT;
				break;
			}
		}

	}

	RecvProxyFn ogRecvProxy_nModelIndex = nullptr;
	void RecvProxy_nModelIndex(const CRecvProxyData* pData, void* pStruct, void* pOut)
	{
		// TODO: Do we need to call the original unconditionally?
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

}