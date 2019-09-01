#ifndef STEAM_API_H
#define STEAM_API_H
#ifdef _WIN32
#pragma once
#endif
#include "isteamclient.hpp"
#include "isteamuser.hpp"
#include "isteamfriends.hpp"
#include "isteamutils.hpp"
#include "isteammatchmaking.hpp"
#include "isteamuserstats.hpp"
#include "isteamapps.hpp"
#include "isteamnetworking.hpp"
#include "isteamremotestorage.hpp"
#include "isteamscreenshots.hpp"
#include "isteamhttp.hpp"
#include "isteamugc.hpp"
#include "isteamapplist.hpp"
#include "isteamhtmlsurface.hpp"
#include "isteaminventory.hpp"
#include "isteamvideo.hpp"
#include "isteamparentalsettings.hpp"
#include "isteamgamecoordinator.hpp"
#define S_API extern "C" __declspec( dllimport ) 

class steam_api_context
{
public:
	steam_api_context() { clear(); }
	void clear();
	bool init();
	ISteamClient*		client() const						{ return m_client; }
	ISteamUser*			user() const						{ return m_user; }
	ISteamFriends*		friends() const						{ return m_friends; }
	ISteamUtils*		utils() const						{ return m_utils; }
	ISteamMatchmaking*	matchmaking() const					{ return m_matchmaking; }
	ISteamUserStats*	userstats() const					{ return m_userstats; }
	ISteamApps*			apps() const						{ return m_apps; }
	ISteamMatchmakingServers* matchmaking_servers() const	{ return m_matchmakingservers; }
	ISteamNetworking*	networking() const					{ return m_networking; }
	ISteamRemoteStorage* remote_storage() const				{ return m_remotestorage; }
	ISteamScreenshots*	screenshots() const					{ return m_screenshots; }
	ISteamHTTP*			http() const						{ return m_http; }
	ISteamUGC*			ugc() const							{ return m_ugc; }
	ISteamAppList*		applist() const						{ return m_applist; }
	ISteamHTMLSurface*	html_surface() const				{ return m_htmlsurface; }
	ISteamInventory*	inventory() const					{ return m_inventory; }
	ISteamVideo*		video() const						{ return m_video; }
	ISteamParentalSettings* parental_settings() const		{ return m_parentalsettings; }
private:
	ISteamClient		*m_client;
	ISteamUser			*m_user;
	ISteamFriends		*m_friends;
	ISteamUtils			*m_utils;
	ISteamMatchmaking	*m_matchmaking;
	ISteamUserStats		*m_userstats;
	ISteamApps			*m_apps;
	ISteamMatchmakingServers *m_matchmakingservers;
	ISteamNetworking	*m_networking;
	ISteamRemoteStorage *m_remotestorage;
	ISteamScreenshots	*m_screenshots;
	ISteamHTTP			*m_http;
	ISteamUGC			*m_ugc;
	ISteamAppList		*m_applist;
	ISteamHTMLSurface	*m_htmlsurface;
	ISteamInventory		*m_inventory;
	ISteamVideo			*m_video;
	ISteamParentalSettings *m_parentalsettings;
};

#include "steam_api_internal.hpp"
#endif // STEAM_API_H
