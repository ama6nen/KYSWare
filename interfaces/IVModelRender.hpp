#pragma once

#include "../Math/QAngle.hpp"
#include "../Math/VMatrix.hpp"

enum PreviewImageRetVal_t
{
    MATERIAL_PREVIEW_IMAGE_BAD = 0,
    MATERIAL_PREVIEW_IMAGE_OK,
    MATERIAL_NO_PREVIEW_IMAGE,
};

enum MaterialVarFlags_t
{
    MATERIAL_VAR_DEBUG = (1 << 0),
    MATERIAL_VAR_NO_DEBUG_OVERRIDE = (1 << 1),
    MATERIAL_VAR_NO_DRAW = (1 << 2),
    MATERIAL_VAR_USE_IN_FILLRATE_MODE = (1 << 3),
    MATERIAL_VAR_VERTEXCOLOR = (1 << 4),
    MATERIAL_VAR_VERTEXALPHA = (1 << 5),
    MATERIAL_VAR_SELFILLUM = (1 << 6),
    MATERIAL_VAR_ADDITIVE = (1 << 7),
    MATERIAL_VAR_ALPHATEST = (1 << 8),
    //MATERIAL_VAR_UNUSED = (1 << 9),
    MATERIAL_VAR_ZNEARER = (1 << 10),
    MATERIAL_VAR_MODEL = (1 << 11),
    MATERIAL_VAR_FLAT = (1 << 12),
    MATERIAL_VAR_NOCULL = (1 << 13),
    MATERIAL_VAR_NOFOG = (1 << 14),
    MATERIAL_VAR_IGNOREZ = (1 << 15),
    MATERIAL_VAR_DECAL = (1 << 16),
    MATERIAL_VAR_ENVMAPSPHERE = (1 << 17), // OBSOLETE
    MATERIAL_VAR_UNUSED = (1 << 18), // UNUSED
    MATERIAL_VAR_ENVMAPCAMERASPACE = (1 << 19), // OBSOLETE
    MATERIAL_VAR_BASEALPHAENVMAPMASK = (1 << 20),
    MATERIAL_VAR_TRANSLUCENT = (1 << 21),
    MATERIAL_VAR_NORMALMAPALPHAENVMAPMASK = (1 << 22),
    MATERIAL_VAR_NEEDS_SOFTWARE_SKINNING = (1 << 23), // OBSOLETE
    MATERIAL_VAR_OPAQUETEXTURE = (1 << 24),
    MATERIAL_VAR_ENVMAPMODE = (1 << 25), // OBSOLETE
    MATERIAL_VAR_SUPPRESS_DECALS = (1 << 26),
    MATERIAL_VAR_HALFLAMBERT = (1 << 27),
    MATERIAL_VAR_WIREFRAME = (1 << 28),
    MATERIAL_VAR_ALLOWALPHATOCOVERAGE = (1 << 29),
    MATERIAL_VAR_ALPHA_MODIFIED_BY_PROXY = (1 << 30),
    MATERIAL_VAR_VERTEXFOG = (1 << 31),
};

typedef unsigned short ModelInstanceHandle_t;
typedef void* LightCacheHandle_t;
typedef void* StudioDecalHandle_t;
typedef int OverrideType_t;
typedef int ImageFormat;
typedef int VertexFormat_t;
typedef int MaterialPropertyTypes_t;

class IClientRenderable;
class IMaterial;
class CStudioHdr;
class IMatRenderContext;
class DataCacheHandle_t;
class ITexture;

enum MaterialVarType_t
{
	MATERIAL_VAR_TYPE_FLOAT = 0,
	MATERIAL_VAR_TYPE_STRING,
	MATERIAL_VAR_TYPE_VECTOR,
	MATERIAL_VAR_TYPE_TEXTURE,
	MATERIAL_VAR_TYPE_INT,
	MATERIAL_VAR_TYPE_FOURCC,
	MATERIAL_VAR_TYPE_UNDEFINED,
	MATERIAL_VAR_TYPE_MATRIX,
	MATERIAL_VAR_TYPE_MATERIAL,
};

typedef unsigned short MaterialVarSym_t;

class IMaterialVar
{
public:
	typedef unsigned long FourCC;

public:
	// base data and accessors
	char* m_pStringVal;
	int m_intVal;
	Vector4D m_VecVal;

	// member data. total = 4 bytes
	uint8_t m_Type : 4;
	uint8_t m_nNumVectorComps : 3;
	uint8_t m_bFakeMaterialVar : 1;
	uint8_t m_nTempIndex;
	void* m_Name;

public:
	// class factory methods
	static IMaterialVar* Create( IMaterial* pMaterial, char const* pKey, VMatrix const& matrix );
	static IMaterialVar* Create( IMaterial* pMaterial, char const* pKey, char const* pVal );
	static IMaterialVar* Create( IMaterial* pMaterial, char const* pKey, float* pVal, int numcomps );
	static IMaterialVar* Create( IMaterial* pMaterial, char const* pKey, float val );
	static IMaterialVar* Create( IMaterial* pMaterial, char const* pKey, int val );
	static IMaterialVar* Create( IMaterial* pMaterial, char const* pKey );
	static void Destroy( IMaterialVar* pVar );
	static MaterialVarSym_t	GetSymbol( char const* pName );
	static MaterialVarSym_t	FindSymbol( char const* pName );
	static bool SymbolMatches( char const* pName, MaterialVarSym_t symbol );
	static void DeleteUnreferencedTextures( bool enable );

	virtual ITexture		*GetTextureValue( void ) = 0;

	virtual char const *	GetName( void ) const = 0;
	virtual MaterialVarSym_t	GetNameAsSymbol( ) const = 0;

	virtual void			SetFloatValue( float val ) = 0;

	virtual void			SetIntValue( int val ) = 0;

	virtual void			SetStringValue( char const *val ) = 0;
	virtual char const *	GetStringValue( void ) const = 0;

	// Use FourCC values to pass app-defined data structures between
	// the proxy and the shader. The shader should ignore the data if
	// its FourCC type not correct.	
	virtual void			SetFourCCValue( FourCC type, void *pData ) = 0;
	virtual void			GetFourCCValue( FourCC *type, void **ppData ) = 0;

	// Vec (dim 2-4)
	virtual void			SetVecValue( float const* val, int numcomps ) = 0;
	virtual void			SetVecValue( float x, float y ) = 0;
	virtual void			SetVecValue( float x, float y, float z ) = 0;
	virtual void			SetVecValue( float x, float y, float z, float w ) = 0;
	virtual void			GetLinearVecValue( float *val, int numcomps ) const = 0;

	// revisit: is this a good interface for textures?
	virtual void			SetTextureValue( ITexture * ) = 0;

	virtual IMaterial *		GetMaterialValue( void ) = 0;
	virtual void			SetMaterialValue( IMaterial * ) = 0;

	virtual bool			IsDefined( ) const = 0;
	virtual void			SetUndefined( ) = 0;

	// Matrix
	virtual void			SetMatrixValue( VMatrix const& matrix ) = 0;
	virtual const VMatrix  &GetMatrixValue( ) = 0;
	virtual bool			MatrixIsIdentity( ) const = 0;

	// Copy....
	virtual void			CopyFrom( IMaterialVar *pMaterialVar ) = 0;

	virtual void			SetValueAutodetectType( char const *val ) = 0;

	virtual IMaterial *		GetOwningMaterial( ) = 0;

	//set just 1 component
	virtual void			SetVecComponentValue( float fVal, int nComponent ) = 0;

protected:
	virtual int				GetIntValueInternal( void ) const = 0;
	virtual float			GetFloatValueInternal( void ) const = 0;
	virtual float const*	GetVecValueInternal( ) const = 0;
	virtual void			GetVecValueInternal( float *val, int numcomps ) const = 0;
	virtual int				VectorSizeInternal( ) const = 0;

public:
	FORCEINLINE MaterialVarType_t GetType( void ) const
	{
		return (MaterialVarType_t)m_Type;
	}

	FORCEINLINE bool IsTexture( ) const
	{
		return m_Type == MATERIAL_VAR_TYPE_TEXTURE;
	}

	FORCEINLINE operator ITexture*( )
	{
		return GetTextureValue( );
	}

	// NOTE: Fast methods should only be called in thread-safe situations
	FORCEINLINE int GetIntValueFast( void ) const
	{
		// Set methods for float and vector update this
		return m_intVal;
	}

	FORCEINLINE float GetFloatValueFast( void ) const
	{
		return m_VecVal[ 0 ];
	}

	FORCEINLINE float const* GetVecValueFast( ) const
	{
		return m_VecVal.Base( );
	}

	FORCEINLINE void GetVecValueFast( float *val, int numcomps ) const
	{
		for ( int i = 0; i < numcomps; i++ )
		{
			val[ i ] = m_VecVal[ i ];
		}
	}

	FORCEINLINE int VectorSizeFast( ) const
	{
		return m_nNumVectorComps;
	}

#ifdef FAST_MATERIALVAR_ACCESS
	FORCEINLINE int GetIntValue( void ) const
	{
		return GetIntValueFast( );
	}

	FORCEINLINE float GetFloatValue( void ) const
	{
		return GetFloatValueFast( );
	}

	FORCEINLINE float const* GetVecValue( ) const
	{
		return GetVecValueFast( );
	}

	FORCEINLINE void GetVecValue( float *val, int numcomps ) const
	{
		GetVecValueFast( val, numcomps );
	}

	FORCEINLINE int VectorSize( ) const
	{
		return VectorSizeFast( );
	}
#else // !FAST_MATERIALVAR_ACCESS
	FORCEINLINE int GetIntValue( void ) const
	{
		return GetIntValueInternal( );
	}

	FORCEINLINE float GetFloatValue( void ) const
	{
		return GetFloatValueInternal( );
	}

	FORCEINLINE float const* GetVecValue( ) const
	{
		return GetVecValueInternal( );
	}

	FORCEINLINE void GetVecValue( float *val, int numcomps ) const
	{
		return GetVecValueInternal( val, numcomps );
	}

	FORCEINLINE int VectorSize( ) const
	{
		return VectorSizeInternal( );
	}
#endif

private:
	FORCEINLINE void SetTempIndex( int nIndex )
	{
		m_nTempIndex = nIndex;
	}

	friend void EnableThreadedMaterialVarAccess( bool bEnable, IMaterialVar **ppParams, int nVarCount );
};
struct FileHandle_t;
typedef unsigned __int64		uint64;
class KeyValues
{
public:
	//	By default, the KeyValues class uses a string table for the key names that is
	//	limited to 4MB. The game will exit in error if this space is exhausted. In
	//	general this is preferable for game code for performance and memory fragmentation
	//	reasons.
	//
	//	If this is not acceptable, you can use this call to switch to a table that can grow
	//	arbitrarily. This call must be made before any KeyValues objects are allocated or it
	//	will result in undefined behavior. If you use the growable string table, you cannot
	//	share KeyValues pointers directly with any other module. You can serialize them across
	//	module boundaries. These limitations are acceptable in the Steam backend code 
	//	this option was written for, but may not be in other situations. Make sure to
	//	understand the implications before using this.
	static void SetUseGrowableStringTable(bool bUseGrowableTable);

	KeyValues(const char *setName) {}

	//
	// AutoDelete class to automatically free the keyvalues.
	// Simply construct it with the keyvalues you allocated and it will free them when falls out of scope.
	// When you decide that keyvalues shouldn't be deleted call Assign(NULL) on it.
	// If you constructed AutoDelete(NULL) you can later assign the keyvalues to be deleted with Assign(pKeyValues).
	// You can also pass temporary KeyValues object as an argument to a function by wrapping it into KeyValues::AutoDelete
	// instance:   call_my_function( KeyValues::AutoDelete( new KeyValues( "test" ) ) )
	//
	class AutoDelete
	{
	public:
		explicit inline AutoDelete(KeyValues *pKeyValues) : m_pKeyValues(pKeyValues) {}
		explicit inline AutoDelete(const char *pchKVName) : m_pKeyValues(new KeyValues(pchKVName)) {}
		inline ~AutoDelete(void) { if (m_pKeyValues) m_pKeyValues->deleteThis(); }
		inline void Assign(KeyValues *pKeyValues) { m_pKeyValues = pKeyValues; }
		KeyValues *operator->() { return m_pKeyValues; }
		operator KeyValues *() { return m_pKeyValues; }
	private:
		AutoDelete(AutoDelete const &x); // forbid
		AutoDelete & operator= (AutoDelete const &x); // forbid
		KeyValues *m_pKeyValues;
	};

	// Quick setup constructors
	KeyValues(const char *setName, const char *firstKey, const char *firstValue);
	KeyValues(const char *setName, const char *firstKey, const wchar_t *firstValue);
	KeyValues(const char *setName, const char *firstKey, int firstValue);
	KeyValues(const char *setName, const char *firstKey, const char *firstValue, const char *secondKey, const char *secondValue);
	KeyValues(const char *setName, const char *firstKey, int firstValue, const char *secondKey, int secondValue);

	// Section name
	const char *GetName() const;
	void SetName(const char *setName);

	// gets the name as a unique int
	int GetNameSymbol() const { return m_iKeyName; }

	// File access. Set UsesEscapeSequences true, if resource file/buffer uses Escape Sequences (eg \n, \t)
	void UsesEscapeSequences(bool state); // default false
	void UsesConditionals(bool state); // default true
	bool LoadFromFile(void *filesystem, const char *resourceName, const char *pathID = NULL);
	bool SaveToFile(void *filesystem, const char *resourceName, const char *pathID = NULL, bool sortKeys = false, bool bAllowEmptyString = false);

	// Read from a buffer...  Note that the buffer must be null terminated
	bool LoadFromBuffer(char const *resourceName, const char *pBuffer, void* pFileSystem = NULL, const char *pPathID = NULL);

	// Read from a utlbuffer...
	bool LoadFromBuffer(char const *resourceName, void*buf, void* pFileSystem = NULL, const char *pPathID = NULL);

	// Find a keyValue, create it if it is not found.
	// Set bCreate to true to create the key if it doesn't already exist (which ensures a valid pointer will be returned)
	KeyValues *FindKey(const char *keyName, bool bCreate = false);
	KeyValues *FindKey(int keySymbol) const;
	KeyValues *CreateNewKey();		// creates a new key, with an autogenerated name.  name is guaranteed to be an integer, of value 1 higher than the highest other integer key name
	void AddSubKey(KeyValues *pSubkey);	// Adds a subkey. Make sure the subkey isn't a child of some other keyvalues
	void RemoveSubKey(KeyValues *subKey);	// removes a subkey from the list, DOES NOT DELETE IT

											// Key iteration.
											//
											// NOTE: GetFirstSubKey/GetNextKey will iterate keys AND values. Use the functions 
											// below if you want to iterate over just the keys or just the values.
											//
	KeyValues *GetFirstSubKey() { return m_pSub; }	// returns the first subkey in the list
	KeyValues *GetNextKey() { return m_pPeer; }		// returns the next subkey
	void SetNextKey(KeyValues * pDat);
	KeyValues *FindLastSubKey();	// returns the LAST subkey in the list.  This requires a linked list iteration to find the key.  Returns NULL if we don't have any children

									//
									// These functions can be used to treat it like a true key/values tree instead of 
									// confusing values with keys.
									//
									// So if you wanted to iterate all subkeys, then all values, it would look like this:
									//     for ( KeyValues *pKey = pRoot->GetFirstTrueSubKey(); pKey; pKey = pKey->GetNextTrueSubKey() )
									//     {
									//		   Msg( "Key name: %s\n", pKey->GetName() );
									//     }
									//     for ( KeyValues *pValue = pRoot->GetFirstValue(); pKey; pKey = pKey->GetNextValue() )
									//     {
									//         Msg( "Int value: %d\n", pValue->GetInt() );  // Assuming pValue->GetDataType() == TYPE_INT...
									//     }
	KeyValues* GetFirstTrueSubKey();
	KeyValues* GetNextTrueSubKey();

	KeyValues* GetFirstValue();	// When you get a value back, you can use GetX and pass in NULL to get the value.
	KeyValues* GetNextValue();


	// Data access
	int   GetInt(const char *keyName = NULL, int defaultValue = 0);
	uint64 GetUint64(const char *keyName = NULL, uint64 defaultValue = 0);
	float GetFloat(const char *keyName = NULL, float defaultValue = 0.0f);
	const char *GetString(const char *keyName = NULL, const char *defaultValue = "");
	const wchar_t *GetWString(const char *keyName = NULL, const wchar_t *defaultValue = L"");
	void *GetPtr(const char *keyName = NULL, void *defaultValue = (void*)0);
	bool GetBool(const char *keyName = NULL, bool defaultValue = false);
	Color GetColor(const char *keyName = NULL /* default value is all black */);
	bool  IsEmpty(const char *keyName = NULL);

	// Data access
	int   GetInt(int keySymbol, int defaultValue = 0);
	float GetFloat(int keySymbol, float defaultValue = 0.0f);
	const char *GetString(int keySymbol, const char *defaultValue = "");
	const wchar_t *GetWString(int keySymbol, const wchar_t *defaultValue = L"");
	void *GetPtr(int keySymbol, void *defaultValue = (void*)0);
	Color GetColor(int keySymbol /* default value is all black */);
	bool  IsEmpty(int keySymbol);

	// Key writing
	void SetWString(const char *keyName, const wchar_t *value);
	void SetString(const char *keyName, const char *value);
	void SetInt(const char *keyName, int value);
	void SetUint64(const char *keyName, uint64 value);
	void SetFloat(const char *keyName, float value);
	void SetPtr(const char *keyName, void *value);
	void SetColor(const char *keyName, Color value);
	void SetBool(const char *keyName, bool value) { SetInt(keyName, value ? 1 : 0); }

	// Adds a chain... if we don't find stuff in this keyvalue, we'll look
	// in the one we're chained to.
	void ChainKeyValue(KeyValues* pChain);

	void RecursiveSaveToFile(void* buf, int indentLevel, bool sortKeys = false, bool bAllowEmptyString = false);

	bool WriteAsBinary(void*buffer);
	bool ReadAsBinary(void*buffer, int nStackDepth = 0);

	// Allocate & create a new copy of the keys
	KeyValues *MakeCopy(void) const;

	// Make a new copy of all subkeys, add them all to the passed-in keyvalues
	void CopySubkeys(KeyValues *pParent) const;

	// Clear out all subkeys, and the current value
	void Clear(void);

	// Data type
	enum types_t
	{
		TYPE_NONE = 0,
		TYPE_STRING,
		TYPE_INT,
		TYPE_FLOAT,
		TYPE_PTR,
		TYPE_WSTRING,
		TYPE_COLOR,
		TYPE_UINT64,
		TYPE_NUMTYPES,
	};
	types_t GetDataType(const char *keyName = NULL);

	// Virtual deletion function - ensures that KeyValues object is deleted from correct heap
	void deleteThis();

	void SetStringValue(char const *strValue);

	// unpack a key values list into a structure
	void UnpackIntoStructure(struct KeyValuesUnpackStructure const *pUnpackTable, void *pDest, size_t DestSizeInBytes);

	// Process conditional keys for widescreen support.
	bool ProcessResolutionKeys(const char *pResString);

	// Dump keyvalues recursively into a dump context
	bool Dump(class IKeyValuesDumpContext *pDump, int nIndentLevel = 0);

	// Merge in another KeyValues, keeping "our" settings
	void RecursiveMergeKeyValues(KeyValues *baseKV);

private:
	KeyValues(KeyValues&);	// prevent copy constructor being used

							// prevent delete being called except through deleteThis()
	~KeyValues();

	KeyValues* CreateKey(const char *keyName);

	/// Create a child key, given that we know which child is currently the last child.
	/// This avoids the O(N^2) behaviour when adding children in sequence to KV,
	/// when CreateKey() wil have to re-locate the end of the list each time.  This happens,
	/// for example, every time we load any KV file whatsoever.
	KeyValues* CreateKeyUsingKnownLastChild(const char *keyName, KeyValues *pLastChild);
	void AddSubkeyUsingKnownLastChild(KeyValues *pSubKey, KeyValues *pLastChild);

	void RecursiveCopyKeyValues(KeyValues& src);
	void RemoveEverything();
	//	void RecursiveSaveToFile( IBaseFileSystem *filesystem, void*buffer, int indentLevel );
	//	void WriteConvertedString( void*buffer, const char *pszString );

	// NOTE: If both filesystem and pBuf are non-null, it'll save to both of them.
	// If filesystem is null, it'll ignore f.
	void RecursiveSaveToFile(void *filesystem, FileHandle_t f, void *pBuf, int indentLevel, bool sortKeys, bool bAllowEmptyString);
	void SaveKeyToFile(KeyValues *dat, void *filesystem, FileHandle_t f, void *pBuf, int indentLevel, bool sortKeys, bool bAllowEmptyString);
	void WriteConvertedString(void *filesystem, FileHandle_t f, void *pBuf, const char *pszString);

	void RecursiveLoadFromBuffer(char const *resourceName, void*buf);

	// For handling #include "filename"
	void AppendIncludedKeys(void* includedKeys);
	void ParseIncludedKeys(char const *resourceName, const char *filetoinclude,
		void* pFileSystem, const char *pPathID, void* includedKeys);

	// For handling #base "filename"
	void MergeBaseKeys(void* baseKeys);

	// NOTE: If both filesystem and pBuf are non-null, it'll save to both of them.
	// If filesystem is null, it'll ignore f.
	void InternalWrite(void *filesystem, FileHandle_t f, void *pBuf, const void *pData, int len);

	void Init();
	const char * ReadToken(void*buf, bool &wasQuoted, bool &wasConditional);
	void WriteIndents(void *filesystem, FileHandle_t f, void *pBuf, int indentLevel);

	void FreeAllocatedValue();
	void AllocateValueBlock(int size);

	int m_iKeyName;	// keyname is a symbol defined in KeyValuesSystem

					// These are needed out of the union because the API returns string pointers
	char *m_sValue;
	wchar_t *m_wsValue;

	// we don't delete these
	union
	{
		int m_iValue;
		float m_flValue;
		void *m_pValue;
		unsigned char m_Color[4];
	};

	char	   m_iDataType;
	char	   m_bHasEscapeSequences; // true, if while parsing this KeyValue, Escape Sequences are used (default false)
	char	   m_bEvaluateConditionals; // true, if while parsing this KeyValue, conditionals blocks are evaluated (default true)
	char	   unused[1];

	KeyValues *m_pPeer;	// pointer to next key in list
	KeyValues *m_pSub;	// pointer to Start of a new sub key list
	KeyValues *m_pChain;// Search here if it's not in our list

private:
	// Statics to implement the optional growable string table
	// Function pointers that will determine which mode we are in
	static int(*s_pfGetSymbolForString)(const char *name, bool bCreate);
	static const char *(*s_pfGetStringForSymbol)(int symbol);
	static void *s_pGrowableStringTable;

public:
	// Functions that invoke the default behavior
	static int GetSymbolForStringClassic(const char *name, bool bCreate = true);
	static const char *GetStringForSymbolClassic(int symbol);

	// Functions that use the growable string table
	static int GetSymbolForStringGrowable(const char *name, bool bCreate = true);
	static const char *GetStringForSymbolGrowable(int symbol);

	// Functions to get external access to whichever of the above functions we're going to call.
	static int CallGetSymbolForString(const char *name, bool bCreate = true) { return s_pfGetSymbolForString(name, bCreate); }
	static const char *CallGetStringForSymbol(int symbol) { return s_pfGetStringForSymbol(symbol); }
};


struct model_t;
struct mstudioanimdesc_t;
struct mstudioseqdesc_t;
struct Ray_t;
struct DrawModelInfo_t;
struct studiohwdata_t;
struct MaterialLightingState_t;
struct ColorMeshInfo_t;

struct DrawModelState_t
{
    studiohdr_t*            m_pStudioHdr;
    studiohwdata_t*         m_pStudioHWData;
    IClientRenderable*      m_pRenderable;
    const matrix3x4_t*      m_pModelToWorld;
    StudioDecalHandle_t     m_decals;
    int                     m_drawFlags;
    int                     m_lod;
};

struct StaticPropRenderInfo_t
{
    const matrix3x4_t*      pModelToWorld;
    const model_t*          pModel;
    IClientRenderable*      pRenderable;
    Vector*                 pLightingOrigin;
    short                   skin;
    ModelInstanceHandle_t   instance;
};

struct ModelRenderInfo_t
{
    Vector                  origin;
    QAngle                  angles;
	char pad[ 0x4 ];
    IClientRenderable*      pRenderable;
    const model_t*          pModel;
    const matrix3x4_t*      pModelToWorld;
    const matrix3x4_t*      pLightingOffset;
    const Vector*           pLightingOrigin;
    int                     flags;
    int                     entity_index;
    int                     skin;
    int                     body;
    int                     hitboxset;
    ModelInstanceHandle_t   instance;

    ModelRenderInfo_t()
    {
        pModelToWorld = NULL;
        pLightingOffset = NULL;
        pLightingOrigin = NULL;
    }
};

struct LightingQuery_t
{
    Vector                  m_LightingOrigin;
    ModelInstanceHandle_t   m_InstanceHandle;
    bool                    m_bAmbientBoost;
};

struct StaticLightingQuery_t : public LightingQuery_t
{
    IClientRenderable*        m_pRenderable;
};

class IMaterial
{
public:
    virtual const char*             GetName() const = 0;
    virtual const char*             GetTextureGroupName() const = 0;
    virtual PreviewImageRetVal_t    GetPreviewImageProperties(int *width, int *height, ImageFormat *imageFormat, bool* isTranslucent) const = 0;
    virtual PreviewImageRetVal_t    GetPreviewImage(unsigned char *data, int width, int height, ImageFormat imageFormat) const = 0;
    virtual int                     GetMappingWidth() = 0;
    virtual int                     GetMappingHeight() = 0;
    virtual int                     GetNumAnimationFrames() = 0;
    virtual bool                    InMaterialPage(void) = 0;
    virtual    void                 GetMaterialOffset(float *pOffset) = 0;
    virtual void                    GetMaterialScale(float *pScale) = 0;
    virtual IMaterial*              GetMaterialPage(void) = 0;
    virtual IMaterialVar*           FindVar(const char *varName, bool *found, bool complain = true) = 0;
    virtual void                    IncrementReferenceCount(void) = 0;
    virtual void                    DecrementReferenceCount(void) = 0;
    inline void                     AddRef() { IncrementReferenceCount(); }
    inline void                     Release() { DecrementReferenceCount(); }
    virtual int                     GetEnumerationID(void) const = 0;
    virtual void                    GetLowResColorSample(float s, float t, float *color) const = 0;
    virtual void                    RecomputeStateSnapshots() = 0;
    virtual bool                    IsTranslucent() = 0;
    virtual bool                    IsAlphaTested() = 0;
    virtual bool                    IsVertexLit() = 0;
    virtual VertexFormat_t          GetVertexFormat() const = 0;
    virtual bool                    HasProxy(void) const = 0;
    virtual bool                    UsesEnvCubemap(void) = 0;
    virtual bool                    NeedsTangentSpace(void) = 0;
    virtual bool                    NeedsPowerOfTwoFrameBufferTexture(bool bCheckSpecificToThisFrame = true) = 0;
    virtual bool                    NeedsFullFrameBufferTexture(bool bCheckSpecificToThisFrame = true) = 0;
    virtual bool                    NeedsSoftwareSkinning(void) = 0;
    virtual void                    AlphaModulate(float alpha) = 0;
    virtual void                    ColorModulate(float r, float g, float b) = 0;
    virtual void                    SetMaterialVarFlag(MaterialVarFlags_t flag, bool on) = 0;
    virtual bool                    GetMaterialVarFlag(MaterialVarFlags_t flag) const = 0;
    virtual void                    GetReflectivity(Vector& reflect) = 0;
    virtual bool                    GetPropertyFlag(MaterialPropertyTypes_t type) = 0;
    virtual bool                    IsTwoSided() = 0;
    virtual void                    SetShader(const char *pShaderName) = 0;
    virtual int                     GetNumPasses(void) = 0;
    virtual int                     GetTextureMemoryBytes(void) = 0;
    virtual void                    Refresh() = 0;
    virtual bool                    NeedsLightmapBlendAlpha(void) = 0;
    virtual bool                    NeedsSoftwareLighting(void) = 0;
    virtual int                     ShaderParamCount() const = 0;
    virtual IMaterialVar**          GetShaderParams(void) = 0;
    virtual bool                    IsErrorMaterial() const = 0;
    virtual void                    Unused() = 0;
    virtual float                   GetAlphaModulation() = 0;
    virtual void                    GetColorModulation(float *r, float *g, float *b) = 0;
    virtual bool                    IsTranslucentUnderModulation(float fAlphaModulation = 1.0f) const = 0;
    virtual IMaterialVar*           FindVarFast(char const *pVarName, unsigned int *pToken) = 0;
    virtual void                    SetShaderAndParams(KeyValues *pKeyValues) = 0;
    virtual const char*             GetShaderName() const = 0;
    virtual void                    DeleteIfUnreferenced() = 0;
    virtual bool                    IsSpriteCard() = 0;
    virtual void                    CallBindProxy(void *proxyData) = 0;
    virtual void                    RefreshPreservingMaterialVars() = 0;
    virtual bool                    WasReloadedFromWhitelist() = 0;
    virtual bool                    SetTempExcluded(bool bSet, int nExcludedDimensionLimit) = 0;
    virtual int                     GetReferenceCount() const = 0;
};

class IVModelRender
{
public:
    virtual int                     DrawModel(int flags, IClientRenderable *pRenderable, ModelInstanceHandle_t instance, int entity_index, const model_t *model, Vector const& origin, QAngle const& angles, int skin, int body, int hitboxset, const matrix3x4_t *modelToWorld = NULL, const matrix3x4_t *pLightingOffset = NULL) = 0;
    virtual void                    ForcedMaterialOverride(IMaterial *newMaterial, OverrideType_t nOverrideType = 0, int nOverrides = 0) = 0;
    virtual bool                    IsForcedMaterialOverride(void) = 0;
    virtual void                    SetViewTarget(const CStudioHdr *pStudioHdr, int nBodyIndex, const Vector& target) = 0;
    virtual ModelInstanceHandle_t   CreateInstance(IClientRenderable *pRenderable, LightCacheHandle_t *pCache = NULL) = 0;
    virtual void                    DestroyInstance(ModelInstanceHandle_t handle) = 0;
    virtual void                    SetStaticLighting(ModelInstanceHandle_t handle, LightCacheHandle_t* pHandle) = 0;
    virtual LightCacheHandle_t      GetStaticLighting(ModelInstanceHandle_t handle) = 0;
    virtual bool                    ChangeInstance(ModelInstanceHandle_t handle, IClientRenderable *pRenderable) = 0;
    virtual void                    AddDecal(ModelInstanceHandle_t handle, Ray_t const& ray, Vector const& decalUp, int decalIndex, int body, bool noPokeThru, int maxLODToDecal) = 0;
    virtual void                    RemoveAllDecals(ModelInstanceHandle_t handle) = 0;
    virtual bool                    ModelHasDecals(ModelInstanceHandle_t handle) = 0;
    virtual void                    RemoveAllDecalsFromAllModels() = 0;
    virtual matrix3x4_t*            DrawModelShadowSetup(IClientRenderable *pRenderable, int body, int skin, DrawModelInfo_t *pInfo, matrix3x4_t *pCustomBoneToWorld = NULL) = 0;
    virtual void                    DrawModelShadow(IClientRenderable *pRenderable, const DrawModelInfo_t &info, matrix3x4_t *pCustomBoneToWorld = NULL) = 0;
    virtual bool                    RecomputeStaticLighting(ModelInstanceHandle_t handle) = 0;
    virtual void                    ReleaseAllStaticPropColorData(void) = 0;
    virtual void                    RestoreAllStaticPropColorData(void) = 0;
    virtual int                     DrawModelEx(ModelRenderInfo_t &pInfo) = 0;
    virtual int                     DrawModelExStaticProp(ModelRenderInfo_t &pInfo) = 0;
    virtual bool                    DrawModelSetup(ModelRenderInfo_t &pInfo, DrawModelState_t *pState, matrix3x4_t **ppBoneToWorldOut) = 0;
    virtual void                    DrawModelExecute(IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld = NULL) = 0;
    virtual void                    SetupLighting(const Vector &vecCenter) = 0;
    virtual int                     DrawStaticPropArrayFast(StaticPropRenderInfo_t *pProps, int count, bool bShadowDepth) = 0;
    virtual void                    SuppressEngineLighting(bool bSuppress) = 0;
    virtual void                    SetupColorMeshes(int nTotalVerts) = 0;
    virtual void                    SetupLightingEx(const Vector &vecCenter, ModelInstanceHandle_t handle) = 0;
    virtual bool                    GetBrightestShadowingLightSource(const Vector &vecCenter, Vector& lightPos, Vector& lightBrightness, bool bAllowNonTaggedLights) = 0;
    virtual void                    ComputeLightingState(int nCount, const LightingQuery_t *pQuery, MaterialLightingState_t *pState, ITexture **ppEnvCubemapTexture) = 0;
    virtual void                    GetModelDecalHandles(StudioDecalHandle_t *pDecals, int nDecalStride, int nCount, const ModelInstanceHandle_t *pHandles) = 0;
    virtual void                    ComputeStaticLightingState(int nCount, const StaticLightingQuery_t *pQuery, MaterialLightingState_t *pState, MaterialLightingState_t *pDecalState, ColorMeshInfo_t **ppStaticLighting, ITexture **ppEnvCubemapTexture, DataCacheHandle_t *pColorMeshHandles) = 0;
    virtual void                    CleanupStaticLightingState(int nCount, DataCacheHandle_t *pColorMeshHandles) = 0;
};
