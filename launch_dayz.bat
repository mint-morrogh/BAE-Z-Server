@echo off
:: ============================================================
:: Blood ^& Barter - Launch DayZ Client
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
set "CUSTOM_MODS=@MinimapTweak"

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
echo  Blood ^& Barter - Launch DayZ
echo ============================================
echo.

:: ============================================================
:: Step 1: Pull latest config changes from GitHub
:: ============================================================
echo --- Pulling latest server updates ---
where git >nul 2>&1
if errorlevel 1 (
    echo   [SKIP] git not found - skipping update check
    echo          Install git to enable auto-updates
) else (
    git pull --ff-only 2>&1
    if errorlevel 1 (
        echo   [WARN] git pull failed - you may have local changes
        echo          Continuing with current files...
    ) else (
        echo   [OK]   Up to date
    )
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
set "NEMSIS_DIR=%WORKSHOP%\3606014796\addons"
if exist "%NEMSIS_DIR%\nm_Crafting_Part_Airbrushing.pbo" (
    del /q "%NEMSIS_DIR%\nm_Crafting_Part_Airbrushing.pbo" >nul 2>&1
    del /q "%NEMSIS_DIR%\nm_Crafting_Part_Airbrushing.pbo.n3msi.bisign" >nul 2>&1
    echo   [FIX]  Removed Nemsis Airbrushing PBO (Mass_ACR_Base compile error^)
) else (
    echo   [OK]   No known issues found
)
echo.

:: ============================================================
:: Step 4: Sync custom client mods to DayZ folder
:: ============================================================
echo --- Syncing custom client mods ---
echo   Server: %~dp0
echo   Client: %DAYZ_CLIENT%
echo.

call :sync_mod MinimapTweak

:: Clear client script cache so new PBOs are picked up
del /q "%LOCALAPPDATA%\DayZ\DataCache\cache.ch" >nul 2>&1
echo   [OK]   Client script cache cleared
echo.

:: ============================================================
:: Step 5: Launch DayZ
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
