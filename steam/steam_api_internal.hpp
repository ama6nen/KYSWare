#pragma once

inline void steam_api_context::clear()
{
	m_user = NULL;
	m_friends = NULL;
	m_utils = NULL;
	m_matchmaking = NULL;
	m_userstats = NULL;
	m_apps = NULL;
	m_matchmakingservers = NULL;
	m_networking = NULL;
	m_remotestorage = NULL;
	m_http = NULL;
	m_screenshots = NULL;
	m_ugc = NULL;
	m_applist = NULL;
	m_htmlsurface = NULL;
	m_inventory = NULL;
	m_video = NULL;
	m_parentalsettings = NULL;
}

inline bool steam_api_context::init() {
	unsigned steam_user = ((unsigned(__cdecl*)(void))GetProcAddress( LI_FIND_CACHED( GetModuleHandleW )(L"steam_api.dll"), "SteamAPI_GetHSteamUser"))();
	unsigned steam_pipe = ((unsigned(__cdecl*)(void))GetProcAddress( LI_FIND_CACHED( GetModuleHandleW )(L"steam_api.dll"), "SteamAPI_GetHSteamPipe"))();
	auto steam_client = ((ISteamClient*(__cdecl*)(void))GetProcAddress( LI_FIND_CACHED( GetModuleHandleW )(L"steam_api.dll"), "SteamClient"))();

	if (!steam_pipe) return false;
	if (!steam_client) return false;
	m_user = steam_client->GetISteamUser(steam_user, steam_pipe, STEAMUSER_INTERFACE_VERSION);	if (!m_user) return false;
	m_friends = steam_client->GetISteamFriends(steam_user, steam_pipe, STEAMFRIENDS_INTERFACE_VERSION);	if (!m_friends) return false;
	m_utils = steam_client->GetISteamUtils(steam_pipe, STEAMUTILS_INTERFACE_VERSION);	if (!m_utils) return false;
	m_matchmaking = steam_client->GetISteamMatchmaking(steam_user, steam_pipe, STEAMMATCHMAKING_INTERFACE_VERSION);	if (!m_matchmaking) return false;
	m_matchmakingservers = steam_client->GetISteamMatchmakingServers(steam_user, steam_pipe, STEAMMATCHMAKINGSERVERS_INTERFACE_VERSION);	if (!m_matchmakingservers) return false;
	m_userstats = steam_client->GetISteamUserStats(steam_user, steam_pipe, STEAMUSERSTATS_INTERFACE_VERSION);	if (!m_userstats) return false;
	m_apps = steam_client->GetISteamApps(steam_user, steam_pipe, STEAMAPPS_INTERFACE_VERSION);	if (!m_apps) return false;
	m_networking = steam_client->GetISteamNetworking(steam_user, steam_pipe, STEAMNETWORKING_INTERFACE_VERSION);	if (!m_networking) return false;
	m_remotestorage = steam_client->GetISteamRemoteStorage(steam_user, steam_pipe, STEAMREMOTESTORAGE_INTERFACE_VERSION);	if (!m_remotestorage) return false;
	m_screenshots = steam_client->GetISteamScreenshots(steam_user, steam_pipe, STEAMSCREENSHOTS_INTERFACE_VERSION);	if (!m_screenshots) return false;
	m_http = steam_client->GetISteamHTTP(steam_user, steam_pipe, STEAMHTTP_INTERFACE_VERSION);	if (!m_http) return false;
	m_ugc = steam_client->GetISteamUGC(steam_user, steam_pipe, STEAMUGC_INTERFACE_VERSION);	if (!m_ugc) return false;
	m_applist = steam_client->GetISteamAppList(steam_user, steam_pipe, STEAMAPPLIST_INTERFACE_VERSION);	if (!m_applist) return false;
	m_htmlsurface = steam_client->GetISteamHTMLSurface(steam_user, steam_pipe, STEAMHTMLSURFACE_INTERFACE_VERSION);	if (!m_htmlsurface) return false;
	m_inventory = steam_client->GetISteamInventory(steam_user, steam_pipe, STEAMINVENTORY_INTERFACE_VERSION);	if (!m_inventory) return false;
	m_video = steam_client->GetISteamVideo(steam_user, steam_pipe, STEAMVIDEO_INTERFACE_VERSION);	if (!m_video) return false;
	m_parentalsettings = steam_client->GetISteamParentalSettings(steam_user, steam_pipe, STEAMPARENTALSETTINGS_INTERFACE_VERSION);	if (!m_parentalsettings) return false;
	
	return true;
}