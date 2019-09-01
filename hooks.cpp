#include "hooks.hpp"
#include "econitem.h"
namespace Hooks
{
	vfunc_hook hlclient_hook;
	vfunc_hook direct3d_hook;
	vfunc_hook material_hook;
	vfunc_hook vguipanel_hook;
	vfunc_hook vguisurf_hook;
	vfunc_hook mdlrender_hook;
	vfunc_hook clientmode_hook;
	vfunc_hook engine_hook;
	vfunc_hook event_hook;
	vfunc_hook mdlcache_hook;
	vfunc_hook renderview_hook;
	vfunc_hook input_hook;
	vfunc_hook netchannel_hook;
	vfunc_hook inputinternal_hook;
	vfunc_hook gamecoordinator_hook;
	vfunc_hook_rtti viewrender_hook;

	using namespace Utils;
	using namespace Math;
	using namespace events;
	using namespace std;

	std::unordered_map<int, EconomyItemCfg> scf;
	bool LBYok = false;
	bool suppressing_979 = false;
	bool fake_scoped = false;
	event_listener* listener;

	void Shutdown( )
	{
		
		g_GameEvents->RemoveListener( listener );
		delete listener;

		hlclient_hook.unhook_all( );
		direct3d_hook.unhook_all( );
		material_hook.unhook_all( );
		vguipanel_hook.unhook_all( );
		vguisurf_hook.unhook_all( );
		mdlrender_hook.unhook_all( );
		clientmode_hook.unhook_all( );
		engine_hook.unhook_all( );
		event_hook.unhook_all( );
		mdlcache_hook.unhook_all( );
		renderview_hook.unhook_all( );
		input_hook.unhook_all( );
		netchannel_hook.unhook_all( );
		inputinternal_hook.unhook_all( );

		Glow::get( ).Shutdown( );

		Visuals::DestroyFonts( );
	}

	inline bool ApplyWeapon( CHandle<C_BaseCombatWeapon> hWeapon )
	{
		auto wep = (C_BaseCombatWeapon*)g_EntityList->GetClientEntityFromHandle( hWeapon );
		if ( !wep )
			return false;

		auto nWeaponIndex = wep->m_Item( ).m_iItemDefinitionIndex( );
		if ( scf.find( nWeaponIndex ) == scf.end( ) )
			return false;

		player_info_t localplayer_info;

		if ( !engine->GetPlayerInfo( local->EntIndex( ), &localplayer_info ) )
			return false;

		
		auto econItemView = wep->GetEconItemView( );
		if ( econItemView )
		{
			auto weaponDefinition = econItemView->GetStaticData( );
			if ( weaponDefinition )
			{
				static auto inventory = **reinterpret_cast<CPlayerInventory***>( Utils::PatternScan( GetModuleHandleA ( "client.dll"), "8B 3D ? ? ? ? 85 FF 74 1A" ) + 0x2 );
				C_EconItemView* econSkinData = inventory->GetItemInLoadout( g_Local->m_iTeamNum(), weaponDefinition->GetEquippedPosition( ) );
				if ( econSkinData )
				{
					auto econData = econSkinData->GetSOCData( );

					if ( econData )
					{
						wep->m_Item( ).m_iAccountID( ) = localplayer_info.xuid_low;
						wep->m_Item( ).m_iItemIDLow( ) = *econData->GetItemID( );
					//	wep->m_Item( ).m_iItemDefinitionIndex( ) = *econData->GetDefIndex( );
					
						ConsolePrint( "x" );
						if (wep->IsKnife() &&  g_Local && g_Local->IsAlive() )
						{
							ConsolePrint( "0" );
							CHandle<C_BaseViewModel> viewmodel_handle = local->m_hViewModel( );

							if ( viewmodel_handle.m_Index == INVALID_EHANDLE_INDEX )
								return false;

							ConsolePrint( "1" );

							C_BaseViewModel* viewmodel = static_cast<C_BaseViewModel*>( entitylist->GetClientEntityFromHandle( viewmodel_handle ) );

							if ( !viewmodel ) return false;

							CHandle<C_BaseCombatWeapon> viewmodel_weapon_handle = viewmodel->m_hWeapon( );
							ConsolePrint( "2" );
							if ( viewmodel_weapon_handle.m_Index == INVALID_EHANDLE_INDEX )
								return false;

							C_BaseCombatWeapon* viewmodel_weapon = static_cast<C_BaseCombatWeapon*>( entitylist->GetClientEntityFromHandle( viewmodel_weapon_handle ) );
							ConsolePrint( "3" );
							if ( !viewmodel_weapon ) return false;

							if ( viewmodel_weapon == wep )
							{
							
								int knife = g_MdlInfo->GetModelIndex( "models/weapons/v_knife_m9_bay.mdl" );
							    int def = *econData->GetDefIndex( );
								ConsolePrint( "applly %d %d %d\n" ,def,knife, viewmodel_weapon->m_iItemDefinitionIndex( ) );
								//42 viewmodel_weapon->m_iItemDefinitionIndex( ) 
								//def 507 (karambit)
								//knife random
								viewmodel->m_nModelIndex( ) = knife;
								viewmodel->m_nViewModelIndex( ) = knife;
								//https://github.com/namazso/nSkinz/blob/master/src/Hooks/FrameStageNotify.cpp#L280
							//	 viewmodel_weapon->m_Item( ).m_iItemDefinitionIndex( ) = *econData->GetDefIndex( );
								//wep = viewmodel_weapon;

								//
								
							
								//ConsolePrint( "%d || %d || %d || %d -> %d -> %d\n", wep->m_iItemDefinitionIndex( ), wep->m_nModelIndex( ), viewmodel->m_nModelIndex( ), viewmodel_weapon->m_nModelIndex( ), def, knife );

								
								////		wep->GetClientNetworkable( )->PreDataUpdate( 0 );
							
								//wep->m_nFallbackPaintKit( ) = 155;


								//wep->m_Item( ).m_nFallbackPaintKit2( ) = 155;


								viewmodel->m_nModelIndex( ) = knife;
								viewmodel_weapon->SetModelIndex( knife );
								viewmodel_weapon->m_nModelIndex( ) = knife;
								//wep->m_Item( ).m_iItemIDHigh( ) = -1;
								//wep->m_nFallbackPaintKit( ) = scf[ nWeaponIndex ].nFallbackPaintKit;
								//wep->m_Item( ).m_iEntityQuality( ) = scf[ nWeaponIndex ].iEntityQuality;
								//wep->m_nFallbackSeed( ) = scf[ nWeaponIndex ].nFallbackSeed;
								//wep->m_nFallbackStatTrak( ) = scf[ nWeaponIndex ].nFallbackStatTrak;
								//wep->m_flFallbackWear( ) = scf[ nWeaponIndex ].flFallbackWear;
							
								wep->m_iItemDefinitionIndex( ) = def;
								wep->m_Item( ).m_iItemDefinitionIndex( ) = def;

								//ConsolePrint( "2) %d || %d || %d || %d -> %d -> %d\n", wep->m_iItemDefinitionIndex( ), wep->m_nModelIndex( ), viewmodel->m_nModelIndex( ), viewmodel_weapon->m_nModelIndex( ), def, knife );

							}
						
						//	
						}
						return true;
					}
					
				}
			}
		}
		
		return true;
		wep->m_nFallbackPaintKit( ) = scf[ nWeaponIndex ].nFallbackPaintKit;
		wep->m_Item( ).m_iEntityQuality( ) = scf[ nWeaponIndex ].iEntityQuality;
		wep->m_nFallbackSeed( ) = scf[ nWeaponIndex ].nFallbackSeed;
		wep->m_nFallbackStatTrak( ) = scf[ nWeaponIndex ].nFallbackStatTrak;
		wep->m_flFallbackWear( ) = scf[ nWeaponIndex ].flFallbackWear;
		wep->m_Item( ).m_iItemIDHigh( ) = -1;
		wep->m_Item( ).m_iAccountID( ) = localplayer_info.xuid_low; 	// (fixes StatTrak)

	
		if ( wep->IsKnife( ) || wep->IsSMG( ) )
		{

			int knife = g_MdlInfo->GetModelIndex( "models/weapons/eminem/v_knife_tactical_candy.mdl" );
			int rifle = g_MdlInfo->GetModelIndex( "models/weapons/v_smg_mp5.mdl" );

			if ( wep->IsKnife( ) )
			{
				wep->m_nModelIndex( ) = knife;
				wep->SetModelIndex( knife );
			} else
			{
				wep->m_nModelIndex( ) = rifle;
				wep->SetModelIndex( rifle );
			}


			//	wep->m_nModelIndex() = knife;

			CHandle<C_BaseViewModel> viewmodel_handle = local->m_hViewModel( );

			if ( viewmodel_handle.m_Index == INVALID_EHANDLE_INDEX )
				return false;

			C_BaseViewModel* viewmodel = static_cast<C_BaseViewModel*>( entitylist->GetClientEntityFromHandle( viewmodel_handle ) );

			if ( !viewmodel ) return false;

			CHandle<C_BaseCombatWeapon> viewmodel_weapon_handle = viewmodel->m_hWeapon( );

			if ( viewmodel_weapon_handle.m_Index == INVALID_EHANDLE_INDEX )
				return false;

			C_BaseCombatWeapon* viewmodel_weapon = static_cast<C_BaseCombatWeapon*>( entitylist->GetClientEntityFromHandle( viewmodel_weapon_handle ) );

			if ( !viewmodel_weapon ) return false;

			if ( viewmodel_weapon == wep )
			{
				//fix using chameleon shit i guess?
				if ( wep->IsKnife( ) )
				{
					viewmodel->m_nModelIndex( ) = knife;
					viewmodel_weapon->SetModelIndex( knife );
					viewmodel_weapon->m_nModelIndex( ) = knife;
					wep->m_nModelIndex( ) = knife;
					wep->SetModelIndex( knife );
				}

				else if ( wep->IsSMG( ) )
				{
					viewmodel->m_nModelIndex( ) = rifle;
					viewmodel_weapon->SetModelIndex( rifle );
					viewmodel_weapon->m_nModelIndex( ) = rifle;

					wep->m_nModelIndex( ) = rifle;
					wep->SetModelIndex( rifle );
				}

			}
			//	viewmodel_weapon->SetModelIndex(g_MdlInfo->GetModelIndex("models/weapons/eminem/v_knife_tactical_candy.mdl"));
		}

		//wep->m_OriginalOwnerXuidHigh() = -1;
		return true;
	}

	int coordSwitch( )
	{
		static clock_t start_t = clock( );
		static int rotation;
		double timeSoFar = (double)( clock( ) - start_t ) / CLOCKS_PER_SEC;
		if ( timeSoFar < 0.01 )
			return rotation;

		if ( rotation > 89 )  rotation = 0;
		rotation++;

		start_t = clock( );
		return rotation;
	}

	bool __fastcall hkShouldDrawCrosshair( void *pEcx, void *pEdx ) //28
	{
		auto oShouldDrawCrosshair = clientmode_hook.get_original<ShouldDrawCrosshair>( 28 );

		if ( options.misc_sniper_crosshair && local )
		{
			C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)g_EntityList->GetClientEntityFromHandle( local->m_hActiveWeapon( ) );
			if ( pWeapon )
			{
				CCSWeaponInfo* info = pWeapon->GetCSWeaponData( );
				if ( info && info->m_WeaponType == WEAPONTYPE_SNIPER_RIFLE )
				{
					info->m_WeaponType = WEAPONTYPE_RIFLE;
					pWeapon->DrawCrosshair( );
					info->m_WeaponType = WEAPONTYPE_SNIPER_RIFLE;
				}
			}
		}
		return oShouldDrawCrosshair( pEcx );
	}

	typedef void( __thiscall* SetKeyCodeState_t ) ( void*, ButtonCode_t, bool );
	typedef void( __thiscall* setmousecodestate_t ) ( void*, ButtonCode_t, MouseCodeState_t );


	void __fastcall SetKeyCodeState_h( void* thisptr, void* EDX, ButtonCode_t code, bool bDown )
	{
		auto o_SetKeyCodeState = inputinternal_hook.get_original<SetKeyCodeState_t>( index::SetKeyCodeState );

		if ( Hooks::inputlisten && bDown )
		{
			Hooks::inputlisten = false;
			if ( input_receivedKeyval )
			{
				Menu::get( ).selecting_key = false;
				*input_receivedKeyval = code;
			}
		}
		return o_SetKeyCodeState( thisptr, code, bDown );
	}

	EGCResults __fastcall retrive_message_hooked( void* ecx, void* edx, uint32_t *punMsgType, void *pubDest, uint32_t cubDest, uint32_t *pcubMsgSize )
	{
		auto original = gamecoordinator_hook.get_original<RetrieveMessageFn>( 2 );

		EGCResults status = original( ecx, punMsgType, pubDest, cubDest, pcubMsgSize );
	
		if ( status != EGCResults::k_EGCResultOK )
			return status;

		uint32_t messageType = *punMsgType & 0x7FFFFFFF;

		proto::get( ).retrieve( punMsgType, pubDest, cubDest, pcubMsgSize );


		ConsolePrint( "got gc [%d]!\n", messageType );

		return status;
	}

	EGCResults __fastcall send_message_hooked( void* ecx, void* edx, uint32_t unMsgType, const void* pubData, uint32_t cubData )
	{
		uint32_t messageType = unMsgType & 0x7FFFFFFF;
		void* pubDataMutable = const_cast<void*>( pubData );
		proto::get().send( unMsgType, pubDataMutable, cubData );

		auto original = gamecoordinator_hook.get_original<SendMessageFn>( 0 );

		EGCResults status = original( ecx, unMsgType, pubDataMutable, cubData );

		if ( status != EGCResults::k_EGCResultOK )
			return status;

		ConsolePrint( "sent gc [%d]!\n", messageType );

		return status;
	}
	
	typedef int( __stdcall* IsBoxVisibleFn )( const Vector&, const Vector& );
	int	__stdcall isboxvisible_hooked( const Vector& mins, const Vector& maxs )
	{
		auto o_isboxvisibile = engine_hook.get_original<IsBoxVisibleFn>( 32 );
	
		if ( !memcmp( _ReturnAddress( ), "\x85\xC0\x74\x2D\x83\x7D\x10\x00\x75\x1C", 10 ) )
		{
			return 1;
		}
			
		return o_isboxvisibile( mins, maxs );
	}


	
	enum ClearFlags_t
	{
		VIEW_CLEAR_COLOR = 0x1,
		VIEW_CLEAR_DEPTH = 0x2,
		VIEW_CLEAR_FULL_TARGET = 0x4,
		VIEW_NO_DRAW = 0x8,
		VIEW_CLEAR_OBEY_STENCIL = 0x10, // Draws a quad allowing stencil test to clear through portals
		VIEW_CLEAR_STENCIL = 0x20,
	};
	
	void __fastcall renderview_hooked( void* ecx, void* edx,  CViewSetup &view,  CViewSetup &hudViewSetup, int nClearFlags, int whatToDraw )
	{
		auto o_renderview = viewrender_hook.get_original<renderview_fn>( 6 );

		if ( engine->IsInGame( ) )
		{
			if ( options.asd )
			{
				static ITexture* texture = nullptr;
				if ( !texture )
				{
					g_MatSystem->forceBeginRenderTargetAllocation( );
					texture = g_MatSystem->createFullFrameRenderTarget( "mirrorcam_rt" );
					g_MatSystem->forceEndRenderTargetAllocation( );
				}


				QAngle a;
				g_EngineClient->GetViewAngles( a );
				a.yaw += 180.f;
				Utils::Clamp( a );
				CViewSetup mirrorView = view;
				mirrorView.x = mirrorView.x_old = 0;
				mirrorView.y = mirrorView.y_old = 0;
				mirrorView.width = mirrorView.width_old = options.mirror_w;
				mirrorView.height = mirrorView.height_old = options.mirror_h;
				mirrorView.angles = Math::QAngleToVector( a );
				mirrorView.m_flAspectRatio = float( mirrorView.width ) / float( mirrorView.height );

				auto renderCtx = g_MatSystem->GetRenderContext( );

				renderCtx->PushRenderTargetAndViewport( );
				renderCtx->SetRenderTarget( texture );

				o_renderview( ecx, mirrorView, mirrorView, VIEW_CLEAR_COLOR | VIEW_CLEAR_DEPTH | VIEW_CLEAR_STENCIL, 0 );

				renderCtx->PopRenderTargetAndViewport( );
				renderCtx->Release( );

				o_renderview( ecx, view, hudViewSetup, nClearFlags, whatToDraw );

				static IMaterial* mat = g_MatSystem->FindMaterial( "mirrorcam_mat", TEXTURE_GROUP_OTHER );

				if ( !mat )
				{
					ConsolePrint( "not found,,, \n" );
				} else
				{
					auto renderCtx = g_MatSystem->GetRenderContext( );

					renderCtx->DrawScreenSpaceRectangle(
						mat,
						options.mirror_x, options.mirror_y, options.mirror_w, options.mirror_h,
						0, 0, float( options.mirror_w ), float( options.mirror_h ),
						texture->GetActualWidth( ), texture->GetActualHeight( ),
						nullptr, 1, 1
					);
					renderCtx->Release( );
				}
			}
			else 
				o_renderview( ecx, view, hudViewSetup, nClearFlags, whatToDraw );
	
		}
				
	}
	void Initialize( )
	{
		direct3d_hook.setup( g_D3DDevice9 );
		hlclient_hook.setup( g_CHLClient );
		vguipanel_hook.setup( g_VGuiPanel );
		vguisurf_hook.setup( g_VGuiSurface );
		mdlrender_hook.setup( g_MdlRender );
		clientmode_hook.setup( g_ClientMode );
		engine_hook.setup( engine );
		mdlcache_hook.setup( g_MdlCache );
		material_hook.setup( g_MatSystem );
		renderview_hook.setup( g_RenderView );
		input_hook.setup( g_Input );
		inputinternal_hook.setup( g_InputInternal );
		gamecoordinator_hook.setup( SteamGameCoordinator );
		viewrender_hook.setup( g_ViewRender );
		
		input_hook.hook_index( index::CAM_FP, hkCAM_FP );
		
		direct3d_hook.hook_index( index::EndScene, hkEndScene );
		direct3d_hook.hook_index( index::Reset, hkReset );

		renderview_hook.hook_index( index::DrawLights, hkDrawLights );
		renderview_hook.hook_index( index::SceneEnd, hkSceneEnd );

		hlclient_hook.hook_index( index::FrameStageNotify, hkFrameStageNotify );
		hlclient_hook.hook_index( index::CreateMove, hook_createmove_proxy );
		
		material_hook.hook_index( index::OverrideConfig, hkOverrideConfig );
		
		vguipanel_hook.hook_index( index::PaintTraverse, hkPaintTraverse );
		
		mdlcache_hook.hook_index( index::FindModel, hkFindModel );

		vguisurf_hook.hook_index( index::playSound, hkPlaySound );

		mdlrender_hook.hook_index( index::DrawModelExecute, hkDrawModelExecute );

		clientmode_hook.hook_index( index::OverrideView, hkOverrideView );
		clientmode_hook.hook_index( index::DoPostScreenSpaceEffects, hkDoPostScreenEffects );
		clientmode_hook.hook_index( index::shouldDrawCrosshair, hkShouldDrawCrosshair ); 
	
		engine_hook.hook_index( index::GetPlayerInfo, hkGetPlayerInfo );
		engine_hook.hook_index( index::IsPlayingTimeDemo, hkIsPlayingTimeDemo );
		engine_hook.hook_index( index::IsConnected, hkIsConnected );
		engine_hook.hook_index( 32, isboxvisible_hooked );

		inputinternal_hook.hook_index( index::SetKeyCodeState, SetKeyCodeState_h ); 

		gamecoordinator_hook.hook_index( 0, send_message_hooked );
		gamecoordinator_hook.hook_index( 2, retrive_message_hooked );

		viewrender_hook.hook_index( 6, renderview_hooked );

		srand( time( 0 ) );
		 
		static const int max_skin = 706; //TODO: make automatical

		for ( ItemDefinitionIndex enumz = ItemDefinitionIndex::ITEM_NONE;
			  enumz != ItemDefinitionIndex::WEAPON_MAX;
			  enumz = static_cast<ItemDefinitionIndex>( static_cast<int>( enumz ) + 1 ) )
		{

			scf[ static_cast<int>( enumz ) ].nFallbackPaintKit = rand( ) % max_skin;
			scf[ static_cast<int>( enumz ) ].nFallbackStatTrak = 1337;

		}

		Visuals::CreateFonts( );

	    listener = new event_listener( );

		g_GameEvents->AddListener( listener, "player_death", false );
		g_GameEvents->AddListener( listener, "player_hurt", false );
		g_GameEvents->AddListener( listener, "round_start", false );

		for ( int i = 1; i <= 65; i++ )
		{
			pResolver2::array_[ i ].method_use = -1;
			pResolver2::array_[ i ].res_state = pResolver::Lowerbody;
			pResolver2::array_[ i ].count = -1;
			pResolver2::array_[ i ].yaw = 0.f;
			pResolver2::array_[ i ].aimbot_target = 0;
		}

		g_EngineModule =  GetModuleHandleA( "engine.dll" );
		g_ClientModule = GetModuleHandleA ( "client.dll" );

		engine->GetScreenSize( width, height ); 

		proto::get( ).SendMatchmakingHelloToServer( );
	}

	void __fastcall hkCAM_FP( void* thisptr, void* )
	{
		auto oCAM_ToFirstPerson = input_hook.get_original<CAM_ToFirstPerson_fn>( index::CAM_FP );

		if ( options.misc_thirdperson_enabled && options.misc_thirdperson_enabled2 && options.misc_enabled && local )
			return; //prevent game from putting us back to firstperson (this fixes the flickering shit)

		return oCAM_ToFirstPerson( thisptr );
	}
	IMatRenderContext* context2;

	void __fastcall  hkSceneEnd( void *pEcx, void *pEdx )
	{
		auto oSceneEnd = renderview_hook.get_original<SceneEnd>( index::SceneEnd );

		oSceneEnd( pEcx );

		//TODO: implement proper options for this
		if ( local && local->IsAlive( ) && *(bool*)( (DWORD)g_Input + 0xA5 ) && chams::get( ).materialRegular )
		{

			QAngle og = local->m_angEyeAngles( );
			Vector og_pos = local->GetRenderOrigin( );
			float rotation = Utils::FakeChamsFix( local->getanimstate( ) );
			IMaterial* mat = chams::get( ).materialRegular;

			local->SetAbsOrigin( fakeanglesOrigin );

			if ( LBYok )
			{
				mat->ColorModulate(
					options.color_fakechams_lby.r( ) / 255.f,
					options.color_fakechams_lby.g( ) / 255.f,
					options.color_fakechams_lby.b( ) / 255.f );


				local->SetAbsAngles( Vector( 0, g_Local->lby() - rotation, 0 ) );
				mdlrender->ForcedMaterialOverride( mat );
				local->DrawModel2( 255 );
			}

			local->SetAbsAngles( Vector( 0, fakeyaw - rotation, 0 ) );
			mat->ColorModulate(
				options.color_fakechams_fake.r( ) / 255.f,
				options.color_fakechams_fake.g( ) / 255.f,
				options.color_fakechams_fake.b( ) / 255.f );
			mdlrender->ForcedMaterialOverride( mat );
			local->DrawModel2( 255 );

			mdlrender->ForcedMaterialOverride( nullptr );
			local->SetAbsAngles( Vector( 0, realyaw, 0 ) );
			local->SetAbsOrigin( og_pos );
		}

		if ( options.esp_player_outlines && !options.chams_player_enabled )
		{
			if ( context2 )
			{
				static StencilState sten;
				static bool stencil_initialized = false;

				if ( !stencil_initialized )
				{
					sten.ref = 1;
					sten.pass = 3;
					sten.fail = 1;
					sten.zfail = 3;
					sten.mask1 = 0xFFFFFFFF;
					sten.mask2 = 0xFFFFFFFF;
					stencil_initialized = true;
				}

				sten.enable = true;
				sten.compare = 8;

				context2->SetStencilState( sten );
				renderview->SetBlend( 0 );

				for ( int i = 1; i < globals->maxClients; ++i )
				{
					if ( i < 65 )
					{
						auto ent = (C_BasePlayer*)entlist->GetClientEntity( i );

						if ( !ent || !ent->isvalidtarget( ) )
							continue;

						bool isEnemy = g_Local->m_iTeamNum( ) != ent->m_iTeamNum( );

						if ( !isEnemy && options.esp_enemies_only )
							continue;

						if ( ent && ent->isvalidtarget( ) )
							ent->DrawModel2( 255 );
					}
				}

				renderview->SetBlend( 1 );

				sten.compare = 6;
				context2->SetStencilState( sten );

				static SpoofedConvar* r_drawothermodels = new SpoofedConvar( "r_drawothermodels" );
				r_drawothermodels->SetInt( 2 );

				for ( int i = 1; i < globals->maxClients; ++i )
				{
					auto ent = (C_BasePlayer*)entlist->GetClientEntity( i );

					if ( ent && ent->isvalidtarget( ) )
					{
						bool isEnemy = g_Local->m_iTeamNum( ) != ent->m_iTeamNum( );

						if ( !isEnemy && options.esp_enemies_only )
							continue;

						bool is_visible = g_Local->CanSeePlayer( ent, HITBOX_CHEST );

						Color col = isEnemy ? ( is_visible ? g_Options.color_esp_enemy_visible : g_Options.color_esp_enemy_occluded ) : ( is_visible ? g_Options.color_esp_ally_visible : g_Options.color_esp_ally_occluded );

						if ( g_Options.highlighted_meme == ent->EntIndex( ) )
							col = Color( 0, 128, 128 );

						renderview->SetColorModulation( col.r( ) / 255.f, col.g( ) / 255.f, col.b( ) / 255.f );
						ent->DrawModel2( col.a() );
					}
				}

				r_drawothermodels->SetInt( 1 );
				sten.enable = false;
				context2->SetStencilState( sten );
			}
		}

		if ( GetAsyncKeyState( VK_F2 ) ) //TODO: implement proper chams later (made dme but make both options and apply the multicolor chams havent seen them elsewhere)
		{
			for ( int i = 1; i < globals->maxClients; ++i )
			{
				auto ent = (C_BasePlayer*)entlist->GetClientEntity( i );

				if ( ent && ent->isvalidtarget( ) && chams::get( ).materialRegular )
				{
					mdlrender->ForcedMaterialOverride( chams::get( ).materialRegular );
					ent->DrawModel2( 255 );
					mdlrender->ForcedMaterialOverride( nullptr );
				}
			}
		}
	}

	int  __fastcall SendDatagram( void* netchan, void*, void *datagram )
	{
		auto oSendDatagram = netchannel_hook.get_original<SendDatagramFn>( 48 );

		if ( !options.fake_latency ) return oSendDatagram( netchan, datagram );

		INetChannel *netchannel = (INetChannel*)netchan;

		fake_latency::get( ).run( netchannel );

		int ret = oSendDatagram( netchan, datagram );

		netchannel->m_nInReliableState = fake_latency::get( ).instate;
		netchannel->m_nInSequenceNr = fake_latency::get( ).insequencenr;

		return ret;
	}

	bool __fastcall hkIsConnected( void* ecx, void* edx )
	{
		auto oIsConnected = engine_hook.get_original<IsConnected>( index::IsConnected );

		if ( options.misc_invenabler )
		{
			static void* unk = Utils::PatternScan(g_ClientModule , "75 04 B0 01 5F" ) - 2;
			if ( _ReturnAddress( ) == unk )
				return false;
			

		}

		return oIsConnected( ecx );
	}

	void __fastcall hkDrawLights( void* thisptr, void* edx )
	{
		auto oDrawLights = renderview_hook.get_original<DrawLights>( index::DrawLights );

		if ( options.worldlights_enabled )
		{


			static CHostState* hoststate;

			if ( !hoststate ) hoststate = **(CHostState***)( Utils::PatternScan( g_EngineModule, "A1 ? ? ? ? 83 EC 08 56 BE ? ? ? ?" ) + 1 );

			if ( hoststate )
			{
				for ( int i = 0; i < hoststate->numworldlights; i++ )
				{
					dworldlight_t* pLight = &hoststate->worldlights[ i ];

					if ( !pLight )
						continue;

					static const float convert_rate = 588.23f;
					static float red = 0.f;
					static float green = 0.f;
					static float blue = 0.f;
					static Color oldColor = Color::Red;

					if ( oldColor != options.worldlights )
					{
						oldColor = options.worldlights;
						red = oldColor.r( ) * convert_rate;
						green = oldColor.g( ) * convert_rate;
						blue = oldColor.b( ) * convert_rate;
					}

					pLight->intensity = Vector( red, green, blue ); //red green blue		
				}

			} else
				Utils::ConsolePrint( "Hoststate is null, update offset" );
		}
		oDrawLights( thisptr );
	}

	bool __fastcall hkIsPlayingTimeDemo( void* thisptr, void* edx )
	{
		auto oIsPlayingTimeDemo = engine_hook.get_original<IsPlayingTimeDemo>( index::IsPlayingTimeDemo );

		if ( options.aim_nointerp && engine->IsInGame( ) && engine->IsConnected( ) && g_Local && g_Local->IsAlive( ) && !g_Local->m_InBuyZone( ) )
			return true;

		return oIsPlayingTimeDemo( thisptr );
	}

	bool __fastcall hkOverrideConfig( void* ecx, void* edx, MaterialSystem_Config_t* config, bool force )
	{
		auto oOverrideConfig = material_hook.get_original<OverrideConfig>( index::OverrideConfig );

		if ( options.vis_overridecfg )
		{
			config->m_nShowMipLevels = options.viso_mipmap;
			config->m_nFullbright = options.viso_fullbright;
			config->m_bDrawGray = options.viso_drawgray;
			config->m_bShowLowResImage = options.viso_lowres;
			config->m_bNoTransparency = options.viso_trans;
			config->m_bVisualizeFillRate = options.viso_visf;
			config->m_bMeasureFillRate = options.viso_meaf;
			config->m_bSuppressRendering = options.viso_suppressrend;
			config->m_bReverseDepth = options.viso_revd;
		}

		return oOverrideConfig( ecx, edx, config, force );
	}

	MDLHandle_t __fastcall hkFindModel( void* ecx, void* edx, const char* ptr )
	{
		auto oFindModel = mdlcache_hook.get_original<FindModel>( index::FindModel );
		return oFindModel( ecx, edx, ptr );
	}

	bool __fastcall hkGetPlayerInfo( void* ecx, void* edx, int ent_num, player_info_t *pinfo )
	{
		auto oGetPlayerInfo = engine_hook.get_original<GetPlayerInfo>( index::GetPlayerInfo );

		bool ret = oGetPlayerInfo( ecx, edx, ent_num, pinfo );
		pinfo->fakeplayer = false; //Remove BOT

		if ( options.clientside_name )
		{
			if ( options.clientside_pic_bool &&  clientpic && sizeof( clientpic ) >= 18 )
			{
				std::string::size_type sz = 0;
				pinfo->steamID64 = std::stoll( clientpic, &sz);
			}
			if ( (C_BasePlayer*)g_Local == (C_BasePlayer*)g_EntityList->GetClientEntity( ent_num ) )
			{
				if ( options.clientside_local_bool )
					strcpy( pinfo->szName, clientname );
			} else if ( options.clientside_hide )
				strcpy( pinfo->szName, clientname_other );
		}

		return ret;
	}

	//TODO: add fake stats
	long __stdcall hkEndScene( IDirect3DDevice9* device )
	{
		auto oEndScene = direct3d_hook.get_original<EndScene>( index::EndScene );

		static auto viewmodel_fov = g_CVar->FindVar( "viewmodel_fov" );
		static auto viewmodel_offset_x = g_CVar->FindVar( "viewmodel_offset_x" );
		static auto viewmodel_offset_y = g_CVar->FindVar( "viewmodel_offset_y" );
		static auto viewmodel_offset_z = g_CVar->FindVar( "viewmodel_offset_z" );
		static auto mat_ambient_light_r = g_CVar->FindVar( "mat_ambient_light_r" );
		static auto mat_ambient_light_g = g_CVar->FindVar( "mat_ambient_light_g" );
		static auto mat_ambient_light_b = g_CVar->FindVar( "mat_ambient_light_b" );
		static auto crosshair_cvar = g_CVar->FindVar( "crosshair" );
		static auto zoom_sensitivity_ratio_mouse = cvars->FindVar( "zoom_sensitivity_ratio_mouse" );

		if ( options.misc_enabled )
		{
			viewmodel_fov->m_fnChangeCallbacks.m_Size = 0;
			viewmodel_fov->SetValue( 60.f + options.viewmodel_fov );

			if ( options.misc_fakezoom )
			{
				static Vector pBackup( 0.f, 0.f, 0.f );
				static bool backed_up = false;

				if ( fake_scoped )
				{
					if ( !backed_up )
					{
						pBackup = Vector( viewmodel_offset_x->GetFloat( ), viewmodel_offset_y->GetFloat( ), viewmodel_offset_z->GetFloat( ) );
						viewmodel_offset_x->SetValue( -2.f );
						viewmodel_offset_y->SetValue( -1.f );
						viewmodel_offset_z->SetValue( 2.f );
						backed_up = true;
					}
					viewmodel_fov->SetValue( 77.f ); 
				} else if ( backed_up )
				{
					viewmodel_offset_x->SetValue( pBackup.x );
					viewmodel_offset_y->SetValue( pBackup.y );
					viewmodel_offset_z->SetValue( pBackup.z );
					backed_up = false;
				}

			}

			static float zoom_original = zoom_sensitivity_ratio_mouse->GetFloat( );
			if ( g_Local && g_Local->m_bIsScoped( ) )
				zoom_sensitivity_ratio_mouse->SetValue( 0.f );
			else
				zoom_sensitivity_ratio_mouse->SetValue( zoom_original );
		}
		if ( options.aimbient_lighting  && options.vis_misc_enabled )
		{
			mat_ambient_light_r->SetValue( (float)( (float)options.mat_ambient.r( ) / 255.f ) );
			mat_ambient_light_g->SetValue( (float)( (float)options.mat_ambient.g( ) / 255.f ) );
			mat_ambient_light_b->SetValue( (float)( (float)options.mat_ambient.b( ) / 255.f ) );
		}

		crosshair_cvar->SetValue( !options.esp_crosshair );

		Menu::get( ).Render( );

		if ( InputSys::get( ).IsKeyDown( VK_TAB ) && options.misc_rank_reveal && options.misc_enabled )
			Utils::RankRevealAll( );

		return oEndScene( device );
	}

	void __stdcall hkOverrideView( CViewSetup* vsview )
	{
		auto oOverrideView = clientmode_hook.get_original<OverrideView>( index::OverrideView );

		if ( engine->IsInGame( ) && g_Local && options.misc_enabled )
		{
			if ( ( !options.misc_fov_sniper && !g_Local->m_bIsScoped( ) ) || options.misc_fov_sniper )
				vsview->fov = 90.f + options.fov;

			if ( options.misc_remove_blur && g_Local->m_bIsScoped( ) )
			{
				if ( g_Weapon && g_Weapon->GetCSWeaponData( )->m_WeaponType == WEAPONTYPE_RIFLE ) //scoped & rifle = SG or AUG
					*(float*)( (uintptr_t)g_Weapon + 0x3330 ) = 2; //m_iZoomlevel or m_flConstraintRadius 
			}
	
			if ( options.misc_fakezoom )
			{
				static C_BaseCombatWeapon* scope_wpn = nullptr;

				if ( !g_Local->m_bIsScoped( ) && keyboard::get( ).key_released( MOUSE_RIGHT ) )
				{
					if ( g_Weapon && !g_Weapon->IsSniper( ) && !g_Weapon->IsKnife( ) && g_Weapon->m_iItemDefinitionIndex( ) != ItemDefinitionIndex::WEAPON_SG553 && g_Weapon->m_iItemDefinitionIndex( ) != ItemDefinitionIndex::WEAPON_AUG )
					{
						fake_scoped = !fake_scoped;
						scope_wpn = g_Weapon;
					}
				}
				if ( scope_wpn && scope_wpn != g_Weapon )
				{
					scope_wpn = nullptr;
					fake_scoped = false;
				}

				if ( fake_scoped )
					vsview->fov -= 49.f; 
			}

			freecam::get( ).override_view( vsview );
			thirdperson::get( ).on_overrideview( );

		}
		oOverrideView( vsview );
	}

	long __stdcall hkReset( IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters )
	{
		auto oReset = direct3d_hook.get_original<Reset>( index::Reset );

		Visuals::DestroyFonts( );
		Menu::get( ).OnDeviceLost( );

		auto hr = oReset( device, pPresentationParameters );

		if ( hr >= 0 )
		{
			Menu::get( ).OnDeviceReset( );
			Visuals::CreateFonts( );

			ConsolePrint( "size changed... old %i %i", height, width );
			engine->GetScreenSize( width, height );
			ConsolePrint( " new %i %i \n", height, width );
		}

		return hr;
	}

	bool islocked = false;
	int targetid = -1;

	float GetHitgroupDamageMultiplier( int hitgroup )
	{
		switch ( hitgroup )
		{
			case HITBOX_HEAD:
					return 4.0f;
			case HITBOX_STOMACH:
					return 1.25f;
			case HITBOX_RIGHT_FOOT:
			case HITBOX_LEFT_FOOT:
			case HITBOX_LEFT_CALF:
			case HITBOX_RIGHT_CALF:
					return 0.75f;
			default:
					return 1.0f;
		}
	}

	float GetDamageToEnemy( C_BasePlayer* pTarget, int hitboxz , matrix3x4_t* matrix = nullptr, mstudiohitboxset_t * set = nullptr)
	{
		auto data = g_Weapon->GetCSWeaponData( );
		int health = pTarget->m_iHealth( );
		Vector hitbox_pos;
		if ( matrix && set )
			hitbox_pos = pTarget->GetHitboxPosFPS( matrix, set, hitboxz );
		else
			hitbox_pos = pTarget->GetHitboxPos( hitboxz );

		if ( g_Local->CanSeePlayer( pTarget, hitbox_pos ) )
		{
			float damage = (float)( data->iDamage );
			damage *= GetHitgroupDamageMultiplier( hitboxz );

			if ( pTarget->m_ArmorValue( ) > 0 )
			{
				if ( hitboxz == HITBOX_HEAD )
				{
					if ( pTarget->m_bHasHelmet( ) )
						damage *= ( data->flArmorRatio * 0.5f );
				} else
					damage *= ( data->flArmorRatio * 0.5f );
			}

			if ( damage < health )
			{
				int dmg_needed = options.min_dmg;

				if ( dmg_needed < 0 )
					dmg_needed = 15;
				
				if ( damage >= dmg_needed )
					return damage;
			} else
				return damage;
		} else if ( options.autowall )
		{
			int damage = (int)Awall( ).GetDamage( hitbox_pos );
			if ( damage < health )
			{
				if ( damage >= options.min_dmgawall )
					return damage;
			} else
				return damage;
		}
		return 0.0f;
	}
	float pred_curtime;
	bool canopenfire( )
	{
		float flServerTime = g_Local->m_nTickBase( ) * global->interval_per_tick;
		return !( g_Weapon->m_flNextPrimaryAttack( ) > flServerTime );
	}

	int hitbox = 0;
	int Hitscan( C_BasePlayer* ent )
	{
		std::vector<int> hitboxes;

		hitboxes.push_back( options.hit_priority );

		if ( options.hit_head && options.hit_priority != Hitboxes::HITBOX_HEAD )
			hitboxes.push_back( Hitboxes::HITBOX_HEAD );

		
		if ( options.hit_neck && options.hit_priority != Hitboxes::HITBOX_NECK )
				hitboxes.push_back( Hitboxes::HITBOX_NECK );
		

		if ( options.hit_chest )
		{
			hitboxes.push_back( Hitboxes::HITBOX_UPPER_CHEST );
			if ( options.hit_priority != Hitboxes::HITBOX_CHEST )
				hitboxes.push_back( Hitboxes::HITBOX_CHEST );
			hitboxes.push_back( Hitboxes::HITBOX_LOWER_CHEST );
		}

		if ( options.hit_stomach && options.hit_priority != Hitboxes::HITBOX_STOMACH )
			hitboxes.push_back( Hitboxes::HITBOX_STOMACH );

		if ( options.hit_pelvis && options.hit_priority != Hitboxes::HITBOX_PELVIS )
			hitboxes.push_back( Hitboxes::HITBOX_PELVIS );

		if ( options.hit_arms )
		{
			hitboxes.push_back( Hitboxes::HITBOX_LEFT_UPPER_ARM );
			hitboxes.push_back( Hitboxes::HITBOX_RIGHT_UPPER_ARM );
			hitboxes.push_back( Hitboxes::HITBOX_RIGHT_FOREARM );
			hitboxes.push_back( Hitboxes::HITBOX_LEFT_FOREARM );
		}

		if ( options.hit_legs )
		{
			hitboxes.push_back( Hitboxes::HITBOX_LEFT_THIGH );
			hitboxes.push_back( Hitboxes::HITBOX_RIGHT_THIGH );
			hitboxes.push_back( Hitboxes::HITBOX_LEFT_CALF );
			hitboxes.push_back( Hitboxes::HITBOX_RIGHT_CALF );
		}

		if ( options.hit_hands )
		{
			hitboxes.push_back( Hitboxes::HITBOX_LEFT_HAND );
			hitboxes.push_back( Hitboxes::HITBOX_RIGHT_HAND );
		}

		if ( options.hit_feet )
		{
			hitboxes.push_back( Hitboxes::HITBOX_RIGHT_FOOT );
			hitboxes.push_back( Hitboxes::HITBOX_LEFT_FOOT );
		}

		matrix3x4_t matrix[ MAXSTUDIOBONES ];
		if ( ent->SetupBones( matrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0.0f ) )
		{
			auto studio_model = g_MdlInfo->GetStudiomodel( ent->GetModel( ) );
			if ( studio_model )
			{
				auto set = studio_model->GetHitboxSet( 0 );
				for each( int hitbox in hitboxes )
				{
					if ( GetDamageToEnemy( ent, hitbox, matrix, set ) <= 0.0f )
						continue;

					return hitbox;
				}
			}
			
		}

		return -1;
	}


	int HitScanSmart( C_BasePlayer* ent )
	{
		std::vector<int> ScanBoxes;

		ScanBoxes.push_back( options.hit_priority );

		if ( options.hit_head && options.hit_priority != HITBOX_HEAD )
			ScanBoxes.push_back( (int)Hitboxes::HITBOX_HEAD );

		if ( options.hit_neck && options.hit_priority != HITBOX_NECK )
				ScanBoxes.push_back( (int)Hitboxes::HITBOX_NECK );
		
		if ( options.hit_chest )
		{
			ScanBoxes.push_back( (int)Hitboxes::HITBOX_UPPER_CHEST );
			ScanBoxes.push_back( (int)Hitboxes::HITBOX_CHEST );
			ScanBoxes.push_back( (int)Hitboxes::HITBOX_LOWER_CHEST );
		}
		if ( options.hit_stomach )
			ScanBoxes.push_back( (int)Hitboxes::HITBOX_STOMACH );

		if ( options.hit_pelvis )
			ScanBoxes.push_back( (int)Hitboxes::HITBOX_PELVIS );

		if ( options.hit_arms )
		{
			ScanBoxes.push_back( (int)Hitboxes::HITBOX_LEFT_UPPER_ARM );
			ScanBoxes.push_back( (int)Hitboxes::HITBOX_RIGHT_UPPER_ARM );
			ScanBoxes.push_back( (int)Hitboxes::HITBOX_RIGHT_FOREARM );
			ScanBoxes.push_back( (int)Hitboxes::HITBOX_LEFT_FOREARM );

		}
		if ( options.hit_legs )
		{
			ScanBoxes.push_back( (int)Hitboxes::HITBOX_LEFT_THIGH );
			ScanBoxes.push_back( (int)Hitboxes::HITBOX_RIGHT_THIGH );
			ScanBoxes.push_back( (int)Hitboxes::HITBOX_LEFT_CALF );
			ScanBoxes.push_back( (int)Hitboxes::HITBOX_RIGHT_CALF );
		}
		if ( options.hit_hands )
		{
			ScanBoxes.push_back( (int)Hitboxes::HITBOX_LEFT_HAND );
			ScanBoxes.push_back( (int)Hitboxes::HITBOX_RIGHT_HAND );
		}
		if ( options.hit_feet )
		{
			ScanBoxes.push_back( (int)Hitboxes::HITBOX_RIGHT_FOOT );
			ScanBoxes.push_back( (int)Hitboxes::HITBOX_LEFT_FOOT );
		}

		for ( int hitbox : ScanBoxes )
		{
			Vector hitboxz;
			ent->GetHitboxPos( hitbox, hitboxz );
			if ( g_Local->CanSeePlayer( ent, hitboxz ) )
			{

				if ( options.baim_lethal )
				{

					if ( g_Local->CanSeePlayer( ent, HITBOX_PELVIS ) )
					{
						int damagepelvis = (int)GetDamageToEnemy( ent, HITBOX_PELVIS );
						if ( ( ent->m_iHealth( ) ) <= (int)damagepelvis )
						{
							hitbox = HITBOX_PELVIS;
						}
					} else if ( g_Local->CanSeePlayer( ent, HITBOX_STOMACH ) )
					{
						int damagepelvis = (int)GetDamageToEnemy( ent, HITBOX_STOMACH );
						if ( ( ent->m_iHealth( ) ) <= (int)damagepelvis )
						{
							hitbox = HITBOX_STOMACH;
						}
					}

				}

				//baim if lower
				if ( ent->m_iHealth( ) <= options.aim_baimlower )
				{
					if ( g_Local->CanSeePlayer( ent, HITBOX_PELVIS ) )
						hitbox = HITBOX_PELVIS;
					else if ( g_Local->CanSeePlayer( ent, HITBOX_STOMACH ) )
						hitbox = HITBOX_STOMACH;
				}
				return hitbox;

			} else if ( options.autowall )
			{
				float damage = Awall( ).GetDamage( hitboxz );
				if ( (int)damage >= options.min_dmgawall )
				{
					if ( options.baim_lethal )
					{
						int damagepelvis = (int)Awall( ).GetDamage( ent->GetHitboxPos( HITBOX_PELVIS ) );
						if ( ( ent->m_iHealth( ) ) <= (int)damagepelvis )
						{
							hitbox = HITBOX_PELVIS;
						} else
						{
							int damagepelvis = (int)Awall( ).GetDamage( ent->GetHitboxPos( HITBOX_STOMACH ) );
							if ( ( ent->m_iHealth( ) ) <= (int)damagepelvis )
							{
								hitbox = HITBOX_STOMACH;
							}
						}
					}

					if ( ent->m_iHealth( ) <= options.aim_baimlower )
					{
						int damagepelvis = (int)Awall( ).GetDamage( ent->GetHitboxPos( HITBOX_PELVIS ) );
						if ( damagepelvis >= options.min_dmgawall )
							hitbox = HITBOX_PELVIS;
						else
						{
							int damagepelvis = (int)Awall( ).GetDamage( ent->GetHitboxPos( HITBOX_STOMACH ) );
							if ( damagepelvis >= options.min_dmgawall )
								hitbox = HITBOX_STOMACH;
						}
					}
					return hitbox;
				}
			}
		}
		return -1;
	}

	//TODO: make more optimized some day
	bool TargetIsValid( C_BasePlayer* pEntity )
	{
		if (
			pEntity && pEntity->IsPlayer( ) && !pEntity->IsDormant( )
			&& pEntity->IsAlive( ) && pEntity->EntIndex( ) != g_Local->EntIndex( )
			&& pEntity->m_iTeamNum( ) != g_Local->m_iTeamNum( )
			&& !pEntity->m_bGunGameImmunity( )
			)
			return true;

		return false;
	}

	tuple<int, int> GetTargetDistance( )
	{
		int target = -1;
		int target_hitbox = -1;
		int minDist = 99999;

		for ( int i = 1; i < g_EntityList->GetHighestEntityIndex( ); i++ )
		{
			auto ent = (C_BasePlayer*)g_EntityList->GetClientEntity( i );

			if ( !TargetIsValid( ent ) )
				continue;

			int hitbox = Hitscan( ent );

			if ( hitbox < 0 )
				continue;

			if ( ent->FovToPlayer( hitbox ) > options.aim_fov )
				continue;

			int Distance = ( g_Local->m_vecOrigin( ) - ent->m_vecOrigin( ) ).Length( );
			if ( Distance < minDist )
			{
				minDist = Distance;
				target_hitbox = hitbox;
				target = i;
			}

		}

		return { target , target_hitbox };
	}

	tuple<int, int> GetFirstTarget( )
	{
		for ( int i = 1; i < g_EntityList->GetHighestEntityIndex( ); i++ )
		{
			auto ent = (C_BasePlayer*)g_EntityList->GetClientEntity( i );

			if ( !TargetIsValid( ent ) )
				continue;

			int hitbox = Hitscan( ent );

			if ( hitbox < 0 )
				continue;

			//TODO: More  GetHitboxPos fixes on fovtoplayer, use tuple return from hitscan maybe?)
			if ( ent->FovToPlayer( hitbox ) <= options.aim_fov )
				return { i, hitbox };
		}

		return { -1, -1 };
	}
	QAngle aim_angles;
	int HighestHitchance( C_BasePlayer* target, int hitchance )
	{
		constexpr auto max_traces = 256;

		if ( g_Local->m_vecVelocity( ).Length2D( ) > 1.f ) hitchance = options.aim_hitchance_move;

		int total_hits = 0;
		int needed_hits = static_cast<int>( max_traces * ( hitchance / 100.f ) );

		g_Weapon->UpdateAccuracyPenalty( );

		auto flRange = g_Weapon->GetCSWeaponData( )->flRange;

		for ( int i = 0; i < max_traces; i++ )
		{
			RandomSeed( i + 1 );

			float fRand1 = RandomFloat( 0.f, 1.f );
			float fRandPi1 = RandomFloat( 0.f, XM_2PI );
			float fRand2 = RandomFloat( 0.f, 1.f );
			float fRandPi2 = RandomFloat( 0.f, XM_2PI );

			float fRandInaccuracy = fRand1 * g_Weapon->GetInaccuracy( );
			float fRandSpread = fRand2 * g_Weapon->GetSpread( );

			float fSpreadX = cos( fRandPi1 ) * fRandInaccuracy + cos( fRandPi2 ) * fRandSpread; //cos(theta) = x
			float fSpreadY = sin( fRandPi1 ) * fRandInaccuracy + sin( fRandPi2 ) * fRandSpread; //sin(theta) = y

			auto viewSpreadForward = ( g_Cmd->viewangles.Forward( ) + fSpreadX * g_Cmd->viewangles.Right( ) + fSpreadY * g_Cmd->viewangles.Up( ) ).Normalized( );

			QAngle viewAnglesSpread;
			VectorAngles( viewSpreadForward, viewAnglesSpread );
			Utils::Normalize( viewAnglesSpread );

			Vector viewForward;
			AngleVectors( viewAnglesSpread, viewForward );
			viewForward.NormalizeInPlace( );

			viewForward = g_EyePos + ( viewForward * flRange );

			trace_t tr;
			Ray_t ray;
			ray.Init( g_EyePos, viewForward );

			g_EngineTrace->ClipRayToEntity( ray, MASK_SHOT | CONTENTS_GRATE, target, &tr );

			if ( tr.hit_entity == target )
				total_hits++;

			if ( ( max_traces - i + total_hits ) < needed_hits )
				return 0;
		}

		if ( total_hits >= needed_hits )
			return (int)( (float)total_hits / 2.56f );

		return 0;
	}

	bool hit_chance( C_BasePlayer* target, int hitchance )
	{
		static constexpr auto max_traces = 256;


		if ( g_Local->m_vecVelocity( ).Length2D( ) > 1.f ) hitchance = options.aim_hitchance_move;
		else if ( hitchance < 0 ) //Automatical mode
		{
			if ( g_Weapon->IsPistol( ) )
				hitchance = 30;
			else if ( g_Weapon->IsSniper( ) )
				hitchance = 70;
			else if ( g_Weapon->IsRifle( ) )
				hitchance = 55;
			else if( g_Weapon->IsSMG() )
				hitchance = 40;
			else if ( g_Weapon->IsHeavy( ) )
				hitchance = 50;
			else if ( g_Weapon->IsShotgun( ) )
				hitchance = 45;

			ConsolePrint( "hitchance now %i \n", hitchance );
		}
		int total_hits = 0;
		int needed_hits = static_cast<int>( max_traces * ( hitchance / 100.f ) );

		g_Weapon->UpdateAccuracyPenalty( );

		auto flRange = g_Weapon->GetCSWeaponData( )->flRange;

		for ( int i = 0; i < max_traces; i++ )
		{
			RandomSeed( i + 1 );

			float fRand1 = RandomFloat( 0.f, 1.f );
			float fRandPi1 = RandomFloat( 0.f, XM_2PI );
			float fRand2 = RandomFloat( 0.f, 1.f );
			float fRandPi2 = RandomFloat( 0.f, XM_2PI );

			float fRandInaccuracy = fRand1 * g_Weapon->GetInaccuracy( );
			float fRandSpread = fRand2 * g_Weapon->GetSpread( );

			float fSpreadX = cos( fRandPi1 ) * fRandInaccuracy + cos( fRandPi2 ) * fRandSpread;
			float fSpreadY = sin( fRandPi1 ) * fRandInaccuracy + sin( fRandPi2 ) * fRandSpread;

			auto viewSpreadForward = ( aim_angles.Forward( ) + fSpreadX * aim_angles.Right( ) + fSpreadY * aim_angles.Up( ) ).Normalized( );

			QAngle viewAnglesSpread;
			VectorAngles( viewSpreadForward, viewAnglesSpread );
			Utils::Normalize( viewAnglesSpread );

			Vector viewForward;
			AngleVectors( viewAnglesSpread, viewForward );
			viewForward.NormalizeInPlace( );

			viewForward = g_EyePos + ( viewForward * flRange );

			trace_t tr;
			Ray_t ray;
			ray.Init( g_EyePos, viewForward );

			
			g_EngineTrace->ClipRayToEntity( ray, MASK_SHOT | CONTENTS_GRATE, target, &tr );

			if ( tr.hit_entity == target )
				total_hits++;

			
			if ( total_hits >= needed_hits )
				return true;

			if ( ( max_traces - i + total_hits ) < needed_hits )
				return false;
		}
		return false;
	}

	bool aimtopoint( Vector point )
	{
		if ( point.Length( ) == 0 ) return false;
		Vector angles;
		Math::CalcAngle( g_EyePos, point, angles );
		Math::NormaliseViewAngle( angles );
		aim_angles = QAngle( angles.x, angles.y, angles.z );
		islocked = true;
		return true;
	}

	QAngle hitchance_angles;
	bool aimtopointSpecial( Vector point )
	{
		if ( point.Length( ) == 0 ) return false;
		Vector angles;
		Math::CalcAngle( g_EyePos, point, angles );
		Math::NormaliseViewAngle( angles );
		hitchance_angles = QAngle( angles.x, angles.y, angles.z );
		islocked = true;
		return true;
	}

	std::tuple<int, int> GetTargetHealth( )
	{
		int target = -1;
		int minHealth = 101;
		int target_hitbox = -1;

		for ( int i = 1; i < g_EntityList->GetHighestEntityIndex( ); i++ )
		{
			auto ent = (C_BasePlayer*)g_EntityList->GetClientEntity( i );

			if ( !TargetIsValid( ent ) )
				continue;

			int hitbox = Hitscan( ent );

			if ( hitbox < 0 )
				continue;

			if ( ent->FovToPlayer( hitbox ) > options.aim_fov )
				continue;

			int Health = ent->m_iHealth( );
			if ( Health < minHealth && Health > 0 )
			{
				minHealth = Health;
				target_hitbox = hitbox;
				target = i;
			}

		}

		return { target, target_hitbox };
	}

	std::tuple<int, int> GetTargetDamage( )
	{
		int target = -1;
		int currentDamage = 0;
		int target_hitbox = -1;

		for ( int i = 1; i < g_EntityList->GetHighestEntityIndex( ); i++ )
		{
			auto ent = (C_BasePlayer*)g_EntityList->GetClientEntity( i );

			if ( !TargetIsValid( ent ) )
				continue;

			int hitbox = Hitscan( ent );

			if ( hitbox < 0 )
				continue;

			if ( ent->FovToPlayer( hitbox ) > options.aim_fov )
				continue;

			int damage = (int)( GetDamageToEnemy( ent, hitbox ) );
			if ( damage > 0 && damage > currentDamage )
			{
				currentDamage = damage;
				target = i;
				target_hitbox = hitbox;
			}
		}
		return { target, target_hitbox };
	}

	tuple<int, int> GetBulletAmounToKill( )
	{
		int target = -1;
		int amount_of_bullets = 100;
		int target_hitbox = -1;

		for ( int i = 1; i < g_EntityList->GetHighestEntityIndex( ); i++ )
		{
			auto ent = (C_BasePlayer*)g_EntityList->GetClientEntity( i );

			if ( !TargetIsValid( ent ) )
				continue;

			int hitbox = HitScanSmart( ent );

			if ( hitbox < 0 )
				continue;

			if ( ent->FovToPlayer( hitbox ) > options.aim_fov )
				continue;

			int damage = (int)( GetDamageToEnemy( ent, hitbox ) );
			int bullets = 1;
			int health = (int)ent->m_iHealth( );

			int dmg_needed = options.min_dmg;

			if ( dmg_needed < 0 )
				dmg_needed = 15;

			if ( dmg_needed> 100 )
				dmg_needed = health;

			if ( damage < dmg_needed )
				continue;

			while ( damage < health )
			{
				bullets++;
				damage += damage;
			}

			if ( bullets < amount_of_bullets )
			{
				amount_of_bullets = bullets;
				target = i;
				target_hitbox = hitbox;
			}
		}
		return  { target, target_hitbox };
	}

	tuple<int, int> GetLethal( )
	{
		int target = -1;
		int target_hitbox = -1;
		int currentDamage = 0;

		for ( int i = 1; i < g_EntityList->GetHighestEntityIndex( ); i++ )
		{
			auto ent = (C_BasePlayer*)g_EntityList->GetClientEntity( i );

			if ( !TargetIsValid( ent ) )
				continue;

			int hitbox = Hitscan( ent );

			if ( hitbox < 0 )
				continue;

			if ( ent->FovToPlayer( hitbox ) > options.aim_fov )
				continue;

			int damage = (int)( GetDamageToEnemy( ent, hitbox ) );
			int health = (int)ent->m_iHealth( );

			if ( damage > 0 && damage > currentDamage )
			{
				currentDamage = damage;
				target = i;
				target_hitbox = hitbox;

				if ( damage >= health )
					return { target, target_hitbox };
			}
		}

		return { target, target_hitbox };
	}

	tuple<int, int> GetTargetHitchance( )
	{
		int target = -1;
		int target_hitbox = -1;
		int start_hitchance = 0;

		for ( int i = 1; i < g_EntityList->GetHighestEntityIndex( ); i++ )
		{
			auto ent = (C_BasePlayer*)g_EntityList->GetClientEntity( i );

			if ( !TargetIsValid( ent ) )
				continue;

			int hitbox = Hitscan( ent );

			if ( hitbox < 0 )
				continue;

			if ( ent->FovToPlayer( hitbox ) > options.aim_fov )
				continue;

			Vector point_scale = ( hitbox == 0 ? Vector( 0, 0, options.aim_ps * 2.5f ) : Vector( 0, 0, 0 ) );
			if ( aimtopointSpecial( ent->GetHitboxPos( hitbox ) + point_scale ) )
			{
				QAngle backup = g_Cmd->viewangles;
				g_Cmd->viewangles = hitchance_angles;
				int hitchance = HighestHitchance( ent, options.aim_hitchance ); //TODO: fix this (just change to g_EyePos)
				g_Cmd->viewangles = backup;
				if ( hitchance > start_hitchance )
				{
					start_hitchance = hitchance;
					target_hitbox = hitbox;
					target = i;
				}
			}
		}
		return { target, target_hitbox };
	}

	inline float screenfov( )
	{
		return RAD2DEG( atan( tan( DEG2RAD( engine->GetScreenAspectRatio( width, height ) * 0.5 ) ) * 0.75 ) ) * 2;
	}

	tuple<int, int> GetTargetCrosshair( )
	{
		int target = -1;
		int target_hitbox = -1;
		float minFoV = options.aim_fov;

		for ( int i = 1; i < g_EntityList->GetHighestEntityIndex( ); i++ )
		{
			auto ent = (C_BasePlayer*)g_EntityList->GetClientEntity( i );

			if ( !TargetIsValid( ent ) )
				continue;

			int hitbox = Hitscan( ent );

			if ( hitbox < 0 )
				continue;

			float fov = ent->FovToPlayer( hitbox );
			if ( fov > options.aim_fov )
				continue;

			if ( fov < minFoV )
			{
				minFoV = fov;
				target = i;
				target_hitbox = hitbox;
			}
		}
		return { target, target_hitbox };
	}

	void aimbot( int shots )
	{
		if ( !g_Weapon ) return;

		if (
			g_Weapon->m_iClip1( ) <= 0
			|| g_Weapon->IsKnife( )
			|| !g_Weapon->IsWeapon( )
			) return;

		C_BasePlayer* target = nullptr;

		g_EyePos = g_Local->GetEyePos( );

		bool findnew = true;

		engine->GetViewAngles( g_ViewAngles );

		if ( options.highlighted_meme != -1 )
		{
			auto player = (C_BasePlayer*)g_EntityList->GetClientEntity( options.highlighted_meme );
			if ( !TargetIsValid( player ) ) //TODO: reduce checks of validity check (see below theres 2nd)
				options.highlighted_meme = -1;

		}

		if ( islocked && targetid >= 0 && hitbox >= 0 )
		{
			target = (C_BasePlayer*)entlist->GetClientEntity( targetid );
			if ( TargetIsValid( target ) )
			{
				hitbox = Hitscan( target );
				if ( hitbox >= 0 )
				{
					float fov = target->FovToPlayer( hitbox );
					if ( fov <= options.aim_fov )
						findnew = false;
				}

			}
		}

		if ( findnew )
		{
			targetid = -1;
			hitbox = -1;

			std::tuple<int, int> data;

			switch ( options.aim_method )
			{
				case 0:
					data = GetTargetCrosshair( );
					break;
				case 1:
					data = GetTargetDistance( );
					break;
				case 2:
					data = GetTargetHealth( );
					break;

				case 3:
					data = GetTargetDamage( );
					break;

				case 4:
					data = GetLethal( );
					break;

				case 5:
					data = GetTargetHitchance( );
					break;

				case 6:
					data = GetBulletAmounToKill( );
					break;

				case 7:
					data = GetFirstTarget( );
					break;

				default:
					return;
			}

			targetid = std::get<0>( data );
			hitbox = std::get<1>( data );

			if ( targetid >= 0 )
				target = (C_BasePlayer*)g_EntityList->GetClientEntity( targetid );
			else
			{
				target = nullptr;
				hitbox = -1;
				targetid = -1;
				islocked = false;
				options.highlighted_meme = -1;
				return;
			}
		}

		if ( targetid >= 0 && target )
		{
			if ( !canopenfire( ) )
				return;

			if ( options.aim_autoscope && !g_Local->m_bIsScoped( ) && g_Weapon->IsSniper( ) )
				g_Cmd->buttons |= IN_ATTACK2;

			if ( options.aim_autostop )
			{
				Vector velocity = g_Local->m_vecVelocity( );
				QAngle direction;
				VectorAngles( velocity, direction );

				float speed = velocity.Length( );

				direction.yaw = g_Cmd->viewangles.yaw - direction.yaw;

				Vector negated_direction = direction.Forward( ) * -speed;

				g_Cmd->forwardmove = negated_direction.x;
				g_Cmd->sidemove = negated_direction.y;
			}

			if ( options.aim_autocrouch )
				g_Cmd->buttons |= IN_DUCK;

			int dmg_stomach = (int)GetDamageToEnemy( target, HITBOX_STOMACH );
			int dmg_pelvis = (int)GetDamageToEnemy( target, HITBOX_PELVIS );
			int health = target->m_iHealth( );

			//baim if lethal
			if ( options.baim_lethal )
			{
				if ( health <= dmg_stomach )
					hitbox = HITBOX_STOMACH;
				else if ( health <= dmg_pelvis )
					hitbox = HITBOX_PELVIS;
			}

			//baim if lower
			if ( health <= options.aim_baimlower )
			{
				if ( dmg_stomach > dmg_pelvis )
					hitbox = HITBOX_STOMACH;
				else if ( dmg_pelvis > dmg_stomach )
					hitbox = HITBOX_PELVIS;
			}

			//baim after shots
			if ( options.baim_shots > 0 && options.baim_shots <= baimShots.shots && baimShots.shots > 0 )
			{
				if ( dmg_stomach > dmg_pelvis )
					hitbox = HITBOX_STOMACH;
				else if ( dmg_pelvis > dmg_stomach )
					hitbox = HITBOX_PELVIS;
			}

			Vector addValue = ( hitbox == 0 ? Vector( 0, 0, options.aim_ps * 2.5f ) : Vector( 0, 0, 0 ) );
			//TODO: make extrapolation better & add option for it aka velocity prediction
			Vector point = target->GetHitboxPos( hitbox ) + ( target->m_vecVelocity( ) * global->interval_per_tick ) ;
			//TODO: also draw extrapolated pos & non extrapolated one to see differences
			if ( aimtopoint( point + addValue ) )
			{
				options.highlighted_meme = ( target->EntIndex( ) );
				pResolver::aim_targ = targetid;

				if ( hit_chance( target, options.aim_hitchance ) || ( g_Cmd->buttons & IN_ATTACK && keyboard::get( ).key_down( MOUSE_LEFT ) ) )
				{
					if ( options.aim_silent )
						g_Cmd->viewangles = aim_angles;
					else
						engine->SetViewAngles( aim_angles );

					if ( options.aim_autoshoot && !( g_Cmd->buttons & IN_ATTACK ) )
					{
						if ( options.aim_autoscope && g_Weapon->IsSniper( ) )
						{
							if ( g_Local->m_bIsScoped( ) )
								g_Cmd->buttons |= IN_ATTACK;
						} else
							g_Cmd->buttons |= IN_ATTACK;
					}
				}
			}
		}
	}

	float CurrentVelocity( C_BasePlayer* LocalPlayer )
	{
		float vel = LocalPlayer->m_vecVelocity( ).Length2D( );
		return vel;
	}

	float outgoing( )
	{
		INetChannelInfo *nci = engine->GetNetChannelInfo( );
		if ( nci )
		{
			float OutgoingLatency = nci->GetLatency( FLOW_OUTGOING );
			return OutgoingLatency;
		}

		return 0.0f;
	}

	float lastlbyupdate = 0.f;
	float server_time;
	bool bWasMoving = false;

	float AngleDiff( float destAngle, float srcAngle )
	{
		float delta;

		delta = destAngle - srcAngle;
		if ( destAngle > srcAngle )
		{
			while ( delta >= 180 )
				delta -= 360;
		} else
		{
			while ( delta <= -180 )
				delta += 360;
		}
		return delta;
	}

	//TODO: make it more like skeets so the angle diff will work on better positions as well
	int twitch_shouldstart( C_BasePlayer* pLocal, CUserCmd* cmd )
	{
		bool bMoving = pLocal->m_vecVelocity( ).Length2D( ) > 0.1f;
		bool onground = pLocal->m_fFlags( ) & FL_ONGROUND;


		float anglediff = AngleDiff( options.aa_real_addyaw, options.switch_value1 );
		if ( fabsf( anglediff ) > 120.f )
		{ //epic  stolen from skeet 
			int times = (int)( anglediff - 119 ) / 5;


			float add = 1.1f;
			for ( int i = 0; i <= times; i++ )
			{
				add -= 0.04f;//lol
			}

			if ( ( ( ( lastlbyupdate + add ) - outgoing( ) ) < server_time ) && onground && !bMoving )
			{
				if ( times == 0 )
					times++;

				return times;
			}
		}

		return 0;
	}

	bool lbyupdates( C_BasePlayer* pLocal, CUserCmd* cmd )
	{
		bool bMoving = pLocal->m_vecVelocity( ).Length2D( ) > 0.1f;
		bool onground = pLocal->m_fFlags( ) & FL_ONGROUND;

		if ( ( ( ( lastlbyupdate + ( bWasMoving ? 0.22 : 1.1f ) ) - outgoing( ) ) < server_time ) && onground )
		{
			LBYok = false;
			if ( bWasMoving )
				lastlbyupdate = server_time + outgoing( );
			else
				lastlbyupdate += global->interval_per_tick == 0.015625 ? 1.125f : 1.113f; // 64 and 128 tick 
																						  //TODO: make lby breaker proper without .025 or 0.013 shit

			if ( bWasMoving ) bWasMoving = false;
			return true;
		}

		return false;
	}

	//TODO: shit leeched one; make one urself later when not lazy (trace headspot dmg using same shit as autowall uses)
	float freestanding( CUserCmd* m_pcmd )
	{
		if ( local->m_vecVelocity( ).Length2D( ) < 300.f )
		{

			auto fov_to_player = []( Vector view_offset, QAngle view, C_BasePlayer* m_entity, int hitbox )
			{
				CONST FLOAT MaxDegrees = 180.0f;
				QAngle Angles = view;
				Vector Origin = view_offset;
				Vector Delta( 0, 0, 0 );
				Vector AimPos = m_entity->GetHitboxPos( hitbox );
				Math::VectorSubtract( AimPos, Origin, Delta );
				Math::Normalize( Delta, Delta );
				FLOAT DotProduct = Angles.Forward( ).Dot( Delta );
				return ( acos( DotProduct ) * ( MaxDegrees / M_PI ) );
			};

			int target = -1;
			float mfov = 50;

			Vector viewoffset = local->m_vecOrigin( ) + local->m_vecViewOffset( );
			QAngle view; engine->GetViewAngles( view );

			for ( int i = 0; i < globals->maxClients; i++ )
			{
				C_BasePlayer* m_entity = (C_BasePlayer*)entlist->GetClientEntity( i );


				if ( TargetIsValid( m_entity ) )
				{

					float fov = fov_to_player( viewoffset, view, m_entity, 0 );
					if ( fov < mfov )
					{
						mfov = fov;
						target = i;
					}
				}
			}

			Vector at_target_angle;

			if ( target )
			{
				auto m_entity = (C_BasePlayer*)entlist->GetClientEntity( target );

				if ( TargetIsValid( m_entity ) )
				{
					Vector head_pos_screen;

					if ( Math::WorldToScreen( m_entity->GetBonePos( 8 ), head_pos_screen ) )
					{
						Math::CalcAngle( local->m_vecOrigin( ), m_entity->m_vecOrigin( ), at_target_angle );
						at_target_angle.x = 0;

						Vector src3D, dst3D, src, dst;
						float back_two, right_two, left_two;
						trace_t tr;
						Ray_t ray, ray2, ray3, ray4, ray5;
						CTraceFilter filter;


						filter.pSkip = local;
						src3D = local->GetEyePos( );
						dst3D = src3D + ( at_target_angle.Forward( ) * 384 ); //Might want to experiment with other numbers, incase you don't know what the number does, its how far the trace will go. Lower = shorter.

						ray.Init( src3D, dst3D );
						g_EngineTrace->TraceRay( ray, MASK_SHOT, &filter, &tr );
						back_two = ( tr.endpos - tr.startpos ).Length( );

						Vector right = at_target_angle.Right( );
						ray2.Init( src3D + right * 35, dst3D + right * 35 );
						g_EngineTrace->TraceRay( ray2, MASK_SHOT, &filter, &tr );
						right_two = ( tr.endpos - tr.startpos ).Length( );

						ray3.Init( src3D - right * 35, dst3D - right * 35 );
						g_EngineTrace->TraceRay( ray3, MASK_SHOT, &filter, &tr );
						left_two = ( tr.endpos - tr.startpos ).Length( );

						if ( left_two > right_two )
							return -90.f;
						else if ( right_two > left_two )
							return 90.f;


						return 0.f;
					}
				}
			}
		}
		return 0.f;
	}

	static bool doAa = true;
	void antiaim( bool& sendpacket, QAngle angles, bool sendpacketForce = false )
	{
		if ( GetAsyncKeyState( VK_F6 ) & 1 )
			doAa = !doAa;

		if ( !doAa )
			return;

		QAngle angle = angles;
		angle.pitch = 89.f;

		static bool switchh = false;
		static bool aaflip = false;
		static float autoMoveDt = 180.f;

		float velocity = g_Local->m_vecVelocity( ).Length2D( );
		if ( g_Cmd->sidemove >= 450.f && velocity > 200.f ) //RIGHT
		{
			autoMoveDt = 90.f;
			autoaaleft = false;
		} else if ( g_Cmd->sidemove >= -450.f && velocity > 200.f ) //LEFT
		{
			autoaaleft = true;
			autoMoveDt = -90.f;
		} else  //NEITHER
			autoMoveDt = autoaaleft ? -150.f : 150.f;

		if ( aaflip && !sendpacketForce )
		{

			static int counter = 0;
			//fixes so lby isn't at real angles bef4 it updates after u stop move for 
			//0.22 sec so if theyre using last lby before break etc, they wont hit
			static int counter2 = 0; //similar fix 

			bool bMoving = g_Local->m_vecVelocity( ).Length2D( ) > 0.5f;

			if ( !bMoving )
			{
				static float freestand_value = 0.f; //disabled for now any other value=freestand on

				static bool lby_updated = false;

				if ( lbyupdates( g_Local, g_Cmd ) )
				{
					switchh = !switchh;

					counter++;
					lby_updated = true;

					if ( options.aa_lby_invertpitch )
						angle.pitch = -angle.pitch;

					if ( !options.aa_lby_antibacktrack )
						angle.yaw += switchh ? options.switch_value1 : options.switch_value2;
					else
						angle.yaw += options.switch_value1;
				} else
				{
					//TODO: remove hardcoding :DDD, this is easy af to make some algo for
					if ( GetAsyncKeyState( VK_LEFT ) && !GetAsyncKeyState( VK_DOWN ) && !GetAsyncKeyState( VK_UP ) || freestand_value == 90.f )
					{
						options.aa_real_addyaw = 90.f;

						options.switch_value1 = -45.f;
						options.switch_value2 = -135.f;
					} else if ( GetAsyncKeyState( VK_LEFT ) && GetAsyncKeyState( VK_DOWN ) )
					{
						options.switch_value1 = 0.f;
						options.switch_value2 = -90.f;
						options.aa_real_addyaw = 135.f;
					} else if ( GetAsyncKeyState( VK_LEFT ) && GetAsyncKeyState( VK_UP ) )
					{
						options.aa_real_addyaw = 45.f;
						options.switch_value1 = -90.f;
						options.switch_value2 = 180.f;
					} else if ( GetAsyncKeyState( VK_RIGHT ) && !GetAsyncKeyState( VK_DOWN ) && !GetAsyncKeyState( VK_UP ) || freestand_value == -90.f )
					{
						options.aa_real_addyaw = -90.f;
						options.switch_value1 = 45.f;
						options.switch_value2 = 135.f;
					} else if ( GetAsyncKeyState( VK_RIGHT ) && GetAsyncKeyState( VK_DOWN ) )
					{
						options.aa_real_addyaw = -135.f;
						options.switch_value1 = 90.f;
						options.switch_value2 = 0.f;
					} else if ( GetAsyncKeyState( VK_RIGHT ) && GetAsyncKeyState( VK_UP ) )
					{
						options.aa_real_addyaw = -45.f;
						options.switch_value1 = 90.f;
						options.switch_value2 = 180.f;
					} else if ( GetAsyncKeyState( VK_DOWN ) && !GetAsyncKeyState( VK_LEFT ) && !GetAsyncKeyState( VK_RIGHT ) ) //|| freestand_value == 0.f
					{
						options.switch_value1 = 45.f;
						options.switch_value2 = -45.f;
						options.aa_real_addyaw = 180.f;
					} else if ( GetAsyncKeyState( VK_UP ) && !GetAsyncKeyState( VK_LEFT ) && !GetAsyncKeyState( VK_RIGHT ) )
					{
						options.aa_real_addyaw = 0.f;
						options.switch_value1 = -135.f;
						options.switch_value2 = 135.f;
					}

					if ( counter > 0 && counter2 > 2 )
					{
						int twitch = twitch_shouldstart( g_Local, g_Cmd );
						if ( options.aa_lby_antibacktrack && twitch > 0 )
						{ //TODO: add opt for lby exploit properly so you can do lby at any angles like at 60 or 80 because most resolvers dont hit there, rn u can but its shit cuz u need to switch

							float val1 = ( options.aa_real_addyaw + options.switch_value1 - 360.f );
							if ( fabsf( val1 ) >= 120.f )
							{
								//36 and 0.9
								float asd2 = AngleDiff( options.aa_real_addyaw, options.switch_value1 );
								float addvalue = fabsf( 120.f - asd2 ) + 1.f;
								angle.yaw += options.aa_real_addyaw - addvalue;
							} else
								angle.yaw += options.aa_real_addyaw;


						} else
							angle.yaw += options.aa_real_addyaw;

					} else
						angle.yaw += autoMoveDt;

					if ( counter > 0 && counter < 5 ) //yeh no need to add int val for nothing
						counter2++;

					//Opposite
					switch ( options.aa_real_opposite )
					{
						case 0:
						default:
							break;

						case 1:
							if ( LBYok && counter > 0 && counter2 > 2 )
								angle.yaw = g_Local->lby() + 180.f;
							break;

						case 2:
							angle.yaw = fakeyaw + 180.f;
							break;
					}

					//Spin
					if ( options.aa_real_spin && options.aa_real_spin_speed != 0 )
					{
						float speed = options.aa_real_spin_speed * 10.f; //improper
						float spin = fmod( server_time * speed, 360.0f );
						angle.yaw += spin;

						if ( options.aa_real_spin_lbyr != 0.f )
						{
							float copy = angle.yaw;
							Utils::ClampYaw( copy );

							bool notDetected = false;
							while ( !notDetected ) //even more improper shit
							{
								float delta = g_Local->lby( ) - copy;
								Utils::ClampYaw( delta );

								if ( fabsf( delta ) < options.aa_real_spin_lbyr )
									angle.yaw += 5.f; //ok lets use this b1g random val for now =)
								else
									notDetected = true;

								copy = angle.yaw;
								Utils::ClampYaw( copy );
							}

						}


					}

					//Switch
					if ( options.aa_real_switch )
					{

						static bool switch_mode = false;

						static clock_t start = clock( );

						double time = (double)( clock( ) - start ) / CLOCKS_PER_SEC;
						double wait_time = ( (double)options.aa_real_switch_wait / 100. );

						if ( options.aa_real_switch_lby )
						{
							if ( lby_updated )
								switch_mode = !switch_mode;
						} else if ( time >= wait_time )
						{
							switch_mode = !switch_mode;
							start = clock( );
						}


						if ( switch_mode )
							angle.yaw += options.aa_real_switch_value;

					}

					//Jitter
					if ( options.aa_real_jitter )
					{
						static float add = 0.f;
						static float minus = 0.f;

						static clock_t start = clock( );
						double time = (double)( clock( ) - start ) / CLOCKS_PER_SEC;

						double wait_time;
						if ( options.aa_real_jitter_wait == 0 )
							wait_time = 0.;
						else
							wait_time = ( (double)options.aa_real_jitter_wait / 100. );


						if ( options.aa_real_jitter_lby )
						{
							if ( lby_updated )
							{
								int initial = options.aa_real_jitter_range;

								minus = (float)initial / 2.f;
								add = (float)( rand( ) % initial );

								angle.yaw -= minus;
								angle.yaw += add;
							} else
							{
								angle.yaw -= minus;
								angle.yaw += add;
							}
						} else if ( time >= wait_time || wait_time == 0. )
						{
							int initial = options.aa_real_jitter_range;

							minus = (float)initial / 2.f;
							add = (float)( rand( ) % initial );

							angle.yaw -= minus;
							angle.yaw += add;

							start = clock( );
						} else
						{
							angle.yaw -= minus;
							angle.yaw += add;
						}
					}

					lby_updated = false;
				}
			} else
			{
				float freestand_value = freestanding( g_Cmd );
				//freestand_value = 0.f; //DISABLE FREESTAND

				if ( freestand_value == 0.f )
					angle.yaw += autoMoveDt;
				else
					angle.yaw += freestand_value;

				counter = counter2 = 0;
				lastlbyupdate = server_time + outgoing( );
				bWasMoving = true;
			}

			//Real Angles
			sendpacket = false;
		} else
		{
			//Fake Angles

			angle.yaw += ( ( switchh ? options.switch_value2 : options.switch_value1 ) - 10.f ) + ( rand( ) % 20 );

			sendpacket = true;
		}

		aaflip = !aaflip;

		if ( Utils::Clamp( angle ) )
		{
			g_Cmd->viewangles = angle;
			if ( !sendpacket )
				LBYok = ( abs( g_Local->lby( ) - g_Cmd->viewangles.yaw ) > 35.f );
		} else
			ConsolePrint( "Failed to clamp angles \n" );
	}

	void __stdcall hook_createmove( int sequence_number, float input_sample_frametime, bool active, bool& bSendPacket )
	{
		auto oCreateMove = hlclient_hook.get_original<CreateMove>( index::CreateMove );
		oCreateMove( g_CHLClient, sequence_number, input_sample_frametime, active );

		auto cmd = g_Input->GetUserCmd( sequence_number );
		auto verified = g_Input->GetVerifiedCmd( sequence_number );

		if ( !cmd || !cmd->command_number )
			return;

		auto pLocal = (C_BasePlayer*)g_EntityList->GetClientEntity( engine->GetLocalPlayer( ) );

		
		if ( pLocal && pLocal->IsAlive( ) )
		{
			g_Cmd = cmd;
			if ( !g_ChatElement )
			{
				//color codes: https://i.imgur.com/81j5Byf.png
				g_ChatElement = FindHudElement<CHudChat>( "CHudChat" );
				ConsolePrint( "Finding chat element... \n" );
			}

			int shots = g_Local->m_iShotsFired( );

			if ( options.baim_shots > 0 && options.aim_bot )
			{

				if ( baimShots.entnum == targetid )
				{
					if ( shots > 0 )
					{
						if ( baimShots.old_shots < shots )
							baimShots.shots++;
					}
					baimShots.old_shots = shots;
				} else if ( targetid > -1 )
				{
					baimShots.entnum = targetid;
					baimShots.old_shots = 0;
					baimShots.shots = 0;
					ConsolePrint( "resetting shots \n" );
				}
			}

			if ( options.misc_enabled )
			{
				if ( options.misc_aw_crasher && g_Cmd->tick_count % 6 == 0 )
					engine->ClientCmd( "sm_c4" );

			}

			if ( options.engine_pred && options.misc_enabled )
				pred::run( );

			if ( !g_Local->InLadderOrNoclip( ) && options.misc_enabled )
			{
				if ( options.misc_bhop )
					bhop::get( ).on_createmove( );

				if ( options.misc_autostrafe )
					autostrafer::get( ).on_createmove( );
			}

			cvar_cheats::run_all( );

			Utils::SetClantag( "[testing]" );


			static bool hooked_send_datagram = false;

			if ( !hooked_send_datagram )
			{
				g_GameEvents->AddListener( listener, "player_death", false );
				g_GameEvents->AddListener( listener, "player_hurt", false );
				g_GameEvents->AddListener( listener, "round_start", false );
				g_GameEvents->AddListener( listener, "bullet_impact", false ); 
					//bullet_impact listener wont setup unless called when in game
					//other ones here just as a fail safe, does not matter anyways
					
				ConsolePrint( "Hooked SendDatagram & registered events \n" );
				INetChannel *netchan = (INetChannel*)g_ClientState->m_NetChannel;

				if ( netchan )
				{
					netchannel_hook.setup( netchan );
					netchannel_hook.hook_index( 48, SendDatagram );
				} else
					ConsolePrint( "null netchannel, cant hook datagram \n" );

				hooked_send_datagram = true;
			}


			//ayyware crasher
			/*
			std::stringstream ss;
			for (auto i = 0; i <= 150; i++)
			ss << static_cast<char>(-1);

			auto cv = g_CVar->FindVar("name");
			*reinterpret_cast<int*>(reinterpret_cast<DWORD>(&cv->m_fnChangeCallbacks) + 0xC) = NULL;
			cv->SetValue(ss.str().c_str());
			*/

			QAngle oldangles = g_Cmd->viewangles; //angles before aimbot

			if ( options.aim_bot )
				aimbot( shots );

			if ( options.aim_norecoil )
				rcs::get( ).run( );


			static int choked = 0;
			if ( g_Cmd->buttons & IN_ATTACK )
			{
				if ( options.aim_pSilent && options.aim_silent && options.aim_bot )
				{
					if ( choked < 3 && canopenfire( ) ) //lol prob one of shittiest methods ever too lazy to make proper, for now
					{
						choked++;
						bSendPacket = false;
					} else
					{
						bSendPacket = true;

						if ( !g_Local->InLadderOrNoclip( ) )
							antiaim( bSendPacket, oldangles, true );

						choked = 0;
					}
				}
			} else
			{
				choked = 0;
				if ( !g_Local->InLadderOrNoclip( ) && g_Weapon->m_iState() != 0 && !g_Local->Frozen())
					antiaim( bSendPacket, oldangles );
			}

			if ( options.misc_enabled && options.misc_eventlag && g_Cmd->tick_count % 3 == 0 )
			{
				g_ClientState->m_NetChannel->m_nInSequenceNr -= 75;
				g_ClientState->m_NetChannel->m_nInReliableState -= 75;
			}

			Utils::CorrectMovement( oldangles );
			Utils::Clamp( g_Cmd->viewangles ); //ANTI UT

			freecam::get( ).create_move( );

			if ( !bSendPacket )
				lastmeme = Vector( g_Cmd->viewangles.pitch, g_Cmd->viewangles.yaw, g_Cmd->viewangles.roll );

			QAngle autism;
			engine->GetViewAngles( autism );
			viewyaw = autism.yaw;

			if ( bSendPacket )
			{
				fakeyaw = g_Cmd->viewangles.yaw;
				Hooks::fakeanglesOrigin = g_Local->GetRenderOrigin( );
			} else
				realyaw = g_Cmd->viewangles.yaw;
		}

		verified->m_cmd = *cmd;
		verified->m_crc = cmd->GetChecksum( );
	}

	__declspec( naked ) void __stdcall hook_createmove_proxy( int sequence_number, float input_sample_frametime, bool active )
	{
		__asm
		{
			push ebp
			mov  ebp, esp
			push ebx
			lea  ecx, [ esp ]
			push ecx
			push dword ptr[ active ]
			push dword ptr[ input_sample_frametime ]
			push dword ptr[ sequence_number ]
			call Hooks::hook_createmove
			pop  ebx
			pop  ebp
			retn 0Ch
		}
	}

	void DrawAALinemiddle( float rad, float d, Color color )
	{
		float mo = 180.f;
		float og_angle = d;
		d = ( (int)( ( (int)( mo - d ) % 360 ) + 540.f ) % 360 ) - 90; //-180 , 180 to autism system 
																	   //back to normal ( if ever needed ) ((360 - value) % 360 + 540) % 360 - 180 - 90

		float x1 = rad * cos( deg2rad( d ) ) + width / 2; //yeehaw moment
		float y1 = rad * sin( deg2rad( d ) ) + height / 2;


		if ( !options.aal_simple )
		{
			DrawLine( ( width / 2 ) + 1, ( height / 2 ) + 1, x1 + 1, y1 + 1, Color::Black );
			DrawLine( ( width / 2 ) - 1, ( height / 2 ) - 1, x1 - 1, y1 - 1, Color::Black );
		}

		DrawLine( width / 2, height / 2, x1, y1, color );

	}

	void AALinesOther( C_BasePlayer* pEnt )
	{
		if ( !pEnt || !pEnt->IsPlayer( ) || !pEnt->IsAlive( ) || pEnt->IsDormant( ) ) return;

		Vector src3D, dst3D, forward, src, dst;
		trace_t tr;
		Ray_t ray;
		CTraceFilter filter;

		float yaw = pEnt->m_angEyeAngles( ).yaw;

		forward = QAngle( 0, g_Local->lby( ), 0 ).Forward( );

		src3D = pEnt->GetRenderOrigin( );


		dst3D = src3D + ( forward * 40.f );
		ray.Init( src3D, dst3D );

		g_EngineTrace->TraceRay( ray, 0, &filter, &tr );

		if ( !Math::WorldToScreen( src3D, src ) || !Math::WorldToScreen( tr.endpos, dst ) )
			return;

		if ( !options.aal_simple )
		{
			DrawLine( src.x + 1, src.y + 1, dst.x + 1, dst.y + 1, Color::Black );
			DrawLine( src.x - 1, src.y - 1, dst.x - 1, dst.y - 1, Color::Black );
		}
		DrawLine( src.x, src.y, dst.x, dst.y, Color( 210, 105, 30, 255 ) );

		forward = QAngle( 0, yaw, 0 ).Forward( );

		dst3D = src3D + ( forward * 40.f );

		ray.Init( src3D, dst3D );

		g_EngineTrace->TraceRay( ray, 0, &filter, &tr );

		if ( !Math::WorldToScreen( src3D, src ) || !Math::WorldToScreen( tr.endpos, dst ) )
			return;

		if ( !options.aal_simple )
		{
			DrawLine( src.x + 1, src.y + 1, dst.x + 1, dst.y + 1, Color::Black );
			DrawLine( src.x - 1, src.y - 1, dst.x - 1, dst.y - 1, Color::Black );
		}
		DrawLine( src.x, src.y, dst.x, dst.y, Color( 255, 0, 0, 255 ) );

		char buffer2[ 25 ];
		sprintf( buffer2, "%.0f", g_Local->lby( ) );

		DrawString( dst.x, dst.y + 10, Color::White, buildFont, buffer2 );
	}

	void DrawAutowallCrosshair( C_BaseCombatWeapon* weapon )
	{
		float damage = 0.f;
		if ( Awall( ).CanWallbang( local, weapon, damage ) )
			DrawFilledRect( width / 2 - 1, height / 2 - 1, 5, 5, Color::Green );
	}

	void AALinesLocal( float real, float fake )
	{
		Vector src3D, dst3D, forward, src, dst;
		trace_t tr;
		Ray_t ray;
		CTraceFilter filter;
		const Color lbycolor = Color( 210, 105, 30, 255 );
		const Color realcolor = Color( 0, 255, 0, 255 );
		const Color fakecolor = Color( 255, 0, 0, 255 );

		float rad = tan( deg2rad( 7.5f * 0.5 ) * 2 ) / screenfov( ) * ( width * 0.5 );
		float lby2 = ( g_Local->lby( ) - viewyaw );
		Utils::ClampYaw( lby2 );

		float real2 = ( real - viewyaw );
		NormalizeYaw( real2 );

		float fake2 = ( fake - viewyaw );
		NormalizeYaw( fake2 );

		if ( options.aal_cross )
		{
			if ( options.aal_lby )
				DrawAALinemiddle( rad, lby2, lbycolor );

			if ( options.aal_real )
				DrawAALinemiddle( rad, real2, realcolor );

			if ( options.aal_fake )
				DrawAALinemiddle( rad, fake2, fakecolor );
		}

		if ( !options.aal_feet ) return;

		src3D = g_Local->GetRenderOrigin( );

		if ( options.aal_lby )
		{
			forward = QAngle( 0, g_Local->lby( ), 0 ).Forward( );

			dst3D = src3D + ( forward * 40.f );

			ray.Init( src3D, dst3D );

			g_EngineTrace->TraceRay( ray, 0, &filter, &tr );

			if ( !Math::WorldToScreen( src3D, src ) || !Math::WorldToScreen( tr.endpos, dst ) )
				return;

			if ( !options.aal_simple )
			{
				DrawLine( src.x + 1, src.y + 1, dst.x + 1, dst.y + 1, Color::Black );
				DrawLine( src.x - 1, src.y - 1, dst.x - 1, dst.y - 1, Color::Black );
			}

			DrawLine( src.x, src.y, dst.x, dst.y, lbycolor );
		}

		if ( options.aal_real )
		{
			forward = QAngle( 0, real, 0 ).Forward( );

			dst3D = src3D + ( forward * 40.f );

			ray.Init( src3D, dst3D );

			g_EngineTrace->TraceRay( ray, 0, &filter, &tr );

			if ( !Math::WorldToScreen( src3D, src ) || !Math::WorldToScreen( tr.endpos, dst ) )
				return;

			if ( !options.aal_simple )
			{
				DrawLine( src.x + 1, src.y + 1, dst.x + 1, dst.y + 1, Color::Black );
				DrawLine( src.x - 1, src.y - 1, dst.x - 1, dst.y - 1, Color::Black );
			}

			DrawLine( src.x, src.y, dst.x, dst.y, realcolor );
		}

		if ( options.aal_fake )
		{
			forward = QAngle( 0, fake, 0 ).Forward( );

			dst3D = src3D + ( forward * 40.f );

			ray.Init( src3D, dst3D );

			g_EngineTrace->TraceRay( ray, 0, &filter, &tr );

			if ( !Math::WorldToScreen( src3D, src ) || !Math::WorldToScreen( tr.endpos, dst ) )
				return;

			if ( !options.aal_simple )
			{
				DrawLine( src.x + 1, src.y + 1, dst.x + 1, dst.y + 1, Color::Black );
				DrawLine( src.x - 1, src.y - 1, dst.x - 1, dst.y - 1, Color::Black );
			}

			DrawLine( src.x, src.y, dst.x, dst.y, fakecolor );
		}
	}

	void SwastikaCrosshair( )
	{
		int rotation = coordSwitch( );

		static int x = ( width / 2 );
		static int y = ( height / 2 );

		int a = (int)( height / 2 / 30 );
		float gamma = atan( a / a );


		int i = 0;
		while ( i < 4 )
		{
			std::vector <int> p;
			p.push_back( a * sin( GRD_TO_BOG( rotation + ( i * 90 ) ) ) );									//p[0]		p0_A.x
			p.push_back( a * cos( GRD_TO_BOG( rotation + ( i * 90 ) ) ) );									//p[1]		p0_A.y
			p.push_back( ( a / cos( gamma ) ) * sin( GRD_TO_BOG( rotation + ( i * 90 ) + BOG_TO_GRD( gamma ) ) ) );	//p[2]		p0_B.x
			p.push_back( ( a / cos( gamma ) ) * cos( GRD_TO_BOG( rotation + ( i * 90 ) + BOG_TO_GRD( gamma ) ) ) );	//p[3]		p0_B.y

			Color k = options.color_esp_crosshair;
			DrawLine( x, y, x + p[ 0 ], y - p[ 1 ], k );
			DrawLine( x + p[ 0 ], y - p[ 1 ], x + p[ 2 ], y - p[ 3 ], k );

			i++;
		}

	}
	bool make_dick_big( ) //from ati, others an see, rn broken fix when no lazy
	{
		if ( steam != nullptr  && steam->friends() != nullptr)
		{
		   steam->friends( )->set_rpc( "status", " dont invite me =)" );
			steam->friends( )->set_rpc( "game:act", "offline" );
			steam->friends( )->set_rpc( "game:mode", "competitive" );
			steam->friends( )->set_rpc( "game:mapgroupname", "mg_active" );
			steam->friends( )->set_rpc( "game:map", "random" );
			steam->friends( )->set_rpc( "game:score", " dont invite me =)" );
			return steam->friends( )->set_rpc( "game:server", "competitive" );

		}
		return false;
	}
		
	void __stdcall hkPaintTraverse( vgui::VPANEL panel, bool forceRepaint, bool allowForce )
	{
		static auto panelId = vgui::VPANEL{ 0 };

		if ( !strcmp( g_VGuiPanel->GetName( panel ), "HudZoom" ) && options.misc_noscope )
			return;

		static auto oPaintTraverse = vguipanel_hook.get_original<PaintTraverse>( index::PaintTraverse );
		oPaintTraverse( g_VGuiPanel, panel, forceRepaint, allowForce );

		if ( !panelId )
		{
			const auto panelName = g_VGuiPanel->GetName( panel );
			if ( !strcmp( panelName, "FocusOverlayPanel" ) )
				panelId = panel;

		} else if ( panelId == panel )
		{

			static float rainbow = 0.f;
			float speed = 0.0001f * options.rainbow_speed; //0.0001 is random lol fix later
			rainbow += speed;
			if ( rainbow > 1.f )
				rainbow = 0.f;
			
			rainbowCol =  FromHSB( rainbow, 1.f, 1.f );

			DrawString( 10, 5, rainbowCol, logoFont, "kysware" );
			DrawString( width - 150, 5, Color::White, buildFont, "built on 2018 etc" );

			dlights::run( ); //d & elights

			static bool setState = false;
			if ( !setState )
			{
				setState = true;
				//make_dick_big( ); //something broke this fix it when not lazy
				proto::get( ).SendClientHello( );
			}
				

			if ( !engine->IsConnected( ) || !engine->IsInGame( ) )
			{
				if ( GetAsyncKeyState( VK_F2 ) & 1 ) //lazy testing shortcut
				{
					engine->ExecuteClientCmd( "sv_cheats 1" );
					engine->ExecuteClientCmd( "bot_stop 1" );
					engine->ExecuteClientCmd( "map a" );
				}
				for each ( pResolver::stored item in pResolver2::array_ )
				{
					item.method_use = -1;
					item.res_state = pResolver::Lowerbody;
					item.count = 0;
					item.yaw = 0.f;
					item.aimbot_target = false;
				}

				hitmarker( ).get( ).players.clear( );
				lastlbyupdate = 1.125f; //map reset meme
			}
			if ( engine->IsInGame( ) && g_Local && g_Local->IsAlive(  ) )
			{
				//TODO: make option for this to not display everything 
				
				
				
				if ( options.aal )
					AALinesLocal( realyaw, fakeyaw );

					const static Color okColor = Color( 46, 204, 113 );
					const static Color warningColor = Color( 231, 76, 60 );

					float delta1 = g_Local->lby( ) - realyaw;
					ClampYaw( delta1 );

					float delta2 = fakeyaw - realyaw;
					ClampYaw( delta2 );

					char buffer[ 25 ];
					sprintf( buffer, "LBY: %.0f", g_Local->lby( ) );

					char buffer2[ 25 ];
					sprintf( buffer2, "Fake: %.0f", fakeyaw );

					char buffer3[ 25 ];
					sprintf( buffer3, "Real: %.0f", realyaw );

					char buffer4[ 25 ];
					sprintf( buffer4, "View: %.0f", viewyaw );

					char buffer5[ 25 ];
					sprintf( buffer5, "Delta (L): %.0f", delta1 );

					char buffer6[ 25 ];
					sprintf( buffer6, "Delta (F): %.0f", delta2 );



					DrawString( 15, height / 2, Color::White, buildFont, buffer );
					DrawString( 15, ( height / 2 ) + 35, Color::White, buildFont, buffer2 );
					DrawString( 15, ( height / 2 ) - 35, Color::White, buildFont, buffer3 );
					DrawString( 15, ( height / 2 ) + 35 + 35, Color::White, buildFont, buffer4 );
					DrawString( 15, ( height / 2 ) - 35 - 35, Color::White, buildFont, buffer5 );
					DrawString( 15, ( height / 2 ) - 35 - 35 - 35, Color::White, buildFont, buffer6 );

					DrawString( 15, ( height / 2 ) + 35 + 35 + 35, LBYok ? okColor : warningColor, VerdanaBig, "LBY" );
					if ( suppressing_979 && local->m_vecVelocity( ).Length2D( ) < 1.f && LBYok )
						DrawString( 15, ( height / 2 ) + 35 + 35 + 35 + 35, okColor, VerdanaBig, "979" );
				
		
				if ( options.misc_hitmarker && options.misc_enabled )
					hitmarker::get( ).on_paint( );

				if ( options.vis_misc_enabled )
				{

					if ( options.vis_fovcircle )
					{
						float rad = tan( DEG2RAD( options.aim_fov * 0.5 ) * 2 ) / screenfov( ) * ( width * 0.5 );
						DrawCircle( width / 2, height / 2, rad, rad, Color::White );
					}

					if ( options.vis_spreadcircle )
					{

						if ( g_Weapon && !g_Weapon->IsKnife( ) && g_Weapon->IsWeapon( ) )
						{
							float rad = ( g_Weapon->GetSpread( ) + g_Weapon->GetInaccuracy( ) ) * 500.f;

							FilledCircle( Vector2D( width / 2, height / 2 ), rad, rad, Color( 35, 35, 35, 120 ) );

							DrawCircle( width / 2, height / 2, rad, rad, Color::Red );

							static float addval = 0.f;
							float calculated = -180.f;
							calculated += addval;

							while ( calculated <= 180.f )
							{
								float converted = ( (int)( ( (int)( 180.f - calculated ) % 360 ) + 540.f ) % 360 ) - 90; //lol 
								float x2 = rad * cos( deg2rad( converted ) ) + width / 2;
								float y2 = rad * sin( deg2rad( converted ) ) + height / 2;

								float x1 = ( rad * 0.8 ) * cos( deg2rad( converted ) ) + width / 2;
								float y1 = ( rad * 0.8 ) * sin( deg2rad( converted ) ) + height / 2;

								DrawLine( x1, y1, x2, y2, Color::Red );
								calculated += 15.f;
							}

							addval += 0.15f;
							if ( addval > 15.f )
								addval = 0.f;

						}
					}

					if ( options.esp_crosshair )
						SwastikaCrosshair( );

					if ( g_Weapon )
						DrawAutowallCrosshair( g_Weapon );

					if ( g_Local->m_bIsScoped( ) && g_Weapon )
					{
						int middley = height / 2;
						int middlex = width / 2;
						if ( options.misc_noscope && options.misc_scopelines &&  g_Weapon->m_iItemDefinitionIndex( ) != ItemDefinitionIndex::WEAPON_SG553 && g_Weapon->m_iItemDefinitionIndex( ) != ItemDefinitionIndex::WEAPON_AUG )
						{
							DrawLine( 0, middley, width, middley, Color::Black );
							DrawLine( middlex, 0, middlex, height, Color::Black );
						} else if ( *(bool*)( (DWORD)g_Input + 0xA5 ) && options.misc_remove_blur )
							DrawString( middlex, middley - 80, Color::White, buildFont, "[scoped]", true );

					}
				}

				//TODO: replace markhc shit esp with own one
				if ( options.esp_enabled )
				{
					static float dormantarray[ 65 ];
					static float nondormantarray[ 65 ];
					for ( auto i = 1; i <= g_EntityList->GetHighestEntityIndex( ); ++i )
					{
						auto entity = C_BasePlayer::GetPlayerByIndex( i );

						if ( !entity )
							continue;

						if ( entity == g_Local )
							continue;

						//add dormant cheCk

						if ( i < 65 && entity->IsAlive( ) )
						{
							if ( Visuals::Player::Begin( entity ) )
							{
								if ( !entity->IsDormant( ) )
								{
									if ( nondormantarray[ i ] + 1.f >= global->curtime )
									{
										float delta = global->curtime - nondormantarray[ i ];
										Visuals::Player::SetAlpha( (unsigned char)( delta * 255 ) );
									}

									dormantarray[ i ] = global->curtime;

									if ( options.aal && options.aal_other )
										AALinesOther( entity );

									if ( options.esp_player_snaplines ) Visuals::Player::RenderSnapline( );
									if ( options.esp_player_boxes )     Visuals::Player::RenderBox( );
									if ( options.esp_player_weapons )   Visuals::Player::RenderWeapon( );
									if ( options.esp_player_names )     Visuals::Player::RenderName( );
									if ( options.esp_player_health )    Visuals::Player::RenderHealth( );
									if ( options.esp_player_armour )    Visuals::Player::RenderArmour( );
											Visuals::Player::RenderSkeleton( );

								} else
								{
									nondormantarray[ i ] = global->curtime;

									if ( dormantarray[ i ] + 5.f >= global->curtime )
									{
										Color col = Color::Gray;
										float delta = global->curtime - dormantarray[ i ];

										col._CColor[ 3 ] = (unsigned char)( ( 5.f - delta ) * 51 );
										//epic
										Visuals::Player::RenderBox( true, col );
										Visuals::Player::RenderLastknown( col.a( ) );
									}


								}

							}
						} else if ( options.esp_dropped_weapons && entity->IsWeapon( ) )
							Visuals::Misc::RenderWeapon( (C_BaseCombatWeapon*)entity );
						else if ( options.esp_defuse_kit && entity->IsDefuseKit( ) )
							Visuals::Misc::RenderDefuseKit( entity );
						else if ( entity->IsPlantedC4( ) )
						{
							if ( options.esp_planted_c4 )
								Visuals::Misc::RenderPlantedC4( entity );
						}
					}
				}

			}
		}
	}

	void __stdcall hkPlaySound( const char* name )
	{
		static auto oPlaySound = vguisurf_hook.get_original<playSoundFn>( index::playSound );

		oPlaySound( g_VGuiSurface, name );

		// Auto Accept
		if ( strstr( name, "UI/competitive_accept_beep.wav" ) )
		{
			static auto fnAccept =
				( void( *)( ) )Utils::PatternScan(g_ClientModule, "55 8B EC 83 E4 F8 83 EC 08 56 8B 35 ? ? ? ? 57 83 BE" );

			fnAccept( );

		FLASHWINFO fi;
			fi.cbSize = sizeof( FLASHWINFO );
			fi.hwnd = InputSys::get( ).GetMainWindow( );
			fi.dwFlags = FLASHW_ALL | FLASHW_TIMERNOFG;
			fi.uCount = 0;
			fi.dwTimeout = 0;
			FlashWindowEx( &fi );
		}
	}

	
	int __stdcall hkDoPostScreenEffects( int a1 )
	{
		auto oDoPostScreenEffects = clientmode_hook.get_original<DoPostScreenEffects>( index::DoPostScreenSpaceEffects );


		if ( options.glow_enabled )
		{
			auto aa = matsystem->FindMaterial( "dev/glow_color", TEXTURE_GROUP_OTHER, true );

			mdlrender->ForcedMaterialOverride( aa ); //test
			Glow::get( ).Run( );
		}
		

		return oDoPostScreenEffects( g_ClientMode, a1 );
	}


	bool PrecacheModel( const char* szModelName )
	{
		INetworkStringTable* m_pModelPrecacheTable = g_NetworkStringTable->FindTable( "modelprecache" );
		if ( m_pModelPrecacheTable )
		{
			g_MdlInfo->FindOrLoadModel( szModelName );
			int idx = m_pModelPrecacheTable->AddString( false, szModelName );
			if ( idx == -1 )//INVALID_STRING_INDEX
				return false;
			else
				return true;
		}
		return false;
	}
	using CCLCMsg_VoiceData_ctor_t = void( __thiscall * )( void *msg );
	using CCLCMsg_VoiceData_dtor_t = void( __thiscall * )( void *msg );
	using protobuf_assign_t = void( __thiscall * )( void *ptr, void *data, size_t len );

	template<typename t> t FollowRel32( uintptr_t address, size_t offset )
	{ //some shit from astra
		if ( !address )
			return t{ };

		auto offsetAddr = address + offset;
		auto relative = *(uint32_t *)offsetAddr;
		if ( !relative )
			return t{ };

		return (t)( offsetAddr + relative + sizeof( uint32_t ) );
	}


	__declspec( noinline ) static void spam_voicedata( )
	{ //doesnt work anyways , from astra
		static CCLCMsg_VoiceData_ctor_t CCLCMsg_VoiceData_ctor = ( CCLCMsg_VoiceData_ctor_t)Utils::PatternScan( g_EngineModule, "56 57 8B F9 8D 4F 08 C7 07 ? ? ? ? E8 ? ? ? ? C7 07" );
		static CCLCMsg_VoiceData_dtor_t CCLCMsg_VoiceData_dtor = (CCLCMsg_VoiceData_dtor_t)Utils::PatternScan( g_EngineModule, "53 8B D9 56 8D 73 3C" );

		static uintptr_t tmp_protobuf_assign = ( uintptr_t)Utils::PatternScan( g_EngineModule, "E8 ? ? ? ? 83 ? ? ? ? 83 ? ? ? ? C7 44" );

		static protobuf_assign_t  protobuf_assign = FollowRel32< protobuf_assign_t >( tmp_protobuf_assign , 1 );

		INetChannel *nc;
		uint8_t     msg[ 104 ]; // "*(_DWORD *)(v1 + 80)" -- used in alloc in alloc... seems to be at least ~104 bytes on stack in CL_SendVoicePacket.

		nc = g_ClientState->m_NetChannel;
		if ( !nc )
			return;

		// construct netmsg for sending voicedata.
		CCLCMsg_VoiceData_ctor( &msg );

		// fill with null voice data.
		protobuf_assign( &msg[ 8 ], nullptr, 0 );

		// fill unreliable stream with voice data.
		for ( int i = 0; i < 128; ++i )
			nc->SendNetMsg( *(INetMessage*)msg );

		// force send message now.
	//	nc->Transmit( );

		// cleanup.
		CCLCMsg_VoiceData_dtor( &msg );
	}

	void __stdcall hkFrameStageNotify( ClientFrameStage_t stage )
	{
		static auto ofunc = hlclient_hook.get_original<FrameStageNotify>( index::FrameStageNotify );

		auto pLocal = (C_BasePlayer*)g_EntityList->GetClientEntity( engine->GetLocalPlayer( ) );

		static bool precached = false;

		QAngle aim_punch_old, view_punch_old;

		QAngle* aim_punch = nullptr;
		QAngle* view_punch = nullptr;

		if ( !pLocal || !engine->IsInGame( ) )
		{
			g_Local = nullptr; 
			g_Weapon = nullptr;
			precached = false;
		}


		if ( engine->IsInGame( ) && pLocal && pLocal->IsAlive( ) && pLocal->IsPlayer( ) )
		{
			g_Local = pLocal;
			g_Weapon = (C_BaseCombatWeapon*)entlist->GetClientEntityFromHandle( g_Local->m_hActiveWeapon( ) );
			server_time = ( ( (float)pLocal->m_nTickBase( ) ) * global->interval_per_tick );


		/*	if ( g_GameRules  )
			{
				if ( g_GameRules->m_bIsDroppingItems( ) )
					ConsolePrint( "dropping weapon \n" );
			} else
			{
				ConsolePrint( "gay af" );
			}*/
				
			if ( !precached )
			{
				if ( PrecacheModel( "models/weapons/eminem/v_knife_tactical_candy.mdl" ) && PrecacheModel( "models/weapons/v_smg_mp5.mdl" ) )
					precached = true;
			}

			//for (auto i = 1; i <= g_EntityList->GetHighestEntityIndex(); ++i)
			//{
			//	//if(GetAsyncKeyState(VK_F4))
			//	//	pResolver2::array_[i].count = 0;

			//	auto entity = C_BasePlayer::GetPlayerByIndex(i);

			//	if (!entity)
			//		continue;

			//	if (entity == pLocal || !entity->IsPlayer())
			//		continue;

			//	if (entity->m_iTeamNum() == pLocal->m_iTeamNum())
			//		continue;
			//	
			//	if (i < 65)
			//	{
			//		if (entity->IsAlive() && !entity->IsDormant())
			//		{
			//			QAngle g = entity->m_angEyeAngles();

			//			entity->m_angEyeAngles() = QAngle(g.pitch, pResolver2::array_[i].yaw, 0.f);
			//		}
			//	}

			//}
		}
		if ( stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START  && engine->IsInGame( ) && pLocal && pLocal->IsAlive( ) )
		{
			for ( int i = 0; i < 13; i++ )
			{
				/*
					bruh observations 
					
				Over 120 delta:
				-weight is 1, except it goes to 0.01 - 0.99 when lby break / animation occurs
				-979 triggered
				-Cycle constantly updating from 0 to 1 gradually increasing, then resetting to 0 and repeating

				Less than 120 delta:
				-979 triggered
				-weight 0
				-cycle is not changing and 0.98-0.9999

				Without AA:
				-979 not triggered
				-Turning right really fast triggers 979
				-Turning left really fast doesn't trigger 979

				*/

				AnimationLayer lol = pLocal->GetAnimOverlay( i );
				Activity activity = (Activity)pLocal->GetSequenceActivity( lol.m_nSequence );


				if ( activity == Activity::ACT_CSGO_IDLE_TURN_BALANCEADJUST && doAa )
				{
					if ( lol.m_flCycle > 0.097f  && lol.m_flWeight == 0.f )
						suppressing_979 = true;

					else  suppressing_979 = false;
				}
			}
			
			pResolver::Run( );
			fake_latency::get( ).UpdateIncomingSequences( );

			auto hWeapons = pLocal->m_hMyWeapons( );
			if ( hWeapons )
			{
				for ( int nIndex = 0; hWeapons[ nIndex ]; nIndex++ )
				{
					//ApplyWeapon( hWeapons[ nIndex ] );
				}
			}

			if ( options.misc_noflash &&  options.vis_misc_enabled )
				pLocal->m_flFlashMaxAlpha( ) = 0;
			else
				pLocal->m_flFlashMaxAlpha( ) = 255.f;

		} else if ( stage == FRAME_NET_UPDATE_POSTDATAUPDATE_END &&  engine->IsInGame( ) && pLocal )
		{
			smoke::get( ).on_framestagenotify( );

			if ( options.wall_colorchanger  && options.vis_misc_enabled )
			{
				static ConVar* staticdrop = g_CVar->FindVar( "r_DrawSpecificStaticProp" );
				static SpoofedConvar* staticdrop_spoofed = new SpoofedConvar( staticdrop );
				staticdrop_spoofed->SetInt( 0 );
			//	staticdrop_spoofed->~SpoofedConvar
				for ( MaterialHandle_t i = g_MatSystem->FirstMaterial2( ); i != g_MatSystem->InvalidMaterial2( ); i = g_MatSystem->NextMaterial2( i ) )
				{
					IMaterial* mat = g_MatSystem->GetMaterial2( i );

					if ( !mat )
						continue;

					//TEXTURE_GROUP_WORLD
					if ( strcmp( mat->GetTextureGroupName( ), "StaticProp textures" ) != 0 && strcmp( mat->GetTextureGroupName( ), TEXTURE_GROUP_WORLD ) != 0 ) //)
						continue;
					if ( SetNewColor || options.color_walls_rainbow )
					{
						Color gay = options.color_walls_rainbow ? Hooks::rainbowCol : options.color_walls;

						float red = (float)( gay.r( ) / 255.f );
						float green = (float)( gay.g( ) / 255.f );
						float blue = (float)( gay.b( ) / 255.f );
						float alpha = (float)( gay.a( ) / 255.f );

						mat->ColorModulate( red, green, blue );
						mat->AlphaModulate( alpha );
						mat->SetMaterialVarFlag( MATERIAL_VAR_ADDITIVE, true );
					}
				}

				if ( SetNewColor ) SetNewColor = false;
			}
		} else if ( stage == FRAME_RENDER_START &&  engine->IsInGame( ) && pLocal )
		{
			if ( pLocal->IsAlive( ) )
			{
				flashlight::get( ).RunFrame( pLocal );

				if ( options.misc_visrcs && options.vis_misc_enabled )
				{
					aim_punch = &pLocal->m_aimPunchAngle( );
					view_punch = &pLocal->m_viewPunchAngle( );

					aim_punch_old = *aim_punch;
					view_punch_old = *view_punch;

					*aim_punch = QAngle( 0, 0, 0 );
					*view_punch = QAngle( 0, 0, 0 );
				}

				*(bool*)OverridePostProcessingDisable = options.misc_postprocess;

				if ( options.aim_pvsfix )
				{
					for ( int i = 1; i <= g_EntityList->GetHighestEntityIndex( ); ++i )
					{
						auto entity = C_BasePlayer::GetPlayerByIndex( i );

						if ( !entity || !entity->IsPlayer( ) || !entity->IsAlive( ) || !entity->IsEnemy( ) )
							continue;

						if ( i > 65 )
							break;

						*(int*)( (uintptr_t)entity + 0xA30 ) = global->framecount; 
						*(int*)( (uintptr_t)entity + 0xA28 ) = 0; 

					}
				}
				//test
				//g_Local->m_fFlags( ) |= FL_WATERJUMP | FL_SWIM | FL_FLY | FL_DISSOLVING | FL_ONFIRE;
			}
		}
		
		thirdperson::get( ).on_framestagenotify( pLocal );

		ofunc( g_CHLClient, stage );


		/*if (local && local->IsAlive())
		{
		static DWORD g_PlayerResource = *(DWORD*)(Utils::PatternScan(GetModuleHandleA("client.dll"), "8B 3D ? ? ? ? 85 FF 0F 84 ? ? ? ? 81 C7") + 2);

		DT_CSPlayerResource* ptrResource = *(DT_CSPlayerResource**)g_PlayerResource;

		DWORD m_nMusicID = (DWORD)ptrResource->m_iCompetitiveWins() + (local->EntIndex() * 4);

		*(PINT)((DWORD)m_nMusicID) = 50;
		}*/


		if ( aim_punch && view_punch )
		{
			*aim_punch = aim_punch_old;
			*view_punch = view_punch_old;
		}
	}

	void __stdcall hkDrawModelExecute( IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld )
	{
		static auto ofunc = mdlrender_hook.get_original<DrawModelExecute>( index::DrawModelExecute );
		context2 = ctx;

		if(engine->IsInGame() )
		chams::get( ).OnDrawModelExecute( ctx, state, pInfo, pCustomBoneToWorld );

		ofunc( g_MdlRender, ctx, state, pInfo, pCustomBoneToWorld );

		//Breaks pulse glow for some reason so P fix ":D"
		if ( ( options.glow_enabled && ( !options.glow_players || ( options.glow_players && options.glow_style_o < 1 ) ) || !options.glow_enabled ) )
			g_MdlRender->ForcedMaterialOverride( nullptr );

	}
}

//TODO: GetHitboxPos really fps expensive,  try to use it less 

//TODO: remove this shit
//
//namespace SDK
//{
//	class KeyValues
//	{
//	public:
//		~KeyValues( )
//		{
//			// 56 57 8B F9 8B 4F 18 85 C9
//			// this SHOULD be calling Plat_Free (it does)
//			// but we should also be manually releasing the object ourselves, fuck that cbf
//			// KeyValues::RemoveEverything(this);
//			// LODWORD(v1) = KeyValuesSystem(this);
//			// result = (*(int(__fastcall **)(__int64, KeyValues *))(*(_QWORD *)v1 + 16LL))(v1, this);
//			g_pMemAlloc->Free( this );
//
//			// I'll implement 'KeyValues::RemoveEverything' if I still get game crashes while the game is in it's closing state
//		}
//
//		// just find any xref's to keyvalues ctor and sig which ever you fancy
//		// E8 ? ? ? ? EB 02 33 C0 8B 7D 10
//		KeyValues* Init( const char* name )
//		{
//			// GetCallAddress is an old function from FuckFace32 for smth I used of his, creds 2 him
//			// (*(PDWORD)(addr + 1)) + addr + 5;
//			static auto ctor = ( KeyValues*( __thiscall* )( void*, const char* ) )( GetCallAddress( GetSignature( "client.dll", "E8 ? ? ? ? EB 02 33 C0 8B 7D 10" ) ) );
//			return ctor( this, name );
//		};
//
//		// xref '%s\\gameinfo.txt'
//		bool LoadFromBuffer( char const* name, const char* buffer )
//		{
//			static auto LoadFromBuffer = ( bool( __thiscall* )( KeyValues*, const char*, const char*, void*, void*, void* ) )( GetSignature( "client.dll", "55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89 4C 24 04" ) );
//			return LoadFromBuffer( this, name, buffer, 0, 0, 0 );
//		}
//	};
//
//	//  v32 = (*(int (__thiscall **)(int, signed int))(*(_DWORD *)kvsystem + 4))(kvsystem, 36);// v32 = (KeyValues*)g_pMemAlloc->Alloc(36)
//	// if ( v32 )
//	//		v33 = (_DWORD *)KeyValues::KeyValues(v32, "WeaponDecal");
//	__forceinline static KeyValues* CreateKV( const char* name )
//	{
//		KeyValues* kv = (KeyValues*)g_pMemAlloc->Alloc( 36 );
//		if ( kv )
//			kv->Init( name );
//		return kv;
//	}
//};
