#ifndef ISTEAMHTTP_H
#define ISTEAMHTTP_H
#ifdef _WIN32
#pragma once
#endif
#include "isteamclient.hpp"
#include "steamhttpenums.hpp"
typedef uint32 HTTPRequestHandle;
#define INVALID_HTTPREQUEST_HANDLE		0
typedef uint32 HTTPCookieContainerHandle;
#define INVALID_HTTPCOOKIE_HANDLE		0
class ISteamHTTP
{
public:
	virtual HTTPRequestHandle CreateHTTPRequest( EHTTPMethod eHTTPRequestMethod, const char *pchAbsoluteURL ) = 0;
	virtual bool SetHTTPRequestContextValue( HTTPRequestHandle hRequest, uint64 ulContextValue ) = 0;
	virtual bool SetHTTPRequestNetworkActivityTimeout( HTTPRequestHandle hRequest, uint32 unTimeoutSeconds ) = 0;
	virtual bool SetHTTPRequestHeaderValue( HTTPRequestHandle hRequest, const char *pchHeaderName, const char *pchHeaderValue ) = 0;
	virtual bool SetHTTPRequestGetOrPostParameter( HTTPRequestHandle hRequest, const char *pchParamName, const char *pchParamValue ) = 0;
	virtual bool SendHTTPRequest( HTTPRequestHandle hRequest, SteamAPICall_t *pCallHandle ) = 0;
	virtual bool SendHTTPRequestAndStreamResponse( HTTPRequestHandle hRequest, SteamAPICall_t *pCallHandle ) = 0;
	virtual bool DeferHTTPRequest( HTTPRequestHandle hRequest ) = 0;
	virtual bool PrioritizeHTTPRequest( HTTPRequestHandle hRequest ) = 0;
	virtual bool GetHTTPResponseHeaderSize( HTTPRequestHandle hRequest, const char *pchHeaderName, uint32 *unResponseHeaderSize ) = 0;
	virtual bool GetHTTPResponseHeaderValue( HTTPRequestHandle hRequest, const char *pchHeaderName, uint8 *pHeaderValueBuffer, uint32 unBufferSize ) = 0;
	virtual bool GetHTTPResponseBodySize( HTTPRequestHandle hRequest, uint32 *unBodySize ) = 0;
	virtual bool GetHTTPResponseBodyData( HTTPRequestHandle hRequest, uint8 *pBodyDataBuffer, uint32 unBufferSize ) = 0;
	virtual bool GetHTTPStreamingResponseBodyData( HTTPRequestHandle hRequest, uint32 cOffset, uint8 *pBodyDataBuffer, uint32 unBufferSize ) = 0;
	virtual bool ReleaseHTTPRequest( HTTPRequestHandle hRequest ) = 0;
	virtual bool GetHTTPDownloadProgressPct( HTTPRequestHandle hRequest, float *pflPercentOut ) = 0;
	virtual bool SetHTTPRequestRawPostBody( HTTPRequestHandle hRequest, const char *pchContentType, uint8 *pubBody, uint32 unBodyLen ) = 0;
	virtual HTTPCookieContainerHandle CreateCookieContainer( bool bAllowResponsesToModify ) = 0;
	virtual bool ReleaseCookieContainer( HTTPCookieContainerHandle hCookieContainer ) = 0;
	virtual bool SetCookie( HTTPCookieContainerHandle hCookieContainer, const char *pchHost, const char *pchUrl, const char *pchCookie ) = 0;
	virtual bool SetHTTPRequestCookieContainer( HTTPRequestHandle hRequest, HTTPCookieContainerHandle hCookieContainer ) = 0;
	virtual bool SetHTTPRequestUserAgentInfo( HTTPRequestHandle hRequest, const char *pchUserAgentInfo ) = 0;
	virtual bool SetHTTPRequestRequiresVerifiedCertificate( HTTPRequestHandle hRequest, bool bRequireVerifiedCertificate ) = 0;
	virtual bool SetHTTPRequestAbsoluteTimeoutMS( HTTPRequestHandle hRequest, uint32 unMilliseconds ) = 0;
	virtual bool GetHTTPRequestWasTimedOut( HTTPRequestHandle hRequest, bool *pbWasTimedOut ) = 0;
};
#define STEAMHTTP_INTERFACE_VERSION "STEAMHTTP_INTERFACE_VERSION002"
#if defined( VALVE_CALLBACK_PACK_SMALL )
#pragma pack( push, 4 )
#elif defined( VALVE_CALLBACK_PACK_LARGE )
#pragma pack( push, 8 )
#else
#error isteamclient.hpp must be included
#endif 
struct HTTPRequestCompleted_t
{
	enum { k_iCallback = k_iClientHTTPCallbacks + 1 };
	HTTPRequestHandle m_hRequest;
	uint64 m_ulContextValue;
	bool m_bRequestSuccessful;
	EHTTPStatusCode m_eStatusCode;
	uint32 m_unBodySize; // Same as GetHTTPResponseBodySize()
};
struct HTTPRequestHeadersReceived_t
{
	enum { k_iCallback = k_iClientHTTPCallbacks + 2 };
	HTTPRequestHandle m_hRequest;
	uint64 m_ulContextValue;
};
struct HTTPRequestDataReceived_t
{
	enum { k_iCallback = k_iClientHTTPCallbacks + 3 };
	HTTPRequestHandle m_hRequest;
	uint64 m_ulContextValue;
	uint32 m_cOffset;
	uint32 m_cBytesReceived;
};
#pragma pack( pop )
#endif // ISTEAMHTTP_H