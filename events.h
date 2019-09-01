#pragma once
#include "valve_sdk\sdk.hpp"
namespace events 
{
	class event_listener : public IGameEventListener2
	{
	public:
	
		void FireGameEvent(IGameEvent *pEvent);
	
		int GetEventDebugID(void);
	};
}