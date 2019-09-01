#pragma once
#include "../valve_sdk/misc/Color.hpp"
class C_BasePlayer;
class C_BaseEntity;
class C_BaseCombatWeapon;
class C_PlantedC4;
class Color;
class ClientClass;

namespace Visuals
{
	

    namespace Player
    {
        bool Begin(C_BasePlayer* pl);

		void RenderBox(bool DrawOpt = false, Color optional = {});
		void RenderLastknown(int alpha);
		void RenderName();
		void SetAlpha(int alpha);
        void RenderHealth();
		void RenderArmour();
        void RenderWeapon();
        void RenderSnapline();
		void RenderSkeleton( );
    }

    namespace Misc
    {
        void RenderCrosshair();
        void RenderWeapon(C_BaseCombatWeapon* ent);
        void RenderDefuseKit(C_BaseEntity* ent);
        void RenderPlantedC4(C_BaseEntity* ent);
    }


    bool CreateFonts();
    void DestroyFonts();
}
