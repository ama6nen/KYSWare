#include "thirdperson.h"
void thirdperson::on_framestagenotify(C_BasePlayer* plocal)
{
	if (!options.misc_enabled || !options.misc_thirdperson_enabled || !options.misc_thirdperson_enabled2 || !engine->IsInGame() || !plocal)
		return;

	if(*(bool*)((DWORD)g_Input + 0xA5) && Hooks::lastmeme.IsValid())
		*(Vector*)((uintptr_t)plocal + 0x31C8) = Hooks::lastmeme;
}

void thirdperson::reset()
{

	g_Input->m_fCameraInThirdPerson = false;

	QAngle vecAngles;
	engine->GetViewAngles(vecAngles);

	if (!options.misc_thirdperson_enabled2)
		g_Input->m_vecCameraOffset = Vector(vecAngles.pitch, vecAngles.yaw, 0.f);

	input->CamToFP();
}

void thirdperson::toggle() 
{
	options.misc_thirdperson_enabled2 = !options.misc_thirdperson_enabled2;

	if (!options.misc_thirdperson_enabled2)
		this->reset();

	g_ChatElement->toggle_feature(options.misc_thirdperson_enabled2, "thirdperson");
}
void thirdperson::on_overrideview()
{

	if (!options.misc_thirdperson_enabled)
		return;

	switch (options.misc_thirdperson_type)
	{
	case 0:
		options.misc_thirdperson_enabled2 = options.misc_thirdperson_enabled;
		break;

	case 1:
	{
		static bool wasDown = false;
		bool keyDown = keyboard::get().key_down(options.thirdperson_button);
		options.misc_thirdperson_enabled2 = keyDown;
		if (wasDown && !keyDown)
			input->CamToFP();

		wasDown = keyDown;
		break;
	}

	case 2:
		if (keyboard::get().key_released(options.thirdperson_button))
			this->toggle();
		break;

		case 3:
		{
			static bool wasDown = false;
			bool keyDown = keyboard::get().key_down(options.thirdperson_button);
			options.misc_thirdperson_enabled2 = !keyDown;
			if (!wasDown && keyDown)
				input->CamToFP();

			wasDown = keyDown;
			break;
		}
	}

	if (!options.misc_enabled || !options.misc_thirdperson_enabled2 || !engine->IsInGame() || !g_Local)
		return;

	if (!g_Input->m_fCameraInThirdPerson)
		*(int*)((DWORD)g_Input + 0xFC) = 0; 

	QAngle vecAngles;
	engine->GetViewAngles(vecAngles);

	g_Input->m_fCameraInThirdPerson = true;

	g_Input->m_vecCameraOffset = Vector(vecAngles.pitch, vecAngles.yaw, options.misc_thirdperson_dist);

	Vector camForward = Vector(g_Input->m_vecCameraOffset.x, g_Input->m_vecCameraOffset.y, 0).Forward();
	Vector origin = g_Local->GetEyePos();

	static constexpr auto CAM_HULL_OFFSET = 14.0f;
	static const Vector CAM_HULL_MIN(-CAM_HULL_OFFSET, -CAM_HULL_OFFSET, -CAM_HULL_OFFSET);
	static const Vector CAM_HULL_MAX(CAM_HULL_OFFSET, CAM_HULL_OFFSET, CAM_HULL_OFFSET);

	trace_t trace;
	CTraceFilter traceFilter;
	traceFilter.pSkip = g_Local;

	Utils::Tracehull(origin, origin - (camForward * g_Input->m_vecCameraOffset.z), CAM_HULL_MIN, CAM_HULL_MAX, MASK_SOLID, &traceFilter, trace);

	if (trace.fraction < 1.0f)
		g_Input->m_vecCameraOffset.z *= trace.fraction;
}