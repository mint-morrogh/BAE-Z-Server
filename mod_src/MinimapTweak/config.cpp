class CfgPatches
{
    class MinimapTweak
    {
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] = {"ExpansionGPSOverride"};
    };
};
class CfgMods
{
    class MinimapTweak
    {
        dir = "MinimapTweak";
        picture = "";
        action = "";
        hideName = 1;
        hidePicture = 1;
        name = "MinimapTweak";
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
                files[] = {"MinimapTweak/Scripts/5_Mission"};
            };
        };
    };
};
