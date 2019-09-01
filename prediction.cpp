#include "prediction.h"


int iTickBaseBackup;
int iFlagsBackup;
int iButtonsBackup;
int* m_pPredictionRandomSeed;
int* m_pSetPredictionPlayer;
static char* cMoveData = nullptr;

CMoveData m_MoveData;

void pred::run()
{
	if (!g_MoveHelper) return;

	CMoveData C_MoveData;

	iFlagsBackup = g_Local->m_fFlags();
	iButtonsBackup = g_Cmd->buttons;

	g_Local->m_pCurrentCommand() = g_Cmd;

	if (!m_pPredictionRandomSeed || !m_pSetPredictionPlayer)
	{
		m_pPredictionRandomSeed = *reinterpret_cast<int**>(Utils::PatternScan( LI_FIND_CACHED( GetModuleHandleA )("client.dll"), "A3 ? ? ? ? 66 0F 6E 86") + 1);
		m_pSetPredictionPlayer = *reinterpret_cast<int**>(Utils::PatternScan( LI_FIND_CACHED( GetModuleHandleA )("client.dll"), "89 35 ? ? ? ? F3 0F 10 48 20") + 2);
		Utils::ConsolePrint("Acquiring prediction\n");
	}

	if (!cMoveData)
		cMoveData = (char*)(calloc(1, sizeof(CMoveData)));

	g_MoveHelper->SetHost(g_Local);
	*m_pPredictionRandomSeed = MD5_PseudoRandom(g_Cmd->command_number) & 0x7FFFFFFF;
	*m_pSetPredictionPlayer = uintptr_t(g_Local);

	g_GlobalVars->curtime = g_Local->m_nTickBase() * g_GlobalVars->interval_per_tick;
	g_GlobalVars->frametime = g_GlobalVars->interval_per_tick;

	g_Cmd->buttons |= *reinterpret_cast< uint8_t* >(uintptr_t(g_Local) + 0x3310);

	if (g_Cmd->impulse)
		*reinterpret_cast< uint8_t* >(uintptr_t(g_Local) + 0x31EC) = g_Cmd->impulse;

	C_MoveData.m_nButtons = g_Cmd->buttons;
	int buttonsChanged = g_Cmd->buttons ^ *reinterpret_cast<int*>(uintptr_t(g_Local) + 0x31E8);
	*reinterpret_cast<int*>(uintptr_t(g_Local) + 0x31DC) = (uintptr_t(g_Local) + 0x31E8);
	*reinterpret_cast<int*>(uintptr_t(g_Local) + 0x31E8) = g_Cmd->buttons;
	*reinterpret_cast<int*>(uintptr_t(g_Local) + 0x31E0) = g_Cmd->buttons & buttonsChanged;  
	*reinterpret_cast<int*>(uintptr_t(g_Local) + 0x31E4) = buttonsChanged & ~g_Cmd->buttons; 

	g_GameMovement->StartTrackPredictionErrors(g_Local);

	iTickBaseBackup = g_Local->m_nTickBase();

	g_Prediction->SetupMove(g_Local, g_Cmd, g_MoveHelper, reinterpret_cast< CMoveData* >(cMoveData));
	g_GameMovement->ProcessMovement(g_Local, reinterpret_cast< CMoveData* >(cMoveData));
	g_Prediction->FinishMove(g_Local, g_Cmd, reinterpret_cast< CMoveData* >(cMoveData));

	g_Local->m_nTickBase() = iTickBaseBackup;

	g_GameMovement->FinishTrackPredictionErrors(g_Local);

	g_Local->m_pCurrentCommand() = nullptr;
	*m_pPredictionRandomSeed = -1;
	*m_pSetPredictionPlayer = 0;
	g_MoveHelper->SetHost(0);

	g_Local->m_fFlags() = iFlagsBackup;
	g_Cmd->buttons = iButtonsBackup;
}
