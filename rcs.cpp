
#include "RCS.h"
#include "helpers\utils.hpp"
#pragma once

void rcs::run()
{
	static float oldpitch;
	QAngle punch = g_Local->m_aimPunchAngle();

	if (punch.LengthSqr() > 0.f && punch.LengthSqr() < 150.f)
	{

		QAngle angles = g_Cmd->viewangles;
		angles -= punch * 2;

		if (!Utils::Clamp(angles))
			Utils::ConsolePrint("failed to clamp\n");
		else
		{
			if (oldpitch > 78.f && angles.pitch > -90.f && angles.pitch < -70.f)
			{
				angles.pitch = oldpitch;
			}

			oldpitch = g_Cmd->viewangles.pitch;
			g_Cmd->viewangles = angles;

		}
	}
}


	

