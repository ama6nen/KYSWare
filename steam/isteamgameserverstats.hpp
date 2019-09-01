#ifndef ISTEAMGAMESERVERSTATS_H
#define ISTEAMGAMESERVERSTATS_H
#ifdef _WIN32
#pragma once
#endif
#include "isteamclient.hpp"
class ISteamGameServerStats
{
public:
	CALL_RESULT( GSStatsReceived_t )
	virtual SteamAPICall_t RequestUserStats( CSteamID steamIDUser ) = 0;
	virtual bool GetUserStat( CSteamID steamIDUser, const char *pchName, int32 *pData ) = 0;
	virtual bool GetUserStat( CSteamID steamIDUser, const char *pchName, float *pData ) = 0;
	virtual bool GetUserAchievement( CSteamID steamIDUser, const char *pchName, bool *pbAchieved ) = 0;
	virtual bool SetUserStat( CSteamID steamIDUser, const char *pchName, int32 nData ) = 0;
	virtual bool SetUserStat( CSteamID steamIDUser, const char *pchName, float fData ) = 0;
	virtual bool UpdateUserAvgRateStat( CSteamID steamIDUser, const char *pchName, float flCountThisSession, double dSessionLength ) = 0;
	virtual bool SetUserAchievement( CSteamID steamIDUser, const char *pchName ) = 0;
	virtual bool ClearUserAchievement( CSteamID steamIDUser, const char *pchName ) = 0;
	CALL_RESULT( GSStatsStored_t )
	virtual SteamAPICall_t StoreUserStats( CSteamID steamIDUser ) = 0;
};
#define STEAMGAMESERVERSTATS_INTERFACE_VERSION "SteamGameServerStats001"
#if defined( VALVE_CALLBACK_PACK_SMALL )
#pragma pack( push, 4 )
#elif defined( VALVE_CALLBACK_PACK_LARGE )
#pragma pack( push, 8 )
#else
#error isteamclient.hpp must be included
#endif 
struct GSStatsReceived_t
{
	enum { k_iCallback = k_iSteamGameServerStatsCallbacks };
	EResult		m_eResult;		      
	CSteamID	m_IDUser;	         
};
struct GSStatsStored_t
{
	enum { k_iCallback = k_iSteamGameServerStatsCallbacks + 1 };
	EResult		m_eResult;		   
	CSteamID	m_IDUser;	        
};
struct GSStatsUnloaded_t
{
	enum { k_iCallback = k_iSteamUserStatsCallbacks + 8 };
	CSteamID	m_IDUser;	      
};
#pragma pack( pop )
#endif  
