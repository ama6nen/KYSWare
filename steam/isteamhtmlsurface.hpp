#ifndef ISTEAMHTMLSURFACE_H
#define ISTEAMHTMLSURFACE_H
#ifdef _WIN32
#pragma once
#endif
#include "isteamclient.hpp"
typedef uint32 HHTMLBrowser;
const uint32 INVALID_HTMLBROWSER = 0;
class ISteamHTMLSurface
{
public:
	virtual ~ISteamHTMLSurface() {}
	virtual bool init() = 0;
	virtual bool Shutdown() = 0;
	virtual SteamAPICall_t CreateBrowser( const char *pchUserAgent, const char *pchUserCSS ) = 0;
	virtual void RemoveBrowser( HHTMLBrowser unBrowserHandle ) = 0;
	virtual void LoadURL( HHTMLBrowser unBrowserHandle, const char *pchURL, const char *pchPostData ) = 0;
	virtual void SetSize( HHTMLBrowser unBrowserHandle, uint32 unWidth, uint32 unHeight ) = 0;
	virtual void StopLoad( HHTMLBrowser unBrowserHandle ) = 0;
	virtual void Reload( HHTMLBrowser unBrowserHandle ) = 0;
	virtual void GoBack( HHTMLBrowser unBrowserHandle ) = 0;
	virtual void GoForward( HHTMLBrowser unBrowserHandle ) = 0;
	virtual void AddHeader( HHTMLBrowser unBrowserHandle, const char *pchKey, const char *pchValue ) = 0;
	virtual void ExecuteJavascript( HHTMLBrowser unBrowserHandle, const char *pchScript ) = 0;
	enum EHTMLMouseButton
	{
		eHTMLMouseButton_Left = 0,
		eHTMLMouseButton_Right = 1,
		eHTMLMouseButton_Middle = 2,
	};
	virtual void MouseUp( HHTMLBrowser unBrowserHandle, EHTMLMouseButton eMouseButton ) = 0;
	virtual void MouseDown( HHTMLBrowser unBrowserHandle, EHTMLMouseButton eMouseButton ) = 0;
	virtual void MouseDoubleClick( HHTMLBrowser unBrowserHandle, EHTMLMouseButton eMouseButton ) = 0;
	virtual void MouseMove( HHTMLBrowser unBrowserHandle, int x, int y ) = 0;
	virtual void MouseWheel( HHTMLBrowser unBrowserHandle, int32 nDelta ) = 0;
	enum EMouseCursor
	{
		dc_user = 0,
		dc_none,
		dc_arrow,
		dc_ibeam,
		dc_hourglass,
		dc_waitarrow,
		dc_crosshair,
		dc_up,
		dc_sizenw,
		dc_sizese,
		dc_sizene,
		dc_sizesw,
		dc_sizew,
		dc_sizee,
		dc_sizen,
		dc_sizes,
		dc_sizewe,
		dc_sizens,
		dc_sizeall,
		dc_no,
		dc_hand,
		dc_blank, // don't show any custom cursor, just use your default
		dc_middle_pan,
		dc_north_pan,
		dc_north_east_pan,
		dc_east_pan,
		dc_south_east_pan,
		dc_south_pan,
		dc_south_west_pan,
		dc_west_pan,
		dc_north_west_pan,
		dc_alias,
		dc_cell,
		dc_colresize,
		dc_copycur,
		dc_verticaltext,
		dc_rowresize,
		dc_zoomin,
		dc_zoomout,
		dc_help,
		dc_custom,
		dc_last, // custom cursors start from this value and up
	};
	enum EHTMLKeyModifiers
	{
		k_eHTMLKeyModifier_None = 0,
		k_eHTMLKeyModifier_AltDown = 1 << 0,
		k_eHTMLKeyModifier_CtrlDown = 1 << 1,
		k_eHTMLKeyModifier_ShiftDown = 1 << 2,
	};
	virtual void KeyDown( HHTMLBrowser unBrowserHandle, uint32 nNativeKeyCode, EHTMLKeyModifiers eHTMLKeyModifiers ) = 0;
	virtual void KeyUp( HHTMLBrowser unBrowserHandle, uint32 nNativeKeyCode, EHTMLKeyModifiers eHTMLKeyModifiers ) = 0;
	virtual void KeyChar( HHTMLBrowser unBrowserHandle, uint32 cUnicodeChar, EHTMLKeyModifiers eHTMLKeyModifiers ) = 0;
	virtual void SetHorizontalScroll( HHTMLBrowser unBrowserHandle, uint32 nAbsolutePixelScroll ) = 0;
	virtual void SetVerticalScroll( HHTMLBrowser unBrowserHandle, uint32 nAbsolutePixelScroll ) = 0;
	virtual void SetKeyFocus( HHTMLBrowser unBrowserHandle, bool bHasKeyFocus ) = 0;
	virtual void ViewSource( HHTMLBrowser unBrowserHandle ) = 0;
	virtual void CopyToClipboard( HHTMLBrowser unBrowserHandle ) = 0;
	virtual void PasteFromClipboard( HHTMLBrowser unBrowserHandle ) = 0;
	virtual void Find( HHTMLBrowser unBrowserHandle, const char *pchSearchStr, bool bCurrentlyInFind, bool bReverse ) = 0;
	virtual void StopFind( HHTMLBrowser unBrowserHandle ) = 0;
	virtual void GetLinkAtPosition(  HHTMLBrowser unBrowserHandle, int x, int y ) = 0;
	virtual void SetCookie( const char *pchHostname, const char *pchKey, const char *pchValue, const char *pchPath = "/", RTime32 nExpires = 0, bool bSecure = false, bool bHTTPOnly = false ) = 0;
	virtual void SetPageScaleFactor( HHTMLBrowser unBrowserHandle, float flZoom, int nPointX, int nPointY ) = 0;
	virtual void SetBackgroundMode( HHTMLBrowser unBrowserHandle, bool bBackgroundMode ) = 0;
	virtual void SetDPIScalingFactor( HHTMLBrowser unBrowserHandle, float flDPIScaling ) = 0;
	virtual void AllowStartRequest( HHTMLBrowser unBrowserHandle, bool bAllowed ) = 0;
	virtual void JSDialogResponse( HHTMLBrowser unBrowserHandle, bool bResult ) = 0;
	IGNOREATTR()
	virtual void FileLoadDialogResponse( HHTMLBrowser unBrowserHandle, const char **pchSelectedFiles ) = 0;
};
#define STEAMHTMLSURFACE_INTERFACE_VERSION "STEAMHTMLSURFACE_INTERFACE_VERSION_004"
#if defined( VALVE_CALLBACK_PACK_SMALL )
#pragma pack( push, 4 )
#elif defined( VALVE_CALLBACK_PACK_LARGE )
#pragma pack( push, 8 )
#else
#error isteamclient.hpp must be included
#endif 
DEFINE_CALLBACK( HTML_BrowserReady_t, k_iSteamHTMLSurfaceCallbacks + 1 )
CALLBACK_MEMBER( 0, HHTMLBrowser, unBrowserHandle ) // this browser is now fully created and ready to navigate to pages
END_DEFINE_CALLBACK_1()
DEFINE_CALLBACK(HTML_NeedsPaint_t, k_iSteamHTMLSurfaceCallbacks + 2)
CALLBACK_MEMBER(0, HHTMLBrowser, unBrowserHandle) // the browser that needs the paint
CALLBACK_MEMBER(1, const char *, pBGRA ) // a pointer to the B8G8R8A8 data for this surface, valid until SteamAPI_RunCallbacks is next called
CALLBACK_MEMBER(2, uint32, unWide) // the total width of the pBGRA texture
CALLBACK_MEMBER(3, uint32, unTall) // the total height of the pBGRA texture
CALLBACK_MEMBER(4, uint32, unUpdateX) // the offset in X for the damage rect for this update
CALLBACK_MEMBER(5, uint32, unUpdateY) // the offset in Y for the damage rect for this update
CALLBACK_MEMBER(6, uint32, unUpdateWide) // the width of the damage rect for this update
CALLBACK_MEMBER(7, uint32, unUpdateTall) // the height of the damage rect for this update
CALLBACK_MEMBER(8, uint32, unScrollX) // the page scroll the browser was at when this texture was rendered
CALLBACK_MEMBER(9, uint32, unScrollY) // the page scroll the browser was at when this texture was rendered
CALLBACK_MEMBER(10, float, flPageScale) // the page scale factor on this page when rendered
CALLBACK_MEMBER(11, uint32, unPageSerial) // incremented on each new page load, you can use this to reject draws while navigating to new pages
END_DEFINE_CALLBACK_12()
DEFINE_CALLBACK(HTML_StartRequest_t, k_iSteamHTMLSurfaceCallbacks + 3)
CALLBACK_MEMBER(0, HHTMLBrowser, unBrowserHandle) // the handle of the surface navigating
CALLBACK_MEMBER(1, const char *, pchURL) // the url they wish to navigate to 
CALLBACK_MEMBER(2, const char *, pchTarget) // the html link target type  (i.e _blank, _self, _parent, _top )
CALLBACK_MEMBER(3, const char *, pchPostData ) // any posted data for the request
CALLBACK_MEMBER(4, bool, bIsRedirect) // true if this was a http/html redirect from the last load request
END_DEFINE_CALLBACK_5()
DEFINE_CALLBACK(HTML_CloseBrowser_t, k_iSteamHTMLSurfaceCallbacks + 4)
CALLBACK_MEMBER(0, HHTMLBrowser, unBrowserHandle) // the handle of the surface 
END_DEFINE_CALLBACK_1()
DEFINE_CALLBACK( HTML_URLChanged_t, k_iSteamHTMLSurfaceCallbacks + 5 )
CALLBACK_MEMBER( 0, HHTMLBrowser, unBrowserHandle ) // the handle of the surface navigating
CALLBACK_MEMBER( 1, const char *, pchURL ) // the url they wish to navigate to 
CALLBACK_MEMBER( 2, const char *, pchPostData ) // any posted data for the request
CALLBACK_MEMBER( 3, bool, bIsRedirect ) // true if this was a http/html redirect from the last load request
CALLBACK_MEMBER( 4, const char *, pchPageTitle ) // the title of the page
CALLBACK_MEMBER( 5, bool, bNewNavigation ) // true if this was from a fresh tab and not a click on an existing page
END_DEFINE_CALLBACK_6()
DEFINE_CALLBACK( HTML_FinishedRequest_t, k_iSteamHTMLSurfaceCallbacks + 6 )
CALLBACK_MEMBER( 0, HHTMLBrowser, unBrowserHandle ) // the handle of the surface 
CALLBACK_MEMBER( 1, const char *, pchURL ) // 
CALLBACK_MEMBER( 2, const char *, pchPageTitle ) // 
END_DEFINE_CALLBACK_3()
DEFINE_CALLBACK( HTML_OpenLinkInNewTab_t, k_iSteamHTMLSurfaceCallbacks + 7 )
CALLBACK_MEMBER( 0, HHTMLBrowser, unBrowserHandle ) // the handle of the surface 
CALLBACK_MEMBER( 1, const char *, pchURL ) // 
END_DEFINE_CALLBACK_2()
DEFINE_CALLBACK( HTML_ChangedTitle_t, k_iSteamHTMLSurfaceCallbacks + 8 )
CALLBACK_MEMBER( 0, HHTMLBrowser, unBrowserHandle ) // the handle of the surface 
CALLBACK_MEMBER( 1, const char *, pchTitle ) // 
END_DEFINE_CALLBACK_2()
DEFINE_CALLBACK( HTML_SearchResults_t, k_iSteamHTMLSurfaceCallbacks + 9 )
CALLBACK_MEMBER( 0, HHTMLBrowser, unBrowserHandle ) // the handle of the surface 
CALLBACK_MEMBER( 1, uint32, unResults ) // 
CALLBACK_MEMBER( 2, uint32, unCurrentMatch ) // 
END_DEFINE_CALLBACK_3()
DEFINE_CALLBACK( HTML_CanGoBackAndForward_t, k_iSteamHTMLSurfaceCallbacks + 10 )
CALLBACK_MEMBER( 0, HHTMLBrowser, unBrowserHandle ) // the handle of the surface 
CALLBACK_MEMBER( 1, bool, bCanGoBack ) // 
CALLBACK_MEMBER( 2, bool, bCanGoForward ) // 
END_DEFINE_CALLBACK_3()
DEFINE_CALLBACK( HTML_HorizontalScroll_t, k_iSteamHTMLSurfaceCallbacks + 11 )
CALLBACK_MEMBER( 0, HHTMLBrowser, unBrowserHandle ) // the handle of the surface 
CALLBACK_MEMBER( 1, uint32, unScrollMax ) // 
CALLBACK_MEMBER( 2, uint32, unScrollCurrent ) // 
CALLBACK_MEMBER( 3, float, flPageScale ) // 
CALLBACK_MEMBER( 4, bool , bVisible ) // 
CALLBACK_MEMBER( 5, uint32, unPageSize ) // 
END_DEFINE_CALLBACK_6()
DEFINE_CALLBACK( HTML_VerticalScroll_t, k_iSteamHTMLSurfaceCallbacks + 12 )
CALLBACK_MEMBER( 0, HHTMLBrowser, unBrowserHandle ) // the handle of the surface 
CALLBACK_MEMBER( 1, uint32, unScrollMax ) // 
CALLBACK_MEMBER( 2, uint32, unScrollCurrent ) // 
CALLBACK_MEMBER( 3, float, flPageScale ) // 
CALLBACK_MEMBER( 4, bool, bVisible ) // 
CALLBACK_MEMBER( 5, uint32, unPageSize ) // 
END_DEFINE_CALLBACK_6()
DEFINE_CALLBACK( HTML_LinkAtPosition_t, k_iSteamHTMLSurfaceCallbacks + 13 )
CALLBACK_MEMBER( 0, HHTMLBrowser, unBrowserHandle ) // the handle of the surface 
CALLBACK_MEMBER( 1, uint32, x ) // NOTE - Not currently set
CALLBACK_MEMBER( 2, uint32, y ) // NOTE - Not currently set
CALLBACK_MEMBER( 3, const char *, pchURL ) // 
CALLBACK_MEMBER( 4, bool, bInput ) // 
CALLBACK_MEMBER( 5, bool, bLiveLink ) // 
END_DEFINE_CALLBACK_6()
DEFINE_CALLBACK( HTML_JSAlert_t, k_iSteamHTMLSurfaceCallbacks + 14 )
CALLBACK_MEMBER( 0, HHTMLBrowser, unBrowserHandle ) // the handle of the surface 
CALLBACK_MEMBER( 1, const char *, pchMessage ) // 
END_DEFINE_CALLBACK_2()
DEFINE_CALLBACK( HTML_JSConfirm_t, k_iSteamHTMLSurfaceCallbacks + 15 )
CALLBACK_MEMBER( 0, HHTMLBrowser, unBrowserHandle ) // the handle of the surface 
CALLBACK_MEMBER( 1, const char *, pchMessage ) // 
END_DEFINE_CALLBACK_2()
DEFINE_CALLBACK( HTML_FileOpenDialog_t, k_iSteamHTMLSurfaceCallbacks + 16 )
CALLBACK_MEMBER( 0, HHTMLBrowser, unBrowserHandle ) // the handle of the surface 
CALLBACK_MEMBER( 1, const char *, pchTitle ) // 
CALLBACK_MEMBER( 2, const char *, pchInitialFile ) // 
END_DEFINE_CALLBACK_3()
DEFINE_CALLBACK( HTML_NewWindow_t, k_iSteamHTMLSurfaceCallbacks + 21 )
CALLBACK_MEMBER( 0, HHTMLBrowser, unBrowserHandle ) // the handle of the current surface 
CALLBACK_MEMBER( 1, const char *, pchURL ) // the page to load
CALLBACK_MEMBER( 2, uint32, unX ) // the x pos into the page to display the popup
CALLBACK_MEMBER( 3, uint32, unY ) // the y pos into the page to display the popup
CALLBACK_MEMBER( 4, uint32, unWide ) // the total width of the pBGRA texture
CALLBACK_MEMBER( 5, uint32, unTall ) // the total height of the pBGRA texture
CALLBACK_MEMBER( 6, HHTMLBrowser, unNewWindow_BrowserHandle ) // the handle of the new window surface 
END_DEFINE_CALLBACK_7()
DEFINE_CALLBACK( HTML_SetCursor_t, k_iSteamHTMLSurfaceCallbacks + 22 )
CALLBACK_MEMBER( 0, HHTMLBrowser, unBrowserHandle ) // the handle of the surface 
CALLBACK_MEMBER( 1, uint32, eMouseCursor ) // the EMouseCursor to display
END_DEFINE_CALLBACK_2()
DEFINE_CALLBACK( HTML_StatusText_t, k_iSteamHTMLSurfaceCallbacks + 23 )
CALLBACK_MEMBER( 0, HHTMLBrowser, unBrowserHandle ) // the handle of the surface 
CALLBACK_MEMBER( 1, const char *, pchMsg ) // the EMouseCursor to display
END_DEFINE_CALLBACK_2()
DEFINE_CALLBACK( HTML_ShowToolTip_t, k_iSteamHTMLSurfaceCallbacks + 24 )
CALLBACK_MEMBER( 0, HHTMLBrowser, unBrowserHandle ) // the handle of the surface 
CALLBACK_MEMBER( 1, const char *, pchMsg ) // the EMouseCursor to display
END_DEFINE_CALLBACK_2()
DEFINE_CALLBACK( HTML_UpdateToolTip_t, k_iSteamHTMLSurfaceCallbacks + 25 )
CALLBACK_MEMBER( 0, HHTMLBrowser, unBrowserHandle ) // the handle of the surface 
CALLBACK_MEMBER( 1, const char *, pchMsg ) // the EMouseCursor to display
END_DEFINE_CALLBACK_2()
DEFINE_CALLBACK( HTML_HideToolTip_t, k_iSteamHTMLSurfaceCallbacks + 26 )
CALLBACK_MEMBER( 0, HHTMLBrowser, unBrowserHandle ) // the handle of the surface 
END_DEFINE_CALLBACK_1()
DEFINE_CALLBACK( HTML_BrowserRestarted_t, k_iSteamHTMLSurfaceCallbacks + 27 )
CALLBACK_MEMBER( 0, HHTMLBrowser, unBrowserHandle ) // this is the new browser handle after the restart
CALLBACK_MEMBER( 1, HHTMLBrowser, unOldBrowserHandle ) // the handle for the browser before the restart, if your handle was this then switch to using unBrowserHandle for API calls
END_DEFINE_CALLBACK_2()
#pragma pack( pop )
#endif // ISTEAMHTMLSURFACE_H
