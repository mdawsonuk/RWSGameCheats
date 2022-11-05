#pragma once

#include "utils.h"
#include "vmthook.h"

namespace Hooks
{

	/// <summary>
	/// Setup all VMTHook classes and find ret addr spoofing gadgets in every dll
	/// where functions will be hooked
	/// </summary>
	/// <returns>true if everything is setup correctly</returns>
	bool SetupHooks();


	/// <summary>
	/// Disable all hooks
	/// </summary>
	void CleanupHooks();
}