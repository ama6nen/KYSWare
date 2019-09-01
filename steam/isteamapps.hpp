#ifndef ISTEAMAPPS_H
#define ISTEAMAPPS_H
#ifdef _WIN32
#pragma once
#endif
const int k_cubAppProofOfPurchaseKeyMax = 240;			// max supported length of a legacy cd key 
class ISteamApps
{
public:
	virtual bool BIsSubscribed() = 0;
	virtual bool BIsLowViolence() = 0;
	virtual bool BIsCybercafe() = 0;
	virtual bool BIsVACBanned() = 0;
	virtual const char *GetCurrentGameLanguage() = 0;
	virtual const char *GetAvailableGameLanguages() = 0;
	virtual bool BIsSubscribedApp( AppId_t appID ) = 0;
	virtual bool BIsDlcInstalled( AppId_t appID ) = 0;
	virtual uint32 GetEarliestPurchaseUnixTime( AppId_t nAppID ) = 0;
	virtual bool BIsSubscribedFromFreeWeekend() = 0;
	virtual int GetDLCCount() = 0;
	virtual bool BGetDLCDataByIndex( int iDLC, AppId_t *pAppID, bool *pbAvailable, char *pchName, int cchNameBufferSize ) = 0;
	virtual void InstallDLC( AppId_t nAppID ) = 0;
	virtual void UninstallDLC( AppId_t nAppID ) = 0;
	virtual void RequestAppProofOfPurchaseKey( AppId_t nAppID ) = 0;
	virtual bool GetCurrentBetaName( char *pchName, int cchNameBufferSize ) = 0; // returns current beta branch name, 'public' is the default branch
	virtual bool MarkContentCorrupt( bool bMissingFilesOnly ) = 0; // signal Steam that game files seems corrupt or missing
	virtual uint32 GetInstalledDepots( AppId_t appID, DepotId_t *pvecDepots, uint32 cMaxDepots ) = 0; // return installed depots in mount order
	virtual uint32 GetAppInstallDir( AppId_t appID, char *pchFolder, uint32 cchFolderBufferSize ) = 0;
	virtual bool BIsAppInstalled( AppId_t appID ) = 0; // returns true if that app is installed (not necessarily owned)
	virtual CSteamID GetAppOwner() = 0; // returns the SteamID of the original owner. If different from current user, it's borrowed
	virtual const char *GetLaunchQueryParam( const char *pchKey ) = 0;
	virtual bool GetDlcDownloadProgress( AppId_t nAppID, uint64 *punBytesDownloaded, uint64 *punBytesTotal ) = 0; 
	virtual int GetAppBuildId() = 0;
	virtual void RequestAllProofOfPurchaseKeys() = 0;
	CALL_RESULT( FileDetailsResult_t )
	virtual SteamAPICall_t GetFileDetails( const char* pszFileName ) = 0;
};
#define STEAMAPPS_INTERFACE_VERSION "STEAMAPPS_INTERFACE_VERSION008"
#if defined( VALVE_CALLBACK_PACK_SMALL )
#pragma pack( push, 4 )
#elif defined( VALVE_CALLBACK_PACK_LARGE )
#pragma pack( push, 8 )
#else
#error isteamclient.hpp must be included
#endif 
struct DlcInstalled_t
{
	enum { k_iCallback = k_iSteamAppsCallbacks + 5 };
	AppId_t m_nAppID;		// AppID of the DLC
};
enum ERegisterActivationCodeResult
{
	k_ERegisterActivationCodeResultOK = 0,
	k_ERegisterActivationCodeResultFail = 1,
	k_ERegisterActivationCodeResultAlreadyRegistered = 2,
	k_ERegisterActivationCodeResultTimeout = 3,
	k_ERegisterActivationCodeAlreadyOwned = 4,
};
struct RegisterActivationCodeResponse_t
{
	enum { k_iCallback = k_iSteamAppsCallbacks + 8 };
	ERegisterActivationCodeResult m_eResult;
	uint32 m_unPackageRegistered;						// package that was registered. Only set on success
};
struct NewLaunchQueryParameters_t
{
	enum { k_iCallback = k_iSteamAppsCallbacks + 14 };
};
struct AppProofOfPurchaseKeyResponse_t
{
	enum { k_iCallback = k_iSteamAppsCallbacks + 21 };
	EResult m_eResult;
	uint32	m_nAppID;
	uint32	m_cchKeyLength;
	char	m_rgchKey[k_cubAppProofOfPurchaseKeyMax];
};
struct FileDetailsResult_t
{
	enum { k_iCallback = k_iSteamAppsCallbacks + 23 };
	EResult		m_eResult;
	uint64		m_ulFileSize;	// original file size in bytes
	uint8		m_FileSHA[20];	// original file SHA1 hash
	uint32		m_unFlags;		// 
};
#pragma pack( pop )
#endif // ISTEAMAPPS_H
