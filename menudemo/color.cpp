#include "color.h"

#include <math.h>
Color Color::Black( 0, 0, 0, 255 );
Color Color::White( 255, 255, 255, 255 );
Color Color::Red( 255, 0, 0, 255 );
Color Color::Green( 0, 128, 0, 255 );
Color Color::Blue( 0, 0, 255, 255 );
Color Color::Cool( 231, 76, 60, 100 );
Color Color::Cool2( 46, 204, 113, 100 );
Color Color::Lime( 0, 255, 0 );
Color Color::Yellow( 255, 255, 0 );
Color Color::Cyan( 0, 255, 255 );
Color Color::Magenta( 255, 0, 255 );
Color Color::Silver( 192, 192, 192 );
Color Color::Gray( 128, 128, 128 );
Color Color::Maroon( 128, 0, 0 );
Color Color::Olive( 128, 128, 0 );
Color Color::Purple( 128, 0, 128 );
Color Color::Teal( 0, 128, 128 );
Color Color::Navy( 0, 0, 128 );
Color Color::DarkRed( 139, 0, 0 );
Color Color::Brown( 165, 42, 42 );
Color Color::Firebrick( 178, 34, 34 );
Color Color::Crimson( 220, 20, 60 );
Color Color::IndianRed( 205, 92, 92 );
Color Color::LightCoral( 240, 128, 128 );
Color Color::DarkSalmon( 233, 150, 122 );
Color Color::Salmon( 250, 128, 114 );
Color Color::LightSalmon( 255, 160, 122 );
Color Color::OrangeRed( 255, 69, 0 );
Color Color::DarkOrange( 255, 140, 0 );
Color Color::Orange( 255, 165, 0 );
Color Color::Gold( 255, 215, 0 );
Color Color::DarkGoldenRod( 184, 134, 11 );
Color Color::GoldenRod( 218, 165, 32 );
Color Color::YellowGreen( 154, 205, 50 );
Color Color::DarkOliveGreen( 85, 107, 47 );
Color Color::OliveDrab( 107, 142, 35 );
Color Color::LawnGreen( 124, 252, 0 );
Color Color::ChartReuse( 127, 255, 0 );
Color Color::GreenYellow( 173, 255, 47 );
Color Color::DarkGreen( 0, 100, 0 );
Color Color::ForestGreen( 34, 139, 34 );
Color Color::LimeGreen( 50, 205, 50 );
Color Color::DarkCyan( 0, 139, 139 );
Color Color::Aqua( 0, 255, 255 );
Color Color::LightCyan( 224, 255, 255 );
Color Color::DarkTurquoise( 0, 206, 209 );
Color Color::Turquoise( 64, 224, 208 );
Color Color::MediumTurquoise( 72, 209, 204 );
Color Color::PaleTurquoise( 175, 238, 238 );
Color Color::Aquamarine( 127, 255, 212 );
Color Color::PowderBlue( 176, 224, 230 );
Color Color::DodgerBlue( 30, 144, 255 );
Color Color::Lightblue( 173, 216, 230 );
Color Color::SkyBlue( 135, 206, 235 );
Color Color::LightSkyBlue( 135, 206, 250 );
Color Color::MidnightBlue( 25, 25, 112 );
Color Color::DarkBlue( 0, 0, 139 );
Color Color::MediumBlue( 0, 0, 205 );
Color Color::RoyalBlue( 65, 105, 225 );
Color Color::BlueViolet( 138, 43, 226 );
Color Color::Indigo( 75, 0, 130 );
Color Color::DarkSlateBlue( 72, 61, 139 );
Color Color::SlateBlue( 106, 90, 205 );
Color Color::MediumSlateBlue( 123, 104, 238 );
Color Color::MediumPurple( 147, 112, 219 );
Color Color::Darkmagenta( 139, 0, 139 );
Color Color::Darkviolet( 148, 0, 211 );
Color Color::DarkGray( 169, 169, 169 );
Color Color::LightGray( 211, 211, 211 );
Color Color::Gainsboro( 220, 220, 220 );


Color::Color( )
{
	*( (int *)this ) = 0;
}
Color::Color( int _r, int _g, int _b )
{
	SetColor( _r, _g, _b, 255 );
}
Color::Color( int _r, int _g, int _b, int _a )
{
	SetColor( _r, _g, _b, _a );
}
void Color::SetRawColor( int color32 )
{
	*( (int *)this ) = color32;
}
int Color::GetRawColor( ) const
{
	return *( (int *)this );
}
void Color::SetColor( int _r, int _g, int _b, int _a )
{
	_CColor[ 0 ] = (unsigned char)_r;
	_CColor[ 1 ] = (unsigned char)_g;
	_CColor[ 2 ] = (unsigned char)_b;
	_CColor[ 3 ] = (unsigned char)_a;
}
void Color::SetColor( float _r, float _g, float _b, float _a )
{
	_CColor[ 0 ] = static_cast<unsigned char>( _r * 255.0f );
	_CColor[ 1 ] = static_cast<unsigned char>( _g * 255.0f );
	_CColor[ 2 ] = static_cast<unsigned char>( _b * 255.0f );
	_CColor[ 3 ] = static_cast<unsigned char>( _a * 255.0f );
}
void Color::GetColor( int &_r, int &_g, int &_b, int &_a ) const
{
	_r = _CColor[ 0 ];
	_g = _CColor[ 1 ];
	_b = _CColor[ 2 ];
	_a = _CColor[ 3 ];
}
Color Color::Opposite( )
{
	return Color( abs( _CColor[ 0 ] + 255 - 510 ), abs( _CColor[ 1 ] + 255 - 510 ), abs( _CColor[ 2 ] + 255 - 510 ) );
}


float* Color::GetImVec( )
{
	float r = ( ( (float)_CColor[ 0 ] ) / 255.f );
	float g = ( ( (float)_CColor[ 1 ] ) / 255.f );
	float b = ( ( (float)_CColor[ 2 ] ) / 255.f );
	float a = ( ( (float)_CColor[ 3 ] ) / 255.f );
	float arr[ 4 ] = { r,g,b,a };
	float* pointer = arr;
	return pointer;
}
bool Color::operator== ( const Color &rhs ) const
{
	return ( *( (int *)this ) == *( (int *)&rhs ) );
}
bool Color::operator!= ( const Color &rhs ) const
{
	return !( operator==( rhs ) );
}
Color& Color::operator=( const Color &rhs )
{
	SetRawColor( rhs.GetRawColor( ) );
	return *this;
}
