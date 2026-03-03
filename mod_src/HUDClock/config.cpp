class CfgPatches
{
    class HUDClock
    {
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] = {"DZ_Data"};
    };
};
class CfgMods
{
    class HUDClock
    {
        dir = "HUDClock";
        picture = "";
        action = "";
        hideName = 1;
        hidePicture = 1;
        name = "HUDClock";
        credits = "BAE-Z";
        author = "BAE-Z";
        version = "1.0";
        extra = 0;
        type = "mod";
        dependencies[] = {"Mission"};
        class defs
        {
            class missionScriptModule
            {
                value = "";
                files[] = {"HUDClock/Scripts/5_Mission"};
            };
        };
    };
};
