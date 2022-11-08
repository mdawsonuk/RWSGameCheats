#pragma once

#include "sdk/common.h"

#include "IStudioRender.h"
#include "sdk/classes/Vector.h"
#include "sdk/classes/QAngle.h"

typedef void* LightCacheHandle_t;
typedef void* StudioDecalHandle_t;
typedef unsigned short ModelInstanceHandle_t;
typedef void* DataCacheHandle_t;
typedef void* FileNameHandle_t;

class IClientRenderable;
class Vector;
class IMaterial;
class CStudioHdr;
class matrix3x4_t;
class matrix3x4a_t;

struct MaterialLightingState_t;
struct studiohdr_t;
struct studiohwdata_t;
struct mstudioanimdesc_t;
struct mstudioseqdesc_t;
enum modtype_t
{
	mod_bad = 0,
	mod_brush,
	mod_sprite,
	mod_studio
};
struct worldbrushdata_t;
struct brushdata_t
{
	worldbrushdata_t* pShared;
	int				firstmodelsurface;
	int				nummodelsurfaces;

	// track the union of all lightstyles on this brush.  That way we can avoid
	// searching all faces if the lightstyle hasn't changed since the last update
	int				nLightstyleLastComputedFrame;
	unsigned short	nLightstyleIndex;	// g_ModelLoader actually holds the allocated data here
	unsigned short	nLightstyleCount;

	unsigned short	renderHandle;
	unsigned short	firstnode;
};
struct spritedata_t
{
	int				numframes;
	int				width;
	int				height;
	CEngineSprite* sprite;
};
struct model_t
{
	FileNameHandle_t	fnHandle;
	char				szPathName[260];

	int					nLoadFlags;		// mark loaded/not loaded
	int					nServerCount;	// marked at load

	modtype_t			type;
	int					flags;			// MODELFLAG_???

	// volume occupied by the model graphics	
	Vector				mins, maxs;
	float				radius;
	KeyValues* m_pKeyValues;
	union
	{
		brushdata_t		brush;
		MDLHandle_t		studio;
		spritedata_t	sprite;
	};

};
struct Ray_t;
struct StaticPropRenderInfo_t;
struct DrawModelState_t
{
	studiohdr_t* m_pStudioHdr;
	studiohwdata_t* m_pStudioHWData;
	IClientRenderable* m_pRenderable;
	const matrix3x4_t* m_pModelToWorld;
	StudioDecalHandle_t		m_decals;
	int						m_drawFlags;
	int						m_lod;
};
struct ModelRenderInfo_t
{
	Vector origin;
	QAngle angles;
	char padding[0x4];
	IClientRenderable* pRenderable;
	const model_t* pModel;
	const matrix3x4_t* pModelToWorld;
	const matrix3x4_t* pLightingOffset;
	const Vector* pLightingOrigin;
	int flags;
	int entity_index;
	int skin;
	int body;
	int hitboxset;
	ModelInstanceHandle_t instance;

	ModelRenderInfo_t()
	{
		pModelToWorld = NULL;
		pLightingOffset = NULL;
		pLightingOrigin = NULL;
	}
};
struct LightingQuery_t;
struct StaticLightingQuery_t;

class IVModelRender
{
public:
	virtual int		DrawModel(int flags,
		IClientRenderable* pRenderable,
		ModelInstanceHandle_t instance,
		int entity_index,
		const model_t* model,
		Vector const& origin,
		QAngle const& angles,
		int skin,
		int body,
		int hitboxset,
		const matrix3x4_t* modelToWorld = NULL,
		const matrix3x4_t* pLightingOffset = NULL) = 0;

	// This causes a material to be used when rendering the model instead 
	// of the materials the model was compiled with
	virtual void	ForcedMaterialOverride(IMaterial* newMaterial, OverrideType_t nOverrideType = OVERRIDE_NORMAL, int nMaterialIndex = -1) = 0;
	virtual bool	IsForcedMaterialOverride() = 0;

	virtual void	SetViewTarget(const CStudioHdr* pStudioHdr, int nBodyIndex, const Vector& target) = 0;

	// Creates, destroys instance data to be associated with the model
	virtual ModelInstanceHandle_t CreateInstance(IClientRenderable* pRenderable, LightCacheHandle_t* pCache = NULL) = 0;
	virtual void DestroyInstance(ModelInstanceHandle_t handle) = 0;

	// Associates a particular lighting condition with a model instance handle.
	// FIXME: This feature currently only works for static props. To make it work for entities, etc.,
	// we must clean up the lightcache handles as the model instances are removed.
	// At the moment, since only the static prop manager uses this, it cleans up all LightCacheHandles 
	// at level shutdown.
	virtual void SetStaticLighting(ModelInstanceHandle_t handle, LightCacheHandle_t* pHandle) = 0;
	virtual LightCacheHandle_t GetStaticLighting(ModelInstanceHandle_t handle) = 0;

	// moves an existing InstanceHandle to a nex Renderable to keep decals etc. Models must be the same
	virtual bool ChangeInstance(ModelInstanceHandle_t handle, IClientRenderable* pRenderable) = 0;

	// Creates a decal on a model instance by doing a planar projection
	// along the ray. The material is the decal material, the radius is the
	// radius of the decal to create.
	virtual void AddDecal(ModelInstanceHandle_t handle, Ray_t const& ray,
		Vector const& decalUp, int decalIndex, int body, bool noPokeThru = false, int maxLODToDecal = ADDDECAL_TO_ALL_LODS, IMaterial* pSpecifyMaterial = NULL, float w = 1.0f, float h = 1.0f, void* pvProxyUserData = NULL, int nAdditionalDecalFlags = 0) = 0;

	// Removes all the decals on a model instance
	virtual void RemoveAllDecals(ModelInstanceHandle_t handle) = 0;

	// Returns true if the model instance is valid and the decal list is not empty
	virtual bool ModelHasDecals(ModelInstanceHandle_t handle) = 0;

	// Remove all decals from all models
	virtual void RemoveAllDecalsFromAllModels(bool bRenderContextValid) = 0;

	// Shadow rendering, DrawModelShadowSetup returns the address of the bone-to-world array, NULL in case of error
	virtual matrix3x4a_t* DrawModelShadowSetup(IClientRenderable* pRenderable, int body, int skin, DrawModelInfo_t* pInfo, matrix3x4a_t* pCustomBoneToWorld = NULL) = 0;
	virtual void DrawModelShadow(IClientRenderable* pRenderable, const DrawModelInfo_t& info, matrix3x4a_t* pCustomBoneToWorld = NULL) = 0;

	// This gets called when overbright, etc gets changed to recompute static prop lighting.
	virtual bool RecomputeStaticLighting(ModelInstanceHandle_t handle) = 0;

	virtual void ReleaseAllStaticPropColorData(void) = 0;
	virtual void RestoreAllStaticPropColorData(void) = 0;

	// Extended version of drawmodel
	virtual int	DrawModelEx(ModelRenderInfo_t& pInfo) = 0;

	virtual int	DrawModelExStaticProp(IMatRenderContext* pRenderContext, ModelRenderInfo_t& pInfo) = 0;

	virtual bool DrawModelSetup(IMatRenderContext* pRenderContext, ModelRenderInfo_t& pInfo, DrawModelState_t* pState, matrix3x4_t** ppBoneToWorldOut) = 0;
	virtual void DrawModelExecute(IMatRenderContext* pRenderContext, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld = NULL) = 0;

	// Sets up lighting context for a point in space
	virtual void SetupLighting(const Vector& vecCenter) = 0;

	// doesn't support any debug visualization modes or other model options, but draws static props in the
	// fastest way possible
	virtual int DrawStaticPropArrayFast(StaticPropRenderInfo_t* pProps, int count, bool bShadowDepth) = 0;

	// Allow client to override lighting state
	virtual void SuppressEngineLighting(bool bSuppress) = 0;

	virtual void SetupColorMeshes(int nTotalVerts) = 0;

	// Sets up lighting context for a point in space, with smooth interpolation per model.
	// Passing MODEL_INSTANCE_INVALID as a handle is equivalent to calling SetupLighting.
	virtual void SetupLightingEx(const Vector& vecCenter, ModelInstanceHandle_t handle) = 0;

	// Finds the brightest light source illuminating a point. Returns false if there isn't any.
	virtual bool GetBrightestShadowingLightSource(const Vector& vecCenter, Vector& lightPos, Vector& lightBrightness, bool bAllowNonTaggedLights) = 0;

	// Computes lighting state for an array of lighting requests
	virtual void ComputeLightingState(int nCount, const LightingQuery_t* pQuery, MaterialLightingState_t* pState, ITexture** ppEnvCubemapTexture) = 0;

	// Gets an array of decal handles given model instances
	virtual void GetModelDecalHandles(StudioDecalHandle_t* pDecals, int nDecalStride, int nCount, const ModelInstanceHandle_t* pHandles) = 0;

	// Computes lighting state for an array of lighting requests for renderables which use static lighting
	virtual void ComputeStaticLightingState(int nCount, const StaticLightingQuery_t* pQuery, MaterialLightingState_t* pState, MaterialLightingState_t* pDecalState, ColorMeshInfo_t** ppStaticLighting, ITexture** ppEnvCubemapTexture, DataCacheHandle_t* pColorMeshHandles) = 0;

	// Cleans up lighting state. Must be called after the draw call that uses
	// the color meshes return from ComputeStaticLightingState has been issued
	virtual void CleanupStaticLightingState(int nCount, DataCacheHandle_t* pColorMeshHandles) = 0;
};