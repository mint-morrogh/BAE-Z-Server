class CfgPatches
{
    class ZombieKillDrops
    {
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] = {"DZ_Data"};
    };
};
class CfgMods
{
    class ZombieKillDrops
    {
        dir = "ZombieKillDrops";
        picture = "";
        action = "";
        hideName = 1;
        hidePicture = 1;
        name = "ZombieKillDrops";
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
                files[] = {"ZombieKillDrops/Scripts/4_World"};
            };
        };
    };
};
