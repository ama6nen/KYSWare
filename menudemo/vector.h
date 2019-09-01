#pragma once

#include <sstream>

static float Deg( float a )
{
	static float pi = 3.14159265358979323846264338327950288f;

	return a * ( pi / 180.f );
}
class Vector
{
public:
	Vector( void )
	{
		Invalidate( );
	}
	Vector( float X, float Y, float Z )
	{
		x = X;
		y = Y;
		z = Z;
	}
	Vector( const float* clr )
	{
		x = clr[ 0 ];
		y = clr[ 1 ];
		z = clr[ 2 ];
	}

	void Init( float ix = 0.0f, float iy = 0.0f, float iz = 0.0f )
	{
		x = ix; y = iy; z = iz;
	}
	bool IsValid( ) const
	{
		return std::isfinite( x ) && std::isfinite( y ) && std::isfinite( z );
	}
	void Invalidate( )
	{
		x = y = z = std::numeric_limits<float>::infinity( );
	}

	float& operator[]( int i )
	{
		return ( (float*)this )[ i ];
	}
	float operator[]( int i ) const
	{
		return ( (float*)this )[ i ];
	}
	inline Vector Cross( Vector a )
	{
		return Vector( y*a.z - z * a.y, z*a.x - x * a.z, x*a.y - y * a.x );
	}

	void Zero( )
	{
		x = y = z = 0.0f;
	}

	bool operator==( const Vector& src ) const
	{
		return ( src.x == x ) && ( src.y == y ) && ( src.z == z );
	}
	bool operator!=( const Vector& src ) const
	{
		return ( src.x != x ) || ( src.y != y ) || ( src.z != z );
	}

	Vector& operator+=( const Vector& v )
	{
		x += v.x; y += v.y; z += v.z;
		return *this;
	}
	Vector& operator-=( const Vector& v )
	{
		x -= v.x; y -= v.y; z -= v.z;
		return *this;
	}
	Vector& operator*=( float fl )
	{
		x *= fl;
		y *= fl;
		z *= fl;
		return *this;
	}
	Vector& operator*=( const Vector& v )
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}
	Vector& operator/=( const Vector& v )
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		return *this;
	}
	Vector& operator+=( float fl )
	{
		x += fl;
		y += fl;
		z += fl;
		return *this;
	}
	Vector& operator/=( float fl )
	{
		x /= fl;
		y /= fl;
		z /= fl;
		return *this;
	}
	Vector& operator-=( float fl )
	{
		x -= fl;
		y -= fl;
		z -= fl;
		return *this;
	}

	void NormalizeInPlace( )
	{
		*this = Normalized( );
	}
	Vector Normalized( ) const
	{
		Vector res = *this;
		float l = res.Length( );
		if ( l != 0.0f )
		{
			res /= l;
		} else
		{
			res.x = res.y = res.z = 0.0f;
		}
		return res;
	}

	Vector Sin( ) const
	{
		Vector vec = *this;
		Vector result;
		result.x = sinf( Deg( vec.x ) );
		result.y = sinf( Deg( vec.y ) );
		result.z = sinf( Deg( vec.z ) );
		return result;
	}

	Vector Cos( ) const
	{
		Vector vec = *this;
		Vector result;
		result.x = cosf( Deg( vec.x ) );
		result.y = cosf( Deg( vec.y ) );
		result.z = cosf( Deg( vec.z ) );
		return result;
	}

	Vector Forward( ) const
	{
		Vector vec = *this;
		Vector s = Sin( );
		Vector c = Cos( );
		Vector result;
		result.x = c.x*c.y;
		result.y = c.x*s.y;
		result.z = -s.x;
		return result;
	}

	Vector Right( ) const
	{
		Vector vec = *this;
		Vector s = Sin( );
		Vector c = Cos( );
		Vector result;
		result.x = -1 * s.z*s.x*c.y + -1 * c.z*-s.y;
		result.y = -1 * s.z*s.x*s.y + -1 * c.z*c.y;
		result.z = -1 * s.z*c.z;

		return result;
	}

	Vector Up( ) const
	{
		Vector vec = *this;
		Vector s = Sin( );
		Vector c = Cos( );
		Vector result;
		result.x = c.z*s.x*c.y + -s.z*-s.y;
		result.y = c.z*s.x*s.y + -s.z*c.y;
		result.z = c.z*c.x;
		return result;
	}
	float DistTo( const Vector &vOther ) const
	{
		Vector delta;

		delta.x = x - vOther.x;
		delta.y = y - vOther.y;
		delta.z = z - vOther.z;

		return delta.Length( );
	}
	float DistToSqr( const Vector &vOther ) const
	{
		Vector delta;

		delta.x = x - vOther.x;
		delta.y = y - vOther.y;
		delta.z = z - vOther.z;

		return delta.LengthSqr( );
	}
	float Dot( const Vector& vOther ) const
	{
		return ( x*vOther.x + y * vOther.y + z * vOther.z );
	}
	float Length( ) const
	{
		return sqrt( x*x + y * y + z * z );
	}
	float LengthSqr( void ) const
	{
		return ( x*x + y * y + z * z );
	}
	float Length2D( ) const
	{
		return sqrt( x*x + y * y );
	}

	Vector& operator=( const Vector &vOther )
	{
		x = vOther.x; y = vOther.y; z = vOther.z;
		return *this;
	}

	Vector operator-( void ) const
	{
		return Vector( -x, -y, -z );
	}
	Vector operator+( const Vector& v ) const
	{
		return Vector( x + v.x, y + v.y, z + v.z );
	}
	Vector operator-( const Vector& v ) const
	{
		return Vector( x - v.x, y - v.y, z - v.z );
	}
	Vector operator*( float fl ) const
	{
		return Vector( x * fl, y * fl, z * fl );
	}
	Vector operator*( const Vector& v ) const
	{
		return Vector( x * v.x, y * v.y, z * v.z );
	}
	Vector operator/( float fl ) const
	{
		return Vector( x / fl, y / fl, z / fl );
	}
	Vector operator/( const Vector& v ) const
	{
		return Vector( x / v.x, y / v.y, z / v.z );
	}

	float x, y, z;
};

inline Vector operator*( float lhs, const Vector& rhs )
{
	return rhs * lhs;
}
inline Vector operator/( float lhs, const Vector& rhs )
{
	return rhs / lhs;
}

class __declspec( align( 16 ) ) VectorAligned : public Vector
{
public:
	inline VectorAligned( void )
	{
	};
	inline VectorAligned( float X, float Y, float Z )
	{
		Init( X, Y, Z );
	}

public:
	explicit VectorAligned( const Vector &vOther )
	{
		Init( vOther.x, vOther.y, vOther.z );
	}

	VectorAligned& operator=( const Vector &vOther )
	{
		Init( vOther.x, vOther.y, vOther.z );
		return *this;
	}

	VectorAligned& operator=( const VectorAligned &vOther )
	{
		Init( vOther.x, vOther.y, vOther.z );
		return *this;
	}

	float w;
};