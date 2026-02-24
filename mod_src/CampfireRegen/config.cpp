class CfgPatches
{
    class CampfireRegen
    {
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] = {"DZ_Data"};
    };
};
class CfgMods
{
    class CampfireRegen
    {
        dir = "CampfireRegen";
        picture = "";
        action = "";
        hideName = 1;
        hidePicture = 1;
        name = "CampfireRegen";
        credits = "Blood and Barter";
        author = "Blood and Barter";
        version = "1.0";
        extra = 0;
        type = "mod";
        dependencies[] = {"World"};
        class defs
        {
            class worldScriptModule
            {
                value = "";
                files[] = {"CampfireRegen/Scripts/4_World"};
            };
        };
    };
};
