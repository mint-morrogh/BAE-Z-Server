@echo off
:: ============================================================
:: Blood ^& Barter - Launch DayZ Client
:: ============================================================
:: Launches DayZ with the custom client mods required to play
:: on the Blood ^& Barter server.
::
:: Usage: double-click this file instead of launching DayZ
::        through Steam.
:: ============================================================

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

:: Sync custom mods to the client folder first
call "%~dp0sync_client_mods.bat"

echo.
echo Launching DayZ with custom mods: %CUSTOM_MODS%
echo.

:: Launch DayZ through Steam with custom mod args
:: Steam URI launches the game via Steam so overlay and auth work
start "" "steam://run/221100//-mod=%CUSTOM_MODS%/"

echo DayZ is starting. You can close this window.
timeout /t 5
exit /b 0
