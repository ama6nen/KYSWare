#ifndef ISTEAMINVENTORY_H
#define ISTEAMINVENTORY_H
#ifdef _WIN32
#pragma once
#endif
#include "isteamclient.hpp"
#if defined( VALVE_CALLBACK_PACK_SMALL )
#pragma pack( push, 4 )
#elif defined( VALVE_CALLBACK_PACK_LARGE )
#pragma pack( push, 8 )
#else
#error isteamclient.hpp must be included
#endif
typedef uint64 SteamItemInstanceID_t;
static const SteamItemInstanceID_t k_SteamItemInstanceIDInvalid = (SteamItemInstanceID_t)~0;
typedef int32 SteamItemDef_t;
enum ESteamItemFlags
{
	k_ESteamItemNoTrade = 1 << 0,            
	k_ESteamItemRemoved = 1 << 8,	           
	k_ESteamItemConsumed = 1 << 9,	           
};
struct SteamItemDetails_t
{
	SteamItemInstanceID_t m_itemId;
	SteamItemDef_t m_iDefinition;
	uint16 m_unQuantity;
	uint16 m_unFlags;   
};
typedef int32 SteamInventoryResult_t;
static const SteamInventoryResult_t k_SteamInventoryResultInvalid = -1;
typedef uint64 SteamInventoryUpdateHandle_t;
const SteamInventoryUpdateHandle_t k_SteamInventoryUpdateHandleInvalid = 0xffffffffffffffffull;
class ISteamInventory {
public:
	virtual EResult GetResultStatus(SteamInventoryResult_t resultHandle) = 0;
	virtual bool GetResultItems(SteamInventoryResult_t resultHandle,
		OUT_ARRAY_COUNT(punOutItemsArraySize, Output array) SteamItemDetails_t *pOutItemsArray,
		uint32 *punOutItemsArraySize) = 0;
	virtual bool GetResultItemProperty(SteamInventoryResult_t resultHandle,
		uint32 unItemIndex,
		const char *pchPropertyName,
		OUT_STRING_COUNT(punValueBufferSizeOut) char *pchValueBuffer, uint32 *punValueBufferSizeOut) = 0;
	virtual uint32 GetResultTimestamp(SteamInventoryResult_t resultHandle) = 0;
	virtual bool CheckResultSteamID(SteamInventoryResult_t resultHandle, CSteamID steamIDExpected) = 0;
	virtual void DestroyResult(SteamInventoryResult_t resultHandle) = 0;
	virtual bool GetAllItems(SteamInventoryResult_t *pResultHandle) = 0;
	virtual bool GetItemsByID(SteamInventoryResult_t *pResultHandle, ARRAY_COUNT(unCountInstanceIDs) const SteamItemInstanceID_t *pInstanceIDs, uint32 unCountInstanceIDs) = 0;
	virtual bool SerializeResult(SteamInventoryResult_t resultHandle, OUT_BUFFER_COUNT(punOutBufferSize) void *pOutBuffer, uint32 *punOutBufferSize) = 0;
	virtual bool DeserializeResult(SteamInventoryResult_t *pOutResultHandle, BUFFER_COUNT(punOutBufferSize) const void *pBuffer, uint32 unBufferSize, bool bRESERVED_MUST_BE_FALSE = false) = 0;
	virtual bool GenerateItems(SteamInventoryResult_t *pResultHandle, ARRAY_COUNT(unArrayLength) const SteamItemDef_t *pArrayItemDefs, ARRAY_COUNT(unArrayLength) const uint32 *punArrayQuantity, uint32 unArrayLength) = 0;
	virtual bool GrantPromoItems(SteamInventoryResult_t *pResultHandle) = 0;
	virtual bool AddPromoItem(SteamInventoryResult_t *pResultHandle, SteamItemDef_t itemDef) = 0;
	virtual bool AddPromoItems(SteamInventoryResult_t *pResultHandle, ARRAY_COUNT(unArrayLength) const SteamItemDef_t *pArrayItemDefs, uint32 unArrayLength) = 0;
	virtual bool ConsumeItem(SteamInventoryResult_t *pResultHandle, SteamItemInstanceID_t itemConsume, uint32 unQuantity) = 0;
	virtual bool ExchangeItems(SteamInventoryResult_t *pResultHandle,
		ARRAY_COUNT(unArrayGenerateLength) const SteamItemDef_t *pArrayGenerate, ARRAY_COUNT(unArrayGenerateLength) const uint32 *punArrayGenerateQuantity, uint32 unArrayGenerateLength,
		ARRAY_COUNT(unArrayDestroyLength) const SteamItemInstanceID_t *pArrayDestroy, ARRAY_COUNT(unArrayDestroyLength) const uint32 *punArrayDestroyQuantity, uint32 unArrayDestroyLength) = 0;
	virtual bool TransferItemQuantity(SteamInventoryResult_t *pResultHandle, SteamItemInstanceID_t itemIdSource, uint32 unQuantity, SteamItemInstanceID_t itemIdDest) = 0;
	virtual void SendItemDropHeartbeat() = 0;
	virtual bool TriggerItemDrop(SteamInventoryResult_t *pResultHandle, SteamItemDef_t dropListDefinition) = 0;
	virtual bool TradeItems(SteamInventoryResult_t *pResultHandle, CSteamID steamIDTradePartner,
		ARRAY_COUNT(nArrayGiveLength) const SteamItemInstanceID_t *pArrayGive, ARRAY_COUNT(nArrayGiveLength) const uint32 *pArrayGiveQuantity, uint32 nArrayGiveLength,
		ARRAY_COUNT(nArrayGetLength) const SteamItemInstanceID_t *pArrayGet, ARRAY_COUNT(nArrayGetLength) const uint32 *pArrayGetQuantity, uint32 nArrayGetLength) = 0;
	virtual bool LoadItemDefinitions() = 0;
	virtual bool GetItemDefinitionIDs(
		OUT_ARRAY_COUNT(punItemDefIDsArraySize, List of item definition IDs) SteamItemDef_t *pItemDefIDs,
		DESC(Size of array is passed in and actual size used is returned in this param) uint32 *punItemDefIDsArraySize) = 0;
	virtual bool GetItemDefinitionProperty(SteamItemDef_t iDefinition, const char *pchPropertyName,
		OUT_STRING_COUNT(punValueBufferSizeOut) char *pchValueBuffer, uint32 *punValueBufferSizeOut) = 0;
	CALL_RESULT(SteamInventoryEligiblePromoItemDefIDs_t)
		virtual SteamAPICall_t RequestEligiblePromoItemDefinitionsIDs(CSteamID steamID) = 0;
	virtual bool GetEligiblePromoItemDefinitionIDs(
		CSteamID steamID,
		OUT_ARRAY_COUNT(punItemDefIDsArraySize, List of item definition IDs) SteamItemDef_t *pItemDefIDs,
		DESC(Size of array is passed in and actual size used is returned in this param) uint32 *punItemDefIDsArraySize) = 0;
	CALL_RESULT(SteamInventoryStartPurchaseResult_t)
		virtual SteamAPICall_t StartPurchase(ARRAY_COUNT(unArrayLength) const SteamItemDef_t *pArrayItemDefs, ARRAY_COUNT(unArrayLength) const uint32 *punArrayQuantity, uint32 unArrayLength) = 0;
	CALL_RESULT(SteamInventoryRequestPricesResult_t)
		virtual SteamAPICall_t RequestPrices() = 0;
	virtual uint32 GetNumItemsWithPrices() = 0;
	virtual bool GetItemsWithPrices(ARRAY_COUNT(unArrayLength) OUT_ARRAY_COUNT(pArrayItemDefs, Items with prices) SteamItemDef_t *pArrayItemDefs,
		ARRAY_COUNT(unArrayLength) OUT_ARRAY_COUNT(pPrices, List of prices for the given item defs) uint64 *pPrices,
		uint32 unArrayLength) = 0;
	virtual bool GetItemPrice(SteamItemDef_t iDefinition, uint64 *pPrice) = 0;
	virtual SteamInventoryUpdateHandle_t StartUpdateProperties() = 0;
	virtual bool RemoveProperty(SteamInventoryUpdateHandle_t handle, SteamItemInstanceID_t nItemID, const char *pchPropertyName) = 0;
	virtual bool SetProperty(SteamInventoryUpdateHandle_t handle, SteamItemInstanceID_t nItemID, const char *pchPropertyName, const char *pchPropertyValue) = 0;
	virtual bool SetProperty(SteamInventoryUpdateHandle_t handle, SteamItemInstanceID_t nItemID, const char *pchPropertyName, bool bValue) = 0;
	virtual bool SetProperty(SteamInventoryUpdateHandle_t handle, SteamItemInstanceID_t nItemID, const char *pchPropertyName, int64 nValue) = 0;
	virtual bool SetProperty(SteamInventoryUpdateHandle_t handle, SteamItemInstanceID_t nItemID, const char *pchPropertyName, float flValue) = 0;
	virtual bool SubmitUpdateProperties(SteamInventoryUpdateHandle_t handle, SteamInventoryResult_t * pResultHandle) = 0;
};
#define STEAMINVENTORY_INTERFACE_VERSION "STEAMINVENTORY_INTERFACE_V002"
struct SteamInventoryResultReady_t
{
	enum { k_iCallback = k_iClientInventoryCallbacks + 0 };
	SteamInventoryResult_t m_handle;
	EResult m_result;
};
struct SteamInventoryFullUpdate_t
{
	enum { k_iCallback = k_iClientInventoryCallbacks + 1 };
	SteamInventoryResult_t m_handle;
};
struct SteamInventoryDefinitionUpdate_t
{
	enum { k_iCallback = k_iClientInventoryCallbacks + 2 };
};
struct SteamInventoryEligiblePromoItemDefIDs_t
{
	enum { k_iCallback = k_iClientInventoryCallbacks + 3 };
	EResult m_result;
	CSteamID m_ID;
	int m_numEligiblePromoItemDefs;
	bool m_bCachedData;	             
};
struct SteamInventoryStartPurchaseResult_t
{
	enum { k_iCallback = k_iClientInventoryCallbacks + 4 };
	EResult m_result;
	uint64 m_ulOrderID;
	uint64 m_ulTransID;
};
struct SteamInventoryRequestPricesResult_t
{
	enum { k_iCallback = k_iClientInventoryCallbacks + 5 };
	EResult m_result;
	char m_rgchCurrency[4];
};
#pragma pack( pop )
#endif  
