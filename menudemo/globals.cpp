#include "Globals.h"
namespace Hooks
{
	int width, height;
	Color rainbowCol;
	bool drawghost;
	bool SetNewColor;
	char clientname[ 128 ];
	char clientname_other[ 128 ];
	char clientpic[ 18 ] = "76561198316747508";
	float fakeyaw = 0.f;
	float realyaw = 0.f;
	Vector lastmeme;
	float viewyaw = 0.f;
	bool autoaaleft = false;
	bool inputlisten;
	int* input_receivedKeyval;
	IDirect3DTexture9* byteImage = nullptr;
	Vector fakeanglesOrigin = Vector( 0, 0, 0 );

}