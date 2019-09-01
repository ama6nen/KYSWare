#include "dlights.h"
#include "Globals.h"
void dlights::run() 
{
	if (!g_Options.dlights && !g_Options.elights)
		return;

	auto pLocal = (C_BasePlayer*)g_LocalPlayer;

	Color rainbowcolor = Hooks::rainbowCol;
	if (pLocal && g_Options.dlights_local) 
	{
		int index = pLocal->EntIndex();

		if (g_Options.dlights)
		{
			dlight_t* pDlight = g_IVEffects->CL_AllocDlight(index);

			pDlight->key = index;
			pDlight->radius = g_Options.dlight_radius;

			if (g_Options.dlights_rainbow)
			{
				pDlight->color.r = (byte)rainbowcolor.r();
				pDlight->color.g = (byte)rainbowcolor.g();
				pDlight->color.b = (byte)rainbowcolor.b();
			}
			else
			{
				auto color = g_Options.color_dlight_local;

				pDlight->color.r = (byte)(color.r());
				pDlight->color.g = (byte)(color.g());
				pDlight->color.b = (byte)(color.b());
			}

			pDlight->m_Direction = pLocal->GetRenderOrigin();
			pDlight->origin = pLocal->GetRenderOrigin();
			pDlight->color.exponent = g_Options.dlight_exponent;
			pDlight->decay = g_Options.dlight_decay;
			pDlight->die = g_GlobalVars->curtime + g_Options.dlight_die;
		}


		if (g_Options.elights)
		{
			dlight_t* pDlight = g_IVEffects->CL_AllocElight(index);

			pDlight->key = index;
			pDlight->radius = g_Options.dlight_radius;

			if (g_Options.dlights_rainbow)
			{
				pDlight->color.r = (byte)rainbowcolor.r();
				pDlight->color.g = (byte)rainbowcolor.g();
				pDlight->color.b = (byte)rainbowcolor.b();
			}
			else
			{
				auto color = g_Options.color_dlight_local;

				pDlight->color.r = (byte)(color.r());
				pDlight->color.g = (byte)(color.g());
				pDlight->color.b = (byte)(color.b());
			}

			pDlight->m_Direction = pLocal->GetRenderOrigin();
			pDlight->origin = pLocal->GetRenderOrigin();
			pDlight->color.exponent = g_Options.dlight_exponent;
			pDlight->decay = g_Options.dlight_decay;
			pDlight->die = g_GlobalVars->curtime + g_Options.dlight_die;
		}
	}

	if (g_Options.dlights_enemy || g_Options.dlights_team)
	{
		for (int i = 0; i < g_EntityList->GetHighestEntityIndex(); i++) 
		{
			auto pEntity = static_cast<C_BasePlayer*>(g_EntityList->GetClientEntity(i));

			if (pEntity && pEntity != pLocal && pEntity->IsPlayer() && pEntity->IsAlive() && !pEntity->IsDormant())
			{
				bool enemy = pLocal->m_iTeamNum() != pEntity->m_iTeamNum();

				if (enemy && g_Options.dlights_enemy || !enemy && g_Options.dlights_team)
				{
					int index = pEntity->EntIndex();

					if (g_Options.dlights)
					{
						dlight_t* pDlight = g_IVEffects->CL_AllocDlight(index);

						pDlight->key = index;
						pDlight->radius = g_Options.dlight_radius;

						if (g_Options.dlights_rainbow)
						{
							pDlight->color.r = (byte)rainbowcolor.r();
							pDlight->color.g = (byte)rainbowcolor.g();
							pDlight->color.b = (byte)rainbowcolor.b();
						}
						else
						{
							auto color = enemy ? g_Options.color_dlight_enemy : g_Options.color_dlight_team;

							pDlight->color.r = (byte)(color.r());
							pDlight->color.g = (byte)(color.g());
							pDlight->color.b = (byte)(color.b());
						}

						pDlight->m_Direction = pEntity->GetRenderOrigin();
						pDlight->origin = pEntity->GetRenderOrigin();
						pDlight->color.exponent = g_Options.dlight_exponent;
						pDlight->decay = g_Options.dlight_decay;
						pDlight->die = g_GlobalVars->curtime + g_Options.dlight_die;
					}

					if (g_Options.elights)
					{
						dlight_t* pDlight = g_IVEffects->CL_AllocElight(index);

						pDlight->key = index;
						pDlight->radius = g_Options.dlight_radius;

						if (g_Options.dlights_rainbow)
						{
							pDlight->color.r = (byte)rainbowcolor.r();
							pDlight->color.g = (byte)rainbowcolor.g();
							pDlight->color.b = (byte)rainbowcolor.b();
						}
						else
						{
							auto color = enemy ? g_Options.color_dlight_enemy : g_Options.color_dlight_team;

							pDlight->color.r = (byte)(color.r());
							pDlight->color.g = (byte)(color.g());
							pDlight->color.b = (byte)(color.b());
						}

						pDlight->m_Direction = pEntity->GetRenderOrigin();
						pDlight->origin = pEntity->GetRenderOrigin();
						pDlight->color.exponent = g_Options.dlight_exponent;
						pDlight->decay = g_Options.dlight_decay;
						pDlight->die = g_GlobalVars->curtime + g_Options.dlight_die;
					}
				}
			}
		}
	}
}