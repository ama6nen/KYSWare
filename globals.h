#pragma once

#include "valve_sdk\sdk.hpp"
#include "options.hpp"


namespace Hooks
{
	struct baimshit 
	{
		int entnum;
		int shots;
		int old_shots;
		int curr_shots;
	};
	#define engine g_EngineClient
	#define clientmode g_ClientMode
	#define local g_LocalPlayer
	#define global g_GlobalVars
	#define globals g_GlobalVars
	#define entlist g_EntityList
	#define entitylist g_EntityList
	#define cvars g_CVar
	#define etrace g_EngineTrace
	#define enginetrace g_EngineTrace
	#define renderview g_RenderView
	#define mdlrender g_MdlRender
	#define matsystem g_MatSystem
	#define materialsystem g_MatSystem
	#define input	g_Input
	#define vguisurface g_VGuiSurface
	#define vsurface g_VGuiSurface
	#define options g_Options
	
	#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)
	#define KEY_PRESSED(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x01) ? 1 : 0)
	#define KEY_TEST(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8001) ? 1 : 0)
	#define ws(x) std::wstring(L##x)
	extern int width, height;
	extern vgui::HFont buildFont;
	extern vgui::HFont logoFont;
	extern vgui::HFont VerdanaBig;
	extern bool drawghost;
	extern char clientname[128];
	extern char clientname_other[128];
	extern char clientpic[18];
	extern bool SetNewColor;
	extern Color rainbowCol;
	extern float fakeyaw ;
	extern float realyaw ;
	extern float viewyaw;
	extern bool autoaaleft;
	extern bool inputlisten;
	extern ButtonCode_t* input_receivedKeyval;
	extern Vector lastmeme;
	extern baimshit baimShots;
	extern Vector fakeanglesOrigin;
	extern bool doubleclicked;
}
class C_BaseCombatWeapon; //gay fix
class C_BasePlayer;
class CUserCmd;

extern CUserCmd* g_Cmd;
extern C_BaseCombatWeapon* g_Weapon;
extern C_BasePlayer* g_Local;
extern Vector g_EyePos;
extern QAngle g_ViewAngles;
extern HMODULE g_EngineModule;
extern HMODULE g_ClientModule;