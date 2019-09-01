#ifndef ISTEAMSCREENSHOTS_H
#define ISTEAMSCREENSHOTS_H
#ifdef _WIN32
#pragma once
#endif
#include "isteamclient.hpp"
const uint32 k_nScreenshotMaxTaggedUsers = 32;
const uint32 k_nScreenshotMaxTaggedPublishedFiles = 32;
const int k_cubUFSTagTypeMax = 255;
const int k_cubUFSTagValueMax = 255;
const int k_ScreenshotThumbWidth = 200;
typedef uint32 ScreenshotHandle; 
#define INVALID_SCREENSHOT_HANDLE 0
enum EVRScreenshotType
{
	k_EVRScreenshotType_None			= 0,
	k_EVRScreenshotType_Mono			= 1,
	k_EVRScreenshotType_Stereo			= 2,
	k_EVRScreenshotType_MonoCubemap		= 3,
	k_EVRScreenshotType_MonoPanorama	= 4,
	k_EVRScreenshotType_StereoPanorama	= 5
};
class ISteamScreenshots
{
public:
	virtual ScreenshotHandle WriteScreenshot( void *pubRGB, uint32 cubRGB, int nWidth, int nHeight ) = 0;
	virtual ScreenshotHandle AddScreenshotToLibrary( const char *pchFilename, const char *pchThumbnailFilename, int nWidth, int nHeight ) = 0;
	virtual void TriggerScreenshot() = 0;
	virtual void HookScreenshots( bool bHook ) = 0;
	virtual bool SetLocation( ScreenshotHandle hScreenshot, const char *pchLocation ) = 0;
	virtual bool TagUser( ScreenshotHandle hScreenshot, CSteamID steamID ) = 0;
	virtual bool TagPublishedFile( ScreenshotHandle hScreenshot, PublishedFileId_t unPublishedFileID ) = 0;
	virtual bool IsScreenshotsHooked() = 0;
	virtual ScreenshotHandle AddVRScreenshotToLibrary( EVRScreenshotType eType, const char *pchFilename, const char *pchVRFilename ) = 0;
};
#define STEAMSCREENSHOTS_INTERFACE_VERSION "STEAMSCREENSHOTS_INTERFACE_VERSION003"
#if defined( VALVE_CALLBACK_PACK_SMALL )
#pragma pack( push, 4 )
#elif defined( VALVE_CALLBACK_PACK_LARGE )
#pragma pack( push, 8 )
#else
#error isteamclient.hpp must be included
#endif 
struct ScreenshotReady_t
{
	enum { k_iCallback = k_iSteamScreenshotsCallbacks + 1 };
	ScreenshotHandle m_hLocal;
	EResult m_eResult;
};
struct ScreenshotRequested_t
{
	enum { k_iCallback = k_iSteamScreenshotsCallbacks + 2 };
};
#pragma pack( pop )
#endif // ISTEAMSCREENSHOTS_H

