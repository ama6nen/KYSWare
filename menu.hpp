#pragma once

#include <string>
#include "singleton.hpp"
#define NOMINMAX
#include "imgui/imgui.h"
#include <Windows.h>
#include <chrono>
#include <algorithm>
#include "valve_sdk/csgostructs.hpp"
#include "helpers/input.hpp"
#include "options.hpp"
#include "Globals.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui_internal.h"
#include "imgui/directx9/imgui_impl_dx9.h"
#include "helpers\utils.hpp"
#include "sky_changer.h"
#include "proto_cheats.h"

struct IDirect3DDevice9;
class Menu
    : public Singleton<Menu>
{
public:

	
    void Initialize();
    void Shutdown();

    void OnDeviceLost();
    void OnDeviceReset();

    void Render();

    void Show();
    void Hide();
    void Toggle();

	bool selecting_key;
    bool IsVisible() const { return _visible; }
	void			 ChangeStyle(ImGuiCol set, Color color);
	Color			 GetColorOfStyle(ImGuiCol get);
private:
    void CreateStyle();
	
	ImGuiStyle        _style;
    bool              _visible;
};