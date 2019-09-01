#pragma once
#include "freecam.h"


 void freecam::create_move()
{
	 if (!options.misc_freecam || !options.misc_enabled) return;

	 g_Cmd->forwardmove = g_Cmd->sidemove = g_Cmd->upmove = g_Cmd->buttons = 0;
}

void freecam::override_view(CViewSetup* vsview)
{
	if (!options.misc_enabled) return;

	static Vector last_origin;

	if (!options.misc_freecam) last_origin = vsview->origin;

	if (options.misc_freecam)
	{
		static const unsigned int move_speed = 2.1f; //TODO: add custom moving speeds

		if (KEY_DOWN(VK_W))
			last_origin += vsview->angles.Forward() * move_speed;

		if (KEY_DOWN(VK_A))
			last_origin -= vsview->angles.Right() * move_speed;

		if (KEY_DOWN(VK_D))
			last_origin += vsview->angles.Right() * move_speed;

		if (KEY_DOWN(VK_S))
			last_origin -= vsview->angles.Forward() * move_speed;

		if (KEY_DOWN(VK_SPACE))
			last_origin += vsview->angles.Up() * move_speed;

		if (KEY_DOWN(VK_SHIFT))
			last_origin -= vsview->angles.Up() * move_speed;

		vsview->origin = last_origin;
	}
}