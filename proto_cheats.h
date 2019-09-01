#pragma once
#include "singleton.hpp"
#include "Globals.h"
#include "helpers\math.hpp"
#include "helpers\utils.hpp"
#include "proto/base_gcmessages.pb.h"
#include "proto/cstrike15_gcmessages.pb.h"
//#include "proto/econ_gcmessages.pb.h"
//#include "proto/engine_gcmessages.pb.h"
#include "proto/gcsystemmsgs.pb.h"
#include "proto/gcsdk_gcmessages.pb.h"
//#include "proto/netmessages.pb.h"
//#include "proto/steammessages.pb.h"



class proto : public Singleton<proto>
{
private:
	friend class Singleton<proto>;
public:
	void ApplyMedals( CMsgSOCacheSubscribed::SubscribedType* pInventoryCacheObject );
	void AddItem( CMsgSOCacheSubscribed::SubscribedType* Object, int index, int itemDefIndex, int rarity, bool in_use , int paintKit, int seed, float wear, std::string name );
	void retrieve( uint32_t* punMsgType, void* pubDest, uint32_t cubDest, uint32_t* pcubMsgSize );
	void send( uint32_t &unMsgType, void* pubData, uint32_t &cubData );
	bool SendClientHello( );
	void SendMatchmakingHelloToServer( );
};