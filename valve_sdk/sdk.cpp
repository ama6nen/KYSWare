#include "sdk.hpp"
#include "../proto/gcsystemmsgs.pb.h"
#include "../proto/gcsdk_gcmessages.pb.h"
#include "../Helpers/Utils.hpp"

IVEngineClient*       g_EngineClient   = nullptr;
IBaseClientDLL*       g_CHLClient      = nullptr;
IClientEntityList*    g_EntityList     = nullptr;
CGlobalVarsBase*      g_GlobalVars     = nullptr;
IEngineTrace*         g_EngineTrace    = nullptr;
ICvar*                g_CVar           = nullptr;
IPanel*               g_VGuiPanel      = nullptr;
IClientMode*          g_ClientMode     = nullptr;
CHostState*			  g_HostState	   = nullptr;
IVDebugOverlay*       g_DebugOverlay   = nullptr;
ISurface*             g_VGuiSurface    = nullptr;
CInput*               g_Input          = nullptr;
IVModelInfoClient*    g_MdlInfo        = nullptr;
IVModelRender*        g_MdlRender      = nullptr;
IVRenderView*         g_RenderView     = nullptr;
IMaterialSystem*      g_MatSystem      = nullptr;
IGameEventManager2*   g_GameEvents     = nullptr;
IMoveHelper*          g_MoveHelper     = nullptr;
IMDLCache*            g_MdlCache       = nullptr;
IPrediction*          g_Prediction     = nullptr;
CGameMovement*        g_GameMovement   = nullptr;
IEngineSound*         g_EngineSound    = nullptr;
CGlowObjectManager*   g_GlowObjManager = nullptr;
IViewRender*          g_ViewRender     = nullptr;
IDirect3DDevice9*     g_D3DDevice9     = nullptr;
CClientState*         g_ClientState    = nullptr;
IPhysicsSurfaceProps* g_PhysSurface    = nullptr;
IVEffects*			  g_IVEffects = nullptr;
IEffects*			  g_IEffects = nullptr;
C_LocalPlayer         g_LocalPlayer;
IViewRenderBeams*	  g_IViewRenderBeams = nullptr;
CHudChat* g_ChatElement = nullptr;
IMemAlloc* g_MemAlloc = nullptr;
IInputSystem* g_InputSystem = nullptr;
DWORD* g_InputInternal = nullptr;
INetworkStringTableContainer* g_NetworkStringTable = nullptr;
DWORD OverridePostProcessingDisable = NULL;
C_CSGameRules* g_GameRules = nullptr;
std::unique_ptr<steam_api_context> steam;
ISteamGameCoordinator* SteamGameCoordinator = nullptr;
ISteamUser* steamUser = nullptr;
namespace Interfaces
{
	CreateInterfaceFn get_module_factory( HMODULE module )
	{
		return reinterpret_cast<CreateInterfaceFn>( GetProcAddress( module, "CreateInterface" ) );
	}

	template<typename T>
	T* get_interface( CreateInterfaceFn f, const char* szInterfaceVersion )
	{
		auto result = reinterpret_cast<T*>( f( szInterfaceVersion, nullptr ) );

		if ( !result )
		{
			throw std::runtime_error( std::string( "[get_interface] Failed to GetOffset interface: " ) + szInterfaceVersion );
		}

		return result;
	}

	ISteamGameCoordinator* GetSteamGameCoordinator( )
	{
		if ( !SteamGameCoordinator )
		{
			typedef uint32_t SteamPipeHandle;
			typedef uint32_t SteamUserHandle;

			SteamUserHandle hSteamUser = ( ( SteamUserHandle( __cdecl* )( void ) )GetProcAddress( LI_FIND_CACHED( GetModuleHandleW )( L"steam_api.dll" ), "SteamAPI_GetHSteamUser" ) )( );
			SteamPipeHandle hSteamPipe = ( ( SteamPipeHandle( __cdecl* )( void ) )GetProcAddress( LI_FIND_CACHED( GetModuleHandleW )( L"steam_api.dll" ), "SteamAPI_GetHSteamPipe" ) )( );

			auto SteamClient = ( ( ISteamClient*( __cdecl* )( void ) )GetProcAddress( LI_FIND_CACHED( GetModuleHandleW )( L"steam_api.dll" ), "SteamClient" ) )( );

			auto SteamHTTP = SteamClient->GetISteamHTTP( hSteamUser, hSteamPipe, "STEAMHTTP_INTERFACE_VERSION002" );
			steamUser = SteamClient->GetISteamUser( hSteamUser, hSteamPipe, "SteamUser019" );
			auto SteamFriends = SteamClient->GetISteamFriends( hSteamUser, hSteamPipe, "SteamFriends015" );
			auto SteamInventory = SteamClient->GetISteamInventory( hSteamUser, hSteamPipe, "STEAMINVENTORY_INTERFACE_V002" );
			SteamGameCoordinator = (ISteamGameCoordinator*)SteamClient->GetISteamGenericInterface( hSteamUser, hSteamPipe, "SteamGameCoordinator001" );
		}

		return SteamGameCoordinator;

	}

	bool SendClientHello( )
	{
		
		CMsgClientHello Message;

		Message.set_client_session_need( 1 );
		Message.clear_socache_have_versions( );

		void* ptr = malloc( Message.ByteSize( ) + 8 );

		if ( !ptr )
			return false;

		( (uint32_t*)ptr )[ 0 ] = k_EMsgGCClientHello | ( (DWORD)1 << 31 );
		( (uint32_t*)ptr )[ 1 ] = 0;

		Message.SerializeToArray( (void*)( (DWORD)ptr + 8 ), Message.ByteSize( ) );
		
		bool result = SteamGameCoordinator->SendMessageW( k_EMsgGCClientHello | ( (DWORD)1 << 31 ), ptr, Message.ByteSize( ) + 8 ) == k_EGCResultOK;

		free( ptr );

		return result;
	}
	void Initialize( )
	{
		auto engineFactory = get_module_factory( LI_FIND_CACHED( GetModuleHandleW )( L"engine.dll" ) );
		auto clientFactory = get_module_factory( LI_FIND_CACHED( GetModuleHandleW )( L"client.dll" ) );
		auto valveStdFactory = get_module_factory( LI_FIND_CACHED( GetModuleHandleW )( L"vstdlib.dll" ) );
		auto vguiFactory = get_module_factory( LI_FIND_CACHED( GetModuleHandleW )( L"vguimatsurface.dll" ) );
		auto vgui2Factory = get_module_factory( LI_FIND_CACHED( GetModuleHandleW )( L"vgui2.dll" ) );
		auto matSysFactory = get_module_factory( LI_FIND_CACHED( GetModuleHandleW )( L"materialsystem.dll" ) );
		auto dataCacheFactory = get_module_factory( LI_FIND_CACHED( GetModuleHandleW )( L"datacache.dll" ) );
		auto vphysicsFactory = get_module_factory( LI_FIND_CACHED( GetModuleHandleW )( L"vphysics.dll" ) );
		auto inputsystemFactory = get_module_factory( LI_FIND_CACHED( GetModuleHandleW )( L"inputsystem.dll" ) );

		g_CHLClient = get_interface<IBaseClientDLL>( clientFactory, "VClient018" );
		g_EntityList = get_interface<IClientEntityList>( clientFactory, "VClientEntityList003" );
		g_Prediction = get_interface<IPrediction>( clientFactory, "VClientPrediction001" );
		g_GameMovement = get_interface<CGameMovement>( clientFactory, "GameMovement001" );
		g_MdlCache = get_interface<IMDLCache>( dataCacheFactory, "MDLCache004" );
		g_EngineClient = get_interface<IVEngineClient>( engineFactory, "VEngineClient014" );
		g_MdlInfo = get_interface<IVModelInfoClient>( engineFactory, "VModelInfoClient004" );
		g_MdlRender = get_interface<IVModelRender>( engineFactory, "VEngineModel016" );
		g_RenderView = get_interface<IVRenderView>( engineFactory, "VEngineRenderView014" );
		g_EngineTrace = get_interface<IEngineTrace>( engineFactory, "EngineTraceClient004" );
		g_DebugOverlay = get_interface<IVDebugOverlay>( engineFactory, "VDebugOverlay004" );
		g_InputSystem = get_interface<IInputSystem>( inputsystemFactory, "InputSystemVersion001" );
		g_GameEvents = get_interface<IGameEventManager2>( engineFactory, "GAMEEVENTSMANAGER002" );
		g_EngineSound = get_interface<IEngineSound>( engineFactory, "IEngineSoundClient003" );
		g_MatSystem = get_interface<IMaterialSystem>( matSysFactory, "VMaterialSystem080" );
		g_CVar = get_interface<ICvar>( valveStdFactory, "VEngineCvar007" );
		g_VGuiPanel = get_interface<IPanel>( vgui2Factory, "VGUI_Panel009" );
		g_InputInternal = get_interface<DWORD>( vgui2Factory, "VGUI_InputInternal001" );
		g_VGuiSurface = get_interface<ISurface>( vguiFactory, "VGUI_Surface031" );
		g_PhysSurface = get_interface<IPhysicsSurfaceProps>( vphysicsFactory, "VPhysicsSurfaceProps001" );
		g_IVEffects = get_interface<IVEffects>( engineFactory, "VEngineEffects001" );
		g_IEffects = get_interface<IEffects>( clientFactory, "IEffects001" );
		g_NetworkStringTable = get_interface<INetworkStringTableContainer>( engineFactory, "VEngineClientStringTable001" );

		auto client = LI_FIND_CACHED( GetModuleHandleW )( L"client.dll" );
		auto engine_ = LI_FIND_CACHED( GetModuleHandleW )( L"engine.dll" );
		auto dx9api = LI_FIND_CACHED( GetModuleHandleW )( L"shaderapidx9.dll" );

		g_GlobalVars = **(CGlobalVarsBase***)( Utils::PatternScan( client, "A1 ? ? ? ? 5E 8B 40 10" ) + 1 );
		g_ClientMode = *(IClientMode**)( Utils::PatternScan( client, "A1 ? ? ? ? 8B 80 ? ? ? ? 5D" ) + 1 );
		g_Input = *(CInput**)( Utils::PatternScan( client, "B9 ? ? ? ? 8B 40 38 FF D0 84 C0 0F 85" ) + 1 );
		g_MoveHelper = **(IMoveHelper***)( Utils::PatternScan( client, "8B 0D ? ? ? ? 8B 45 ? 51 8B D4 89 02 8B 01" ) + 2 );
		g_GlowObjManager = *(CGlowObjectManager**)( Utils::PatternScan( client, "0F 11 05 ? ? ? ? 83 C8 01" ) + 3 );
		g_ViewRender = *(IViewRender**)( Utils::PatternScan( client, "A1 ? ? ? ? B9 ? ? ? ? C7 05 ? ? ? ? ? ? ? ? FF 10" ) + 1 );
		g_D3DDevice9 = **(IDirect3DDevice9***)( Utils::PatternScan( dx9api, "A1 ? ? ? ? 50 8B 08 FF 51 0C" ) + 1 );
		g_ClientState = **(CClientState***)( Utils::PatternScan( engine_, "A1 ? ? ? ? 8B 80 ? ? ? ? C3" ) + 1 );
		g_IViewRenderBeams = *reinterpret_cast<IViewRenderBeams**>( Utils::PatternScan( client, "A1 ? ? ? ? 56 8B F1 B9 ? ? ? ? FF 50 08" ) + 0x1 );

		g_LocalPlayer = *(C_LocalPlayer*)( Utils::PatternScan( client, "8B 0D ? ? ? ? 83 FF FF 74 07" ) + 2 );
		g_MemAlloc = *reinterpret_cast<IMemAlloc**>( GetProcAddress( LI_FIND_CACHED( GetModuleHandleA )( "tier0.dll" ), "g_pMemAlloc" ) );
		g_ChatElement = FindHudElement<CHudChat>( "CHudChat" );
		OverridePostProcessingDisable = *(DWORD*)( Utils::PatternScan( client, "80 3D ? ? ? ? ? 53 56 57 0F 85" ) + 2 );
		g_GameRules = *(C_CSGameRules**)( Utils::PatternScan( client, "A1 ? ? ? ? 8B 0D ? ? ? ? 6A 00 68 ? ? ? ? C6 80" ) + 1 );
							//"\xA1\x00\x00\x00\x00\x8B\x0D\x00\x00\x00\x00\x6A\x00\x68\x00\x00\x00\x00\xC6\x80", "x????xx????xxx????xx"

		steam = std::make_unique<steam_api_context>( );
		if ( !steam->init( ) )
			Utils::ConsolePrint( "cant initialize steam api \n" );
		//	
		
		GetSteamGameCoordinator( );
		SendClientHello( );
		Dump( );
		
	}

	void Dump( )
	{
	#define STRINGIFY_IMPL(s) #s
	#define STRINGIFY(s)      STRINGIFY_IMPL(s)
	#define PRINT_INTERFACE(name) Utils::ConsolePrint("\n%-20s: %p", STRINGIFY(name), name)

		PRINT_INTERFACE( g_CHLClient );
		PRINT_INTERFACE( g_EntityList );
		PRINT_INTERFACE( g_Prediction );
		PRINT_INTERFACE( g_GameMovement );
		PRINT_INTERFACE( g_MdlCache );
		PRINT_INTERFACE( g_EngineClient );
		PRINT_INTERFACE( g_MdlInfo );
		PRINT_INTERFACE( g_MdlRender );
		PRINT_INTERFACE( g_RenderView );
		PRINT_INTERFACE( g_EngineTrace );
		PRINT_INTERFACE( g_DebugOverlay );
		PRINT_INTERFACE( g_GameEvents );
		PRINT_INTERFACE( g_EngineSound );
		PRINT_INTERFACE( g_MatSystem );
		PRINT_INTERFACE( g_CVar );
		PRINT_INTERFACE( g_VGuiPanel );
		PRINT_INTERFACE( g_VGuiSurface );
		PRINT_INTERFACE( g_PhysSurface );
		PRINT_INTERFACE( g_IVEffects );
		PRINT_INTERFACE( g_IViewRenderBeams );
		PRINT_INTERFACE( g_GlobalVars );
		PRINT_INTERFACE( g_ClientMode );
		PRINT_INTERFACE( g_Input );
		PRINT_INTERFACE( g_MoveHelper );
		PRINT_INTERFACE( g_GlowObjManager );
		PRINT_INTERFACE( g_ViewRender );
		PRINT_INTERFACE( g_D3DDevice9 );
		PRINT_INTERFACE( g_LocalPlayer );
		PRINT_INTERFACE( g_MemAlloc );
		PRINT_INTERFACE( g_ChatElement );
		PRINT_INTERFACE( g_InputSystem );
		PRINT_INTERFACE( g_InputInternal );
		PRINT_INTERFACE( g_NetworkStringTable );
		PRINT_INTERFACE( OverridePostProcessingDisable );
		PRINT_INTERFACE( g_GameRules );
		PRINT_INTERFACE( SteamGameCoordinator );
		PRINT_INTERFACE( steamUser );
		PRINT_INTERFACE( g_IEffects );
	}
}