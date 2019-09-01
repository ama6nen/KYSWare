#pragma once
#include "valve_sdk\sdk.hpp"

enum ParticleAttachment_t
{
	PATTACH_ABSORIGIN = 0,			// Create at absorigin, but don't follow
	PATTACH_ABSORIGIN_FOLLOW,		// Create at absorigin, and update to follow the entity
	PATTACH_CUSTOMORIGIN,			// Create at a custom origin, but don't follow
	PATTACH_POINT,					// Create on attachment point, but don't follow
	PATTACH_POINT_FOLLOW,			// Create on attachment point, and update to follow the entity
	PATTACH_WORLDORIGIN,			// Used for control points that don't attach to an entity
	MAX_PATTACH_TYPES,
};

struct ColorRGBExp32
{
	byte r, g, b;
	signed char exponent;
};

struct dlight_t
{
	int		flags;
	Vector	origin;
	float	radius;
	ColorRGBExp32	color;
	float	die;
	float	decay;
	float	minlight;
	int		key;
	int		style;
	Vector	m_Direction;
	float	m_InnerAngle;
	float	m_OuterAngle;
	float GetRadius() const
	{
		return radius;
	}
	float GetRadiusSquared() const
	{
		return radius * radius;
	}
	float IsRadiusGreaterThanZero() const
	{
		return radius > 0.0f;
	}
};


class CTeslaInfo
{
public:
	Vector			m_vPos;
	QAngle			m_vAngles;
	int				m_nEntIndex;
	const char		*m_pszSpriteName;
	float			m_flBeamWidth;
	int				m_nBeams;
	Vector			m_vColor;
	float			m_flTimeVisible;
	float			m_flRadius;
};

struct te_tf_particle_effects_colors_t
{
	Vector m_vecColor1;
	Vector m_vecColor2;
};

struct te_tf_particle_effects_control_point_t
{
	ParticleAttachment_t m_eParticleAttachment;
	Vector m_vecOffset;
};

class CEffectData
{
public:
	Vector m_vOrigin;
	Vector m_vStart;
	Vector m_vNormal;
	QAngle m_vAngles;
	int		m_fFlags;
	int		m_nEntIndex;
	float	m_flScale;
	float	m_flMagnitude;
	float	m_flRadius;
	int		m_nAttachmentIndex;
	short	m_nSurfaceProp;

	// Some TF2 specific things
	int		m_nMaterial;
	int		m_nDamageType;
	int		m_nHitBox;

	unsigned char	m_nColor;

	// Color customizability
	bool							m_bCustomColors;
	te_tf_particle_effects_colors_t	m_CustomColors;

	bool									m_bControlPoint1;
	te_tf_particle_effects_control_point_t	m_ControlPoint1;

	// Don't mess with stuff below here. DispatchEffect handles all of this.
public:
	CEffectData( )
	{
		m_vOrigin.Init( );
		m_vStart.Init( );
		m_vNormal.Init( );
		m_vAngles.Init( );

		m_fFlags = 0;
		m_nEntIndex = 0;
		m_flScale = 1.f;
		m_nAttachmentIndex = 0;
		m_nSurfaceProp = 0;

		m_flMagnitude = 0.0f;
		m_flRadius = 0.0f;

		m_nMaterial = 0;
		m_nDamageType = 0;
		m_nHitBox = 0;

		m_nColor = 0;

		m_bCustomColors = false;
		m_CustomColors.m_vecColor1.Init( 1.f, 1.f, 1.f );
		m_CustomColors.m_vecColor2.Init( 1.f, 1.f, 1.f );

		m_bControlPoint1 = false;
		m_ControlPoint1.m_eParticleAttachment = PATTACH_ABSORIGIN;
		m_ControlPoint1.m_vecOffset.Init( );
	}

	int GetEffectNameIndex( )
	{
		return m_iEffectName;
	}

private:
	int m_iEffectName;	// Entry in the EffectDispatch network string table. The is automatically handled by DispatchEffect().
};

class IEffects
{
public:
	virtual ~IEffects( void )
	{
	}


	virtual void Beam( const Vector &Start, const Vector &End, int nModelIndex,
					   int nHaloIndex, unsigned char frameStart, unsigned char frameRate,
					   float flLife, unsigned char width, unsigned char endWidth, unsigned char fadeLength,
					   unsigned char noise, unsigned char red, unsigned char green,
					   unsigned char blue, unsigned char brightness, unsigned char speed ) = 0;

	//-----------------------------------------------------------------------------
	// Purpose: Emits smoke sprites.
	// Input  : origin - Where to emit the sprites.
	//			scale - Sprite scale * 10.
	//			framerate - Framerate at which to animate the smoke sprites.
	//-----------------------------------------------------------------------------
	virtual void Smoke( const Vector &origin, int modelIndex, float scale, float framerate ) = 0;

	virtual void Sparks( const Vector &position, int nMagnitude = 1, int nTrailLength = 1, const Vector *pvecDir = NULL ) = 0;

	virtual void Dust( const Vector &pos, const Vector &dir, float size, float speed ) = 0;

	virtual void MuzzleFlash( const Vector &vecOrigin, const QAngle &vecAngles, float flScale, int iType ) = 0;

	// like ricochet, but no sound
	virtual void MetalSparks( const Vector &position, const Vector &direction ) = 0;

	virtual void EnergySplash( const Vector &position, const Vector &direction, bool bExplosive = false ) = 0;

	virtual void Ricochet( const Vector &position, const Vector &direction ) = 0;

	// FIXME: Should these methods remain in this interface? Or go in some 
	// other client-server neutral interface?
	virtual float Time( ) = 0;
	virtual bool IsServer( ) = 0;

	// Used by the playback system to suppress sounds
	virtual void SuppressEffectsSounds( bool bSuppress ) = 0;

};
class IVEffects
{
public:

	dlight_t* CL_AllocDlight(int key)
	{
		typedef dlight_t*(__thiscall* OriginalFn)(PVOID, int);
		return CallVFunction<OriginalFn>(this, 4)(this, key);
	}
	dlight_t* CL_AllocElight(int key)
	{
		typedef dlight_t*(__thiscall* OriginalFn)(PVOID, int);
		return CallVFunction<OriginalFn>(this, 5)(this, key);
	}
	dlight_t* GetElightByKey(int key)
	{
		typedef dlight_t*(__thiscall* OriginalFn)(PVOID, int);
		return CallVFunction<OriginalFn>(this, 8)(this, key);
	}
};
