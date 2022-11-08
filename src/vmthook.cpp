#include "vmthook.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

VMTHook::VMTHook()
	: vtablePtr(nullptr), originalVtable(nullptr), vtableLength(0), hookVtable(nullptr)
{
}

VMTHook::VMTHook(void** vtable)
	: vtablePtr(vtable)
{
	originalVtable = *reinterpret_cast<uintptr_t**>(vtable);
	
	// Calculate length of the vtable and allocate memory for the hook vtable
	//		Allocated hook vtable is length + 1 because originalVtable[-1] is a pointer
	//		to the class RTTI. (See https://learn.microsoft.com/en-us/cpp/cpp/run-time-type-information?view=msvc-170)
	vtableLength = CalculateVtableLength();
	hookVtable = std::shared_ptr<uintptr_t[]>(new uintptr_t[vtableLength + 1]);
	
	// Copy the original vtable to the hook vtable
	memcpy(&hookVtable[0], &originalVtable[-1], (vtableLength + 1) * sizeof(uintptr_t));
}

VMTHook::~VMTHook()
{
	if (vtablePtr)
	{
		DisableHooks();
	}
}

void VMTHook::Hook(int index, void* hookFn)
{
	if (!vtablePtr)
	{
#ifdef _DEBUG
		printf_s("[!] Attempted to hook function when vtable hasn't been set\n");
#endif
		return;
	}

	// Replace pointer in hookVtable to the address of the hook handler
	hookVtable[index + 1] = reinterpret_cast<uintptr_t>(hookFn);
}

void VMTHook::EnableHooks()
{
	if (!vtablePtr)
	{
#ifdef _DEBUG
		printf_s("[!] Attempted to enable hooks when vtable hasn't been set\n");
#endif
		return;
	}

	DWORD oldProtect;
	VirtualProtect(vtablePtr, sizeof(uintptr_t), PAGE_READWRITE, &oldProtect);

	*vtablePtr = &hookVtable[1];

	VirtualProtect(vtablePtr, sizeof(uintptr_t), oldProtect, &oldProtect);
}

void VMTHook::DisableHooks()
{
	if (!vtablePtr)
	{
#ifdef _DEBUG
		printf_s("[!] Attempted to disable hooks when vtable hasn't been set\n");
#endif
		return;
	}

	DWORD oldProtect;
	VirtualProtect(vtablePtr, sizeof(uintptr_t), PAGE_READWRITE, &oldProtect);

	*vtablePtr = originalVtable;

	VirtualProtect(vtablePtr, sizeof(uintptr_t), oldProtect, &oldProtect);
}

int VMTHook::CalculateVtableLength()
{
	// Check the page protection at each function in the vtable
	//		The "address" after the final function won't be executable

	int size = -1;
	MEMORY_BASIC_INFORMATION mbi = { 0 };
	do
	{
		size++;
		VirtualQuery(reinterpret_cast<LPCVOID>(originalVtable[size]), &mbi, sizeof(mbi));
	} while (mbi.Protect & (PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_READ));

	return size;
}
