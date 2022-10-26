#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Psapi.h>


namespace Utils
{
	/// <summary>
	/// Scan for a signature in a given module
	/// </summary>
	/// <param name="moduleName">Name of the module to scan</param>
	/// <param name="signature">Signature to scan for. Must be an escaped string of bytes</param>
	/// <param name="mask">Signature mask where ? denotes a non-static byte</param>
	/// <returns>Address of the found signature. nullptr if not found</returns>
	BYTE* SigScan(const char* moduleName, const char* signature, const char* mask);

	/// <summary>
	/// Scan for a signature in a given memory region
	/// </summary>
	/// <param name="base">Address to start scanning from</param>
	/// <param name="size">Size of the memory region to scan</param>
	/// <param name="signature">Signature to scan for. Must be an escaped string of bytes</param>
	/// <param name="mask">Signature mask where ? denotes a non-static byte</param>
	/// <returns>Address of the found signature. nullptr if not found</returns>
	BYTE* SigScan(void* base, size_t size, const char* signature, const char* mask);

	/// <summary>
	/// Scan through a module for a gadget that can be used for spoofing return addresses
	///	The gadget we use is "jmp [ebx]" (\xFF\x23) which should be present in every dll 
	/// </summary>
	/// <param name="moduleName">Name of the module to scan</param>
	/// <returns>Pointer to jmp [ebx] instruction</returns>
	void* GetRetAddrSpoofGadget(const char* moduleName);

	namespace Spoof
	{

		struct SavedVals
		{
			void* gadgetRetAddr;
			void* savedRetAddr;
			void* savedEbx;
		};

		template<typename T = void, typename... Args>
		__declspec(naked) T __stdcall StdCall(void* functionToCall, SavedVals* savedVals, void* gadgetAddr, Args... args)
		{
			/*
			Current stack layout (low - high)
			
			<retAddr>			<<< esp
			<functionToCall>
			<savedValsPtr>
			<gadgetAddr>
			<args...>
			<callerSavedRbp>	<<< ebp
			<callerRetAddr>		
			*/

			__asm
			{
				mov ecx, [esp + 8]			// move pointer to saved vals into ecx
				mov [ecx + 8], ebx			// store the value of ebx in savedVals->savedEbx
				lea ebx, gadgetReturn		// load the address of where we want the gadget to jump to
				mov [ecx], ebx				// store the loaded address into savedVals->gadgetRetAddr
				pop dword ptr [ecx + 4]		// store the current return address in savedVals->savedRetAddr	

				/*
				Current stack layout (low - high)

				<functionToCall>	<<< esp
				<savedValsPtr>
				<gadgetAddr>
				<args...>
				<callerSavedRbp>	<<< ebp
				<callerRetAddr>
				*/

				mov ebx, ecx				// move the saved vals pointer into ebx (the gadget is 'jmp, [ebx]' so it will jump to savedVals->gadgetRetAddr)
				ret 0x4						// return to the function we want to call, removing the SavedVals pointer from the stack
											//		this means that the return address from functionToCall will be the address of our gadget

			gadgetReturn:
				mov ecx, ebx				// move the saved vals pointer into ecx
				mov ebx, [ecx + 8]			// restore the saved ebx value
				push dword ptr [ecx + 4]	// restore the original return address
				ret
			}
		}
	}

	template<typename T = void, typename... Args>
	T SpoofStdCall(void* functionToCall, void* gadgetAddr, Args... args)
	{
		Spoof::SavedVals savedVals = { 0 };
		return Spoof::StdCall<T>(functionToCall, &savedVals, gadgetAddr, args...);
	}
	
	// TODO: spoof __thiscall and __fastcall
	//		__thiscall with edx set to 0 is the same as __fastcall
}