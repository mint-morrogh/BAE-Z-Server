class CfgPatches
{
    class TraderSearchFix
    {
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
        // "trader" = CfgPatches name of Dr Jones Trader (@Trader/addons/Trader.pbo)
        requiredAddons[] = {"DZ_Data", "trader"};
    };
};
class CfgMods
{
    class TraderSearchFix
    {
        dir = "TraderSearchFix";
        picture = "";
        action = "";
        hideName = 1;
        hidePicture = 1;
        name = "TraderSearchFix";
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
                files[] = {"TraderSearchFix/Scripts/4_World"};
            };
        };
    };
};
