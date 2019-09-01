#include "events.h"
#include "Globals.h"
#include "valve_sdk\csgostructs.hpp"
#include "helpers\utils.hpp"
#include "resolver.h"
#include "hitmarker.h"
#include <random>
#include "drawing.h"

namespace events
{

	float get_random( )
	{
		static std::default_random_engine e;
		static std::uniform_real_distribution<> dis( 0, 1 ); // rage 0 - 1
		return dis( e );
	}

	const char* CT_Buy[] =
	{
		"buy scar20",
		"buy vesthelm",
		"buy elite",
		"buy defuser",
		"buy hegrenade",
		"buy incgrenade",
		"buy smokegrenade",
		"buy flashbang",
	};
	const char* T_Buy[] =
	{
		"buy g3sg1",
		"buy vesthelm",
		"buy elite",
		"buy defuser",
		"buy hegrenade",
		"buy molotov",
		"buy smokegrenade",
		"buy flashbang",
	};

	const char* hitgroup_tostring( int hitgroup )
	{
		switch ( hitgroup )
		{
			default:
			case 0:
				return "generic";

			case 1:
				return "head";

			case 2:
				return "chest";

			case 3:
				return "stomach";

			case 4:
				return "left arm";

			case 5:
				return "right arm";

			case 6:
				return "left leg";

			case 7:
				return "right leg";

			case 10:
				return "gear";

		}
	}
	void event_listener::FireGameEvent( IGameEvent *pEvent )
	{
		if ( !pEvent )
		{
			Utils::ConsolePrint( "event is nullptr \n" );
			return;
		}

		const char* name = pEvent->GetName( );

		if ( !strcmp( name, "player_death" ) )
		{
			if ( g_EngineClient->GetPlayerForUserID( pEvent->GetInt( "attacker" ) ) == g_LocalPlayer->EntIndex( ) )
			{
				if ( pEvent->GetInt( "headshot" ) == 1 )
				{
					int guy = g_EngineClient->GetPlayerForUserID( pEvent->GetInt( "userid" ) );
					Utils::ConsolePrint( "player_death (HS) with resolver: %i\n", (int)pResolver2::array_[ guy ].res_state );
					pResolver2::array_[ guy ].method_use = (int)pResolver2::array_[ guy ].res_state;



				}
				//	Utils::ConsolePrint("wpn: %s | %s\n", pEvent->GetString("weapon_fauxitemid"), pEvent->GetString("weapon_itemid"));
				if ( !g_Options.fake_kill )
					return;


				if ( g_Options.fake_kill_hs )
					pEvent->SetInt( "headshot", 1 );

				if ( g_Options.fake_kill_name )
					pEvent->SetInt( "attacker", 0 );

				if ( g_Options.fake_kill_domin )
					pEvent->SetInt( "dominated", 1 );

				if ( g_Options.fake_kill_wall )
					pEvent->SetInt( "penetrated", 1 );

				if ( g_Options.fake_kill_revenge )
					pEvent->SetInt( "revenge", 1 );
			}
		}

		else if ( !strcmp( name, "player_hurt" ) )
		{
			if ( g_EngineClient->GetPlayerForUserID( pEvent->GetInt( "attacker" ) ) == g_LocalPlayer->EntIndex( ) )
			{
				int guy = g_EngineClient->GetPlayerForUserID( pEvent->GetInt( "userid" ) );
				int hitgroup = pEvent->GetInt( "hitgroup" );
				C_BasePlayer* hurt_player = (C_BasePlayer*)entlist->GetClientEntity( guy );

				if ( hurt_player && hurt_player != local )
				{

					if ( g_Options.misc_hitmarker )
					{
						Vector hitgrouppos;
						if ( hurt_player->GetHitgroupPos( hitgroup, hitgrouppos ) )
						{
							hitmarker::get( ).players.push_back( hitmarker::info( global->curtime, hitgrouppos ) );
						//	PlaySoundA(skeetsound, NULL, SND_ASYNC | SND_MEMORY);
							g_VGuiSurface->PlaySound_( "buttons\\arena_switch_press_02.wav" );
						}
					}

					player_info_t info = hurt_player->GetPlayerInfo( );


					studiohdr_t* pStudioModel = g_MdlInfo->GetStudiomodel( hurt_player->GetModel( ) );
					if ( pStudioModel )
					{
						matrix3x4_t matrix[ MAXSTUDIOBONES ];
						if ( hurt_player->SetupBones( matrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, global->curtime ) )
						{
							mstudiohitboxset_t* pHitboxSet = pStudioModel->GetHitboxSet( 0 );
							if ( pHitboxSet )
							{
								for ( int i = 0; i < pHitboxSet->numhitboxes; i++ )
								{
									mstudiobbox_t* pHitbox = pHitboxSet->GetHitbox( i );
									if ( !pHitbox )
										continue;


									if ( pHitbox->m_flRadius > -1 )
									{
										Vector min, max;

										Math::VectorTransform( pHitbox->bbmin, matrix[ pHitbox->bone ], min );
										Math::VectorTransform( pHitbox->bbmax, matrix[ pHitbox->bone ], max );
										g_DebugOverlay->DrawPill2( min, max, pHitbox->m_flRadius, 235, 15, 15, 255, 2.2f );
									}
								}
							}
						}
					}


					int damage = pEvent->GetInt( "dmg_health" );
					int new_health = hurt_player->m_iHealth( ) - damage;
					bool is_dead = ( new_health <= 0 );
					static const char* hit_msg = "Hit""\x0C"" %s ""\x01""in ""\x04""%s""\x01"" for ""\x02""%d""\x01"" dmg (""\x02""%d""\x01"" hp left)";
					static const char* hit_msg_unkw = "Damaged""\x0C"" %s ""\x01""for ""\x02""%d""\x01"" dmg (""\x02""%d""\x01"" hp left)";
					static const char* hit_msg_dead_unkwn = "Killed""\x0C"" %s ""\x01""(dealt ""\x02""%d""\x01"" dmg)";
					static const char* hit_msg_dead = "Killed""\x0C"" %s ""\x01""(hit ""\x04""%s""\x01"" for ""\x02""%d""\x01"" dmg)";

					auto hitstr = hitgroup_tostring( hitgroup );

					if ( is_dead )
					{
						if ( hitstr[ 2 ] == 'n' )
							g_ChatElement->print( hit_msg_dead_unkwn, std::string( info.szName ).c_str( ), damage );
						else
							g_ChatElement->print( hit_msg_dead, std::string( info.szName ).c_str( ), hitstr, damage );
					} 
					else if ( hitstr[ 2 ] == 'n' )
						g_ChatElement->print( hit_msg_unkw, std::string( info.szName ).c_str( ), damage, new_health );
					else
						g_ChatElement->print( hit_msg, std::string( info.szName ).c_str( ), hitstr, damage, new_health );
				}
				if ( hitgroup == 1 )
				{

					Utils::ConsolePrint( "player_hurt (HS) with resolver: %i\n", (int)pResolver2::array_[ guy ].res_state );
					pResolver2::array_[ guy ].method_use = (int)pResolver2::array_[ guy ].res_state;
				} else
				{
					Utils::ConsolePrint( "player_hurt (Not HS) with hitgroup %i\n", pEvent->GetInt( "hitgroup" ) );

				}
			}
		}

		else if ( !strcmp( name, "round_start" ) )
		{

			if ( options.autobuy )
			{
				g_ChatElement->print( "buying weapons" );

				Utils::ConsolePrint( "\n%i", g_LocalPlayer->m_iTeamNum( ) );
				//ct
				if ( g_LocalPlayer->m_iTeamNum( ) == 3 )
				{

					for ( int i = 0; i < ARRAYSIZE( CT_Buy ); i++ )
					{

						g_EngineClient->ClientCmd( CT_Buy[ i ] );
					}
				} else if ( g_LocalPlayer->m_iTeamNum( ) == 2 )
				{
					for ( int i = 0; i < ARRAYSIZE( T_Buy ); i++ )
					{

						g_EngineClient->ClientCmd( T_Buy[ i ] );
					}
				}
			}
		}

		else if ( !strcmp( name, "bullet_impact" ) )
		{
			bool isLocal = ( engine->GetPlayerForUserID( pEvent->GetInt( "userid" ) ) == local );

		//	C_BasePlayer* player = (C_BasePlayer*)engine->GetPlayerForUserID(pEvent->GetInt("userid"));
			if ( local && isLocal ) // && player->m_iTeamNum() != local->m_iTeamNum()
			{
				float x = pEvent->GetFloat( "x" ), y = pEvent->GetFloat( "y" ), z = pEvent->GetFloat( "z" );
				Vector position( x, y, z );

				Ray_t ray;
				ray.Init( local->GetEyePos( ), position );

				CTraceFilter filter;
				filter.pSkip = local;

				trace_t tr;
				enginetrace->TraceRay( ray, MASK_SHOT, &filter, &tr );

				auto color = ( tr.hit_entity && reinterpret_cast<C_BasePlayer*>( tr.hit_entity )->IsPlayer( ) ) ? Color( 0, 255, 0, 220 ) : Color( 255, 0, 0, 220 );

			/*	g_DebugOverlay->AddLineOverlay(local->GetEyePos(), position, 255, 255, 0, true, 2.f);

				g_DebugOverlay->AddBoxOverlay(position, Vector(-2, -2, -2), Vector(2, 2, 2), QAngle(0,0,0), color.r(), color.g(), color.b(), 160.f, 2.f);*/

				//g_DebugOverlay->AddSphereOverlay( position, 10.f, 90, 180, 255, 255, 0, 255, 2.f );
				QAngle ang;
				engine->GetViewAngles( ang );
				float flBaseSize = 10;
				float flHeight = 80;
				Vector vBasePos = local->m_vecOrigin( ) + Vector( 0, 0, 3 );
				QAngle angles( 0, 0, 0 );
				angles.yaw = ang.yaw;
				angles.pitch = ang.pitch;
				Vector vForward, vRight, vUp;
				Math::AngleVectors( angles, vForward, vRight, vUp );
			/*	g_DebugOverlay->AddTriangleOverlay( vBasePos + vRight * flBaseSize / 2, vBasePos - vRight * flBaseSize / 2, vBasePos + vForward * flHeight, 255, 0, 0, 255, false, 2.5f );*/


				Vector p1 = position + Vector( 0, 10, 0 );
				Vector p2 = position + Vector( -10, -10, 0 );
				Vector p3 = position + Vector( 10, -10, 0 );
				g_DebugOverlay->AddTriangleOverlay( position + vRight * flBaseSize, position - vRight * flBaseSize, position + vUp * flBaseSize, 128, 0, 128, 255, false, 2.5f );

				using FX_TeslaFn = void( __thiscall* )( CTeslaInfo& );
				using FX_GunshipImpactFn = void( __cdecl* )( const Vector &origin, const QAngle &angles, float scale, int numParticles, unsigned char *pColor, int iAlpha );
				using FX_ElectricSparkFn = void( __thiscall* ) ( const Vector &pos, int nMagnitude, int nTrailLength, const Vector *vecDir );

				static FX_TeslaFn meme = (FX_TeslaFn)Utils::PatternScan( LI_FIND_CACHED( GetModuleHandleA )( "client.dll" ), "55 8B EC 81 EC ? ? ? ? 56 57 8B F9 8B 47 18" );
				static FX_GunshipImpactFn meme_2 = (FX_GunshipImpactFn)Utils::PatternScan( LI_FIND_CACHED( GetModuleHandleA )( "client.dll" ), "55 8B 6B 04 89 6C 24 04 8B EC 83 EC 58 89 4D C0" );
				static FX_ElectricSparkFn spark = (FX_ElectricSparkFn)Utils::PatternScan( LI_FIND_CACHED( GetModuleHandleA )( "client.dll" ), "55 8B EC 83 EC 3C 53 8B D9 89 55 FC" );


				CTeslaInfo teslaInfo;
				teslaInfo.m_flBeamWidth = 5.f;
				teslaInfo.m_flRadius = 175.f;
				teslaInfo.m_nEntIndex = 1;

				Color rb = FromHSB( get_random( ), 1.f, 1.f );//= Hooks::rainbowCol;
				teslaInfo.m_vPos = position; //wherever you want it to spawn from, like enemy's head;
				teslaInfo.m_flTimeVisible = 1.05f;
				teslaInfo.m_nBeams = 5;
				teslaInfo.m_pszSpriteName = "sprites/physbeam.vmt"; //physbeam


				Vector dir;
				Math::AngleVectors( QAngle( 0, 0, 0 ), dir );



				Vector dir2;
				Math::AngleVectors( ang, dir2 );
				dir2.z = -100;

			/*	g_IViewRenderBeams->crea
				g_IEffects->Beam(position, dir2, -1, -1, )
				g_IEffects->EnergySplash( position,  -dir2, false );*/
			//	g_IEffects->Smoke( position, -1, 100.f, 3.f );
			//	g_IEffects->Dust( position, local->m_vecViewOffset(), 25.f, 2.f );
				//sub_103C1EC


				BeamInfo_t beamInfo;
				beamInfo.m_nType = TE_BEAMPOINTS;
				beamInfo.m_pszModelName = "sprites/physbeam.vmt"; // sprites/purplelaser1.vmt
				beamInfo.m_nModelIndex = -1;
				beamInfo.m_flHaloScale = 0.0f;
				beamInfo.m_flLife = 3.0f;
				beamInfo.m_flWidth = 3.0f;
				beamInfo.m_flEndWidth = 3.0f;
				beamInfo.m_flFadeLength = 0.0f;
				beamInfo.m_flAmplitude = 2.0f;
				beamInfo.m_flBrightness = 255.f;
				beamInfo.m_flSpeed = 0.2f;
				beamInfo.m_nStartFrame = 0;
				beamInfo.m_flFrameRate = 0.f;
				beamInfo.m_flRed = rb.r( );
				beamInfo.m_flGreen = rb.g( );
				beamInfo.m_flBlue = rb.b( );
				beamInfo.m_nSegments = 2;
				beamInfo.m_bRenderable = true;
			//	beamInfo.m_nFlags = FBEAM_ONLYNOISEONCE | FBEAM_NOTILE | FBEAM_HALOBEAM;

				beamInfo.m_vecStart = local->GetBonePos( 8 ); //local->m_vecOrigin( ) + local->m_vecViewOffset( );
				beamInfo.m_vecEnd = position;

				auto beam = g_IViewRenderBeams->CreateBeamPoints( beamInfo );
			/*	if ( beam )
					g_IViewRenderBeams->DrawBeam( beam );*/
				//	g_IVEffects->
				for ( int i = 1; i < 6; i++ )
				{
					rb = FromHSB( get_random( ), 1.f, 1.f );
					teslaInfo.m_vColor.Init( rb.r( ), rb.g( ), rb.b( ) ); //your value up to 255 / 255.f
					teslaInfo.m_nEntIndex = i;
				//	meme( teslaInfo );
				}
					//meme_2( teslaInfo.m_vPos, ang, 10.f, 4, Color::Red._CColor, 255 );


			//	spark( position, 1, 1, &ang );
			}
		}

	}

	int event_listener::GetEventDebugID( void )
	{
		return 42;
	}
}