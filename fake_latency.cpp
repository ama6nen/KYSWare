#include "fake_latency.h"
void fake_latency::UpdateIncomingSequences()
{

	INetChannel *netchan = (INetChannel*)g_ClientState->m_NetChannel;
	if (netchan)
	{
		if (netchan->m_nInSequenceNr > lastincomingsequencenumber)
		{
			lastincomingsequencenumber = netchan->m_nInSequenceNr;

			sequences.push_front(incoming_sequence(netchan->m_nInReliableState, netchan->m_nOutReliableState, netchan->m_nInSequenceNr, globals->realtime));
		}

		if (sequences.size() > 2048)
			sequences.pop_back();
	}

}

void fake_latency::run(INetChannel* netchannel) 
{
	instate = netchannel->m_nInReliableState;
	insequencenr = netchannel->m_nInSequenceNr;

	float rate = ((options.fake_latency_amount + 27.f) / 1000.f);
	AddLatencyToNetchan(netchannel, rate);
}
void fake_latency::AddLatencyToNetchan(INetChannel *netchan, float Latency)
{
	for (auto& seq : sequences)
	{
		if (globals->realtime - seq.curtime >= Latency)
		{
			netchan->m_nInReliableState = seq.inreliablestate;
			netchan->m_nInSequenceNr = seq.sequencenr;
			break;
		}
	}
}
