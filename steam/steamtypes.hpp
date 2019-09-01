#pragma once

#define S_CALLTYPE __cdecl
typedef unsigned char uint8;
typedef unsigned char uint8;
typedef signed char int8;
typedef __int16 int16;
typedef unsigned __int16 uint16;
typedef __int32 int32;
typedef unsigned __int32 uint32;
typedef __int64 int64;
typedef unsigned __int64 uint64;
typedef int64 lint64;
typedef uint64 ulint64;
typedef __int32 intp;
typedef unsigned __int32 uintp;

# define CLANG_ATTR(ATTR)

#define IGNOREATTR() CLANG_ATTR( "ignore" )
#define OUT_STRUCT() CLANG_ATTR( "out_struct: ;" )
#define OUT_STRING() CLANG_ATTR( "out_string: ;" )
#define OUT_ARRAY_CALL(COUNTER,FUNCTION,PARAMS) CLANG_ATTR( "out_array_call:" #COUNTER "," #FUNCTION "," #PARAMS ";" )
#define OUT_ARRAY_COUNT(COUNTER, DESC) CLANG_ATTR( "out_array_count:" #COUNTER  ";desc:" #DESC )
#define ARRAY_COUNT(COUNTER) CLANG_ATTR( "array_count:" #COUNTER ";" )
#define ARRAY_COUNT_D(COUNTER, DESC) CLANG_ATTR( "array_count:" #COUNTER ";desc:" #DESC )
#define BUFFER_COUNT(COUNTER) CLANG_ATTR( "buffer_count:" #COUNTER ";" )
#define OUT_BUFFER_COUNT(COUNTER) CLANG_ATTR( "out_buffer_count:" #COUNTER ";" )
#define OUT_STRING_COUNT(COUNTER) CLANG_ATTR( "out_string_count:" #COUNTER ";" )
#define DESC(DESC) CLANG_ATTR("desc:" #DESC ";")
#define CALL_RESULT(RESULT_TYPE) CLANG_ATTR("callresult:" #RESULT_TYPE ";")
#define CALL_BACK(RESULT_TYPE) CLANG_ATTR("callback:" #RESULT_TYPE ";")
const int k_cubSaltSize   = 8;
typedef	uint8 Salt_t[ k_cubSaltSize ];
typedef uint64 GID_t;
const GID_t k_GIDNil = 0xffffffffffffffffull;
typedef uint64 JobID_t;			// Each Job has a unique ID
typedef GID_t TxnID_t;			// Each financial transaction has a unique ID
const GID_t k_TxnIDNil = k_GIDNil;
const GID_t k_TxnIDUnknown = 0;
const JobID_t k_JobIDNil = 0xffffffffffffffffull;
typedef uint32 PackageId_t;
const PackageId_t k_uPackageIdFreeSub = 0x0;
const PackageId_t k_uPackageIdInvalid = 0xFFFFFFFF;
typedef uint32 BundleId_t;
const BundleId_t k_uBundleIdInvalid = 0;
typedef uint32 AppId_t;
const AppId_t k_uAppIdInvalid = 0x0;
typedef uint64 AssetClassId_t;
const AssetClassId_t k_ulAssetClassIdInvalid = 0x0;
typedef uint32 PhysicalItemId_t;
const PhysicalItemId_t k_uPhysicalItemIdInvalid = 0x0;
typedef uint32 DepotId_t;
const DepotId_t k_uDepotIdInvalid = 0x0;
typedef uint32 RTime32;
typedef uint32 CellID_t;
const CellID_t k_uCellIDInvalid = 0xFFFFFFFF;
typedef uint64 SteamAPICall_t;
const SteamAPICall_t k_uAPICallInvalid = 0x0;
typedef uint32 AccountID_t;
typedef uint32 PartnerId_t;
const PartnerId_t k_uPartnerIdInvalid = 0;
typedef uint64 ManifestId_t; 
const ManifestId_t k_uManifestIdInvalid = 0;
typedef uint64 SiteId_t;
const SiteId_t k_ulSiteIdInvalid = 0;