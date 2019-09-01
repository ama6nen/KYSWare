#pragma once
#include <math.h>
#include "../valve_sdk/sdk.hpp"

#include <DirectXMath.h>

#define RAD2DEG(x) DirectX::XMConvertToDegrees(x)
#define DEG2RAD(x) DirectX::XMConvertToRadians(x)
#define M_PI   3.14159265358979323846264338327950288
#define XM_2PI 6.283185307f
#define deg(a) a*(M_PI/180)
#define TICKS_TO_TIME(t) (global->interval_per_tick * (t) )
#define TIME_TO_TICKS( dt )	( ( int )( 0.5f + ( float )( dt ) / global->interval_per_tick ) )

namespace Math
{

	inline float BOG_TO_GRD(float BOG) 
	{
		return (180 / M_PI) * BOG;
	}

	inline float GRD_TO_BOG(float GRD)
	{
		return (M_PI / 180) * GRD;
	}

	double deg2rad(double degrees);

	float RandomFloat(float min, float max);

	void RandomSeed(int seed);

	void CalcAngle(Vector src, Vector dst, Vector &angles);

	QAngle VectorToQAngle(Vector in);

	Vector QAngleToVector(QAngle in);

	void NormaliseViewAngle(Vector &angle);

	void VectorSubtract(const Vector& a, const Vector& b, Vector& c);

	void Normalize(Vector &vIn, Vector &vOut);

    void NormalizeAngles(QAngle& angles);

    void ClampAngles(QAngle& angles);

    void VectorTransform(const Vector& in1, const matrix3x4_t& in2, Vector& out);

    void AngleVectors(const QAngle &angles, Vector& forward);

    void AngleVectors(const QAngle &angles, Vector& forward, Vector& right, Vector& up);

    void VectorAngles(const Vector& forward, QAngle& angles);

    bool WorldToScreen(const Vector& in, Vector& out);

	void MatrixMultiply(const matrix3x4_t& in1, const matrix3x4_t& in2, matrix3x4_t& out);

	void SinCos(float radians, float *sine, float *cosine);

	void AngleMatrix(const QAngle &angles, matrix3x4_t& matrix);

	void AngleMatrix(const QAngle &angles, const Vector &position, matrix3x4_t& matrix);

	void MatrixCopy(const matrix3x4_t& src, matrix3x4_t& dst);

	void VectorRotate(const Vector &in1, const QAngle &in2, Vector &out);

	void VectorRotate(const Vector &in1, matrix3x4_t matRotate, Vector &out);

	float DotProduct(const Vector &v1, const float *v2);

	void MatrixSetColumn(const Vector &in, int column, matrix3x4_t& out);

}