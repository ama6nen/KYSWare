#include "sky_changer.h"


void sky_changer::change(const char* name)
{
	//tbh idk if any class is actually needed for such a small feature like 2 lines but whatever :D

	static auto LoadNamedSky = reinterpret_cast<void(__fastcall*)(const char*)>(Utils::PatternScan( LI_FIND_CACHED( GetModuleHandleA )("engine.dll"), ("55 8B EC 81 EC ? ? ? ? 56 57 8B F9 C7 45")));

	LoadNamedSky(name);

	Utils::ConsolePrint("Changed sky to %s \n", name);
}