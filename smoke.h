#pragma once
#include "singleton.hpp"
#include "Globals.h"
#include "helpers\math.hpp"
#include "helpers\utils.hpp"

class smoke : public Singleton<smoke>
{
private:
	friend class Singleton<smoke>;


public:
	void on_framestagenotify();
};