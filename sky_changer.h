#pragma once
#include "globals.h"
#include "helpers\utils.hpp"

class sky_changer : public Singleton<sky_changer>
{
private:
	friend class Singleton<sky_changer>;

public:
	void change(const char* name);
}; 
