#pragma once

#include <memory>

/// <summary>
/// Class for hooking virtual method tables
/// </summary>
class VMTHook
{
public:

	/// <summary>
	/// Default constructor
	/// </summary>
	VMTHook();

	/// <summary>
	/// Main constructor which sets up the hook vtable ready for hooking
	/// </summary>
	/// <param name="vtable">Address of the start of the vtable</param>
	VMTHook(void** vtable);

	
	~VMTHook();

public:

	/// <summary>
	/// Replace a pointer in the hook vtable with a new function
	/// </summary>
	/// <param name="index">Index of the function to hook</param>
	/// <param name="hookFn">Address of the hook handler</param>
	void Hook(int index, void* hookFn);

	/// <summary>
	/// Swap the original vtable pointer with a pointer to the hook vtable
	/// </summary>
	void EnableHooks();

	/// <summary>
	/// Swap the hook vtable pointer with a pointer to the original vtable
	/// </summary>
	void DisableHooks();

	/// <summary>
	/// Get the address of an unhooked function
	/// </summary>
	/// <typeparam name="T">Function typedef (void* by default)</typeparam>
	/// <param name="index">Index of the original function in the vtable</param>
	/// <returns>Address of an original function, cast to typename T</returns>
	template<typename T = void*>
	T GetOriginalFn(int index)
	{
		return reinterpret_cast<T>(originalVtable[index]);
	}

private:

	/// <summary>
	/// Calculate the length of the vtable
	/// </summary>
	/// <returns></returns>
	int CalculateVtableLength();

private:

	void** vtablePtr;
	uintptr_t* originalVtable;
	int vtableLength;
	std::shared_ptr<uintptr_t[]> hookVtable;
};