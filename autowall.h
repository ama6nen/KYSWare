#pragma once
#include "Globals.h"
#include "valve_sdk\sdk.hpp"
#include "helpers\math.hpp"
#include "helpers\utils.hpp"
#include "valve_sdk\csgostructs.hpp"
#include <math.h>
#include <stdio.h>
#include <algorithm>
#define        HITGROUP_GENERIC    0
#define        HITGROUP_HEAD       1
#define        HITGROUP_CHEST      2
#define        HITGROUP_STOMACH    3
#define        HITGROUP_LEFTARM    4
#define        HITGROUP_RIGHTARM   5
#define        HITGROUP_LEFTLEG    6
#define        HITGROUP_RIGHTLEG   7
#define        HITGROUP_GEAR       10

enum Collision_Group_t
{
	COLLISION_GROUP_NONE = 0,
	COLLISION_GROUP_DEBRIS,			// Collides with nothing but world and static stuff
	COLLISION_GROUP_DEBRIS_TRIGGER, // Same as debris, but hits triggers
	COLLISION_GROUP_INTERACTIVE_DEBRIS,	// Collides with everything except other interactive debris or debris
	COLLISION_GROUP_INTERACTIVE,	// Collides with everything except interactive debris or debris
	COLLISION_GROUP_PLAYER,
	COLLISION_GROUP_BREAKABLE_GLASS,
	COLLISION_GROUP_VEHICLE,
	COLLISION_GROUP_PLAYER_MOVEMENT,  // For HL2, same as Collision_Group_Player, for
									  // TF2, this filters out other players and CBaseObjects
									  COLLISION_GROUP_NPC,			// Generic NPC group
									  COLLISION_GROUP_IN_VEHICLE,		// for any entity inside a vehicle
									  COLLISION_GROUP_WEAPON,			// for any weapons that need collision detection
									  COLLISION_GROUP_VEHICLE_CLIP,	// vehicle clip brush to restrict vehicle movement
									  COLLISION_GROUP_PROJECTILE,		// Projectiles!
									  COLLISION_GROUP_DOOR_BLOCKER,	// Blocks entities not permitted to get near moving doors
									  COLLISION_GROUP_PASSABLE_DOOR,	// Doors that the player shouldn't collide with
									  COLLISION_GROUP_DISSOLVING,		// Things that are dissolving are in this group
									  COLLISION_GROUP_PUSHAWAY,		// Nonsolid on client and server, pushaway in player code

									  COLLISION_GROUP_NPC_ACTOR,		// Used so NPCs in scripts ignore the player.
									  COLLISION_GROUP_NPC_SCRIPTED,	// USed for NPCs in scripts that should not collide with each other
									  COLLISION_GROUP_PZ_CLIP,



									  COLLISION_GROUP_DEBRIS_BLOCK_PROJECTILE, // Only collides with bullets

									  LAST_SHARED_COLLISION_GROUP
};


struct FireBulletData
{
	FireBulletData(const Vector& eye_pos) : src(eye_pos)
	{
	}

	Vector src;
	trace_t enter_trace;
	Vector direction;
	CTraceFilter filter;
	float trace_length;
	float trace_length_remaining;
	float current_damage;
	int penetrate_count;
};
class Awall
{
public:
	static float MultiplyDamage(int iHitGroup)
	{
		switch (iHitGroup)
		{
			case HITGROUP_HEAD:
				return 4.0f;
			
			case HITGROUP_STOMACH:
				return 1.25f;
			
			case HITGROUP_LEFTLEG:
			case HITGROUP_RIGHTLEG:
				return 0.75f;
			
		}

		return 1.0f;
	}

	static void ScaleDamage(int hitgroup, C_BasePlayer* enemy, float weapon_armor_ratio, float& current_damage)
	{
		current_damage *= MultiplyDamage(hitgroup);

		float armor = (float)enemy->m_ArmorValue();

		if (armor > 0.f && hitgroup < HITGROUP_LEFTLEG)
		{
			if (hitgroup == HITGROUP_HEAD && enemy->m_bHasHelmet())
				current_damage *= (weapon_armor_ratio);
			else
			{
				float armorscaled = (weapon_armor_ratio * 0.5f) * current_damage;

				if ((current_damage - armorscaled) * 0.5f > armor)
					armorscaled = current_damage - (armor * 2.0f);

				current_damage = armorscaled;

			//	current_damage *= (weapon_armor_ratio);
			}
		}
	}
	void FixTraceRay(Vector end, Vector start, trace_t* oldtrace, C_BasePlayer* ent) {
		auto mins = ent->m_vecMins();
		auto maxs = ent->m_vecMaxs();

		auto CenterOfBBOX = (maxs + mins) * 0.5f;
		auto origin = ent->GetRenderOrigin();

		Vector CenterOfEntityInWorld = (CenterOfBBOX + origin);

		Vector delta(end - start);
		delta.NormalizeInPlace();

		auto v20 = (CenterOfEntityInWorld - start);
		auto ForwardDot = delta.Dot(v20);

		float v23;

		if (ForwardDot >= 0.0f) {
			//Player is at or in front
			if (ForwardDot <= delta.Length())
			{
				auto fuck(CenterOfEntityInWorld - ((delta * ForwardDot) + start));
				v23 = fuck.Length();
			}
			else
			{
				auto lol(CenterOfEntityInWorld - end);
				v23 = -lol.Length();
			}
		}

		else
			v23 = -v20.Length();

		if (v23 <= 60.f) {
			Ray_t ray;
			ray.Init(start, end);

			trace_t trace;
			enginetrace->ClipRayToEntity(ray, 0x4600400B, (IHandleEntity*)ent, &trace);

			if (oldtrace->fraction > trace.fraction)
				*oldtrace = trace;
		}
	}

	enum
	{
		CHAR_TEX_ANTLION = 'A',
		CHAR_TEX_BLOODYFLESH = 'B',
		CHAR_TEX_CONCRETE = 'C',
		CHAR_TEX_DIRT = 'D',
		CHAR_TEX_EGGSHELL = 'E',
		CHAR_TEX_FLESH = 'F',
		CHAR_TEX_GRATE = 'G',
		CHAR_TEX_ALIENFLESH = 'H',
		CHAR_TEX_CLIP = 'I',
		CHAR_TEX_PLASTIC = 'L',
		CHAR_TEX_METAL = 'M',
		CHAR_TEX_SAND = 'N',
		CHAR_TEX_FOLIAGE = 'O',
		CHAR_TEX_COMPUTER = 'P',
		CHAR_TEX_SLOSH = 'S',
		CHAR_TEX_TILE = 'T',
		CHAR_TEX_CARDBOARD = 'U',
		CHAR_TEX_VENT = 'V',
		CHAR_TEX_WOOD = 'W',
		CHAR_TEX_GLASS = 'Y',
		CHAR_TEX_WARPSHIELD = 'Z',
	};
	bool IsEntityBreakable(C_BaseEntity* pEntity)
	{

	}
	/*

	bool trace_to_exit(Vector& end, Vector start, Vector dir, trace_t* enter_trace, trace_t* exit_trace)
	{
		float distance = 0.f;

		while (distance <= 90.f)
		{
			distance += 4.f;
			end = start + (dir * distance);
			auto a1 = end - (dir * 4.f);

			auto point_contents = enginetrace->GetPointContents(end, MASK_SHOT_HULL | CONTENTS_HITBOX, 0);
			if (point_contents & MASK_SHOT_HULL && (!(point_contents & CONTENTS_HITBOX)))
				continue;

			Ray_t ray;
			ray.Init(end, a1);

			enginetrace->TraceRay(ray, MASK_SHOT_HULL | CONTENTS_HITBOX, 0, exit_trace);

			if (exit_trace->startsolid && exit_trace->surface.flags & SURF_HITBOX)
			{
				CTraceFilter filter;
				filter.m_icollisionGroup = COLLISION_GROUP_NONE;
				filter.pSkip = (IHandleEntity*)exit_trace->hit_entity;

				Ray_t ray2;
				ray2.Init(end, start);

				enginetrace->TraceRay(ray2, MASK_SHOT_HULL, &filter, exit_trace);

				if (exit_trace->DidHit() && !exit_trace->startsolid)
				{
					end = exit_trace->endpos;
					return true;
				}

				continue;
			}

			if (!exit_trace->DidHit() || exit_trace->startsolid)
			{
				if (enter_trace->hit_entity)
				{
					if (enter_trace->DidHitNonWorldEntity() && IsEntityBreakable(enter_trace->hit_entity))
					{
						*exit_trace = *enter_trace;
						exit_trace->endpos = start + dir;
						return true;
					}
				}
				continue;
			}

			if ((exit_trace->surface.flags >> 7) & SURF_LIGHT)
			{
				if (IsEntityBreakable(exit_trace->hit_entity) && IsEntityBreakable(enter_trace->hit_entity))
				{
					end = exit_trace->endpos;
					return true;
				}

				if (!((enter_trace->surface.flags >> 7) & SURF_LIGHT))
					continue;
			}

			if (exit_trace->plane.normal.Dot(dir) <= 1.f)
			{
				end = end - (dir * (exit_trace->fraction * 4.f));
				return true;
			}
		}

		return false;
	}
	
	bool inline handle_bullet_penetration(surfacedata_t* enter_surface, trace_t* enter_trace, Vector direction, Vector* origin/*, float range, float penetration, int& penetration_count, float& current_damage)
	{
		int i = 1;
		bool a5 = (enter_trace->contents >> 3) & CONTENTS_SOLID;
		bool v19 = (enter_trace->surface.flags >> 7) & SURF_LIGHT;
		auto enter_material = enter_surface->game.material;

		Vector end;
		trace_t exit_trace;

		if (!penetration_count && !a5 && !v19 && enter_material != CHAR_TEX_GLASS)
		{
			if (enter_material != CHAR_TEX_GRATE)
				return false;
		}

		if (penetration_count <= 0)
			return false;

		if (!trace_to_exit(end, enter_trace->endpos, direction, enter_trace, &exit_trace))
		{
			if (!(enginetrace->GetPointContents(end, MASK_SHOT_HULL, 0) & MASK_SHOT_HULL))
				return false;
		}

		auto exit_surfce_data = g_PhysSurface->GetSurfaceData(exit_trace.surface.surfaceProps);

		auto damage_modifier = 0.16f;

		auto average_penetration_modifier = (enter_surface->game.flPenetrationModifier + exit_surfce_data->game.flPenetrationModifier) / 2;

		if (enter_material == CHAR_TEX_GRATE || enter_material == CHAR_TEX_GLASS)
		{
			damage_modifier = 0.05f;
			average_penetration_modifier = 3.f;
		}

		else if (a5 || v19)
		{
			damage_modifier = 0.16f;
			average_penetration_modifier = 1.f;
		}

		if (enter_material == exit_surfce_data->game.material)
		{
			if (exit_surfce_data->game.material == CHAR_TEX_WOOD || exit_surfce_data->game.material == CHAR_TEX_CARDBOARD)
				average_penetration_modifier = 3.f;

			if (exit_surfce_data->game.material == CHAR_TEX_PLASTIC)
				average_penetration_modifier = 2.f;
		}

		auto modifier = std::max(0.f, 1.f / average_penetration_modifier);
		auto penetration_length = (exit_trace.endpos - enter_trace->endpos).Length();
		auto taken_damage = ((modifier * 3.f) * std::max(0.f, (3.f / penetration) * 1.25f) + (current_damage * damage_modifier)) + (((penetration_length * penetration_length) * modifier) / 24.f);

		current_damage -= std::max(0.f, taken_damage);

		if (current_damage < 1.f)
			return false;

		*origin = exit_trace.endpos;

		--penetration_count;

		return true;
	}
	*/
	static bool HandleBulletPen(CCSWeaponInfo* wpn_data, FireBulletData& data)
	{
		surfacedata_t* enter_surface = g_PhysSurface->GetSurfaceData(data.enter_trace.surface.surfaceProps);
	
		
		//int i = 1;
		bool a5 = (data.enter_trace.contents >> 3) & CONTENTS_SOLID;
		bool v19 = (data.enter_trace.surface.flags >> 7) & SURF_LIGHT;
		auto enter_material = enter_surface->game.material;

		Vector end;
		trace_t exit_trace;

		if (!data.penetrate_count && !a5 && !v19 && enter_material != CHAR_TEX_GLASS)
			if (enter_material != CHAR_TEX_GRATE) 
				return false;
			
			
		

		if (data.penetrate_count <= 0) 
			return false;
		
			

	//	Utils::TraceToExit(dummy, data.enter_trace, data.enter_trace.endpos, data.direction, &trace_exit))

		if (!Utils::TraceToExit(end, data.enter_trace, data.enter_trace.endpos, data.direction, &exit_trace))
			if (!(enginetrace->GetPointContents(end, MASK_SHOT_HULL, 0) & MASK_SHOT_HULL)) 
				return false;
			
				
		

		auto exit_surfce_data = g_PhysSurface->GetSurfaceData(exit_trace.surface.surfaceProps);

		auto damage_modifier = 0.16f;

		auto average_penetration_modifier = (enter_surface->game.flPenetrationModifier + exit_surfce_data->game.flPenetrationModifier) / 2;

		if (enter_material == CHAR_TEX_GRATE || enter_material == CHAR_TEX_GLASS)
		{
			damage_modifier = 0.05f;
			average_penetration_modifier = 3.f;
		}
		else if (a5 || v19)
		{
			damage_modifier = 0.16f;
			average_penetration_modifier = 1.f;
		}

		if (enter_material == exit_surfce_data->game.material)
		{
			if (exit_surfce_data->game.material == CHAR_TEX_WOOD || exit_surfce_data->game.material == CHAR_TEX_CARDBOARD)
				average_penetration_modifier = 3.f;

			if (exit_surfce_data->game.material == CHAR_TEX_PLASTIC)
				average_penetration_modifier = 2.f;
		}

		
		auto modifier = fmaxf(0.f, 1.f / average_penetration_modifier);
		auto penetration_length = (exit_trace.endpos - data.enter_trace.endpos).Length();
		auto taken_damage = ((modifier * 3.f) * fmaxf(0.f, (3.f / wpn_data->flPenetration) * 1.25f) + (data.current_damage * damage_modifier)) + (((penetration_length * penetration_length) * modifier) / 24.f);

		data.current_damage -= fmaxf(0.f, taken_damage);

		if (data.current_damage < 1.f) 
			return false;
		
			

		data.src = exit_trace.endpos;

		--data.penetrate_count;

		return true;
		/*
		surfacedata_t* enter_surface_data = g_PhysSurface->GetSurfaceData(data.enter_trace.surface.surfaceProps);
		int enter_material = enter_surface_data->game.material;
		float enter_surf_penetration_mod = enter_surface_data->game.flPenetrationModifier;
		data.trace_length += data.enter_trace.fraction * data.trace_length_remaining;
		data.current_damage *= pow(wpn_data->flRangeModifier, (data.trace_length * 0.002));
		if ((data.trace_length > 3000.f) || (enter_surf_penetration_mod < 0.1f))
			data.penetrate_count = 0;
		if (data.penetrate_count <= 0)
		{
			Utils::ConsolePrint("Penetrate count 0 \n");
			return false;
		}
			
		Vector dummy;
		trace_t trace_exit;
		if (!Utils::TraceToExit(dummy, data.enter_trace, data.enter_trace.endpos, data.direction, &trace_exit)) 
		{
			Utils::ConsolePrint("Cant tracetoexit \n");
			return false;
		}
		
		surfacedata_t* exit_surface_data = g_PhysSurface->GetSurfaceData(trace_exit.surface.surfaceProps);
		int exit_material = exit_surface_data->game.material;
		float exit_surf_penetration_mod = exit_surface_data->game.flPenetrationModifier;
		float final_damage_modifier = 0.16f;
		float combined_penetration_modifier = 0.0f;
		if (((data.enter_trace.contents & CONTENTS_GRATE) != 0) || (enter_material == 89) || (enter_material == 71))
		{
			combined_penetration_modifier = 3.0f;
			final_damage_modifier = 0.05f;
		}
		else
		{
			combined_penetration_modifier = (enter_surf_penetration_mod + exit_surf_penetration_mod) * 0.5f;
		}
		if (enter_material == exit_material)
		{
			if (exit_material == 87 || exit_material == 85)
				combined_penetration_modifier = 3.0f;
			else if (exit_material == 76)
				combined_penetration_modifier = 2.0f;
		}
		float v34 = fmaxf(0.f, 1.0f / combined_penetration_modifier);
		float v35 = (data.current_damage * final_damage_modifier) + v34 * 3.0f * fmaxf(0.0f, (3.0f / wpn_data->flPenetration) * 1.25f);
		float thickness = (trace_exit.endpos - data.enter_trace.endpos).Length();
		thickness *= thickness;
		thickness *= v34;
		thickness /= 24.0f;
		float lost_damage = fmaxf(0.0f, v35 + thickness);
		if (lost_damage > data.current_damage) 
		{
			Utils::ConsolePrint("More lost dmg \n");
			return false;
		}
			
		if (lost_damage >= 0.0f)
			data.current_damage -= lost_damage;
		if (data.current_damage <= g_Options.min_dmgawall) 
		{
			Utils::ConsolePrint("min dmg too big \n");
			return false;
		}
			
		data.src = trace_exit.endpos;
		data.penetrate_count--;
		return true;*/
	}
	static bool FireSimulatedBullet(FireBulletData& data, C_BaseCombatWeapon* weapon)
	{
		data.penetrate_count = 4;
		data.trace_length = 0.0f;
		CCSWeaponInfo* Weapon_Info = weapon->GetCSWeaponData();
		data.current_damage = (float)Weapon_Info->iDamage;
		while ((data.penetrate_count > 0) && (data.current_damage >= 1.0f))
		{
			data.trace_length_remaining = Weapon_Info->flRange - data.trace_length;
			Vector End_Point = data.src + data.direction * data.trace_length_remaining;
			Utils::TraceLine(data.src, End_Point, MASK_SHOT, g_LocalPlayer, 0, &data.enter_trace);
			Utils::ClipTraceToPlayers(data.src, End_Point * 40.f, MASK_SHOT, &data.filter, &data.enter_trace);
			if (data.enter_trace.fraction == 1.0f)
				break;
			if (!(g_LocalPlayer->m_iTeamNum() != data.enter_trace.hit_entity->GetBaseEntity()->m_iTeamNum())
				|| data.enter_trace.hit_entity->GetBaseEntity()->GetClientClass()->m_ClassID == ClassId_CHostage
				|| data.enter_trace.hit_entity->GetBaseEntity()->GetClientClass()->m_ClassID == ClassId_CChicken)
				break;
			if ((data.enter_trace.hitgroup <= 7) && (data.enter_trace.hitgroup > 0))
			{
				data.trace_length += data.enter_trace.fraction * data.trace_length_remaining;
				data.current_damage *= pow(Weapon_Info->flRangeModifier, data.trace_length * 0.002);
				ScaleDamage(data.enter_trace.hitgroup, (C_BasePlayer*)data.enter_trace.hit_entity, Weapon_Info->flArmorRatio, data.current_damage);
				return true;
			}
			if (!HandleBulletPen(Weapon_Info, data))
				break;
		}
		return false;
	}


	static bool CanWallbang(C_BasePlayer* pLocal,C_BaseCombatWeapon* pWeapon, float &dmg)
	{
		if (!pLocal || !pWeapon)
			return false;

		FireBulletData data(g_LocalPlayer->GetEyePos());
		data.filter.pSkip = g_LocalPlayer;

		QAngle EyeAng;
		engine->GetViewAngles(EyeAng);

		Vector dst = data.src + (EyeAng.Forward() * 8196.f);

		Vector angles;
		Math::CalcAngle(data.src, dst, angles);

		data.direction = angles.Forward().Normalized();

		if (!pWeapon)
			return false;

		data.penetrate_count = 1;
		data.trace_length = 0.0f;

		CCSWeaponInfo *weaponData = pWeapon->GetCSWeaponData();

		if (!weaponData)
			return false;

		data.current_damage = (float)weaponData->iDamage;

		data.trace_length_remaining = weaponData->flRange - data.trace_length;

		Vector end = data.src + data.direction * data.trace_length_remaining;

		Utils::TraceLine(data.src, end, MASK_SHOT | CONTENTS_GRATE, local, 0, &data.enter_trace);

		if (data.enter_trace.fraction == 1.0f) 
			return false;
		
		if (HandleBulletPen(weaponData, data))
		{
			dmg = data.current_damage;
			return true;
		}

		return false;
	}

	static float GetDamage(Vector Target)
	{
		FireBulletData Bullet_Data(g_LocalPlayer->GetEyePos());
		Bullet_Data.filter.pSkip = g_LocalPlayer;
		Vector Angle(0, 0, 0);
		Math::CalcAngle(Bullet_Data.src, Target, Angle);
		Bullet_Data.direction = Angle.Forward();
		if (FireSimulatedBullet(Bullet_Data, (C_BaseCombatWeapon*)g_EntityList->GetClientEntityFromHandle(g_LocalPlayer->m_hActiveWeapon())))
			return Bullet_Data.current_damage;
		return 0;
	}
};
