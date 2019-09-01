#include "glow.hpp"

#include "../valve_sdk/csgostructs.hpp"
#include "../options.hpp"
#include "../Drawing.h"
#include "../Globals.h"
#include "../helpers/utils.hpp"
Glow::Glow()
{
}

Glow::~Glow()
{
    // We cannot call shutdown here unfortunately.
    // Reason is not very straightforward but anyways:
    // - This destructor will be called when the dll unloads
    //   but it cannot distinguish between manual unload 
    //   (pressing the Unload button or calling FreeLibrary)
    //   or unload due to game exit.
    //   What that means is that this destructor will be called
    //   when the game exits.
    // - When the game is exiting, other dlls might already 
    //   have been unloaded before us, so it is not safe to 
    //   access intermodular variables or functions.
    //   
    //   Trying to call Shutdown here will crash CSGO when it is
    //   exiting (because we try to access g_GlowObjManager).
    //
}

void Glow::Shutdown()
{
    // Remove glow from all entities
    for(auto i = 0; i < g_GlowObjManager->m_GlowObjectDefinitions.Count(); i++) {
        auto& glowObject = g_GlowObjManager->m_GlowObjectDefinitions[i];
        auto entity = reinterpret_cast<C_BasePlayer*>(glowObject.m_pEntity);

        if(glowObject.IsUnused())
            continue;

        if(!entity || entity->IsDormant())
            continue;

        glowObject.m_flAlpha = 0.0f;
    }
}

void Glow::Run()
{
    for(auto i = 0; i < g_GlowObjManager->m_GlowObjectDefinitions.Count(); i++)
	{
        auto& glowObject = g_GlowObjManager->m_GlowObjectDefinitions[i];
        auto entity = reinterpret_cast<C_BasePlayer*>(glowObject.m_pEntity);

        if(glowObject.IsUnused())
            continue;

        if(!entity || entity->IsDormant())
            continue;

        auto class_id = entity->GetClientClass()->m_ClassID;
        auto color = Color{};

        switch(class_id) 
		{
            case ClassId_CCSPlayer:
            {
                auto is_enemy = entity->m_iTeamNum() != g_LocalPlayer->m_iTeamNum();

				if (entity == local && options.glow_local && local->IsAlive())
				{

					switch (options.glow_style_local)
					{
						case 0: //normal
							glowObject.m_nGlowStyle = 0;
							glowObject.m_bFullBloomRender = false;
							break;

						case 1: //filled
							glowObject.m_nGlowStyle = 0;
							glowObject.m_bFullBloomRender = true;
							break;

						case 2: //pulsate
							glowObject.m_nGlowStyle = 1;
							glowObject.m_bFullBloomRender = false;
							break;

						case 3: //smooth
							glowObject.m_nGlowStyle = 2;
							glowObject.m_bFullBloomRender = false;
							break;

						case 4: //pulsate + smooth
							glowObject.m_nGlowStyle = 3;
							glowObject.m_bFullBloomRender = false;
							break;
					}

				
					glowObject.m_flBloomAmount = options.glow_bloomamount_local;

					color = options.color_glow_local;
				}
				else if (options.glow_players && entity->IsAlive() && entity != local)
				{
					if (!is_enemy && g_Options.glow_enemies_only)
						continue;

					bool visible = g_LocalPlayer->CanSeePlayer(entity, HITBOX_CHEST);

					if (g_Options.glow_visible_only && !visible)
						continue;

					color = is_enemy ? (visible ? g_Options.color_glow_enemy_vis : g_Options.color_glow_enemy) : (visible ? g_Options.color_glow_ally_vis : g_Options.color_glow_ally);

					if ((is_enemy && g_Options.glow_health_e) || (!is_enemy && g_Options.glow_health_f))
						color = HealthBased(entity);


					if ((is_enemy && g_Options.glow_rb_e) || (!is_enemy && g_Options.glow_rb_f))
						color = Hooks::rainbowCol;

					switch (options.glow_style_o)
					{
						case 0: //normal

							glowObject.m_nGlowStyle = 0;
							glowObject.m_bFullBloomRender = false;
							break;

						case 1: //filled
							glowObject.m_nGlowStyle = 0;
							glowObject.m_bFullBloomRender = true;
							break;

						case 2: //pulsate
							glowObject.m_nGlowStyle = 1;
							glowObject.m_bFullBloomRender = false;
							break;

						case 3: //smooth
							glowObject.m_nGlowStyle = 2;
							glowObject.m_bFullBloomRender = false;
							break;

						case 4: //pul + smooth
							glowObject.m_nGlowStyle = 3;
							glowObject.m_bFullBloomRender = false;
							break;
					}

					glowObject.m_flBloomAmount = options.glow_bloomamount_o;

				}
				else { continue; }

                break;
            }
            case ClassId_CChicken:
                if(!g_Options.glow_chickens)
                    continue;
                entity->m_bShouldGlow() = true;
                color = g_Options.color_glow_chickens;
                break;
            case ClassId_CBaseAnimating:
                if(!g_Options.glow_defuse_kits)
                    continue;
                color = g_Options.color_glow_defuse;
                break;
            case ClassId_CPlantedC4:
                if(!g_Options.glow_planted_c4)
                    continue;
                color = g_Options.color_glow_planted_c4;
                break;
            default:
            {
                if(entity->IsWeapon()) {
                    if(!g_Options.glow_weapons)
                        continue;
                    color = g_Options.color_glow_weapons;
                }
            }
        }

		if (g_Options.glow_rb_o && class_id != ClassId_CCSPlayer)
			color = Hooks::rainbowCol;

		if (class_id != ClassId_CCSPlayer)
		{
			glowObject.m_nGlowStyle = 0;
			glowObject.m_bFullBloomRender = false;
		}

        glowObject.m_flRed = color.r() / 255.0f;
        glowObject.m_flGreen = color.g() / 255.0f;
        glowObject.m_flBlue = color.b() / 255.0f;
        glowObject.m_flAlpha = color.a() / 255.0f;

        glowObject.m_bRenderWhenOccluded = true;
        glowObject.m_bRenderWhenUnoccluded = false;

    }
}
//from source sdk as reference
//struct ShaderStencilState_t
//{
//	bool m_bEnable;
//	int m_FailOp;
//	int m_ZFailOp;
//	int m_PassOp;
//	int m_CompareFunc;
//	int m_nReferenceValue;
//	uint32 m_nTestMask;
//	uint32 m_nWriteMask;
//
//	ShaderStencilState_t( )
//	{
//		m_bEnable = false;
//		m_PassOp = m_FailOp = m_ZFailOp = STENCILOPERATION_KEEP;
//		m_CompareFunc = STENCILCOMPARISONFUNCTION_ALWAYS;
//		m_nReferenceValue = 0;
//		m_nTestMask = m_nWriteMask = 0xFFFFFFFF;
//	}
//};
//
//CGlow::CGlow( void )
//{
//	console->Print( DEBUG, "CGlow loaded \n" );
//
//	full_frame = materialsystem_dll.base->FindTexture( "_rt_FullFrameFB", "RenderTargets", true, 0 );
//	q_size0 = materialsystem_dll.base->FindTexture( "_rt_SmallFB0", "RenderTargets", true, 0 );
//	q_size1 = materialsystem_dll.base->FindTexture( "_rt_SmallFB1", "RenderTargets", true, 0 );
//
//	mat_glow = materialsystem_dll.base->FindMaterial( "dev/glow_color", "Other textures", true, "fuck" );
//	mat_rim = materialsystem_dll.base->FindMaterial( "dev/glow_rim3d", "Other textures", true, "fuck" );
//	mat_down = materialsystem_dll.base->FindMaterial( "dev/glow_downsample", "Other textures", true, "fuck" );
//	mat_blur_x = materialsystem_dll.base->FindMaterial( "dev/glow_blur_x", "Other textures", true, "fuck" );
//	mat_blur_y = materialsystem_dll.base->FindMaterial( "dev/glow_blur_y", "Other textures", true, "fuck" );
//	mat_halo = materialsystem_dll.base->FindMaterial( "dev/halo_add_to_screen", "Other textures", true, "fuck" );
//}
//
//void CGlow::Think( void )
//{
//	if ( !settings->glow_players &&
//		 !settings->glow_world )
//		return;
//
//	auto ctx = materialsystem_dll.base->GetRenderContext( );
//
//	ctx->BeginPIXEvent( 0xFFF5940F, "EntityGlowEffects" );
//	Apply( ctx, settings->glow_alpha );
//}
//
//void CGlow::Render( IMatRenderContext* ctx, float alpha, int x, int y )
//{
//	ctx->PushRenderTargetAndViewport( );
//
//	float org_col[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
//	engine_dll.view->GetColorModulation( org_col );
//	float org_blend = engine_dll.view->GetBlend( );
//
//	ctx->SetRenderTarget( full_frame );
//	ctx->Viewport( 0, 0, x, y );
//
//	ctx->ClearColor3ub( 0, 0, 0 );
//	ctx->ClearBuffers( true, false, false );
//
//	engine_dll.render->ForcedMaterialOverride( mat_glow, OVERRIDE_NORMAL, NULL );
//
//	CBaseEntity* entity;
//
//	if ( settings->glow_players )
//	{
//		for ( size_t i = 0; i < CEntList::instance( ).players.size( ); ++i )
//		{
//			if ( !( entity = client_dll.entlist->GetClientEntity( CEntList::instance( ).players.at( i ) ) ) )
//				continue;
//
//			if ( entity->GetNetworkable( ).IsDormant( ) )
//				continue;
//
//			if ( settings->glow_hide_team && entity->TeamNum( ) == local->TeamNum( ) ) continue;
//			if ( settings->glow_hide_enemy && entity->TeamNum( ) != local->TeamNum( ) ) continue;
//
//			float col[ 4 ];
//
//			if ( entity->TeamNum( ) == 2 )
//			{
//				col[ 0 ] = (float)settings->color_glow_t.r / 255.0f;
//				col[ 1 ] = (float)settings->color_glow_t.g / 255.0f;
//				col[ 2 ] = (float)settings->color_glow_t.b / 255.0f;
//				col[ 3 ] = (float)settings->color_glow_t.a / 255.0f;
//			} else if (  entity->TeamNum(  ) == 3  )
// {
//				col[ 0 ] = (float)settings->color_glow_ct.r / 255.0f;
//			col[ 1 ] = (float)settings->color_glow_ct.g / 255.0f;
//			col[ 2 ] = (float)settings->color_glow_ct.b / 255.0f;
//			col[ 3 ] = (float)settings->color_glow_ct.a / 255.0f;
//		}
//
//		engine_dll.view->SetBlend( alpha );
//		engine_dll.view->SetColorModulation( col );
//
//		RenderableInstance_t instance;
//		instance.m_nAlpha = (uint8_t)( alpha * 255.0f );
//
//		entity->GetRenderable( ).DrawModel( CMaterial::instance( ).GLOW, instance );
//	}
//}
//
//if ( settings->glow_world )
//{
//	for ( size_t i = 0; i < CEntList::instance( ).weapons.size( ); ++i )
//	{
//		if ( !( entity = client_dll.entlist->GetClientEntity( CEntList::instance( ).weapons.at( i ) ) ) )
//			continue;
//
//		if ( entity->OwnerEntity( ) != -1 && (
//			!( entity->GetNetworkable( ).GetClientClass( )->m_ClassID == eClassIDs::ID_CPlantedC4 )
//			&& !( entity->GetNetworkable( ).GetClientClass( )->m_ClassID == eClassIDs::ID_CBaseCSGrenadeProjectile )
//			&& !( entity->GetNetworkable( ).GetClientClass( )->m_ClassID == eClassIDs::ID_CSmokeGrenadeProjectile )
//			&& !( entity->GetNetworkable( ).GetClientClass( )->m_ClassID == eClassIDs::ID_CDecoyProjectile )
//			&& !( entity->GetNetworkable( ).GetClientClass( )->m_ClassID == eClassIDs::ID_CMolotovProjectile ) ) )
//			continue;
//
//		float col[ 4 ];
//		col[ 0 ] = (float)settings->color_glow_world.r / 255.0f;
//		col[ 1 ] = (float)settings->color_glow_world.g / 255.0f;
//		col[ 2 ] = (float)settings->color_glow_world.b / 255.0f;
//		col[ 3 ] = (float)settings->color_glow_world.a / 255.0f;
//
//		engine_dll.view->SetBlend( alpha );
//		engine_dll.view->SetColorModulation( col );
//
//		RenderableInstance_t instance;
//		instance.m_nAlpha = (uint8_t)( alpha * 255.0f );
//
//		entity->GetRenderable( ).DrawModel( STUDIO_RENDER, instance );
//	}
//}
//
//engine_dll.render->ForcedMaterialOverride( NULL, OVERRIDE_NORMAL, NULL );
//engine_dll.view->SetColorModulation( org_col );
//engine_dll.view->SetBlend( org_blend );
//
//ShaderStencilState_t stencilStateDisable;
//stencilStateDisable.m_bEnable = false;
//ctx->SetStencilState( stencilStateDisable );
//
//ctx->PopRenderTargetAndViewport( );
//}
//
//void CGlow::Apply( IMatRenderContext* ctx, float alpha )
//{
//	static bool first = true;
//
//	engine_dll.render->ForcedMaterialOverride( mat_glow, OVERRIDE_NORMAL, NULL );
//
//	ShaderStencilState_t stencilStateDisable;
//	stencilStateDisable.m_bEnable = false;
//	float org_blend = engine_dll.view->GetBlend( );
//
//	engine_dll.view->SetBlend( 0.0f );
//	ctx->OverrideDepthEnable( true, false, false );
//
//	int iNumGlowObjects = 0;
//
//	ShaderStencilState_t stencilState;
//	stencilState.m_bEnable = true;
//	stencilState.m_nReferenceValue = 1;
//	stencilState.m_CompareFunc = SHADER_STENCILFUNC_ALWAYS;
//	stencilState.m_PassOp = SHADER_STENCILOP_KEEP;
//	stencilState.m_FailOp = SHADER_STENCILOP_KEEP;
//	stencilState.m_ZFailOp = SHADER_STENCILOP_SET_TO_REFERENCE;
//
//	ctx->SetStencilState( stencilState );
//
//	CBaseEntity* entity;
//
//	if ( settings->glow_players )
//	{
//		for ( size_t i = 0; i < CEntList::instance( ).players.size( ); ++i )
//		{
//			if ( !( entity = client_dll.entlist->GetClientEntity( CEntList::instance( ).players.at( i ) ) ) )
//				continue;
//
//			if ( entity->GetNetworkable( ).IsDormant( ) )
//				continue;
//
//			if ( settings->glow_hide_team && entity->TeamNum( ) == local->TeamNum( ) ) continue;
//			if ( settings->glow_hide_enemy && entity->TeamNum( ) != local->TeamNum( ) ) continue;
//
//			RenderableInstance_t instance;
//			instance.m_nAlpha = (uint8_t)( alpha * 255.0f );
//
//			entity->GetRenderable( ).DrawModel( CMaterial::instance( ).GLOW, instance );
//
//			iNumGlowObjects++;
//		}
//	}
//
//	if ( settings->glow_world )
//	{
//		for ( size_t i = 0; i < CEntList::instance( ).weapons.size( ); ++i )
//		{
//			if ( !( entity = client_dll.entlist->GetClientEntity( CEntList::instance( ).weapons.at( i ) ) ) )
//				continue;
//
//			if ( entity->OwnerEntity( ) != -1 && (
//				!( entity->GetNetworkable( ).GetClientClass( )->m_ClassID == eClassIDs::ID_CPlantedC4 )
//				&& !( entity->GetNetworkable( ).GetClientClass( )->m_ClassID == eClassIDs::ID_CBaseCSGrenadeProjectile )
//				&& !( entity->GetNetworkable( ).GetClientClass( )->m_ClassID == eClassIDs::ID_CSmokeGrenadeProjectile )
//				&& !( entity->GetNetworkable( ).GetClientClass( )->m_ClassID == eClassIDs::ID_CDecoyProjectile )
//				&& !( entity->GetNetworkable( ).GetClientClass( )->m_ClassID == eClassIDs::ID_CMolotovProjectile ) ) )
//				continue;
//
//			RenderableInstance_t instance;
//			instance.m_nAlpha = (uint8_t)( alpha * 255.0f );
//
//			entity->GetRenderable( ).DrawModel( STUDIO_RENDER, instance );
//
//			iNumGlowObjects++;
//		}
//	}
//
//	ShaderStencilState_t stencilStateWarm;
//	stencilStateWarm.m_bEnable = true;
//	stencilStateWarm.m_nReferenceValue = 2;
//	stencilStateWarm.m_CompareFunc = SHADER_STENCILFUNC_ALWAYS;
//	stencilStateWarm.m_PassOp = SHADER_STENCILOP_SET_TO_REFERENCE;
//	stencilStateWarm.m_FailOp = SHADER_STENCILOP_KEEP;
//	stencilStateWarm.m_ZFailOp = SHADER_STENCILOP_KEEP;
//
//	ctx->SetStencilState( stencilStateWarm );
//
//	if ( settings->glow_players )
//	{
//		for ( size_t i = 0; i < CEntList::instance( ).players.size( ); ++i )
//		{
//			if ( !( entity = client_dll.entlist->GetClientEntity( CEntList::instance( ).players.at( i ) ) ) )
//				continue;
//
//			if ( entity->GetNetworkable( ).IsDormant( ) )
//				continue;
//
//			if ( settings->glow_hide_team && entity->TeamNum( ) == local->TeamNum( ) ) continue;
//			if ( settings->glow_hide_enemy && entity->TeamNum( ) != local->TeamNum( ) ) continue;
//
//			RenderableInstance_t instance;
//			instance.m_nAlpha = (uint8_t)( alpha * 255.0f );
//
//			entity->GetRenderable( ).DrawModel( CMaterial::instance( ).GLOW, instance );
//		}
//	}
//
//	if ( settings->glow_world )
//	{
//		for ( size_t i = 0; i < CEntList::instance( ).weapons.size( ); ++i )
//		{
//			if ( !( entity = client_dll.entlist->GetClientEntity( CEntList::instance( ).weapons.at( i ) ) ) )
//				continue;
//
//			if ( entity->OwnerEntity( ) != -1 && (
//				!( entity->GetNetworkable( ).GetClientClass( )->m_ClassID == eClassIDs::ID_CPlantedC4 )
//				&& !( entity->GetNetworkable( ).GetClientClass( )->m_ClassID == eClassIDs::ID_CBaseCSGrenadeProjectile )
//				&& !( entity->GetNetworkable( ).GetClientClass( )->m_ClassID == eClassIDs::ID_CSmokeGrenadeProjectile )
//				&& !( entity->GetNetworkable( ).GetClientClass( )->m_ClassID == eClassIDs::ID_CDecoyProjectile )
//				&& !( entity->GetNetworkable( ).GetClientClass( )->m_ClassID == eClassIDs::ID_CMolotovProjectile ) ) )
//				continue;
//
//			RenderableInstance_t instance;
//			instance.m_nAlpha = (uint8_t)( alpha * 255.0f );
//
//			entity->GetRenderable( ).DrawModel( STUDIO_RENDER, instance );
//		}
//	}
//
//	ctx->OverrideDepthEnable( false, false, false );
//	engine_dll.view->SetBlend( org_blend );
//	ctx->SetStencilState( stencilStateDisable );
//	engine_dll.render->ForcedMaterialOverride( NULL, OVERRIDE_NORMAL, NULL );
//
//	if ( iNumGlowObjects <= 0 )
//		return;
//
//	int x, y;
//	vguimatsurface_dll.base->GetScreenSize( x, y );
//
//	ctx->BeginPIXEvent( 0xFFF5940F, "RenderGlowModels" );
//	Render( ctx, alpha, x, y );
//
//	ctx->PushRenderTargetAndViewport( );
//
//	int nSrcWidth = x;
//	int nSrcHeight = y;
//	int nViewportX, nViewportY, nViewportWidth, nViewportHeight;
//	ctx->GetViewport( nViewportX, nViewportY, nViewportWidth, nViewportHeight );
//
//	if ( ( q_size0->GetActualWidth( ) != ( x / 4 ) ) || ( q_size0->GetActualHeight( ) != ( y / 4 ) ) )
//	{
//		ctx->SetRenderTarget( q_size0 );
//		ctx->Viewport( 0, 0, q_size0->GetActualWidth( ), q_size0->GetActualHeight( ) );
//		ctx->ClearColor3ub( 0, 0, 0 );
//		ctx->ClearBuffers( true, false, false );
//	}
//
//	ctx->SetRenderTarget( q_size0 );
//	ctx->Viewport( 0, 0, x / 4, y / 4 );
//
//	IMaterialVar *pbloomexpvar = mat_down->FindVar( "$bloomexp", NULL, false );
//	pbloomexpvar->SetFloatValue( settings->glow_exponent );
//
//	IMaterialVar *pbloomsaturationvar = mat_down->FindVar( "$bloomsaturation", NULL, false );
//	pbloomsaturationvar->SetFloatValue( settings->glow_saturation );
//
//	int nFullFbWidth = nSrcWidth;
//	int nFullFbHeight = nSrcHeight;
//
//	ctx->DrawScreenSpaceRectangle( mat_down, 0, 0, nSrcWidth / 4, nSrcHeight / 4,
//								   0, 0, nFullFbWidth - 4, nFullFbHeight - 4,
//								   full_frame->GetActualWidth( ), full_frame->GetActualHeight( ), nullptr, 0, 0 );
//
//	if ( first || ( q_size1->GetActualWidth( ) != ( x / 4 ) ) || ( q_size1->GetActualHeight( ) != ( y / 4 ) ) )
//	{
//		first = false;
//		ctx->SetRenderTarget( q_size1 );
//		ctx->Viewport( 0, 0, q_size1->GetActualWidth( ), q_size1->GetActualHeight( ) );
//		ctx->ClearColor3ub( 0, 0, 0 );
//		ctx->ClearBuffers( true, false, false );
//	}
//
//	ctx->SetRenderTarget( q_size1 );
//	ctx->Viewport( 0, 0, x / 4, y / 4 );
//
//	ctx->DrawScreenSpaceRectangle( mat_blur_x, 0, 0, nSrcWidth / 4, nSrcHeight / 4,
//								   0, 0, nSrcWidth / 4 - 1, nSrcHeight / 4 - 1,
//								   q_size0->GetActualWidth( ), q_size0->GetActualHeight( ), nullptr, 0, 0 );
//
//	ctx->SetRenderTarget( q_size0 );
//	ctx->Viewport( 0, 0, x / 4, y / 4 );
//
//	IMaterialVar *pBloomAmountVar = mat_blur_y->FindVar( "$bloomamount", NULL, false );
//	pBloomAmountVar->SetFloatValue( settings->glow_bloom );
//
//	ctx->DrawScreenSpaceRectangle( mat_blur_y, 0, 0, nSrcWidth / 4, nSrcHeight / 4,
//								   0, 0, nSrcWidth / 4 - 1, nSrcHeight / 4 - 1,
//								   q_size1->GetActualWidth( ), q_size1->GetActualHeight( ), nullptr, 0, 0 );
//
//	ctx->PopRenderTargetAndViewport( );
//
//	IMaterialVar *pDimVar = mat_halo->FindVar( "$C0_X", NULL, false );
//	pDimVar->SetFloatValue( 1.0f );
//
//	ShaderStencilState_t stencilStateLast;
//	stencilStateLast.m_bEnable = true;
//	stencilStateLast.m_nWriteMask = 0x0;
//	stencilStateLast.m_nTestMask = 0x3;
//	stencilStateLast.m_nReferenceValue = 0x0;
//	stencilStateLast.m_CompareFunc = SHADER_STENCILFUNC_EQUAL;
//	stencilStateLast.m_PassOp = SHADER_STENCILOP_KEEP;
//	stencilStateLast.m_FailOp = SHADER_STENCILOP_KEEP;
//	stencilStateLast.m_ZFailOp = SHADER_STENCILOP_KEEP;
//	ctx->SetStencilState( stencilStateLast );
//
//	ctx->DrawScreenSpaceRectangle( mat_halo, 0, 0, nViewportWidth, nViewportHeight,
//								   0.0f, -0.5f, nSrcWidth / 4 - 1, nSrcHeight / 4 - 1,
//								   q_size1->GetActualWidth( ),
//								   q_size1->GetActualHeight( ), nullptr, 0, 0 );
//
//	ctx->SetStencilState( stencilStateDisable );
//}