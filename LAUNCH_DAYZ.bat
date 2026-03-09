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

setlocal enabledelayedexpansion
pushd "%~dp0"

:: Custom client mods that aren't on the Workshop.
:: Add new custom mods here as semicolon-separated entries.
set "CUSTOM_MODS=@MinimapTweak;@HUDClock;@StackableItems;@MWGSM_TraderFix;@BAEZLoadingScreen;@EnableInventoryInVehicle"

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
) else if "!SELF_UPDATE!"=="1" (
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


:: Fix 3: Expansion Animations — REMOVED
:: The stripped PBO (missing .anm clips) caused native crashes in both
:: CreateCharacterPerson (client mannequin) and CreateObjectExSafe (server
:: quest NPCs). Using the full unmodified Workshop PBO instead.
:: If SurvivorAnims sit/fire conflicts resurface, investigate which specific
:: .anm files conflict rather than stripping all of them.

:: Fix 4: Expansion Quests GUI — wider quest tracker HUD
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

:: Fix 5: VanillaPlusPlusMap — null-pointer crash on game exit
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

:: Fix 6: DayZ-Dog — IntroSceneCharacter startup crash (null m_CharacterDta)
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

if "!FIXES_APPLIED!"=="0" echo   [OK]   No known issues found
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
:: Step 6: Launch DayZ
:: ============================================================
echo ============================================
echo  Launching DayZ with mods: %CUSTOM_MODS%
echo ============================================
echo.

:: Launch DayZ through Steam with custom mod args
:: Steam URI launches the game via Steam so overlay and auth work
start "" "steam://run/221100//-mod=%CUSTOM_MODS%/"

echo DayZ is starting. You can close this window.
popd
timeout /t 5
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
