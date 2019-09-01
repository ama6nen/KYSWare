#ifndef ISTEAMUSERSTATS_H
#define ISTEAMUSERSTATS_H
#ifdef _WIN32
#pragma once
#endif
#include "isteamclient.hpp"
#include "isteamremotestorage.hpp"
enum { k_cchStatNameMax = 128 };
enum { k_cchLeaderboardNameMax = 128 };
enum { k_cLeaderboardDetailsMax = 64 };
typedef uint64 SteamLeaderboard_t;
typedef uint64 SteamLeaderboardEntries_t;
enum ELeaderboardDataRequest
{
	k_ELeaderboardDataRequestGlobal = 0,
	k_ELeaderboardDataRequestGlobalAroundUser = 1,
	k_ELeaderboardDataRequestFriends = 2,
	k_ELeaderboardDataRequestUsers = 3
};
enum ELeaderboardSortMethod
{
	k_ELeaderboardSortMethodNone = 0,
	k_ELeaderboardSortMethodAscending = 1,	    
	k_ELeaderboardSortMethodDescending = 2,	    
};
enum ELeaderboardDisplayType
{
	k_ELeaderboardDisplayTypeNone = 0, 
	k_ELeaderboardDisplayTypeNumeric = 1,			   
	k_ELeaderboardDisplayTypeTimeSeconds = 2,		       
	k_ELeaderboardDisplayTypeTimeMilliSeconds = 3,	       
};
enum ELeaderboardUploadScoreMethod
{
	k_ELeaderboardUploadScoreMethodNone = 0,
	k_ELeaderboardUploadScoreMethodKeepBest = 1,	      
	k_ELeaderboardUploadScoreMethodForceUpdate = 2,	       
};
#if defined( VALVE_CALLBACK_PACK_SMALL )
#pragma pack( push, 4 )
#elif defined( VALVE_CALLBACK_PACK_LARGE )
#pragma pack( push, 8 )
#else
#error isteamclient.hpp must be included
#endif 
struct LeaderboardEntry_t
{
	CSteamID m_IDUser;              
	int32 m_nGlobalRank;	              
	int32 m_nScore;			      
	int32 m_cDetails;		        
	UGCHandle_t m_hUGC;		       
};
#pragma pack( pop )
class ISteamUserStats
{
public:
	CALL_BACK( UserStatsReceived_t )
	virtual bool request_current() = 0;
	virtual bool GetStat( const char *pchName, int32 *pData ) = 0;
	virtual bool GetStat( const char *pchName, float *pData ) = 0;
	virtual bool SetStat( const char *pchName, int32 nData ) = 0;
	virtual bool SetStat( const char *pchName, float fData ) = 0;
	virtual bool UpdateAvgRateStat( const char *pchName, float flCountThisSession, double dSessionLength ) = 0;
	virtual bool get_achievement( const char *pchName, bool *pbAchieved ) = 0;
	virtual bool set_achievement( const char *pchName ) = 0;
	virtual bool ClearAchievement( const char *pchName ) = 0;
	virtual bool GetAchievementAndUnlockTime( const char *pchName, bool *pbAchieved, uint32 *punUnlockTime ) = 0;
	virtual bool store() = 0;
	virtual int GetAchievementIcon( const char *pchName ) = 0;
	virtual const char *GetAchievementDisplayAttribute( const char *pchName, const char *pchKey ) = 0;
	virtual bool IndicateAchievementProgress( const char *pchName, uint32 nCurProgress, uint32 nMaxProgress ) = 0;
	virtual uint32 num_achievements() = 0;
	virtual const char *get_ach_name( uint32 iAchievement ) = 0;
	CALL_RESULT( UserStatsReceived_t )
	virtual SteamAPICall_t RequestUserStats( CSteamID steamIDUser ) = 0;
	virtual bool GetUserStat( CSteamID steamIDUser, const char *pchName, int32 *pData ) = 0;
	virtual bool GetUserStat( CSteamID steamIDUser, const char *pchName, float *pData ) = 0;
	virtual bool GetUserAchievement( CSteamID steamIDUser, const char *pchName, bool *pbAchieved ) = 0;
	virtual bool GetUserAchievementAndUnlockTime( CSteamID steamIDUser, const char *pchName, bool *pbAchieved, uint32 *punUnlockTime ) = 0;
	virtual bool ResetAllStats( bool bAchievementsToo ) = 0;
	CALL_RESULT(LeaderboardFindResult_t)
	virtual SteamAPICall_t FindOrCreateLeaderboard( const char *pchLeaderboardName, ELeaderboardSortMethod eLeaderboardSortMethod, ELeaderboardDisplayType eLeaderboardDisplayType ) = 0;
	CALL_RESULT( LeaderboardFindResult_t )
	virtual SteamAPICall_t FindLeaderboard( const char *pchLeaderboardName ) = 0;
	virtual const char *GetLeaderboardName( SteamLeaderboard_t hSteamLeaderboard ) = 0;
	virtual int GetLeaderboardEntryCount( SteamLeaderboard_t hSteamLeaderboard ) = 0;
	virtual ELeaderboardSortMethod GetLeaderboardSortMethod( SteamLeaderboard_t hSteamLeaderboard ) = 0;
	virtual ELeaderboardDisplayType GetLeaderboardDisplayType( SteamLeaderboard_t hSteamLeaderboard ) = 0;
	CALL_RESULT( LeaderboardScoresDownloaded_t )
	virtual SteamAPICall_t DownloadLeaderboardEntries( SteamLeaderboard_t hSteamLeaderboard, ELeaderboardDataRequest eLeaderboardDataRequest, int nRangeStart, int nRangeEnd ) = 0;
			CALL_RESULT( LeaderboardScoresDownloaded_t )
	virtual SteamAPICall_t DownloadLeaderboardEntriesForUsers( SteamLeaderboard_t hSteamLeaderboard,
															   ARRAY_COUNT_D(cUsers, Array of users to retrieve) CSteamID *prgUsers, int cUsers ) = 0;
	virtual bool GetDownloadedLeaderboardEntry( SteamLeaderboardEntries_t hSteamLeaderboardEntries, int index, LeaderboardEntry_t *pLeaderboardEntry, int32 *pDetails, int cDetailsMax ) = 0;
	CALL_RESULT( LeaderboardScoreUploaded_t )
	virtual SteamAPICall_t UploadLeaderboardScore( SteamLeaderboard_t hSteamLeaderboard, ELeaderboardUploadScoreMethod eLeaderboardUploadScoreMethod, int32 nScore, const int32 *pScoreDetails, int cScoreDetailsCount ) = 0;
	CALL_RESULT( LeaderboardUGCSet_t )
	virtual SteamAPICall_t AttachLeaderboardUGC( SteamLeaderboard_t hSteamLeaderboard, UGCHandle_t hUGC ) = 0;
	CALL_RESULT( NumberOfCurrentPlayers_t )
	virtual SteamAPICall_t GetNumberOfCurrentPlayers() = 0;
	CALL_RESULT( GlobalAchievementPercentagesReady_t )
	virtual SteamAPICall_t RequestGlobalAchievementPercentages() = 0;
	virtual int GetMostAchievedAchievementInfo( char *pchName, uint32 unNameBufLen, float *pflPercent, bool *pbAchieved ) = 0;
	virtual int GetNextMostAchievedAchievementInfo( int iIteratorPrevious, char *pchName, uint32 unNameBufLen, float *pflPercent, bool *pbAchieved ) = 0;
	virtual bool GetAchievementAchievedPercent( const char *pchName, float *pflPercent ) = 0;
	CALL_RESULT( GlobalStatsReceived_t )
	virtual SteamAPICall_t RequestGlobalStats( int nHistoryDays ) = 0;
	virtual bool GetGlobalStat( const char *pchStatName, int64 *pData ) = 0;
	virtual bool GetGlobalStat( const char *pchStatName, double *pData ) = 0;
	virtual int32 GetGlobalStatHistory( const char *pchStatName, ARRAY_COUNT(cubData) int64 *pData, uint32 cubData ) = 0;
	virtual int32 GetGlobalStatHistory( const char *pchStatName, ARRAY_COUNT(cubData) double *pData, uint32 cubData ) = 0;
};
#define STEAMUSERSTATS_INTERFACE_VERSION "STEAMUSERSTATS_INTERFACE_VERSION011"
#if defined( VALVE_CALLBACK_PACK_SMALL )
#pragma pack( push, 4 )
#elif defined( VALVE_CALLBACK_PACK_LARGE )
#pragma pack( push, 8 )
#else
#error isteamclient.hpp must be included
#endif 
struct UserStatsReceived_t
{
	enum { k_iCallback = k_iSteamUserStatsCallbacks + 1 };
	uint64		m_nGameID;		     
	EResult		m_eResult;		      
	CSteamID	m_IDUser;	         
};
struct UserStatsStored_t
{
	enum { k_iCallback = k_iSteamUserStatsCallbacks + 2 };
	uint64		m_nGameID;		     
	EResult		m_eResult;		   
};
struct UserAchievementStored_t
{
	enum { k_iCallback = k_iSteamUserStatsCallbacks + 3 };
	uint64		m_nGameID;				    
	bool		m_bGroupAchievement;	      
	char		m_rgchAchievementName[k_cchStatNameMax];		    
	uint32		m_nCurProgress;			     
	uint32		m_nMaxProgress;			    
};
struct LeaderboardFindResult_t
{
	enum { k_iCallback = k_iSteamUserStatsCallbacks + 4 };
	SteamLeaderboard_t m_hSteamLeaderboard;	           
	uint8 m_bLeaderboardFound;				     
};
struct LeaderboardScoresDownloaded_t
{
	enum { k_iCallback = k_iSteamUserStatsCallbacks + 5 };
	SteamLeaderboard_t m_hSteamLeaderboard;
	SteamLeaderboardEntries_t m_hSteamLeaderboardEntries;	      
	int m_cEntryCount;      
};
struct LeaderboardScoreUploaded_t
{
	enum { k_iCallback = k_iSteamUserStatsCallbacks + 6 };
	uint8 m_bSuccess;			      
	SteamLeaderboard_t m_hSteamLeaderboard;	     
	int32 m_nScore;				       
	uint8 m_bScoreChanged;		               
	int m_nGlobalRankNew;		          
	int m_nGlobalRankPrevious;	                     
};
struct NumberOfCurrentPlayers_t
{
	enum { k_iCallback = k_iSteamUserStatsCallbacks + 7 };
	uint8 m_bSuccess;			      
	int32 m_cPlayers;			     
};
struct UserStatsUnloaded_t
{
	enum { k_iCallback = k_iSteamUserStatsCallbacks + 8 };
	CSteamID	m_IDUser;	      
};
struct UserAchievementIconFetched_t
{
	enum { k_iCallback = k_iSteamUserStatsCallbacks + 9 };
	CGameID		m_nGameID;				    
	char		m_rgchAchievementName[k_cchStatNameMax];		    
	bool		m_bAchieved;		          
	int			m_nIconHandle;		                   
};
struct GlobalAchievementPercentagesReady_t
{
	enum { k_iCallback = k_iSteamUserStatsCallbacks + 10 };
	uint64		m_nGameID;				    
	EResult		m_eResult;				    
};
struct LeaderboardUGCSet_t
{
	enum { k_iCallback = k_iSteamUserStatsCallbacks + 11 };
	EResult m_eResult;				     
	SteamLeaderboard_t m_hSteamLeaderboard;	     
};
struct PS3TrophiesInstalled_t
{
	enum { k_iCallback = k_iSteamUserStatsCallbacks + 12 };
	uint64	m_nGameID;				     
	EResult m_eResult;				     
	uint64 m_ulRequiredDiskSpace;	              
};
struct GlobalStatsReceived_t
{
	enum { k_iCallback = k_iSteamUserStatsCallbacks + 12 };
	uint64	m_nGameID;				      
	EResult	m_eResult;				     
};
#pragma pack( pop )
#endif  
