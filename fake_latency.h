#pragma once
#include "singleton.hpp"
#include <deque>
#include "Globals.h"
class fake_latency : public Singleton<fake_latency>
{
private:
	friend class Singleton<fake_latency>;

	struct incoming_sequence
	{
		incoming_sequence::incoming_sequence(int instate, int outstate, int seqnr, float time)
		{
			inreliablestate = instate;
			outreliablestate = outstate;
			sequencenr = seqnr;
			curtime = time;
		}
		int inreliablestate;
		int outreliablestate;
		int sequencenr;
		float curtime;
	};

	std::deque<incoming_sequence>sequences;
	int lastincomingsequencenumber;




	void AddLatencyToNetchan(INetChannel *netchan, float Latency);

public:

	 void UpdateIncomingSequences();
	 int instate;
	 int insequencenr;
	 void run(INetChannel* net);
};