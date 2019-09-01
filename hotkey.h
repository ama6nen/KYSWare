#pragma once
#include <Windows.h>
#include "globals.h"
#include "singleton.hpp"

class keyboard : public Singleton<keyboard>
{
private:
	friend class Singleton<keyboard>;
public:

	bool key_down(ButtonCode_t code)
	{
		return g_InputSystem->IsButtonDown(code);
	}

	bool key_released(ButtonCode_t code)
	{
		if (code <= BUTTON_CODE_NONE)
			return false;

		static bool states[256];

		if (key_down(code)) //Holding down
			states[code] = true;
		else if (states[code] && !key_down(code) )//Not holding but was holding so key is released
		{
			states[code] = false;
			return true;
		}

		return false;
	}
};