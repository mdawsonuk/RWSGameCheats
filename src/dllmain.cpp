#include <stdio.h>


#include "sdk/interfaces.h"
#include "sdk/netvars.h"
#include "hooks.h"

bool attached = false;
FILE* conout = nullptr;
FILE* conin = nullptr;
FILE* conerr = nullptr;

/// <summary>
/// Called when the cheat module is being unloaded
/// 
/// Disables all hooks and cleans up all resources used
/// </summary>
void ProcessDetach()
{
    // Disable all hooks
    Hooks::CleanupHooks();

    // TODO: Any other necessary clean up


#ifdef _DEBUG
    fclose(conout);
    fclose(conin);
    fclose(conerr);
    FreeConsole();
#endif
}

/// <summary>
/// Called when the cheat module is attached to the CS:GO process.
/// 
/// Sets up all interfaces, hooks and other resources.
/// Everything set up here needs to be cleaned up in ProcessDetach.
/// </summary>
bool ProcessAttach()
{

#ifdef _DEBUG
    // If this is a debug build, allocate the console
    // and redirect IO to it
    AllocConsole();
    freopen_s(&conout, "CONOUT$", "w", stdout);
    freopen_s(&conin, "CONIN$", "r", stdin);
    freopen_s(&conerr, "CONERR$", "w", stderr);
#endif

    // Setup all interfaces
    bool success = Interfaces::SetupInterfaces();
    if (!success)
    {
        ProcessDetach();
        return false;
    }

    // Setup netvars before hooks because hooks will use them
    success = Netvars::SetupNetvars();
    if (!success)
    {
        ProcessDetach();
        return false;
    }


    // Hooks must be set up last
    success = Hooks::SetupHooks();
    if (!success)
    {
        ProcessDetach();
        return false;
    }

    attached = true;
    return true;
}



// DllEntry
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    // Perform actions based on the reason for calling.
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        
        // Don't allow repeated injections
        if (attached)
        {
            return FALSE;
        }

        // We aren't starting a thread here since everything is based on hooks
        return ProcessAttach();
        
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        return TRUE;

    case DLL_PROCESS_DETACH:

        // Don't do cleanup when process is terminating
        if (lpvReserved != nullptr)
        {
            break;
        }

        ProcessDetach();
        break;

    }
    return TRUE;
}