#ifndef ISTEAMREMOTESTORAGE_H
#define ISTEAMREMOTESTORAGE_H
#ifdef _WIN32
#pragma once
#endif
#include "isteamclient.hpp"
const uint32 k_unMaxCloudFileChunkSize = 100 * 1024 * 1024;
#if defined( VALVE_CALLBACK_PACK_SMALL )
#pragma pack( push, 4 )
#elif defined( VALVE_CALLBACK_PACK_LARGE )
#pragma pack( push, 8 )
#else
#error isteamclient.hpp must be included
#endif 
struct SteamParamStringArray_t
{
	const char ** m_ppStrings;
	int32 m_nNumStrings;
};
#pragma pack( pop )
typedef uint64 UGCHandle_t;
typedef uint64 PublishedFileUpdateHandle_t;
typedef uint64 PublishedFileId_t;
const PublishedFileId_t k_PublishedFileIdInvalid = 0;
const UGCHandle_t k_UGCHandleInvalid = 0xffffffffffffffffull;
const PublishedFileUpdateHandle_t k_PublishedFileUpdateHandleInvalid = 0xffffffffffffffffull;
typedef uint64 UGCFileWriteStreamHandle_t;
const UGCFileWriteStreamHandle_t k_UGCFileStreamHandleInvalid = 0xffffffffffffffffull;
const uint32 k_cchPublishedDocumentTitleMax = 128 + 1;
const uint32 k_cchPublishedDocumentDescriptionMax = 8000;
const uint32 k_cchPublishedDocumentChangeDescriptionMax = 8000;
const uint32 k_unEnumeratePublishedFilesMaxResults = 50;
const uint32 k_cchTagListMax = 1024 + 1;
const uint32 k_cchFilenameMax = 260;
const uint32 k_cchPublishedFileURLMax = 256;
enum ERemoteStoragePlatform
{
	k_ERemoteStoragePlatformNone		= 0,
	k_ERemoteStoragePlatformWindows		= (1 << 0),
	k_ERemoteStoragePlatformOSX			= (1 << 1),
	k_ERemoteStoragePlatformPS3			= (1 << 2),
	k_ERemoteStoragePlatformLinux		= (1 << 3),
	k_ERemoteStoragePlatformReserved2	= (1 << 4),
	k_ERemoteStoragePlatformAll = 0xffffffff
};
enum ERemoteStoragePublishedFileVisibility
{
	k_ERemoteStoragePublishedFileVisibilityPublic = 0,
	k_ERemoteStoragePublishedFileVisibilityFriendsOnly = 1,
	k_ERemoteStoragePublishedFileVisibilityPrivate = 2,
};
enum EWorkshopFileType
{
	k_EWorkshopFileTypeFirst = 0,
	k_EWorkshopFileTypeCommunity			  = 0,		// normal Workshop item that can be subscribed to
	k_EWorkshopFileTypeMicrotransaction		  = 1,		// Workshop item that is meant to be voted on for the purpose of selling in-game
	k_EWorkshopFileTypeCollection			  = 2,		// a collection of Workshop or Greenlight items
	k_EWorkshopFileTypeArt					  = 3,		// artwork
	k_EWorkshopFileTypeVideo				  = 4,		// external video
	k_EWorkshopFileTypeScreenshot			  = 5,		// screenshot
	k_EWorkshopFileTypeGame					  = 6,		// Greenlight game entry
	k_EWorkshopFileTypeSoftware				  = 7,		// Greenlight software entry
	k_EWorkshopFileTypeConcept				  = 8,		// Greenlight concept
	k_EWorkshopFileTypeWebGuide				  = 9,		// Steam web guide
	k_EWorkshopFileTypeIntegratedGuide		  = 10,		// application integrated guide
	k_EWorkshopFileTypeMerch				  = 11,		// Workshop merchandise meant to be voted on for the purpose of being sold
	k_EWorkshopFileTypeControllerBinding	  = 12,		// Steam Controller bindings
	k_EWorkshopFileTypeSteamworksAccessInvite = 13,		// internal
	k_EWorkshopFileTypeSteamVideo			  = 14,		// Steam video
	k_EWorkshopFileTypeGameManagedItem		  = 15,		// managed completely by the game, not the user, and not shown on the web
	k_EWorkshopFileTypeMax = 16
};
enum EWorkshopVote
{
	k_EWorkshopVoteUnvoted = 0,
	k_EWorkshopVoteFor = 1,
	k_EWorkshopVoteAgainst = 2,
	k_EWorkshopVoteLater = 3,
};
enum EWorkshopFileAction
{
	k_EWorkshopFileActionPlayed = 0,
	k_EWorkshopFileActionCompleted = 1,
};
enum EWorkshopEnumerationType
{
	k_EWorkshopEnumerationTypeRankedByVote = 0,
	k_EWorkshopEnumerationTypeRecent = 1,
	k_EWorkshopEnumerationTypeTrending = 2,
	k_EWorkshopEnumerationTypeFavoritesOfFriends = 3,
	k_EWorkshopEnumerationTypeVotedByFriends = 4,
	k_EWorkshopEnumerationTypeContentByFriends = 5,
	k_EWorkshopEnumerationTypeRecentFromFollowedUsers = 6,
};
enum EWorkshopVideoProvider
{
	k_EWorkshopVideoProviderNone = 0,
	k_EWorkshopVideoProviderYoutube = 1
};
enum EUGCReadAction
{
	k_EUGCRead_ContinueReadingUntilFinished = 0,
	k_EUGCRead_ContinueReading = 1,
	k_EUGCRead_Close = 2,	
};
class ISteamRemoteStorage
{
	public:
		virtual bool	FileWrite( const char *pchFile, const void *pvData, int32 cubData ) = 0;
		virtual int32	FileRead( const char *pchFile, void *pvData, int32 cubDataToRead ) = 0;
		CALL_RESULT( RemoteStorageFileWriteAsyncComplete_t )
		virtual SteamAPICall_t FileWriteAsync( const char *pchFile, const void *pvData, uint32 cubData ) = 0;
		CALL_RESULT( RemoteStorageFileReadAsyncComplete_t )
		virtual SteamAPICall_t FileReadAsync( const char *pchFile, uint32 nOffset, uint32 cubToRead ) = 0;
		virtual bool	FileReadAsyncComplete( SteamAPICall_t hReadCall, void *pvBuffer, uint32 cubToRead ) = 0;
		virtual bool	FileForget( const char *pchFile ) = 0;
		virtual bool	FileDelete( const char *pchFile ) = 0;
		CALL_RESULT( RemoteStorageFileShareResult_t )
		virtual SteamAPICall_t FileShare( const char *pchFile ) = 0;
		virtual bool	SetSyncPlatforms( const char *pchFile, ERemoteStoragePlatform eRemoteStoragePlatform ) = 0;
		virtual UGCFileWriteStreamHandle_t FileWriteStreamOpen( const char *pchFile ) = 0;
		virtual bool FileWriteStreamWriteChunk( UGCFileWriteStreamHandle_t writeHandle, const void *pvData, int32 cubData ) = 0;
		virtual bool FileWriteStreamClose( UGCFileWriteStreamHandle_t writeHandle ) = 0;
		virtual bool FileWriteStreamCancel( UGCFileWriteStreamHandle_t writeHandle ) = 0;
		virtual bool	FileExists( const char *pchFile ) = 0;
		virtual bool	FilePersisted( const char *pchFile ) = 0;
		virtual int32	GetFileSize( const char *pchFile ) = 0;
		virtual int64	GetFileTimestamp( const char *pchFile ) = 0;
		virtual ERemoteStoragePlatform GetSyncPlatforms( const char *pchFile ) = 0;
		virtual int32 GetFileCount() = 0;
		virtual const char *GetFileNameAndSize( int iFile, int32 *pnFileSizeInBytes ) = 0;
		virtual bool GetQuota( uint64 *pnTotalBytes, uint64 *puAvailableBytes ) = 0;
		virtual bool IsCloudEnabledForAccount() = 0;
		virtual bool IsCloudEnabledForApp() = 0;
		virtual void SetCloudEnabledForApp( bool bEnabled ) = 0;
		CALL_RESULT( RemoteStorageDownloadUGCResult_t )
		virtual SteamAPICall_t UGCDownload( UGCHandle_t hContent, uint32 unPriority ) = 0;
		virtual bool	GetUGCDownloadProgress( UGCHandle_t hContent, int32 *pnBytesDownloaded, int32 *pnBytesExpected ) = 0;
		virtual bool	GetUGCDetails( UGCHandle_t hContent, AppId_t *pnAppID, OUT_STRING() char **ppchName, int32 *pnFileSizeInBytes, OUT_STRUCT() CSteamID *pSteamIDOwner ) = 0;
		virtual int32	UGCRead( UGCHandle_t hContent, void *pvData, int32 cubDataToRead, uint32 cOffset, EUGCReadAction eAction ) = 0;
		virtual int32	GetCachedUGCCount() = 0;
		virtual	UGCHandle_t GetCachedUGCHandle( int32 iCachedContent ) = 0;
#if defined(_PS3) || defined(_SERVER)
		virtual void GetFileListFromServer() = 0;
		virtual bool FileFetch( const char *pchFile ) = 0;
		virtual bool FilePersist( const char *pchFile ) = 0;
		virtual bool SynchronizeToClient() = 0;
		virtual bool SynchronizeToServer() = 0;
		virtual bool ResetFileRequestState() = 0;
#endif
		CALL_RESULT( RemoteStoragePublishFileProgress_t )
		virtual SteamAPICall_t	PublishWorkshopFile( const char *pchFile, const char *pchPreviewFile, AppId_t nConsumerAppId, const char *pchTitle, const char *pchDescription, ERemoteStoragePublishedFileVisibility eVisibility, SteamParamStringArray_t *pTags, EWorkshopFileType eWorkshopFileType ) = 0;
		virtual PublishedFileUpdateHandle_t CreatePublishedFileUpdateRequest( PublishedFileId_t unPublishedFileId ) = 0;
		virtual bool UpdatePublishedFileFile( PublishedFileUpdateHandle_t updateHandle, const char *pchFile ) = 0;
		virtual bool UpdatePublishedFilePreviewFile( PublishedFileUpdateHandle_t updateHandle, const char *pchPreviewFile ) = 0;
		virtual bool UpdatePublishedFileTitle( PublishedFileUpdateHandle_t updateHandle, const char *pchTitle ) = 0;
		virtual bool UpdatePublishedFileDescription( PublishedFileUpdateHandle_t updateHandle, const char *pchDescription ) = 0;
		virtual bool UpdatePublishedFileVisibility( PublishedFileUpdateHandle_t updateHandle, ERemoteStoragePublishedFileVisibility eVisibility ) = 0;
		virtual bool UpdatePublishedFileTags( PublishedFileUpdateHandle_t updateHandle, SteamParamStringArray_t *pTags ) = 0;
		CALL_RESULT( RemoteStorageUpdatePublishedFileResult_t )
		virtual SteamAPICall_t	CommitPublishedFileUpdate( PublishedFileUpdateHandle_t updateHandle ) = 0;
		CALL_RESULT( RemoteStorageGetPublishedFileDetailsResult_t )
		virtual SteamAPICall_t	GetPublishedFileDetails( PublishedFileId_t unPublishedFileId, uint32 unMaxSecondsOld ) = 0;
		CALL_RESULT( RemoteStorageDeletePublishedFileResult_t )
		virtual SteamAPICall_t	DeletePublishedFile( PublishedFileId_t unPublishedFileId ) = 0;
		CALL_RESULT( RemoteStorageEnumerateUserPublishedFilesResult_t )
		virtual SteamAPICall_t	EnumerateUserPublishedFiles( uint32 unStartIndex ) = 0;
		CALL_RESULT( RemoteStorageSubscribePublishedFileResult_t )
		virtual SteamAPICall_t	SubscribePublishedFile( PublishedFileId_t unPublishedFileId ) = 0;
		CALL_RESULT( RemoteStorageEnumerateUserSubscribedFilesResult_t )
		virtual SteamAPICall_t	EnumerateUserSubscribedFiles( uint32 unStartIndex ) = 0;
		CALL_RESULT( RemoteStorageUnsubscribePublishedFileResult_t )
		virtual SteamAPICall_t	UnsubscribePublishedFile( PublishedFileId_t unPublishedFileId ) = 0;
		virtual bool UpdatePublishedFileSetChangeDescription( PublishedFileUpdateHandle_t updateHandle, const char *pchChangeDescription ) = 0;
		CALL_RESULT( RemoteStorageGetPublishedItemVoteDetailsResult_t )
		virtual SteamAPICall_t	GetPublishedItemVoteDetails( PublishedFileId_t unPublishedFileId ) = 0;
		CALL_RESULT( RemoteStorageUpdateUserPublishedItemVoteResult_t )
		virtual SteamAPICall_t	UpdateUserPublishedItemVote( PublishedFileId_t unPublishedFileId, bool bVoteUp ) = 0;
		CALL_RESULT( RemoteStorageGetPublishedItemVoteDetailsResult_t )
		virtual SteamAPICall_t	GetUserPublishedItemVoteDetails( PublishedFileId_t unPublishedFileId ) = 0;
		CALL_RESULT( RemoteStorageEnumerateUserPublishedFilesResult_t )
		virtual SteamAPICall_t	EnumerateUserSharedWorkshopFiles( CSteamID steamId, uint32 unStartIndex, SteamParamStringArray_t *pRequiredTags, SteamParamStringArray_t *pExcludedTags ) = 0;
		CALL_RESULT( RemoteStoragePublishFileProgress_t )
		virtual SteamAPICall_t	PublishVideo( EWorkshopVideoProvider eVideoProvider, const char *pchVideoAccount, const char *pchVideoIdentifier, const char *pchPreviewFile, AppId_t nConsumerAppId, const char *pchTitle, const char *pchDescription, ERemoteStoragePublishedFileVisibility eVisibility, SteamParamStringArray_t *pTags ) = 0;
		CALL_RESULT( RemoteStorageSetUserPublishedFileActionResult_t )
		virtual SteamAPICall_t	SetUserPublishedFileAction( PublishedFileId_t unPublishedFileId, EWorkshopFileAction eAction ) = 0;
		CALL_RESULT( RemoteStorageEnumeratePublishedFilesByUserActionResult_t )
		virtual SteamAPICall_t	EnumeratePublishedFilesByUserAction( EWorkshopFileAction eAction, uint32 unStartIndex ) = 0;
		CALL_RESULT( RemoteStorageEnumerateWorkshopFilesResult_t )
		virtual SteamAPICall_t	EnumeratePublishedWorkshopFiles( EWorkshopEnumerationType eEnumerationType, uint32 unStartIndex, uint32 unCount, uint32 unDays, SteamParamStringArray_t *pTags, SteamParamStringArray_t *pUserTags ) = 0;
		CALL_RESULT( RemoteStorageDownloadUGCResult_t )
		virtual SteamAPICall_t UGCDownloadToLocation( UGCHandle_t hContent, const char *pchLocation, uint32 unPriority ) = 0;
};
#define STEAMREMOTESTORAGE_INTERFACE_VERSION "STEAMREMOTESTORAGE_INTERFACE_VERSION014"
#if defined( VALVE_CALLBACK_PACK_SMALL )
#pragma pack( push, 4 )
#elif defined( VALVE_CALLBACK_PACK_LARGE )
#pragma pack( push, 8 )
#else
#error isteamclient.hpp must be included
#endif 
struct RemoteStorageAppSyncedClient_t
{
	enum { k_iCallback = k_iClientRemoteStorageCallbacks + 1 };
	AppId_t m_nAppID;
	EResult m_eResult;
	int m_unNumDownloads;
};
struct RemoteStorageAppSyncedServer_t
{
	enum { k_iCallback = k_iClientRemoteStorageCallbacks + 2 };
	AppId_t m_nAppID;
	EResult m_eResult;
	int m_unNumUploads;
};
struct RemoteStorageAppSyncProgress_t
{
	enum { k_iCallback = k_iClientRemoteStorageCallbacks + 3 };
	char m_rgchCurrentFile[k_cchFilenameMax];				// Current file being transferred
	AppId_t m_nAppID;							// App this info relates to
	uint32 m_uBytesTransferredThisChunk;		// Bytes transferred this chunk
	double m_dAppPercentComplete;				// Percent complete that this app's transfers are
	bool m_bUploading;							// if false, downloading
};
struct RemoteStorageAppSyncStatusCheck_t
{
	enum { k_iCallback = k_iClientRemoteStorageCallbacks + 5 };
	AppId_t m_nAppID;
	EResult m_eResult;
};
struct RemoteStorageFileShareResult_t
{
	enum { k_iCallback = k_iClientRemoteStorageCallbacks + 7 };
	EResult m_eResult;			// The result of the operation
	UGCHandle_t m_hFile;		// The handle that can be shared with users and features
	char m_rgchFilename[k_cchFilenameMax]; // The name of the file that was shared
};
struct RemoteStoragePublishFileResult_t
{
	enum { k_iCallback = k_iClientRemoteStorageCallbacks + 9 };
	EResult m_eResult;				// The result of the operation.
	PublishedFileId_t m_nPublishedFileId;
	bool m_bUserNeedsToAcceptWorkshopLegalAgreement;
};
struct RemoteStorageDeletePublishedFileResult_t
{
	enum { k_iCallback = k_iClientRemoteStorageCallbacks + 11 };
	EResult m_eResult;				// The result of the operation.
	PublishedFileId_t m_nPublishedFileId;
};
struct RemoteStorageEnumerateUserPublishedFilesResult_t
{
	enum { k_iCallback = k_iClientRemoteStorageCallbacks + 12 };
	EResult m_eResult;				// The result of the operation.
	int32 m_nResultsReturned;
	int32 m_nTotalResultCount;
	PublishedFileId_t m_rgPublishedFileId[ k_unEnumeratePublishedFilesMaxResults ];
};
struct RemoteStorageSubscribePublishedFileResult_t
{
	enum { k_iCallback = k_iClientRemoteStorageCallbacks + 13 };
	EResult m_eResult;				// The result of the operation.
	PublishedFileId_t m_nPublishedFileId;
};
struct RemoteStorageEnumerateUserSubscribedFilesResult_t
{
	enum { k_iCallback = k_iClientRemoteStorageCallbacks + 14 };
	EResult m_eResult;				// The result of the operation.
	int32 m_nResultsReturned;
	int32 m_nTotalResultCount;
	PublishedFileId_t m_rgPublishedFileId[ k_unEnumeratePublishedFilesMaxResults ];
	uint32 m_rgRTimeSubscribed[ k_unEnumeratePublishedFilesMaxResults ];
};
#if defined(VALVE_CALLBACK_PACK_SMALL)
	VALVE_COMPILE_TIME_ASSERT( sizeof( RemoteStorageEnumerateUserSubscribedFilesResult_t ) == (1 + 1 + 1 + 50 + 100) * 4 );
#elif defined(VALVE_CALLBACK_PACK_LARGE)
	VALVE_COMPILE_TIME_ASSERT( sizeof( RemoteStorageEnumerateUserSubscribedFilesResult_t ) == (1 + 1 + 1 + 50 + 100) * 4 + 4 );
#else
#warning You must first include isteamclient.hpp
#endif
struct RemoteStorageUnsubscribePublishedFileResult_t
{
	enum { k_iCallback = k_iClientRemoteStorageCallbacks + 15 };
	EResult m_eResult;				// The result of the operation.
	PublishedFileId_t m_nPublishedFileId;
};
struct RemoteStorageUpdatePublishedFileResult_t
{
	enum { k_iCallback = k_iClientRemoteStorageCallbacks + 16 };
	EResult m_eResult;				// The result of the operation.
	PublishedFileId_t m_nPublishedFileId;
	bool m_bUserNeedsToAcceptWorkshopLegalAgreement;
};
struct RemoteStorageDownloadUGCResult_t
{
	enum { k_iCallback = k_iClientRemoteStorageCallbacks + 17 };
	EResult m_eResult;				// The result of the operation.
	UGCHandle_t m_hFile;			// The handle to the file that was attempted to be downloaded.
	AppId_t m_nAppID;				// ID of the app that created this file.
	int32 m_nSizeInBytes;			// The size of the file that was downloaded, in bytes.
	char m_pchFileName[k_cchFilenameMax];		// The name of the file that was downloaded. 
	uint64 m_ulSteamIDOwner;		// Steam ID of the user who created this content.
};
struct RemoteStorageGetPublishedFileDetailsResult_t
{
	enum { k_iCallback = k_iClientRemoteStorageCallbacks + 18 };
	EResult m_eResult;				// The result of the operation.
	PublishedFileId_t m_nPublishedFileId;
	AppId_t m_nCreatorAppID;		// ID of the app that created this file.
	AppId_t m_nConsumerAppID;		// ID of the app that will consume this file.
	char m_rgchTitle[k_cchPublishedDocumentTitleMax];		// title of document
	char m_rgchDescription[k_cchPublishedDocumentDescriptionMax];	// description of document
	UGCHandle_t m_hFile;			// The handle of the primary file
	UGCHandle_t m_hPreviewFile;		// The handle of the preview file
	uint64 m_ulSteamIDOwner;		// Steam ID of the user who created this content.
	uint32 m_rtimeCreated;			// time when the published file was created
	uint32 m_rtimeUpdated;			// time when the published file was last updated
	ERemoteStoragePublishedFileVisibility m_eVisibility;
	bool m_bBanned;
	char m_rgchTags[k_cchTagListMax];	// comma separated list of all tags associated with this file
	bool m_bTagsTruncated;			// whether the list of tags was too long to be returned in the provided buffer
	char m_pchFileName[k_cchFilenameMax];		// The name of the primary file
	int32 m_nFileSize;				// Size of the primary file
	int32 m_nPreviewFileSize;		// Size of the preview file
	char m_rgchURL[k_cchPublishedFileURLMax];	// URL (for a video or a website)
	EWorkshopFileType m_eFileType;	// Type of the file
	bool m_bAcceptedForUse;			// developer has specifically flagged this item as accepted in the Workshop
};
struct RemoteStorageEnumerateWorkshopFilesResult_t
{
	enum { k_iCallback = k_iClientRemoteStorageCallbacks + 19 };
	EResult m_eResult;
	int32 m_nResultsReturned;
	int32 m_nTotalResultCount;
	PublishedFileId_t m_rgPublishedFileId[ k_unEnumeratePublishedFilesMaxResults ];
	float m_rgScore[ k_unEnumeratePublishedFilesMaxResults ];
	AppId_t m_nAppId;
	uint32 m_unStartIndex;
};
struct RemoteStorageGetPublishedItemVoteDetailsResult_t
{
	enum { k_iCallback = k_iClientRemoteStorageCallbacks + 20 };
	EResult m_eResult;
	PublishedFileId_t m_unPublishedFileId;
	int32 m_nVotesFor;
	int32 m_nVotesAgainst;
	int32 m_nReports;
	float m_fScore;
};
struct RemoteStoragePublishedFileSubscribed_t
{
	enum { k_iCallback = k_iClientRemoteStorageCallbacks + 21 };
	PublishedFileId_t m_nPublishedFileId;	// The published file id
	AppId_t m_nAppID;						// ID of the app that will consume this file.
};
struct RemoteStoragePublishedFileUnsubscribed_t
{
	enum { k_iCallback = k_iClientRemoteStorageCallbacks + 22 };
	PublishedFileId_t m_nPublishedFileId;	// The published file id
	AppId_t m_nAppID;						// ID of the app that will consume this file.
};
struct RemoteStoragePublishedFileDeleted_t
{
	enum { k_iCallback = k_iClientRemoteStorageCallbacks + 23 };
	PublishedFileId_t m_nPublishedFileId;	// The published file id
	AppId_t m_nAppID;						// ID of the app that will consume this file.
};
struct RemoteStorageUpdateUserPublishedItemVoteResult_t
{
	enum { k_iCallback = k_iClientRemoteStorageCallbacks + 24 };
	EResult m_eResult;				// The result of the operation.
	PublishedFileId_t m_nPublishedFileId;	// The published file id
};
struct RemoteStorageUserVoteDetails_t
{
	enum { k_iCallback = k_iClientRemoteStorageCallbacks + 25 };
	EResult m_eResult;				// The result of the operation.
	PublishedFileId_t m_nPublishedFileId;	// The published file id
	EWorkshopVote m_eVote;			// what the user voted
};
struct RemoteStorageEnumerateUserSharedWorkshopFilesResult_t
{
	enum { k_iCallback = k_iClientRemoteStorageCallbacks + 26 };
	EResult m_eResult;				// The result of the operation.
	int32 m_nResultsReturned;
	int32 m_nTotalResultCount;
	PublishedFileId_t m_rgPublishedFileId[ k_unEnumeratePublishedFilesMaxResults ];
};
struct RemoteStorageSetUserPublishedFileActionResult_t
{
	enum { k_iCallback = k_iClientRemoteStorageCallbacks + 27 };
	EResult m_eResult;				// The result of the operation.
	PublishedFileId_t m_nPublishedFileId;	// The published file id
	EWorkshopFileAction m_eAction;	// the action that was attempted
};
struct RemoteStorageEnumeratePublishedFilesByUserActionResult_t
{
	enum { k_iCallback = k_iClientRemoteStorageCallbacks + 28 };
	EResult m_eResult;				// The result of the operation.
	EWorkshopFileAction m_eAction;	// the action that was filtered on
	int32 m_nResultsReturned;
	int32 m_nTotalResultCount;
	PublishedFileId_t m_rgPublishedFileId[ k_unEnumeratePublishedFilesMaxResults ];
	uint32 m_rgRTimeUpdated[ k_unEnumeratePublishedFilesMaxResults ];
};
struct RemoteStoragePublishFileProgress_t
{
	enum { k_iCallback = k_iClientRemoteStorageCallbacks + 29 };
	double m_dPercentFile;
	bool m_bPreview;
};
struct RemoteStoragePublishedFileUpdated_t
{
	enum { k_iCallback = k_iClientRemoteStorageCallbacks + 30 };
	PublishedFileId_t m_nPublishedFileId;	// The published file id
	AppId_t m_nAppID;						// ID of the app that will consume this file.
	uint64 m_ulUnused;						// not used anymore
};
struct RemoteStorageFileWriteAsyncComplete_t
{
	enum { k_iCallback = k_iClientRemoteStorageCallbacks + 31 };
	EResult	m_eResult;						// result
};
struct RemoteStorageFileReadAsyncComplete_t
{
	enum { k_iCallback = k_iClientRemoteStorageCallbacks + 32 };
	SteamAPICall_t m_hFileReadAsync;		// call handle of the async read which was made
	EResult	m_eResult;						// result
	uint32 m_nOffset;						// offset in the file this read was at
	uint32 m_cubRead;						// amount read - will the <= the amount requested
};
#pragma pack( pop )
#endif // ISTEAMREMOTESTORAGE_H
