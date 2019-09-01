#include "bhop.hpp"


void bhop::on_createmove()
{

  static bool jumped_last_tick = false;
  static bool should_fake_jump = false;

  if(!jumped_last_tick && should_fake_jump)
  {
    should_fake_jump = false;
    g_Cmd->buttons |= IN_JUMP;
  } 
  else if(g_Cmd->buttons & IN_JUMP)
  {
    if(g_Local->m_fFlags() & FL_ONGROUND) 
	{
      jumped_last_tick = true;
      should_fake_jump = true;
    } 
	else 
	{
	  g_Cmd->buttons &= ~IN_JUMP;
      jumped_last_tick = false;
    }
  } 
  else 
  {
    jumped_last_tick = false;
    should_fake_jump = false;
  }

}


void autostrafer::on_createmove()
{
	static bool leftRight;
	bool inMove = g_Cmd->buttons & IN_FORWARD || g_Cmd->buttons & IN_BACK || g_Cmd->buttons & IN_MOVELEFT || g_Cmd->buttons & IN_MOVERIGHT;

	if (g_Cmd->buttons & IN_FORWARD && g_Local->m_vecVelocity().Length() <= 50.0f)
		g_Cmd->forwardmove = 450.0f;

	float yaw_change = 0.0f;
	if (g_Local->m_vecVelocity().Length() > 50.f)
		yaw_change = 30.0f * fabsf(30.0f / g_Local->m_vecVelocity().Length());



	if (g_Cmd->buttons & IN_ATTACK)
		yaw_change = 0.0f;

	QAngle viewAngles;
	g_EngineClient->GetViewAngles(viewAngles);


	if (!(g_Local->m_fFlags() & (int)FL_ONGROUND) && !inMove)
	{
		if (leftRight || g_Cmd->mousedx > 1)
		{
			viewAngles.yaw += yaw_change;
			g_Cmd->sidemove = 450.0f;
		}
		else if (!leftRight || g_Cmd->mousedx < 1)
		{
			viewAngles.yaw -= yaw_change;
			g_Cmd->sidemove = -450.0f;
		}

		leftRight = !leftRight;
	}

	Utils::Clamp(viewAngles);
	Utils::CorrectMovement(viewAngles);
}