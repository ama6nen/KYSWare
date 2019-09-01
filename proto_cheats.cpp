#include "proto_cheats.h"
std::vector<uint32_t> medals = { 930,931,948,958,6021,6020,6019,6012,6032,6027,6002 };


void proto::AddItem( CMsgSOCacheSubscribed::SubscribedType* Object, int index, int itemDefIndex, int rarity, bool in_use , int paintKit, int seed, float wear, std::string name )
{
	CSOEconItem Skin;

	Skin.set_id( 20000 + index );
	Skin.set_account_id( steamUser->GetSteamID( ).GetAccountID( ) );
	Skin.set_def_index( itemDefIndex );
	Skin.set_inventory( 20000 + index );
	Skin.set_origin( 24 );
	Skin.set_quantity( 1 );
	Skin.set_level( 1 );
	Skin.set_style( 0 );
	Skin.set_flags( 0 );
	Skin.set_in_use( in_use );
	Skin.set_original_id( 0 );
	Skin.set_rarity( rarity );
	Skin.set_quality( 0 );

	if ( name.size( ) > 0 )
		Skin.set_custom_name( name.data( ) );

	// Paint Kit
	auto PaintKitAttribute = Skin.add_attribute( );
	float PaintKitAttributeValue = (float)paintKit;

	PaintKitAttribute->set_def_index( 6 );

	PaintKitAttribute->set_value_bytes( &PaintKitAttributeValue, 4 );

	// Paint Seed
	auto SeedAttribute = Skin.add_attribute( );
	float SeedAttributeValue = (float)seed;

	SeedAttribute->set_def_index( 7 );

	SeedAttribute->set_value_bytes( &SeedAttributeValue, 4 );

	// Paint Wear
	auto WearAttribute = Skin.add_attribute( );
	float WearAttributeValue = wear;

	WearAttribute->set_def_index( 8 );

	WearAttribute->set_value_bytes( &WearAttributeValue, 4 );

	// Stickers
	for ( int j = 0; j < 4; j++ )
	{
		// Sticker Kit
		CSOEconItemAttribute* StickerKitAttribute = Skin.add_attribute( );
		uint32_t StickerKitAttributeValue = 0;

		StickerKitAttribute->set_def_index( 113 + 4 * j );

		StickerKitAttribute->set_value_bytes( &StickerKitAttributeValue, 4 );

		// Sticker Wear
		CSOEconItemAttribute* StickerWearAttribute = Skin.add_attribute( );
		float StickerWearAttributeValue = 0.001f;

		StickerWearAttribute->set_def_index( 114 + 4 * j );

		StickerWearAttribute->set_value_bytes( &StickerWearAttributeValue, 4 );

		// Sticker Scale
		CSOEconItemAttribute* StickerScaleAttribute = Skin.add_attribute( );
		float StickerScaleAttributeValue = 1.f;

		StickerScaleAttribute->set_def_index( 115 + 4 * j );

		StickerScaleAttribute->set_value_bytes( &StickerScaleAttributeValue, 4 );

		// Sticker Rotation
		CSOEconItemAttribute* StickerRotationAttribute = Skin.add_attribute( );
		float StickerRotationAttributeValue = 0.f;

		StickerRotationAttribute->set_def_index( 116 + 4 * j );

		StickerRotationAttribute->set_value_bytes( &StickerRotationAttributeValue, 4 );
	}

	Object->add_object_data( Skin.SerializeAsString( ) );
}

void proto::ApplyMedals( CMsgSOCacheSubscribed::SubscribedType* pInventoryCacheObject )
{
	CSOEconItem Medal;

	Medal.set_account_id( steamUser->GetSteamID( ).GetAccountID( ) );
	Medal.set_origin( 9 );
	Medal.set_rarity( 6 );
	Medal.set_quantity( 1 );
	Medal.set_quality( 4 );
	Medal.set_level( 1 );

	CSOEconItemAttribute* TimeAcquiredAttribute = Medal.add_attribute( );
	uint32_t TimeAcquiredAttributeValue = 0;

	TimeAcquiredAttribute->set_def_index( 222 );

	TimeAcquiredAttribute->set_value_bytes( &TimeAcquiredAttributeValue, 4 );

	int i = 10000;
	for ( uint32_t MedalIndex : medals )
	{
		Medal.set_def_index( MedalIndex );
		Medal.set_inventory( i );
		Medal.set_id( i );

		pInventoryCacheObject->add_object_data( Medal.SerializeAsString( ) );

		i++;
	}
}

void proto::retrieve( uint32_t* punMsgType, void* pubDest, uint32_t cubDest, uint32_t* pcubMsgSize )
{
	uint32_t MessageType = *punMsgType & 0x7FFFFFFF;

	if ( MessageType == k_EMsgGCClientWelcome )
	{

		CMsgClientWelcome Message;
		try
		{
			if ( !Message.ParsePartialFromArray( (void*)( (DWORD)pubDest + 8 ), *pcubMsgSize - 8 ) )
				return;
		} catch ( ... )
		{
			return;
		}

		if ( Message.outofdate_subscribed_caches_size( ) <= 0 )
			return;

		CMsgSOCacheSubscribed* Cache = Message.mutable_outofdate_subscribed_caches( 0 );

		for ( int i = 0; i < Cache->objects_size( ); i++ )
		{
			CMsgSOCacheSubscribed::SubscribedType* Object = Cache->mutable_objects( i );

			if ( !Object->has_type_id( ) )
				continue;

			if ( Object->type_id( ) == 1 )
			{


				for ( int j = 0; j < Object->object_data_size( ); j++ )
				{
					std::string* ObjectData = Object->mutable_object_data( j );

					CSOEconItem Item;

					if ( !Item.ParseFromArray( ( void* )const_cast<char*>( ObjectData->data( ) ), ObjectData->size( ) ) )
						continue;

					if ( Item.equipped_state_size( ) <= 0 )
						continue;
				}
				
				AddItem( Object, 1, WEAPON_AWP, WEAPONRARITY_IMMORTAL, true, 320, 38, 0.001f, "" ); //279
				AddItem( Object, 2, WEAPON_AK47, WEAPONRARITY_ANCIENT,false, 474, 38, 0.001f, "" );
				AddItem( Object, 4, WEAPON_KNIFE_SURVIVAL_BOWIE, WEAPONRARITY_ANCIENT,false, 158, 38, 0.001f, "amen x 10 " ); //323
				AddItem( Object, 3, WEAPON_SCAR20, WEAPONRARITY_IMMORTAL,true, 597, 38, 0.001f, "christian" ); //323
				
				ApplyMedals( Object );
				Utils::ConsolePrint( "APPLY \n" );
			}
		}

		if ( (uint32_t)Message.ByteSize( ) <= cubDest - 8 )
		{
			Message.SerializeToArray( (void*)( (DWORD)pubDest + 8 ), Message.ByteSize( ) );

			*pcubMsgSize = Message.ByteSize( ) + 8;
		}
	} 
	else if ( MessageType == k_EMsgGCCStrike15_v2_MatchmakingGC2ClientHello )
	{
		CMsgGCCStrike15_v2_MatchmakingGC2ClientHello PlayerMessage;
		try
		{
			if ( !PlayerMessage.ParsePartialFromArray( (void*)( (DWORD)pubDest + 8 ), *pcubMsgSize - 8 ) )
				return;
		} catch ( ... )
		{
			return;
		}
		PlayerMessage.set_penalty_reason( options.misc_proto_ban_type ); // Ban id from list
		PlayerMessage.set_penalty_seconds( 60 * 60 ); // Time in seconds
		PlayerMessage.set_player_level( 69 );
		PlayerMessage.set_player_cur_xp( 100000 ); //PlayerXP
		PlayerMessage.set_player_xp_bonus_flags( 4 );

		PlayerRankingInfo* RankingInfo = PlayerMessage.mutable_ranking( );
		RankingInfo->set_account_id( steamUser->GetSteamID( ).GetAccountID( ) );
		RankingInfo->set_wins( 1337 ); // MM Win amount
		RankingInfo->set_rank_id( 18 ); // Rank 
	
		PlayerCommendationInfo* commendshit = PlayerMessage.mutable_commendation( );
		commendshit->set_cmd_leader( 100 );
		commendshit->set_cmd_teaching( 100 );
		commendshit->set_cmd_friendly( 100 );
		
		/*auto match = PlayerMessage.mutable_ongoingmatch( );
		match->set_map( "gay af" );
		match->set_serverid( 2 );
		match->set_server_address( "6.6.6.6" );
		match->set_reservationid( 0 );
*/
		

		if ( (uint32_t)PlayerMessage.ByteSize( ) <= cubDest - 8 )
		{
			PlayerMessage.SerializeToArray( (void*)( (DWORD)pubDest + 8 ), PlayerMessage.ByteSize( ) );
			*pcubMsgSize = PlayerMessage.ByteSize( ) + 8;
		}
	}
}

void proto::send( uint32_t &unMsgType, void* pubData, uint32_t &cubData )
{
	uint32_t MessageType = unMsgType & 0x7FFFFFFF;
	return;
}

bool proto::SendClientHello( )
{
	CMsgClientHello Message;

	Message.set_client_session_need( 1 );
	Message.clear_socache_have_versions( );

	void* ptr = malloc( Message.ByteSize( ) + 8 );

	if ( !ptr )
		return false;

	( (uint32_t*)ptr )[ 0 ] = k_EMsgGCClientHello | ( (DWORD)1 << 31 );
	( (uint32_t*)ptr )[ 1 ] = 0;

	Message.SerializeToArray( (void*)( (DWORD)ptr + 8 ), Message.ByteSize( ) );

	bool result = SteamGameCoordinator->SendMessage( k_EMsgGCClientHello | ( (DWORD)1 << 31 ), ptr, Message.ByteSize( ) + 8 ) == k_EGCResultOK;

	free( ptr );

	return result;
}

void proto::SendMatchmakingHelloToServer( )
{
	CMsgGCCStrike15_v2_MatchmakingClient2GCHello ServerMessage;
	void* pointer = malloc( ServerMessage.ByteSize( ) + 8 );
	if ( !pointer )
		return;
	auto unMsgType = k_EMsgGCCStrike15_v2_MatchmakingClient2GCHello | ( (DWORD)1 << 31 );
	( (uint32_t*)pointer )[ 0 ] = unMsgType;
	( (uint32_t*)pointer )[ 1 ] = 0;
	ServerMessage.SerializeToArray( (void*)( (DWORD)pointer + 8 ), ServerMessage.ByteSize( ) );
	bool result = SteamGameCoordinator->SendMessage( unMsgType, pointer, ServerMessage.ByteSize( ) + 8 ) == k_EGCResultOK; // you don't need to save it in a bool but when you want it xD
	free( pointer );
}