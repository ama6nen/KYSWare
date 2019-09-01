#pragma once
#include <string>
#include "singleton.h"
#include "imgui/imgui.h"
#define NOMINMAX
#include <Windows.h>
#include <chrono>
#include <algorithm>
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui_internal.h"
#include "imgui/directx9/imgui_impl_dx9.h"
#include <d3d9.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <tchar.h>
#include <cstring>
#include "options.h"
#include "Globals.h"
#include <vector>
#include "input.h"
#include <iostream>
#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <stdexcept>
#include <atlstr.h>
#include <math.h>
#include <filesystem>
#include <stdio.h>
#include <stdlib.h>

namespace fs = std::experimental::filesystem;
using namespace std;
using namespace Hooks;
#define options g_Options
#pragma once

struct IDirect3DDevice9;

class Menu
	: public Singleton<Menu>
{
public:


	void Initialize( );
	void Shutdown( );

	void OnDeviceLost( );
	void OnDeviceReset( );

	void Render( );

	void Show( );
	void Hide( );
	void Toggle( );

	bool selecting_key;
	bool IsVisible( ) const
	{
		return _visible;
	}
	void			 ChangeStyle( ImGuiCol set, Color color );
	Color			 GetColorOfStyle( ImGuiCol get );

	void CreateStyle( );

	ImGuiStyle        _style;
private:
	bool              _visible;
};
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp;