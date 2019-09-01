#ifndef STEAMUNIVERSE_H
#define STEAMUNIVERSE_H
#ifdef _WIN32
#pragma once
#endif
enum EUniverse
{
	k_EUniverseInvalid = 0,
	k_EUniversePublic = 1,
	k_EUniverseBeta = 2,
	k_EUniverseInternal = 3,
	k_EUniverseDev = 4,
	k_EUniverseMax
};
#endif // STEAMUNIVERSE_H
