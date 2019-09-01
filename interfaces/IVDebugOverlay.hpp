#pragma once

#include "../Math/Vector.hpp"
#include "../Math/QAngle.hpp"
#include "../Math/VMatrix.hpp"

class OverlayText_t;
#define FMTFUNCTION( a, b )

class IVDebugOverlay
{
public:
	virtual void AddCBaseEntityTextOverlay(int ent_index, int line_offset, float duration, int r, int g, int b, int a, const char *format, ...) FMTFUNCTION(9, 10) = 0;
	virtual void AddBoxOverlay(const Vector& origin, const Vector& mins, const Vector& max, QAngle const& orientation, int r, int g, int b, int a, float duration = 0) = 0;
	virtual void AddSphereOverlay(const Vector& vOrigin, float flRadius, int nTheta, int nPhi, int r, int g, int b, int a, float flDuration = 0) = 0;
	virtual void AddTriangleOverlay(const Vector& p1, const Vector& p2, const Vector& p3, int r, int g, int b, int a, bool noDepthTest, float duration = 0) = 0;
	virtual void AddBoxOverlayNew(const Vector& v1, const Vector& v2, const Vector& v3, const Vector& v4, int r, int g, int b, int a) = 0;
	virtual void AddLineOverlay(const Vector& origin, const Vector& dest, int r, int g, int b, bool noDepthTest, float duration = 0) = 0;
	virtual void AddTextOverlay(const Vector& origin, float duration, const char *format, ...) FMTFUNCTION(4, 5) = 0;
	virtual void AddTextOverlay(const Vector& origin, int line_offset, float duration, const char *format, ...) FMTFUNCTION(5, 6) = 0;
	virtual void AddScreenTextOverlay(float flXPos, float flYPos, float flDuration, int r, int g, int b, int a, const char *text) = 0;
	virtual void AddSweptBoxOverlay(const Vector& start, const Vector& end, const Vector& mins, const Vector& max, const QAngle & angles, int r, int g, int b, int a, float flDuration = 0) = 0;
	virtual void AddGridOverlay(const Vector& origin) = 0;
	virtual void AddCoordFrameOverlay(const matrix3x4_t& frame, float flScale, int vColorTable[3][3] = NULL) = 0;
	virtual int ScreenPosition(const Vector& point, Vector& screen) = 0;
	virtual int ScreenPosition(float flXPos, float flYPos, Vector& screen) = 0;
	virtual OverlayText_t *GetFirst(void) = 0;
	virtual OverlayText_t *GetNext(OverlayText_t *current) = 0;
	virtual void ClearDeadOverlays(void) = 0;
	virtual void ClearAllOverlays() = 0;
	virtual void AddTextOverlayRGB(const Vector& origin, int line_offset, float duration, float r, float g, float b, float alpha, const char *format, ...) FMTFUNCTION(9, 10) = 0;
	virtual void AddTextOverlayRGB(const Vector& origin, int line_offset, float duration, int r, int g, int b, int a, const char *format, ...) FMTFUNCTION(9, 10) = 0;
	virtual void AddLineOverlayAlpha(const Vector& origin, const Vector& dest, int r, int g, int b, int a, bool noDepthTest, float duration) = 0;
	virtual void AddBoxOverlay2(const Vector& origin, const Vector& mins, const Vector& max, QAngle const& orientation, const Color& faceColor, const Color& edgeColor, float duration) = 0;
	virtual void PurgeTextOverlays() = 0;
	virtual void DrawPill( const Vector& mins, const Vector& max, float& diameter, int r, int g, int b, int a, float duration ) = 0;
	virtual void DrawPill2( const Vector& mins, const Vector& max, float& diameter, int r, int g, int b, int a, float duration ) = 0;

private:
	inline void AddTextOverlay(const Vector& origin, int line_offset, float duration, int r, int g, int b, int a, const char *format, ...) {}

//public:
//	void DrawPill( const Vector& mins, const Vector& max, float& diameter, int r, int g, int b, int a, float duration )
//	{
//		
//		typedef void( *OriginalFn )( void*, const Vector&, const Vector&, float&, int, int, int, int, float );
//		__asm push esp
//		return CallVFunction<OriginalFn>( this, 23 )( this, mins, max, diameter, r, g, b, a, duration );
//		__asm pop esp
//	}
};