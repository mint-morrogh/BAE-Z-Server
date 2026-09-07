class CfgPatches
{
    class InventoryMoveSoundsFix
    {
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
        // InventoryMoveSounds_Scripts = CfgPatches name of Inventory Move Sounds' Scripts.pbo
        requiredAddons[] = {"DZ_Data", "InventoryMoveSounds_Scripts"};
    };
};
class CfgMods
{
    class InventoryMoveSoundsFix
    {
        dir = "InventoryMoveSoundsFix";
        picture = "";
        action = "";
        hideName = 1;
        hidePicture = 1;
        name = "InventoryMoveSoundsFix";
        credits = "BAE-Z";
        author = "BAE-Z";
        version = "1.0";
        extra = 0;
        type = "mod";
        dependencies[] = {"Game"};
        class defs
        {
            class gameScriptModule
            {
                value = "";
                files[] = {"InventoryMoveSoundsFix/Scripts/3_Game"};
            };
        };
    };
};
