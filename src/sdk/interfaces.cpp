#include "sdk\interfaces.h"

#include <stdio.h>

#include "utils.h"

// Define all interfaces from interfaces.h here
IBaseClientDLL* g_ClientDLL = nullptr; 
IClientMode* g_ClientMode = nullptr;


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

		// Then use GET_INTERFACE with the global var, interface type, factory, and the interface name/version
		// 
		//		To find interface names, search for strings in the dll with the format V<name><version>
		//			e.g. IBaseClientDLL is currently VClient018
		GET_INTERFACE(g_ClientDLL, IBaseClientDLL, clientFactory, "VClient018");
		

		// Unfortunately, some of the interfaces that are needed for common cheats aren't actually "exported" like above.
		//		So we need to find a place in code where the pointer is used and create a signature that we scan scan for.
		//
		//		TODO: Explain how this was found and the code
		g_ClientMode = **reinterpret_cast<IClientMode***>(Utils::SigScan("client.dll", "\x8B\x0D\x00\x00\x00\x00\x8B\x01\xFF\x50\x1C\x85", "xx????xxxxxx") + 2);
		DUMP_INTERFACE(g_ClientMode);




		return true;
	}
}