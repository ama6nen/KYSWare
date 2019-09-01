#include "Utils.hpp"

#define NOMINMAX

#include <stdio.h>
#include <string>
#include <vector>

#include "../valve_sdk/csgostructs.hpp"
#include "Math.hpp"
#include <algorithm>
#include "../lazy_importer.h"
HANDLE _out = NULL, _old_out = NULL;
HANDLE _err = NULL, _old_err = NULL;
HANDLE _in = NULL, _old_in = NULL;

namespace Utils
{
    void attach_console()
    {

#ifdef _DEBUG
        _old_out = LI_FIND_CACHED( GetStdHandle) (STD_OUTPUT_HANDLE);
        _old_err = LI_FIND_CACHED( GetStdHandle )(STD_ERROR_HANDLE);
        _old_in  = LI_FIND_CACHED( GetStdHandle )(STD_INPUT_HANDLE);

		LI_FIND_CACHED(AllocConsole)() && LI_FIND_CACHED(AttachConsole)( LI_FIND_CACHED( GetCurrentProcessId )( ) );
		
        _out     = LI_FIND_CACHED( GetStdHandle )(STD_OUTPUT_HANDLE);
        _err     = LI_FIND_CACHED( GetStdHandle )(STD_ERROR_HANDLE);
        _in      = LI_FIND_CACHED( GetStdHandle )(STD_INPUT_HANDLE);

		LI_FIND_CACHED(SetConsoleMode)(_out,
            ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT);

		LI_FIND_CACHED(SetConsoleMode)(_in,
            ENABLE_INSERT_MODE | ENABLE_EXTENDED_FLAGS |
            ENABLE_PROCESSED_INPUT | ENABLE_QUICK_EDIT_MODE);
#endif
    }

	 void DetachConsole()
    {
        if(_out && _err && _in) {
			LI_FIND_CACHED( FreeConsole )( );
			

            if(_old_out)
				LI_FIND_CACHED(SetStdHandle)(STD_OUTPUT_HANDLE, _old_out);
            if(_old_err)
				LI_FIND_CACHED( SetStdHandle)(STD_ERROR_HANDLE, _old_err);
            if(_old_in)
				LI_FIND_CACHED( SetStdHandle)(STD_INPUT_HANDLE, _old_in);
        }
    }

    bool ConsolePrint(const char* fmt, ...)
    {
        if(!_out) 
            return false;

	#ifdef _DEBUG
		
        char buf[1024];
        va_list va;

        va_start(va, fmt);
        _vsnprintf_s(buf, 1024, fmt, va);
        va_end(va);

		
        return !!LI_FIND_CACHED( WriteConsoleA )( _out, buf, static_cast<DWORD>( strlen( buf ) ), nullptr, nullptr );
	#endif

		return false;
    }

    /*
     * @brief Blocks execution until a key is pressed on the console window
     *
     */
    char ConsoleReadKey()
    {
        if(!_in)
            return false;

#ifdef _DEBUG
        auto key = char{ 0 };
        auto keysread = DWORD{ 0 };

		LI_FIND_CACHED( ReadConsoleA)(_in, &key, 1, &keysread, nullptr);

        return key;
#endif

		return ' ';
    }


    int WaitForModules(std::int32_t timeout, const std::initializer_list<std::wstring>& modules)
    {
        bool signaled[32] = { 0 };
        bool success = false;

        std::uint32_t totalSlept = 0;

        if(timeout == 0) {
            for(auto& mod : modules) {
                if(LI_FIND_CACHED(GetModuleHandleW)(std::data(mod)) == NULL)
                    return WAIT_TIMEOUT;
            }
            return WAIT_OBJECT_0;
        }

        if(timeout < 0)
            timeout = INT32_MAX;

        while(true) {
            for(auto i = 0u; i < modules.size(); ++i) {
                auto& module = *(modules.begin() + i);
                if(!signaled[i] && LI_FIND_CACHED( GetModuleHandleW )(std::data(module)) != NULL) {
                    signaled[i] = true;

                    //
                    // Checks if all modules are signaled
                    //
                    bool done = true;
                    for(auto j = 0u; j < modules.size(); ++j) {
                        if(!signaled[j]) {
                            done = false;
                            break;
                        }
                    }
                    if(done) {
                        success = true;
                        goto exit;
                    }
                }
            }
            if(totalSlept > std::uint32_t(timeout)) {
                break;
            }
			LI_FIND_CACHED(Sleep)(10);
            totalSlept += 10;
        }

    exit:
        return success ? WAIT_OBJECT_0 : WAIT_TIMEOUT;
    }

    /*
     * @brief Scan for a given byte pattern on a module
     *
     * @param module    Base of the module to search
     * @param signature IDA-style byte array pattern
     *
     * @returns Address of the first occurence
     */
    std::uint8_t* PatternScan(void* module, const char* signature)
    {
        static auto pattern_to_byte = [](const char* pattern) {
            auto bytes = std::vector<int>{};
            auto start = const_cast<char*>(pattern);
            auto end = const_cast<char*>(pattern) + strlen(pattern);

            for(auto current = start; current < end; ++current) {
                if(*current == '?') {
                    ++current;
                    if(*current == '?')
                        ++current;
                    bytes.push_back(-1);
                } else {
                    bytes.push_back(strtoul(current, &current, 16));
                }
            }
            return bytes;
        };

        auto dosHeader = (PIMAGE_DOS_HEADER)module;
        auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)module + dosHeader->e_lfanew);

        auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
        auto patternBytes = pattern_to_byte(signature);
        auto scanBytes = reinterpret_cast<std::uint8_t*>(module);

        auto s = patternBytes.size();
        auto d = patternBytes.data();

        for(auto i = 0ul; i < sizeOfImage - s; ++i) {
            bool found = true;
            for(auto j = 0ul; j < s; ++j) {
                if(scanBytes[i + j] != d[j] && d[j] != -1) {
                    found = false;
                    break;
                }
            }
            if(found)
			{
                return &scanBytes[i];
            }
        }
        return nullptr;
    }

    /*
     * @brief Set player clantag
     *
     * @param tag New clantag
     */
    void SetClantag(const char* tag)
    {
        static auto fnClantagChanged = (int(__fastcall*)(const char*, const char*))PatternScan( LI_FIND_CACHED( GetModuleHandleW )(L"engine.dll"), "53 56 57 8B DA 8B F9 FF 15");

        fnClantagChanged(tag, tag);
    }

    /*
     * @brief Set player name
     *
     * @param name New name
     */
    void SetName(const char* name)
    {
        static auto nameConvar = g_CVar->FindVar("name");
        nameConvar->m_fnChangeCallbacks.m_Size = 0;

        // Fix so we can change names how many times we want
        static auto do_once = (nameConvar->SetValue("\nญญญ"), true);

        nameConvar->SetValue(name);
    }

	void Normalize(QAngle& angle)
	{
		while (angle.pitch > 89.0f) {
			angle.pitch -= 180.f;
		}
		while (angle.pitch < -89.0f) {
			angle.pitch += 180.f;
		}
		while (angle.yaw > 180.f) {
			angle.yaw -= 360.f;
		}
		while (angle.yaw < -180.f) {
			angle.yaw += 360.f;
		}
	}

	float DistanceToRay(const Vector& pos, const Vector& rayStart, const Vector& rayEnd, float* along, Vector* pointOnRay)
	{
		Vector to = pos - rayStart;
		Vector dir = rayEnd - rayStart;
		float length = dir.Length();
		float rangeAlong = dir.Dot(to);
		if (along)
		{
			*along = rangeAlong;
		}

		float range;

		if (rangeAlong < 0.0f) {
			range = -(pos - rayStart).Length();
			if (pointOnRay) {
				*pointOnRay = rayStart;
			}
		}
		else if (rangeAlong > length) {
			range = -(pos - rayEnd).Length();
			if (pointOnRay) {
				*pointOnRay = rayEnd;
			}
		}
		else {
			Vector onRay = rayStart + (dir.operator*(rangeAlong));
			range = (pos - onRay).Length();
			if (pointOnRay) {
				*pointOnRay = onRay;
			}
		}
		return range;
	}

	void TraceLine(Vector& vecAbsStart, Vector& vecAbsEnd, unsigned int mask, const IHandleEntity* ignore, int collisionGroup, trace_t* ptr)
	{
		Ray_t ray;
		ray.Init(vecAbsStart, vecAbsEnd);
		CTraceFilter traceFilter;
		traceFilter.pSkip = (void*)ignore;
		g_EngineTrace->TraceRay(ray, mask, &traceFilter, ptr);
	}

	void ClipTraceToPlayers(Vector& vecAbsStart, Vector& vecAbsEnd, unsigned int mask, ITraceFilter* filter, trace_t* tr)
	{
		trace_t playerTrace;
		Ray_t ray;
		float smallestFraction = tr->fraction;
		const static float maxRange = 60.0f;
		ray.Init(vecAbsStart, vecAbsEnd);
		for (int k = 1; k <= g_GlobalVars->maxClients; ++k)
		{
			C_BasePlayer* player = (C_BasePlayer*)g_EntityList->GetClientEntity(k);

			if (!player || !player->IsAlive())
				continue;

			if (player->IsDormant())
				continue;

			if (filter && filter->ShouldHitEntity(player, mask) == false)
				continue;

			float range = DistanceToRay(player->WorldSpaceCenter(), vecAbsStart, vecAbsEnd);
			if (range < 0.0f || range > maxRange)
				continue;
			g_EngineTrace->ClipRayToEntity(ray, mask | CONTENTS_HITBOX, player, &playerTrace);
			if (playerTrace.fraction < smallestFraction)
			{
				*tr = playerTrace;
				smallestFraction = playerTrace.fraction;
			}
		}
	}

	void Tracehull(Vector& src, Vector& end, const Vector& mins, const Vector& maxs, unsigned int mask, ITraceFilter* filter, trace_t& tr)
	{
		Ray_t ray;
		ray.Init(src, end, mins, maxs);
		enginetrace->TraceRay(ray, mask, filter, &tr);
	}

    bool TraceToExit(Vector& end, trace_t& tr, Vector start, Vector vEnd, trace_t* trace)
	{
		typedef bool(__fastcall* TraceToExitFn)(Vector&, trace_t&, float, float, float, float, float, float, trace_t*);
		static auto TraceToExit = PatternScan( LI_FIND_CACHED( GetModuleHandleW )(L"client.dll"), "55 8B EC 83 EC 30 F3 0F 10 75");

		if (!TraceToExit)
			return false;

		float start_y = start.y, start_z = start.z, start_x = start.x, dir_y = vEnd.y, dir_x = vEnd.x, dir_z = vEnd.z;
		_asm
		{
			nop
			nop
			push trace
			push dir_z
			push dir_y
			push dir_x
			nop
			push start_z
			push start_y
			nop
			push start_x
			mov edx, tr
			mov ecx, end
			call TraceToExit
			nop
			add esp, 0x1C
		}

	}

	void NormalizeYaw(float& yaw)
	{
		while (yaw > 180.f) {
			yaw -= 360.f;
		}
		while (yaw < -180.f) {
			yaw += 360.f;
		}
	}

	void ClampYaw(float& yaw)
	{
		while (yaw > 180.f) {
			yaw -= 360.f;
		}
		while (yaw < -180.f) {
			yaw += 360.f;
		}

		if (yaw > 180.0f)
			yaw = 180.0f;
		else if (yaw < -180.0f)
			yaw = -180.0f;

	}

	void ClampAngles(QAngle &angles)
	{
		if (angles.yaw > 180.0f)
			angles.yaw = 180.0f;
		else if (angles.yaw < -180.0f)
			angles.yaw = -180.0f;

		if (angles.pitch > 89.0f)
			angles.pitch = 89.0f;
		else if (angles.pitch < -89.0f)
			angles.pitch = -89.0f;

		angles.roll = 0;
	}

	bool Clamp(QAngle &angles)
	{
		QAngle a = angles;
		Normalize(a);
		ClampAngles(a);

		if (isnan(a.pitch) || isinf(a.pitch) ||
			isnan(a.yaw) || isinf(a.yaw) ||
			isnan(a.roll) || isinf(a.roll)) {
			return false;
		}
		else {
			angles = a;
			return true;
		}
	}

	//TODO: make a better one, current one straight aimtux paste
	void CorrectMovement(QAngle vOldAngles)
	{
		float deltaView, f1,f2;


		if (vOldAngles.yaw < 0.f)
			f1 = 360.0f + vOldAngles.yaw;
		else
			f1 = vOldAngles.yaw;

		if (g_Cmd->viewangles.yaw < 0.0f)
			f2 = 360.0f + g_Cmd->viewangles.yaw;
		else
			f2 = g_Cmd->viewangles.yaw;

		if (f2 < f1)
			deltaView = abs(f2 - f1);
		else
			deltaView = 360.0f - abs(f1 - f2);

		deltaView = 360.0f - deltaView;
		
		float formv = g_Cmd->forwardmove, sidemv = g_Cmd->sidemove;

		g_Cmd->forwardmove = cos(Math::deg2rad(deltaView)) * formv + cos(Math::deg2rad(deltaView + 90.f)) * sidemv;
		g_Cmd->sidemove = sin(Math::deg2rad(deltaView)) * formv + sin(Math::deg2rad(deltaView + 90.f)) * sidemv;
	}

	ULONG PatternSearch(std::string sModuleName, PBYTE pbPattern, std::string sMask, ULONG uCodeBase, ULONG uSizeOfCode)
	{
		BOOL bPatternDidMatch = FALSE;
		HMODULE hModule = LI_FIND_CACHED( GetModuleHandleA )(sModuleName.c_str());

		if (!hModule)
			return 0x0;

		PIMAGE_DOS_HEADER pDsHeader = PIMAGE_DOS_HEADER(hModule);
		PIMAGE_NT_HEADERS pPeHeader = PIMAGE_NT_HEADERS(LONG(hModule) + pDsHeader->e_lfanew);
		PIMAGE_OPTIONAL_HEADER pOptionalHeader = &pPeHeader->OptionalHeader;

		if (uCodeBase == 0x0)
			uCodeBase = (ULONG)hModule + pOptionalHeader->BaseOfCode;

		if (uSizeOfCode == 0x0)
			uSizeOfCode = pOptionalHeader->SizeOfCode;

		ULONG uArraySize = sMask.length();

		if (!uCodeBase || !uSizeOfCode || !uArraySize)
			return 0x0;

		for (size_t i = uCodeBase; i <= uCodeBase + uSizeOfCode; i++)
		{
			for (size_t t = 0; t < uArraySize; t++)
			{
				if (*((PBYTE)i + t) == pbPattern[t] || sMask.c_str()[t] == '?')
					bPatternDidMatch = TRUE;

				else
				{
					bPatternDidMatch = FALSE;
					break;
				}
			}

			if (bPatternDidMatch)
				return i;
		}

		return 0x0;
	}

	float FakeChamsFix(CBasePlayerAnimState* animstate)
	{
		if (!animstate)
			return 0.f;

		float flYawDelta = animstate->m_flGoalFeetYaw - animstate->m_flEyeYaw;
		Utils::ClampYaw(flYawDelta);

		float flYawDelta2 = g_Local->lby( ) - Hooks::realyaw;
		Utils::ClampYaw(flYawDelta2);

		if (fabs(flYawDelta2) > 35.0f && fabs(flYawDelta) > 35.0f)
			return ((flYawDelta > 0.0f) ? true : false) ? -options.fakechams_fix_val : options.fakechams_fix_val;

		return 0.f;
	}


    void RankRevealAll()
    {
        using ServerRankRevealAll = char(__cdecl*)(int*);

        static auto fnServerRankRevealAll = PatternScan( LI_FIND_CACHED( GetModuleHandleW )(L"client.dll"), "55 8B EC 8B 0D ? ? ? ? 68");

        int v[3] = { 0,0,0 };

        reinterpret_cast<ServerRankRevealAll>(fnServerRankRevealAll)(v);
    }
}