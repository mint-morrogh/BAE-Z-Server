class CfgPatches
{
    class StackableItems
    {
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] = {"DZ_Data"};
    };
};
class CfgVehicles
{
    class Paper
    {
        canBeSplit = 1;
        varStackMax = 100;
        varQuantityInit = 1;
        varQuantityMin = 0;
        varQuantityMax = 100;
        varQuantityDestroyOnMin = 1;
    };
    class MoneyRuble1
    {
        varStackMax = 100;
        varQuantityInit = 1;
        varQuantityMin = 0;
        varQuantityMax = 100;
        varQuantityDestroyOnMin = 1;
    };
    class MoneyRuble5
    {
        varStackMax = 100;
        varQuantityInit = 1;
        varQuantityMin = 0;
        varQuantityMax = 100;
        varQuantityDestroyOnMin = 1;
    };
    class MoneyRuble10
    {
        varStackMax = 100;
        varQuantityInit = 1;
        varQuantityMin = 0;
        varQuantityMax = 100;
        varQuantityDestroyOnMin = 1;
    };
    class MoneyRuble25
    {
        varStackMax = 100;
        varQuantityInit = 1;
        varQuantityMin = 0;
        varQuantityMax = 100;
        varQuantityDestroyOnMin = 1;
    };
    class MoneyRuble50
    {
        varStackMax = 100;
        varQuantityInit = 1;
        varQuantityMin = 0;
        varQuantityMax = 100;
        varQuantityDestroyOnMin = 1;
    };
    class MoneyRuble100
    {
        varStackMax = 100;
        varQuantityInit = 1;
        varQuantityMin = 0;
        varQuantityMax = 100;
        varQuantityDestroyOnMin = 1;
    };
    class Nail
    {
        varStackMax = 100;
        varQuantityInit = 1;
        varQuantityMin = 0;
        varQuantityMax = 100;
        varQuantityDestroyOnMin = 1;
    };
    class SmallStone
    {
        canBeSplit = 1;
        varStackMax = 100;
        varQuantityInit = 1;
        varQuantityMin = 0;
        varQuantityMax = 100;
        varQuantityDestroyOnMin = 1;
    };
    class Bone
    {
        varStackMax = 100;
        varQuantityInit = 1;
        varQuantityMin = 0;
        varQuantityMax = 100;
        varQuantityDestroyOnMin = 1;
    };
    class Bark_Oak
    {
        varStackMax = 100;
        varQuantityInit = 1;
        varQuantityMin = 0;
        varQuantityMax = 100;
        varQuantityDestroyOnMin = 1;
    };
    class Bark_Birch
    {
        varStackMax = 100;
        varQuantityInit = 1;
        varQuantityMin = 0;
        varQuantityMax = 100;
        varQuantityDestroyOnMin = 1;
    };
    class Hook
    {
        canBeSplit = 1;
        varStackMax = 100;
        varQuantityInit = 1;
        varQuantityMin = 0;
        varQuantityMax = 100;
        varQuantityDestroyOnMin = 1;
    };
    class Worm
    {
        canBeSplit = 1;
        varStackMax = 100;
        varQuantityInit = 1;
        varQuantityMin = 0;
        varQuantityMax = 100;
        varQuantityDestroyOnMin = 1;
    };
};
class CfgMods
{
    class StackableItems
    {
        dir = "StackableItems";
        picture = "";
        action = "";
        hideName = 1;
        hidePicture = 1;
        name = "StackableItems";
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
                files[] = {};
            };
        };
    };
};
