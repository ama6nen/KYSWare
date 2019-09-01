#pragma once

#include "../singleton.hpp"
#include "../valve_sdk/misc/vfunc.hpp"
#include "../valve_sdk/csgostructs.hpp"
#include "../helpers/utils.hpp"

class IMatRenderContext
{
public:
	void SetStencilState(StencilState& sten)
	{
		typedef void(__thiscall* dfn)(void*, StencilState&);
		if ( this )
			CallVFunction<dfn>( this, 127 )( this, sten );
	}

	int Release( )
	{
		typedef int( __thiscall* asdf )( void* );
		return CallVFunction<asdf>( this, 1 )( this );
	}

	void SetRenderTarget( ITexture* texture )
	{
		typedef void( __thiscall* asdf )( void*, ITexture* );
		return CallVFunction<asdf>( this, 6 )( this, texture );
	}

	void PushRenderTargetAndViewport( )
	{
		typedef void( __thiscall* asdf )( void* );
		return CallVFunction<asdf>( this, 118 )( this );
	}

	void PopRenderTargetAndViewport( )
	{
		typedef void( __thiscall* asdf )( void* );
		return CallVFunction<asdf>( this, 119 )( this );
	}
	void DrawScreenSpaceRectangle( IMaterial* pMaterial,
								   int destX, int destY, int width, int height,
								   float srcTextureX0, float srcTextureY0, float srcTextureX1, float srcTextureY1,
								   int srcTextureWidth, int srcTextureHeight,
								   void* pClientRenderable, int nXDice, int nYDice )
	{
		typedef void( __thiscall* asdf )( void*, IMaterial*, int, int , int , int , float, float, float, float, int, int, void*, int, int );
		return CallVFunction<asdf>( this, 113 )( this , pMaterial, destX, destY, width, height, srcTextureX0, srcTextureY0, srcTextureX1, srcTextureY1, srcTextureWidth, srcTextureHeight, pClientRenderable, nXDice, nYDice);
	}
};
struct DrawModelState_t;
struct ModelRenderInfo_t;
class matrix3x4_t;
class IMaterial;
class Color;

class chams
    : public Singleton<chams>
{
    friend class Singleton<chams>;

	chams();
    ~chams();

public:
    void OnDrawModelExecute(
        IMatRenderContext* ctx,
        const DrawModelState_t &state,
		const ModelRenderInfo_t &pInfo,
        matrix3x4_t *pCustomBoneToWorld);

    void OverrideMaterial(bool ignoreZ, bool flat, bool wireframe, bool glass, const Color& rgba, bool nodraw = false, bool additive = false, IMaterial* custom = nullptr);

    IMaterial* materialRegular = nullptr;
    IMaterial* materialRegularIgnoreZ = nullptr;
    IMaterial* materialFlatIgnoreZ = nullptr;
    IMaterial* materialFlat = nullptr;
};