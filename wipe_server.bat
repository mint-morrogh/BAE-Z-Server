@echo off
:: ============================================================
:: Blood ^& Barter - Full Server Wipe
:: ============================================================
:: Wipes ALL player data, world state, and caches for a fresh
:: start. Server configs and mods are NOT touched.
::
:: IMPORTANT: Stop the server before running this!
:: ============================================================

setlocal
pushd "%~dp0"

echo ============================================
echo  Blood ^& Barter - Full Server Wipe
echo ============================================
echo.
echo This will DELETE all of the following:
echo   - Player characters and inventory
echo   - World state (bases, stashes, dropped items)
echo   - Dog and horse ownership data
echo   - Skill tree progress
echo   - Expansion mod settings (regenerate on next boot)
echo   - Script caches (server + client)
echo.
echo Server configs, mods, and balance settings are NOT affected.
echo.
echo MAKE SURE THE SERVER IS STOPPED FIRST!
echo.
set /p CONFIRM="Type WIPE to confirm, or anything else to cancel: "
if /i not "%CONFIRM%"=="WIPE" (
    echo Cancelled. Nothing was deleted.
    pause
    exit /b 0
)

echo.
echo --- Wiping server storage ---

:: Mission-folder persistence (player DB + world state)
if exist "mpmissions\dayzOffline.chernarusplus\storage_0" (
    rd /s /q "mpmissions\dayzOffline.chernarusplus\storage_0" 2>nul
    mkdir "mpmissions\dayzOffline.chernarusplus\storage_0"
    echo   [OK] storage_0 wiped
)
if exist "mpmissions\dayzOffline.chernarusplus\storage_1" (
    rd /s /q "mpmissions\dayzOffline.chernarusplus\storage_1" 2>nul
    mkdir "mpmissions\dayzOffline.chernarusplus\storage_1"
    echo   [OK] storage_1 wiped
)

:: Mod-specific player data
if exist "config\Dayz-Dog\players" (
    rd /s /q "config\Dayz-Dog\players" 2>nul
    mkdir "config\Dayz-Dog\players"
    echo   [OK] Dog player data wiped
)
if exist "config\Zenarchist\Skills\PlayerDB" (
    rd /s /q "config\Zenarchist\Skills\PlayerDB" 2>nul
    mkdir "config\Zenarchist\Skills\PlayerDB"
    echo   [OK] Skill tree progress wiped
)
if exist "config\ExpansionMod\Settings" (
    rd /s /q "config\ExpansionMod\Settings" 2>nul
    mkdir "config\ExpansionMod\Settings"
    echo   [OK] Expansion settings wiped (will regenerate)
)

:: Script cache
del /q "config\DataCache\cache.ch" 2>nul
del /q "config\DataCache\cache_lock" 2>nul
echo   [OK] Server script cache cleared

echo.
echo --- Wiping client character data ---

:: Client-side character cache
del /q "%USERPROFILE%\Documents\DayZ\chars.DayZProfile" 2>nul
del /q "%USERPROFILE%\Documents\DayZ\profile.vars.DayZProfile" 2>nul
del /q "%USERPROFILE%\Documents\DayZ\*_settings.DayZProfile" 2>nul
del /q "%LOCALAPPDATA%\DayZ\DataCache\cache.ch" 2>nul
echo   [OK] Client character data cleared

echo.
echo ============================================
echo  Wipe complete! Fresh world ready.
echo  Start the server with start_server.bat
echo ============================================
echo.
popd
pause
exit /b 0
