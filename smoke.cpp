#include "smoke.h"

void smoke::on_framestagenotify() 
{
	if (!options.vis_misc_enabled)
		return;

	static std::vector<const char*> materials =
	{
		"particle/vistasmokev1/vistasmokev1_fire",
		"particle/vistasmokev1/vistasmokev1_smokegrenade",
		"particle/vistasmokev1/vistasmokev1_emods",
		"particle/vistasmokev1/vistasmokev1_emods_impactdust",
	};

	for (auto mat_name : materials)
	{
		IMaterial* mat = matsystem->FindMaterial(mat_name, TEXTURE_GROUP_OTHER);
		
		switch (options.smoke_style) 
		{
		case 0:
			mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
			mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
			break;

		case 1:
			mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
			mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
			break;

		case 2:
			mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
			break;
		}
	
	}
	if (options.smoke_style == 0)
		return;

	static int* smokecount = *(int**)(Utils::PatternScan( LI_FIND_CACHED( GetModuleHandleA )("client.dll"), "A3 ?? ?? ?? ?? 57 8B CB") + 0x1);

	*smokecount = 0;
}