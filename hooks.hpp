#pragma once
#include "valve_sdk/csgostructs.hpp"
#include "helpers/vfunc_hook.hpp"
#include <d3d9.h>
#include "Drawing.h"
#include "menu.hpp"
#include "options.hpp"
#include "helpers/input.hpp"
#include "helpers/utils.hpp"
#include "features/bhop.hpp"
#include "features/chams.hpp"
#include "features/visuals.hpp"
#include "helpers\math.hpp"
#include "features/glow.hpp"
#include "Globals.h"
#include "cvar_cheats.h"
#include "rcs.h"
#include "freecam.h"
#include <time.h>
#include "events.h"
#include "resolver.h"
#include "hitmarker.h"
#include "prediction.h"
#include "autowall.h"
#include "dlights.h"
#include "flashlight.h"
#include <deque>
#include "SpoofedConvar.h"
#include "fake_latency.h"
#include "thirdperson.h"
#include "smoke.h"
#include "hotkey.h"
#include "proto_cheats.h"
#pragma once

namespace index
{
    constexpr int EndScene                 = 42;
    constexpr int Reset                    = 16;
    constexpr int PaintTraverse            = 41;
    constexpr int CreateMove               = 21;
    constexpr int playSound                = 82;
    constexpr int FrameStageNotify         = 36;
    constexpr int DrawModelExecute         = 21;
    constexpr int DoPostScreenSpaceEffects = 44;
	constexpr int OverrideView = 18;
	constexpr int GetPlayerInfo = 8;
	constexpr int FindModel = 10;
	constexpr int OverrideConfig = 21;
	constexpr int IsPlayingTimeDemo = 84;
	constexpr int IsPlayingDemo = 82;
	constexpr int DrawLights = 21;
	constexpr int CAM_FP = 36;
	constexpr int IsConnected = 27;
	constexpr int SceneEnd = 9;
	constexpr int shouldDrawCrosshair = 28;
	constexpr int SetKeyCodeState = 91;
	constexpr int onScreenSizeChanged = 116;
}

namespace Hooks
{
	struct EconomyItemCfg {
		int nFallbackPaintKit = 0;
		int nFallbackSeed = 0;
		int nFallbackStatTrak = -1;
		int iEntityQuality = 4;
		float flFallbackWear = 0.1f;
	};
	void Initialize();
	void Shutdown();

	extern vfunc_hook hlclient_hook;
	extern vfunc_hook direct3d_hook;
	extern vfunc_hook vguipanel_hook;
	extern vfunc_hook vguisurf_hook;
	extern vfunc_hook mdlrender_hook;
	extern vfunc_hook engine_hook;

	using EndScene = long(__stdcall *)(IDirect3DDevice9*);
	using Reset = long(__stdcall *)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
	using CreateMove = void(__thiscall*)(IBaseClientDLL*, int, float, bool);
	using PaintTraverse = void(__thiscall*)(IPanel*, vgui::VPANEL, bool, bool);
	using FrameStageNotify = void(__thiscall*)(IBaseClientDLL*, ClientFrameStage_t);
	using playSoundFn = void(__thiscall*)(ISurface*, const char* name);
	using FindModel = MDLHandle_t(__fastcall*)(void*, void*, const char* name);
	using CAM_ToFirstPerson_fn = void(__thiscall*)(void*);
	using ShouldDrawCrosshair =  bool(__thiscall *)(void *pThis);
	using DrawModelExecute = void(__thiscall*)(IVModelRender*, IMatRenderContext*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4_t*);
	using FireEvent = bool(__thiscall*)(IGameEventManager2*, IGameEvent* pEvent);
	using DoPostScreenEffects = int(__thiscall*)(IClientMode*, int);
	using OverrideView = void(__stdcall *)(CViewSetup*);
	using GetPlayerInfo = bool(__fastcall*)(void*, void*, int, player_info_t*);
	using OverrideConfig = bool(__fastcall*)(void* ecx, void* edx, MaterialSystem_Config_t*, bool);
	using IsPlayingTimeDemo = bool(__thiscall*)(void*);
	using DrawLights = void(__thiscall*)(void*);
	using SceneEnd = void(__thiscall*)(void*);
	using IsConnected = bool(__thiscall*)(void*);
	using SendDatagramFn = int(__thiscall*)(void*, void *);
	using RetrieveMessageFn = EGCResults( __thiscall* )( void*, uint32_t*, void *, uint32_t, uint32_t* );
	using SendMessageFn = EGCResults( __thiscall* )( void*, uint32_t, void *, uint32_t );
	using renderview_fn = void( __thiscall* )( void*, CViewSetup&,  CViewSetup&, int, int );

	long __stdcall hkEndScene(IDirect3DDevice9* device);
	long __stdcall hkReset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters);
	void __stdcall hook_createmove(int sequence_number, float input_sample_frametime, bool active, bool& bSendPacket);
	void __stdcall hook_createmove_proxy(int sequence_number, float input_sample_frametime, bool active);
	void __stdcall hkPaintTraverse(vgui::VPANEL panel, bool forceRepaint, bool allowForce);
	MDLHandle_t __fastcall hkFindModel(void* ecx, void* edx, const char* ptr);
	void __stdcall hkPlaySound(const char* name);
	void __stdcall hkDrawModelExecute(IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld);
	void __stdcall hkFrameStageNotify(ClientFrameStage_t stage);
	int  __stdcall hkDoPostScreenEffects(int a1);
	void __stdcall hkOverrideView(CViewSetup* vsView);
	bool __fastcall hkGetPlayerInfo(void* ecx, void* edx, int ent_num, player_info_t *pinfo);
	bool __fastcall hkOverrideConfig(void* ecx, void* edx, MaterialSystem_Config_t* config, bool force);
	bool __fastcall hkIsPlayingTimeDemo(void* thisptr, void* edx);
	void __fastcall hkDrawLights(void* thisptr, void* edx);
	void __fastcall hkCAM_FP(void* thisptr, void*);
	void __fastcall hkSceneEnd(void *pEcx, void *pEdx);
	bool __fastcall hkIsConnected(void* ecx, void* edx);
	int  __fastcall SendDatagram(void* netchan, void*, void *datagram);
}