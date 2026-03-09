@echo off
:start
::Server name (This is just for the bat file)
set serverName=BAE-Z
::Server files location
set serverLocation="%~dp0"
::Server Port
set serverPort=2302
::Server config
set serverConfig=serverDZ.cfg
::Logical CPU cores to use (Equal or less than available)
set serverCPU=2
::Sets title for terminal (DONT edit)
title %serverName% batch
::DayZServer location (DONT edit)
cd "%serverLocation%"
del /q "config\DataCache\cache.ch" >nul 2>&1
del /q "config\DataCache\cache_lock" >nul 2>&1
echo (%time%) %serverName% started.
::Launch parameters (edit end: -config=|-port=|-profiles=|-doLogs|-adminLog|-netLog|-freezeCheck|-filePatching|-BEpath=|-cpuCount=)
start "DayZ Server" /min "DayZServer_x64.exe" -config=%serverConfig% -port=%serverPort% "-profiles=config" "-mod=@CF;@DayZ-Expansion-Core;@DabsFramework;@DayZ-Expansion;@DayZ-Expansion-Navigation;@DayZ-Expansion-Book;@DayZ-Expansion-Groups;@DayZ-Expansion-Spawn-Selection;@ExpansionMinimap;@DayZ-Expansion-AI;@DayZ-Expansion-Quests;@DayZ-Expansion-Weapons;@DayZ-Expansion-Vehicles;@DayZ-Expansion-Licensed;@MinimapTweak;@HUDClock;@Trader;@Mass'sManyItemOverhaul;@UnlimitedRun;@VanillaPlusPlusMap;@GoreZ;@Inventory-Move-Sounds;@PvZmoD;@ZenSkills;@CZOptics;@PercentageHUD;@ZenTreasure;@CarePackages;@NemsisCraftingpack;@LMsPlanes;@CookZ;@CookZRealisticPackaging;@SleepTillMorning;@4KBOSSKVehicles;@DayZDog;@SurvivorAnims;@DayZHorse;@GelyaBackpacks;@MWGSM_Trader;@MWGSM_TraderFix;@SagaShakeTree;@StackableItems;@BAEZLoadingScreen;@VVN_Greenhouse;@VVN_Old_refrigerator;@FlipCar;@EnableInventoryInVehicle;@AJsWeapons;@JosiesLilBuggyZ;@MBM_HarleyDavidsonFatBoy;@LMsHelicopters" "-serverMod=@DayZombieManager;@CampfireRegen;@SitRest;@DurableGear;@HealthBoost;@SFE_NoVehicleDamage;@SobrMods_Signal_Overnight_Stay;@BandageBoost;@AmmoStacks;@BatteryBoost" -cpuCount=%serverCPU% -dologs -adminlog -netlog -freezecheck
::Time in seconds before kill server process (43200 = 12 hours)
timeout 43190
taskkill /im DayZServer_x64.exe /F
::Time in seconds to wait before..
timeout 10
::Go back to the top and repeat the whole cycle again
goto start