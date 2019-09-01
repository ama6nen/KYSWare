#pragma once

#include <string>
#include "color.h"
#include <Windows.h>

#define VK_0 0x30
#define VK_1 0x31
#define VK_2 0x32
#define VK_3 0x33
#define VK_4 0x34
#define VK_5 0x35
#define VK_6 0x36
#define VK_7 0x37
#define VK_8 0x38
#define VK_9 0x39
#define VK_A 0x41
#define VK_B 0x42
#define VK_C 0x43
#define VK_D 0x44
#define VK_E 0x45
#define VK_F 0x46
#define VK_G 0x47
#define VK_H 0x48
#define VK_I 0x49
#define VK_J 0x4A
#define VK_K 0x4B
#define VK_L 0x4C
#define VK_M 0x4D
#define VK_N 0x4E
#define VK_O 0x4F
#define VK_P 0x50
#define VK_Q 0x51
#define VK_R 0x52
#define VK_S 0x53
#define VK_T 0x54
#define VK_U 0x55
#define VK_V 0x56
#define VK_W 0x57
#define VK_X 0x58
#define VK_Y 0x59
#define VK_Z 0x5A
#define VK_ALT VK_MENU
#define VK_CAPS VK_CAPITAL

class Config
{
public:
#define OPTION(type, var, val) type var = val
	// ESP
	OPTION( bool, esp_enabled, true );
	OPTION( bool, esp_enemies_only, false );
	OPTION( bool, esp_player_boxes, true );
	OPTION( bool, esp_player_outlines, true ); //stencil 
	OPTION( bool, esp_player_names, true );
	OPTION( bool, esp_player_health, true );
	OPTION( bool, esp_player_armour, true );
	OPTION( bool, esp_player_weapons, true );
	OPTION( bool, esp_player_snaplines, false );

	OPTION( bool, esp_dropped_weapons, false );
	OPTION( bool, esp_defuse_kit, false );
	OPTION( bool, esp_planted_c4, false );



	//AIM
	OPTION( bool, aim_enabled, true );
	OPTION( bool, aim_norecoil, true );
	OPTION( bool, aim_bot, true );
	OPTION( float, aim_fov, 180.f );
	OPTION( bool, aim_autoshoot, true );

	OPTION( int, aim_hitchance, 60 );
	OPTION( int, aim_hitchance_move, 35 );
	OPTION( int, min_dmg, 10 );
	OPTION( int, min_dmgawall, 10 );
	OPTION( float, aim_ps, 0.5f );
	OPTION( int, aim_method, 0 );
	OPTION( int, aim_baimlower, 25 );
	OPTION( int, baim_shots, 10 );
	OPTION( bool, baim_lethal, true );
	OPTION( bool, aim_autoscope, true );
	OPTION( bool, aim_autostop, false );
	OPTION( bool, aim_autocrouch, false );
	OPTION( bool, aim_nointerp, true );
	OPTION( bool, aim_nospread, false );
	OPTION( bool, aim_pSilent, false );
	OPTION( bool, aim_silent, true );
	OPTION( bool, aim_pvsfix, true );
	OPTION( bool, aim_hitscan, true );
	OPTION( bool, aim_dmghitbox, false );
	OPTION( bool, autowall, true );
	OPTION( bool, engine_pred, true );
	OPTION( bool, autobuy, true );

	//hitscan
	OPTION( int, hit_priority, 0 );
	OPTION( bool, hit_head, true );
	OPTION( bool, hit_neck, true );
	OPTION( bool, hit_chest, true );
	OPTION( bool, hit_pelvis, true );
	OPTION( bool, hit_stomach, true );
	OPTION( bool, hit_arms, true );
	OPTION( bool, hit_legs, true );
	OPTION( bool, hit_hands, false );
	OPTION( bool, hit_feet, false );


	//anti aim shit
	OPTION( bool, aa_real_jitter, false );
	OPTION( int, aa_real_jitter_range, 30 );
	OPTION( int, aa_real_jitter_wait, 0 );
	OPTION( bool, aa_real_jitter_lby, false );

	OPTION( bool, aa_real_switch, false );
	OPTION( int, aa_real_switch_value, 35 );
	OPTION( int, aa_real_switch_wait, 100 );
	OPTION( bool, aa_real_switch_lby, false );

	OPTION( bool, aa_real_spin, false );
	OPTION( float, aa_real_spin_speed, 15 );
	OPTION( float, aa_real_spin_lbyr, 35 );

	OPTION( int, aa_real_opposite, 0 );
	OPTION( float, aa_real_addyaw, 180.f );

	OPTION( float, switch_value1, 45.f );
	OPTION( float, switch_value2, -45.f );




	OPTION( float, chams1, 1.f );
	OPTION( float, chams2, 1.f );
	OPTION( float, chams3, 1.f );

	OPTION( bool, asd, false );


	OPTION( bool, aa_lby_invertpitch, false );
	OPTION( bool, aa_lby_antibacktrack, false );

	// GLOW
OPTION( bool, glow_enabled, false );
	OPTION( bool, glow_players, false );
		OPTION( bool, glow_enemy, false );
			OPTION( int, glow_enemy_type, 0 );
			OPTION( int, glow_enemy_colortype, 0 );
			OPTION( int, glow_enemy_filter, 0 );
			OPTION( float, glow_enemy_bloom, 1.f );
			OPTION( Color, color_glow_enemy, Color( 255, 0, 0 ) );
			OPTION( Color, color_glow_enemy_vis, Color( 255, 128, 0 ) );
		OPTION( bool, glow_team, false );
			OPTION( int, glow_team_type, 0 );
			OPTION( int, glow_team_colortype, 0 );
			OPTION( int, glow_team_filter, 0 );
			OPTION( float, glow_team_bloom, 1.f );
			OPTION( Color, color_glow_team, Color( 0, 128, 255 ) );
			OPTION( Color, color_glow_team_vis, Color( 0, 128, 0 ) );
		OPTION( bool, glow_local, false );
			OPTION( int, glow_local_type, 0 );
			OPTION( int, glow_local_colortype, 0 );
			OPTION( float, glow_local_bloom, 1.f );
			OPTION( Color, color_glow_local, Color( 255, 128, 69 ) );
	OPTION( bool, glow_chickens, false );
	OPTION( bool, glow_planted_c4, false );
	OPTION( bool, glow_defuse_kits, false );
	OPTION( bool, glow_weapons, false );	

		
	//MISC VISUALZ
	OPTION( bool, vis_misc_enabled, true );
	OPTION( bool, vis_fovcircle, false );
	OPTION( bool, vis_spreadcircle, false );
	OPTION( bool, esp_crosshair, false );
	OPTION( bool, wall_colorchanger, false );
	OPTION( bool, particleremove, false );
	OPTION( int, smoke_style, 0 );
	OPTION( int, selected_sky, 0 );

	//Flashlight
	OPTION( bool, flashlight_on, false );
	OPTION( float, flashlight_fov, 36.f );

	//fake angles chams
	OPTION( bool, fakechams_enabled, true );
	OPTION( bool, fakechams_fake, true );
	OPTION( bool, fakechams_lby, true );
	OPTION( bool, fakechams_fix, false );
	OPTION( float, fakechams_fix_val, 35.f );


	//overridecfg
	OPTION( bool, vis_overridecfg, false );
	OPTION( bool, viso_mipmap, false );
	OPTION( bool, viso_fullbright, false );
	OPTION( bool, viso_drawgray, false );
	OPTION( bool, viso_lowres, false );
	OPTION( bool, viso_trans, false );
	OPTION( bool, viso_visf, false );
	OPTION( bool, viso_meaf, false );
	OPTION( bool, viso_suppressrend, false );
	OPTION( bool, viso_revd, false );


	//aa lines
	OPTION( bool, aal, true );
	OPTION( bool, aal_cross, true );
	OPTION( bool, aal_feet, true );
	OPTION( bool, aal_other, false );
	OPTION( bool, aal_fake, true );
	OPTION( bool, aal_real, true );
	OPTION( bool, aal_lby, true );
	OPTION( bool, aal_simple, false );


	// CHAMS
	OPTION( bool, chams_player_enabled, true );
	OPTION( bool, chams_player_enemies_only, false );
	OPTION( bool, chams_player_wireframe, false );
	OPTION( bool, chams_player_flat, false );
	OPTION( bool, chams_player_ignorez, false );
	OPTION( bool, chams_player_glass, false );
	OPTION( bool, chams_arms_enabled, false );
	OPTION( bool, chams_arms_wireframe, false );
	OPTION( bool, chams_arms_flat, false );
	OPTION( bool, chams_arms_ignorez, false );
	OPTION( bool, chams_arms_glass, false );


	//Dlights & Elights
	OPTION( bool, dlights, false );
	OPTION( bool, elights, false );
	OPTION( float, dlight_radius, 50.f );
	OPTION( int, dlight_exponent, 10 );
	OPTION( float, dlight_decay, 40.f );
	OPTION( float, dlight_die, 40.f );
	OPTION( bool, dlights_enemy, false );
	OPTION( bool, dlights_team, false );
	OPTION( bool, dlights_local, false );
	OPTION( bool, dlights_rainbow, false );


	// MISC
	OPTION( bool, misc_enabled, true );
	OPTION( bool, misc_bhop, true );
	OPTION( bool, misc_autostrafe, true );
	OPTION( bool, misc_visrcs, true );
	OPTION( bool, misc_no_hands, true );
	OPTION( bool, misc_nofog, true );
	OPTION( bool, misc_noscope, true );
	OPTION( bool, misc_scopelines, true );
	OPTION( bool, misc_noflash, false );
	OPTION( bool, misc_freecam, false );
	OPTION( bool, misc_hitmarker, true );
	OPTION( bool, misc_invenabler, true );
	OPTION( bool, misc_sniper_crosshair, true );
	OPTION( bool, misc_rank_reveal, true );
	OPTION( bool, misc_remove_blur, true );
	OPTION( bool, misc_fov_sniper, true );
	OPTION( bool, misc_fakezoom, false );
	OPTION( bool, misc_postprocess, true );


	OPTION( float, fov, 0.f );
	OPTION( float, viewmodel_fov, 0.f );
	OPTION( float, weapon_sway, 1.6f );
	OPTION( bool, misc_eventlag, false );
	OPTION( float, fake_latency_amount, 300.f );
	OPTION( bool, fake_latency, false );
	OPTION( bool, aimbient_lighting, false );
	OPTION( bool, clientside_name, false );
	OPTION( bool, clientside_hide, false );
	OPTION( bool, clientside_pic_bool, false );
	OPTION( long long, clientside_pic, 76561198316747508 );
	OPTION( bool, clientside_local_bool, false );
	OPTION( bool, clientside_other_bool, false );
	OPTION( bool, misc_aw_crasher, false );
	OPTION( bool, misc_client_lagger, false );
	OPTION( int, misc_proto_ban_type, 0 );

	//thirdperson
	OPTION( bool, misc_thirdperson_enabled, true );
	OPTION( bool, misc_thirdperson_enabled2, false );
	OPTION( int, misc_thirdperson_type, 2 );
	OPTION( float, misc_thirdperson_dist, 110.f );
	OPTION( int, thirdperson_button, VK_F1 );


	//fakekill
	OPTION( bool, fake_kill, false );
	OPTION( bool, fake_kill_hs, false );
	OPTION( bool, fake_kill_wall, false );
	OPTION( bool, fake_kill_domin, false );
	OPTION( bool, fake_kill_revenge, false );
	OPTION( bool, fake_kill_name, false );

	OPTION( float, rainbow_speed, 6.f );

	OPTION( float, mirror_x, 0.f );
	OPTION( float, mirror_y, 0.f );
	OPTION( float, mirror_w, 100.f );
	OPTION( float, mirror_h, 100.f );
	OPTION( int, highlighted_meme, -1 );

	OPTION( bool, worldlights_enabled, false );

	OPTION( Color, worldlights, Color( 101, 200, 51 ) );
	// COLORS
	OPTION( Color, color_esp_ally_visible, Color( 52, 152, 219 ) ); //
	OPTION( Color, color_esp_enemy_visible, Color( 46, 204, 0 ) );
	OPTION( Color, color_esp_ally_occluded, Color( 155, 89, 182 ) );
	OPTION( Color, color_esp_enemy_occluded, Color( 231, 76, 60 ) );
	OPTION( Color, color_esp_crosshair, Color( 255, 255, 255 ) );
	OPTION( Color, color_esp_weapons, Color( 128, 0, 128 ) );
	OPTION( Color, color_esp_defuse, Color( 0, 128, 255 ) );
	OPTION( Color, color_esp_c4, Color( 255, 255, 0 ) );

	OPTION( Color, color_glow_chickens, Color( 0, 128, 0 ) );
	OPTION( Color, color_glow_planted_c4, Color( 128, 0, 128 ) );
	OPTION( Color, color_glow_defuse, Color( 255, 255, 255 ) );
	OPTION( Color, color_glow_weapons, Color( 255, 128, 0 ) );

	OPTION( Color, color_chams_player_ally_visible, Color( 52, 152, 219 ) );
	OPTION( Color, color_chams_player_ally_occluded, Color( 155, 89, 182 ) );
	OPTION( Color, color_chams_player_enemy_visible, Color( 46, 204, 0 ) );
	OPTION( Color, color_chams_player_enemy_occluded, Color( 231, 76, 60 ) );
	OPTION( Color, color_chams_arms_visible, Color( 0, 128, 255 ) );
	OPTION( Color, color_chams_arms_occluded, Color( 0, 128, 255 ) );

	//dlight
	OPTION( Color, color_dlight_local, Color( 255, 255, 0 ) );
	OPTION( Color, color_dlight_enemy, Color( 255, 0, 255 ) );
	OPTION( Color, color_dlight_team, Color( 0, 255, 255 ) );

	//fake chams
	OPTION( Color, color_fakechams_lby, Color( 210, 105, 30, 210 ) );
	OPTION( Color, color_fakechams_fake, Color( 255, 0, 0, 210 ) );

	//misc
	OPTION( Color, mat_ambient, Color( 255, 128, 128 ) );
	OPTION( Color, color_walls, Color( 0, 128, 255, 255 ) );

	OPTION( bool, color_walls_rainbow, false );

	//menu
	OPTION( Color, menu_button, Color( 0, 128, 255, 255 ) );
	OPTION( Color, menu_button_active, Color( 0, 128, 255, 255 ) );
	OPTION( Color, menu_button_hovered, Color( 0, 128, 255, 255 ) );
	OPTION( Color, Text, Color( 0, 128, 255, 255 ) );
	OPTION( Color, TextDisabled, Color( 0, 128, 255, 255 ) );
	OPTION( Color, WindowBg, Color( 0, 128, 255, 255 ) );
	OPTION( Color, ChildWindowBg, Color( 0, 128, 255, 255 ) );
	OPTION( Color, PopupBg, Color( 0, 128, 255, 255 ) );
	OPTION( Color, Border, Color( 0, 128, 255, 255 ) );
	OPTION( Color, BorderShadow, Color( 0, 128, 255, 255 ) );

	OPTION( Color, ImGuiCol_FrameBg, Color( 0, 128, 255, 255 ) );
	OPTION( Color, ImGuiCol_FrameBgHovered, Color( 0, 128, 255, 255 ) );
	OPTION( Color, ImGuiCol_FrameBgActive, Color( 0, 128, 255, 255 ) );
	OPTION( Color, ImGuiCol_TitleBg, Color( 0, 128, 255, 255 ) );
	OPTION( Color, ImGuiCol_TitleBgCollapsed, Color( 0, 128, 255, 255 ) );
	OPTION( Color, ImGuiCol_TitleBgActive, Color( 0, 128, 255, 255 ) );

	OPTION( Color, ImGuiCol_MenuBarBg, Color( 0, 128, 255, 255 ) );
	OPTION( Color, ImGuiCol_ScrollbarBg, Color( 0, 128, 255, 255 ) );
	OPTION( Color, ImGuiCol_ScrollbarGrab, Color( 0, 128, 255, 255 ) );
	OPTION( Color, ImGuiCol_ScrollbarGrabHovered, Color( 0, 128, 255, 255 ) );
	OPTION( Color, ImGuiCol_ScrollbarGrabActive, Color( 0, 128, 255, 255 ) );
	OPTION( Color, ImGuiCol_ComboBg, Color( 0, 128, 255, 255 ) );
	OPTION( Color, ImGuiCol_CheckMark, Color( 0, 128, 255, 255 ) );

	OPTION( Color, ImGuiCol_SliderGrab, Color( 0, 128, 255, 255 ) );
	OPTION( Color, ImGuiCol_SliderGrabActive, Color( 0, 128, 255, 255 ) );
	OPTION( Color, ImGuiCol_Header, Color( 0, 128, 255, 255 ) );
	OPTION( Color, ImGuiCol_HeaderHovered, Color( 0, 128, 255, 255 ) );
	OPTION( Color, ImGuiCol_HeaderActive, Color( 0, 128, 255, 255 ) );
	OPTION( Color, ImGuiCol_Column, Color( 0, 128, 255, 255 ) );
	OPTION( Color, ImGuiCol_ColumnHovered, Color( 0, 128, 255, 255 ) );
	OPTION( Color, ImGuiCol_ColumnActive, Color( 0, 128, 255, 255 ) );
	OPTION( Color, ImGuiCol_ResizeGrip, Color( 0, 128, 255, 255 ) );
	OPTION( Color, ImGuiCol_ResizeGripHovered, Color( 0, 128, 255, 255 ) );
	OPTION( Color, ImGuiCol_ResizeGripActive, Color( 0, 128, 255, 255 ) );
	OPTION( Color, ImGuiCol_CloseButton, Color( 0, 128, 255, 255 ) );
	OPTION( Color, ImGuiCol_CloseButtonHovered, Color( 0, 128, 255, 255 ) );
	OPTION( Color, ImGuiCol_CloseButtonActive, Color( 0, 128, 255, 255 ) );
	OPTION( Color, ImGuiCol_PlotLines, Color( 0, 128, 255, 255 ) );
	OPTION( Color, ImGuiCol_PlotLinesHovered, Color( 0, 128, 255, 255 ) );
	OPTION( Color, ImGuiCol_PlotHistogram, Color( 0, 128, 255, 255 ) );
	OPTION( Color, ImGuiCol_PlotHistogramHovered, Color( 0, 128, 255, 255 ) );
	OPTION( Color, ImGuiCol_TextSelectedBg, Color( 0, 128, 255, 255 ) );
	OPTION( Color, ImGuiCol_ModalWindowDarkening, Color( 0, 128, 255, 255 ) );

};

extern Config g_Options;
extern bool   g_Unload;
