#ifndef ISTEAMVIDEO_H
#define ISTEAMVIDEO_H
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
class ISteamVideo
{
public:
	virtual void GetVideoURL( AppId_t unVideoAppID ) = 0;
	virtual bool IsBroadcasting( int *pnNumViewers ) = 0;
	CALL_BACK( GetOPFSettingsResult_t )
	virtual void GetOPFSettings( AppId_t unVideoAppID ) = 0;
	virtual bool GetOPFStringForApp( AppId_t unVideoAppID, char *pchBuffer, int32 *pnBufferSize ) = 0;
};
#define STEAMVIDEO_INTERFACE_VERSION "STEAMVIDEO_INTERFACE_V002"
DEFINE_CALLBACK( BroadcastUploadStart_t, k_iClientVideoCallbacks + 4 )
END_DEFINE_CALLBACK_0()
DEFINE_CALLBACK( BroadcastUploadStop_t, k_iClientVideoCallbacks + 5 )
	CALLBACK_MEMBER( 0, EBroadcastUploadResult, m_eResult )
END_DEFINE_CALLBACK_1()
DEFINE_CALLBACK( GetVideoURLResult_t, k_iClientVideoCallbacks + 11 )
	CALLBACK_MEMBER( 0, EResult, m_eResult )
	CALLBACK_MEMBER( 1, AppId_t, m_unVideoAppID )
	CALLBACK_MEMBER( 2, char, m_rgchURL[256] )
END_DEFINE_CALLBACK_3()
DEFINE_CALLBACK( GetOPFSettingsResult_t, k_iClientVideoCallbacks + 24 )
	CALLBACK_MEMBER( 0, EResult, m_eResult )
	CALLBACK_MEMBER( 1, AppId_t, m_unVideoAppID )
END_DEFINE_CALLBACK_2()
#pragma pack( pop )
#endif // ISTEAMVIDEO_H
