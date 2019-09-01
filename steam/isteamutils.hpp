#ifndef ISTEAMUTILS_H
#define ISTEAMUTILS_H
#ifdef _WIN32
#pragma once
#endif
#include "isteamclient.hpp"
enum ESteamAPICallFailure
{
	k_ESteamAPICallFailureNone = -1,			// no failure
	k_ESteamAPICallFailureSteamGone = 0,		// the local Steam process has gone away
	k_ESteamAPICallFailureNetworkFailure = 1,	// the network connection to Steam has been broken, or was already broken
	k_ESteamAPICallFailureInvalidHandle = 2,	// the SteamAPICall_t handle passed in no longer exists
	k_ESteamAPICallFailureMismatchedCallback = 3,// GetAPICallResult() was called with the wrong callback type for this API call
};
enum EGamepadTextInputMode
{
	k_EGamepadTextInputModeNormal = 0,
	k_EGamepadTextInputModePassword = 1
};
enum EGamepadTextInputLineMode
{
	k_EGamepadTextInputLineModeSingleLine = 0,
	k_EGamepadTextInputLineModeMultipleLines = 1
};
#if defined( POSIX )
#define __cdecl
#endif
extern "C" typedef void (__cdecl *SteamAPIWarningMessageHook_t)(int, const char *);
class ISteamUtils
{
public:
	virtual uint32 GetSecondsSinceAppActive() = 0;
	virtual uint32 GetSecondsSinceComputerActive() = 0;
	virtual EUniverse GetConnectedUniverse() = 0;
	virtual uint32 GetServerRealTime() = 0;
	virtual const char *GetIPCountry() = 0;
	virtual bool GetImageSize( int iImage, uint32 *pnWidth, uint32 *pnHeight ) = 0;
	virtual bool GetImageRGBA( int iImage, uint8 *pubDest, int nDestBufferSize ) = 0;
	virtual bool GetCSERIPPort( uint32 *unIP, uint16 *usPort ) = 0;
	virtual uint8 GetCurrentBatteryPower() = 0;
	virtual uint32 GetAppID() = 0;
	virtual void SetOverlayNotificationPosition( ENotificationPosition eNotificationPosition ) = 0;
	virtual bool IsAPICallCompleted( SteamAPICall_t hSteamAPICall, bool *pbFailed ) = 0;
	virtual ESteamAPICallFailure GetAPICallFailureReason( SteamAPICall_t hSteamAPICall ) = 0;
	virtual bool GetAPICallResult( SteamAPICall_t hSteamAPICall, void *pCallback, int cubCallback, int iCallbackExpected, bool *pbFailed ) = 0;
	STEAM_PRIVATE_API( virtual void RunFrame() = 0; )
	virtual uint32 GetIPCCallCount() = 0;
	virtual void SetWarningMessageHook( SteamAPIWarningMessageHook_t pFunction ) = 0;
	virtual bool IsOverlayEnabled() = 0;
	virtual bool BOverlayNeedsPresent() = 0;
	CALL_RESULT( CheckFileSignature_t )
	virtual SteamAPICall_t CheckFileSignature( const char *szFileName ) = 0;
	virtual bool ShowGamepadTextInput( EGamepadTextInputMode eInputMode, EGamepadTextInputLineMode eLineInputMode, const char *pchDescription, uint32 unCharMax, const char *pchExistingText ) = 0;
	virtual uint32 GetEnteredGamepadTextLength() = 0;
	virtual bool GetEnteredGamepadTextInput( char *pchText, uint32 cchText ) = 0;
	virtual const char *GetSteamUILanguage() = 0;
	virtual bool IsSteamRunningInVR() = 0;
	virtual void SetOverlayNotificationInset( int nHorizontalInset, int nVerticalInset ) = 0;
	virtual bool IsSteamInBigPictureMode() = 0;
	virtual void StartVRDashboard() = 0;
	virtual bool IsVRHeadsetStreamingEnabled() = 0;
	virtual void SetVRHeadsetStreamingEnabled( bool bEnabled ) = 0;
};
#define STEAMUTILS_INTERFACE_VERSION "SteamUtils009"
#if defined( VALVE_CALLBACK_PACK_SMALL )
#pragma pack( push, 4 )
#elif defined( VALVE_CALLBACK_PACK_LARGE )
#pragma pack( push, 8 )
#else
#error isteamclient.hpp must be included
#endif 
struct IPCountry_t
{
	enum { k_iCallback = k_iSteamUtilsCallbacks + 1 };
};
struct LowBatteryPower_t
{
	enum { k_iCallback = k_iSteamUtilsCallbacks + 2 };
	uint8 m_nMinutesBatteryLeft;
};
struct SteamAPICallCompleted_t
{
	enum { k_iCallback = k_iSteamUtilsCallbacks + 3 };
	SteamAPICall_t m_hAsyncCall;
	int m_iCallback;
	uint32 m_cubParam;
};
struct SteamShutdown_t
{
	enum { k_iCallback = k_iSteamUtilsCallbacks + 4 };
};
enum ECheckFileSignature
{
	k_ECheckFileSignatureInvalidSignature = 0,
	k_ECheckFileSignatureValidSignature = 1,
	k_ECheckFileSignatureFileNotFound = 2,
	k_ECheckFileSignatureNoSignaturesFoundForThisApp = 3,
	k_ECheckFileSignatureNoSignaturesFoundForThisFile = 4,
};
struct CheckFileSignature_t
{
	enum { k_iCallback = k_iSteamUtilsCallbacks + 5 };
	ECheckFileSignature m_eCheckFileSignature;
};
struct GamepadTextInputDismissed_t
{
	enum { k_iCallback = k_iSteamUtilsCallbacks + 14 };
	bool m_bSubmitted;										// true if user entered & accepted text (Call ISteamUtils::GetEnteredGamepadTextInput() for text), false if canceled input
	uint32 m_unSubmittedText;
};
#pragma pack( pop )
#endif // ISTEAMUTILS_H
