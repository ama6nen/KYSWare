#ifndef ISTEAMNETWORKING
#define ISTEAMNETWORKING
#ifdef _WIN32
#pragma once
#endif
#include "steamtypes.hpp"
#include "steamclientpublic.hpp"
enum EP2PSessionError
{
	k_EP2PSessionErrorNone = 0,
	k_EP2PSessionErrorNotRunningApp = 1,			// target is not running the same game
	k_EP2PSessionErrorNoRightsToApp = 2,			// local user doesn't own the app that is running
	k_EP2PSessionErrorDestinationNotLoggedIn = 3,	// target user isn't connected to Steam
	k_EP2PSessionErrorTimeout = 4,					// target isn't responding, perhaps not calling AcceptP2PSessionWithUser()
	k_EP2PSessionErrorMax = 5
};
enum EP2PSend
{
	k_EP2PSendUnreliable = 0,
	k_EP2PSendUnreliableNoDelay = 1,
	k_EP2PSendReliable = 2,
	k_EP2PSendReliableWithBuffering = 3,
};
#if defined( VALVE_CALLBACK_PACK_SMALL )
#pragma pack( push, 4 )
#elif defined( VALVE_CALLBACK_PACK_LARGE )
#pragma pack( push, 8 )
#else
#error isteamclient.hpp must be included
#endif 
struct P2PSessionState_t
{
	uint8 m_bConnectionActive;		// true if we've got an active open connection
	uint8 m_bConnecting;			// true if we're currently trying to establish a connection
	uint8 m_eP2PSessionError;		// last error recorded (see enum above)
	uint8 m_bUsingRelay;			// true if it's going through a relay server (TURN)
	int32 m_nBytesQueuedForSend;
	int32 m_nPacketsQueuedForSend;
	uint32 m_nRemoteIP;				// potential IP:Port of remote host. Could be TURN server. 
	uint16 m_nRemotePort;			// Only exists for compatibility with older authentication api's
};
#pragma pack( pop )
typedef uint32 SNetSocket_t;		// CreateP2PConnectionSocket()
typedef uint32 SNetListenSocket_t;	// CreateListenSocket()
enum ESNetSocketState
{
	k_ESNetSocketStateInvalid = 0,						
	k_ESNetSocketStateConnected = 1,				
	k_ESNetSocketStateInitiated = 10,				// the connection state machine has started
	k_ESNetSocketStateLocalCandidatesFound = 11,	// we've found our local IP info
	k_ESNetSocketStateReceivedRemoteCandidates = 12,// we've received information from the remote machine, via the Steam back-end, about their IP info
	k_ESNetSocketStateChallengeHandshake = 15,		// we've received a challenge packet from the server
	k_ESNetSocketStateDisconnecting = 21,			// the API shut it down, and we're in the process of telling the other end	
	k_ESNetSocketStateLocalDisconnect = 22,			// the API shut it down, and we've completed shutdown
	k_ESNetSocketStateTimeoutDuringConnect = 23,	// we timed out while trying to creating the connection
	k_ESNetSocketStateRemoteEndDisconnected = 24,	// the remote end has disconnected from us
	k_ESNetSocketStateConnectionBroken = 25,		// connection has been broken; either the other end has disappeared or our local network connection has broke
};
enum ESNetSocketConnectionType
{
	k_ESNetSocketConnectionTypeNotConnected = 0,
	k_ESNetSocketConnectionTypeUDP = 1,
	k_ESNetSocketConnectionTypeUDPRelay = 2,
};
class ISteamNetworking
{
public:
	////////////////////////////////////////////////////////////////////////////////////////////
	virtual bool SendP2PPacket( CSteamID steamIDRemote, const void *pubData, uint32 cubData, EP2PSend eP2PSendType, int nChannel = 0 ) = 0;
	virtual bool IsP2PPacketAvailable( uint32 *pcubMsgSize, int nChannel = 0 ) = 0;
	virtual bool ReadP2PPacket( void *pubDest, uint32 cubDest, uint32 *pcubMsgSize, CSteamID *psteamIDRemote, int nChannel = 0 ) = 0;
	virtual bool AcceptP2PSessionWithUser( CSteamID steamIDRemote ) = 0;
	virtual bool CloseP2PSessionWithUser( CSteamID steamIDRemote ) = 0;
	virtual bool CloseP2PChannelWithUser( CSteamID steamIDRemote, int nChannel ) = 0;
	virtual bool GetP2PSessionState( CSteamID steamIDRemote, P2PSessionState_t *pConnectionState ) = 0;
	virtual bool AllowP2PPacketRelay( bool bAllow ) = 0;
	////////////////////////////////////////////////////////////////////////////////////////////
	virtual SNetListenSocket_t CreateListenSocket( int nVirtualP2PPort, uint32 nIP, uint16 nPort, bool bAllowUseOfPacketRelay ) = 0;
	virtual SNetSocket_t CreateP2PConnectionSocket( CSteamID steamIDTarget, int nVirtualPort, int nTimeoutSec, bool bAllowUseOfPacketRelay ) = 0;
	virtual SNetSocket_t CreateConnectionSocket( uint32 nIP, uint16 nPort, int nTimeoutSec ) = 0;
	virtual bool DestroySocket( SNetSocket_t hSocket, bool bNotifyRemoteEnd ) = 0;
	virtual bool DestroyListenSocket( SNetListenSocket_t hSocket, bool bNotifyRemoteEnd ) = 0;
	virtual bool SendDataOnSocket( SNetSocket_t hSocket, void *pubData, uint32 cubData, bool bReliable ) = 0;
	virtual bool IsDataAvailableOnSocket( SNetSocket_t hSocket, uint32 *pcubMsgSize ) = 0; 
	virtual bool RetrieveDataFromSocket( SNetSocket_t hSocket, void *pubDest, uint32 cubDest, uint32 *pcubMsgSize ) = 0; 
	virtual bool IsDataAvailable( SNetListenSocket_t hListenSocket, uint32 *pcubMsgSize, SNetSocket_t *phSocket ) = 0;
	virtual bool RetrieveData( SNetListenSocket_t hListenSocket, void *pubDest, uint32 cubDest, uint32 *pcubMsgSize, SNetSocket_t *phSocket ) = 0;
	virtual bool GetSocketInfo( SNetSocket_t hSocket, CSteamID *pSteamIDRemote, int *peSocketStatus, uint32 *punIPRemote, uint16 *punPortRemote ) = 0;
	virtual bool GetListenSocketInfo( SNetListenSocket_t hListenSocket, uint32 *pnIP, uint16 *pnPort ) = 0;
	virtual ESNetSocketConnectionType GetSocketConnectionType( SNetSocket_t hSocket ) = 0;
	virtual int GetMaxPacketSize( SNetSocket_t hSocket ) = 0;
};
#define STEAMNETWORKING_INTERFACE_VERSION "SteamNetworking005"
#if defined( VALVE_CALLBACK_PACK_SMALL )
#pragma pack( push, 4 )
#elif defined( VALVE_CALLBACK_PACK_LARGE )
#pragma pack( push, 8 )
#else
#error isteamclient.hpp must be included
#endif 
struct P2PSessionRequest_t
{ 
	enum { k_iCallback = k_iSteamNetworkingCallbacks + 2 };
	CSteamID m_IDRemote;			// user who wants to talk to us
};
struct P2PSessionConnectFail_t
{ 
	enum { k_iCallback = k_iSteamNetworkingCallbacks + 3 };
	CSteamID m_IDRemote;			// user we were sending packets to
	uint8 m_eP2PSessionError;			// EP2PSessionError indicating why we're having trouble
};
struct SocketStatusCallback_t
{ 
	enum { k_iCallback = k_iSteamNetworkingCallbacks + 1 };
	SNetSocket_t m_hSocket;				// the socket used to send/receive data to the remote host
	SNetListenSocket_t m_hListenSocket;	// this is the server socket that we were listening on; NULL if this was an outgoing connection
	CSteamID m_IDRemote;			// remote steamID we have connected to, if it has one
	int m_eSNetSocketState;				// socket state, ESNetSocketState
};
#pragma pack( pop )
#endif // ISTEAMNETWORKING
