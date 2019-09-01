#ifndef ISTEAMUSER_H
#define ISTEAMUSER_H
#ifdef _WIN32
#pragma once
#endif
#include "isteamclient.hpp"
#if defined( VALVE_CALLBACK_PACK_SMALL )
#pragma pack( push, 4 )
#elif defined( VALVE_CALLBACK_PACK_LARGE )
#pragma pack( push, 8 )
#else
#error isteamclient.hpp must be included
#endif 
struct CallbackMsg_t
{
	HSteamUser m_hSteamUser;
	int m_iCallback;
	uint8 *m_pubParam;
	int m_cubParam;
};
#pragma pack( pop )
class ISteamUser
{
public:
	virtual HSteamUser GetHSteamUser() = 0;
	virtual bool BLoggedOn() = 0;
	virtual CSteamID GetSteamID() = 0;
	virtual int InitiateGameConnection( void *pAuthBlob, int cbMaxAuthBlob, CSteamID steamIDGameServer, uint32 unIPServer, uint16 usPortServer, bool bSecure ) = 0;
	virtual void TerminateGameConnection( uint32 unIPServer, uint16 usPortServer ) = 0;
	virtual void TrackAppUsageEvent( CGameID gameID, int eAppUsageEvent, const char *pchExtraInfo = "" ) = 0;
	virtual bool GetUserDataFolder( char *pchBuffer, int cubBuffer ) = 0;
	virtual void StartVoiceRecording( ) = 0;
	virtual void StopVoiceRecording( ) = 0;
	virtual EVoiceResult GetAvailableVoice( uint32 *pcbCompressed, uint32 *pcbUncompressed_Deprecated = 0, uint32 nUncompressedVoiceDesiredSampleRate_Deprecated = 0 ) = 0;
	virtual EVoiceResult GetVoice( bool bWantCompressed, void *pDestBuffer, uint32 cbDestBufferSize, uint32 *nBytesWritten, bool bWantUncompressed_Deprecated = false, void *pUncompressedDestBuffer_Deprecated = 0, uint32 cbUncompressedDestBufferSize_Deprecated = 0, uint32 *nUncompressBytesWritten_Deprecated = 0, uint32 nUncompressedVoiceDesiredSampleRate_Deprecated = 0 ) = 0;
	virtual EVoiceResult DecompressVoice( const void *pCompressed, uint32 cbCompressed, void *pDestBuffer, uint32 cbDestBufferSize, uint32 *nBytesWritten, uint32 nDesiredSampleRate ) = 0;
	virtual uint32 GetVoiceOptimalSampleRate() = 0;
	virtual HAuthTicket GetAuthSessionTicket( void *pTicket, int cbMaxTicket, uint32 *pcbTicket ) = 0;
	virtual EBeginAuthSessionResult BeginAuthSession( const void *pAuthTicket, int cbAuthTicket, CSteamID steamID ) = 0;
	virtual void EndAuthSession( CSteamID steamID ) = 0;
	virtual void CancelAuthTicket( HAuthTicket hAuthTicket ) = 0;
	virtual EUserHasLicenseForAppResult UserHasLicenseForApp( CSteamID steamID, AppId_t appID ) = 0;
	virtual bool BIsBehindNAT() = 0;
	virtual void AdvertiseGame( CSteamID steamIDGameServer, uint32 unIPServer, uint16 usPortServer ) = 0;
	CALL_RESULT( EncryptedAppTicketResponse_t )
	virtual SteamAPICall_t RequestEncryptedAppTicket( void *pDataToInclude, int cbDataToInclude ) = 0;
	virtual bool GetEncryptedAppTicket( void *pTicket, int cbMaxTicket, uint32 *pcbTicket ) = 0;
	virtual int GetGameBadgeLevel( int nSeries, bool bFoil ) = 0;
	virtual int GetPlayerSteamLevel() = 0;
	CALL_RESULT( StoreAuthURLResponse_t )
	virtual SteamAPICall_t RequestStoreAuthURL( const char *pchRedirectURL ) = 0;
	virtual bool BIsPhoneVerified() = 0;
	virtual bool BIsTwoFactorEnabled() = 0;
	virtual bool BIsPhoneIdentifying() = 0;
	virtual bool BIsPhoneRequiringVerification() = 0;
};
#define STEAMUSER_INTERFACE_VERSION "SteamUser019"
#if defined( VALVE_CALLBACK_PACK_SMALL )
#pragma pack( push, 4 )
#elif defined( VALVE_CALLBACK_PACK_LARGE )
#pragma pack( push, 8 )
#else
#error isteamclient.hpp must be included
#endif 
struct SteamServersConnected_t
{
	enum { k_iCallback = k_iSteamUserCallbacks + 1 };
};
struct SteamServerConnectFailure_t
{
	enum { k_iCallback = k_iSteamUserCallbacks + 2 };
	EResult m_eResult;
	bool m_bStillRetrying;
};
struct SteamServersDisconnected_t
{
	enum { k_iCallback = k_iSteamUserCallbacks + 3 };
	EResult m_eResult;
};
struct ClientGameServerDeny_t
{
	enum { k_iCallback = k_iSteamUserCallbacks + 13 };
	uint32 m_uAppID;
	uint32 m_unGameServerIP;
	uint16 m_usGameServerPort;
	uint16 m_bSecure;
	uint32 m_uReason;
};
struct IPCFailure_t
{
	enum { k_iCallback = k_iSteamUserCallbacks + 17 };
	enum EFailureType 
	{ 
		k_EFailureFlushedCallbackQueue, 
		k_EFailurePipeFail,
	};
	uint8 m_eFailureType;
};
struct LicensesUpdated_t
{
	enum { k_iCallback = k_iSteamUserCallbacks + 25 };
};
struct ValidateAuthTicketResponse_t
{
	enum { k_iCallback = k_iSteamUserCallbacks + 43 };
	CSteamID m_ID;
	EAuthSessionResponse m_eAuthSessionResponse;
	CSteamID m_OwnerSteamID; // different from m_ID if borrowed
};
struct MicroTxnAuthorizationResponse_t
{
	enum { k_iCallback = k_iSteamUserCallbacks + 52 };
	uint32 m_unAppID;			// AppID for this microtransaction
	uint64 m_ulOrderID;			// OrderID provided for the microtransaction
	uint8 m_bAuthorized;		// if user authorized transaction
};
struct EncryptedAppTicketResponse_t
{
	enum { k_iCallback = k_iSteamUserCallbacks + 54 };
	EResult m_eResult;
};
struct GetAuthSessionTicketResponse_t
{
	enum { k_iCallback = k_iSteamUserCallbacks + 63 };
	HAuthTicket m_hAuthTicket;
	EResult m_eResult;
};
struct GameWebCallback_t
{
	enum { k_iCallback = k_iSteamUserCallbacks + 64 };
	char m_szURL[256];
};
struct StoreAuthURLResponse_t
{
	enum { k_iCallback = k_iSteamUserCallbacks + 65 };
	char m_szURL[512];
};
#pragma pack( pop )
#endif // ISTEAMUSER_H
