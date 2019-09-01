#include "visuals.hpp"

#include "../options.hpp"
#include "../valve_sdk/csgostructs.hpp"
#include "../helpers/math.hpp"
#include "../helpers/utils.hpp"
#include "../Globals.h"
vgui::HFont esp_font;
vgui::HFont defuse_font;
vgui::HFont dropped_weapons_font;

using namespace Hooks;
// ESP Context
// This is used so that we dont have to calculate player color and position
// on each individual function over and over
struct
{
    C_BasePlayer* pl;
    bool          is_enemy;
    bool          is_visible;
    Color         clr;
    Vector        head_pos;
    Vector        feet_pos;
    RECT          bbox;
} esp_ctx;

RECT GetBBox(C_BaseEntity* ent)
{
    RECT rect{};
    auto collideable = ent->GetCollideable();

    if(!collideable)
        return rect;

    auto min = collideable->OBBMins();
    auto max = collideable->OBBMaxs();

    const matrix3x4_t& trans = ent->m_rgflCoordinateFrame();

    Vector points[] = {
        Vector(min.x, min.y, min.z),
        Vector(min.x, max.y, min.z),
        Vector(max.x, max.y, min.z),
        Vector(max.x, min.y, min.z),
        Vector(max.x, max.y, max.z),
        Vector(min.x, max.y, max.z),
        Vector(min.x, min.y, max.z),
        Vector(max.x, min.y, max.z)
    };

    Vector pointsTransformed[8];
    for(int i = 0; i < 8; i++) {
        Math::VectorTransform(points[i], trans, pointsTransformed[i]);
    }

    Vector pos = ent->m_vecOrigin();
    Vector screen_points[8] = {};

    for(int i = 0; i < 8; i++) {
        if(!Math::WorldToScreen(pointsTransformed[i], screen_points[i]))
            return rect;
    }

    auto left = screen_points[0].x;
    auto top = screen_points[0].y;
    auto right = screen_points[0].x;
    auto bottom = screen_points[0].y;

    for(int i = 1; i < 8; i++) {
        if(left > screen_points[i].x)
            left = screen_points[i].x;
        if(top < screen_points[i].y)
            top = screen_points[i].y;
        if(right < screen_points[i].x)
            right = screen_points[i].x;
        if(bottom > screen_points[i].y)
            bottom = screen_points[i].y;
    }
    return RECT{ (long)left, (long)top, (long)right, (long)bottom };
}
//--------------------------------------------------------------------------------
bool Visuals::CreateFonts()
{
    esp_font = g_VGuiSurface->CreateFont_();
    defuse_font = g_VGuiSurface->CreateFont_();
    dropped_weapons_font = g_VGuiSurface->CreateFont_();
	buildFont = g_VGuiSurface->CreateFont_();
	logoFont = g_VGuiSurface->CreateFont_();
	VerdanaBig = g_VGuiSurface->CreateFont_();

    g_VGuiSurface->SetFontGlyphSet(esp_font, "Arial", 14, 700, 0, 0, FONTFLAG_OUTLINE);
    g_VGuiSurface->SetFontGlyphSet(defuse_font, "Arial", 15, 700, 0, 0, FONTFLAG_OUTLINE);
    g_VGuiSurface->SetFontGlyphSet(dropped_weapons_font, "Arial", 18, 700, 0, 0, FONTFLAG_OUTLINE);
	g_VGuiSurface->SetFontGlyphSet(VerdanaBig, "Arial", 33, FW_EXTRABOLD, 0,50,  FONTFLAG_DROPSHADOW  | FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE);
	g_VGuiSurface->SetFontGlyphSet(buildFont, "Arial", 21, 150, 0, 0, FONTFLAG_OUTLINE);
	g_VGuiSurface->SetFontGlyphSet(logoFont, "Arial", 24, 150, 0, 0, FONTFLAG_OUTLINE);
    return true;
}
//--------------------------------------------------------------------------------
void Visuals::DestroyFonts()
{
    // Is there a way to destroy vgui fonts?
    // TODO: Find out
}
//--------------------------------------------------------------------------------
bool Visuals::Player::Begin(C_BasePlayer* pl)
{
    esp_ctx.pl = pl;
    esp_ctx.is_enemy = g_LocalPlayer->m_iTeamNum() != pl->m_iTeamNum();
    esp_ctx.is_visible = g_LocalPlayer->CanSeePlayer(pl, HITBOX_CHEST);

    if(!esp_ctx.is_enemy && g_Options.esp_enemies_only)
        return false;

    esp_ctx.clr = esp_ctx.is_enemy ? (esp_ctx.is_visible ? g_Options.color_esp_enemy_visible : g_Options.color_esp_enemy_occluded) : (esp_ctx.is_visible ? g_Options.color_esp_ally_visible : g_Options.color_esp_ally_occluded);

	if (g_Options.highlighted_meme == pl->EntIndex())
		esp_ctx.clr = Color(0, 128, 128);

    auto head = pl->GetHitboxPos(HITBOX_HEAD);
    auto origin = pl->m_vecOrigin();


	
    if(!Math::WorldToScreen(head, esp_ctx.head_pos) ||
       !Math::WorldToScreen(origin, esp_ctx.feet_pos))
        return false;

    auto h = fabs(esp_ctx.head_pos.y - esp_ctx.feet_pos.y);
    auto w = h / 1.65f;

    esp_ctx.bbox.left = static_cast<long>(esp_ctx.feet_pos.x - w * 0.5f);
    esp_ctx.bbox.right = static_cast<long>(esp_ctx.bbox.left + w);
    esp_ctx.bbox.bottom = static_cast<long>(esp_ctx.feet_pos.y);
    esp_ctx.bbox.top = static_cast<long>(esp_ctx.head_pos.y);

    return true;
}
//--------------------------------------------------------------------------------
void Visuals::Player::RenderBox(bool drawOpt, Color optional)
{
	Color col = Color::Black;

	if (drawOpt) col._CColor[3] = optional.a();
	else col._CColor[3] = esp_ctx.clr.a();


    g_VGuiSurface->DrawSetColor(drawOpt ? optional : esp_ctx.clr);
    g_VGuiSurface->DrawOutlinedRect(esp_ctx.bbox.left, esp_ctx.bbox.top, esp_ctx.bbox.right, esp_ctx.bbox.bottom);
    g_VGuiSurface->DrawSetColor(col);
    g_VGuiSurface->DrawOutlinedRect(esp_ctx.bbox.left - 1, esp_ctx.bbox.top - 1, esp_ctx.bbox.right + 1, esp_ctx.bbox.bottom + 1);
    g_VGuiSurface->DrawOutlinedRect(esp_ctx.bbox.left + 1, esp_ctx.bbox.top + 1, esp_ctx.bbox.right - 1, esp_ctx.bbox.bottom - 1);
}

void Visuals::Player::SetAlpha(int alpha)
{
	esp_ctx.clr._CColor[3] = alpha;
}
//--------------------------------------------------------------------------------
void Visuals::Player::RenderName()
{
    wchar_t* buf = new wchar_t[128];

    player_info_t info = esp_ctx.pl->GetPlayerInfo();

    if(MultiByteToWideChar(CP_UTF8, 0, info.szName, -1, buf, 128) > 0) 
	{
        int tw, th;
        g_VGuiSurface->GetTextSize(esp_font, buf, tw, th);

        g_VGuiSurface->DrawSetTextFont(esp_font);
        g_VGuiSurface->DrawSetTextColor(esp_ctx.clr);
        g_VGuiSurface->DrawSetTextPos(esp_ctx.feet_pos.x - tw / 2, esp_ctx.head_pos.y - th);
        g_VGuiSurface->DrawPrintText(buf, wcslen(buf));
    }
	delete[] buf;
}
void Visuals::Player::RenderLastknown(int alpha)
{
	{
		wchar_t buf[128];

		player_info_t info = esp_ctx.pl->GetPlayerInfo();

		if (MultiByteToWideChar(CP_UTF8, 0, info.szName, -1, buf, 128) > 0) {
			int tw, th;
			g_VGuiSurface->GetTextSize(esp_font, buf, tw, th);

			Color col = Color::Gray;
			col._CColor[3] = alpha;
			g_VGuiSurface->DrawSetTextFont(esp_font);
			g_VGuiSurface->DrawSetTextColor(col);
			g_VGuiSurface->DrawSetTextPos(esp_ctx.feet_pos.x - tw / 2, esp_ctx.head_pos.y - th);
			g_VGuiSurface->DrawPrintText(buf, wcslen(buf));
		}
	}
}
//--------------------------------------------------------------------------------
void Visuals::Player::RenderHealth()
{
    auto  hp = esp_ctx.pl->m_iHealth();
    float box_h = (float)fabs(esp_ctx.bbox.bottom - esp_ctx.bbox.top);
    //float off = (box_h / 6.f) + 5;
    float off = 8;

    auto height = (((box_h * hp) / 100));

    int red = int(255 - (hp * 2.55f));
    int green = int(hp * 2.55f);

    int x = esp_ctx.bbox.left - off;
    int y = esp_ctx.bbox.top;
    int w = 4;
    int h = box_h;

	Color col = Color::Black;
	col._CColor[3] = esp_ctx.clr.a();

	Color col2(red, green, 0, 255);
	col2._CColor[3] = esp_ctx.clr.a();

    g_VGuiSurface->DrawSetColor(col);
    g_VGuiSurface->DrawFilledRect(x, y, x + w, y + h);

    g_VGuiSurface->DrawSetColor(col2);
    g_VGuiSurface->DrawFilledRect(x + 1, y + 1, x + w - 1, y + height - 2);
}

void Visuals::Player::RenderArmour()
{
	auto  armour = esp_ctx.pl->m_ArmorValue();
	float box_h = (float)fabs(esp_ctx.bbox.bottom - esp_ctx.bbox.top);
	//float off = (box_h / 6.f) + 5;
	float off = 4;

	auto height = (((box_h * armour) / 100));

	int x = esp_ctx.bbox.right + off;
	int y = esp_ctx.bbox.top;
	int w = 4;
	int h = box_h;

	Color col = Color::Black;
	col._CColor[3] = esp_ctx.clr.a();

	Color col2(0, 50, 255, 255);
	col2._CColor[3] = esp_ctx.clr.a();

	g_VGuiSurface->DrawSetColor(col);
	g_VGuiSurface->DrawFilledRect(x, y, x + w, y + h);

	g_VGuiSurface->DrawSetColor(col2);
	g_VGuiSurface->DrawFilledRect(x + 1, y + 1, x + w - 1, y + height - 2);
}
//--------------------------------------------------------------------------------
void Visuals::Player::RenderWeapon()
{
    wchar_t buf[80];

    auto weapon = esp_ctx.pl->m_hActiveWeapon().Get();

    if(!weapon) return;

    if(MultiByteToWideChar(CP_UTF8, 0, weapon->GetCSWeaponData()->szWeaponName + 7, -1, buf, 80) > 0) {
        int tw, th;
        g_VGuiSurface->GetTextSize(esp_font, buf, tw, th);

        g_VGuiSurface->DrawSetTextFont(esp_font);
        g_VGuiSurface->DrawSetTextColor(esp_ctx.clr);
        g_VGuiSurface->DrawSetTextPos(esp_ctx.feet_pos.x - tw / 2, esp_ctx.feet_pos.y);
        g_VGuiSurface->DrawPrintText(buf, wcslen(buf));
    }
}
//--------------------------------------------------------------------------------
void Visuals::Player::RenderSnapline()
{
    int screen_w, screen_h;
    g_EngineClient->GetScreenSize(screen_w, screen_h);

    g_VGuiSurface->DrawSetColor(esp_ctx.clr);

    g_VGuiSurface->DrawLine(
        screen_w / 2,
        screen_h,
        esp_ctx.feet_pos.x,
        esp_ctx.feet_pos.y);
}

void Visuals::Player::RenderSkeleton( )
{
	auto studio_hdr = g_MdlInfo->GetStudiomodel( esp_ctx.pl->GetModel( ) );
	if ( studio_hdr )
	{
		mstudiobone_t* bone;
		Vector child_screen, parent_screen;
		int parent;
		static matrix3x4_t pBoneToWorldOut[ 128 ];

		if ( esp_ctx.pl->SetupBones( pBoneToWorldOut, 128, 0x100, global->curtime ) )
		{
			for ( int i = 0; i < studio_hdr->numbones; i++ )
			{
				bone = studio_hdr->GetBone( i );
				parent = bone->parent;
				if ( bone && ( bone->flags & 256 ) && bone->flags != -1 && ( parent != -1 ) )
				{

					if ( !Math::WorldToScreen( Vector( pBoneToWorldOut[ i ][ 0 ][ 3 ], pBoneToWorldOut[ i ][ 1 ][ 3 ], pBoneToWorldOut[ i ][ 2 ][ 3 ] ), child_screen ) )
						continue;


					if ( !Math::WorldToScreen( Vector( pBoneToWorldOut[ parent ][ 0 ][ 3 ], pBoneToWorldOut[ parent ][ 1 ][ 3 ], pBoneToWorldOut[ parent ][ 2 ][ 3 ] ), parent_screen ) )
						continue;

					vguisurface->DrawSetColor( esp_ctx.clr );
					vguisurface->DrawLine( child_screen.x, child_screen.y, parent_screen.x, parent_screen.y );
				}
			}
		}

	}
	/*if ( studio_hdr && esp_ctx.is_visible )
	{
		matrix3x4_t matrix[ MAXSTUDIOBONES ];
		if ( esp_ctx.pl->SetupBones( matrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, global->curtime ) )
		{
			mstudiohitboxset_t* pHitboxSet = studio_hdr->GetHitboxSet( 0 );
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
						g_DebugOverlay->DrawPill2( min, max, pHitbox->m_flRadius, 255, 0, 128, 100, 0.2f );
					}
				}
			}
		}
	}*/
}


void Visuals::Misc::RenderCrosshair()
{
    int w, h;

    g_EngineClient->GetScreenSize(w, h);

    g_VGuiSurface->DrawSetColor(g_Options.color_esp_crosshair);

    int cx = w / 2;
    int cy = h / 2;

    g_VGuiSurface->DrawLine(cx - 25, cy, cx + 25, cy);
    g_VGuiSurface->DrawLine(cx, cy - 25, cx, cy + 25);
}
//--------------------------------------------------------------------------------
void Visuals::Misc::RenderWeapon(C_BaseCombatWeapon* ent)
{
    wchar_t buf[80];
    auto clean_item_name = [](const char* name) -> const char* {
        if(name[0] == 'C')
            name++;

        auto start = strstr(name, "Weapon");
        if(start != nullptr)
            name = start + 6;

        return name;
    };

    // We don't want to Render weapons that are being held
    if(ent->m_hOwnerEntity().IsValid())
        return;

    auto bbox = GetBBox(ent);

    if(bbox.right == 0 || bbox.bottom == 0)
        return;

    g_VGuiSurface->DrawSetColor(g_Options.color_esp_weapons);
    g_VGuiSurface->DrawLine(bbox.left, bbox.top, bbox.right, bbox.top);
    g_VGuiSurface->DrawLine(bbox.left, bbox.bottom, bbox.right, bbox.bottom);
    g_VGuiSurface->DrawLine(bbox.left, bbox.top, bbox.left, bbox.bottom);
    g_VGuiSurface->DrawLine(bbox.right, bbox.top, bbox.right, bbox.bottom);

    auto name = clean_item_name(ent->GetClientClass()->m_pNetworkName);

    if(MultiByteToWideChar(CP_UTF8, 0, name, -1, buf, 80) > 0) {
        int w = bbox.right - bbox.left;
        int tw, th;
        g_VGuiSurface->GetTextSize(esp_font, buf, tw, th);

        g_VGuiSurface->DrawSetTextFont(esp_font);
        g_VGuiSurface->DrawSetTextColor(g_Options.color_esp_weapons);
        g_VGuiSurface->DrawSetTextPos((bbox.left + w * 0.5f) - tw * 0.5f, bbox.bottom + 10);
        g_VGuiSurface->DrawPrintText(buf, wcslen(buf));
    }
}
//--------------------------------------------------------------------------------
void Visuals::Misc::RenderDefuseKit(C_BaseEntity* ent)
{
    if(ent->m_hOwnerEntity().IsValid())
        return;

    auto bbox = GetBBox(ent);

    if(bbox.right == 0 || bbox.bottom == 0)
        return;

    g_VGuiSurface->DrawSetColor(g_Options.color_esp_defuse);
    g_VGuiSurface->DrawLine(bbox.left, bbox.bottom, bbox.left, bbox.top);
    g_VGuiSurface->DrawLine(bbox.left, bbox.top, bbox.right, bbox.top);
    g_VGuiSurface->DrawLine(bbox.right, bbox.top, bbox.right, bbox.bottom);
    g_VGuiSurface->DrawLine(bbox.right, bbox.bottom, bbox.left, bbox.bottom);

    const wchar_t* buf = L"Defuse Kit";

    int w = bbox.right - bbox.left;
    int tw, th;
    g_VGuiSurface->GetTextSize(esp_font, buf, tw, th);

    g_VGuiSurface->DrawSetTextFont(esp_font);
    g_VGuiSurface->DrawSetTextColor(esp_ctx.clr);
    g_VGuiSurface->DrawSetTextPos((bbox.left + w * 0.5f) - tw * 0.5f, bbox.bottom + 1);
    g_VGuiSurface->DrawPrintText(buf, wcslen(buf));
}
//--------------------------------------------------------------------------------
void Visuals::Misc::RenderPlantedC4(C_BaseEntity* ent)
{
    auto bbox = GetBBox(ent);

    if(bbox.right == 0 || bbox.bottom == 0)
        return;

    g_VGuiSurface->DrawSetColor(g_Options.color_esp_c4);
    g_VGuiSurface->DrawLine(bbox.left, bbox.bottom, bbox.left, bbox.top);
    g_VGuiSurface->DrawLine(bbox.left, bbox.top, bbox.right, bbox.top);
    g_VGuiSurface->DrawLine(bbox.right, bbox.top, bbox.right, bbox.bottom);
    g_VGuiSurface->DrawLine(bbox.right, bbox.bottom, bbox.left, bbox.bottom);

    const wchar_t* buf = L"Bomb";

    int w = bbox.right - bbox.left;
    int tw, th;
    g_VGuiSurface->GetTextSize(esp_font, buf, tw, th);

    g_VGuiSurface->DrawSetTextFont(esp_font);
    g_VGuiSurface->DrawSetTextColor(esp_ctx.clr);
    g_VGuiSurface->DrawSetTextPos((bbox.left + w * 0.5f) - tw * 0.5f, bbox.bottom + 1);
    g_VGuiSurface->DrawPrintText(buf, wcslen(buf));
}
