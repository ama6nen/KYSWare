#include "Math.hpp"

namespace Math
{

	float RandomFloat( float min, float max )
	{
		static auto fn = (decltype( &RandomFloat ))( GetProcAddress( LI_FIND_CACHED( GetModuleHandleW )( L"vstdlib.dll" ), "RandomFloat" ) );
		return fn( min, max );
	}

	void RandomSeed( int seed )
	{
		static auto fn = (decltype( &RandomSeed ))( GetProcAddress( LI_FIND_CACHED( GetModuleHandleW )( L"vstdlib.dll" ), "RandomSeed" ) );
		return fn( seed );
	}

	void NormaliseViewAngle( Vector &angle )
	{
		while ( angle.y <= -180 ) angle.y += 360;
		while ( angle.y > 180 ) angle.y -= 360;
		while ( angle.x <= -180 ) angle.x += 360;
		while ( angle.x > 180 ) angle.x -= 360;


		if ( angle.x > 89 ) angle.x = 89;
		if ( angle.x < -89 ) angle.x = -89;
		if ( angle.y < -180 ) angle.y = -179.999;
		if ( angle.y > 180 ) angle.y = 179.999;

		angle.z = 0;
	}

	void VectorSubtract( const Vector& a, const Vector& b, Vector& c )
	{
		c.x = a.x - b.x;
		c.y = a.y - b.y;
		c.z = a.z - b.z;
	}

	void Normalize( Vector &vIn, Vector &vOut )
	{
		float flLen = vIn.Length( );
		if ( flLen == 0 )
		{
			vOut.Init( 0, 0, 1 );
			return;
		}
		flLen = 1 / flLen;
		vOut.Init( vIn.x * flLen, vIn.y * flLen, vIn.z * flLen );
	}

	void NormalizeAngles( QAngle& angles )
	{
		for ( auto i = 0; i < 3; i++ )
		{
			while ( angles[ i ] < -180.0f ) angles[ i ] += 360.0f;
			while ( angles[ i ] > 180.0f ) angles[ i ] -= 360.0f;
		}
	}

	void CalcAngle( Vector src, Vector dst, Vector &angles )
	{
		Vector delta = src - dst;
		double hyp = delta.Length2D( );
		angles.y = ( atan( delta.y / delta.x ) * 57.295779513082f );
		angles.x = ( atan( delta.z / hyp ) * 57.295779513082f );
		angles[ 2 ] = 0.00;

		if ( delta.x >= 0.0 ) angles.y += 180.0f;
	}

	QAngle VectorToQAngle( Vector in )
	{
		return QAngle( in.x, in.y, in.z );
	}

	Vector QAngleToVector( QAngle in )
	{
		return Vector( in.pitch, in.yaw, in.roll );
	}

	double deg2rad( double degrees )
	{
		static const double pi_on_180 = 4.0 * atan( 1.0 ) / 180.0;
		return degrees * pi_on_180;
	}

	void ClampAngles( QAngle& angles )
	{
		if ( angles.pitch > 89.0f ) angles.pitch = 89.0f;
		else if ( angles.pitch < -89.0f ) angles.pitch = -89.0f;

		if ( angles.yaw > 180.0f ) angles.yaw = 180.0f;
		else if ( angles.yaw < -180.0f ) angles.yaw = -180.0f;

		angles.roll = 0;
	}

	void VectorTransform( const Vector& in1, const matrix3x4_t& in2, Vector& out )
	{
		out[ 0 ] = in1.Dot( in2[ 0 ] ) + in2[ 0 ][ 3 ];
		out[ 1 ] = in1.Dot( in2[ 1 ] ) + in2[ 1 ][ 3 ];
		out[ 2 ] = in1.Dot( in2[ 2 ] ) + in2[ 2 ][ 3 ];
	}

	void AngleVectors( const QAngle &angles, Vector& forward )
	{
		float sp, sy, cp, cy;

		DirectX::XMScalarSinCos( &sp, &cp, DEG2RAD( angles[ 0 ] ) );
		DirectX::XMScalarSinCos( &sy, &cy, DEG2RAD( angles[ 1 ] ) );

		forward.x = cp*cy;
		forward.y = cp*sy;
		forward.z = -sp;
	}

	void AngleVectors( const QAngle &angles, Vector& forward, Vector& right, Vector& up )
	{
		float sr, sp, sy, cr, cp, cy;

		DirectX::XMScalarSinCos( &sp, &cp, DEG2RAD( angles[ 0 ] ) );
		DirectX::XMScalarSinCos( &sy, &cy, DEG2RAD( angles[ 1 ] ) );
		DirectX::XMScalarSinCos( &sr, &cr, DEG2RAD( angles[ 2 ] ) );

		forward.x = ( cp * cy );
		forward.y = ( cp * sy );
		forward.z = ( -sp );
		right.x = ( -1 * sr * sp * cy + -1 * cr * -sy );
		right.y = ( -1 * sr * sp * sy + -1 * cr *  cy );
		right.z = ( -1 * sr * cp );
		up.x = ( cr * sp * cy + -sr*-sy );
		up.y = ( cr * sp * sy + -sr*cy );
		up.z = ( cr * cp );
	}
	//--------------------------------------------------------------------------------
	void VectorAngles( const Vector& forward, QAngle& angles )
	{
		float tmp, yaw, pitch;

		if ( forward[ 1 ] == 0 && forward[ 0 ] == 0 )
		{
			yaw = 0;
			if ( forward[ 2 ] > 0 )
				pitch = 270;
			else
				pitch = 90;
		} else
		{
			yaw = ( atan2( forward[ 1 ], forward[ 0 ] ) * 180 / DirectX::XM_PI );
			if ( yaw < 0 )
				yaw += 360;

			tmp = sqrt( forward[ 0 ] * forward[ 0 ] + forward[ 1 ] * forward[ 1 ] );
			pitch = ( atan2( -forward[ 2 ], tmp ) * 180 / DirectX::XM_PI );
			if ( pitch < 0 )
				pitch += 360;
		}

		angles[ 0 ] = pitch;
		angles[ 1 ] = yaw;
		angles[ 2 ] = 0;
	}

	static bool screen_transform( const Vector& in, Vector& out )
	{
		static auto& w2sMatrix = g_EngineClient->WorldToScreenMatrix( );

		out.x = w2sMatrix.m[ 0 ][ 0 ] * in.x + w2sMatrix.m[ 0 ][ 1 ] * in.y + w2sMatrix.m[ 0 ][ 2 ] * in.z + w2sMatrix.m[ 0 ][ 3 ];
		out.y = w2sMatrix.m[ 1 ][ 0 ] * in.x + w2sMatrix.m[ 1 ][ 1 ] * in.y + w2sMatrix.m[ 1 ][ 2 ] * in.z + w2sMatrix.m[ 1 ][ 3 ];
		out.z = 0.0f;

		float w = w2sMatrix.m[ 3 ][ 0 ] * in.x + w2sMatrix.m[ 3 ][ 1 ] * in.y + w2sMatrix.m[ 3 ][ 2 ] * in.z + w2sMatrix.m[ 3 ][ 3 ];

		if ( w < 0.001f )
		{
			out.x *= 100000;
			out.y *= 100000;
			return false;
		}

		out.x /= w;
		out.y /= w;

		return true;
	}

	float DotProduct( const Vector &v1, const float *v2 )
	{
		return v1[ 0 ] * v2[ 0 ] + v1[ 1 ] * v2[ 1 ] + v1[ 2 ] * v2[ 2 ];
	}

	void MatrixSetColumn( const Vector &in, int column, matrix3x4_t& out )
	{
		out[ 0 ][ column ] = in.x;
		out[ 1 ][ column ] = in.y;
		out[ 2 ][ column ] = in.z;
	}

	void VectorRotate( const Vector &in1, const QAngle &in2, Vector &out )
	{
		matrix3x4_t matRotate;

		AngleMatrix( in2, matRotate );
		out[ 0 ] = DotProduct( in1, matRotate[ 0 ] );
		out[ 1 ] = DotProduct( in1, matRotate[ 1 ] );
		out[ 2 ] = DotProduct( in1, matRotate[ 2 ] );
	}

	void VectorRotate( const Vector &in1, matrix3x4_t matRotate, Vector &out )
	{
		out[ 0 ] = DotProduct( in1, matRotate[ 0 ] );
		out[ 1 ] = DotProduct( in1, matRotate[ 1 ] );
		out[ 2 ] = DotProduct( in1, matRotate[ 2 ] );
	}

	void MatrixCopy( const matrix3x4_t& src, matrix3x4_t& dst )
	{
		if ( &src != &dst )
			memcpy( dst.m_flMatVal, src.m_flMatVal, 16 * sizeof( float ) );
	}

	void SinCos( float radians, float *sine, float *cosine )
	{
		*sine = sin( radians );
		*cosine = cos( radians );
	}

	void AngleMatrix( const QAngle &angles, matrix3x4_t& matrix )
	{
		float sr, sp, sy, cr, cp, cy;

		SinCos( DEG2RAD( angles.yaw ), &sy, &cy );
		SinCos( DEG2RAD( angles.pitch ), &sp, &cp );
		SinCos( DEG2RAD( angles.roll ), &sr, &cr );

		matrix[ 0 ][ 0 ] = cp*cy;
		matrix[ 1 ][ 0 ] = cp*sy;
		matrix[ 2 ][ 0 ] = -sp;

		float crcy = cr*cy;
		float crsy = cr*sy;
		float srcy = sr*cy;
		float srsy = sr*sy;

		matrix[ 0 ][ 1 ] = sp*srcy - crsy;
		matrix[ 1 ][ 1 ] = sp*srsy + crcy;
		matrix[ 2 ][ 1 ] = sr*cp;

		matrix[ 0 ][ 2 ] = ( sp*crcy + srsy );
		matrix[ 1 ][ 2 ] = ( sp*crsy - srcy );
		matrix[ 2 ][ 2 ] = cr*cp;

		matrix[ 0 ][ 3 ] = 0.0f;
		matrix[ 1 ][ 3 ] = 0.0f;
		matrix[ 2 ][ 3 ] = 0.0f;
	}

	void AngleMatrix( const QAngle &angles, const Vector &position, matrix3x4_t& matrix )
	{
		AngleMatrix( angles, matrix );
		MatrixSetColumn( position, 3, matrix );
	}

	void MatrixMultiply( const matrix3x4_t& in1, const matrix3x4_t& in2, matrix3x4_t& out )
	{
		if ( &in1 == &out )
		{
			matrix3x4_t in1b;
			MatrixCopy( in1, in1b );
			MatrixMultiply( in1b, in2, out );
			return;
		}
		if ( &in2 == &out )
		{
			matrix3x4_t in2b;
			MatrixCopy( in2, in2b );
			MatrixMultiply( in1, in2b, out );
			return;
		}

		out[ 0 ][ 0 ] = in1[ 0 ][ 0 ] * in2[ 0 ][ 0 ] + in1[ 0 ][ 1 ] * in2[ 1 ][ 0 ] +
			in1[ 0 ][ 2 ] * in2[ 2 ][ 0 ];
		out[ 0 ][ 1 ] = in1[ 0 ][ 0 ] * in2[ 0 ][ 1 ] + in1[ 0 ][ 1 ] * in2[ 1 ][ 1 ] +
			in1[ 0 ][ 2 ] * in2[ 2 ][ 1 ];
		out[ 0 ][ 2 ] = in1[ 0 ][ 0 ] * in2[ 0 ][ 2 ] + in1[ 0 ][ 1 ] * in2[ 1 ][ 2 ] +
			in1[ 0 ][ 2 ] * in2[ 2 ][ 2 ];
		out[ 0 ][ 3 ] = in1[ 0 ][ 0 ] * in2[ 0 ][ 3 ] + in1[ 0 ][ 1 ] * in2[ 1 ][ 3 ] +
			in1[ 0 ][ 2 ] * in2[ 2 ][ 3 ] + in1[ 0 ][ 3 ];
		out[ 1 ][ 0 ] = in1[ 1 ][ 0 ] * in2[ 0 ][ 0 ] + in1[ 1 ][ 1 ] * in2[ 1 ][ 0 ] +
			in1[ 1 ][ 2 ] * in2[ 2 ][ 0 ];
		out[ 1 ][ 1 ] = in1[ 1 ][ 0 ] * in2[ 0 ][ 1 ] + in1[ 1 ][ 1 ] * in2[ 1 ][ 1 ] +
			in1[ 1 ][ 2 ] * in2[ 2 ][ 1 ];
		out[ 1 ][ 2 ] = in1[ 1 ][ 0 ] * in2[ 0 ][ 2 ] + in1[ 1 ][ 1 ] * in2[ 1 ][ 2 ] +
			in1[ 1 ][ 2 ] * in2[ 2 ][ 2 ];
		out[ 1 ][ 3 ] = in1[ 1 ][ 0 ] * in2[ 0 ][ 3 ] + in1[ 1 ][ 1 ] * in2[ 1 ][ 3 ] +
			in1[ 1 ][ 2 ] * in2[ 2 ][ 3 ] + in1[ 1 ][ 3 ];
		out[ 2 ][ 0 ] = in1[ 2 ][ 0 ] * in2[ 0 ][ 0 ] + in1[ 2 ][ 1 ] * in2[ 1 ][ 0 ] +
			in1[ 2 ][ 2 ] * in2[ 2 ][ 0 ];
		out[ 2 ][ 1 ] = in1[ 2 ][ 0 ] * in2[ 0 ][ 1 ] + in1[ 2 ][ 1 ] * in2[ 1 ][ 1 ] +
			in1[ 2 ][ 2 ] * in2[ 2 ][ 1 ];
		out[ 2 ][ 2 ] = in1[ 2 ][ 0 ] * in2[ 0 ][ 2 ] + in1[ 2 ][ 1 ] * in2[ 1 ][ 2 ] +
			in1[ 2 ][ 2 ] * in2[ 2 ][ 2 ];
		out[ 2 ][ 3 ] = in1[ 2 ][ 0 ] * in2[ 0 ][ 3 ] + in1[ 2 ][ 1 ] * in2[ 1 ][ 3 ] +
			in1[ 2 ][ 2 ] * in2[ 2 ][ 3 ] + in1[ 2 ][ 3 ];
	}

	bool WorldToScreen( const Vector& in, Vector& out )
	{
		if ( screen_transform( in, out ) )
		{
			int w = Hooks::width, h = Hooks::height;

			out.x = ( w / 2.0f ) + ( out.x * w ) / 2.0f;
			out.y = ( h / 2.0f ) - ( out.y * h ) / 2.0f;

			return true;
		}

		return false;
	}

}