#ifndef ISTEAMFRIENDS_H
#define ISTEAMFRIENDS_H
#ifdef _WIN32
#pragma once
#endif
#include "isteamclient.hpp"
#include "steamclientpublic.hpp"
enum EFriendRelationship
{
	k_EFriendRelationshipNone = 0,
	k_EFriendRelationshipBlocked = 1,			               
	k_EFriendRelationshipRequestRecipient = 2,
	k_EFriendRelationshipFriend = 3,
	k_EFriendRelationshipRequestInitiator = 4,
	k_EFriendRelationshipIgnored = 5,			             
	k_EFriendRelationshipIgnoredFriend = 6,
	k_EFriendRelationshipSuggested_DEPRECATED = 7,		              
	k_EFriendRelationshipMax = 8,
};
const int k_cchMaxFriendsGroupName = 64;
const int k_cFriendsGroupLimit = 100;
typedef int16 FriendsGroupID_t;
const FriendsGroupID_t k_FriendsGroupID_Invalid = -1;
const int k_cEnumerateFollowersMax = 50;
enum EPersonaState
{
	k_EPersonaStateOffline = 0,			      
	k_EPersonaStateOnline = 1,			    
	k_EPersonaStateBusy = 2,			     
	k_EPersonaStateAway = 3,			  
	k_EPersonaStateSnooze = 4,			     
	k_EPersonaStateLookingToTrade = 5,	  
	k_EPersonaStateLookingToPlay = 6,	    
	k_EPersonaStateMax,
};
enum EFriendFlags
{
	k_EFriendFlagNone			= 0x00,
	k_EFriendFlagBlocked		= 0x01,
	k_EFriendFlagFriendshipRequested	= 0x02,
	k_EFriendFlagImmediate		= 0x04,			  
	k_EFriendFlagClanMember		= 0x08,
	k_EFriendFlagOnGameServer	= 0x10,	
	k_EFriendFlagRequestingFriendship = 0x80,
	k_EFriendFlagRequestingInfo = 0x100,
	k_EFriendFlagIgnored		= 0x200,
	k_EFriendFlagIgnoredFriend	= 0x400,
	k_EFriendFlagChatMember		= 0x1000,
	k_EFriendFlagAll			= 0xFFFF,
};
#if defined( VALVE_CALLBACK_PACK_SMALL )
#pragma pack( push, 4 )
#elif defined( VALVE_CALLBACK_PACK_LARGE )
#pragma pack( push, 8 )
#else
#error isteamclient.hpp must be included
#endif 
struct FriendGameInfo_t
{
	CGameID m_gameID;
	uint32 m_unGameIP;
	uint16 m_usGamePort;
	uint16 m_usQueryPort;
	CSteamID m_IDLobby;
};
#pragma pack( pop )
enum
{
	k_cchPersonaNameMax = 128,
	k_cwchPersonaNameMax = 32,
};
enum EUserRestriction
{
	k_nUserRestrictionNone		= 0,	    
	k_nUserRestrictionUnknown	= 1,	      
	k_nUserRestrictionAnyChat	= 2,	          
	k_nUserRestrictionVoiceChat	= 4,	          
	k_nUserRestrictionGroupChat	= 8,	          
	k_nUserRestrictionRating	= 16,	          
	k_nUserRestrictionGameInvites	= 32,	         
	k_nUserRestrictionTrading	= 64,	       
};
struct FriendSessionStateInfo_t
{
	uint32 m_uiOnlineSessionInstances;
	uint8 m_uiPublishedToFriendsSessionInstance;
};
const uint32 k_cubChatMetadataMax = 8192;
enum { k_cchMaxRichPresenceKeys = 20 };
enum { k_cchMaxRichPresenceKeyLength = 64 };
enum { k_cchMaxRichPresenceValueLength = 256 };
enum EOverlayToStoreFlag
{
	k_EOverlayToStoreFlag_None = 0,
	k_EOverlayToStoreFlag_AddToCart = 1,
	k_EOverlayToStoreFlag_AddToCartAndShow = 2,
};
class ISteamFriends
{
public:
	virtual const char *GetPersonaName() = 0;
	CALL_RESULT( SetPersonaNameResponse_t )
	virtual SteamAPICall_t set_name( const char *pchPersonaName ) = 0;
	virtual EPersonaState GetPersonaState() = 0;
	virtual int GetFriendCount( int iFriendFlags ) = 0;
	virtual CSteamID GetFriendByIndex( int iFriend, int iFriendFlags ) = 0;
	virtual EFriendRelationship GetFriendRelationship( CSteamID steamIDFriend ) = 0;
	virtual EPersonaState GetFriendPersonaState( CSteamID steamIDFriend ) = 0;
	virtual const char *GetFriendPersonaName( CSteamID steamIDFriend ) = 0;
	virtual bool GetFriendGamePlayed( CSteamID steamIDFriend, OUT_STRUCT() FriendGameInfo_t *pFriendGameInfo ) = 0;
	virtual const char *GetFriendPersonaNameHistory( CSteamID steamIDFriend, int iPersonaName ) = 0;
	virtual int GetFriendSteamLevel( CSteamID steamIDFriend ) = 0;
	virtual const char *GetPlayerNickname( CSteamID steamIDPlayer ) = 0;
	virtual int GetFriendsGroupCount() = 0;
	virtual FriendsGroupID_t GetFriendsGroupIDByIndex( int iFG ) = 0;
	virtual const char *GetFriendsGroupName( FriendsGroupID_t friendsGroupID ) = 0;
	virtual int GetFriendsGroupMembersCount( FriendsGroupID_t friendsGroupID ) = 0;
	virtual void GetFriendsGroupMembersList( FriendsGroupID_t friendsGroupID, OUT_ARRAY_CALL(nMembersCount, GetFriendsGroupMembersCount, friendsGroupID ) CSteamID *pOutSteamIDMembers, int nMembersCount ) = 0;
	virtual bool HasFriend( CSteamID steamIDFriend, int iFriendFlags ) = 0;
	virtual int GetClanCount() = 0;
	virtual CSteamID GetClanByIndex( int iClan ) = 0;
	virtual const char *GetClanName( CSteamID steamIDClan ) = 0;
	virtual const char *GetClanTag( CSteamID steamIDClan ) = 0;
	virtual bool GetClanActivityCounts( CSteamID steamIDClan, int *pnOnline, int *pnInGame, int *pnChatting ) = 0;
	virtual SteamAPICall_t DownloadClanActivityCounts( ARRAY_COUNT(cClansToRequest) CSteamID *psteamIDClans, int cClansToRequest ) = 0;
	virtual int GetFriendCountFromSource( CSteamID steamIDSource ) = 0;
	virtual CSteamID GetFriendFromSourceByIndex( CSteamID steamIDSource, int iFriend ) = 0;
	virtual bool IsUserInSource( CSteamID steamIDUser, CSteamID steamIDSource ) = 0;
	virtual void SetInGameVoiceSpeaking( CSteamID steamIDUser, bool bSpeaking ) = 0;
	virtual void ActivateGameOverlay( const char *pchDialog ) = 0;
	virtual void ActivateGameOverlayToUser( const char *pchDialog, CSteamID steamID ) = 0;
	virtual void ActivateGameOverlayToWebPage( const char *pchURL ) = 0;
	virtual void ActivateGameOverlayToStore( AppId_t nAppID, EOverlayToStoreFlag eFlag ) = 0;
	virtual void SetPlayedWith( CSteamID steamIDUserPlayedWith ) = 0;
	virtual void ActivateGameOverlayInviteDialog( CSteamID steamIDLobby ) = 0;
	virtual int GetSmallFriendAvatar( CSteamID steamIDFriend ) = 0;
	virtual int GetMediumFriendAvatar( CSteamID steamIDFriend ) = 0;
	virtual int GetLargeFriendAvatar( CSteamID steamIDFriend ) = 0;
	virtual bool RequestUserInformation( CSteamID steamIDUser, bool bRequireNameOnly ) = 0;
	CALL_RESULT( ClanOfficerListResponse_t )
	virtual SteamAPICall_t RequestClanOfficerList( CSteamID steamIDClan ) = 0;
	virtual CSteamID GetClanOwner( CSteamID steamIDClan ) = 0;
	virtual int GetClanOfficerCount( CSteamID steamIDClan ) = 0;
	virtual CSteamID GetClanOfficerByIndex( CSteamID steamIDClan, int iOfficer ) = 0;
	virtual uint32 GetUserRestrictions() = 0;
	virtual bool set_rpc( const char *pchKey, const char *pchValue ) = 0;
	virtual void ClearRichPresence() = 0;
	virtual const char *GetFriendRichPresence( CSteamID steamIDFriend, const char *pchKey ) = 0;
	virtual int GetFriendRichPresenceKeyCount( CSteamID steamIDFriend ) = 0;
	virtual const char *GetFriendRichPresenceKeyByIndex( CSteamID steamIDFriend, int iKey ) = 0;
	virtual void RequestFriendRichPresence( CSteamID steamIDFriend ) = 0;
	virtual bool InviteUserToGame( CSteamID steamIDFriend, const char *pchConnectString ) = 0;
	virtual int GetCoplayFriendCount() = 0;
	virtual CSteamID GetCoplayFriend( int iCoplayFriend ) = 0;
	virtual int GetFriendCoplayTime( CSteamID steamIDFriend ) = 0;
	virtual AppId_t GetFriendCoplayGame( CSteamID steamIDFriend ) = 0;
	CALL_RESULT( JoinClanChatRoomCompletionResult_t )
	virtual SteamAPICall_t JoinClanChatRoom( CSteamID steamIDClan ) = 0;
	virtual bool LeaveClanChatRoom( CSteamID steamIDClan ) = 0;
	virtual int GetClanChatMemberCount( CSteamID steamIDClan ) = 0;
	virtual CSteamID GetChatMemberByIndex( CSteamID steamIDClan, int iUser ) = 0;
	virtual bool SendClanChatMessage( CSteamID steamIDClanChat, const char *pchText ) = 0;
	virtual int GetClanChatMessage( CSteamID steamIDClanChat, int iMessage, void *prgchText, int cchTextMax, EChatEntryType *peChatEntryType, OUT_STRUCT() CSteamID *psteamidChatter ) = 0;
	virtual bool IsClanChatAdmin( CSteamID steamIDClanChat, CSteamID steamIDUser ) = 0;
	virtual bool IsClanChatWindowOpenInSteam( CSteamID steamIDClanChat ) = 0;
	virtual bool OpenClanChatWindowInSteam( CSteamID steamIDClanChat ) = 0;
	virtual bool CloseClanChatWindowInSteam( CSteamID steamIDClanChat ) = 0;
	virtual bool SetListenForFriendsMessages( bool bInterceptEnabled ) = 0;
	virtual bool ReplyToFriendMessage( CSteamID steamIDFriend, const char *pchMsgToSend ) = 0;
	virtual int GetFriendMessage( CSteamID steamIDFriend, int iMessageID, void *pvData, int cubData, EChatEntryType *peChatEntryType ) = 0;
	CALL_RESULT( FriendsGetFollowerCount_t )
	virtual SteamAPICall_t GetFollowerCount( CSteamID steamID ) = 0;
	CALL_RESULT( FriendsIsFollowing_t )
	virtual SteamAPICall_t IsFollowing( CSteamID steamID ) = 0;
	CALL_RESULT( FriendsEnumerateFollowingList_t )
	virtual SteamAPICall_t EnumerateFollowingList( uint32 unStartIndex ) = 0;
	virtual bool IsClanPublic( CSteamID steamIDClan ) = 0;
	virtual bool IsClanOfficialGameGroup( CSteamID steamIDClan ) = 0;
};
#define STEAMFRIENDS_INTERFACE_VERSION "SteamFriends015"
#if defined( VALVE_CALLBACK_PACK_SMALL )
#pragma pack( push, 4 )
#elif defined( VALVE_CALLBACK_PACK_LARGE )
#pragma pack( push, 8 )
#else
#error isteamclient.hpp must be included
#endif 
struct PersonaStateChange_t
{
	enum { k_iCallback = k_iSteamFriendsCallbacks + 4 };
	uint64 m_ulSteamID;		      
	int m_nChangeFlags;		  
};
enum EPersonaChange
{
	k_EPersonaChangeName		= 0x0001,
	k_EPersonaChangeStatus		= 0x0002,
	k_EPersonaChangeComeOnline	= 0x0004,
	k_EPersonaChangeGoneOffline	= 0x0008,
	k_EPersonaChangeGamePlayed	= 0x0010,
	k_EPersonaChangeGameServer	= 0x0020,
	k_EPersonaChangeAvatar		= 0x0040,
	k_EPersonaChangeJoinedSource= 0x0080,
	k_EPersonaChangeLeftSource	= 0x0100,
	k_EPersonaChangeRelationshipChanged = 0x0200,
	k_EPersonaChangeNameFirstSet = 0x0400,
	k_EPersonaChangeFacebookInfo = 0x0800,
	k_EPersonaChangeNickname =	0x1000,
	k_EPersonaChangeSteamLevel = 0x2000,
};
struct GameOverlayActivated_t
{
	enum { k_iCallback = k_iSteamFriendsCallbacks + 31 };
	uint8 m_bActive;	        
};
struct GameServerChangeRequested_t
{
	enum { k_iCallback = k_iSteamFriendsCallbacks + 32 };
	char m_rgchServer[64];		    
	char m_rgchPassword[64];	    
};
struct GameLobbyJoinRequested_t
{
	enum { k_iCallback = k_iSteamFriendsCallbacks + 33 };
	CSteamID m_IDLobby;
	CSteamID m_IDFriend;		
};
struct AvatarImageLoaded_t
{
	enum { k_iCallback = k_iSteamFriendsCallbacks + 34 };
	CSteamID m_ID;        
	int m_iImage;         
	int m_iWide;      
	int m_iTall;      
};
struct ClanOfficerListResponse_t
{
	enum { k_iCallback = k_iSteamFriendsCallbacks + 35 };
	CSteamID m_IDClan;
	int m_cOfficers;
	uint8 m_bSuccess;
};
struct FriendRichPresenceUpdate_t
{
	enum { k_iCallback = k_iSteamFriendsCallbacks + 36 };
	CSteamID m_IDFriend;	      
	AppId_t m_nAppID;			           
};
struct GameRichPresenceJoinRequested_t
{
	enum { k_iCallback = k_iSteamFriendsCallbacks + 37 };
	CSteamID m_IDFriend;		                
	char m_rgchConnect[k_cchMaxRichPresenceValueLength];
};
struct GameConnectedClanChatMsg_t
{
	enum { k_iCallback = k_iSteamFriendsCallbacks + 38 };
	CSteamID m_IDClanChat;
	CSteamID m_IDUser;
	int m_iMessageID;
};
struct GameConnectedChatJoin_t
{
	enum { k_iCallback = k_iSteamFriendsCallbacks + 39 };
	CSteamID m_IDClanChat;
	CSteamID m_IDUser;
};
struct GameConnectedChatLeave_t
{
	enum { k_iCallback = k_iSteamFriendsCallbacks + 40 };
	CSteamID m_IDClanChat;
	CSteamID m_IDUser;
	bool m_bKicked;		    
	bool m_bDropped;	     
};
struct DownloadClanActivityCountsResult_t
{
	enum { k_iCallback = k_iSteamFriendsCallbacks + 41 };
	bool m_bSuccess;
};
struct JoinClanChatRoomCompletionResult_t
{
	enum { k_iCallback = k_iSteamFriendsCallbacks + 42 };
	CSteamID m_IDClanChat;
	EChatRoomEnterResponse m_eChatRoomEnterResponse;
};
struct GameConnectedFriendChatMsg_t
{
	enum { k_iCallback = k_iSteamFriendsCallbacks + 43 };
	CSteamID m_IDUser;
	int m_iMessageID;
};
struct FriendsGetFollowerCount_t
{
	enum { k_iCallback = k_iSteamFriendsCallbacks + 44 };
	EResult m_eResult;
	CSteamID m_ID;
	int m_nCount;
};
struct FriendsIsFollowing_t
{
	enum { k_iCallback = k_iSteamFriendsCallbacks + 45 };
	EResult m_eResult;
	CSteamID m_ID;
	bool m_bIsFollowing;
};
struct FriendsEnumerateFollowingList_t
{
	enum { k_iCallback = k_iSteamFriendsCallbacks + 46 };
	EResult m_eResult;
	CSteamID m_rgSteamID[ k_cEnumerateFollowersMax ];
	int32 m_nResultsReturned;
	int32 m_nTotalResultCount;
};
struct SetPersonaNameResponse_t
{
	enum { k_iCallback = k_iSteamFriendsCallbacks + 47 };
	bool m_bSuccess;       
	bool m_bLocalSuccess;                   
	EResult m_result;    
};
#pragma pack( pop )
#endif  
