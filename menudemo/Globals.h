#pragma once
#include "vector.h"
#include "color.h"
#include <d3dx9tex.h>
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

#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)
#define KEY_PRESSED(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x01) ? 1 : 0)
#define KEY_TEST(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8001) ? 1 : 0)
#define ws(x) std::wstring(L##x)
	extern int width, height;
	extern bool drawghost;
	extern char clientname[ 128 ];
	extern char clientname_other[ 128 ];
	extern char clientpic[ 18 ];
	extern bool SetNewColor;
	extern Color rainbowCol;
	extern float fakeyaw;
	extern float realyaw;
	extern float viewyaw;
	extern bool autoaaleft;
	extern bool inputlisten;
	extern int* input_receivedKeyval;
	extern Vector lastmeme;
	extern  IDirect3DTexture9* byteImage;
	extern Vector fakeanglesOrigin;
}