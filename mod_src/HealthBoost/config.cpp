class CfgPatches
{
    class HealthBoost
    {
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] = {"DZ_Data", "dayz_dog", "dayz_horse"};
    };
};
// Double dog bite damage (80→160 Health, 200→400 Blood, 22→44 Shock)
class CfgAmmo
{
    class MeleeDamage;
    class MeleeDog: MeleeDamage
    {
        class DamageApplied
        {
            type = "Melee";
            bleedThreshold = 1;
            class Health
            {
                damage = 160;
            };
            class Blood
            {
                damage = 400;
            };
            class Shock
            {
                damage = 44;
            };
        };
    };
    class MeleeDog_Heavy: MeleeDog
    {
        hitAnimation = 1;
    };
};
// 12.5x dog health (800→10000 HP, 5000→20000 Blood)
// Huge horse health pool (10000 HP, 20000 Blood, 400 Shock)
class CfgVehicles
{
    class AnimalBase;
    class Dayz_Doggo: AnimalBase
    {
        class DamageSystem
        {
            class GlobalHealth
            {
                class Health
                {
                    hitpoints = 10000;
                };
                class Blood
                {
                    hitpoints = 20000;
                };
                class Shock
                {
                    hitpoints = 400;
                };
            };
        };
    };
    class Animal_Horse: AnimalBase
    {
        class DamageSystem
        {
            class GlobalHealth
            {
                class Health
                {
                    hitpoints = 10000;
                };
                class Blood
                {
                    hitpoints = 20000;
                };
                class Shock
                {
                    hitpoints = 400;
                };
            };
        };
    };
};
class CfgMods
{
    class HealthBoost
    {
        dir = "HealthBoost";
        picture = "";
        action = "";
        hideName = 1;
        hidePicture = 1;
        name = "HealthBoost";
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
                files[] = {"HealthBoost/Scripts/4_World"};
            };
        };
    };
};
