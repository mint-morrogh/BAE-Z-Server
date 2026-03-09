@echo off
:: ============================================================
:: BAE-Z - Launch DayZ Client
:: ============================================================
:: All-in-one launcher: pulls latest config changes, installs
:: or updates Workshop mods, syncs custom client mods, then
:: launches DayZ with the correct mod list.
::
:: Usage: double-click this file instead of launching DayZ
::        through Steam. Run every time you play to stay synced.
:: ============================================================

setlocal
pushd "%~dp0"

:: Resolve the DayZ client path (sibling of DayZServer under steamapps\common\)
pushd "%~dp0..\DayZ" 2>nul
if errorlevel 1 (
    echo ERROR: DayZ client not found next to DayZServer folder.
    echo Expected: steamapps\common\DayZ
    pause
    exit /b 1
)
set "DAYZ_CLIENT=%CD%"
popd

:: Build the full mod list now that DAYZ_CLIENT is known.
:: NOTE: delayed expansion is OFF so !Workshop paths stay literal.
call :build_modlist

echo ============================================
echo  BAE-Z - Launch DayZ
echo ============================================
echo.

:: ============================================================
:: Step 1: Pull latest updates from GitHub
:: ============================================================
echo --- Checking for updates ---
where git >nul 2>&1
if errorlevel 1 (
    echo.
    echo   =====================================================
    echo   Git is not installed!
    echo   =====================================================
    echo.
    echo   Git is needed to keep your game files up to date.
    echo   Without it, you may get errors or crashes.
    echo.
    echo   HOW TO INSTALL:
    echo     1. Open this link in your browser:
    echo        https://git-scm.com/download/win
    echo     2. Click "Click here to download" at the top
    echo     3. Run the installer - click Next on every screen
    echo        (all the defaults are fine^)
    echo     4. When it finishes, CLOSE this window and
    echo        double-click LAUNCH_DAYZ.bat again
    echo.
    echo   =====================================================
    echo.
    pause
    exit /b 1
)

:: If this folder isn't a git repo yet, clone into it.
:: This happens when someone copies the files instead of cloning.
git rev-parse --git-dir >nul 2>&1
if errorlevel 1 (
    echo   [SETUP] Not a git repo - setting up...
    git init >nul 2>&1
    git remote add origin https://github.com/mint-morrogh/BAE-Z-Server.git >nul 2>&1
    git fetch --quiet 2>nul
    git checkout -f main 2>nul
    echo   [OK]    Repository initialized
    echo.
    echo   Restarting with latest files...
    start "" cmd /c "%~f0"
    exit /b 0
)

:: Check if this launcher would be modified by the update.
:: If so, pull and restart so the new version runs cleanly.
git fetch --quiet 2>nul
set "SELF_UPDATE=0"
for /f %%f in ('git diff HEAD..origin/main --name-only -- LAUNCH_DAYZ.bat 2^>nul') do set "SELF_UPDATE=1"
:: Reset any locally modified tracked files so pull never conflicts.
:: This is safe — players should always match the remote repo.
git checkout -- . >nul 2>&1
git pull --ff-only 2>&1
if errorlevel 1 (
    echo   [WARN] git pull failed - you may have local changes
    echo          Continuing with current files...
) else if "%SELF_UPDATE%"=="1" (
    echo   [UPD]  Launcher updated - restarting...
    start "" cmd /c "%~f0"
    exit /b 0
) else (
    echo   [OK]   Up to date
)
echo.

:: ============================================================
:: Step 2: Install / update Workshop mods + bikeys
:: ============================================================
set "QUIET=1"
call "%~dp0install_mods.bat"
if errorlevel 2 (
    echo ============================================
    echo  CANNOT LAUNCH - Missing Workshop mods!
    echo ============================================
    echo.
    echo Subscribe to the mods listed above, then
    echo launch DayZ from Steam once so they download,
    echo then run this script again.
    echo.
    pause
    exit /b 1
)

:: ============================================================
:: Step 3: Fix known Workshop mod issues
:: ============================================================
:: Nemsis Craftingpack: the Airbrushing PBO references Mass_ACR_Base
:: which doesn't exist, causing a script compile error on launch.
:: The mod author removed it in an update but Steam sometimes serves
:: the old version from cache. Delete it if present.
echo --- Fixing known Workshop mod issues ---
set "WORKSHOP=%~dp0..\..\workshop\content\221100"
set "FIXES_APPLIED=0"

:: Fix 1: Nemsis Craftingpack Airbrushing PBO (references missing Mass_ACR_Base)
set "NEMSIS_DIR=%WORKSHOP%\3606014796\addons"
if exist "%NEMSIS_DIR%\nm_Crafting_Part_Airbrushing.pbo" (
    del /q "%NEMSIS_DIR%\nm_Crafting_Part_Airbrushing.pbo" >nul 2>&1
    del /q "%NEMSIS_DIR%\nm_Crafting_Part_Airbrushing.pbo.n3msi.bisign" >nul 2>&1
    echo   [FIX]  Removed Nemsis Airbrushing PBO (Mass_ACR_Base compile error^)
    set "FIXES_APPLIED=1"
)

:: Fix 2: SurvivorAnims PBO (compile-order bug + GetHealth01 client crash)
:: The Workshop PBO has a subdirectory structure that causes script compile
:: errors and a client crash on the main menu mannequin. Our patched PBO
:: flattens the directory and adds a client guard.
set "SA_WORKSHOP=%WORKSHOP%\2918418331\Addons"
set "SA_PATCH=%~dp0mod_src\SurvivorAnimsPatch\SurvivorAnims.pbo.patched"
if exist "%SA_PATCH%" (
    if exist "%SA_WORKSHOP%" (
        fc /b "%SA_PATCH%" "%SA_WORKSHOP%\SurvivorAnims.pbo" >nul 2>&1
        if errorlevel 1 (
            copy /Y "%SA_PATCH%" "%SA_WORKSHOP%\SurvivorAnims.pbo" >nul
            echo   [FIX]  Patched SurvivorAnims PBO in Workshop (mannequin crash fix^)
            set "FIXES_APPLIED=1"
        )
    )
)

:: Fix 3: Expansion Quests GUI — wider quest tracker HUD
:: Default tracker is 18% screen width, causing text to wrap and get clipped.
:: Our patched PBO widens it to 24% so objective text fits without cutoff.
set "QG_WORKSHOP=%WORKSHOP%\2828486817\addons"
set "QG_PATCH=%~dp0mod_src\ExpansionQuestsGUIPatch\quests_gui.pbo.patched"
if exist "%QG_PATCH%" (
    if exist "%QG_WORKSHOP%" (
        fc /b "%QG_PATCH%" "%QG_WORKSHOP%\quests_gui.pbo" >nul 2>&1
        if errorlevel 1 (
            copy /Y "%QG_PATCH%" "%QG_WORKSHOP%\quests_gui.pbo" >nul
            echo   [FIX]  Patched Expansion Quests GUI in Workshop (wider quest tracker^)
            set "FIXES_APPLIED=1"
        )
    )
)

:: Fix 4: VanillaPlusPlusMap — null-pointer crash on game exit
:: VPP3DMarker destructor calls GetGame().GetCallQueue() during teardown
:: when GetGame() is already NULL, causing 9x NULL pointer errors and
:: contributing to ACCESS_VIOLATION crash on exit.
set "VPP_WORKSHOP=%WORKSHOP%\1623711988\Addons"
set "VPP_PATCH=%~dp0mod_src\VanillaPPMapPatch\VanillaPPMap.pbo.patched"
if exist "%VPP_PATCH%" (
    if exist "%VPP_WORKSHOP%" (
        fc /b "%VPP_PATCH%" "%VPP_WORKSHOP%\VanillaPPMap.pbo" >nul 2>&1
        if errorlevel 1 (
            copy /Y "%VPP_PATCH%" "%VPP_WORKSHOP%\VanillaPPMap.pbo" >nul
            echo   [FIX]  Patched VanillaPPMap in Workshop (exit crash null-pointer fix^)
            set "FIXES_APPLIED=1"
        )
    )
)

:: Fix 5: DayZ-Dog — IntroSceneCharacter startup crash (null m_CharacterDta)
:: DayZ-Dog overrides CharacterLoad but crashes when m_CharacterDta is NULL
:: during heavy mod loading (Access violation at 0x8). Our patched PBO adds
:: null guards around CreateCharacterPerson.
set "DD_WORKSHOP=%WORKSHOP%\2471347750\Addons"
set "DD_PATCH=%~dp0mod_src\DayZDogPatch\dayz_dog.pbo.patched"
if exist "%DD_PATCH%" (
    if exist "%DD_WORKSHOP%" (
        fc /b "%DD_PATCH%" "%DD_WORKSHOP%\dayz_dog.pbo" >nul 2>&1
        if errorlevel 1 (
            copy /Y "%DD_PATCH%" "%DD_WORKSHOP%\dayz_dog.pbo" >nul
            echo   [FIX]  Patched DayZ-Dog PBO in Workshop (startup crash null-pointer fix^)
            set "FIXES_APPLIED=1"
        )
    )
)

if "%FIXES_APPLIED%"=="0" echo   [OK]   No known issues found
echo.

:: ============================================================
:: Step 4: Sync Roaming Trader config to client profile
:: ============================================================
:: The MWGSM Roaming Trader reads config from $profile: which on
:: the client resolves to Documents\DayZ\. Without this copy the
:: client defaults to "Nails" as currency instead of Rubles.
echo --- Syncing Roaming Trader config to client ---
set "RT_SRC=%~dp0config\MWGSM_RoamingTrader\MWGSM_RoamingTraderConfig.json"
set "RT_DST=%USERPROFILE%\Documents\DayZ\MWGSM_RoamingTrader"
if exist "%RT_SRC%" (
    if not exist "%RT_DST%" mkdir "%RT_DST%"
    copy /Y "%RT_SRC%" "%RT_DST%\" >nul 2>&1
    echo   [SYNC] Roaming Trader config -^> %RT_DST%
) else (
    echo   [SKIP] Roaming Trader config not found on server
)
echo.

:: ============================================================
:: Step 5: Sync custom client mods to DayZ folder
:: ============================================================
echo --- Syncing custom client mods ---
echo   Server: %~dp0
echo   Client: %DAYZ_CLIENT%
echo.

call :sync_mod MinimapTweak
call :sync_mod HUDClock
call :sync_mod StackableItems
call :sync_mod MWGSM_TraderFix
call :sync_mod BAEZLoadingScreen
call :sync_mod EnableInventoryInVehicle

:: Clear client script cache so new PBOs are picked up
del /q "%LOCALAPPDATA%\DayZ\DataCache\cache.ch" >nul 2>&1
echo   [OK]   Client script cache cleared
echo.

:: ============================================================
:: Step 6: Clear DayZ Launcher preset
:: ============================================================
:: The DayZ Launcher prepends its own mod list from its preset,
:: doubling every mod and ignoring our load order. Clear it so
:: ONLY our -mod= parameter controls which mods load and in what order.
set "PRESET_DIR=%LOCALAPPDATA%\DayZ Launcher\Presets"
if exist "%PRESET_DIR%\dayz.defaultpreset2" (
    > "%PRESET_DIR%\dayz.defaultpreset2" (
        echo ^<?xml version="1.0" encoding="utf-8"?^>
        echo ^<addons-presets^>
        echo   ^<last-update^>2026-01-01T00:00:00.0000000-05:00^</last-update^>
        echo   ^<published-ids /^>
        echo ^</addons-presets^>
    )
    echo   [OK]   DayZ Launcher preset cleared (mod order controlled by BAE-Z^)
    echo.
)

:: ============================================================
:: Step 7: Launch DayZ
:: ============================================================
echo ============================================
echo  Launching DayZ with full mod list
echo ============================================
echo.

:: Launch DayZ through Steam with the complete mod list.
:: Launcher preset is cleared so only our -mod= controls load order.
start "" "steam://run/221100//-mod=%MODLIST%/"

echo DayZ is starting. You can close this window.
popd
timeout /t 5
exit /b 0

:: ============================================================
:build_modlist
:: Builds the full mod list with correct load order.
:: Workshop mods use !Workshop symlinks. Custom mods use @Name.
:: ORDER MATTERS: last mod wins for animation/script overrides.
::
:: NOTE: delayed expansion is OFF, so !Workshop is literal text
:: and %MODLIST% expands correctly on each line (flat subroutine).
:: ============================================================
set "W=%DAYZ_CLIENT%\!Workshop"
set "MODLIST="

:: --- Frameworks (must load first) ---
set "MODLIST=%W%\@CF;"
set "MODLIST=%MODLIST%%W%\@Dabs Framework;"
set "MODLIST=%MODLIST%%W%\@DayZ-Expansion-Core;"
set "MODLIST=%MODLIST%%W%\@DayZ-Expansion-Licensed;"

:: --- Standalone mods (order flexible) ---
set "MODLIST=%MODLIST%%W%\@UnlimitedRun;"
set "MODLIST=%MODLIST%%W%\@GoreZ;"
set "MODLIST=%MODLIST%%W%\@Inventory Move Sounds;"
set "MODLIST=%MODLIST%%W%\@Trader;"
set "MODLIST=%MODLIST%%W%\@CZ Optics;"
set "MODLIST=%MODLIST%%W%\@CookZ;"
set "MODLIST=%MODLIST%%W%\@CookZ Realistic Packaging;"
set "MODLIST=%MODLIST%%W%\@LMs Planes;"
set "MODLIST=%MODLIST%%W%\@Care Packages V2;"
set "MODLIST=%MODLIST%%W%\@Zens Treasure;"
set "MODLIST=%MODLIST%%W%\@PercentageHUD;"
set "MODLIST=%MODLIST%%W%\@4KBOSSKVehicles;"
set "MODLIST=%MODLIST%%W%\@dzr_sleep_till_morning EXPERIMENTAL;"
set "MODLIST=%MODLIST%%W%\@ExpansionMinimap_Override;"
set "MODLIST=%MODLIST%%W%\@Pack complete - Backpacks FREE;"
set "MODLIST=%MODLIST%%W%\@MWGSM_Trader;"
set "MODLIST=%MODLIST%%W%\@Tree Shake;"
set "MODLIST=%MODLIST%%W%\@DayZ-Expansion-Book;"
set "MODLIST=%MODLIST%%W%\@Enable Inventory In Vehicle;"
set "MODLIST=%MODLIST%%W%\@FLIP CAR;"
set "MODLIST=%MODLIST%%W%\@SFE- No Vehicle Damage Customizable Crash Protection;"
set "MODLIST=%MODLIST%%W%\@VVN_Greenhouse;"
set "MODLIST=%MODLIST%%W%\@VVN_Old_refrigerator;"
set "MODLIST=%MODLIST%%W%\@AJs Weapons;"
set "MODLIST=%MODLIST%%W%\@MBM_HarleyDavidsonFatBoy;"
set "MODLIST=%MODLIST%%W%\@JosiesLilBuggyZ;"
set "MODLIST=%MODLIST%%W%\@LMs Helicopter Flight Systems;"
set "MODLIST=%MODLIST%%W%\@Mass'sManyItemOverhaul;"
set "MODLIST=%MODLIST%%W%\@VanillaPlusPlusMap;"
set "MODLIST=%MODLIST%%W%\@Zens Skill Perk Tree;"
set "MODLIST=%MODLIST%%W%\@Nemsis Craftingpack All in One;"
set "MODLIST=%MODLIST%%W%\@DayZ Horse;"
set "MODLIST=%MODLIST%%W%\@DayZ-Dog;"
set "MODLIST=%MODLIST%%W%\@PvZmoD_CustomisableZombies;"
set "MODLIST=%MODLIST%%W%\@[SobrMods] Signal Overnight Stay;"

:: --- Expansion feature mods ---
set "MODLIST=%MODLIST%%W%\@DayZ-Expansion;"
set "MODLIST=%MODLIST%%W%\@DayZ-Expansion-Navigation;"
set "MODLIST=%MODLIST%%W%\@DayZ-Expansion-Groups;"
set "MODLIST=%MODLIST%%W%\@DayZ-Expansion-Spawn-Selection;"
set "MODLIST=%MODLIST%%W%\@DayZ-Expansion-Quests;"
set "MODLIST=%MODLIST%%W%\@DayZ-Expansion-AI;"
set "MODLIST=%MODLIST%%W%\@DayZ-Expansion-Weapons;"
set "MODLIST=%MODLIST%%W%\@DayZ-Expansion-Vehicles;"

:: --- Animation mods (order critical — last wins) ---
:: Expansion Animations FIRST, then SurvivorAnims overrides it
set "MODLIST=%MODLIST%%W%\@DayZ-Expansion-Animations;"
set "MODLIST=%MODLIST%%W%\@Survivor Animations;"

:: --- Custom mods (not on Workshop, synced from server) ---
set "MODLIST=%MODLIST%@MinimapTweak;"
set "MODLIST=%MODLIST%@HUDClock;"
set "MODLIST=%MODLIST%@StackableItems;"
set "MODLIST=%MODLIST%@MWGSM_TraderFix;"
set "MODLIST=%MODLIST%@BAEZLoadingScreen;"
set "MODLIST=%MODLIST%@EnableInventoryInVehicle"

exit /b 0

:: ============================================================
:sync_mod
:: Copies @%1 folder to the DayZ client directory
:: ============================================================
set "MOD_NAME=%~1"
if not exist "@%MOD_NAME%" (
    echo   [SKIP] @%MOD_NAME% - not found in server folder
    exit /b 0
)
xcopy /E /I /D /Y "@%MOD_NAME%" "%DAYZ_CLIENT%\@%MOD_NAME%" >nul 2>&1
echo   [SYNC] @%MOD_NAME% -^> %DAYZ_CLIENT%\@%MOD_NAME%
exit /b 0
