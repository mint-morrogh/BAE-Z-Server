class CfgPatches
{
    class SitRest
    {
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] = {"DZ_Data"};
    };
};
class CfgMods
{
    class SitRest
    {
        dir = "SitRest";
        picture = "";
        action = "";
        hideName = 1;
        hidePicture = 1;
        name = "SitRest";
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
                files[] = {"SitRest/Scripts/4_World"};
            };
        };
    };
};
