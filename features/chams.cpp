#include "chams.hpp"
#include <fstream>

#include "../valve_sdk/csgostructs.hpp"
#include "../options.hpp"
#include "../hooks.hpp"
#include "../helpers/input.hpp"
#include "../Globals.h"
#include "../helpers/math.hpp"
#include "../helpers/utils.hpp"
#include <math.h>
#include <iterator>



chams::chams()
{
    std::ofstream("csgo\\materials\\simple_regular.vmt") << R"#("VertexLitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "0"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";

	std::ofstream( "csgo\\materials\\simple_regular_reflective.vmt" ) << R"#("VertexLitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "0"
  "$envmap"       "env_cubemap"
  "$normalmapalphaenvmapmask"  "1"
  "$envmapcontrast"             "1"
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";

	//  "$envmapsaturation"			"0.3"
	//  


	/*"$phong" "1"
		"$phongfresnelranges" "[0.5 0.75 1]"
		"$phongexponent" "25"
		"$phongboost"		"1"
		"$rimlight"               "1"
		"$rimlightexponent"       "5"
		"$rimlightboost"          "1"*/

	//  "$bumpmap"		"effects/blood" 
	//bumpmap = drastically changes the material etc

	std::ofstream( "csgo\\materials\\simple_regular_reflective2.vmt" ) << R"#("VertexLitGeneric"
{

  "$basetexture" "vgui/white_additive"
  "$ignorez"      "0"
  "$envmap"       "env_cubemap"
  "$bumpmap"		"effects/blood"
  "$normalmapalphaenvmapmask"  "1"
  "$envmapcontrast"             "1"
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";

    std::ofstream("csgo\\materials\\simple_ignorez.vmt") << R"#("VertexLitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "1"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";
    std::ofstream("csgo\\materials\\simple_flat.vmt") << R"#("UnlitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "0"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";
    std::ofstream("csgo\\materials\\simple_flat_ignorez.vmt") << R"#("UnlitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "1"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";


	//models\\inventory_items\\cologne_prediction\\cologne_prediction_glass
	
	//simple_regular
    materialRegular = g_MatSystem->FindMaterial("simple_regular_reflective", TEXTURE_GROUP_MODEL);
    materialRegularIgnoreZ = g_MatSystem->FindMaterial("simple_regular_reflective2", TEXTURE_GROUP_MODEL); //simple_ignorez
    materialFlatIgnoreZ = g_MatSystem->FindMaterial("simple_flat_ignorez", TEXTURE_GROUP_MODEL);
    materialFlat = g_MatSystem->FindMaterial("simple_flat", TEXTURE_GROUP_MODEL);
}

chams::~chams()
{
    std::remove("csgo\\materials\\simple_regular.vmt");
	std::remove( "csgo\\materials\\simple_regular_reflective.vmt" );
	std::remove( "csgo\\materials\\simple_regular_reflective2.vmt" );
    std::remove("csgo\\materials\\simple_ignorez.vmt");
    std::remove("csgo\\materials\\simple_flat.vmt");
    std::remove("csgo\\materials\\simple_flat_ignorez.vmt");
}

void chams::OverrideMaterial(bool ignoreZ, bool flat, bool wireframe, bool glass, const Color& rgba, bool nodraw, bool additive, IMaterial* custom)
{
    IMaterial* material = nullptr;

    if(flat)
	{
        if(ignoreZ)
            material = materialFlatIgnoreZ;
        else
            material = materialFlat;
    } else {
        if(ignoreZ)
            material = materialRegularIgnoreZ;
        else
            material = materialRegular;
    }


    if(glass) {
        material = materialFlat;
        material->AlphaModulate(0.45f);
    } else {
        material->AlphaModulate(
            rgba.a() / 255.0f);
    }
	
	if ( custom )
	{
		material = custom;
		material->AlphaModulate( rgba.a( ) / 255.0f );
	}
		

	material->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, nodraw);
    material->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, wireframe);
	material->SetMaterialVarFlag(MATERIAL_VAR_ADDITIVE, additive);
	
	
    material->ColorModulate(
        rgba.r() / 255.0f,
        rgba.g() / 255.0f,
        rgba.b() / 255.0f);



	//"$normalmapalphaenvmapmask"  "1"
	//	"$envmapcontrast"             "1"
	//XQZ set option
//	material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);

    g_MdlRender->ForcedMaterialOverride(material);
}
matrix3x4_t* oldmatrix;

void InitKeyValues(KeyValues* keyValues, const char* name)
{
	static DWORD initKeyValueAddr = 0;
	
	if (!initKeyValueAddr)initKeyValueAddr = (DWORD)Utils::PatternScan( LI_FIND_CACHED( GetModuleHandleA )("client.dll"), "68 ?? ?? ?? ?? 8B C8 E8 ?? ?? ?? ?? 89 45 FC EB 07 C7 45 ?? ?? ?? ?? ?? 8B 03 56*") + 7;

	static DWORD dwFunction = 0;

	
	if (!dwFunction) dwFunction = initKeyValueAddr + *reinterpret_cast<PDWORD_PTR>(initKeyValueAddr + 1) + 5;

	if (!dwFunction) 
		return;
	
	__asm
	{
		push name
		mov ecx, keyValues
		call dwFunction
	}
}

typedef bool(__thiscall *LoadFromBuffer_t)(KeyValues*, const char*, const char*, PVOID, PVOID, PVOID);
bool LoadFromBuffer(KeyValues *pThis, const char *pszFirst, const char *pszSecond, PVOID pSomething = 0, PVOID pAnother = 0, PVOID pLast = 0)
{

	static LoadFromBuffer_t callLoadBuff;
	
	if(!callLoadBuff) callLoadBuff = (LoadFromBuffer_t)Utils::PatternScan( LI_FIND_CACHED( GetModuleHandleA )("client.dll"), "55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89 4C 24 04");


	return callLoadBuff(pThis, pszFirst, pszSecond, pSomething, pAnother, pLast);
}

int created;
IMaterial* CreateMaterial(std::string mat_type, std::string base_material, bool ignorez, bool nofog)
{
	std::stringstream mat_data;

	mat_data << "\"" + mat_type + "\"\n"
		"{\n"
		"\t\"$surfaceprop\" Metal\r\n"
		"\t\"$envmap\" \"env_cubemap\"\r\n"
		"\t\"$phong\" \"1\"\r\n"
		"\t$phongfresnelranges \"[0.5 0.75 1]\"\r\n"
		"\t\"$basetexture\" \"" + base_material + "\"\n"
		"\t\"$ignorez\" \"" + std::to_string(ignorez) + "\"\n"
		"\t\"$nofog\" \"" + std::to_string(nofog) + "\"\n"
		"\t\"$model\" \"1\"\n"
		"\t\"$nocull\" \"1\"\n"
		"\t\"$halflambert\" \"1\"\n"
		"\t$phongexponent 0\r\n"
		"\t$rimlight 0\r\n"
		"\t$rimlightexponent 0\r\n"
		"\t$rimlightboost 0\r\n"
		"\t$selfillum 0\r\n"
		"\t$selfillum_envmapmask_alpha 1\r\n"
		"\t$selfillumtint \"[0.61 0.61 0.61]\"\r\n"

		"}\n" << std::flush;

	
	std::string matname = "chams_" + created;
	KeyValues* kv = new KeyValues(matname.c_str());
	InitKeyValues(kv, matname.c_str());
	LoadFromBuffer(kv, matname.c_str(), mat_data.str().c_str());
	created++;

	return g_MatSystem->CreateMaterial(matname.c_str(), kv);
}

 KeyValues* KeyValues_FindKey( KeyValues* key, const char *keyName, bool bCreate = false )
{
	static auto key_values_find_key = reinterpret_cast<KeyValues*( __thiscall* )( void*, const char*, bool )>( Utils::PatternSearch( "client.dll", ( PBYTE )"\x55\x8B\xEC\x83\xEC\x1C\x53\x8B\xD9\x85\xDB", "xxxxxxxxxxx", NULL, NULL ) );
	return key_values_find_key( key, keyName, bCreate );
}

 void KeyValues_SetInt( KeyValues* key, const char *keyName, int Value )
{
	auto key_int = KeyValues_FindKey( key, keyName );
	if ( key_int )
	{
		*reinterpret_cast<int*>( reinterpret_cast<uintptr_t>( key_int ) + 0xC ) = Value;
		*reinterpret_cast<char*>( reinterpret_cast<uintptr_t>( key_int ) + 0x10 ) = 2;
	}
}

 //"$envmapfresnel"             "0"
	// "$envmapsaturation"  "0"
	// "$envmapcontrast"             "0"
KeyValues* CreateKeyValue( const char* matname)
{

	std::stringstream mat_data;


	mat_data << R"#("VertexLitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "0"
  "$envmap"       "env_cubemap"
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";

	KeyValues* pKeyValues = new KeyValues( matname );
	InitKeyValues( pKeyValues, matname );
	LoadFromBuffer( pKeyValues, matname, mat_data.str().c_str( ) );

	return pKeyValues;
}

void EditKeyValue(KeyValues* kv, const char* matname, float envmapmask, float contrast , float reflection)
{

//	std::stringstream mat_data;
//
//
//	mat_data << R"#("VertexLitGeneric"
//{
//  "$basetexture" "vgui/white_additive"
//  "$ignorez"      "0"
//  "$envmap"       "env_cubemap"
//  "$normalmapalphaenvmapmask"  "1"
//  "$envmapcontrast"             "1"
//  "$nofog"        "1"
//  "$model"        "1"
//  "$nocull"       "0"
//  "$selfillum"    "1"
//  "$halflambert"  "1"
//  "$znearer"      "0"
//  "$flat"         "1"
//}
//)#";
//	

	//		"\t\"$envmaptint\" \"[1 0 0.5]\"\r\n"
	std::stringstream mat_data;
//	std::to_string( nofog )
	//		"\t\"$selfillummaskscale\" \"" + std::to_string( reflection ) + "\"\n"
	std::string asd = "VertexLitGeneric";
	mat_data << "\"" + asd + "\"\n"
		"{\n"
		"\t\"$basetexture\" vgui/white_additive\r\n"
		"\t\"$ignorez\" \"0\"\n"
		"\t\"$envmap\" \"env_cubemap\"\r\n"
		"\t\"$envmapsaturation\" \"" + std::to_string( envmapmask ) + "\"\n"
		"\t\"$envmapcontrast\" \"" + std::to_string( contrast ) + "\"\n"
		"\t\"$envmaplightscale\" \"" + std::to_string( reflection ) + "\"\n"
		"\t\"$nofog\" \"1\"\n"
		"\t\"$model\" \"1\"\n"
		"\t\"$nocull\" \"0\"\n"
		"\t\"$halflambert\" \"1\"\n"
		"\t$selfillum 1\r\n"
		"\t$znearer 0\r\n"
		"\t$flat 0\r\n"
		"}\n" << std::flush;
	InitKeyValues( kv, matname );
	LoadFromBuffer( kv, matname, mat_data.str( ).c_str( ) );


}
 float clamp(float val, float minVal, float maxVal)
{
	if (maxVal < minVal)
		return maxVal;
	else if (val < minVal)
		return minVal;
	else if (val > maxVal)
		return maxVal;
	else
		return val;
}

void invalidate(matrix3x4_t* matrix, int i) 
{
	matrix[i][0][0] = 0;
	matrix[i][1][0] = 0;
	matrix[i][2][0] = 0;

	matrix[i][0][1] = 0;
	matrix[i][1][1] = 0;
	matrix[i][2][1] = 0;

	matrix[i][0][2] = 0;
	matrix[i][1][2] = 0;
	matrix[i][2][2] = 0;
}

void invalidate2(matrix3x4_t* matrix, int ignore)
{
	for (int i = 0; i < MAXSTUDIOBONES; i++) 
		if (i != ignore) 
			invalidate(matrix, i); 
}
struct multicolor_t
{
	int bone = -1;
	Color color;
};


multicolor_t multicol[128];
void chams::OnDrawModelExecute(
    IMatRenderContext* ctx,
    const DrawModelState_t &state,
	const  ModelRenderInfo_t &info,
    matrix3x4_t *matrix
)
{
    static auto fnDME = Hooks::mdlrender_hook.get_original<Hooks::DrawModelExecute>(index::DrawModelExecute);

    const auto mdl = info.pModel;

    bool is_arm = strstr(mdl->szName, "arms")             != nullptr;
    bool is_player = strstr(mdl->szName, "models/player") != nullptr;
    bool is_sleeve = strstr(mdl->szName, "sleeve")        != nullptr;
	bool is_autism = strstr(mdl->szName, "weapons/v_rif_ak47") != nullptr;
    bool is_weapon = strstr(mdl->szName, "weapons/v_")  != nullptr;
	bool is_glove = strstr(mdl->szName, "glove") != nullptr;
	bool is_world = strstr(mdl->szName, "world") != nullptr;

		
		
	
	
	static IMaterial* bay_aqua = nullptr;

	if (!bay_aqua)
		bay_aqua = CreateMaterial("VertexLitGeneric", "models/weapons/akz", false, false);

	if (is_autism && bay_aqua) 
		g_MdlRender->ForcedMaterialOverride(bay_aqua);
	
	//IMaterial *mats[ 32 ];
	//studiohdr_t* hdr = Interface.ModelInfo->GetStudiomodel( pInfo.pModel );
	//Interface.ModelInfo->GetModelMaterials( pInfo.pModel, hdr->numtextures, mats );
	//for ( int m = 0; m < 2; m++ )
	//{
	//	bool ignore = m == 0;
	//	if ( ignore && Options::Visuals::Chams::VisibleOnly )
	//		continue;

	//	for ( int i = 0; i < hdr->numtextures; i++ )
	//	{
	//		IMaterial* mat = mats[ i ];
	//		if ( !mat )
	//			break;

	//		mat->SetMaterialVarFlag( MATERIAL_VAR_IGNOREZ, ignore );

	//		/*static KeyValues* visibleMat = 0;
	//		static KeyValues* invisibleMat = 0;
	//		if (!visibleMat)
	//		visibleMat = U::CreateKeyValue("temp_1", false, false);
	//		if (!invisibleMat)
	//		invisibleMat = U::CreateKeyValue("temp_2", false, true);

	//		if (mat->GetShaderName() != "VertexLitGeneric")
	//		mat->SetShaderAndParams(ignore ? invisibleMat : visibleMat);

	//		if (ignore) mat->ColorModulate(0, 0, 0); else mat->ColorModulate(0, 1, 0);*/
	//	}
	//	O::DrawModelExecute( thisptr, matctx, state, pInfo, pCustomBoneToWorld );
	//}
		
	static KeyValues* mememan = CreateKeyValue( "large_meme" );

	//TODO: make this actually not be shit so u can have slider for rim and shit like skeet
	if ( ( keyboard::get().key_down(KEY_F8) ) && materialRegular )
	{
		

		if ( mememan )
		{
			EditKeyValue( mememan, "large_meme", options.chams1, options.chams2, options.chams3);
		/*	KeyValues_SetInt( mememan, "normalmapalphaenvmapmask", 1 );
			KeyValues_SetInt( mememan, "envmapcontrast", 1 );*/
	/*		"$normalmapalphaenvmapmask"  "1"
				"$envmapcontrast"             "1"*/
			//mememan->GetName( ); // ( "2", 21 );
			//mememan->SetInt( "$envmapcontrast", 5 );
			materialRegular->SetShaderAndParams( mememan );
		}
	/*	bool found ;
		auto a = materialRegular->FindVar( "$envmapcontrast", &found, false );
		a->SetFloatValue( 0.1f );
		materialRegular->Refresh( );*/
		Utils::ConsolePrint( "DKDK" );
											// "$bumpmap"		"effects/blood"
	}
	
	//virtual void			SetShaderAndParams( KeyValues *pKeyValues ) = 0;

	//if (is_weapon && !is_glove && !is_autism && !is_sleeve)
	//{
	//	Color Rainbow = Hooks::rainbowCol;
	////	Color Opposite;

	////	Opposite.SetColor( );
	//	//Utils::ConsolePrint("%s \n", mdl->szName);
	//	OverrideMaterial(
	//		false,
	//		false,
	//		false,
	//		true,
	//		Rainbow); //toisin pai oikeestiHooks::pulse, Hooks::pulse, Hooks::pulse
	//	fnDME(g_MdlRender, ctx, state, info, matrix);
	//	OverrideMaterial
	//	(
	//		false,
	//		false,
	//		true,
	//		true,
	//		Rainbow.Opposite()
	//	);
	//}


    if(is_player && g_Options.chams_player_enabled)
	{
        // 
        // Draw player Chams.
        // 
        auto ent = C_BasePlayer::GetPlayerByIndex(info.entity_index);

	
		

        if(ent && g_LocalPlayer && ent->IsAlive())
		{

		

            const auto enemy = ent->m_iTeamNum() != g_LocalPlayer->m_iTeamNum();
            if(!enemy && g_Options.chams_player_enemies_only)
                return;

			
			const auto clr_front = enemy ? g_Options.color_chams_player_enemy_visible : g_Options.color_chams_player_ally_visible;
            const auto clr_back = enemy ? g_Options.color_chams_player_enemy_occluded : g_Options.color_chams_player_ally_occluded;


			if (options.fakechams_enabled && ent == local)
			{
				float fixYaw = 0.f;
				bool moving = local->m_vecVelocity().Length2D() > 1.f;

				if (options.fakechams_fix && !moving)
					fixYaw = Utils::FakeChamsFix(local->getanimstate());

				float flYawDelta2 = local->lby() - Hooks::realyaw;
				Utils::ClampYaw(flYawDelta2);

				bool lbyOK = fabsf(flYawDelta2) > 35.f;
				static float fake_yaw = 0.f;
				static float lby_yaw = 0.f; 
				
				matrix3x4_t anglefix_bones[MAXSTUDIOBONES];
				matrix3x4_t anglefix_angle;
				QAngle anglefix_qangle(0, fixYaw, 0);

				matrix3x4_t fake_bones[MAXSTUDIOBONES];
				matrix3x4_t fake_angle;
				QAngle fake_qangle(0, fake_yaw, 0);

				matrix3x4_t lby_bones[MAXSTUDIOBONES];
				matrix3x4_t lby_angle;
				QAngle lby_qangle(0, lby_yaw, 0);
				
				if (options.fakechams_fake)
				{
					Math::AngleMatrix(fake_qangle, fake_angle);
					fake_yaw = Hooks::fakeyaw - Hooks::lastmeme.y;
					Utils::ClampYaw(fake_yaw);
				}
			
				if (options.fakechams_lby) 
				{
					Math::AngleMatrix(lby_qangle, lby_angle);
					lby_yaw = g_Local->lby( ) - Hooks::lastmeme.y;
					Utils::ClampYaw(lby_yaw);	
				}
				
				if(options.fakechams_fix && fixYaw != 0.f)
				Math::AngleMatrix(anglefix_qangle, anglefix_angle);

				static Vector Out, bonepos;
				for (int i = 0; i < MAXSTUDIOBONES; i++)
				{
					bonepos = Vector(matrix[i][0][3], matrix[i][1][3], matrix[i][2][3]) - info.origin;
					
					if (options.fakechams_fix && fixYaw != 0.f)
					{
						Math::MatrixMultiply(anglefix_angle, matrix[i], anglefix_bones[i]);
						Math::VectorRotate(bonepos, anglefix_qangle, Out);
						Out += info.origin;
						anglefix_bones[i][0][3] = Out.x;
						anglefix_bones[i][1][3] = Out.y;
						anglefix_bones[i][2][3] = Out.z;
						matrix[i] = anglefix_bones[i];
					}

					bonepos = Vector(matrix[i][0][3], matrix[i][1][3], matrix[i][2][3]) - info.origin;

					if (options.fakechams_fake)
					{
						Math::MatrixMultiply(fake_angle, matrix[i], fake_bones[i]);
						Math::VectorRotate(bonepos, fake_angle, Out);		
						Out += Hooks::fakeanglesOrigin;
						fake_bones[i][0][3] = Out.x;
						fake_bones[i][1][3] = Out.y;
						fake_bones[i][2][3] = Out.z;
					}
					
					if (options.fakechams_lby && lbyOK)
					{
						Math::MatrixMultiply(lby_angle, matrix[i], lby_bones[i]);
						Math::VectorRotate(bonepos, lby_angle, Out);
						Out += Hooks::fakeanglesOrigin;
						lby_bones[i][0][3] = Out.x;
						lby_bones[i][1][3] = Out.y;
						lby_bones[i][2][3] = Out.z;
					}
				}

				if (options.fakechams_fake)
				{
					IMaterial* material = materialRegular;
					material->ColorModulate(
						options.color_fakechams_fake.r() / 255.0f,
						options.color_fakechams_fake.g() / 255.0f,
						options.color_fakechams_fake.b() / 255.0f);
					material->AlphaModulate(options.color_fakechams_fake.a() / 255.0f);
					g_MdlRender->ForcedMaterialOverride(material);
					fnDME(g_MdlRender, ctx, state, info, fake_bones);
				}

				if (options.fakechams_lby && lbyOK)
				{
					IMaterial* material = materialRegular;
					material->ColorModulate(
						options.color_fakechams_lby.r() / 255.0f,
						options.color_fakechams_lby.g() / 255.0f,
						options.color_fakechams_lby.b() / 255.0f);
					material->AlphaModulate(options.color_fakechams_lby.a() / 255.0f);
					g_MdlRender->ForcedMaterialOverride(material);
					fnDME(g_MdlRender, ctx, state, info, lby_bones);
				}
			}

				if (g_Options.chams_player_ignorez) 
				{

					OverrideMaterial(
						true,
						g_Options.chams_player_flat,
						g_Options.chams_player_wireframe,
						false,
						clr_front);
					
				}
				else 
				{
					

					
				
					
					if (ent == g_LocalPlayer && true == false)
					{ //epic priv multicolor chams 
					//	multicol[38].bone = 38;
					//	multicol[66].bone = 66;
					//	multicol[87].bone = 87;
					//	multicol[9].bone = 9;
						multicol[8].bone = 8;
						multicol[8].color = Color::White;
				/*		multicol[7].bone = 7;
						multicol[7].color = Color::MediumBlue;
						multicol[38].bone = 38;
						multicol[38].color = Color::IndianRed;
						multicol[39].bone = 39;
						multicol[39].color = Color::IndianRed;
						multicol[63].bone = 63;
						multicol[63].color = Color::IndianRed;
						multicol[64].bone = 64;
						multicol[64].color = Color::IndianRed;
						multicol[87].bone = 87;
						multicol[87].color = Color::Darkviolet;
						multicol[10].bone = 10;
						multicol[10].color = Color::Turquoise;
						multicol[6].bone = 6;
						multicol[6].color = Color::Orange;
					*/
						
						matrix3x4_t original[MAXSTUDIOBONES];
						for (int i = 0; i < MAXSTUDIOBONES; i++) 
							original[i] = matrix[i];
						
							
						

						for (int i = 0; i < MAXSTUDIOBONES; i++)
						{
							if (multicol[i].bone != -1)
							{
								matrix3x4_t copy2[MAXSTUDIOBONES];
							
								for (int i = 0; i < MAXSTUDIOBONES; i++)
									copy2[i] = original[i];
								

								invalidate2(copy2, i);	

								OverrideMaterial(
									false,
									g_Options.chams_player_flat,
									false,
									false,
									multicol[i].color);

								fnDME(g_MdlRender, ctx, state, info, copy2);
							}
						}

						
						OverrideMaterial(
							false,
							true,
							false,
							false,
							Color(0,0,0,133), false, false);
					
					}
					else 
					{
						OverrideMaterial(
							false,
							g_Options.chams_player_flat,
							g_Options.chams_player_wireframe,
							g_Options.chams_player_glass,
							clr_front);
						
					}
					
				
				


			}

			
        }
    }


 else if(is_sleeve && g_Options.chams_arms_enabled)
	{
        auto material = g_MatSystem->FindMaterial(mdl->szName, TEXTURE_GROUP_MODEL);
        if(!material)
            return;
        // 
        // Remove sleeves when drawing Chams.
        // 
        material->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
        g_MdlRender->ForcedMaterialOverride(material);
    } 
	else if(is_arm)
	{

        auto material = g_MatSystem->FindMaterial(mdl->szName, TEXTURE_GROUP_MODEL);

        if(!material)
            return;

		
		

        if(g_Options.misc_no_hands  && options.vis_misc_enabled)
		
		{
            // 
            // No hands.
            // 
            material->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
            g_MdlRender->ForcedMaterialOverride(material);
        }
		
		else if(g_Options.chams_arms_enabled)
		{
            if(g_Options.chams_arms_ignorez)
			{
                OverrideMaterial(
                    true,
                    g_Options.chams_arms_flat,
                    g_Options.chams_arms_wireframe,
                    false,
                    g_Options.color_chams_arms_occluded);
                fnDME(g_MdlRender, ctx, state, info, matrix);
                OverrideMaterial(
                    false,
                    g_Options.chams_arms_flat,
                    g_Options.chams_arms_wireframe,
                    false,
                    g_Options.color_chams_arms_visible);
            }
			else 
			{
                OverrideMaterial(
                    false,
                    g_Options.chams_arms_flat,
                    g_Options.chams_arms_wireframe,
                    g_Options.chams_arms_glass,
                    g_Options.color_chams_arms_visible);
				fnDME(g_MdlRender, ctx, state, info, matrix);
            }
        }
		
    }
}