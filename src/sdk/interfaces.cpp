#include "sdk\interfaces.h"

#include <stdio.h>

#include "utils.h"

// Define all interfaces from interfaces.h here
IBaseClientDLL* g_ClientDLL = nullptr; 
IClientMode* g_ClientMode = nullptr;
IClientEntityList* g_EntityList = nullptr;
IVModelRender* g_ModelRender = nullptr;
IMaterialSystem* g_MaterialSystem = nullptr;
IVModelInfoClient* g_ModelInfoClient = nullptr;

C_BasePlayer** g_LocalPlayer = nullptr;
CGlowObjectManager** g_GlowManager = nullptr;

#define GET_FACTORY(var, moduleName) CreateInterfaceFn var = GetCreateInterfaceFn(moduleName); if (!var) return false;

#ifdef _DEBUG
	#define DUMP_INTERFACE(iface) printf_s("%-25s: 0x%p\n", #iface, iface);
	#define GET_INTERFACE(var, type, factory, interfaceName) var = GetInterface<type>(factory, interfaceName); if (!var) return false; DUMP_INTERFACE(var);
#else
	#define DUMP_INTERFACE(iface)
	#define GET_INTERFACE(var, type, factory, interfaceName) var = GetInterface<type>(factory, interfaceName); if (!var) return false;
#endif

#ifdef _DEBUG
#else
#endif

namespace Interfaces
{
	CreateInterfaceFn GetCreateInterfaceFn(const char* moduleName)
	{
		auto module = GetModuleHandle(moduleName);
		if (!module)
		{
			return nullptr;
		}
		auto createInterfaceFn = GetProcAddress(module, "CreateInterface");
		if (!createInterfaceFn)
		{
			return nullptr;
		}

		return reinterpret_cast<CreateInterfaceFn>(createInterfaceFn);
	}

	bool SetupInterfaces()
	{
		// The two macros GET_FACTORY and GET_INTERFACE call GetInterfaceFactory and GetInterface with nullptr checks
		//		If a single call fails, the DLL injection will fail

		// First use GET_FACTORY to get the exported CreateInterface function from a module
		GET_FACTORY(clientFactory, "client.dll");
		GET_FACTORY(engineFactory, "engine.dll");
		GET_FACTORY(matSystemFactory, "materialsystem.dll");

		// Then use GET_INTERFACE with the global var, interface type, factory, and the interface name/version
		// 
		//		To find interface names, search for strings in the dll with the format V<name><version>
		//			e.g. IBaseClientDLL is currently VClient018
		GET_INTERFACE(g_ClientDLL, IBaseClientDLL, clientFactory, "VClient018");
		GET_INTERFACE(g_EntityList, IClientEntityList, clientFactory, "VClientEntityList003");

		GET_INTERFACE(g_ModelRender, IVModelRender, engineFactory, "VEngineModel016");
		GET_INTERFACE(g_ModelInfoClient, IVModelInfoClient, engineFactory, "VModelInfoClient004");

		GET_INTERFACE(g_MaterialSystem, IMaterialSystem, matSystemFactory, "VMaterialSystem080");

		// Unfortunately, some of the interfaces that are needed for common cheats aren't actually "exported" like above.
		//		So we need to find a place in code where the pointer is used and create a signature that we scan scan for.
		g_ClientMode = **reinterpret_cast<IClientMode***>(Utils::SigScan("client.dll", "\x8B\x0D\x00\x00\x00\x00\x8B\x01\xFF\x50\x1C\x85", "xx????xxxxxx") + 2);
		DUMP_INTERFACE(g_ClientMode);


		// We need the local player for a lot of features so store this as a global
		//		To find this signature I found the function C_BasePlayer::GetLocalPlayer which returns s_pLocalPlayer[0]
		//		I then looked in the client.dylib to find the GetLocalPlayer function
		//		Then checked where that function was used and found it is used in CHLClient::OnDemoPlaybackRestart
		//		I found that function in client.dll and matched up where the function was called in the mac dylib to
		//			the windows dll (the compiler had inlined the function, so it just dereferences g_pLocalPlayer)
		//
		//		Because the local player pointer will change between games, we can't dereference it fully so just store a pointer
		//		to where the local player pointer will be
		//		to use this in game we would do (*g_localPlayer)->SomePlayerFunction()
		g_LocalPlayer = *reinterpret_cast<C_BasePlayer***>(Utils::SigScan("client.dll", "\x8B\x35\x00\x00\x00\x00\x85\xF6\x74\x42\x8D", "xx????xxxxx") + 2);

		g_GlowManager = reinterpret_cast<CGlowObjectManager**>(Utils::SigScan("client.dll", "\x0F\x11\x05\x00\x00\x00\x00\x83\xC8\x01",  "xxx????xxx") + 3);



		return true;
	}
}