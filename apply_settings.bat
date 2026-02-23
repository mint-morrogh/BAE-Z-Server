@echo off
echo ============================================
echo  DayZ Server Settings Patcher
echo ============================================
echo.
echo Reading server_settings.json and patching all config files...
echo.
powershell -ExecutionPolicy Bypass -File "%~dp0apply_settings.ps1"
echo.
pause
