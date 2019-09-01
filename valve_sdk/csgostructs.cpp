#include "csgostructs.hpp"
#include "../Helpers/Math.hpp"
#include "../Helpers/Utils.hpp"
#include "../lazy_importer.h"
bool C_BaseEntity::IsPlayer()
{
    //index: 152
    //ref: "effects/nightvision"
    //sig: 8B 92 ? ? ? ? FF D2 84 C0 0F 45 F7 85 F6
    return CallVFunction<bool(__thiscall*)(C_BaseEntity*)>(this, 152)(this);
}

CBasePlayerAnimState* C_BaseEntity::getanimstate()
{
	return (*(CBasePlayerAnimState**)((DWORD)this + 0x3874)); //update offset 
}



void C_BaseEntity::SetModelIndex(int nModelIndex)
{
	typedef void(__thiscall* oSetModelIndex)(PVOID, int);
	return CallVFunction< oSetModelIndex >(this, 75)(this, nModelIndex);
}

bool C_BaseEntity::IsWeapon()
{
    //index: 160
    //ref: "CNewParticleEffect::DrawModel"
    //sig: 8B 80 ? ? ? ? FF D0 84 C0 74 6F 8B 4D A4
    return CallVFunction<bool(__thiscall*)(C_BaseEntity*)>(this, 160)(this);
}

bool C_BaseEntity::IsPlantedC4()
{
    return GetClientClass()->m_ClassID == ClassId_CPlantedC4;
}

bool C_BaseEntity::IsDefuseKit()
{
    return GetClientClass()->m_ClassID == ClassId_CBaseAnimating;
}

void C_BaseCombatWeapon::SetWeaponModel(const char* Filename)
{

	typedef void(__thiscall* tSetWeaponModel)(void*, const char*, C_BaseCombatWeapon*);
	return CallVFunction<tSetWeaponModel>(this, 242)(this, Filename, this);
}

CEconItemDefinition* C_EconItemView::GetStaticData( )
{
	static auto fnGetStaticData
		= reinterpret_cast<CEconItemDefinition*( __thiscall* )( void* )>(
			Utils::PatternScan( LI_FIND_CACHED( GetModuleHandleA )( "client.dll" ), "55 8B EC 51 56 57 8B F1 E8 ? ? ? ? 0F B7 8E" )
			);
	return fnGetStaticData( this );
}

CEconItem* C_EconItemView::GetSOCData( )
{
	static auto fnGetSOCData
		= reinterpret_cast<CEconItem*( __thiscall* )( C_EconItemView* )>(
			Utils::PatternScan( LI_FIND_CACHED( GetModuleHandleA )( "client.dll" ), "55 8B EC 83 E4 F0 83 EC 18 56 8B F1 57 8B 86" )
			);

	return fnGetSOCData( this );
}

C_EconItemView* CPlayerInventory::GetItemInLoadout( int team, int slot )
{
	typedef C_EconItemView*( __thiscall* tOriginal )( void*, unsigned int, signed int );
	return CallVFunction<tOriginal>( this, 8 )( this, team, slot );
}


int CEconItemDefinition::GetEquippedPosition( )
{
	return *reinterpret_cast<int*>( (uintptr_t)this + 0x24C );
}
C_EconItemView* C_BaseCombatWeapon::GetEconItemView( )
{
	static auto Address = Utils::PatternScan( LI_FIND_CACHED( GetModuleHandleA )("client.dll"), "E8 ? ? ? ? 89 44 24 28 85 C0 0F 84 ? ? ? ? 8B C8" );
	static auto fnGetEconItemView
		= reinterpret_cast<C_EconItemView*( __thiscall* )( void* )>(
			*reinterpret_cast<uintptr_t*>( Address + 1 ) + Address + 5
			);
	
	return fnGetEconItemView( this );
}



Vector C_BasePlayer::WorldSpaceCenter()
{
	Vector mins, maxs;
	this->GetRenderBounds(mins, maxs);
	Vector Max = maxs + this->m_vecOrigin();
	Vector Min = mins + this->m_vecOrigin();

	Vector Size = Max - Min;

	Size /= 2;

	Size += Min;

	return Size;
}
CCSWeaponInfo* C_BaseCombatWeapon::GetCSWeaponData()
{

    static auto fnGetWpnData
        = reinterpret_cast<CCSWeaponInfo*(__thiscall*)(void*)>(
            Utils::PatternScan( LI_FIND_CACHED( GetModuleHandleW )(L"client.dll"), "55 8B EC 81 EC ? ? ? ? 53 8B D9 56 57 8D 8B")
            );

    return fnGetWpnData(this);
	
}

bool C_BaseCombatWeapon::HasBullets()
{
    return !IsReloading() && m_iClip1() > 0;
}

bool C_BaseCombatWeapon::CanFire()
{
    if(IsReloading() || m_iClip1() <= 0)
        return false;

    if(!g_LocalPlayer)
        return false;

    float flServerTime = g_LocalPlayer->m_nTickBase() * g_GlobalVars->interval_per_tick;

    return m_flNextPrimaryAttack() <= flServerTime;
}

bool C_BaseCombatWeapon::IsGrenade()
{
    return GetCSWeaponData()->m_WeaponType == WEAPONTYPE_GRENADE;
}

//bool C_BaseCombatWeapon::IsKnife()
//{
//    return GetCSWeaponData()->m_WeaponType == WEAPONTYPE_KNIFE;
//}
bool C_BaseCombatWeapon::IsKnife( )
{
	int i = this->m_iItemDefinitionIndex( );
	return ( i >= WEAPON_KNIFE_BAYONET && i < 5027 ) || i == WEAPON_KNIFE_T || i == WEAPON_KNIFE;
}
bool C_BaseCombatWeapon::IsRifle()
{
	return GetCSWeaponData( )->m_WeaponType == WEAPONTYPE_RIFLE;
}

bool C_BaseCombatWeapon::IsSMG( )
{
	return GetCSWeaponData( )->m_WeaponType == WEAPONTYPE_SUBMACHINEGUN;
}

bool C_BaseCombatWeapon::IsHeavy( )
{
	return GetCSWeaponData( )->m_WeaponType == WEAPONTYPE_MACHINEGUN;
}

bool C_BaseCombatWeapon::IsShotgun( )
{
	return GetCSWeaponData( )->m_WeaponType == WEAPONTYPE_SHOTGUN;
}

bool C_BaseCombatWeapon::IsPistol()
{
    switch (GetCSWeaponData()->m_WeaponType)
    {
    case WEAPONTYPE_PISTOL:
        return true;
    default:
        return false;
    }
}

bool C_BaseCombatWeapon::IsSniper()
{
	
    switch (GetCSWeaponData()->m_WeaponType)
    {
    case WEAPONTYPE_SNIPER_RIFLE:
        return true;
    default:
        return false;
    }
}


bool C_BaseCombatWeapon::IsReloading()
{
    static auto inReload = *(uint32_t*)(Utils::PatternScan( LI_FIND_CACHED( GetModuleHandleW )(L"client.dll"), "C6 87 ? ? ? ? ? 8B 06 8B CE FF 90") + 2);
    return *(bool*)((uintptr_t)this + inReload);
}

float C_BaseCombatWeapon::GetInaccuracy()
{
    return CallVFunction<float(__thiscall*)(void*)>(this, 468)(this);
}
//CBaseWeapon::GetName: 376
float C_BaseCombatWeapon::GetSpread()
{
    return CallVFunction<float(__thiscall*)(void*)>(this, 437)(this);
}
void C_BaseCombatWeapon::DrawCrosshair()
{
	CallVFunction<void(__thiscall*)(void*)>(this, 391)(this);
}

void C_BaseCombatWeapon::UpdateAccuracyPenalty()
{
    CallVFunction<void(__thiscall*)(void*)>(this, 469)(this);
}


CUserCmd*& C_BasePlayer::m_pCurrentCommand()
{
    static auto currentCommand = *(uint32_t*)(Utils::PatternScan( LI_FIND_CACHED(GetModuleHandleW)(L"client.dll"), "89 BE ? ? ? ? E8 ? ? ? ? 85 FF") + 2);
    return *(CUserCmd**)((uintptr_t)this + currentCommand);
}

AnimationLayer C_BasePlayer::GetAnimOverlay(int index) 
{
	return (*(AnimationLayer**)((DWORD)this + 0x2970))[index];
}

int C_BasePlayer::GetSequenceActivity(int sequence)
{
	auto hdr = g_MdlInfo->GetStudiomodel(this->GetModel());

	if (!hdr)
		return -1;

	// c_csplayer vfunc 242, follow calls to find the function.
	static auto GetSequenceActivityc = reinterpret_cast<int(__fastcall*)(void*, studiohdr_t*, int)>(Utils::PatternScan( LI_FIND_CACHED( GetModuleHandleW )(L"client.dll"), "55 8B EC 83 7D 08 FF 56 8B F1 74 3D"));


	return GetSequenceActivityc(this, hdr, sequence);
}
void C_BasePlayer::SetAbsAngles(Vector angle)
{
	typedef void(__thiscall* SetAngleFn)(void*, const Vector &);
	static SetAngleFn SetAngle = (SetAngleFn)(Utils::PatternScan( LI_FIND_CACHED( GetModuleHandleW )(L"client.dll"), "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1"));
	
	if (SetAngle)
		SetAngle(this, angle);
	else
		Utils::ConsolePrint("setabsangles null...");
}

void C_BasePlayer::SetAbsOrigin(Vector origin)
{
	typedef void(__thiscall* SetOriginFn)(void*, const Vector &);
	static SetOriginFn SetOrigin = (SetOriginFn)(Utils::PatternScan( LI_FIND_CACHED( GetModuleHandleW )(L"client.dll"), "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8"));

	if(SetOrigin)
		SetOrigin(this, origin);
	else
		Utils::ConsolePrint("setabsorigin null...");
}

bool C_BasePlayer::InLadderOrNoclip() 
{
	return (GetMoveType() == MOVETYPE_LADDER || GetMoveType() == MOVETYPE_NOCLIP);
}

bool C_BasePlayer::Frozen( )
{
	return ( m_fFlags( ) & FL_FROZEN);
}

Vector C_BasePlayer::GetEyePos()
{
    return m_vecOrigin() + m_vecViewOffset();
}


MoveType_t C_BaseEntity::GetMoveType()
{
	return *(MoveType_t*)((DWORD)this + 0x258);
}


enum DrawModelFlags_t
{
	STUDIO_NONE = 0x00000000,
	STUDIO_RENDER = 0x00000001,
	STUDIO_VIEWXFORMATTACHMENTS = 0x00000002,
	STUDIO_DRAWTRANSLUCENTSUBMODELS = 0x00000004,
	STUDIO_TWOPASS = 0x00000008,
	STUDIO_STATIC_LIGHTING = 0x00000010,
	STUDIO_WIREFRAME = 0x00000020,
	STUDIO_ITEM_BLINK = 0x00000040,
	STUDIO_NOSHADOWS = 0x00000080,
	STUDIO_WIREFRAME_VCOLLIDE = 0x00000100,
	STUDIO_NOLIGHTING_OR_CUBEMAP = 0x00000200,
	STUDIO_SKIP_FLEXES = 0x00000400,
	STUDIO_DONOTMODIFYSTENCILSTATE = 0x00000800,
	// Not a studio flag, but used to flag model as a non-sorting brush model
	STUDIO_TRANSPARENCY = 0x80000000,
	// Not a studio flag, but used to flag model as using shadow depth material override
	STUDIO_SHADOWDEPTHTEXTURE = 0x40000000,
	// Not a studio flag, but used to flag model as doing custom rendering into shadow texture
	STUDIO_SHADOWTEXTURE = 0x20000000,
	STUDIO_SKIP_DECALS = 0x10000000,
};


void C_BasePlayer::DrawModel2(uint8_t alpha)
{
	
	using fn = int(__thiscall*)(void*, int, uint8_t);
	CallVFunction< fn >(this->GetRenderable(), 9)(GetRenderable(), 0x1, alpha);
}
player_info_t C_BasePlayer::GetPlayerInfo()
{
    player_info_t info;
    g_EngineClient->GetPlayerInfo(EntIndex(), &info);
    return info;
}

bool C_BasePlayer::IsAlive()
{
    return m_lifeState() == LIFE_ALIVE;
}

bool C_BasePlayer::IsEnemy()
{
	return (this->m_iTeamNum() != local->m_iTeamNum());
}
bool C_BasePlayer::isvalidtarget()
{
	return (IsAlive() && IsPlayer() && !IsDormant() && this->GetClientClass()->m_ClassID == (int)ClassId::ClassId_CCSPlayer);
}

bool C_BasePlayer::HasC4()
{
    static auto fnHasC4
        = reinterpret_cast<bool(__thiscall*)(void*)>(
            Utils::PatternScan( LI_FIND_CACHED( GetModuleHandleW )(L"client.dll"), "56 8B F1 85 F6 74 31")
            );

    return fnHasC4(this);
}

Vector C_BasePlayer::GetHitboxPos(int hitbox_id)
{
    matrix3x4_t boneMatrix[MAXSTUDIOBONES];

    if(SetupBones(boneMatrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0.0f)) 
	{
        auto studio_model = g_MdlInfo->GetStudiomodel(GetModel());
        if(studio_model) {
            auto hitbox = studio_model->GetHitboxSet(0)->GetHitbox(hitbox_id);
            if(hitbox) {
                auto
                    min = Vector{},
                    max = Vector{};

                Math::VectorTransform(hitbox->bbmin, boneMatrix[hitbox->bone], min);
                Math::VectorTransform(hitbox->bbmax, boneMatrix[hitbox->bone], max);

                return (min + max) / 2.0f;
            }
        }
    }
    return Vector{};
}

Vector C_BasePlayer::GetHitboxPosFPS( matrix3x4_t* matrix, mstudiohitboxset_t* set, int hitbox_id )
{
	auto hitbox = set->GetHitbox( hitbox_id );
	if ( hitbox )
	{
		auto
			min = Vector{ },
			max = Vector{ };

		Math::VectorTransform( hitbox->bbmin, matrix[ hitbox->bone ], min );
		Math::VectorTransform( hitbox->bbmax, matrix[ hitbox->bone ], max );

		return ( min + max ) / 2.0f;

	}
	return Vector{ };
}


bool C_BasePlayer::GetHitgroupPos(int hitgroup, Vector &output)
{
	const model_t *model = this->GetModel();
	if (!model)
		return false;

	studiohdr_t *studioHdr = g_MdlInfo->GetStudiomodel(model);
	if (!studioHdr)
		return false;

	matrix3x4_t matrix[MAXSTUDIOBONES];
	if (!this->SetupBones(matrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0))
		return false;

	mstudiohitboxset_t* set = studioHdr->GetHitboxSet(this->m_nHitboxSet());
	mstudiobbox_t* box;
	for (int i = 0; i < set->numhitboxes; i++)
	{
		box = set->GetHitbox(i);
		if (box->group == hitgroup)
			break;
	}

	Vector min, max;

	Math::VectorTransform(box->bbmin, matrix[box->bone], min);
	Math::VectorTransform(box->bbmax, matrix[box->bone], max);

	output = (min + max) * 0.5f;

	return true;
}

bool C_BasePlayer::GetHitboxPos(int hitbox, Vector &output)
{
    if(hitbox >= HITBOX_MAX)
        return false;

    const model_t *model = this->GetModel();
    if(!model)
        return false;

	   auto studio_model = g_MdlInfo->GetStudiomodel(GetModel());
    studiohdr_t *studioHdr = g_MdlInfo->GetStudiomodel(model);
    if(!studioHdr)
        return false;

    matrix3x4_t matrix[MAXSTUDIOBONES];
    if(!this->SetupBones(matrix, MAXSTUDIOBONES, 0x100, 0))
        return false;

    mstudiobbox_t *studioBox = studioHdr->GetHitboxSet(0)->GetHitbox(hitbox);
    if(!studioBox)
        return false;

    Vector min, max;

    Math::VectorTransform(studioBox->bbmin, matrix[studioBox->bone], min);
    Math::VectorTransform(studioBox->bbmax, matrix[studioBox->bone], max);

    output = (min + max) * 0.5f;

    return true;
}

Vector C_BasePlayer::GetBonePos(int bone)
{
    matrix3x4_t boneMatrix[MAXSTUDIOBONES];

    if(SetupBones(boneMatrix, MAXSTUDIOBONES, BONE_USED_BY_ANYTHING, 0.0f)) {
        return boneMatrix[bone].at(3);
    }
    return Vector{};
}


bool C_BasePlayer::CanSeePlayer(C_BasePlayer* player, int hitbox)
{
    CGameTrace tr;
    Ray_t ray;
    CTraceFilter filter;
    filter.pSkip = this;

    auto endpos = player->GetHitboxPos(hitbox);

    ray.Init(GetEyePos(), endpos);
    g_EngineTrace->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr);

    return tr.hit_entity == player || tr.fraction > 0.97f;
}
//bool& IMaterialSystem::gameStarted( )
//{
//	static bool asd = *(bool*)( Utils::PatternScan( GetModuleHandleA( "materialsystem.dll" ), "80 B9 ? ? ? ? ? 74 0F" ) + 2 );
//	return asd;
//}
float C_BasePlayer::FovToPlayer(int hitbox)
{
	static const float max = 180.0f;
	Vector delta(0, 0, 0);

	Math::VectorSubtract(this->GetHitboxPos(hitbox), g_EyePos, delta);
	Math::Normalize(delta, delta);

	return (acos(g_ViewAngles.Forward().Dot(delta)) * (max / M_PI));
}



bool C_BasePlayer::CanSeePlayer(C_BasePlayer* player, const Vector& pos)
{
    CGameTrace tr;
    Ray_t ray;
    CTraceFilter filter;
    filter.pSkip = this;

    auto start = GetEyePos();
    auto dir = (pos - start).Normalized();

    ray.Init(start, pos);
    g_EngineTrace->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr);

    return tr.hit_entity == player || tr.fraction > 0.97f;
}

