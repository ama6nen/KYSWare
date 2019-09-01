#pragma once
#include "Globals.h"
#include "helpers\utils.hpp"
#include "MemAlloc.h"
class CFlashLightEffect
{
public:
	bool m_bIsOn; //0x0000 
	char pad_0x0001[0x3]; //0x0001
	int m_nEntIndex; //0x0004 
	WORD m_FlashLightHandle; //0x0008 
	char pad_0x000A[0x2]; //0x000A
	float m_flMuzzleFlashBrightness; //0x000C 
	float m_flFov; //0x0010 
	float m_flFarZ; //0x0014 
	float m_flLinearAtten; //0x0018 
	bool m_bCastsShadows; //0x001C 
	char pad_0x001D[0x3]; //0x001D
	float m_flCurrentPullBackDist; //0x0020 
	DWORD m_MuzzleFlashTexture; //0x0024 
	DWORD m_FlashLightTexture; //0x0028 
	char m_szTextureName[64]; //0x1559888 
}; //Size=0x006C


class flashlight : public Singleton<flashlight>
{
	friend class Singleton<flashlight>;

private:
	CFlashLightEffect *CreateFlashLight(int nEntIndex, const char *pszTextureName, float flFov, float flFarZ, float flLinearAtten)
	{
		static DWORD oConstructor = (DWORD)Utils::PatternScan( LI_FIND_CACHED( GetModuleHandleA )("client.dll"), "55 8B EC F3 0F 10 45 ? B8");

		// we need to use the engine memory management if we are calling the destructor later
		CFlashLightEffect *pFlashLight = (CFlashLightEffect*)g_MemAlloc->Alloc(sizeof(CFlashLightEffect));

		if (!pFlashLight)
			return NULL;

		// we need to call this function on a non standard way
		__asm
		{
			movss xmm3, flFov
			mov ecx, pFlashLight
			push flLinearAtten
			push flFarZ
			push pszTextureName
			push nEntIndex
			call oConstructor
		}

		return pFlashLight;
	}
	void DestroyFlashLight(CFlashLightEffect *pFlashLight)
	{
		static DWORD oDestructor = (DWORD)Utils::PatternScan( LI_FIND_CACHED( GetModuleHandleA )("client.dll"), "56 8B F1 E8 ? ? ? ? 8B 4E 28");

		// the flash light destructor handles the memory management, so we dont need to free the allocated memory
		// call the destructor of the flashlight
		__asm
		{
			mov ecx, pFlashLight
			push ecx
			call oDestructor
		}
	}
	void UpdateFlashLight(CFlashLightEffect *pFlashLight, const Vector &vecPos, const Vector &vecForward, const Vector &vecRight, const Vector &vecUp)
	{
		// here we tell to the compiler wich calling convention we will use to call the function and the paramaters needed (note the __thiscall*)
		typedef void(__thiscall* UpdateLight_t)(void*, int, const Vector&, const Vector&, const Vector&, const Vector&, float, float, float, bool, const char*);

		static UpdateLight_t oUpdateLight = NULL;

		if (!oUpdateLight)
		{
			// here we have to deal with a call instruction (E8 rel32)
			DWORD callInstruction = (DWORD)Utils::PatternScan( LI_FIND_CACHED( GetModuleHandleA )("client.dll"), "E8 ? ? ? ? 8B 06 F3 0F 10 46"); // get the instruction address
			DWORD relativeAddress = *(DWORD*)(callInstruction + 1); // read the rel32
			DWORD nextInstruction = callInstruction + 5; // get the address of next instruction
			oUpdateLight = (UpdateLight_t)(nextInstruction + relativeAddress); // our function address will be nextInstruction + relativeAddress
		}

		oUpdateLight(pFlashLight, pFlashLight->m_nEntIndex, vecPos, vecForward, vecRight, vecUp, pFlashLight->m_flFov, pFlashLight->m_flFarZ, pFlashLight->m_flLinearAtten, pFlashLight->m_bCastsShadows, pFlashLight->m_szTextureName);
	}
public:
	void RunFrame(C_BasePlayer *pLocal)
	{
		static CFlashLightEffect *pFlashLight = NULL;

		if (!options.flashlight_on)
		{

			if (pFlashLight) //Destroy it once smh
			{
				DestroyFlashLight(pFlashLight);
				pFlashLight = NULL;
			}
			return;
		}
		if (GetAsyncKeyState('F') & 1 )
		{
			if (!pFlashLight)
			{
				//flashlight001
				pFlashLight = CreateFlashLight(pLocal->EntIndex(), "effects/flashlight001", 35, 1000, 1000);
			}
			else
			{
				DestroyFlashLight(pFlashLight);
				pFlashLight = NULL;
			}
		}

		if (pFlashLight)
		{
			QAngle viewAngles;
			engine->GetViewAngles(viewAngles);

			pFlashLight->m_bIsOn = true;
			pFlashLight->m_bCastsShadows = false;
			pFlashLight->m_flFov = options.flashlight_fov;
			UpdateFlashLight(pFlashLight, pLocal->GetEyePos(), viewAngles.Forward(), viewAngles.Right(), viewAngles.Up());
		}
	}
};
