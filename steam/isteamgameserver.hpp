#ifndef ISTEAMGAMESERVER_H
#define ISTEAMGAMESERVER_H
#ifdef _WIN32
#pragma once
#endif
#include "isteamclient.hpp"
#define MASTERSERVERUPDATERPORT_USEGAMESOCKETSHARE	((uint16)-1)
class ISteamGameServer
{
public:
	/// This is called by SteamGameServer_Init, and you will usually not need to call it directly
	virtual bool InitGameServer( uint32 unIP, uint16 usGamePort, uint16 usQueryPort, uint32 unFlags, AppId_t nGameAppId, const char *pchVersionString ) = 0;
	/// Game product identifier.  This is currently used by the master server for version checking purposes.
	/// It's a required field, but will eventually will go away, and the AppID will be used for this purpose.
	virtual void SetProduct( const char *pszProduct ) = 0;
	/// Description of the game.  This is a required field and is displayed in the steam server browser....for now.
	/// This is a required field, but it will go away eventually, as the data should be determined from the AppID.
	virtual void SetGameDescription( const char *pszGameDescription ) = 0;
	/// If your game is a "mod," pass the string that identifies it.  The default is an empty string, meaning
	/// this application is the original game, not a mod.
	///
	/// @see k_cbMaxGameServerGameDir
	virtual void SetModDir( const char *pszModDir ) = 0;
	/// Is this is a dedicated server?  The default value is false.
	virtual void SetDedicatedServer( bool bDedicated ) = 0;
	/// Begin process to login to a persistent game server account
	///
	/// You need to register for callbacks to determine the result of this operation.
	/// @see SteamServersConnected_t
	/// @see SteamServerConnectFailure_t
	/// @see SteamServersDisconnected_t
	virtual void LogOn( const char *pszToken ) = 0;
	/// Login to a generic, anonymous account.
	///
	/// Note: in previous versions of the SDK, this was automatically called within SteamGameServer_Init,
	/// but this is no longer the case.
	virtual void LogOnAnonymous() = 0;
	/// Begin process of logging game server out of steam
	virtual void LogOff() = 0;
	virtual bool BLoggedOn() = 0;
	virtual bool BSecure() = 0; 
	virtual CSteamID GetSteamID() = 0;
	/// Returns true if the master server has requested a restart.
	/// Only returns true once per request.
	virtual bool WasRestartRequested() = 0;
	/// Max player count that will be reported to server browser and client queries
	virtual void SetMaxPlayerCount( int cPlayersMax ) = 0;
	/// Number of bots.  Default value is zero
	virtual void SetBotPlayerCount( int cBotplayers ) = 0;
	/// Set the name of server as it will appear in the server browser
	///
	/// @see k_cbMaxGameServerName
	virtual void SetServerName( const char *pszServerName ) = 0;
	/// Set name of map to report in the server browser
	///
	/// @see k_cbMaxGameServerName
	virtual void SetMapName( const char *pszMapName ) = 0;
	/// Let people know if your server will require a password
	virtual void SetPasswordProtected( bool bPasswordProtected ) = 0;
	/// Spectator server.  The default value is zero, meaning the service
	/// is not used.
	virtual void SetSpectatorPort( uint16 unSpectatorPort ) = 0;
	/// Name of the spectator server.  (Only used if spectator port is nonzero.)
	///
	/// @see k_cbMaxGameServerMapName
	virtual void SetSpectatorServerName( const char *pszSpectatorServerName ) = 0;
	/// Call this to clear the whole list of key/values that are sent in rules queries.
	virtual void ClearAllKeyValues() = 0;
	/// Call this to add/update a key/value pair.
	virtual void SetKeyValue( const char *pKey, const char *pValue ) = 0;
	/// Sets a string defining the "gametags" for this server, this is optional, but if it is set
	/// it allows users to filter in the matchmaking/server-browser interfaces based on the value
	///
	/// @see k_cbMaxGameServerTags
	virtual void SetGameTags( const char *pchGameTags ) = 0;
	/// Sets a string defining the "gamedata" for this server, this is optional, but if it is set
	/// it allows users to filter in the matchmaking/server-browser interfaces based on the value
	/// don't set this unless it actually changes, its only uploaded to the master once (when
	/// acknowledged)
	///
	/// @see k_cbMaxGameServerGameData
	virtual void SetGameData( const char *pchGameData ) = 0;
	/// Region identifier.  This is an optional field, the default value is empty, meaning the "world" region
	virtual void SetRegion( const char *pszRegion ) = 0;
	virtual bool SendUserConnectAndAuthenticate( uint32 unIPClient, const void *pvAuthBlob, uint32 cubAuthBlobSize, CSteamID *pSteamIDUser ) = 0;
	virtual CSteamID CreateUnauthenticatedUserConnection() = 0;
	virtual void SendUserDisconnect( CSteamID steamIDUser ) = 0;
	virtual bool BUpdateUserData( CSteamID steamIDUser, const char *pchPlayerName, uint32 uScore ) = 0;
	virtual HAuthTicket GetAuthSessionTicket( void *pTicket, int cbMaxTicket, uint32 *pcbTicket ) = 0;
	virtual EBeginAuthSessionResult BeginAuthSession( const void *pAuthTicket, int cbAuthTicket, CSteamID steamID ) = 0;
	virtual void EndAuthSession( CSteamID steamID ) = 0;
	virtual void CancelAuthTicket( HAuthTicket hAuthTicket ) = 0;
	virtual EUserHasLicenseForAppResult UserHasLicenseForApp( CSteamID steamID, AppId_t appID ) = 0;
	virtual bool RequestUserGroupStatus( CSteamID steamIDUser, CSteamID steamIDGroup ) = 0;
	virtual void GetGameplayStats( ) = 0;
	CALL_RESULT( GSReputation_t )
	virtual SteamAPICall_t GetServerReputation() = 0;
	virtual uint32 GetPublicIP() = 0;
	virtual bool HandleIncomingPacket( const void *pData, int cbData, uint32 srcIP, uint16 srcPort ) = 0;
	virtual int GetNextOutgoingPacket( void *pOut, int cbMaxOut, uint32 *pNetAdr, uint16 *pPort ) = 0;
	virtual void EnableHeartbeats( bool bActive ) = 0;
	virtual void SetHeartbeatInterval( int iHeartbeatInterval ) = 0;
	virtual void ForceHeartbeat() = 0;
	CALL_RESULT( AssociateWithClanResult_t )
	virtual SteamAPICall_t AssociateWithClan( CSteamID steamIDClan ) = 0;
	CALL_RESULT( ComputeNewPlayerCompatibilityResult_t )
	virtual SteamAPICall_t ComputeNewPlayerCompatibility( CSteamID steamIDNewPlayer ) = 0;
};
#define STEAMGAMESERVER_INTERFACE_VERSION "SteamGameServer012"
const uint32 k_unServerFlagNone			= 0x00;
const uint32 k_unServerFlagActive		= 0x01;		// server has users playing
const uint32 k_unServerFlagSecure		= 0x02;		// server wants to be secure
const uint32 k_unServerFlagDedicated	= 0x04;		// server is dedicated
const uint32 k_unServerFlagLinux		= 0x08;		// linux build
const uint32 k_unServerFlagPassworded	= 0x10;		// password protected
const uint32 k_unServerFlagPrivate		= 0x20;		// server shouldn't list on master server and
#if defined( VALVE_CALLBACK_PACK_SMALL )
#pragma pack( push, 4 )
#elif defined( VALVE_CALLBACK_PACK_LARGE )
#pragma pack( push, 8 )
#else
#error isteamclient.hpp must be included
#endif 
struct GSClientApprove_t
{
	enum { k_iCallback = k_iSteamGameServerCallbacks + 1 };
	CSteamID m_ID;			// SteamID of approved player
	CSteamID m_OwnerSteamID;	// SteamID of original owner for game license
};
struct GSClientDeny_t
{
	enum { k_iCallback = k_iSteamGameServerCallbacks + 2 };
	CSteamID m_ID;
	EDenyReason m_eDenyReason;
	char m_rgchOptionalText[128];
};
struct GSClientKick_t
{
	enum { k_iCallback = k_iSteamGameServerCallbacks + 3 };
	CSteamID m_ID;
	EDenyReason m_eDenyReason;
};
struct GSClientAchievementStatus_t
{
	enum { k_iCallback = k_iSteamGameServerCallbacks + 6 };
	uint64 m_ID;
	char m_pchAchievement[128];
	bool m_bUnlocked;
};
struct GSPolicyResponse_t
{
	enum { k_iCallback = k_iSteamUserCallbacks + 15 };
	uint8 m_bSecure;
};
struct GSGameplayStats_t
{
	enum { k_iCallback = k_iSteamGameServerCallbacks + 7 };
	EResult m_eResult;					// Result of the call
	int32	m_nRank;					// Overall rank of the server (0-based)
	uint32	m_unTotalConnects;			// Total number of clients who have ever connected to the server
	uint32	m_unTotalMinutesPlayed;		// Total number of minutes ever played on the server
};
struct GSClientGroupStatus_t
{
	enum { k_iCallback = k_iSteamGameServerCallbacks + 8 };
	CSteamID m_IDUser;
	CSteamID m_IDGroup;
	bool m_bMember;
	bool m_bOfficer;
};
struct GSReputation_t
{
	enum { k_iCallback = k_iSteamGameServerCallbacks + 9 };
	EResult	m_eResult;				// Result of the call;
	uint32	m_unReputationScore;	// The reputation score for the game server
	bool	m_bBanned;				// True if the server is banned from the Steam
	uint32	m_unBannedIP;		// The IP of the banned server
	uint16	m_usBannedPort;		// The port of the banned server
	uint64	m_ulBannedGameID;	// The game ID the banned server is serving
	uint32	m_unBanExpires;		// Time the ban expires, expressed in the Unix epoch (seconds since 1/1/1970)
};
struct AssociateWithClanResult_t
{
	enum { k_iCallback = k_iSteamGameServerCallbacks + 10 };
	EResult	m_eResult;				// Result of the call;
};
struct ComputeNewPlayerCompatibilityResult_t
{
	enum { k_iCallback = k_iSteamGameServerCallbacks + 11 };
	EResult	m_eResult;				// Result of the call;
	int m_cPlayersThatDontLikeCandidate;
	int m_cPlayersThatCandidateDoesntLike;
	int m_cClanPlayersThatDontLikeCandidate;
	CSteamID m_IDCandidate;
};
#pragma pack( pop )
#endif // ISTEAMGAMESERVER_H
