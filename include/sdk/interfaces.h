#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "interfaces/IBaseClientDLL.h"
#include "interfaces/IClientMode.h"
#include "interfaces/IClientEntityList.h"
#include "interfaces/IVModelRender.h"
#include "interfaces/IVModelInfo.h"
#include "interfaces/IGameResources.h"
#include "interfaces/IVEngineClient.h"

// This feels wrong
#include "classes/C_PlayerResource.h"

extern IBaseClientDLL* g_ClientDLL;
extern IClientMode* g_ClientMode;
extern IClientEntityList* g_EntityList;
extern IVModelRender* g_ModelRender;
extern IMaterialSystem* g_MaterialSystem;
extern IVModelInfoClient* g_ModelInfoClient;
extern IVEngineClient* g_EngineClient;

extern C_PlayerResource** g_PlayerResource;

extern C_BasePlayer** g_LocalPlayer;


namespace Interfaces
{
	// Get the exported CreateInterface function from a loaded dll
	CreateInterfaceFn GetCreateInterfaceFn(const char* moduleName);
	
	// Setup all required interfaces, must be done before the interfaces are used...
	bool SetupInterfaces();
	

	// Convenience wrapper that casts the returned void* from CreateInterface to the correct type
	template<class T = void>
	T* GetInterface(CreateInterfaceFn createInterfaceFn, const char* interfaceName)
	{
		return reinterpret_cast<T*>(createInterfaceFn(interfaceName, nullptr));
	}
}