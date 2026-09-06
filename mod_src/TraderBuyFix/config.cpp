class CfgPatches
{
	class TraderBuyFix
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data", "Trader"};
	};
};
class CfgMods
{
	class TraderBuyFix
	{
		dir = "TraderBuyFix";
		picture = "";
		action = "";
		hideName = 1;
		hidePicture = 1;
		name = "TraderBuyFix";
		credits = "BAE-Z";
		author = "BAE-Z";
		version = "1.0";
		extra = 0;
		type = "mod";
		dependencies[] = {"World"};
		class defs
		{
			class worldScriptModule
			{
				value = "";
				files[] = {"TraderBuyFix/Scripts/4_World"};
			};
		};
	};
};
