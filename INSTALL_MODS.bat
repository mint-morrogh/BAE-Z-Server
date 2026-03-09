@echo off
:: ============================================================
:: Blood ^& Barter - Install / Update Workshop Mods
:: ============================================================
:: Copies Workshop mod addons and bikeys into this server
:: directory. Uses /D flag to only copy newer files, so repeat
:: runs are fast when nothing has changed.
::
:: Can be run standalone or called by LAUNCH_DAYZ.bat.
::
:: Set QUIET=1 before calling to suppress the pause at the end.
:: ============================================================

setlocal enabledelayedexpansion
pushd "%~dp0"

echo.
echo --- Checking Workshop mods ---

:: Resolve the Workshop content path
:: DayZServer is at: steamapps\common\DayZServer
:: Workshop mods at: steamapps\workshop\content\221100\<id>
set "WORKSHOP=%~dp0..\..\workshop\content\221100"
pushd "%WORKSHOP%" 2>nul
if errorlevel 1 (
    echo ERROR: Workshop folder not found.
    echo Expected: steamapps\workshop\content\221100\
    echo Make sure you subscribed to the mods and launched DayZ once.
    if not "%QUIET%"=="1" pause
    exit /b 1
)
set "WORKSHOP=%CD%"
popd

:: Create keys folder if it doesn't exist
if not exist "keys" mkdir keys

set TOTAL=0
set INSTALLED=0
set UPDATED=0
set MISSING=0

:: ============================================================
:: Workshop ID -> @ModName mapping (43 mods)
:: ============================================================
call :install_mod 1559212036 CF
call :install_mod 2291785308 DayZ-Expansion-Core
call :install_mod 2545327648 DabsFramework
call :install_mod 2116151222 DayZ-Expansion
call :install_mod 2792984722 DayZ-Expansion-Navigation
call :install_mod 2572324799 DayZ-Expansion-Book
call :install_mod 2792983364 DayZ-Expansion-Groups
call :install_mod 2804241648 DayZ-Expansion-Spawn-Selection
call :install_mod 3626138230 ExpansionMinimap
call :install_mod 2792982069 DayZ-Expansion-AI
call :install_mod 2828486817 DayZ-Expansion-Quests
call :install_mod 2792985069 DayZ-Expansion-Weapons
call :install_mod 2116157322 DayZ-Expansion-Licensed
call :install_mod 1590841260 Trader
call :install_mod 1566911166 "Mass'sManyItemOverhaul"
call :install_mod 2040872847 UnlimitedRun
call :install_mod 1623711988 VanillaPlusPlusMap
call :install_mod 1648967877 GoreZ
call :install_mod 2444247391 Inventory-Move-Sounds
call :install_mod 2051775667 PvZmoD
call :install_mod 3601119520 ZenSkills
call :install_mod 3571068454 CZOptics
call :install_mod 3443562573 PercentageHUD
call :install_mod 3426979799 ZenTreasure
call :install_mod 2691041685 CarePackages
call :install_mod 3606014796 NemsisCraftingpack
call :install_mod 3639695989 LMsPlanes
call :install_mod 3624740747 LMsHelicopters
call :install_mod 3302732231 CookZ
call :install_mod 3566508757 CookZRealisticPackaging
call :install_mod 3578708533 SleepTillMorning
call :install_mod 3369325490 4KBOSSKVehicles
call :install_mod 2471347750 DayZDog
call :install_mod 2918418331 SurvivorAnims
call :install_mod 3295021220 DayZHorse
call :install_mod 3636799682 MWGSM_Trader
call :install_mod 3545040196 SagaShakeTree
call :install_mod 3648464156 GelyaBackpacks
call :install_mod 3459215297 SFE_NoVehicleDamage
call :install_mod 3664943616 VVN_Greenhouse
call :install_mod 3592127439 VVN_Old_refrigerator
call :install_mod 3659550946 SobrMods_Signal_Overnight_Stay
call :install_mod 3594596641 EnableInventoryInVehicle
call :install_mod 3623001011 FlipCar
call :install_mod 3571685323 AJsWeapons
call :install_mod 3671771423 JosiesLilBuggyZ
call :install_mod 2291785437 DayZ-Expansion-Vehicles
call :install_mod 3665840738 MBM_HarleyDavidsonFatBoy

:: ============================================================
:: Post-install: Re-apply SurvivorAnims PBO patch
:: The Workshop PBO has a compile-order bug (actions/ subdirectory)
:: and a client crash (GetHealth01 on AI proxies).
:: Our patched PBO flattens the directory and adds a client guard.
:: If SurvivorAnims updates on Workshop, re-run the patch script
:: in mod_src/SurvivorAnimsPatch/ to rebuild the patched PBO.
:: ============================================================
if exist "%~dp0mod_src\SurvivorAnimsPatch\SurvivorAnims.pbo.patched" (
    echo.
    echo --- Re-applying SurvivorAnims PBO patch ---
    copy /Y "%~dp0mod_src\SurvivorAnimsPatch\SurvivorAnims.pbo.patched" "%~dp0@SurvivorAnims\Addons\SurvivorAnims.pbo" >nul
    echo   [PATCH] SurvivorAnims PBO patched (compile-order fix + client guard^)
)

:: ============================================================
:: Post-install: Expansion Animations PBO patch — REMOVED
:: The stripped PBO caused native crashes. Using full Workshop PBO.
:: ============================================================

:: ============================================================
:: Post-install: Re-apply DayZ-Dog PBO patch
:: DayZ-Dog overrides CharacterLoad but crashes when
:: m_CharacterDta is NULL during heavy mod loading.
:: Our patched PBO adds null guards around CreateCharacterPerson.
:: ============================================================
if exist "%~dp0mod_src\DayZDogPatch\dayz_dog.pbo.patched" (
    echo.
    echo --- Re-applying DayZ-Dog PBO patch ---
    copy /Y "%~dp0mod_src\DayZDogPatch\dayz_dog.pbo.patched" "%~dp0@DayZDog\addons\dayz_dog.pbo" >nul
    echo   [PATCH] DayZ-Dog PBO patched (startup crash null-pointer fix^)
)

:: ============================================================
:: Post-install: Re-apply VanillaPPMap PBO patch
:: VPP3DMarker destructor crashes on exit (GetGame() is NULL
:: during teardown). Our patched PBO adds null guards.
:: ============================================================
if exist "%~dp0mod_src\VanillaPPMapPatch\VanillaPPMap.pbo.patched" (
    echo.
    echo --- Re-applying VanillaPPMap PBO patch ---
    copy /Y "%~dp0mod_src\VanillaPPMapPatch\VanillaPPMap.pbo.patched" "%~dp0@VanillaPlusPlusMap\Addons\VanillaPPMap.pbo" >nul
    echo   [PATCH] VanillaPPMap PBO patched (exit crash null-pointer fix^)
)


echo.
if %INSTALLED% GTR 0 echo   %INSTALLED% mods newly installed
if %UPDATED% GTR 0 echo   %UPDATED% mods updated
if %MISSING% GTR 0 (
    echo   %MISSING% mods NOT FOUND - subscribe in Steam Workshop!
)
set /a OK=TOTAL-MISSING
echo   %OK%/%TOTAL% mods ready
echo.

popd
if not "%QUIET%"=="1" pause
if %MISSING% GTR 0 exit /b 2
exit /b 0

:: ============================================================
:install_mod
:: %1 = Workshop ID, %2 = Mod name (without @)
:: ============================================================
set /a TOTAL+=1
set "WS_ID=%~1"
set "MOD_NAME=%~2"
set "SRC=%WORKSHOP%\%WS_ID%"
set "DST=%~dp0@%MOD_NAME%"

if not exist "%SRC%" (
    echo   [MISS] @%MOD_NAME% - not subscribed! Go to:
    echo          https://steamcommunity.com/sharedfiles/filedetails/?id=%WS_ID%
    set /a MISSING+=1
    exit /b 0
)

:: Check if this is a new install or an update
set "IS_NEW=0"
if not exist "%DST%\addons" set "IS_NEW=1"

:: Copy addons folder (only newer files with /D)
if exist "%SRC%\addons" (
    if "!IS_NEW!"=="1" (
        echo   [COPY] @%MOD_NAME% - installing...
        xcopy /E /I /D /Y "%SRC%\addons" "%DST%\addons" >nul 2>&1
        echo   [NEW]  @%MOD_NAME%
        set /a INSTALLED+=1
    ) else (
        <nul set /p "=   [CHECK] @%MOD_NAME%... "
        xcopy /E /I /D /Y "%SRC%\addons" "%DST%\addons" >nul 2>&1
        echo OK
    )
) else if exist "%SRC%\Addons" (
    if "!IS_NEW!"=="1" (
        echo   [COPY] @%MOD_NAME% - installing...
        xcopy /E /I /D /Y "%SRC%\Addons" "%DST%\addons" >nul 2>&1
        echo   [NEW]  @%MOD_NAME%
        set /a INSTALLED+=1
    ) else (
        <nul set /p "=   [CHECK] @%MOD_NAME%... "
        xcopy /E /I /D /Y "%SRC%\Addons" "%DST%\addons" >nul 2>&1
        echo OK
    )
)

:: Copy .bikey files to server keys folder
if exist "%SRC%\keys" (
    for %%k in ("%SRC%\keys\*.bikey") do (
        copy /Y "%%k" "%~dp0keys\" >nul 2>&1
    )
) else if exist "%SRC%\key" (
    for %%k in ("%SRC%\key\*.bikey") do (
        copy /Y "%%k" "%~dp0keys\" >nul 2>&1
    )
)

exit /b 0
