#include "helpers\utils.hpp"
#pragma once


enum ItemQuality
{
	ITEM_QUALITY_DEFAULT,
	ITEM_QUALITY_GENUINE,
	ITEM_QUALITY_VINTAGE,
	ITEM_QUALITY_UNUSUAL,
	ITEM_QUALITY_UNK,
	ITEM_QUALITY_COMMUNITY,
	ITEM_QUALITY_DEVELOPER,
	ITEM_QUALITY_SELFMADE,
	ITEM_QUALITY_CUSTOMIZED,
	ITEM_QUALITY_STRANGE,
	ITEM_QUALITY_COMPLETED,
	ITEM_QUALITY_UNK2,
	ITEM_QUALITY_TOURNAMENT
};

enum ItemRarity
{
	ITEM_RARITY_DEFAULT,
	ITEM_RARITY_COMMON,
	ITEM_RARITY_UNCOMMON,
	ITEM_RARITY_RARE,
	ITEM_RARITY_MYTHICAL,
	ITEM_RARITY_LEGENDARY,
	ITEM_RARITY_ANCIENT,
	ITEM_RARITY_IMMORTAL
};

class CEconItem
{
	unsigned short* GetEconItemData( );
	void UpdateEquippedState( unsigned int state );
public:
	uint32_t * GetInventory( );
	uint32_t * GetAccountID( );
	uint16_t* GetDefIndex( );
	uint64_t* GetItemID( );
	uint64_t* GetOriginalID( );
	unsigned char* GetFlags( );
	void SetQuality( ItemQuality quality );
	void SetRarity( ItemRarity rarity );
	void SetOrigin( int origin );
	void SetLevel( int level );
	void SetInUse( bool in_use );
	void SetCustomName( const char* name );
	void SetCustomDesc( const char* name );
	void SetPaintSeed( float seed );
	void SetPaintKit( float kit );
	void SetPaintWear( float wear );
	void SetStatTrak( int val );
	void AddSticker( int index, int kit, float wear, float scale, float rotation );
	void EquipTT( );
	void EquipCT( );
	void EquipAll( );

	template<typename TYPE>
	void SetAttributeValue( int index, TYPE val )
	{
		auto v15 = (DWORD*)Utils::PatternScan( LI_FIND_CACHED( GetModuleHandleA )( "client.dll" ), "A1 ? ? ? ? 85 C0 75 53" );
		auto v16 = *(DWORD *)( v15[ 72 ] + 4 * index );

		static auto fnSetDynamicAttributeValue
			= reinterpret_cast<int( __thiscall* )( CEconItem*, DWORD, void* )>(
				Utils::PatternScan( LI_FIND_CACHED( GetModuleHandleA )( "client.dll"), "55 8B EC 83 E4 F8 83 EC 3C 53 8B 5D 08 56 57 6A 00" )
				);

		fnSetDynamicAttributeValue( this, v16, &val );
	}
};

//CEconItem* CreateEconItem( )
//{
//	static auto fnCreateSharedObjectSubclass_EconItem_
//		= reinterpret_cast<CEconItem*( __stdcall* )( )>(
//			*reinterpret_cast<uintptr_t*>( Utils::PatternScan( GetModuleHandleA("client.dll"), "C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? E8 ? ? ? ? 83 F8 FF 75 09 8D 45 E4 50 E8 ? ? ? ? 8D 45 E4 C7 45 ? ? ? ? ? 50 C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? E8 ? ? ? ? 83 F8 FF 75 09 8D 45 E4 50 E8 ? ? ? ? 8D 45 E4 C7 45 ? ? ? ? ? 50 C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? E8 ? ? ? ? 83 F8 FF 75 09 8D 45 E4 50 E8 ? ? ? ? 8D 45 E4 C7 45 ? ? ? ? ? 50 C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? E8 ? ? ? ? 83 F8 FF 75 09 8D 45 E4 50 E8 ? ? ? ? 8D 45 E4" ) + 3 )
//			);
//
//	return fnCreateSharedObjectSubclass_EconItem_( );
//}

//class CPlayerInventory
//{
//	void CPlayerInventory::RemoveItem( uint64_t ID )
//	{
//		static auto fnRemoveItem
//			= reinterpret_cast<int( __thiscall* )( void*, int64_t )>(
//				Utils::PatternScan( GetModuleHandleA( "client.dll" ), "55 8B EC 83 E4 F8 56 57 FF 75 0C 8B F1" )
//				);
//
//		fnRemoveItem( this, ID );
//	}
//
//	void CPlayerInventory::RemoveItem( CEconItem* item )
//	{
//		RemoveItem( *item->GetItemID( ) );
//		GetBaseTypeCache( )->RemoveObject( item );
//	}
//
//	void CPlayerInventory::ClearInventory( )
//	{
//		auto BaseTypeCache = this->GetBaseTypeCache( );
//		auto items = BaseTypeCache->GetEconItems( );
//		for ( auto item : items )
//		{
//			RemoveItem( *item->GetItemID( ) );
//			BaseTypeCache->RemoveObject( item );
//		}
//	}
//
//	CSharedObjectTypeCache* CPlayerInventory::GetBaseTypeCache( )
//	{
//		static auto fnGCSDK_CGCClient_FindSOCache
//			= reinterpret_cast<uintptr_t( __thiscall* )( uintptr_t, uint64_t, uint64_t, bool )>(
//				Utils::PatternScan( GetModuleHandleA( "client.dll" ), "55 8B EC 83 E4 F8 83 EC 1C 0F 10 45 08" )
//				);
//
//		static auto fnGCSDK_CSharedObjectCache_FindBaseTypeCache
//			= reinterpret_cast<CSharedObjectTypeCache**( __fastcall* )( uintptr_t, uintptr_t, int* )>(
//				Utils::PatternScan( GetModuleHandleA( "client.dll" ), "55 8B EC 56 57 8B F2 8B F9" )
//				);
//
//		auto SOCahce = fnGCSDK_CGCClient_FindSOCache( g_CHLClient + 0x60, *reinterpret_cast<uint64_t*>( this + 0x8 ), *reinterpret_cast<uint64_t*>( this + 0x10 ), 0 );
//
//		auto unk1 = *(DWORD*)( SOCahce + 0x1C );
//		auto unk2 = *(DWORD*)( SOCahce + 0x10 );
//		auto type = 1;
//
//		__asm push this;
//		auto ret = fnGCSDK_CSharedObjectCache_FindBaseTypeCache( unk2, unk2 + 4 * unk1, &type );
//		__asm add esp, 8;
//		return *ret;
//	}
//
//	uint32_t CPlayerInventory::GetSteamID( )
//	{
//		return *reinterpret_cast<uint32_t*>( this + 0x8 );
//	}
//
//	CUtlVector< C_EconItemView* >* CPlayerInventory::GetInventoryItems( )
//	{
//		return reinterpret_cast<CUtlVector<C_EconItemView*>*>( this + 0x2C );
//	}
//
//	bool CPlayerInventory::AddEconItem( CEconItem* item, int a3, int a4, char a5 )
//	{
//		static auto fnAddEconItem
//			= reinterpret_cast<C_EconItemView*( __thiscall* )( void*, CEconItem*, int, int, char )>(
//				Utils::PatternScan( GetModuleHandleA( "client.dll" ), "55 8B EC 83 E4 F8 A1 ? ? ? ? 83 EC 14 53 56 57 8B F9 8B 08" )
//				);
//
//		GetBaseTypeCache( )->AddObject( item );
//
//		return fnAddEconItem( this, item, a3, a4, a5 );
//	}
//};