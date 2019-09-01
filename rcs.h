#pragma once
#include "valve_sdk\csgostructs.hpp"
class rcs : public Singleton<rcs>
{
private:
	friend class Singleton<rcs>;
public:
	 void run();
};