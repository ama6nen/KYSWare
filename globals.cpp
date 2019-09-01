#include "Globals.h"
namespace Hooks 
{
	 int width, height;
	 vgui::HFont buildFont;
	 vgui::HFont logoFont;
	 vgui::HFont VerdanaBig;
	 Color rainbowCol;
	 bool drawghost;
	 bool SetNewColor;
	 char clientname[128];
	 char clientname_other[128];
	 char clientpic[18] = "76561198316747508";
	 float fakeyaw = 0.f;
	 float realyaw = 0.f;
	 Vector lastmeme;
	 float viewyaw = 0.f;
	 bool autoaaleft = false;
	 bool inputlisten;
	 ButtonCode_t* input_receivedKeyval;
	 baimshit baimShots;
	 Vector fakeanglesOrigin = Vector(0,0,0);
	 bool doubleclicked;
}
CUserCmd* g_Cmd = nullptr;
C_BaseCombatWeapon* g_Weapon = nullptr;
C_BasePlayer* g_Local = nullptr;
Vector g_EyePos;
QAngle g_ViewAngles;
HMODULE g_EngineModule;
HMODULE g_ClientModule;