#define NOMINMAX
#pragma once
#include "helpers/utils.hpp"
#include "helpers/input.hpp"
#include "hooks.hpp"
#include "menu.hpp"
#include "options.hpp"
#include "lazy_importer.h"
#pragma once
DWORD WINAPI OnDllAttach(LPVOID base)
{
    if(Utils::WaitForModules(10000, { L"client.dll", L"engine.dll", L"shaderapidx9.dll" }) == WAIT_TIMEOUT)
        return FALSE;
    
	
#ifdef _DEBUG
    Utils::attach_console();
#endif
	
    try {


        Utils::ConsolePrint("Initializing... ");

        Interfaces::Initialize();

        NetvarSys::get().Initialize();
        InputSys::get().Initialize();
        Menu::get().Initialize();

        Hooks::Initialize();

        // Register some hotkeys.
        // - Note:  The function that is called when the hotkey is pressed
        //          is called from the WndProc thread, not this thread.
        // 

        // Panic button
        InputSys::get().RegisterHotkey(VK_HOME, []() {
            g_Unload = true;
        });

        // Menu Toggle
        InputSys::get().RegisterHotkey(VK_DELETE, []() {
            Menu::get().Toggle();
        });

        Utils::ConsolePrint("\nFinished.\n");
		
        while(!g_Unload)
			LI_FIND_CACHED(Sleep)(1000);

        g_CVar->FindVar("crosshair")->SetValue(true);
		
		
        FreeLibraryAndExitThread(static_cast<HMODULE>(base), 1);

    } catch(const std::exception& ex) {
        Utils::ConsolePrint("An error occured during initialization:\n");
        Utils::ConsolePrint("%s\n", ex.what());
        Utils::ConsolePrint("Press any key to exit.\n");
        Utils::ConsoleReadKey();
        Utils::DetachConsole();

        FreeLibraryAndExitThread(static_cast<HMODULE>(base), 1);
    }

    // unreachable
    //return TRUE;
}

BOOL WINAPI OnDllDetach()
{

	

#ifdef _DEBUG
	Utils::ConsolePrint( "shutting down...\n" );
    Utils::DetachConsole();
#endif

    Hooks::Shutdown();

    Menu::get().Shutdown();

	LI_FIND_CACHED(Sleep)(1000);
    return TRUE;
}

BOOL WINAPI DllMain(
    _In_      HINSTANCE hinstDll,
    _In_      DWORD     fdwReason,
    _In_opt_  LPVOID    lpvReserved
)
{
	
    switch(fdwReason) {
        case DLL_PROCESS_ATTACH:
		//	LI_FIND_CACHED( LoadLibraryA )( "user32.dll" );
		//	LI_FIND_CACHED( LoadLibraryA )( "kernel32.dll" );
            DisableThreadLibraryCalls(hinstDll);
            CreateThread(nullptr, 0, OnDllAttach, hinstDll, 0, nullptr);
            return TRUE;
        case DLL_PROCESS_DETACH:
            if(lpvReserved == nullptr)
                return OnDllDetach();
            return TRUE;
        default:
            return TRUE;
    }
}
