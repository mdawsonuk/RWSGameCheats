#pragma once

#include "sdk/common.h"


struct TCombinedResults;
typedef unsigned short MDLHandle_t;
typedef void (*CombinedModelLoadedCallback)(void* pUserData, MDLHandle_t OldHandle, MDLHandle_t NewHandle, TCombinedResults& CombinedResults);

class Vector;
class IMaterial;
class CUtlBuffer;
class QAngle;
class CGameTrace;
typedef CGameTrace trace_t;
class IClientRenderable;
class CPhysCollide;
class IModelLoadCallback;
class KeyValues;

struct model_t;
struct vcollide_t;
struct studiohdr_t;
struct virtualmodel_t;
struct cplane_t;
struct SCombinerModelInput_t;

enum RenderableTranslucencyType_t;


class IVModelInfo
{
public:
	virtual							~IVModelInfo(void) { }

	virtual const model_t* GetModel(int modelindex) const = 0;
	// Returns index of model by name
	virtual int						GetModelIndex(const char* name) const = 0;

	// Returns name of model
	virtual const char* GetModelName(const model_t* model) const = 0;
	virtual vcollide_t* GetVCollide(const model_t* model) const = 0;
	virtual vcollide_t* GetVCollide(int modelindex) const = 0;
	virtual void					GetModelBounds(const model_t* model, Vector& mins, Vector& maxs) const = 0;
	virtual	void					GetModelRenderBounds(const model_t* model, Vector& mins, Vector& maxs) const = 0;
	virtual int						GetModelFrameCount(const model_t* model) const = 0;
	virtual int						GetModelType(const model_t* model) const = 0;
	virtual void* GetModelExtraData(const model_t* model) = 0;
	virtual bool					ModelHasMaterialProxy(const model_t* model) const = 0;
	virtual bool					IsTranslucent(model_t const* model) const = 0;
	virtual bool					IsTranslucentTwoPass(const model_t* model) const = 0;
	virtual void					Unused0() {};
	virtual RenderableTranslucencyType_t ComputeTranslucencyType(const model_t* model, int nSkin, int nBody) = 0;
	virtual int						GetModelMaterialCount(const model_t* model) const = 0;
	virtual int 					GetModelMaterials(const model_t* model, int count, IMaterial** ppMaterial) = 0;
	virtual bool					IsModelVertexLit(const model_t* model) const = 0;
	virtual const char* GetModelKeyValueText(const model_t* model) = 0;
	virtual bool					GetModelKeyValue(const model_t* model, CUtlBuffer& buf) = 0; // supports keyvalue blocks in submodels
	virtual float					GetModelRadius(const model_t* model) = 0;

	virtual const studiohdr_t* FindModel(const studiohdr_t* pStudioHdr, void** cache, const char* modelname) const = 0;
	virtual const studiohdr_t* FindModel(void* cache) const = 0;
	virtual	virtualmodel_t* GetVirtualModel(const studiohdr_t* pStudioHdr) const = 0;
	virtual byte* GetAnimBlock(const studiohdr_t* pStudioHdr, int nBlock, bool bPreloadIfMissing) const = 0;
	virtual bool					HasAnimBlockBeenPreloaded(const studiohdr_t* pStudioHdr, int nBlock) const = 0;

	// Available on client only!!!
	virtual void					GetModelMaterialColorAndLighting(const model_t* model, Vector const& origin,
		QAngle const& angles, trace_t* pTrace,
		Vector& lighting, Vector& matColor) = 0;
	virtual void					GetIlluminationPoint(const model_t* model, IClientRenderable* pRenderable, Vector const& origin,
		QAngle const& angles, Vector* pLightingCenter) = 0;

	virtual int						GetModelContents(int modelIndex) const = 0;
	virtual studiohdr_t* GetStudiomodel(const model_t* mod) = 0;
	virtual int						GetModelSpriteWidth(const model_t* model) const = 0;
	virtual int						GetModelSpriteHeight(const model_t* model) const = 0;

	// Sets/gets a map-specified fade range (client only)
	virtual void					SetLevelScreenFadeRange(float flMinSize, float flMaxSize) = 0;
	virtual void					GetLevelScreenFadeRange(float* pMinArea, float* pMaxArea) const = 0;

	// Sets/gets a map-specified per-view fade range (client only)
	virtual void					SetViewScreenFadeRange(float flMinSize, float flMaxSize) = 0;

	// Computes fade alpha based on distance fade + screen fade (client only)
	virtual unsigned char			ComputeLevelScreenFade(const Vector& vecAbsOrigin, float flRadius, float flFadeScale) const = 0;
	virtual unsigned char			ComputeViewScreenFade(const Vector& vecAbsOrigin, float flRadius, float flFadeScale) const = 0;

	// both client and server
	virtual int						GetAutoplayList(const studiohdr_t* pStudioHdr, unsigned short** pAutoplayList) const = 0;

	// Gets a virtual terrain collision model (creates if necessary)
	// NOTE: This may return NULL if the terrain model cannot be virtualized
	virtual CPhysCollide* GetCollideForVirtualTerrain(int index) = 0;

	virtual bool					IsUsingFBTexture(const model_t* model, int nSkin, int nBody, void /*IClientRenderable*/* pClientRenderable) const = 0;

	virtual const model_t* FindOrLoadModel(const char* name) const = 0;

	virtual MDLHandle_t				GetCacheHandle(const model_t* model) const = 0;

	// Returns planes of non-nodraw brush model surfaces
	virtual int						GetBrushModelPlaneCount(const model_t* model) const = 0;
	virtual void					GetBrushModelPlane(const model_t* model, int nIndex, cplane_t& plane, Vector* pOrigin) const = 0;
	virtual int						GetSurfacepropsForVirtualTerrain(int index) = 0;
	virtual bool					UsesEnvCubemap(const model_t* model) const = 0;
	virtual bool					UsesStaticLighting(const model_t* model) const = 0;

	// Returns index of model by name, dynamically registered if not already known.
	virtual int						RegisterDynamicModel(const char* name, bool bClientSide) = 0;
	virtual int						RegisterCombinedDynamicModel(const char* pszName, MDLHandle_t Handle) = 0;
	virtual void					UpdateCombinedDynamicModel(int nModelIndex, MDLHandle_t Handle) = 0;

	virtual int						BeginCombinedModel(const char* pszName, bool bReuseExisting) = 0;
	virtual bool					SetCombineModels(int nModelIndex, const CUtlVector< SCombinerModelInput_t >& vecModelsToCombine) = 0;
	virtual bool					FinishCombinedModel(int nModelIndex, CombinedModelLoadedCallback pFunc, void* pUserData = 0) = 0;
	virtual void					ReleaseCombinedModel(int nModelIndex) = 0;

	virtual bool					IsDynamicModelLoading(int modelIndex) = 0;
	virtual void					AddRefDynamicModel(int modelIndex) = 0;
	virtual void					ReleaseDynamicModel(int modelIndex) = 0;

	// Registers callback for when dynamic model has finished loading.
	virtual bool					RegisterModelLoadCallback(int modelindex, IModelLoadCallback* pCallback, bool bCallImmediatelyIfLoaded = true) = 0;
	virtual void					UnregisterModelLoadCallback(int modelindex, IModelLoadCallback* pCallback) = 0;

	// Poked by engine
	virtual void					OnLevelChange() = 0;

	virtual KeyValues* GetModelKeyValues(const model_t* pModel) = 0;

	virtual void					UpdateViewWeaponModelCache(const char** ppWeaponModels, int nWeaponModels) = 0;
	virtual void					TouchWorldWeaponModelCache(const char** ppWeaponModels, int nWeaponModels) = 0;
};


class IVModelInfoClient : public IVModelInfo
{
public:
	// Returns a model as a client-side index which is stable across server changes
	virtual int						GetModelClientSideIndex(const char* name) const = 0;

	// Poked by engine
	virtual void					OnDynamicModelStringTableChanged(int nStringIndex, const char* pString, const void* pData) = 0;

	// Reference and Unload
	// Don't assume Reference flags - doing this for now
	virtual model_t* ReferenceModel(const char* name) = 0;
	virtual void					UnreferenceModel(model_t* model) = 0;
	virtual void					UnloadUnreferencedModels(void) = 0;
};