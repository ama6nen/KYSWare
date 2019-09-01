#ifndef ISTEAMMATCHMAKING
#define ISTEAMMATCHMAKING
#ifdef _WIN32
#pragma once
#endif
#include "steamtypes.hpp"
#include "steamclientpublic.hpp"
#include "matchmakingtypes.hpp" 
#include "isteamclient.hpp"
#include "isteamfriends.hpp"
enum ELobbyType
{
	k_ELobbyTypePrivate = 0,		// only way to join the lobby is to invite to someone else
	k_ELobbyTypeFriendsOnly = 1,	// shows for friends or invitees, but not in lobby list
	k_ELobbyTypePublic = 2,			// visible for friends and in lobby list
	k_ELobbyTypeInvisible = 3,		// returned by search, but not visible to other friends 
};
enum ELobbyComparison
{
	k_ELobbyComparisonEqualToOrLessThan = -2,
	k_ELobbyComparisonLessThan = -1,
	k_ELobbyComparisonEqual = 0,
	k_ELobbyComparisonGreaterThan = 1,
	k_ELobbyComparisonEqualToOrGreaterThan = 2,
	k_ELobbyComparisonNotEqual = 3,
};
enum ELobbyDistanceFilter
{
	k_ELobbyDistanceFilterClose,		// only lobbies in the same immediate region will be returned
	k_ELobbyDistanceFilterDefault,		// only lobbies in the same region or near by regions
	k_ELobbyDistanceFilterFar,			// for games that don't have many latency requirements, will return lobbies about half-way around the globe
	k_ELobbyDistanceFilterWorldwide,	// no filtering, will match lobbies as far as India to NY (not recommended, expect multiple seconds of latency between the clients)
};
#define k_nMaxLobbyKeyLength 255
class ISteamMatchmaking
{
public:
	virtual int GetFavoriteGameCount() = 0;
	virtual bool GetFavoriteGame( int iGame, AppId_t *pnAppID, uint32 *pnIP, uint16 *pnConnPort, uint16 *pnQueryPort, uint32 *punFlags, uint32 *pRTime32LastPlayedOnServer ) = 0;
	virtual int AddFavoriteGame( AppId_t nAppID, uint32 nIP, uint16 nConnPort, uint16 nQueryPort, uint32 unFlags, uint32 rTime32LastPlayedOnServer ) = 0;
	virtual bool RemoveFavoriteGame( AppId_t nAppID, uint32 nIP, uint16 nConnPort, uint16 nQueryPort, uint32 unFlags ) = 0;
	///////
	/*
		class CMyLobbyListManager
		{
			CCallResult<CMyLobbyListManager, LobbyMatchList_t> m_CallResultLobbyMatchList;
			void FindLobbies()
			{
				SteamAPICall_t hSteamAPICall = matchmaking()->RequestLobbyList();
				m_CallResultLobbyMatchList.Set( hSteamAPICall, this, &CMyLobbyListManager::OnLobbyMatchList );
			}
			void OnLobbyMatchList( LobbyMatchList_t *pLobbyMatchList, bool bIOFailure )
			{
			}
		}
	*/
	virtual SteamAPICall_t RequestLobbyList() = 0;
	virtual void AddRequestLobbyListStringFilter( const char *pchKeyToMatch, const char *pchValueToMatch, ELobbyComparison eComparisonType ) = 0;
	virtual void AddRequestLobbyListNumericalFilter( const char *pchKeyToMatch, int nValueToMatch, ELobbyComparison eComparisonType ) = 0;
	virtual void AddRequestLobbyListNearValueFilter( const char *pchKeyToMatch, int nValueToBeCloseTo ) = 0;
	virtual void AddRequestLobbyListFilterSlotsAvailable( int nSlotsAvailable ) = 0;
	virtual void AddRequestLobbyListDistanceFilter( ELobbyDistanceFilter eLobbyDistanceFilter ) = 0;
	virtual void AddRequestLobbyListResultCountFilter( int cMaxResults ) = 0;
	virtual void AddRequestLobbyListCompatibleMembersFilter( CSteamID steamIDLobby ) = 0;
	virtual CSteamID GetLobbyByIndex( int iLobby ) = 0;
	CALL_RESULT( LobbyCreated_t )
	virtual SteamAPICall_t CreateLobby( ELobbyType eLobbyType, int cMaxMembers ) = 0;
	CALL_RESULT( LobbyEnter_t )
	virtual SteamAPICall_t JoinLobby( CSteamID steamIDLobby ) = 0;
	virtual void LeaveLobby( CSteamID steamIDLobby ) = 0;
	virtual bool InviteUserToLobby( CSteamID steamIDLobby, CSteamID steamIDInvitee ) = 0;
	virtual int GetNumLobbyMembers( CSteamID steamIDLobby ) = 0;
	virtual CSteamID GetLobbyMemberByIndex( CSteamID steamIDLobby, int iMember ) = 0;
	virtual const char *GetLobbyData( CSteamID steamIDLobby, const char *pchKey ) = 0;
	virtual bool SetLobbyData( CSteamID steamIDLobby, const char *pchKey, const char *pchValue ) = 0;
	virtual int GetLobbyDataCount( CSteamID steamIDLobby ) = 0;
	virtual bool GetLobbyDataByIndex( CSteamID steamIDLobby, int iLobbyData, char *pchKey, int cchKeyBufferSize, char *pchValue, int cchValueBufferSize ) = 0;
	virtual bool DeleteLobbyData( CSteamID steamIDLobby, const char *pchKey ) = 0;
	virtual const char *GetLobbyMemberData( CSteamID steamIDLobby, CSteamID steamIDUser, const char *pchKey ) = 0;
	virtual void SetLobbyMemberData( CSteamID steamIDLobby, const char *pchKey, const char *pchValue ) = 0;
	virtual bool SendLobbyChatMsg( CSteamID steamIDLobby, const void *pvMsgBody, int cubMsgBody ) = 0;
	virtual int GetLobbyChatEntry( CSteamID steamIDLobby, int iChatID, OUT_STRUCT() CSteamID *pSteamIDUser, void *pvData, int cubData, EChatEntryType *peChatEntryType ) = 0;
	virtual bool RequestLobbyData( CSteamID steamIDLobby ) = 0;
	virtual void SetLobbyGameServer( CSteamID steamIDLobby, uint32 unGameServerIP, uint16 unGameServerPort, CSteamID steamIDGameServer ) = 0;
	virtual bool GetLobbyGameServer( CSteamID steamIDLobby, uint32 *punGameServerIP, uint16 *punGameServerPort, OUT_STRUCT() CSteamID *psteamIDGameServer ) = 0;
	virtual bool SetLobbyMemberLimit( CSteamID steamIDLobby, int cMaxMembers ) = 0;
	virtual int GetLobbyMemberLimit( CSteamID steamIDLobby ) = 0;
	virtual bool SetLobbyType( CSteamID steamIDLobby, ELobbyType eLobbyType ) = 0;
	virtual bool SetLobbyJoinable( CSteamID steamIDLobby, bool bLobbyJoinable ) = 0;
	virtual CSteamID GetLobbyOwner( CSteamID steamIDLobby ) = 0;
	virtual bool SetLobbyOwner( CSteamID steamIDLobby, CSteamID steamIDNewOwner ) = 0;
	virtual bool SetLinkedLobby( CSteamID steamIDLobby, CSteamID steamIDLobbyDependent ) = 0;
#ifdef _PS3
	virtual void CheckForPSNGameBootInvite( unsigned int iGameBootAttributes  ) = 0;
#endif
	CALL_BACK( LobbyChatUpdate_t )
};
#define STEAMMATCHMAKING_INTERFACE_VERSION "SteamMatchMaking009"
typedef void* HServerListRequest;
class ISteamMatchmakingServerListResponse
{
public:
	virtual void ServerResponded( HServerListRequest hRequest, int iServer ) = 0; 
	virtual void ServerFailedToRespond( HServerListRequest hRequest, int iServer ) = 0; 
	virtual void RefreshComplete( HServerListRequest hRequest, EMatchMakingServerResponse response ) = 0; 
};
class ISteamMatchmakingPingResponse
{
public:
	virtual void ServerResponded( gameserveritem_t &server ) = 0;
	virtual void ServerFailedToRespond() = 0;
};
class ISteamMatchmakingPlayersResponse
{
public:
	virtual void AddPlayerToList( const char *pchName, int nScore, float flTimePlayed ) = 0;
	virtual void PlayersFailedToRespond() = 0;
	virtual void PlayersRefreshComplete() = 0;
};
class ISteamMatchmakingRulesResponse
{
public:
	virtual void RulesResponded( const char *pchRule, const char *pchValue ) = 0;
	virtual void RulesFailedToRespond() = 0;
	virtual void RulesRefreshComplete() = 0;
};
typedef int HServerQuery;
const int HSERVERQUERY_INVALID = 0xffffffff;
class ISteamMatchmakingServers
{
public:
	virtual HServerListRequest RequestInternetServerList( AppId_t iApp, ARRAY_COUNT(nFilters) MatchMakingKeyValuePair_t **ppchFilters, uint32 nFilters, ISteamMatchmakingServerListResponse *pRequestServersResponse ) = 0;
	virtual HServerListRequest RequestLANServerList( AppId_t iApp, ISteamMatchmakingServerListResponse *pRequestServersResponse ) = 0;
	virtual HServerListRequest RequestFriendsServerList( AppId_t iApp, ARRAY_COUNT(nFilters) MatchMakingKeyValuePair_t **ppchFilters, uint32 nFilters, ISteamMatchmakingServerListResponse *pRequestServersResponse ) = 0;
	virtual HServerListRequest RequestFavoritesServerList( AppId_t iApp, ARRAY_COUNT(nFilters) MatchMakingKeyValuePair_t **ppchFilters, uint32 nFilters, ISteamMatchmakingServerListResponse *pRequestServersResponse ) = 0;
	virtual HServerListRequest RequestHistoryServerList( AppId_t iApp, ARRAY_COUNT(nFilters) MatchMakingKeyValuePair_t **ppchFilters, uint32 nFilters, ISteamMatchmakingServerListResponse *pRequestServersResponse ) = 0;
	virtual HServerListRequest RequestSpectatorServerList( AppId_t iApp, ARRAY_COUNT(nFilters) MatchMakingKeyValuePair_t **ppchFilters, uint32 nFilters, ISteamMatchmakingServerListResponse *pRequestServersResponse ) = 0;
	virtual void ReleaseRequest( HServerListRequest hServerListRequest ) = 0;
	/* the filter operation codes that go in the key part of MatchMakingKeyValuePair_t should be one of these:
		"map"
			- Server passes the filter if the server is playing the specified map.
		"gamedataand"
			- Server passes the filter if the server's game data (ISteamGameServer::SetGameData) contains all of the
			specified strings.  The value field is a comma-delimited list of strings to match.
		"gamedataor"
			- Server passes the filter if the server's game data (ISteamGameServer::SetGameData) contains at least one of the
			specified strings.  The value field is a comma-delimited list of strings to match.
		"gamedatanor"
			- Server passes the filter if the server's game data (ISteamGameServer::SetGameData) does not contain any
			of the specified strings.  The value field is a comma-delimited list of strings to check.
		"gametagsand"
			- Server passes the filter if the server's game tags (ISteamGameServer::SetGameTags) contains all
			of the specified strings.  The value field is a comma-delimited list of strings to check.
		"gametagsnor"
			- Server passes the filter if the server's game tags (ISteamGameServer::SetGameTags) does not contain any
			of the specified strings.  The value field is a comma-delimited list of strings to check.
		"and" (x1 && x2 && ... && xn)
		"or" (x1 || x2 || ... || xn)
		"nand" !(x1 && x2 && ... && xn)
		"nor" !(x1 || x2 || ... || xn)
			- Performs Boolean operation on the following filters.  The operand to this filter specifies
			the "size" of the Boolean inputs to the operation, in Key/value pairs.  (The keyvalue
			pairs must immediately follow, i.e. this is a prefix logical operator notation.)
			In the simplest case where Boolean expressions are not nested, this is simply
			the number of operands.
			For example, to match servers on a particular map or with a particular tag, would would
			use these filters.
				( server.map == "cp_dustbowl" || server.gametags.contains("payload") )
				"or", "2"
				"map", "cp_dustbowl"
				"gametagsand", "payload"
			If logical inputs are nested, then the operand specifies the size of the entire
			"length" of its operands, not the number of immediate children.
				( server.map == "cp_dustbowl" || ( server.gametags.contains("payload") && !server.gametags.contains("payloadrace") ) )
				"or", "4"
				"map", "cp_dustbowl"
				"and", "2"
				"gametagsand", "payload"
				"gametagsnor", "payloadrace"
			Unary NOT can be achieved using either "nand" or "nor" with a single operand.
		"addr"
			- Server passes the filter if the server's query address matches the specified IP or IP:port.
		"gameaddr"
			- Server passes the filter if the server's game address matches the specified IP or IP:port.
		The following filter operations ignore the "value" part of MatchMakingKeyValuePair_t
		"dedicated"
			- Server passes the filter if it passed true to SetDedicatedServer.
		"secure"
			- Server passes the filter if the server is VAC-enabled.
		"notfull"
			- Server passes the filter if the player count is less than the reported max player count.
		"hasplayers"
			- Server passes the filter if the player count is greater than zero.
		"noplayers"
			- Server passes the filter if it doesn't have any players.
		"linux"
			- Server passes the filter if it's a linux server
	*/
	virtual gameserveritem_t *GetServerDetails( HServerListRequest hRequest, int iServer ) = 0; 
	virtual void CancelQuery( HServerListRequest hRequest ) = 0; 
	virtual void RefreshQuery( HServerListRequest hRequest ) = 0; 
	virtual bool IsRefreshing( HServerListRequest hRequest ) = 0; 
	virtual int GetServerCount( HServerListRequest hRequest ) = 0; 
	virtual void RefreshServer( HServerListRequest hRequest, int iServer ) = 0; 
	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	virtual HServerQuery PingServer( uint32 unIP, uint16 usPort, ISteamMatchmakingPingResponse *pRequestServersResponse ) = 0; 
	virtual HServerQuery PlayerDetails( uint32 unIP, uint16 usPort, ISteamMatchmakingPlayersResponse *pRequestServersResponse ) = 0;
	virtual HServerQuery ServerRules( uint32 unIP, uint16 usPort, ISteamMatchmakingRulesResponse *pRequestServersResponse ) = 0; 
	virtual void CancelServerQuery( HServerQuery hServerQuery ) = 0; 
};
#define STEAMMATCHMAKINGSERVERS_INTERFACE_VERSION "SteamMatchMakingServers002"
const uint32 k_unFavoriteFlagNone			= 0x00;
const uint32 k_unFavoriteFlagFavorite		= 0x01; // this game favorite entry is for the favorites list
const uint32 k_unFavoriteFlagHistory		= 0x02; // this game favorite entry is for the history list
enum EChatMemberStateChange
{
	k_EChatMemberStateChangeEntered			= 0x0001,		// This user has joined or is joining the chat room
	k_EChatMemberStateChangeLeft			= 0x0002,		// This user has left or is leaving the chat room
	k_EChatMemberStateChangeDisconnected	= 0x0004,		// User disconnected without leaving the chat first
	k_EChatMemberStateChangeKicked			= 0x0008,		// User kicked
	k_EChatMemberStateChangeBanned			= 0x0010,		// User kicked and banned
};
#define BChatMemberStateChangeRemoved( rgfChatMemberStateChangeFlags ) ( rgfChatMemberStateChangeFlags & ( k_EChatMemberStateChangeDisconnected | k_EChatMemberStateChangeLeft | k_EChatMemberStateChangeKicked | k_EChatMemberStateChangeBanned ) )
#if defined( VALVE_CALLBACK_PACK_SMALL )
#pragma pack( push, 4 )
#elif defined( VALVE_CALLBACK_PACK_LARGE )
#pragma pack( push, 8 )
#else
#error isteamclient.hpp must be included
#endif 
struct FavoritesListChanged_t
{
	enum { k_iCallback = k_iSteamMatchmakingCallbacks + 2 };
	uint32 m_nIP; // an IP of 0 means reload the whole list, any other value means just one server
	uint32 m_nQueryPort;
	uint32 m_nConnPort;
	uint32 m_nAppID;
	uint32 m_nFlags;
	bool m_bAdd; // true if this is adding the entry, otherwise it is a remove
	AccountID_t m_unAccountId;
};
struct LobbyInvite_t
{
	enum { k_iCallback = k_iSteamMatchmakingCallbacks + 3 };
	uint64 m_ulSteamIDUser;		// Steam ID of the person making the invite
	uint64 m_ulSteamIDLobby;	// Steam ID of the Lobby
	uint64 m_ulGameID;			// GameID of the Lobby
};
struct LobbyEnter_t
{
	enum { k_iCallback = k_iSteamMatchmakingCallbacks + 4 };
	uint64 m_ulSteamIDLobby;							// SteamID of the Lobby you have entered
	uint32 m_rgfChatPermissions;						// Permissions of the current user
	bool m_bLocked;										// If true, then only invited users may join
	uint32 m_EChatRoomEnterResponse;	// EChatRoomEnterResponse
};
struct LobbyDataUpdate_t
{
	enum { k_iCallback = k_iSteamMatchmakingCallbacks + 5 };
	uint64 m_ulSteamIDLobby;		// steamID of the Lobby
	uint64 m_ulSteamIDMember;		// steamID of the member whose data changed, or the room itself
	uint8 m_bSuccess;				// true if we lobby data was successfully changed; 
};
struct LobbyChatUpdate_t
{
	enum { k_iCallback = k_iSteamMatchmakingCallbacks + 6 };
	uint64 m_ulSteamIDLobby;			// Lobby ID
	uint64 m_ulSteamIDUserChanged;		// user who's status in the lobby just changed - can be recipient
	uint64 m_ulSteamIDMakingChange;		// Chat member who made the change (different from SteamIDUserChange if kicking, muting, etc.)
	uint32 m_rgfChatMemberStateChange;	// bitfield of EChatMemberStateChange values
};
struct LobbyChatMsg_t
{
	enum { k_iCallback = k_iSteamMatchmakingCallbacks + 7 };
	uint64 m_ulSteamIDLobby;			// the lobby id this is in
	uint64 m_ulSteamIDUser;			// steamID of the user who has sent this message
	uint8 m_eChatEntryType;			// type of message
	uint32 m_iChatID;				// index of the chat entry to lookup
};
struct LobbyGameCreated_t
{
	enum { k_iCallback = k_iSteamMatchmakingCallbacks + 9 };
	uint64 m_ulSteamIDLobby;		// the lobby we were in
	uint64 m_ulSteamIDGameServer;	// the new game server that has been created or found for the lobby members
	uint32 m_unIP;					// IP & Port of the game server (if any)
	uint16 m_usPort;
};
struct LobbyMatchList_t
{
	enum { k_iCallback = k_iSteamMatchmakingCallbacks + 10 };
	uint32 m_nLobbiesMatching;		// Number of lobbies that matched search criteria and we have SteamIDs for
};
struct LobbyKicked_t
{
	enum { k_iCallback = k_iSteamMatchmakingCallbacks + 12 };
	uint64 m_ulSteamIDLobby;			// Lobby
	uint64 m_ulSteamIDAdmin;			// User who kicked you - possibly the ID of the lobby itself
	uint8 m_bKickedDueToDisconnect;		// true if you were kicked from the lobby due to the user losing connection to Steam (currently always true)
};
struct LobbyCreated_t
{
	enum { k_iCallback = k_iSteamMatchmakingCallbacks + 13 };
	EResult m_eResult;		// k_EResultOK - the lobby was successfully created
	uint64 m_ulSteamIDLobby;		// chat room, zero if failed
};
struct PSNGameBootInviteResult_t
{
	enum { k_iCallback = k_iSteamMatchmakingCallbacks + 15 };
	bool m_bGameBootInviteExists;
	CSteamID m_IDLobby;		// Should be valid if m_bGameBootInviteExists == true
};
struct FavoritesListAccountsUpdated_t
{
	enum { k_iCallback = k_iSteamMatchmakingCallbacks + 16 };
	EResult m_eResult;
};
#pragma pack( pop )
#endif // ISTEAMMATCHMAKING
