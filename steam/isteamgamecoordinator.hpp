#ifndef ISTEAMGAMECOORDINATOR
#define ISTEAMGAMECOORDINATOR
#ifdef _WIN32
#pragma once
#endif
#include "steamtypes.hpp"
#include "steamclientpublic.hpp"
enum EGCResults
{
	k_EGCResultOK = 0,
	k_EGCResultNoMessage = 1,			// There is no message in the queue
	k_EGCResultBufferTooSmall = 2,		// The buffer is too small for the requested message
	k_EGCResultNotLoggedOn = 3,			// The client is not logged onto Steam
	k_EGCResultInvalidMessage = 4,		// Something was wrong with the message being sent with SendMessage
};
class ISteamGameCoordinator
{
public:
	virtual EGCResults SendMessage( uint32 unMsgType, const void *pubData, uint32 cubData ) = 0;
	virtual bool IsMessageAvailable( uint32 *pcubMsgSize ) = 0; 
	virtual EGCResults RetrieveMessage( uint32 *punMsgType, void *pubDest, uint32 cubDest, uint32 *pcubMsgSize ) = 0; 
};
#define STEAMGAMECOORDINATOR_INTERFACE_VERSION "SteamGameCoordinator001"
#if defined( VALVE_CALLBACK_PACK_SMALL )
#pragma pack( push, 4 )
#elif defined( VALVE_CALLBACK_PACK_LARGE )
#pragma pack( push, 8 )
#else
#error isteamclient.hpp must be included
#endif 
struct GCMessageAvailable_t
{
	enum { k_iCallback = k_iSteamGameCoordinatorCallbacks + 1 };
	uint32 m_nMessageSize;
};
struct GCMessageFailed_t
{
	enum { k_iCallback = k_iSteamGameCoordinatorCallbacks + 2 };
};
#pragma pack( pop )
#endif // ISTEAMGAMECOORDINATOR
