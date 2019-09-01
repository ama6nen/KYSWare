#ifndef ISTEAMAPPLIST_H
#define ISTEAMAPPLIST_H
#ifdef _WIN32
#pragma once
#endif
#include "isteamclient.hpp"
#include "steamtypes.hpp"
class ISteamAppList
{
public:
	virtual uint32 GetNumInstalledApps() = 0;
	virtual uint32 GetInstalledApps( AppId_t *pvecAppID, uint32 unMaxAppIDs ) = 0;
	virtual int  GetAppName( AppId_t nAppID, OUT_STRING() char *pchName, int cchNameMax ) = 0; // returns -1 if no name was found
	virtual int  GetAppInstallDir( AppId_t nAppID, char *pchDirectory, int cchNameMax ) = 0; // returns -1 if no dir was found
	virtual int GetAppBuildId( AppId_t nAppID ) = 0; // return the buildid of this app, may change at any time based on backend updates to the game
};
#define STEAMAPPLIST_INTERFACE_VERSION "STEAMAPPLIST_INTERFACE_VERSION001"
#if defined( VALVE_CALLBACK_PACK_SMALL )
#pragma pack( push, 4 )
#elif defined( VALVE_CALLBACK_PACK_LARGE )
#pragma pack( push, 8 )
#else
#error isteamclient.hpp must be included
#endif 
DEFINE_CALLBACK( SteamAppInstalled_t, k_iSteamAppListCallbacks + 1 );
	CALLBACK_MEMBER( 0,	AppId_t,	m_nAppID )			// ID of the app that installs
END_DEFINE_CALLBACK_1()
DEFINE_CALLBACK( SteamAppUninstalled_t, k_iSteamAppListCallbacks + 2 );
	CALLBACK_MEMBER( 0,	AppId_t,	m_nAppID )			// ID of the app that installs
END_DEFINE_CALLBACK_1()
#pragma pack( pop )
#endif // ISTEAMAPPLIST_H
