#pragma once
#include "singleton.hpp"
#include "Globals.h"
#include "helpers\math.hpp"
#include "helpers\utils.hpp"
#include "hotkey.h"

class thirdperson : public Singleton<thirdperson>
{
private:
	friend class Singleton<thirdperson>;


public:
	void on_overrideview();
	void toggle();
	void reset();
	void on_framestagenotify(C_BasePlayer* plocal);
};