class CfgPatches
{
	class MWGSM_TraderFix
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data", "MWGSM_RoamingTrader"};
	};
};
class CfgMods
{
	class MWGSM_TraderFix
	{
		dir = "MWGSM_TraderFix";
		picture = "";
		action = "";
		hideName = 1;
		hidePicture = 1;
		name = "MWGSM Trader Currency Label Fix";
		credits = "";
		author = "";
		version = "1.0";
		extra = 0;
		type = "mod";
		dependencies[] = {"Mission"};
		class defs
		{
			class missionScriptModule
			{
				value = "";
				files[] = {"MWGSM_TraderFix/Scripts/5_Mission"};
			};
		};
	};
};
