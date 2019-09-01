#pragma once

#define NOMINMAX

#include <string>
#include <initializer_list>
#include <math.h> 
#include "../globals.h"
#include "..\options.hpp"
#include "..\singleton.hpp"
#include "..\valve_sdk\sdk.hpp"
#include "..\valve_sdk\csgostructs.hpp"
#include "..\helpers\math.hpp"
#include "..\helpers\utils.hpp"
namespace Utils
{
    void attach_console();

    void DetachConsole();

    bool ConsolePrint(const char* fmt, ...);
    
    char ConsoleReadKey();

    int WaitForModules(std::int32_t timeout, const std::initializer_list<std::wstring>& modules);

    std::uint8_t* PatternScan(void* module, const char* signature);

    void SetClantag(const char* tag);

    void SetName(const char* name);

	float FakeChamsFix(CBasePlayerAnimState* animstate);

	void Normalize(QAngle& angle);

	float DistanceToRay(const Vector& pos, const Vector& rayStart, const Vector& rayEnd, float* along = nullptr, Vector* pointOnRay = nullptr);

	void TraceLine(Vector& vecAbsStart, Vector& vecAbsEnd, unsigned int mask, const IHandleEntity* ignore, int collisionGroup, trace_t* ptr);

	void ClipTraceToPlayers(Vector& vecAbsStart, Vector& vecAbsEnd, unsigned int mask, ITraceFilter* filter, trace_t* tr);

	void Tracehull(Vector& src, Vector& end, const Vector& mins, const Vector& maxs, unsigned int mask, ITraceFilter* filter, trace_t& tr);

	bool TraceToExit(Vector& end, trace_t& tr, Vector start, Vector vEnd, trace_t* trace);

	void NormalizeYaw(float& yaw);

	void ClampYaw(float& yaw);

	void ClampAngles(QAngle &angles);

	bool Clamp(QAngle &angles);

	void CorrectMovement(QAngle vOldAngles);

	ULONG PatternSearch(std::string sModuleName, PBYTE pbPattern, std::string sMask, ULONG uCodeBase, ULONG uSizeOfCode);

    void RankRevealAll();

}
