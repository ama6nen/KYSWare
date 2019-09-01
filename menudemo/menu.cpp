#pragma once
#include "menu.h"
#include "cfg_manager.h"
static char visuals[15] = "Visuals";
static char hvh[15] = "HvH";
static char misc[15] = "Misc";
static char colors[15] = "Colors";
static char config[15] = "Config";

static char* sidebar_tabs[] = {
	visuals,
	hvh,
	misc,
	colors,
	config
};

struct tab_info
{
	int index;
	char* name;
	bool different_window;
	bool set_new;
	bool set_pos;
	bool show;
	ImVec2 spawn_pos;
};
#define VISUALS 0
#define HVH 1
#define MISC 2
#define COLORS 3
#define CONFIG 4
tab_info tabs_info[ 5 ] = { };

ImGuiContext* p;

using namespace ImGui;

constexpr static float get_sidebar_item_width( )
{
	return 110.0f;
}
constexpr static float get_sidebar_item_height( )
{
	return  60.0f;
}
#define ColorToImVec(floatptr) ImVec4(floatptr[0], floatptr[1], floatptr[2], floatptr[3])
#define GetImvec(cc) ColorToImVec(cc.GetImVec())
void TripleSpacing( )
{

	
	Spacing( ), Spacing( ), Spacing( );
}

bool LabelClick( const char* concatoff, const char* concaton, const char* label, bool* v, const char* unique_id )
{
	ImGuiWindow* window = GetCurrentWindow( );
	if ( window->SkipItems )
		return false;


	// The concatoff/on thingies were for my weapon config system so if we're going to make that, we still need this aids.
	char Buf[ 64 ];
	_snprintf( Buf, 62, "%s%s", ( ( *v ) ? concatoff : concaton ), label );

	ImGuiContext& g = *GImGui;
	ImGuiStyle style = g.Style;
	ImGuiID id = window->GetID( unique_id );
	const ImVec2 label_size = CalcTextSize( label, NULL, true );

	const ImRect check_bb( window->DC.CursorPos, window->DC.CursorPos + ImVec2( label_size.y + style.FramePadding.y * 2, label_size.y + style.FramePadding.y * 2 ) );
	ItemSize( check_bb, style.FramePadding.y );

	ImRect total_bb = check_bb;
	if ( label_size.x > 0 )
		SameLine( 0, style.ItemInnerSpacing.x );

	const ImRect text_bb( window->DC.CursorPos + ImVec2( 0, style.FramePadding.y ), window->DC.CursorPos + ImVec2( 0, style.FramePadding.y ) + label_size );
	if ( label_size.x > 0 )
	{
		ItemSize( ImVec2( text_bb.GetWidth( ), check_bb.GetHeight( ) ), style.FramePadding.y );
		total_bb = ImRect( ImMin( check_bb.Min, text_bb.Min ), ImMax( check_bb.Max, text_bb.Max ) );
	}

	if ( !ItemAdd( total_bb, id ) )
		return false;

	bool hovered, held;
	bool pressed = ButtonBehavior( total_bb, id, &hovered, &held );
	if ( pressed )
		*v = !( *v );

	if ( label_size.x > 0.0f )
		RenderText( check_bb.GetTL( ), Buf );

	return pressed;
}

#define caseStringify(x) case x: return string(#x)

string vkToString( int vk )
{
	char c[ 2 ] = { 0 };
	if ( vk >= '0' && vk <= '9' )
	{
		c[ 0 ] = (char)vk; return string( c );
	}
	if ( vk >= 'A' && vk <= 'Z' )
	{
		c[ 0 ] = (char)vk; return string( c );
	}
	switch ( vk )
	{
		// VK_0 - VK_9 are the same as ASCII '0' - '9' (0x30 - 0x39)
		// 0x40 : unassigned
		// VK_A - VK_Z are the same as ASCII 'A' - 'Z' (0x41 - 0x5A)
		caseStringify( VK_LBUTTON );
		caseStringify( VK_RBUTTON );
		caseStringify( VK_CANCEL );
		caseStringify( VK_MBUTTON );  // NOT contiguous with L & RBUTTON
		caseStringify( VK_XBUTTON1 ); // NOT contiguous with L & RBUTTON
		caseStringify( VK_XBUTTON2 ); // NOT contiguous with L & RBUTTON
		caseStringify( VK_BACK );
		caseStringify( VK_TAB );
		caseStringify( VK_CLEAR );
		caseStringify( VK_RETURN );
		caseStringify( VK_SHIFT );
		caseStringify( VK_CONTROL );
		caseStringify( VK_MENU );
		caseStringify( VK_PAUSE );
		caseStringify( VK_CAPITAL );
		caseStringify( VK_KANA );
		caseStringify( VK_JUNJA );
		caseStringify( VK_FINAL );
		caseStringify( VK_KANJI );
		caseStringify( VK_ESCAPE );
		caseStringify( VK_CONVERT );
		caseStringify( VK_NONCONVERT );
		caseStringify( VK_ACCEPT );
		caseStringify( VK_MODECHANGE );
		caseStringify( VK_SPACE );
		caseStringify( VK_PRIOR );
		caseStringify( VK_NEXT );
		caseStringify( VK_END );
		caseStringify( VK_HOME );
		caseStringify( VK_LEFT );
		caseStringify( VK_UP );
		caseStringify( VK_RIGHT );
		caseStringify( VK_DOWN );
		caseStringify( VK_SELECT );
		caseStringify( VK_PRINT );
		caseStringify( VK_EXECUTE );
		caseStringify( VK_SNAPSHOT );
		caseStringify( VK_INSERT );
		caseStringify( VK_DELETE );
		caseStringify( VK_HELP );
		caseStringify( VK_LWIN );
		caseStringify( VK_RWIN );
		caseStringify( VK_APPS );
		caseStringify( VK_SLEEP );
		caseStringify( VK_NUMPAD0 );
		caseStringify( VK_NUMPAD1 );
		caseStringify( VK_NUMPAD2 );
		caseStringify( VK_NUMPAD3 );
		caseStringify( VK_NUMPAD4 );
		caseStringify( VK_NUMPAD5 );
		caseStringify( VK_NUMPAD6 );
		caseStringify( VK_NUMPAD7 );
		caseStringify( VK_NUMPAD8 );
		caseStringify( VK_NUMPAD9 );
		caseStringify( VK_MULTIPLY );
		caseStringify( VK_ADD );
		caseStringify( VK_SEPARATOR );
		caseStringify( VK_SUBTRACT );
		caseStringify( VK_DECIMAL );
		caseStringify( VK_DIVIDE );
		caseStringify( VK_F1 );
		caseStringify( VK_F2 );
		caseStringify( VK_F3 );
		caseStringify( VK_F4 );
		caseStringify( VK_F5 );
		caseStringify( VK_F6 );
		caseStringify( VK_F7 );
		caseStringify( VK_F8 );
		caseStringify( VK_F9 );
		caseStringify( VK_F10 );
		caseStringify( VK_F11 );
		caseStringify( VK_F12 );
		caseStringify( VK_F13 );
		caseStringify( VK_F14 );
		caseStringify( VK_F15 );
		caseStringify( VK_F16 );
		caseStringify( VK_F17 );
		caseStringify( VK_F18 );
		caseStringify( VK_F19 );
		caseStringify( VK_F20 );
		caseStringify( VK_F21 );
		caseStringify( VK_F22 );
		caseStringify( VK_F23 );
		caseStringify( VK_F24 );
		caseStringify( VK_NUMLOCK );
		caseStringify( VK_SCROLL );
		caseStringify( VK_OEM_NEC_EQUAL );  // '=' key on numpad
		caseStringify( VK_OEM_FJ_MASSHOU ); // 'Unregister word' key
		caseStringify( VK_OEM_FJ_TOUROKU ); // 'Register word' key
		caseStringify( VK_OEM_FJ_LOYA );    // 'Left OYAYUBI' key
		caseStringify( VK_OEM_FJ_ROYA );    // 'Right OYAYUBI' key
		caseStringify( VK_LSHIFT );
		caseStringify( VK_RSHIFT );
		caseStringify( VK_LCONTROL );
		caseStringify( VK_RCONTROL );
		caseStringify( VK_LMENU );
		caseStringify( VK_RMENU );
		caseStringify( VK_BROWSER_BACK );
		caseStringify( VK_BROWSER_FORWARD );
		caseStringify( VK_BROWSER_REFRESH );
		caseStringify( VK_BROWSER_STOP );
		caseStringify( VK_BROWSER_SEARCH );
		caseStringify( VK_BROWSER_FAVORITES );
		caseStringify( VK_BROWSER_HOME );
		caseStringify( VK_VOLUME_MUTE );
		caseStringify( VK_VOLUME_DOWN );
		caseStringify( VK_VOLUME_UP );
		caseStringify( VK_MEDIA_NEXT_TRACK );
		caseStringify( VK_MEDIA_PREV_TRACK );
		caseStringify( VK_MEDIA_STOP );
		caseStringify( VK_MEDIA_PLAY_PAUSE );
		caseStringify( VK_LAUNCH_MAIL );
		caseStringify( VK_LAUNCH_MEDIA_SELECT );
		caseStringify( VK_LAUNCH_APP1 );
		caseStringify( VK_LAUNCH_APP2 );
		caseStringify( VK_OEM_1 );      // ';:' for US
		caseStringify( VK_OEM_PLUS );   // '+' any country
		caseStringify( VK_OEM_COMMA );  // ',' any country
		caseStringify( VK_OEM_MINUS );  // '-' any country
		caseStringify( VK_OEM_PERIOD ); // '.' any country
		caseStringify( VK_OEM_2 );  // '/?' for US
		caseStringify( VK_OEM_3 );  // '`~' for US
		caseStringify( VK_OEM_4 );  //  '[{' for US
		caseStringify( VK_OEM_5 );  //  '\|' for US
		caseStringify( VK_OEM_6 );  //  ']}' for US
		caseStringify( VK_OEM_7 );  //  ''"' for US
		caseStringify( VK_OEM_8 );
		caseStringify( VK_OEM_AX );   //  'AX' key on Japanese AX kbd
		caseStringify( VK_OEM_102 );  //  "<>" or "\|" on RT 102-key kbd.
		caseStringify( VK_ICO_HELP ); //  Help key on ICO
		caseStringify( VK_ICO_00 );   //  00 key on ICO
		caseStringify( VK_PROCESSKEY );
		caseStringify( VK_ICO_CLEAR );
		caseStringify( VK_PACKET );
	}
	c[ 0 ] = (char)vk;
	return string( c );
}

void KeyBindButton( int* key ) //ButtonCode_t*
{
	bool clicked = false;

	std::string text = vkToString(*key);// CW2A( str, CP_WINUNICODE );
	
	// = std::to_string(str); // g_InputSystem->ButtonCodeToString( *key );
	std::string unique_id = std::to_string( (int)*key );

	if ( *key == VK_NONAME )
		text = "-";
	else
		std::transform( text.begin( ), text.end( ), text.begin( ), ::toupper );

	
	if ( Hooks::inputlisten && Hooks::input_receivedKeyval == key )
	{
		clicked = true;
		text = "...";
	}
	text += "]";

	ImGui::SameLine( );

	LabelClick( "[", "[", text.c_str( ), &clicked, unique_id.c_str( ) );

	if ( clicked )
	{
		Hooks::inputlisten = true;
		Hooks::input_receivedKeyval = key;
	//	*key = Hooks
		Menu::get( ).selecting_key = true;
	}

	if ( *key == VK_ESCAPE )
		*key = VK_NONAME;


}
void Keybind( const char* id, int* keybind, int* selected )
{
	ImGui::PushStyleVar( ImGuiStyleVar_WindowMinSize, ImVec2( 0, 0 ) );
	{
		
		static const char* hotkey_select[] = { "Always on", "On Hotkey", "Toggle" , "Off Hotkey" };

		if ( ImGui::BeginPopupContextItem( id ) )
		{
			PushItemWidth( 100.f );
			Combo( id, selected, hotkey_select, 4 );
			PopItemWidth( );


			if ( ImGui::Button( "Close" ) )
				ImGui::CloseCurrentPopup( );

			if ( selected != 0 )
				KeyBindButton( keybind );


			ImGui::EndPopup( );
		}
	}
	ImGui::PopStyleVar( );
}


namespace ImGuiEx
{

	inline bool ColorEdit4( const char* label, Color* v, bool show_alpha = true )
	{
		// ImVec4 clr = ImColor( (float)v->r( ), (float)v->g( ), (float)v->b( ), (float)v->a( ) );
		  auto clr = ImVec4{
			(float)v->r( ) / 255.0f,
			(float)v->g( ) / 255.0f,
			(float)v->b( ) / 255.0f,
			(float)v->a( ) / 255.0f
		};


		 if ( ImGui::ColorEdit4( label, &clr.x, show_alpha ) )
		 {
			 v->SetColor( clr.x, clr.y, clr.z, clr.w );
			 return true;
		 }
		return false;
	}
	inline bool ColorEdit3( const char* label, Color* v )
	{
		return ColorEdit4( label, v, true );
	}

	static auto vector_getter = []( void* vec, int idx, const char** out_text )
	{
		auto& vector = *static_cast<std::vector<std::string>*>( vec );
		if ( idx < 0 || idx >= static_cast<int>( vector.size( ) ) )
		{
			return false;
		}
		*out_text = vector.at( idx ).c_str( );
		return true;
	};

	bool Combo( const char* label, int* currIndex, std::vector<std::string>& values )
	{
		if ( values.empty( ) )
		{
			return false;
		}
		return ImGui::Combo( label, currIndex, vector_getter,
					  static_cast<void*>( &values ), values.size( ) );
	}

	bool ListBox( const char* label, int* currIndex, std::vector<std::string>& values, int height_in_items = -1 )
	{
		/*ListBox( "##cfgs", &cfg_selected,
				 []( void* vec, int index, const char** out ) {
					 auto fuckimgui = reinterpret_cast<std::vector<std::string>*>( vec );
					 if ( index < 0 || index >= fuckimgui->size( ) )return false;
					 *out = fuckimgui->at( index ).c_str( );
					 return true;
				 }, reinterpret_cast<void*>( &cfg_files ), cfg_files.size( ), 7
					 );*/

		if ( values.empty( ) )
		{
			return false;
		}
		return ImGui::ListBox( label, currIndex, vector_getter,
						static_cast<void*>( &values ), values.size( ), height_in_items );
	}

}

void ColorSelect( std::string name, Color* option )
{
	ImGui::PushStyleVar( ImGuiStyleVar_WindowMinSize, ImVec2( 0, 0 ) );
	{
		name += "##clr";
		if ( ImGui::BeginPopupContextItem( name.c_str( ) ) )
		{
			ImGuiEx::ColorEdit3( name.c_str( ), option );

			if ( ImGui::Button( "Close" ) )
				ImGui::CloseCurrentPopup( );

			ImGui::EndPopup( );
		}
	}
	ImGui::PopStyleVar( );
}

bool IsItemActivePreviousFrame( )
{
	ImGuiContext& g = *GImGui;
	if ( g.ActiveIdPreviousFrame )
		return g.ActiveIdPreviousFrame == GImGui->CurrentWindow->DC.LastItemId;
	return false;
}

template<size_t N>
void render_tabs( char* ( &names )[ N ], int& activetab, float w, float h, bool sameline )
{
	bool values[ N ] = { false };
	values[ activetab ] = true;
	static const int COUNT = 5;
	static int items_list[ COUNT ] = { 0, 1, 2, 3, 4 };

	for ( int i = 0; i < N; ++i )
	{
		if ( !sameline )
		{
			int item_no = items_list[ i ];
			if ( tabs_info[ item_no ].different_window )
				continue;

			tabs_info[ i ].index = item_no;

			if ( ImGui::ToggleButton( names[ item_no ], &values[ item_no ], ImVec2{ w, h }, 2 ) )
				activetab = item_no;
			
			if ( ImGui::IsMouseDoubleClicked( 0 ) && !tabs_info[ activetab ].show )
			{
				for ( int cc = 0; cc < 5; cc++ )
					if ( cc != activetab )
						tabs_info[ cc ].show = false;
				
				tabs_info[ activetab ].show = true;
				ImGui::SetKeyboardFocusHere( 1 );
			}
			
			if ( tabs_info[ item_no ].show )
			{
				if ( ImGui::InputText( "", names[ item_no ], 15, ImGuiInputTextFlags_EnterReturnsTrue ) )
					if ( names[ item_no ][ 0 ] != '\0' && names[ item_no ][ 0 ] != '!' )
						tabs_info[ item_no ].show = false;
				
				if ( names[ item_no ][ 0 ] == '\0' )
					names[ item_no ][ 0 ] = '!';
			}
			else 
			{
				if ( ImGui::IsMouseDragging( 0, 0 ) && ImGui::IsItemActive( ) )
				{
					auto dragd = ImGui::GetMouseDragDelta( 0 );
					if ( fabsf( dragd.x ) > 10.f || fabsf( dragd.y ) > 10.f )
					{
						ImDrawList* draw_list = ImGui::GetWindowDrawList( );
						draw_list->PushClipRectFullScreen( );
						ImVec2 pos1 = ImGui::GetIO( ).MousePos + ImVec2( w / 2, h / 2 );
						ImVec2 pos2 = ImGui::GetIO( ).MousePos - ImVec2( w / 2, h / 2 );
						draw_list->AddRect( pos1, pos2, ImGui::GetColorU32( ImGuiCol_ButtonActive ) );
						draw_list->PopClipRect( );
					}	
				}

				if ( ImGui::IsItemActive( ) && !ImGui::IsItemHovered( ) )
				{
					if ( ImGui::IsWindowHovered( ImGuiHoveredFlags_AllowWhenBlockedByActiveItem ) )
					{
						float drag_dy = ImGui::GetMouseDragDelta( 0 ).y;
						if ( drag_dy < 0.0f && i > 0 )
						{
							items_list[ i ] = items_list[ i - 1 ];
							items_list[ i - 1 ] = item_no;
							ImGui::ResetMouseDragDelta( );
						} else if ( drag_dy > 0.0f && i < COUNT - 1 )
						{
							items_list[ i ] = items_list[ i + 1 ];
							items_list[ i + 1 ] = item_no;
							ImGui::ResetMouseDragDelta( );
						}
					}
				} else if ( IsItemActivePreviousFrame( ) && !ImGui::IsWindowHovered( ImGuiHoveredFlags_AllowWhenBlockedByActiveItem ) )
				{
					tabs_info[ item_no ].spawn_pos = ImGui::GetMousePos( );
					tabs_info[ item_no ].set_new = true;
					tabs_info[ item_no ].set_pos = true;
				}
			}
		} 
		else
		{

			if ( ImGui::ToggleButton( names[ i ], &values[ i ], ImVec2{ w, h },1) )
				activetab = i;
			
			if ( i < N - 1 )
				ImGui::SameLine( );
		}	
	}

}

ImVec2 get_sidebar_size( )
{
	constexpr float padding = 2.f;
	constexpr auto size_w = padding * 2.0f + get_sidebar_item_width( );
	constexpr auto size_h = padding * 0.1f + ( sizeof( sidebar_tabs ) / sizeof( char* ) ) * get_sidebar_item_height( );

	return ImVec2{ size_w, ImMax( 0.f, size_h ) }; //change 300 to 385 for original
}

void RenderEspTab( )
{
	static char* esp_tab_names[] = { "Part 1", "Part 2", "Part 3" };
	static int   active_esp_tab = 0;
	static const ImVec4 green = ImVec4( 0.f, 1.f, 0.f, 1.f );
	static const ImVec4 red = ImVec4( 1.f, 0.f, 0.f, 1.f );

	bool placeholder_true = true;
	
	auto& style = ImGui::GetStyle( );
	PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2{ 0,  style.WindowPadding.y } );

	float group_w = ImGui::GetCurrentWindow( )->Size.x - style.WindowPadding.x * 2;
	ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2( 0.f, 9.f ) );
	{
		ImGui::PushStyleVar( ImGuiStyleVar_FrameBorderSize, 0.f );

		render_tabs( esp_tab_names, active_esp_tab, group_w / _countof( esp_tab_names ), 33.0f, true );
	}
	ImGui::PopStyleVar(3 );

	ImGui::BeginChild( "##body_content", ImVec2( 0, 0 ), false );
	{
		if ( active_esp_tab == 0 ) //part 1
		{
			ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2{ style.WindowPadding.x, style.ItemSpacing.y } );
			ImGui::Columns( 3, nullptr, false );
			ImGui::SetColumnOffset( 1, group_w / 3.0f );
			ImGui::SetColumnOffset( 2, 2 * group_w / 3.0f );
			ImGui::SetColumnOffset( 3, 3 * group_w / 3.0f );

			//ESP -  1st column
			{

				SetNextWindowPos( GetWindowPos( ) + ImVec2( 0.f, 5.f ) );
				ImGui::BeginChild( "ESP", ImVec2( 216.f, 239.f ), true, ImGuiWindowFlags_MenuBar );

				if ( ImGui::BeginMenuBar( ) )
				{
					Text( "ESP" );
					//Keybind( "##t2p", &options.thirdperson_button, &options.misc_thirdperson_type );
					ImGui::EndMenuBar( );
				}


				ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2{ 4.f,2.0f } );
				Checkbox( "Enabled##1", &options.esp_enabled );
				ImGui::Separator( );
				Checkbox( "Enemy only##1", &options.esp_enemies_only );
				Checkbox( "Bounding box##1", &options.esp_player_boxes );
				Checkbox( "Model outline##1", &options.esp_player_outlines );
				Checkbox( "Names##1", &options.esp_player_names );
				Checkbox( "Health##1", &options.esp_player_health );
				Checkbox( "Armor##1", &options.esp_player_armour );

				Checkbox( "Weapon##1", &options.esp_player_weapons );
				Checkbox( "Snaplines##1", &options.esp_player_snaplines );

				Checkbox( "Planted C4##1", &options.esp_planted_c4 );
				SameLine( ImGui::GetWindowWidth( ) - 40 );
				ImGuiEx::ColorEdit3( "##Planted_C41", &options.color_esp_c4 );

				Checkbox( "Defuse Kit##1", &options.esp_defuse_kit );
				SameLine( ImGui::GetWindowWidth( ) - 40 );
				ImGuiEx::ColorEdit3( "##Defuse_Kit1", &options.color_esp_defuse );

				Checkbox( "Dropped Weapons##1", &options.esp_dropped_weapons );
				SameLine( ImGui::GetWindowWidth( ) - 40 );
				ImGuiEx::ColorEdit3( "##Dropped_Weapons1", &options.color_esp_weapons );

				ImGuiEx::ColorEdit3( "Team Visible##1", &options.color_esp_ally_visible );
				ImGuiEx::ColorEdit3( "Team Invisible##1", &options.color_esp_ally_occluded );
				ImGuiEx::ColorEdit3( "Enemy Visible##1", &options.color_esp_enemy_visible );
				ImGuiEx::ColorEdit3( "Enemy Invisible##1", &options.color_esp_enemy_occluded );

				ImGui::PopStyleVar( 1 );
				ImGui::EndChild( );
				NextColumn( );
			}

			//GLOW - 2nd column
			{
				SetNextWindowPos( GetWindowPos( ) + ImVec2( 9.f + ( group_w / 3.0f ), 5.f ) );


				ImGui::BeginChild( "Glow", ImVec2( 207.f, 239 ), true, ImGuiWindowFlags_MenuBar );


				if ( ImGui::BeginMenuBar( ) )
				{

					Text( "Glow" );
					//Keybind( "##t2p", &options.thirdperson_button, &options.misc_thirdperson_type );
					ImGui::EndMenuBar( );
				}
				static const char* glowtype[] = { "Normal", "Filled", "Pulse", "Smooth" , "Smooth pulse" };
				static const char* glowstyle[] = { "Normal", "Health", "Rainbow" };
				static const char* glowfilter[] = { "All", "Visible", "Invisible" };

				PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2{ 4.f,2.0f } );
				Checkbox( "Enabled##2", &options.glow_enabled );

				if ( CollapsingHeader( "Players" ) )
				{
					if ( TreeNode( "Local" ) )
					{
						Checkbox( "Active##local_glow", &options.glow_local );
						PushItemWidth( 105.f );
						Combo( "Type##local_glow", &options.glow_local_type, glowtype, 5 );
						Combo( "Color##local_glow", &options.glow_local_colortype, glowstyle, 3 );
						SliderFloat( "Bloom##local_glow", &options.glow_local_bloom, 0.3f, 1.f, "%0.1f" );
						PopItemWidth( );

						if ( options.glow_local_colortype == 0 )
							ImGuiEx::ColorEdit3( "Color##local_glow", &options.color_glow_local );

						Separator( );
						TreePop( );
					}

					if ( TreeNode( "Team" ) )
					{
						Checkbox( "Active##team_glow", &options.glow_team );
						PushItemWidth( 105.f );
						Combo( "Type##team_glow", &options.glow_team_type, glowtype, 5 );
						Combo( "Color##team_glow", &options.glow_team_colortype, glowstyle, 3 );
						Combo( "Filter##team_glow", &options.glow_team_filter, glowfilter, 3 );
						SliderFloat( "Bloom###team_glow", &options.glow_team_bloom, 0.3f, 1.f, "%0.1f" );
						PopItemWidth( );

						if ( options.glow_team_colortype == 0 )
						{
							if ( options.glow_team_filter != 2 )
								ImGuiEx::ColorEdit3( "Visible##team_glow", &options.color_glow_team_vis );

							if ( options.glow_team_filter != 1 )
								ImGuiEx::ColorEdit3( "Invisible##team_glow", &options.color_glow_team );
						}
						Separator( );
						TreePop( );
					}

					if ( TreeNode( "Enemies" ) )
					{
						Checkbox( "Active##glow_enemy", &options.glow_enemy );
						PushItemWidth( 105.f );
						Combo( "Type##glow_enemy", &options.glow_enemy_type, glowtype, 5 );
						Combo( "Color##glow_enemy", &options.glow_enemy_colortype, glowstyle, 3 );
						Combo( "Filter##glow_enemy", &options.glow_enemy_filter, glowfilter, 3 );
						SliderFloat( "Bloom###glow_enemy", &options.glow_enemy_bloom, 0.3f, 1.f, "%0.1f" );
						PopItemWidth( );

						if ( options.glow_enemy_colortype == 0 )
						{
							if ( options.glow_enemy_filter != 2 )
								ImGuiEx::ColorEdit3( "Visible##glow_enemy", &options.color_glow_enemy_vis );

							if ( options.glow_enemy_filter != 1 )
								ImGuiEx::ColorEdit3( "Invisible##glow_enemy", &options.color_glow_enemy );
						}
						TreePop( );
					}

				}

				if ( CollapsingHeader( "World" ) )
				{
					Checkbox( "Chickens##2", &options.glow_chickens );
					SameLine( ImGui::GetWindowWidth( ) - 28 );
					ImGuiEx::ColorEdit3( "##Chickens_glow", &options.color_glow_chickens );

					Checkbox( "Planted C4##2", &options.glow_planted_c4 );
					SameLine( ImGui::GetWindowWidth( ) - 28 );
					ImGuiEx::ColorEdit3( "##c4_glow", &options.color_glow_planted_c4 );

					Checkbox( "Defuse Kits##2", &options.glow_defuse_kits );
					SameLine( ImGui::GetWindowWidth( ) - 28 );
					ImGuiEx::ColorEdit3( "##defusekit_glow", &options.color_glow_defuse );

					Checkbox( "Weapons##2", &options.glow_weapons );
					SameLine( ImGui::GetWindowWidth( ) - 28 );
					ImGuiEx::ColorEdit3( "##wpnworld_glow", &options.color_glow_weapons );

					Separator( );
				}




		/*	ImGui::Selectable( "Health Based (Enemy)##2", &options.glow_health_e );
			ImGui::Selectable( "Health Based (Team)##2", &options.glow_health_f );
			ImGui::Selectable( "Rainbow (Enemy)##2", &options.glow_rb_e );
			ImGui::Selectable( "Rainbow (Team)##2", &options.glow_rb_f );
			ImGui::Selectable( "Rainbow (Other)##2", &options.glow_rb_o );*/



				PopStyleVar( );
				ImGui::EndChild( );
				NextColumn( );
			}


			//3rd column
			SetNextWindowPos( GetWindowPos( ) + ImVec2( 9.f + ( group_w / 3.0f * 2.f ), 5.f ) );

			ImGui::BeginChild( "Chams", ImVec2( 216.f, 239.f ), true, ImGuiWindowFlags_MenuBar );


			if ( ImGui::BeginMenuBar( ) )
			{

				Text( "Chams" );
				//Keybind( "##t2p", &options.thirdperson_button, &options.misc_thirdperson_type );
				ImGui::EndMenuBar( );
			}

			ImGui::Checkbox( "Enabled (Players)##3", &options.chams_player_enabled );
			ImGui::Separator( );
			ImGui::Selectable( "Team Check##3", &options.chams_player_enemies_only );
			ImGui::Selectable( "Wireframe##3", &options.chams_player_wireframe );
			ImGui::Selectable( "Flat##3", &options.chams_player_flat );
			ImGui::Selectable( "Ignore-Z##3", &options.chams_player_ignorez );
			ImGui::Selectable( "Glass##3", &options.chams_player_glass );
			ImGui::Spacing( ), ImGui::Spacing( );
			ImGui::Separator( );
			ImGui::Checkbox( "Enabled (Arms)##4", &options.chams_arms_enabled );
			ImGui::Spacing( );
			ImGui::Selectable( "Wireframe##4", &options.chams_arms_wireframe );
			ImGui::Selectable( "Flat##4", &options.chams_arms_flat );
			ImGui::Selectable( "Ignore-Z##4", &options.chams_arms_ignorez );
			ImGui::Selectable( "Glass##4", &options.chams_arms_glass );
			ImGui::Spacing( ), ImGui::Spacing( );
			ImGui::Separator( );
			ImGui::Checkbox( "Enabled (Angles)##4", &options.fakechams_enabled );
			ImGui::Spacing( );
			ImGui::Selectable( "LBY##4", &options.fakechams_lby );
			ImGui::Selectable( "Fake##4", &options.fakechams_fake );
			ImGui::Selectable( "Fix LBY Break pos##4", &options.fakechams_fix );
			ImGui::PushItemWidth( ImGui::GetContentRegionAvailWidth( ) * 0.65f );
			ImGui::SliderFloat( "Pos Fix Value##6", &options.fakechams_fix_val, 15.f, 35.f, "%0.f" );
			PopItemWidth( );
			ImGui::EndChild( );

			//add the gay shit from 

			ImGui::Columns( 1, nullptr, false );
			ImGui::PopStyleVar( );
		}
		else if ( active_esp_tab == 1 ) //part 2
		{
			ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2{ style.WindowPadding.x, style.ItemSpacing.y } );
			ImGui::Columns( 3, nullptr, false );
			ImGui::SetColumnOffset( 1, group_w / 3.0f );
			ImGui::SetColumnOffset( 2, 2 * group_w / 3.0f );
			ImGui::SetColumnOffset( 3, 3 * group_w / 3.0f );

			// 1st column
			ImGui::Text( "AA Lines" );
			ImGui::BeginChild( "AA Lines", ImVec2( 180, 220 ), true );
			Checkbox( "Enabled##5", &options.aal );
			ImGui::Separator( );
			Selectable( "Local Feet##5", &options.aal_feet );
			Selectable( "Local Crosshair##5", &options.aal_cross );
			Selectable( "Other Feet##5", &options.aal_other );
			Selectable( "No Border##5", &options.aal_simple );
			ImGui::Separator( );
			Text( "Filters" );
			ImGui::Separator( );
			Selectable( "LBY##5", &options.aal_lby );
			Selectable( "Real##5", &options.aal_real );
			Selectable( "Fake##5", &options.aal_fake );
			EndChild( );
			//TODO: add fake stats from FootPwner

			ImGui::NextColumn( );
			//2nd column

			ImGui::Text( "DLights" );
			ImGui::BeginChild( "DLights", ImVec2( 180, 220 ), true );
			Checkbox( "Dlights##6", &options.dlights );
			ImGui::SameLine( 90 );
			Checkbox( "Elights##6", &options.elights );
			ImGui::Separator( );
			ImGui::PushItemWidth( -1.f );
			ImGui::SliderFloat( "Radius##6", &options.dlight_radius, 20.f, 185.f, "%1.f" );
			ImGui::SliderInt( "Exponent##6", &options.dlight_exponent, 1, 15, "%0.f" );
			ImGui::PopItemWidth( );
			ImGui::Separator( );
			Selectable( "Team##6", &options.dlights_team );
			Selectable( "Enemies##6", &options.dlights_enemy );
			Selectable( "Local##6", &options.dlights_local );
			Selectable( "Rainbow##6", &options.dlights_rainbow );
			EndChild( );

			ImGui::NextColumn( );
			//3rd column

			ImGui::Text( "Override Config" );
			ImGui::BeginChild( "Override Config", ImVec2( 180, 220 ), true );
			Checkbox( "Enabled##7", &options.vis_overridecfg );
			ImGui::Separator( );
			Selectable( "Mipmap levels##7", &options.viso_mipmap );
			Selectable( "Fullbright##7", &options.viso_fullbright );
			Selectable( "Draw gray##7", &options.viso_drawgray );
			Selectable( "Low Resolution##7", &options.viso_lowres );
			Selectable( "No Transparent##7", &options.viso_trans );
			Selectable( "Visualize Fillrate##7", &options.viso_visf );
			Selectable( "Measure Fillrate##7", &options.viso_meaf );
			Selectable( "Supress Rendering##7", &options.viso_suppressrend );
			Selectable( "Reverse Depth##7", &options.viso_revd );
			EndChild( );



			ImGui::Columns( 1, nullptr, false );
			ImGui::PopStyleVar( );
		} 
		else if ( active_esp_tab == 2 )
		{
			ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2{ style.WindowPadding.x, style.ItemSpacing.y } );
			ImGui::Columns( 3, nullptr, false );
			ImGui::SetColumnOffset( 1, group_w / 3.0f );
			ImGui::SetColumnOffset( 2, 2 * group_w / 3.0f );
			ImGui::SetColumnOffset( 3, 3 * group_w / 3.0f );

			ImGui::Text( "Misc Visuals" );
			ImGui::BeginChild( "Misc Visuals", ImVec2( 180, 220 ), true );
			Checkbox( "Enabled##8", &options.vis_misc_enabled );
			ImGui::Separator( );
			ImGui::Selectable( "Fov Circle##8", &options.vis_fovcircle );
			ImGui::Selectable( "Spread Circle##8", &options.vis_spreadcircle );
			ImGui::Selectable( "Crosshair##8", &options.esp_crosshair );
			if ( ImGui::Selectable( "Wall Color Changer##8", &options.wall_colorchanger ) )
			{
				if ( !options.wall_colorchanger )
				{
					options.color_walls = Color( 255, 255, 255 );
					Hooks::SetNewColor = true;
				}
				
			}
			Selectable( "No visual recoil##8", &options.misc_visrcs );
			Selectable( "No flash##8", &options.misc_noflash );
			Selectable( "No fog##8", &options.misc_nofog );
			Selectable( "No hands##8", &options.misc_no_hands );
			Selectable( "No particles##8", &options.particleremove );
			Selectable( "No post processing##8", &options.misc_postprocess );
			Selectable( "Ambient lighting", &options.aimbient_lighting );
			Selectable( "World lights", &options.worldlights_enabled );

			Separator( );
			static const char* smoke_select[] = { "Normal", "Wireframe", "No smoke" };
			Text( "Smoke style" );
			Combo( "##smokestyle", &options.smoke_style, smoke_select, 3 );
			Separator( );
			static const char* skyboxes[] = { "sky_l4d_rural02_ldr", "cs_baggage_skybox_", "cs_tibet" , "embassy", "italy", "jungle", "nukeblank", "office", "sky_cs15_daylight01_hdr", "sky_cs15_daylight02_hdr", "sky_cs15_daylight03_hdr", "sky_cs15_daylight04_hdr", "sky_day02_05", "sky_day02_05_hdr", "sky_csgo_cloudy01",  "sky_csgo_night_flat",  "sky_csgo_night02", "sky_csgo_night02b", "vertigo", "vietnam" , "vertigoblue_hdr" , "vertigo_hdr" , "sky_dust", "sky_venice" };
			Text( "Sky changer" );
			if ( Combo( "##skychanger", &options.selected_sky, skyboxes, 24 ) )
			{
			//	sky_changer::get( ).change( skyboxes[ options.selected_sky ] );
			}
				


			EndChild( );

			ImGui::NextColumn( );

			ImGui::Columns( 1, nullptr, false );
			ImGui::PopStyleVar( );
		}
	}
	ImGui::EndChild( );
}

void RenderMiscTab( )
{
	static char* misc_tab_names[] = { "Part 1", "Part 2" };
	static int   active_misc_tab = 0;

	bool placeholder_true = true;

	auto& style = ImGui::GetStyle( );
	PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2{ 0,  style.WindowPadding.y } );

	float group_w = ImGui::GetCurrentWindow( )->Size.x - style.WindowPadding.x * 2;
	ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2( 0, 10 ) );
	{
		ImGui::PushStyleVar( ImGuiStyleVar_FrameBorderSize, 0.f );

		render_tabs( misc_tab_names, active_misc_tab, group_w / _countof( misc_tab_names ), 33.0f, true );
	}
	ImGui::PopStyleVar(3 );

	ImGui::BeginChild( "##body_content", ImVec2( 0, 0 ), false );
	{
		if ( active_misc_tab == 0 )
		{
			ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2{ style.WindowPadding.x, style.ItemSpacing.y } );
			ImGui::Columns( 2, nullptr, false );
			ImGui::SetColumnOffset( 1, group_w / 1.8f );
			ImGui::SetColumnOffset( 2, group_w );

			Text( "Misc Features" );
			BeginChild( "Misc Features", ImVec2( 330, 220 ), true );
			Checkbox( "Enabled##misc", &options.misc_enabled );
			Separator( );
			Selectable( "Autostrafer##11", &options.misc_autostrafe );
			Selectable( "Bunny hop##11", &options.misc_bhop );
			Selectable( "Engine Prediction##11", &options.engine_pred );
			Selectable( "Event Lag##11", &options.misc_eventlag );
			Selectable( "Flashlight##11", &options.flashlight_on );
			Selectable( "Freecam##11", &options.misc_freecam );
			Selectable( "Hitmarker##11", &options.misc_hitmarker );
			Selectable( "HvH Autobuy##11", &options.autobuy );
			Selectable( "Inventory Enabler##11", &options.misc_invenabler );
			Selectable( "Rank reveal##11", &options.misc_rank_reveal );

			Selectable( "Thirdperson", &options.misc_thirdperson_enabled );
			Keybind( "##tp", &options.thirdperson_button, &options.misc_thirdperson_type );

			Selectable( "Crash AW Bot server", &options.misc_aw_crasher );

			static const char* rarities[] = {
				"Default",
				"Common (White)",
				"Uncommon (Light Blue)",
				"Rare (Blue)",
				"Mythical (Purple)",
				"Legendary (Pink)",
				"Ancient (Red)",
				"Immortal (Gold)"
			};

			static const char* bans[] = {
				"Competitive cooldown - No message",
				"Competitive cooldown - Kicked",
				"Competitive cooldown - Too many teammates killed",
				"Competitive cooldown - Killed teammate at round start",
				"Competitive cooldown - Failed to reconnect",
				"Competitive cooldown - Abandoned the last match",
				"Competitive cooldown - Too much teamdamage",
				"Competitive cooldown - Too much teamdamage (round start)",
				"Global cooldown - Pernamently untrusted",
				"Competitive cooldown - Kicked too much",
				"Overwatch ban - Majorly disruptive",
				"Overwatch ban - Minorly disruptive",
				"Temporary cooldown - Resolving state for account",
				"Temporary cooldown - Resolving state after last match",
				"Global cooldown - Pernamently untrusted",
				"Global cooldown - No message",
				"Competitive cooldown - Failed to connect by match start",
				"Competitive cooldown - Kicked too many teammates",
				"Temporary cooldown - Adjusting rank",
				"Global cooldown - GSLT Ban"
			};
			//fix unloading  
			if ( Combo( "Ban type", &options.misc_proto_ban_type, bans, 20 ) )
			{
				//proto::get( ).SendClientHello( );
				//proto::get( ).SendMatchmakingHelloToServer( );
			}
			/*if ( SliderInt( "Ban Type", &options.misc_proto_ban_type, 0, 19 , bans[ options.misc_proto_ban_type ] ) )
			{

			}*/






			Separator( );
			ImGui::PushItemWidth( ImGui::GetContentRegionAvailWidth( ) * 0.65f );

			if ( options.flashlight_on )
				ImGui::SliderFloat( "Flashlight Fov##11", &options.flashlight_fov, 10.f, 100.f, "%.0f°" );

			ImGui::SliderFloat( "Fov##11", &options.fov, 0.f, 70.f, "%.0f°" );

			ImGui::SliderFloat( "Rainbow Speed##11", &options.rainbow_speed, 1.f, 20.f, "%.0f" );

			if ( options.misc_thirdperson_enabled )
				ImGui::SliderFloat( "Thirdperson Distance##11", &options.misc_thirdperson_dist, 80.f, 160.f, "%.0f" );

			ImGui::SliderFloat( "Viewmodel Fov##11", &options.viewmodel_fov, 0.f, 70.f, "%.0f°" );
			ImGui::SliderFloat( "Weapon Sway##11", &options.weapon_sway, 0.f, 20.f, "%.1f" );

			ImGui::PopItemWidth( );
			EndChild( );
			ImGui::NextColumn( );

			const char* selection[] = { "Off", "Low", "High" , "Event Lag" };

			static int selectedd2 = 0;

			Text( "Fake Features" );
			BeginChild( "Fake Features", ImVec2( 250, 220 ), true );

			ImGui::Checkbox( "Fake Kill##12", &options.fake_kill );
			if ( options.fake_kill )
			{
				ImGui::Selectable( "Headshot##12", &options.fake_kill_hs );
				ImGui::Selectable( "Wallbang##12", &options.fake_kill_wall );
				ImGui::Selectable( "Domination##12", &options.fake_kill_domin );
				ImGui::Selectable( "Revenge##12", &options.fake_kill_revenge );
				ImGui::Selectable( "Suicide##12", &options.fake_kill_name );
			}

			Separator( );

			ImGui::Checkbox( "Fake Info##13", &options.clientside_name );
			if ( options.clientside_name )
			{
				ImGui::Selectable( "Hide Names##13", &options.clientside_hide );
				ImGui::Selectable( "Picture Changer##13", &options.clientside_pic_bool );
				ImGui::Selectable( "Name Changer (local)##13", &options.clientside_local_bool );
				ImGui::Selectable( "Name Changer (others)##13", &options.clientside_other_bool );
				ImGui::PushItemWidth( ImGui::GetContentRegionAvailWidth( ) * 0.65f );
				ImGui::InputText( "Local name##13", Hooks::clientname, 128 );
				ImGui::InputText( "Others' name##13", Hooks::clientname_other, 128 );
				ImGui::InputText( "SteamID64##13", Hooks::clientpic, 18, ImGuiInputTextFlags_CharsDecimal );
				Text( "(SteamID is for the picture changer)" );
				ImGui::PopItemWidth( );
			}

			Separator( );

			ImGui::Checkbox( "Fake Latency##12", &options.fake_latency );
			if ( options.fake_latency )
			{
				ImGui::PushItemWidth( ImGui::GetContentRegionAvailWidth( ) * 0.65f );
				ImGui::SliderFloat( "Amount##11", &options.fake_latency_amount, 50.f, 850.f, "%.0f" );
				ImGui::PopItemWidth( );
			}


			EndChild( );

			ImGui::Columns( 1, nullptr, false );
			ImGui::PopStyleVar( );
		} else if ( active_misc_tab == 1 )
		{
			ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2{ style.WindowPadding.x, style.ItemSpacing.y } );
			ImGui::Columns( 2, nullptr, false );
			ImGui::SetColumnOffset( 1, group_w / 2.0f );
			ImGui::SetColumnOffset( 2, 2 * group_w / 2.0f );


			Text( "Scope modifiers" );
			if ( BeginChild( "Scope modifiers", ImVec2( 250, 220 ), true ) )
			{
				Selectable( "Fake zoom##11", &options.misc_fakezoom );
				Selectable( "Force crosshair##11", &options.misc_sniper_crosshair );
				Selectable( "No scope##8", &options.misc_noscope );
				Selectable( "No SG/AUG blur##8", &options.misc_remove_blur );
				Selectable( "No sniper zoom##11", &options.misc_fov_sniper );
				Selectable( "Scope lines##8", &options.misc_scopelines );
				EndChild( );
			}




			ImGui::Columns( 1, nullptr, false );
			ImGui::PopStyleVar( );
		}




	}
	ImGui::EndChild( );
}

void RenderColorTab( )
{
	static char* col_tab_names[] = { "Part 1", "Part 2", "Part 3" };
	static int   active_col_tab = 0;

	bool placeholder_true = true;

	auto& style = ImGui::GetStyle( );
	PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2{ 0,  style.WindowPadding.y } );

	float group_w = ImGui::GetCurrentWindow( )->Size.x - style.WindowPadding.x * 2;
	ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2( 0, 10 ) );
	{
		ImGui::PushStyleVar( ImGuiStyleVar_FrameBorderSize, 0.f );

		render_tabs( col_tab_names, active_col_tab, group_w / _countof( col_tab_names ), 33.0f, true );
	}
	ImGui::PopStyleVar( 3);

	ImGui::BeginChild( "##body_content", ImVec2( 0, 0 ), false );
	{

		if ( active_col_tab == 0 )
		{
			ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2{ style.WindowPadding.x, style.ItemSpacing.y } );
			ImGui::Columns( 3, nullptr, false );
			ImGui::SetColumnOffset( 1, group_w / 2.0f );
			ImGui::SetColumnOffset( 2, 2 * group_w / 1.9f );


			Text( "ESP" );
			BeginChild( "ESP", ImVec2( 0, 0 ), true );
			ImGui::PushItemWidth( 170 );
			//ImGui::ColorEditMode(ImGuiColorEditMode_UserSelect);

			ImGuiEx::ColorEdit3( "Crosshair##1", &options.color_esp_crosshair );
			ImGuiEx::ColorEdit3( "Dropped Weapons##1", &options.color_esp_weapons );
			ImGuiEx::ColorEdit3( "Defuse Kit##1", &options.color_esp_defuse );
			ImGuiEx::ColorEdit3( "Planted C4##1", &options.color_esp_c4 );
			ImGui::PopItemWidth( );
			EndChild( );

			ImGui::NextColumn( );

			
			Text( "Glow" );
			BeginChild( "Glow", ImVec2( 270, 220 ), true );
			ImGui::PushItemWidth( 170 );
			//	ImGui::ColorEditMode(ImGuiColorEditMode_UserSelect);
			ImGuiEx::ColorEdit3( "Local Player##2c", &options.color_glow_local );
			ImGuiEx::ColorEdit3( "Chickens##2", &options.color_glow_chickens );
			ImGuiEx::ColorEdit3( "Planted C4##2", &options.color_glow_planted_c4 );
			ImGuiEx::ColorEdit3( "Defuse Kits##2", &options.color_glow_defuse );
			ImGuiEx::ColorEdit3( "Weapons##2", &options.color_glow_weapons );
			ImGui::PopItemWidth( );
			EndChild( );

			ImGui::Columns( 1, nullptr, false );
			ImGui::PopStyleVar( );
		} else if ( active_col_tab == 1 )
		{
			ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2{ style.WindowPadding.x, style.ItemSpacing.y } );
			ImGui::Columns( 3, nullptr, false );
			ImGui::SetColumnOffset( 1, group_w / 2.0f );
			ImGui::SetColumnOffset( 2, 2 * group_w / 1.9f );

			Text( "Chams" );
			BeginChild( "Chams", ImVec2( 0, 0 ), true );
			//	ImGui::ColorEditMode(ImGuiColorEditMode_UserSelect);
			ImGui::PushItemWidth( 170 );
			ImGuiEx::ColorEdit4( "Team Visible##3", &options.color_chams_player_ally_visible );
			ImGuiEx::ColorEdit4( "Team Invisible##3", &options.color_chams_player_ally_occluded );
			ImGuiEx::ColorEdit4( "Enemy Visible##3", &options.color_chams_player_enemy_visible );
			ImGuiEx::ColorEdit4( "Enemy Invisible##3", &options.color_chams_player_enemy_occluded );
			ImGuiEx::ColorEdit4( "Hands Visible##3", &options.color_chams_arms_visible );
			ImGuiEx::ColorEdit4( "Hands Invisible##3", &options.color_chams_arms_occluded );
			ImGuiEx::ColorEdit4( "Fake##3", &options.color_fakechams_fake );
			ImGuiEx::ColorEdit4( "LBY##3", &options.color_fakechams_lby );
			ImGui::PopItemWidth( );
			EndChild( );

			NextColumn( );

			Text( "Dlights" );
			BeginChild( "Dlights", ImVec2( 250, 110 ), true );
			//	ImGui::ColorEditMode(ImGuiColorEditMode_UserSelect);
			ImGui::PushItemWidth( 170 );
			ImGuiEx::ColorEdit3( "Team##4", &options.color_dlight_team );
			ImGuiEx::ColorEdit3( "Enemy##4", &options.color_dlight_enemy );
			ImGuiEx::ColorEdit3( "Local##4", &options.color_dlight_local );
			ImGui::PopItemWidth( );
			EndChild( );


			//Hotkey()
			Spacing( ), Spacing( );

			Text( "Misc Colors" );
			BeginChild( "Misc Colors", ImVec2( 285, 70 ), true );
			//	ImGui::ColorEditMode(ImGuiColorEditMode_UserSelect);
			ImGui::PushItemWidth( 170 );

			if ( ImGuiEx::ColorEdit4( "Wall Colors", &options.color_walls, true ) )
				Hooks::SetNewColor = true;
			ImGui::Checkbox( "Wall Colors (Rainbow)", &options.color_walls_rainbow );

			ImGuiEx::ColorEdit3( "Ambient Lighting##4", &options.mat_ambient );
			ImGuiEx::ColorEdit3( "World Lights##4", &options.worldlights );
			ImGui::PopItemWidth( );
			EndChild( );

			ImGui::Columns( 1, nullptr, false );
			ImGui::PopStyleVar( );
		}

		else if ( active_col_tab == 2 )
		{
			ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2{ style.WindowPadding.x, style.ItemSpacing.y } );
			ImGui::Columns( 3, nullptr, false );
			ImGui::SetColumnOffset( 1, group_w / 1.9f );
			ImGui::SetColumnOffset( 2, 2 * group_w / 1.9f );

			Text( "Menu Colors" );
			BeginChild( "Menu Colors", ImVec2( 0, 0 ), true );

			//	ImGui::SetColorEditOptions(ImGuiColorEditMode_UserSelect);
			ImGui::PushItemWidth( 170 );
			if ( ImGuiEx::ColorEdit4( "Button##23", &options.menu_button ) )
				Menu::get( ).ChangeStyle( ImGuiCol_Button, options.menu_button );
			if ( ImGuiEx::ColorEdit4( "Button (Hovered)##23", &options.menu_button_hovered ) )
				Menu::get( ).ChangeStyle( ImGuiCol_ButtonHovered, options.menu_button_hovered );
			if ( ImGuiEx::ColorEdit4( "Button (Active)##23", &options.menu_button_active ) )
				Menu::get( ).ChangeStyle( ImGuiCol_ButtonActive, options.menu_button_active );

			if ( ImGuiEx::ColorEdit4( "Text##23", &options.Text ) )
				Menu::get( ).ChangeStyle( ImGuiCol_Text, options.Text );
			if ( ImGuiEx::ColorEdit4( "Text (Active tab)##23", &options.TextDisabled ) )
				Menu::get( ).ChangeStyle( ImGuiCol_TextDisabled, options.TextDisabled );
			if ( ImGuiEx::ColorEdit4( "Window Background##23", &options.WindowBg ) )
				Menu::get( ).ChangeStyle( ImGuiCol_WindowBg, options.WindowBg );
			if ( ImGuiEx::ColorEdit4( "Child Background##23", &options.ChildWindowBg ) )
				Menu::get( ).ChangeStyle( ImGuiCol_ChildBg, options.ChildWindowBg );
			if ( ImGuiEx::ColorEdit4( "Popup Background##23", &options.PopupBg ) )
				Menu::get( ).ChangeStyle( ImGuiCol_PopupBg, options.PopupBg );

			if ( ImGuiEx::ColorEdit4( "Border##23", &options.Border ) )
				Menu::get( ).ChangeStyle( ImGuiCol_Border, options.Border );
			if ( ImGuiEx::ColorEdit4( "Border Shadow##23", &options.BorderShadow ) )
				Menu::get( ).ChangeStyle( ImGuiCol_BorderShadow, options.BorderShadow );

			if ( ImGuiEx::ColorEdit4( "Frame Background##23", &options.ImGuiCol_FrameBg ) )
				Menu::get( ).ChangeStyle( ImGuiCol_FrameBg, options.ImGuiCol_FrameBg );
			if ( ImGuiEx::ColorEdit4( "Frame BG (Hovered)##23", &options.ImGuiCol_FrameBgHovered ) )
				Menu::get( ).ChangeStyle( ImGuiCol_FrameBgHovered, options.ImGuiCol_FrameBgHovered );
			if ( ImGuiEx::ColorEdit4( "Frame BG (Active)##23", &options.ImGuiCol_FrameBgActive ) )
				Menu::get( ).ChangeStyle( ImGuiCol_FrameBgActive, options.ImGuiCol_FrameBgActive );
			if ( ImGuiEx::ColorEdit4( "Title Background##23", &options.ImGuiCol_TitleBg ) )
				Menu::get( ).ChangeStyle( ImGuiCol_TitleBg, options.ImGuiCol_TitleBg );
			if ( ImGuiEx::ColorEdit4( "Title BG (Collapsed)##23", &options.ImGuiCol_TitleBgCollapsed ) )
				Menu::get( ).ChangeStyle( ImGuiCol_TitleBgCollapsed, options.ImGuiCol_TitleBgCollapsed );
			if ( ImGuiEx::ColorEdit4( "Title BG (Active)##23", &options.ImGuiCol_TitleBgActive ) )
				Menu::get( ).ChangeStyle( ImGuiCol_TitleBgActive, options.ImGuiCol_TitleBgActive );

			if ( ImGuiEx::ColorEdit4( "Menu bar BG##23", &options.ImGuiCol_MenuBarBg ) )
				Menu::get( ).ChangeStyle( ImGuiCol_MenuBarBg, options.ImGuiCol_MenuBarBg );
			if ( ImGuiEx::ColorEdit4( "Scrollbar BG##23", &options.ImGuiCol_ScrollbarBg ) )
				Menu::get( ).ChangeStyle( ImGuiCol_ScrollbarBg, options.ImGuiCol_ScrollbarBg );
			if ( ImGuiEx::ColorEdit4( "Scrollbar Grab##23", &options.ImGuiCol_ScrollbarGrab ) )
				Menu::get( ).ChangeStyle( ImGuiCol_ScrollbarGrab, options.ImGuiCol_ScrollbarGrab );
			if ( ImGuiEx::ColorEdit4( "Scrollbar Grab (Hovered)##23", &options.ImGuiCol_ScrollbarGrabHovered ) )
				Menu::get( ).ChangeStyle( ImGuiCol_ScrollbarGrabHovered, options.ImGuiCol_ScrollbarGrabHovered );
			if ( ImGuiEx::ColorEdit4( "Scrollbar Grab (Active)##23", &options.ImGuiCol_ScrollbarGrabActive ) )
				Menu::get( ).ChangeStyle( ImGuiCol_ScrollbarGrabActive, options.ImGuiCol_ScrollbarGrabActive );
			if ( ImGuiEx::ColorEdit4( "Combo Background##23", &options.ImGuiCol_ComboBg ) )
				Menu::get( ).ChangeStyle( ImGuiCol_PopupBg, options.ImGuiCol_ComboBg );
			if ( ImGuiEx::ColorEdit4( "Checkmark##23", &options.ImGuiCol_CheckMark ) )
				Menu::get( ).ChangeStyle( ImGuiCol_CheckMark, options.ImGuiCol_CheckMark );

			if ( ImGuiEx::ColorEdit4( "Slider Grab##23", &options.ImGuiCol_SliderGrab ) )
				Menu::get( ).ChangeStyle( ImGuiCol_SliderGrab, options.ImGuiCol_SliderGrab );
			if ( ImGuiEx::ColorEdit4( "Slider Grab (Active)##23", &options.ImGuiCol_SliderGrabActive ) )
				Menu::get( ).ChangeStyle( ImGuiCol_SliderGrabActive, options.ImGuiCol_SliderGrabActive );
			if ( ImGuiEx::ColorEdit4( "Header##23", &options.ImGuiCol_Header ) )
				Menu::get( ).ChangeStyle( ImGuiCol_Header, options.ImGuiCol_Header );
			if ( ImGuiEx::ColorEdit4( "Header (Hovered)##23", &options.ImGuiCol_HeaderHovered ) )
				Menu::get( ).ChangeStyle( ImGuiCol_HeaderHovered, options.ImGuiCol_HeaderHovered );
			if ( ImGuiEx::ColorEdit4( "Header (Active)##23", &options.ImGuiCol_HeaderActive ) )
				Menu::get( ).ChangeStyle( ImGuiCol_HeaderActive, options.ImGuiCol_HeaderActive );

			if ( ImGuiEx::ColorEdit4( "Column##23", &options.ImGuiCol_Column ) )
				Menu::get( ).ChangeStyle( ImGuiCol_Separator, options.ImGuiCol_Column );
			if ( ImGuiEx::ColorEdit4( "Column (Hovered)##23", &options.ImGuiCol_ColumnHovered ) )
				Menu::get( ).ChangeStyle( ImGuiCol_SeparatorHovered, options.ImGuiCol_ColumnHovered );
			if ( ImGuiEx::ColorEdit4( "Column (Active)##23", &options.ImGuiCol_ColumnActive ) )
				Menu::get( ).ChangeStyle( ImGuiCol_SeparatorActive, options.ImGuiCol_ColumnActive );
			if ( ImGuiEx::ColorEdit4( "Close Button##23", &options.ImGuiCol_CloseButton ) )
				Menu::get( ).ChangeStyle( ImGuiCol_CloseButton, options.ImGuiCol_CloseButton );
			if ( ImGuiEx::ColorEdit4( "Close Button (Hovered)##23", &options.ImGuiCol_CloseButtonHovered ) )
				Menu::get( ).ChangeStyle( ImGuiCol_CloseButtonHovered, options.ImGuiCol_CloseButtonHovered );
			if ( ImGuiEx::ColorEdit4( "Close Button (Active)##23", &options.ImGuiCol_CloseButtonActive ) )
				Menu::get( ).ChangeStyle( ImGuiCol_CloseButtonActive, options.ImGuiCol_CloseButtonActive );

			if ( ImGuiEx::ColorEdit4( "Plotlines##23", &options.ImGuiCol_PlotLines ) )
				Menu::get( ).ChangeStyle( ImGuiCol_PlotLines, options.ImGuiCol_PlotLines );
			if ( ImGuiEx::ColorEdit4( "Plotlines (Hovered)##23", &options.ImGuiCol_PlotLinesHovered ) )
				Menu::get( ).ChangeStyle( ImGuiCol_PlotLinesHovered, options.ImGuiCol_PlotLinesHovered );
			if ( ImGuiEx::ColorEdit4( "Plot Histogram##23", &options.ImGuiCol_PlotHistogram ) )
				Menu::get( ).ChangeStyle( ImGuiCol_PlotHistogram, options.ImGuiCol_PlotHistogram );
			if ( ImGuiEx::ColorEdit4( "Plot Histogram (Hovered)##23", &options.ImGuiCol_PlotHistogramHovered ) )
				Menu::get( ).ChangeStyle( ImGuiCol_PlotHistogramHovered, options.ImGuiCol_PlotHistogramHovered );
			if ( ImGuiEx::ColorEdit4( "Text Selected Background##23", &options.ImGuiCol_TextSelectedBg ) )
				Menu::get( ).ChangeStyle( ImGuiCol_TextSelectedBg, options.ImGuiCol_TextSelectedBg );
			if ( ImGuiEx::ColorEdit4( "Modal Window Darkening##23", &options.ImGuiCol_ModalWindowDarkening ) )
				Menu::get( ).ChangeStyle( ImGuiCol_ModalWindowDarkening, options.ImGuiCol_ModalWindowDarkening );
			ImGui::PopItemWidth( );
			EndChild( );






			ImGui::Columns( 1, nullptr, false );
			ImGui::PopStyleVar( );
		}
	}
	ImGui::EndChild( );

}

enum Hitboxes
{
	HITBOX_HEAD,
	HITBOX_NECK,
	HITBOX_LOWER_NECK,
	HITBOX_PELVIS,
	HITBOX_STOMACH,
	HITBOX_LOWER_CHEST,
	HITBOX_CHEST,
	HITBOX_UPPER_CHEST,
	HITBOX_RIGHT_THIGH,
	HITBOX_LEFT_THIGH,
	HITBOX_RIGHT_CALF,
	HITBOX_LEFT_CALF,
	HITBOX_RIGHT_FOOT,
	HITBOX_LEFT_FOOT,
	HITBOX_RIGHT_HAND,
	HITBOX_LEFT_HAND,
	HITBOX_RIGHT_UPPER_ARM,
	HITBOX_RIGHT_FOREARM,
	HITBOX_LEFT_UPPER_ARM,
	HITBOX_LEFT_FOREARM,
	HITBOX_MAX
};

void RenderAimTab( )
{
	static char* aim_tab_names[] = { "Part 1", "Part 2" };
	static int   active_aim_tab = 0;

	bool placeholder_true = true;

	auto& style = ImGui::GetStyle( );
	PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2{ 0,  style.WindowPadding.y } );

	float group_w = ImGui::GetCurrentWindow( )->Size.x - style.WindowPadding.x * 2;
	ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2( 0, 10 ) );
	{
		ImGui::PushStyleVar( ImGuiStyleVar_FrameBorderSize, 0.f );

		render_tabs( aim_tab_names, active_aim_tab, group_w / _countof( aim_tab_names ), 33.0f, true );
	}
	ImGui::PopStyleVar( 3);

	const char* hitboxx[] = { "Head" , "Neck", "Chest", "Pelvis", "Stomach" };
	static int selectedd = 0;
	const char* selection[] = { "Crosshair" , "Distance", "Lowest HP", "Highest Damage", "Lethal / Highest DMG" , "Highest Hitchance" , "Lowest bullets to kill" , "First valid target" };

	static int selectedd2 = 0;

	ImGui::BeginChild( "##body_content", ImVec2( 0, 0 ), false );
	{
		if ( active_aim_tab == 0 )
		{
			ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2{ style.WindowPadding.x, style.ItemSpacing.y } );
			ImGui::Columns( 2, nullptr, false );
			ImGui::SetColumnOffset( 1, group_w / 1.75f );
			ImGui::SetColumnOffset( 2, group_w );

			Text( "Aimbot" );
			BeginChild( "Aimbot", ImVec2( 365, 220 ), true );
			Checkbox( "Enabled##9", &options.aim_bot );
			Separator( );
			ImGui::PushItemWidth( ImGui::GetContentRegionAvailWidth( ) * 0.65f );
			SliderFloat( "Fov##9", &options.aim_fov, 0.f, 180.f, u8"%.0f°" );

			static const char* auto_txt = "auto";
			static const char* enemy_hp = "enemy hp";

			SliderInt( "Hitchance (Stand)##9", &options.aim_hitchance, -1, 100, options.aim_hitchance < 0 ? auto_txt : "%.0f%%" );
			SliderInt( "Hitchance (Move)##9", &options.aim_hitchance_move, 0, 100, "%.0f%%" );
			SliderInt( "Min Damage##9", &options.min_dmg, -1, 101, options.min_dmg < 0 ? auto_txt : ( options.min_dmg > 100 ? enemy_hp : "%.0f" ) );


			if ( options.autowall )
				SliderInt( "Min Awall Damage##9", &options.min_dmgawall, 0, 100, "%.0f" );
			SliderFloat( "Pointscale##9", &options.aim_ps, 0.f, 1.f, "%.2f" );
			SliderInt( "Baim if lower##9", &options.aim_baimlower, 0, 100, "%.0f" );
			SliderInt( "Baim after shots##9", &options.baim_shots, 0, 15, "%.0f" );

			PopItemWidth( );

			Separator( );
			Selectable( "Auto-Scope##9", &options.aim_autoscope );
			Selectable( "Auto-Stop##9", &options.aim_autostop );
			Selectable( "Auto-Crouch##9", &options.aim_autocrouch );
			Selectable( "Autoshoot##9", &options.aim_autoshoot );

			Selectable( "Autowall##9", &options.autowall );
			Selectable( "Baim if lethal##9", &options.baim_lethal );
			Selectable( "Silent aim##9", &options.aim_silent );
			if ( options.aim_silent )
				Selectable( "pSilent##9", &options.aim_pSilent );
			Selectable( "PVS Fix##9", &options.aim_pvsfix );

			EndChild( );


			ImGui::NextColumn( );


			Text( "Target Selection" );
			BeginChild( "Target Selection", ImVec2( 240, 120 ), true );
			if ( ImGui::Combo( "Priority##10", &selectedd, hitboxx, IM_ARRAYSIZE( hitboxx ) ) )
			{
				switch ( selectedd )
				{
					case 0:
						options.hit_priority = HITBOX_HEAD;
						break;
					case 1:
						options.hit_priority = HITBOX_NECK;
						break;
					case 2:
						options.hit_priority = HITBOX_CHEST;
						break;
					case 3:
						options.hit_priority = HITBOX_PELVIS;
						break;
					case 4:
						options.hit_priority = HITBOX_STOMACH;
						break;
				}
			}

			if ( ImGui::Combo( "Selection##10", &selectedd2, selection, IM_ARRAYSIZE( selection ) ) )
			{
				options.aim_method = selectedd2;
			}

			Spacing( );
			static const char* hitscan_s[] = { "Head", "Neck", "Chest", "Stomach", "Pelvis", "Arms", "Legs", "Feet", "Hands" };



			static bool toggles[] = { true,true,true,true,true,true,true,false,false };

			if ( ImGui::Button( "Hitscan", ImVec2( 75, 30 ) ) )
				ImGui::OpenPopup( "hitscantoggle" );

			if ( ImGui::BeginPopup( "hitscantoggle" ) )
			{
				for ( int i = 0; i < IM_ARRAYSIZE( hitscan_s ); i++ )
				{
					if ( ImGui::MenuItem( hitscan_s[ i ], "", &toggles[ i ] ) )
					{
						switch ( i )
						{
							case 0:
								options.hit_head = toggles[ i ];
								break;

							case 1:
								options.hit_neck = toggles[ i ];
								break;

							case 2:
								options.hit_chest = toggles[ i ];
								break;
							case 3:
								options.hit_stomach = toggles[ i ];
								break;
							case 4:
								options.hit_pelvis = toggles[ i ];
								break;
							case 5:
								options.hit_arms = toggles[ i ];
								break;
							case 6:
								options.hit_legs = toggles[ i ];
								break;
							case 7:
								options.hit_feet = toggles[ i ];
								break;
							case 8:
								options.hit_hands = toggles[ i ];
								break;
						}
					}
				}



				ImGui::EndPopup( );
			}
			EndChild( );



			ImGui::Columns( 1, nullptr, false );
			ImGui::PopStyleVar( );
		} else if ( active_aim_tab == 1 )
		{
			ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2{ style.WindowPadding.x, style.ItemSpacing.y } );
			ImGui::Columns( 2, nullptr, false );
			ImGui::SetColumnOffset( 1, group_w / 1.75f );
			ImGui::SetColumnOffset( 2, group_w );

			const char* oppositee[] = { "Off" , "LBY", "Fake" };

			static int oppositeselc = 0;


			Text( "Anti-Aim" );
			BeginChild( "Anti-Aim", ImVec2( 365, 220 ), true );


			Checkbox( "Enable Spin##11", &options.aa_real_spin );

			if ( options.aa_real_spin )
			{
				Spacing( );
				ImGui::PushItemWidth( ImGui::GetContentRegionAvailWidth( ) * 0.65f );
				SliderFloat( "Speed##11", &options.aa_real_spin_speed, -100.f, 100.f, "%.0f" );
				SliderFloat( "LBY avoid range##11", &options.aa_real_spin_lbyr, 0.f, 45.f, "%.0f" );
				PopItemWidth( );
				Spacing( ); Spacing( );
			}




			if ( options.aa_real_switch )
				Separator( );

			Checkbox( "Enable Switch##10", &options.aa_real_switch );
			if ( options.aa_real_switch )
			{
				SameLine( 200 );
				Checkbox( "On LBY Update##10", &options.aa_real_switch_lby );
				Spacing( );
				ImGui::PushItemWidth( ImGui::GetContentRegionAvailWidth( ) * 0.65f );
				SliderInt( "Value##10", &options.aa_real_switch_value, -180, 180, "%.0f" );
				SliderInt( "Wait Time##10", &options.aa_real_switch_wait, 1, 200, "%.0f" );
				PopItemWidth( );
				Spacing( ); Spacing( );
			}

			if ( options.aa_real_jitter )
				Separator( );

			Checkbox( "Enable Jitter##9", &options.aa_real_jitter );
			if ( options.aa_real_jitter )
			{
				SameLine( 200 );
				Checkbox( "On LBY Update##9", &options.aa_real_jitter_lby );
				Spacing( );
				ImGui::PushItemWidth( ImGui::GetContentRegionAvailWidth( ) * 0.65f );
				SliderInt( "Range##9", &options.aa_real_jitter_range, 1, 180, "%.0f" );
				SliderInt( "Wait Time##9", &options.aa_real_jitter_wait, 0, 100, "%.0f" );
				PopItemWidth( );
				Spacing( ); Spacing( );
			}

			Separator( );
			ImGui::Combo( "Opposite##12", &options.aa_real_opposite, oppositee, IM_ARRAYSIZE( oppositee ) );
			Checkbox( "Invert Pitch on LBY update##10", &options.aa_lby_invertpitch );
			Checkbox( "Suppress 979 Activity##10", &options.aa_lby_antibacktrack );
			Spacing( ); Spacing( );

			Separator( );
			Spacing( );
			ImGui::PushItemWidth( ImGui::GetContentRegionAvailWidth( ) * 0.65f );
			SliderFloat( "Switch Value 1##9", &options.switch_value1, -180.f, 180.f, "%.0f" );
			SliderFloat( "Switch value 2##9", &options.switch_value2, -180.f, 180.f, "%.0f" );
			SliderFloat( "Add Value (Real)##9", &options.aa_real_addyaw, -180.f, 180.f, "%.0f" );

			PopItemWidth( );

			EndChild( );

			NextColumn( );

			Text( "Removals" );
			BeginChild( "Removals", ImVec2( 180, 220 ), true );
			ImGui::Selectable( "No Recoil", &options.aim_norecoil );
			ImGui::Selectable( "No Interpolation", &options.aim_nointerp );
			ImGui::Selectable( "No Spread", &options.aim_nospread );
			EndChild( );

			ImGui::Columns( 1, nullptr, false );
			ImGui::PopStyleVar( );
		}
	}
	ImGui::EndChild( );
}

void RenderEmptyTab( )
{
	auto& style = ImGui::GetStyle( );
	PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2{ 0,  style.WindowPadding.y } );
	float group_w = ImGui::GetCurrentWindow( )->Size.x - style.WindowPadding.x * 2;

	bool placeholder_true = true;

	ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2( 0, 0 ) );
	ImGui::ToggleButton( "Config", &placeholder_true, ImVec2{ group_w, 33.0f } );
	ImGui::PopStyleVar(2 );

	ImGui::BeginChild( "##body_content" );
	{
		auto message = "There's nothing here yet.";

		auto pos = ImGui::GetCurrentWindow( )->Pos;
		auto wsize = ImGui::GetCurrentWindow( )->Size;

		pos = pos + wsize / 2.0f;

		static float floats[] = { 10.f, 30.f, 180.f, -180.f, 0.f, 0.f, 90.f, 135.f, -30.f };
		const static int max_size = 60;

		static float refresh_time = 0.0f;



		static float values[ 90 ] = { 0 };

		static int values_offset = 0;

		if ( refresh_time == 0.0f )
			refresh_time = ImGui::GetTime( );

	
		while ( refresh_time < ImGui::GetTime( ) ) // Create dummy data at fixed 60 hz rate for the demo
		{
			values[ values_offset ] = Hooks::realyaw;
			values_offset = ( values_offset + 1 ) % IM_ARRAYSIZE( values );
			refresh_time += 1.0f / 30.0f;
		}


		//TripleSpacing( );
		//PlotLines( "histogram test", values, IM_ARRAYSIZE( values ), values_offset, "ur mom", -180.f, 180.f, ImVec2( 400, 100 ) );
		//TripleSpacing( );
		//SliderFloat( "chams 1##9", &options.chams1, 0.f, 1.f, "%.2f" );
		//SliderFloat( "chams 2##9", &options.chams2, 0.f, 1.f, "%.2f" );
		//SliderFloat( "chams 3##9", &options.chams3, 0.f, 1.f, "%.2f" );
		////	ImGui::RenderText(pos - ImGui::CalcTextSize(message) / 2.0f, message);

		//Checkbox( "ati gay af (mirrorcam)", &options.asd );
		

		{
			static cfg_manager manager;
			static bool clicked = false;
			Spacing( );
			ImGui::BeginChild( "Config", ImVec2( 219.f, 172.f + (clicked ? 26.f : 0.f) ), true, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar );
			if ( ImGui::BeginMenuBar( ) )
			{
				Text( "Config" );
				ImGui::EndMenuBar( );
			}
			
			static const string cfg_path = "F:\\Data\\VS\\serialization_test\\Debug\\cfg\\";
			static const string cfg_suffix = ".cfg";
			static const string cfg_default = cfg_path + "default" + cfg_suffix;

			if ( !fs::exists( cfg_default ) )
				manager.save( cfg_default, g_Options );
			
			
			static float timer = 0.f;
			static vector<string> cfg_files;
			//CalcResizePosSizeFromAnyCorner
			if ( ImGui::GetTime( ) > ( timer + 0.33f ) ) // update cfg every 1/3 sec to avoid bigger cpu usage even if its not a big chance
			{
				timer = ImGui::GetTime( );
				cfg_files.clear( );
				cfg_files.push_back( "default" );

				for ( fs::path p : fs::directory_iterator( cfg_path ) )
				{
					string name = p.stem( ).string( );
					if ( name != "default" )
						cfg_files.push_back( name );
				}
			}
				
			static int cfg_selected = 0;
			static char lol[ 15 ] = "name";		
			static bool set = false;
			static int action = -1;

			string path = cfg_path + cfg_files.at( cfg_selected ) + cfg_suffix;
			string new_path = cfg_path + string( lol ) + cfg_suffix;

			if ( clicked )
			{
				if ( Button( "done" ) ) set = true;
				SameLine( );
				if ( Button( "cancel" ) )
				{
					set = false;
					clicked = false;
				}
				SameLine( );
				PushItemWidth( 106.f );
				InputText( "", lol, 15 );
				PopItemWidth( );

			}
			
			
			float a = GetCursorPosY( );
			PushItemWidth( 140.f );
			ImGuiEx::ListBox( "##cfgs", &cfg_selected, cfg_files, 7 );
			/*ListBox( "##cfgs", &cfg_selected,
					 []( void* vec, int index, const char** out ) {
						 auto fuckimgui = reinterpret_cast<std::vector<std::string>*>( vec );
						 if ( index < 0 || index >= fuckimgui->size( ) )return false;
						 *out = fuckimgui->at( index ).c_str( );
						 return true;
					 }, reinterpret_cast<void*>( &cfg_files ), cfg_files.size( ), 7
						 );*/

			PopItemWidth( );
			
			
			auto btnpos = ImVec2( 159.f, a );
			static const auto btnsize = ImVec2( 50.f, 0.f );
			ImGui::SetCursorPos( btnpos );
			if ( Button( "new" , btnsize ) || ( set && action == 0 ) )
			{
				clicked = true;
				action = 0;
				if ( set )
				{
					action = -1;
					bool exists = false;
					for ( string file : cfg_files )
						if ( file == string( lol ) )
							exists = true;

					if ( !exists )
					{
						manager.save( new_path, g_Options );
						set = false;
						clicked = false;
					}
				}

			}
			if ( cfg_selected < 0 || cfg_files.at( cfg_selected ) == "default" )
			{
				ImGui::PushItemFlag( ImGuiItemFlags_Disabled, true );
				ImGui::PushStyleVar( ImGuiStyleVar_Alpha, ImGui::GetStyle( ).Alpha * 0.5f );
			}

			btnpos.y += ImGui::GetFrameHeightWithSpacing( );
			ImGui::SetCursorPos( btnpos );
			if ( Button( "rename", btnsize ) || ( set && action == 1 ) )
			{
				clicked = true;
				action = 1;
				if ( set )
				{
					action = -1;
					bool exists = false;
					for ( string file : cfg_files )
						if ( file == string( lol ) )
							exists = true;


					if ( !exists )
					{
						fs::rename( path, new_path );
						set = false;
						clicked = false;
					}
				}

			}

			btnpos.y += ImGui::GetFrameHeightWithSpacing( );
			ImGui::SetCursorPos( btnpos );
			if ( Button( "delete", btnsize ) )
			{
				fs::remove( path );
				cfg_selected = 0;
			} else if ( cfg_files.at( cfg_selected ) == "default" )
			{
				PopItemFlag( );
				PopStyleVar( );
			}
			btnpos.y += ImGui::GetFrameHeightWithSpacing( );
			ImGui::SetCursorPos( btnpos );
			if ( Button( "save", btnsize ) )
				manager.save( path, g_Options );

			btnpos.y += ImGui::GetFrameHeightWithSpacing( );
			ImGui::SetCursorPos( btnpos );
			if ( Button( "load", btnsize ) )
				manager.load( path, g_Options );

			if ( cfg_selected < 0 )
			{
				PopItemFlag( );
				PopStyleVar( );
			}
			ImGui::EndChild( );

		}
		
	/*	ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2{ 4.f,2.0f } );
		Checkbox( "Enabled##1", &options.esp_enabled );
		ImGui::Separator( );
		Checkbox( "Enemy only##1", &options.esp_enemies_only );
		Checkbox( "Bounding box##1", &options.esp_player_boxes );
		Checkbox( "Model outline##1", &options.esp_player_outlines );
		Checkbox( "Names##1", &options.esp_player_names );
		Checkbox( "Health##1", &options.esp_player_health );
		Checkbox( "Armor##1", &options.esp_player_armour );

		Checkbox( "Weapon##1", &options.esp_player_weapons );
		Checkbox( "Snaplines##1", &options.esp_player_snaplines );

		Checkbox( "Planted C4##1", &options.esp_planted_c4 );
		SameLine( ImGui::GetWindowWidth( ) - 40 );
		ImGuiEx::ColorEdit3( "##Planted_C41", &options.color_esp_c4 );

		Checkbox( "Defuse Kit##1", &options.esp_defuse_kit );
		SameLine( ImGui::GetWindowWidth( ) - 40 );
		ImGuiEx::ColorEdit3( "##Defuse_Kit1", &options.color_esp_defuse );

		Checkbox( "Dropped Weapons##1", &options.esp_dropped_weapons );
		SameLine( ImGui::GetWindowWidth( ) - 40 );
		ImGuiEx::ColorEdit3( "##Dropped_Weapons1", &options.color_esp_weapons );

		ImGuiEx::ColorEdit3( "Team Visible##1", &options.color_esp_ally_visible );
		ImGuiEx::ColorEdit3( "Team Invisible##1", &options.color_esp_ally_occluded );
		ImGuiEx::ColorEdit3( "Enemy Visible##1", &options.color_esp_enemy_visible );
		ImGuiEx::ColorEdit3( "Enemy Invisible##1", &options.color_esp_enemy_occluded );

		ImGui::PopStyleVar( 1 );
		ImGui::EndChild( );
	}*/

	}
	ImGui::EndChild( );

	//PopStyleVar( );
}

void fix_color( float& in )
{
	if ( in > 1.f )
		in = 1.f;

	if ( in < 0.f )
		in = 0.f;

}
Color Menu::GetColorOfStyle( ImGuiCol get )
{
	ImVec4 vec = _style.Colors[ get ];

	fix_color( vec.x );
	fix_color( vec.y );
	fix_color( vec.z );
	fix_color( vec.w );

	int r = (int)( ( vec.x ) * 255.f );
	int g = (int)( ( vec.y ) * 255.f );
	int b = (int)( ( vec.z ) * 255.f );
	int a = (int)( ( vec.w ) * 255.f );

	return Color( r, g, b, a );
}
void Menu::ChangeStyle( ImGuiCol set, Color color )
{
	float r = ( color.r( ) / 255.f );
	float g = ( color.g( ) / 255.f );
	float b = ( color.b( ) / 255.f );
	float a = ( color.a( ) / 255.f );
	_style.Colors[ set ] = ImVec4( r, g, b, a );
	p->Style = _style;
}

enum class KeyState
{
	None = 1,
	Down,
	Up,
	Pressed /*Down and then up*/
};

DEFINE_ENUM_FLAG_OPERATORS( KeyState );
HWND            m_hTargetWindow;
LONG_PTR        m_ulOldWndProc;
KeyState       m_iKeyMap[ 256 ];

std::function<void( void )> m_Hotkeys[ 256 ];

LRESULT ImGui_ImplDX9_WndProcHandler( HWND, UINT msg, WPARAM wParam, LPARAM lParam )
{
	ImGuiIO& io = ImGui::GetIO( );
	switch ( msg )
	{
		case WM_LBUTTONDOWN:
			io.MouseDown[ 0 ] = true;
			return true;
		case WM_LBUTTONUP:
			io.MouseDown[ 0 ] = false;
			return true;
		case WM_RBUTTONDOWN:
			io.MouseDown[ 1 ] = true;
			return true;
		case WM_RBUTTONUP:
			io.MouseDown[ 1 ] = false;
			return true;
		case WM_MBUTTONDOWN:
			io.MouseDown[ 2 ] = true;
			return true;
		case WM_MBUTTONUP:
			io.MouseDown[ 2 ] = false;
			return true;
		case WM_XBUTTONDOWN:
			if ( ( GET_KEYSTATE_WPARAM( wParam ) & MK_XBUTTON1 ) == MK_XBUTTON1 )
				io.MouseDown[ 3 ] = true;
			else if ( ( GET_KEYSTATE_WPARAM( wParam ) & MK_XBUTTON2 ) == MK_XBUTTON2 )
				io.MouseDown[ 4 ] = true;
			return true;
		case WM_XBUTTONUP:
			if ( ( GET_KEYSTATE_WPARAM( wParam ) & MK_XBUTTON1 ) == MK_XBUTTON1 )
				io.MouseDown[ 3 ] = false;
			else if ( ( GET_KEYSTATE_WPARAM( wParam ) & MK_XBUTTON2 ) == MK_XBUTTON2 )
				io.MouseDown[ 4 ] = false;
			return true;
		case WM_MOUSEWHEEL:
			io.MouseWheel += GET_WHEEL_DELTA_WPARAM( wParam ) > 0 ? +1.0f : -1.0f;
			return true;
		case WM_MOUSEMOVE:
			io.MousePos.x = (signed short)( lParam );
			io.MousePos.y = (signed short)( lParam >> 16 );
			return true;
		case WM_KEYDOWN:
			if ( wParam < 256 )
				io.KeysDown[ wParam ] = 1;
			return true;
		case WM_KEYUP:
			if ( wParam < 256 )
				io.KeysDown[ wParam ] = 0;
			return true;
		case WM_CHAR:
			// You can also use ToAscii()+GetKeyboardState() to retrieve characters.
			if ( wParam > 0 && wParam < 0x10000 )
				io.AddInputCharacter( (unsigned short)wParam );
			return true;
	}
	return 0;
}
bool ProcessMouseMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	auto key = VK_LBUTTON;
	auto state = KeyState::None;
	switch ( uMsg )
	{
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
			state = uMsg == WM_MBUTTONUP ? KeyState::Up : KeyState::Down;
			key = VK_MBUTTON;
			break;
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
			state = uMsg == WM_RBUTTONUP ? KeyState::Up : KeyState::Down;
			key = VK_RBUTTON;
			break;
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
			state = uMsg == WM_LBUTTONUP ? KeyState::Up : KeyState::Down;
			key = VK_LBUTTON;
			break;
		case WM_XBUTTONDOWN:
		case WM_XBUTTONUP:
			state = uMsg == WM_XBUTTONUP ? KeyState::Up : KeyState::Down;
			key = ( HIWORD( wParam ) == XBUTTON1 ? VK_XBUTTON1 : VK_XBUTTON2 );
			break;
		default:
			return false;
	}

	if ( state == KeyState::Up && m_iKeyMap[ key ] == KeyState::Down )
		m_iKeyMap[ key ] = KeyState::Pressed;
	else
		m_iKeyMap[ key ] = state;
	return true;
}

bool ProcessKeybdMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	auto key = wParam;
	auto state = KeyState::None;

	switch ( uMsg )
	{
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			state = KeyState::Down;
			break;
		case WM_KEYUP:
		case WM_SYSKEYUP:
			state = KeyState::Up;
			break;
		default:
			return false;
	}

	if ( state == KeyState::Up && m_iKeyMap[ int( key ) ] == KeyState::Down )
	{
		m_iKeyMap[ int( key ) ] = KeyState::Pressed;

		auto& hotkey_callback = m_Hotkeys[ key ];

		if ( hotkey_callback )
			hotkey_callback( );

		if ( Hooks::inputlisten )
		{
			Hooks::inputlisten = false;
			if ( input_receivedKeyval )
			{
				Menu::get( ).selecting_key = false;
				*input_receivedKeyval = int( key );
				std::cout << "WOM!";
			}
		}
	} else
	{
		m_iKeyMap[ int( key ) ] = state;
	}

	return true;
}
bool ProcessMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( uMsg )
	{
		case WM_MBUTTONDBLCLK:
		case WM_RBUTTONDBLCLK:
		case WM_LBUTTONDBLCLK:
		case WM_XBUTTONDBLCLK:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_LBUTTONDOWN:
		case WM_XBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
		case WM_LBUTTONUP:
		case WM_XBUTTONUP:
			return ProcessMouseMessage( uMsg, wParam, lParam );
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
			return ProcessKeybdMessage( uMsg, wParam, lParam );
		default:
			return false;
	}
}


KeyState GetKeyState( std::uint32_t vk )
{
	return m_iKeyMap[ vk ];
}
bool IsKeyDown( std::uint32_t vk )
{
	return m_iKeyMap[ vk ] == KeyState::Down;
}
bool WasKeyPressed( std::uint32_t vk )
{
	if ( m_iKeyMap[ vk ] == KeyState::Pressed )
	{
		m_iKeyMap[ vk ] = KeyState::Up;
		return true;
	}
	return false;
}

void RegisterHotkey( std::uint32_t vk, std::function<void( void )> f )
{
	m_Hotkeys[ vk ] = f;
}
void RemoveHotkey( std::uint32_t vk )
{
	m_Hotkeys[ vk ] = nullptr;
}

extern LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
LRESULT WINAPI WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{

	if ( g_pd3dDevice != NULL )
	{
		
		ProcessMessage( msg, wParam, lParam );

		/*if ( !Menu::get( ).selecting_key )
		{*/
		if ( Menu::get( ).IsVisible( ) && ImGui_ImplDX9_WndProcHandler( hWnd, msg, wParam, lParam ) )
			return true;
		//}
	}
	
	if ( ImGui_ImplWin32_WndProcHandler( hWnd, msg, wParam, lParam ) )
		return true;

	
	//

	switch ( msg )
	{
		case WM_SIZE:
			if ( g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED )
			{
				ImGui_ImplDX9_InvalidateDeviceObjects( );
				g_d3dpp.BackBufferWidth = LOWORD( lParam );
				g_d3dpp.BackBufferHeight = HIWORD( lParam );
				HRESULT hr = g_pd3dDevice->Reset( &g_d3dpp );
				if ( hr == D3DERR_INVALIDCALL )
					IM_ASSERT( 0 );
				ImGui_ImplDX9_CreateDeviceObjects( );
			}
			return 0;
		case WM_SYSCOMMAND:
			if ( ( wParam & 0xfff0 ) == SC_KEYMENU ) // Disable ALT application menu
				return 0;
			break;
		case WM_DESTROY:
			PostQuitMessage( 0 );
			return 0;
	}
	return DefWindowProc( hWnd, msg, wParam, lParam );
}

void Menu::Initialize( )
{
	_visible = true;

	WNDCLASSEX wc = { sizeof( WNDCLASSEX ), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle( NULL ), NULL, NULL, NULL, NULL, _T( "asd" ), NULL };
	RegisterClassEx( &wc );
	HWND hwnd = CreateWindow( _T( "asd" ), _T( "ads" ), WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, NULL, NULL, wc.hInstance, NULL );

	LPDIRECT3D9 pD3D;
	if ( ( pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) == NULL )
	{
		UnregisterClass( _T( "asd" ), wc.hInstance );
		cout << "unregister ";
		return;
	}

	ZeroMemory( &g_d3dpp, sizeof( g_d3dpp ) );
	g_d3dpp.Windowed = TRUE;
	g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	g_d3dpp.EnableAutoDepthStencil = TRUE;
	g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

	if ( pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice ) < 0 )
	{
		pD3D->Release( );
		UnregisterClass( _T( "asd" ), wc.hInstance );
		cout << "unregister2 ";
		return;
	}


	p = ImGui::CreateContext( );
	ImGuiIO& io = ImGui::GetIO( ); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

	
	ImGui_ImplDX9_Init( hwnd, g_pd3dDevice );
	CreateStyle( );
	MSG msg;
	ZeroMemory( &msg, sizeof( msg ) );
	ShowWindow( hwnd, SW_SHOWDEFAULT );
	UpdateWindow( hwnd );

	

	while ( msg.message != WM_QUIT )
	{
		if ( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
			continue;
		}

		Menu::get( ).Render( );
		
	}
}


void Menu::Shutdown( )
{
	ImGui_ImplDX9_Shutdown( );
}

void Menu::OnDeviceLost( )
{
	ImGui_ImplDX9_InvalidateDeviceObjects( );
}

void Menu::OnDeviceReset( )
{
	ImGui_ImplDX9_CreateDeviceObjects( );
}


void Menu::Render( )
{
	if ( !_visible || !p )
		return;

	ImGui_ImplDX9_NewFrame( );


	ImGui::GetIO( ).MouseDrawCursor = _visible;

	const auto sidebar_size = get_sidebar_size( );
	static int active_sidebar_tab = 0;

	ImGui::SetNextWindowPos( ImVec2{ 0, 0 }, ImGuiCond_Once );
	ImGui::SetNextWindowSize( ImVec2{ 815, 0 }, ImGuiCond_Once );

	if ( ImGui::Begin( "kysware",
					   &_visible,
					   ImGuiWindowFlags_NoCollapse |
					   ImGuiWindowFlags_NoResize ) )
	{
		ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2( 0, 0 ) );
		{
			ImGui::BeginChild( "##sidebar", sidebar_size, false );
			{
				//         ImGui::GetCurrentWindow()->Flags &= ~ImGuiWindowFlags_ShowBorders;

				ImGui::PushStyleVar( ImGuiStyleVar_FrameBorderSize, 0.f );

				render_tabs( sidebar_tabs, active_sidebar_tab, get_sidebar_item_width( ), get_sidebar_item_height( ), false );
				ImGui::PopStyleVar( );


			}
			
			ImGui::EndChild( );
		}
		ImGui::PopStyleVar( );
		ImGui::SameLine( );

		// Make the body the same vertical size as the sidebar
		// except for the width, which we will set to auto
		auto size = ImVec2{ 0.0f, sidebar_size.y };
		
		ImGui::BeginChild( "##body", size, false );

	

		tabs_info[ VISUALS ].different_window = tabs_info[ VISUALS ].set_new;
		if ( tabs_info[ VISUALS ].different_window )
		{
			if ( ImGui::Begin( sidebar_tabs[ VISUALS ], &tabs_info[ VISUALS ].set_new, ImGuiWindowFlags_NoCollapse ) )
			{
				
				if ( tabs_info[ VISUALS ].set_pos )
				{
					ImGui::SetWindowSize( ImVec2( 850.f - get_sidebar_item_width( ), sidebar_size.y + 15.f ) );
					ImGui::SetWindowPos( tabs_info[ VISUALS ].spawn_pos );
					tabs_info[ VISUALS ].set_pos = false;
				}

				RenderEspTab( );
				ImGui::End( );
			}
		} 

		tabs_info[ HVH ].different_window = tabs_info[ HVH ].set_new;
		if ( tabs_info[ HVH ].different_window )
		{
			if ( ImGui::Begin( sidebar_tabs[ HVH ], &tabs_info[ HVH ].set_new, ImGuiWindowFlags_NoCollapse ) )
			{
				
				if ( tabs_info[ HVH ].set_pos )
				{
					ImGui::SetWindowSize( ImVec2( 850.f - get_sidebar_item_width( ), sidebar_size.y + 15.f ) );
					ImGui::SetWindowPos( tabs_info[ HVH ].spawn_pos );
					tabs_info[ HVH ].set_pos = false;
				}

				RenderAimTab( );
				ImGui::End( );
			}
		} 

		tabs_info[ MISC ].different_window = tabs_info[ MISC ].set_new;
		if ( tabs_info[ MISC ].different_window )
		{
			if ( ImGui::Begin( sidebar_tabs[ MISC ], &tabs_info[ MISC ].set_new, ImGuiWindowFlags_NoCollapse ) )
			{	
				if ( tabs_info[ MISC ].set_pos )
				{
					ImGui::SetWindowSize( ImVec2( 850.f - get_sidebar_item_width( ), sidebar_size.y + 15.f ) );
					ImGui::SetWindowPos( tabs_info[ MISC ].spawn_pos );
					tabs_info[ MISC ].set_pos = false;
				}

				RenderMiscTab( );
				ImGui::End( );
			}
		}

		tabs_info[ COLORS ].different_window = tabs_info[ COLORS ].set_new;
		if ( tabs_info[ COLORS ].different_window )
		{
			if ( ImGui::Begin( sidebar_tabs[ COLORS ], &tabs_info[ COLORS ].set_new, ImGuiWindowFlags_NoCollapse ) )
			{
				if ( tabs_info[ COLORS ].set_pos )
				{
					ImGui::SetWindowSize( ImVec2( 850.f - get_sidebar_item_width( ), sidebar_size.y + 15.f ) );
					ImGui::SetWindowPos( tabs_info[ COLORS ].spawn_pos );
					tabs_info[ COLORS ].set_pos = false;
				}

				RenderColorTab( );
				ImGui::End( );
			}
		}

		tabs_info[ CONFIG ].different_window = tabs_info[ CONFIG ].set_new;
		if ( tabs_info[ CONFIG ].different_window )
		{
			if ( ImGui::Begin( sidebar_tabs[ CONFIG ], &tabs_info[ CONFIG ].set_new, ImGuiWindowFlags_NoCollapse ) )
			{
				
				if ( tabs_info[ CONFIG ].set_pos )
				{
					ImGui::SetWindowSize( ImVec2( 850.f - get_sidebar_item_width( ), sidebar_size.y + 15.f ) );
					ImGui::SetWindowPos( tabs_info[ CONFIG ].spawn_pos );
					tabs_info[ CONFIG ].set_pos = false;
				}

				RenderEmptyTab( );
				ImGui::End( );
			}
		}

		switch ( active_sidebar_tab )
		{
			case VISUALS:
				RenderEspTab( );
				break;

			case HVH:
				RenderAimTab( );
				break;

			case MISC:
				RenderMiscTab( );
				break;

			case COLORS:
				RenderColorTab( );
				break;

			case CONFIG:
				RenderEmptyTab( );
				break;
		}

	

		ImGui::EndChild( );
		ImGui::End( );
	}

	//  ImGui::PopStyleVar();
	//ImGui::PushStyleColor( ImGuiCol_WindowBg, ImVec4( 0.3, 0.3, 0.3, 0.3 ) );

	//if ( ImGui::Begin( "Mirrorcam Setup", &_visible, ImGuiWindowFlags_NoCollapse ) )
	//{
	//	options.mirror_h = ImGui::GetWindowHeight( );
	//	options.mirror_w = ImGui::GetWindowWidth( );
	//	options.mirror_x = ImGui::GetWindowPos( ).x;
	//	options.mirror_y = ImGui::GetWindowPos( ).y;

	//	ImGui::End( );
	//}
	//ImGui::PopStyleColor( );

	//ImGui::Render( );
	//ImGui_ImplDX9_RenderDrawData( ImGui::GetDrawData( ) );

	ImGui::EndFrame( );
	g_pd3dDevice->SetRenderState( D3DRS_ZENABLE, false );
	g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, false );
	g_pd3dDevice->SetRenderState( D3DRS_SCISSORTESTENABLE, false );

	static ImVec4 clear_color = ImVec4( 0.45f, 0.55f, 0.60f, 1.00f );

	D3DCOLOR clear_col_dx = D3DCOLOR_RGBA( (int)( clear_color.x*255.0f ), (int)( clear_color.y*255.0f ), (int)( clear_color.z*255.0f ), (int)( clear_color.w*255.0f ) );
	g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0 );
	//g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0 );
	if ( g_pd3dDevice->BeginScene( ) >= 0 )
	{
		ImGui::Render( );
		ImGui_ImplDX9_RenderDrawData( ImGui::GetDrawData( ) );
		g_pd3dDevice->EndScene( );
	}

	HRESULT result = g_pd3dDevice->Present( NULL, NULL, NULL, NULL );

	// Handle loss of D3D9 device
	if ( result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel( ) == D3DERR_DEVICENOTRESET )
	{
		ImGui_ImplDX9_InvalidateDeviceObjects( );
		g_pd3dDevice->Reset( &g_d3dpp );
		ImGui_ImplDX9_CreateDeviceObjects( );
	}

}

void Menu::Show( )
{
	_visible = true;
}

void Menu::Hide( )
{
	_visible = false;
}

void Menu::Toggle( )
{
	_visible = !_visible;
}

void Menu::CreateStyle( )
{
	_style.Alpha = 1.0f;    // Global alpha applies to everything in ImGui
	_style.WindowPadding = ImVec2( 10, 10 );                      // Padding within a window
	_style.WindowMinSize = ImVec2( 32, 32 );                    // Minimum window size
	_style.WindowRounding = 0.0f;                                // Radius of window corners rounding. Set to 0.0f to have rectangular windows
	_style.WindowTitleAlign = ImVec2( 0.0f, 0.5f );                  // Alignment for title bar text
	_style.ChildRounding = 0.0f;                                // Radius of child window corners rounding. Set to 0.0f to have rectangular child windows
	_style.FramePadding = ImVec2( 4, 4 );                        // Padding within a framed rectangle (used by most widgets)
	_style.FrameRounding = 0.0f;                                // Radius of frame corners rounding. Set to 0.0f to have rectangular frames (used by most widgets).
	_style.ItemSpacing = ImVec2( 5, 5 );                        // Horizontal and vertical spacing between widgets/lines
	_style.ItemInnerSpacing = ImVec2( 6, 4 );                        // Horizontal and vertical spacing between within elements of a composed widget (e.g. a slider and its label)
	_style.TouchExtraPadding = ImVec2( 0, 0 );                        // Expand reactive bounding box for touch-based system where touch position is not accurate enough. Unfortunately we don't sort widgets so priority on overlap will always be given to the first widget. So don't grow this too much!
	_style.IndentSpacing = 21.0f;                               // Horizontal spacing when e.g. entering a tree node. Generally == (FontSize + FramePadding.x*2).
	_style.ColumnsMinSpacing = 6.0f;                                // Minimum horizontal spacing between two columns
	_style.ScrollbarSize = 16.0f;                               // Width of the vertical scrollbar, Height of the horizontal scrollbar
	_style.ScrollbarRounding = 0.0f;                                // Radius of grab corners rounding for scrollbar
	_style.GrabMinSize = 10.5f;                               // Minimum width/height of a grab box for slider/scrollbar
	_style.GrabRounding = 0.0f;                                // Radius of grabs corners rounding. Set to 0.0f to have rectangular slider grabs.
	_style.ButtonTextAlign = ImVec2( 0.5f, 0.5f );                  // Alignment of button text when button is larger than text.
	_style.DisplayWindowPadding = ImVec2( 22, 22 );                      // Window positions are clamped to be IsVisible within the display area by at least this amount. Only covers regular windows.
	_style.DisplaySafeAreaPadding = ImVec2( 4, 4 );                        // If you cannot see the edge of your screen (e.g. on a TV) increase the safe area padding. Covers popups/tooltips as well regular windows.
	_style.AntiAliasedLines = true;                                // Enable anti-aliasing on lines/borders. Disable if you are really short on CPU/GPU.
	_style.AntiAliasedFill = true;                                // Enable anti-aliasing on filled shapes (rounded rectangles, circles, etc.)
	_style.CurveTessellationTol = 1.25f;                               // Tessellation tolerance. Decrease for highly tessellated curves (higher quality, more polygons), increase to reduce quality.
	_style.FrameBorderSize = 1.f;
	
	
	//GetImvec(Color::GoldenRod);
	//_style.Colors[ ImGuiCol_Text ] = ImVec4( 1.0f, 1.0, 1.0f, 1.00f );
	//_style.Colors[ ImGuiCol_TextDisabled ] = ImVec4( 0.9f, 0.9f, 1.f, 1.f );
	//_style.Colors[ ImGuiCol_WindowBg ] = ImVec4( 0.12, 0.15, 0.22, 0.94 ); //OG: 0.09, 0.09,  0.09 new: 0.20, 0.23,  0.31 
	//_style.Colors[ ImGuiCol_ChildBg ] = ImVec4( 0.17, 0.17, 0.26, .2f );
	//_style.Colors[ ImGuiCol_PopupBg ] = ImVec4( 0.12, 0.15, 0.22, 0.94 + .05f );
	//_style.Colors[ ImGuiCol_Border ] = ImVec4( 0.25, 0.28, 0.37, 1.0f ); //0.8 //0.28
	//_style.Colors[ ImGuiCol_BorderShadow ] = ImVec4( 0.22f, 0.21f, 0.28f, 1.00f ); //1
	//_style.Colors[ ImGuiCol_FrameBg ] = ImVec4( 0.10, 0.13, 0.20, 0.94 + .1f ); //0.13,  0.13, 0.13
	//_style.Colors[ ImGuiCol_FrameBgHovered ] = ImVec4( 0.21 + 0.1f, 0.21 + 0.1f, 0.21 + 0.1f, 1.0f );
	//_style.Colors[ ImGuiCol_FrameBgActive ] = ImVec4( 0.21 + 0.2f, 0.21 + 0.2f, 0.21 + 0.2f, 255 );
	//_style.Colors[ ImGuiCol_TitleBg ] = ImVec4( 0.25, 0.28, 0.37, 1.00 );
	//_style.Colors[ ImGuiCol_TitleBgCollapsed ] = ImVec4( 0.30f, 0.38f, 0.35f, 0.75f );
	//_style.Colors[ ImGuiCol_TitleBgActive ] = ImVec4( 0.25, 0.28, 0.37, 1.00 );
	//_style.Colors[ ImGuiCol_MenuBarBg ] = ImVec4( 0.12, 0.15, 0.22, 0.94 - 0.8f );
	//_style.Colors[ ImGuiCol_ScrollbarBg ] = ImVec4( 24 + .05f, 24 + .05f, 24 + .05f, .1f );
	//_style.Colors[ ImGuiCol_ScrollbarGrab ] = ImVec4( 0.25, 0.28, 0.37, 1.00f );
	//_style.Colors[ ImGuiCol_ScrollbarGrabHovered ] = ImVec4( 0.21 + 0.1f, 0.21 + 0.1f, 0.21 + 0.1f, 1.00f );
	//_style.Colors[ ImGuiCol_ScrollbarGrabActive ] = ImVec4( 0.21 + 0.2f, 0.21 + 0.2f, 0.21 + 0.2f, 1.00f );
	//_style.Colors[ ImGuiCol_PopupBg ] = ImVec4( 0.12, 0.15, 0.22, 0.94 + .1f );
	//_style.Colors[ ImGuiCol_CheckMark ] = ImVec4( 0.298f, 1.000f, 0.455f, 1.000f );//ImVec4(0.25, 0.28, 0.37, 1.00f);
	//_style.Colors[ ImGuiCol_SliderGrab ] = ImVec4( 0.03, 0.61, 0.92, 0.70f );
	//_style.Colors[ ImGuiCol_SliderGrabActive ] = ImVec4( 0.03 + 0.1f, 0.61 + 0.1f, 0.92 + 0.1f, 0.70f );
	//_style.Colors[ ImGuiCol_Button ] = ImVec4( 0.25, 0.28, 0.37, 1.00f ); //OG: 0.21, 0.21,  0.21 , 0.25, 0.28, 0.37
	//_style.Colors[ ImGuiCol_ButtonHovered ] = ImVec4( 0.03 + 0.1f, 0.61 + 0.1f, 0.92 + 0.1f, 1.00f );
	//_style.Colors[ ImGuiCol_ButtonActive ] = ImVec4( 0.03, 0.61, 0.92, 1.00f );//0.56, 0.27, 0.68 ORIGINAL COLORS
	//_style.Colors[ ImGuiCol_Header ] = ImVec4( 0.25, 0.28, 0.37, 1.00f );
	//_style.Colors[ ImGuiCol_HeaderHovered ] = ImVec4( 0.21 + 0.1f, 0.21 + 0.1f, 0.21 + 0.1f, 1.00f );
	//_style.Colors[ ImGuiCol_HeaderActive ] = ImVec4( 0.21 + 0.2f, 0.21 + 0.2f, 0.21 + 0.2f, 1.00f );
	//_style.Colors[ ImGuiCol_Separator ] = ImVec4( 0.25, 0.28, 0.37, 1.00f );
	//_style.Colors[ ImGuiCol_SeparatorHovered ] = ImVec4( 0.21 + 0.1f, 0.21 + 0.1f, 0.21 + 0.1f, 1.00f );
	//_style.Colors[ ImGuiCol_SeparatorActive ] = ImVec4( 0.21 + 0.2f, 0.21 + 0.2f, 0.21 + 0.2f, 1.00f );
	//_style.Colors[ ImGuiCol_ResizeGrip ] = ImVec4( 0.25, 0.28, 0.37, 1.00f );
	//_style.Colors[ ImGuiCol_ResizeGripHovered ] = ImVec4( 0.21 + 0.1f, 0.21 + 0.1f, 0.21 + 0.1f, 1.00f );
	//_style.Colors[ ImGuiCol_ResizeGripActive ] = ImVec4( 0.03, 0.61, 0.92, 1.00f );//ImVec4(0.21 + 0.2f, 0.21 + 0.2f, 0.21 + 0.2f, 1.00f);
	//_style.Colors[ ImGuiCol_CloseButton ] = ImVec4( 0.25, 0.28, 0.37, 1.00f );
	//_style.Colors[ ImGuiCol_CloseButtonHovered ] = ImVec4( 0.21 + 0.1f, 0.21 + 0.1f, 0.21 + 0.1f, 1.00f );
	//_style.Colors[ ImGuiCol_CloseButtonActive ] = ImVec4( 0.21 + 0.2f, 0.21 + 0.2f, 0.21 + 0.2f, 1.00f );
	//_style.Colors[ ImGuiCol_PlotLines ] = ImVec4( 0.25, 0.28, 0.37, 1.00f );
	//_style.Colors[ ImGuiCol_PlotLinesHovered ] = ImVec4( 0.21 + 0.1f, 0.21 + 0.1f, 0.21 + 0.1f, 1.00f );
	//_style.Colors[ ImGuiCol_PlotHistogram ] = ImVec4( 0.25, 0.28, 0.37, 1.00f );
	//_style.Colors[ ImGuiCol_PlotHistogramHovered ] = ImVec4( 0.21 + 0.1f, 0.21 + 0.1f, 0.21 + 0.1f, 1.00f );
	//_style.Colors[ ImGuiCol_TextSelectedBg ] = ImVec4( 0.21 + 0.1f, 0.21 + 0.1f, 0.21 + 0.1f, 1.00f );
	//_style.Colors[ ImGuiCol_ModalWindowDarkening ] = ImVec4( 0.0f, 0.0f, 0.0f, 0.75f );

	StyleColorsDark( &_style );

	options.menu_button = GetColorOfStyle( ImGuiCol_Button );
	options.menu_button_hovered = GetColorOfStyle( ImGuiCol_ButtonHovered );
	options.menu_button_active = GetColorOfStyle( ImGuiCol_ButtonActive );

	options.Text = GetColorOfStyle( ImGuiCol_Text );
	options.TextDisabled = GetColorOfStyle( ImGuiCol_TextDisabled );
	options.WindowBg = GetColorOfStyle( ImGuiCol_WindowBg );
	options.ChildWindowBg = GetColorOfStyle( ImGuiCol_ChildBg );
	options.PopupBg = GetColorOfStyle( ImGuiCol_PopupBg );
	options.Border = GetColorOfStyle( ImGuiCol_Border );
	options.BorderShadow = GetColorOfStyle( ImGuiCol_BorderShadow );

	options.ImGuiCol_FrameBg = GetColorOfStyle( ImGuiCol_FrameBg );
	options.ImGuiCol_FrameBgHovered = GetColorOfStyle( ImGuiCol_FrameBgHovered );
	options.ImGuiCol_FrameBgActive = GetColorOfStyle( ImGuiCol_FrameBgActive );
	options.ImGuiCol_TitleBg = GetColorOfStyle( ImGuiCol_TitleBg );
	options.ImGuiCol_TitleBgCollapsed = GetColorOfStyle( ImGuiCol_TitleBgCollapsed );
	options.ImGuiCol_TitleBgActive = GetColorOfStyle( ImGuiCol_TitleBgActive );

	options.ImGuiCol_MenuBarBg = GetColorOfStyle( ImGuiCol_MenuBarBg );
	options.ImGuiCol_ScrollbarBg = GetColorOfStyle( ImGuiCol_ScrollbarBg );
	options.ImGuiCol_ScrollbarGrab = GetColorOfStyle( ImGuiCol_ScrollbarGrab );
	options.ImGuiCol_ScrollbarGrabHovered = GetColorOfStyle( ImGuiCol_ScrollbarGrabHovered );
	options.ImGuiCol_ScrollbarGrabActive = GetColorOfStyle( ImGuiCol_ScrollbarGrabActive );
	options.ImGuiCol_ComboBg = GetColorOfStyle( ImGuiCol_PopupBg );
	options.ImGuiCol_CheckMark = GetColorOfStyle( ImGuiCol_CheckMark );

	options.ImGuiCol_SliderGrab = GetColorOfStyle( ImGuiCol_SliderGrab );
	options.ImGuiCol_SliderGrabActive = GetColorOfStyle( ImGuiCol_SliderGrabActive );
	options.ImGuiCol_Header = GetColorOfStyle( ImGuiCol_Header );
	options.ImGuiCol_HeaderHovered = GetColorOfStyle( ImGuiCol_HeaderHovered );
	options.ImGuiCol_HeaderActive = GetColorOfStyle( ImGuiCol_HeaderActive );
	options.ImGuiCol_Column = GetColorOfStyle( ImGuiCol_Separator );
	options.ImGuiCol_ColumnHovered = GetColorOfStyle( ImGuiCol_SeparatorHovered );
	options.ImGuiCol_ColumnActive = GetColorOfStyle( ImGuiCol_SeparatorActive );
	options.ImGuiCol_ResizeGrip = GetColorOfStyle( ImGuiCol_ResizeGrip );
	options.ImGuiCol_ResizeGripHovered = GetColorOfStyle( ImGuiCol_ResizeGripHovered );
	options.ImGuiCol_ResizeGripActive = GetColorOfStyle( ImGuiCol_ResizeGripActive );
	options.ImGuiCol_CloseButton = GetColorOfStyle( ImGuiCol_CloseButton );
	options.ImGuiCol_CloseButtonHovered = GetColorOfStyle( ImGuiCol_CloseButtonHovered );
	options.ImGuiCol_CloseButtonActive = GetColorOfStyle( ImGuiCol_CloseButtonActive );
	options.ImGuiCol_PlotLines = GetColorOfStyle( ImGuiCol_PlotLines );
	options.ImGuiCol_PlotLinesHovered = GetColorOfStyle( ImGuiCol_PlotLinesHovered );
	options.ImGuiCol_PlotHistogram = GetColorOfStyle( ImGuiCol_PlotHistogram );
	options.ImGuiCol_PlotHistogramHovered = GetColorOfStyle( ImGuiCol_PlotHistogramHovered );
	options.ImGuiCol_TextSelectedBg = GetColorOfStyle( ImGuiCol_TextSelectedBg );
	options.ImGuiCol_ModalWindowDarkening = GetColorOfStyle( ImGuiCol_ModalWindowDarkening );


	p->Style = _style;
	//ImGui::set
	//_style.
}


template<typename T>
void pop_front( std::vector<T>& vec )
{
	assert( !vec.empty( ) );
	vec.erase( vec.begin( ) );
}
