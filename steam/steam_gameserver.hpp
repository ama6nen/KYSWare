#ifndef STEAM_GAMESERVER_H
#define STEAM_GAMESERVER_H
#ifdef _WIN32
#pragma once
#endif
#include "steam_api.hpp"
#include "isteamgameserver.hpp"
#include "isteamgameserverstats.hpp"
enum EServerMode
{
	eServerModeInvalid = 0, // DO NOT USE		
	eServerModeNoAuthentication = 1, // Don't authenticate user logins and don't list on the server list
	eServerModeAuthentication = 2, // Authenticate users, list on the server list, don't run VAC on clients that connect
	eServerModeAuthenticationAndSecure = 3, // Authenticate users, list on the server list and VAC protect clients
};													
inline bool SteamGameServer_Init( uint32 unIP, uint16 usSteamPort, uint16 usGamePort, uint16 usQueryPort, EServerMode eServerMode, const char *pchVersionString );
S_API void SteamGameServer_Shutdown();
S_API void SteamGameServer_RunCallbacks();
inline void SteamGameServer_ReleaseCurrentThreadMemory();
S_API bool SteamGameServer_BSecure();
S_API uint64 SteamGameServer_GetSteamID();
#if !defined( STEAM_API_EXPORTS )
inline ISteamClient *SteamGameServerClient();
inline ISteamGameServer *SteamGameServer();
inline ISteamUtils *SteamGameServerUtils();
inline ISteamNetworking *SteamGameServerNetworking();
inline ISteamGameServerStats *SteamGameServerStats();
inline ISteamHTTP *SteamGameServerHTTP();
inline ISteamInventory *SteamGameServerInventory();
inline ISteamUGC *SteamGameServerUGC();
inline ISteamApps *SteamGameServerApps();
#endif
class CSteamGameServerAPIContext
{
public:
	CSteamGameServerAPIContext() { clear(); }
	inline void clear();
	inline bool init();
	ISteamClient *client() const					{ return m_client; }
	ISteamGameServer *SteamGameServer() const			{ return m_pSteamGameServer; }
	ISteamUtils *SteamGameServerUtils() const			{ return m_pSteamGameServerUtils; }
	ISteamNetworking *SteamGameServerNetworking() const	{ return m_pSteamGameServerNetworking; }
	ISteamGameServerStats *SteamGameServerStats() const	{ return m_pSteamGameServerStats; }
	ISteamHTTP *http() const						{ return m_http; }
	ISteamInventory *inventory() const				{ return m_inventory; }
	ISteamUGC *ugc() const							{ return m_ugc; }
	ISteamApps *apps() const						{ return m_apps; }
private:
	ISteamClient				*m_client;
	ISteamGameServer			*m_pSteamGameServer;
	ISteamUtils					*m_pSteamGameServerUtils;
	ISteamNetworking			*m_pSteamGameServerNetworking;
	ISteamGameServerStats		*m_pSteamGameServerStats;
	ISteamHTTP					*m_http;
	ISteamInventory				*m_inventory;
	ISteamUGC					*m_ugc;
	ISteamApps					*m_apps;
};
#define STEAM_GAMESERVER_CALLBACK( thisclass, func, /*callback_type, [deprecated] var*/... ) \
	_STEAM_CALLBACK_SELECT( ( __VA_ARGS__, GS, 3 ), ( this->SetGameserverFlag();, thisclass, func, __VA_ARGS__ ) )
#define STEAM_GAMESERVER_CALLBACK_MANUAL( thisclass, func, callback_type, var ) \
	CCallbackManual< thisclass, callback_type, true > var; void func( callback_type *pParam )
#define _STEAM_CALLBACK_GS( _, thisclass, func, param, var ) \
	CCallback< thisclass, param, true > var; void func( param *pParam )
S_API HSteamPipe S_CALLTYPE SteamGameServer_GetHSteamPipe();
S_API HSteamUser S_CALLTYPE SteamGameServer_GetHSteamUser();
S_API bool S_CALLTYPE SteamInternal_GameServer_Init( uint32 unIP, uint16 usPort, uint16 usGamePort, uint16 usQueryPort, EServerMode eServerMode, const char *pchVersionString );
#if !defined( STEAM_API_EXPORTS )
inline void S_CALLTYPE SteamGameServerInternal_OnContextInit( void* p )
{
	((CSteamGameServerAPIContext*)p)->clear();
	if ( SteamGameServer_GetHSteamPipe() )
		((CSteamGameServerAPIContext*)p)->init();
}
inline CSteamGameServerAPIContext& SteamGameServerInternal_ModuleContext()
{
	static void* s_CallbackCounterAndContext[2 + sizeof( CSteamGameServerAPIContext ) / sizeof( void* )] = { (void*)&SteamGameServerInternal_OnContextInit, 0 };
	return *(CSteamGameServerAPIContext*)SteamInternal_ContextInit( s_CallbackCounterAndContext );
}
inline ISteamClient *SteamGameServerClient() { return SteamGameServerInternal_ModuleContext().client(); }
inline ISteamGameServer *SteamGameServer() { return SteamGameServerInternal_ModuleContext().SteamGameServer(); }
inline ISteamUtils *SteamGameServerUtils() { return SteamGameServerInternal_ModuleContext().SteamGameServerUtils(); }
inline ISteamNetworking *SteamGameServerNetworking() { return SteamGameServerInternal_ModuleContext().SteamGameServerNetworking(); }
inline ISteamGameServerStats *SteamGameServerStats() { return SteamGameServerInternal_ModuleContext().SteamGameServerStats(); }
inline ISteamHTTP *SteamGameServerHTTP() { return SteamGameServerInternal_ModuleContext().http(); }
inline ISteamInventory *SteamGameServerInventory() { return SteamGameServerInternal_ModuleContext().inventory(); }
inline ISteamUGC *SteamGameServerUGC() { return SteamGameServerInternal_ModuleContext().ugc(); }
inline ISteamApps *SteamGameServerApps() { return SteamGameServerInternal_ModuleContext().apps(); }
#endif // !defined( STEAM_API_EXPORTS )
inline void CSteamGameServerAPIContext::clear()
{
	m_client = NULL;
	m_pSteamGameServer = NULL;
	m_pSteamGameServerUtils = NULL;
	m_pSteamGameServerNetworking = NULL;
	m_pSteamGameServerStats = NULL;
	m_http = NULL;
	m_inventory = NULL;
	m_ugc = NULL;
	m_apps = NULL;
}
inline bool CSteamGameServerAPIContext::init()
{
	HSteamUser hSteamUser = SteamGameServer_GetHSteamUser();
	HSteamPipe hSteamPipe = SteamGameServer_GetHSteamPipe();
	if ( !hSteamPipe )
		return false;
	m_client = (ISteamClient*) SteamInternal_CreateInterface( STEAMCLIENT_INTERFACE_VERSION );
	if ( !m_client )
		return false;
	m_pSteamGameServer = m_client->GetISteamGameServer( hSteamUser, hSteamPipe, STEAMGAMESERVER_INTERFACE_VERSION );
	if ( !m_pSteamGameServer )
		return false;
	m_pSteamGameServerUtils = m_client->GetISteamUtils( hSteamPipe, STEAMUTILS_INTERFACE_VERSION );
	if ( !m_pSteamGameServerUtils )
		return false;
	m_pSteamGameServerNetworking = m_client->GetISteamNetworking( hSteamUser, hSteamPipe, STEAMNETWORKING_INTERFACE_VERSION );
	if ( !m_pSteamGameServerNetworking )
		return false;
	m_pSteamGameServerStats = m_client->GetISteamGameServerStats( hSteamUser, hSteamPipe, STEAMGAMESERVERSTATS_INTERFACE_VERSION );
	if ( !m_pSteamGameServerStats )
		return false;
	m_http = m_client->GetISteamHTTP( hSteamUser, hSteamPipe, STEAMHTTP_INTERFACE_VERSION );
	if ( !m_http )
		return false;
	m_inventory = m_client->GetISteamInventory( hSteamUser, hSteamPipe, STEAMINVENTORY_INTERFACE_VERSION );
	if ( !m_inventory )
		return false;
	m_ugc = m_client->GetISteamUGC( hSteamUser, hSteamPipe, STEAMUGC_INTERFACE_VERSION );
	if ( !m_ugc )
		return false;
	m_apps = m_client->GetISteamApps( hSteamUser, hSteamPipe, STEAMAPPS_INTERFACE_VERSION );
	if ( !m_apps )
		return false;
	return true;
}
inline bool SteamGameServer_Init( uint32 unIP, uint16 usSteamPort, uint16 usGamePort, uint16 usQueryPort, EServerMode eServerMode, const char *pchVersionString )
{
	if ( !SteamInternal_GameServer_Init( unIP, usSteamPort, usGamePort, usQueryPort, eServerMode, pchVersionString ) )
		return false;
	return true;
}
inline void SteamGameServer_ReleaseCurrentThreadMemory()
{
	SteamAPI_ReleaseCurrentThreadMemory();
}
#endif // STEAM_GAMESERVER_H
