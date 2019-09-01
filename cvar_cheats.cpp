#include "cvar_cheats.h"
#include "SpoofedConvar.h"
#include "valve_sdk\sdk.hpp"
#include "options.hpp"
#include "Globals.h"


void cvar_cheats::wpn_scale()
{
	static bool isSpoofed = false;
	static SpoofedConvar* spoofed = nullptr;

	if (!isSpoofed) 
	{
		spoofed = new SpoofedConvar(g_CVar->FindVar("cl_wpn_sway_scale"));
		isSpoofed = true;
	}

	if(spoofed)
		spoofed->SetFloat(options.weapon_sway);
}

void cvar_cheats::particles()
{
	//insane coding skill here

	static bool isSpoofed = true;
	static bool wasEnabled = false;
	static SpoofedConvar* spoofed = nullptr;

	if (options.particleremove && options.vis_misc_enabled)
	{
		if (!isSpoofed)
		{
			spoofed = new SpoofedConvar(cvars->FindVar("r_drawparticles"));
			isSpoofed = true;
		}

		if (spoofed)
			spoofed->SetInt(0);

		wasEnabled = true;
	}
	else if(wasEnabled && spoofed)
	{
		spoofed->SetInt(1);
		wasEnabled = false;
	}
}

void cvar_cheats::no_fog() 
{
	static bool isSpoofed = true;
	static bool wasEnabled = false;
	static SpoofedConvar* spoofed_1 = nullptr;
	static SpoofedConvar* spoofed_2 = nullptr;

	if (options.misc_nofog && options.vis_misc_enabled)
	{
		if (!isSpoofed)
		{
			spoofed_1 = new SpoofedConvar(cvars->FindVar("fog_enable"));
			spoofed_2 = new SpoofedConvar(cvars->FindVar("fog_override"));
			isSpoofed = true;
		}

		if (spoofed_1 && spoofed_2) 
		{
			spoofed_1->SetInt(0); //fog_enable 0
			spoofed_2->SetInt(1); //fog_override 1
		}
		
		wasEnabled = true;
	}
	else if (wasEnabled && spoofed_1 && spoofed_2)
	{
		spoofed_1->SetInt(1);
		spoofed_2->SetInt(0);
		wasEnabled = false;
	}
}

void cvar_cheats::run_all()
{
	wpn_scale();
	particles();
	no_fog();
}