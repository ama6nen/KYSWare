#pragma once
#include "valve_sdk\sdk.hpp"

class freecam : public Singleton<freecam>
{
private:
	friend class Singleton<freecam>;

public:
	 void create_move();
	 void override_view(CViewSetup* vsview);
};
