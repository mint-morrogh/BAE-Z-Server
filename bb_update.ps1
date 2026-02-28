# ============================================================
# Blood & Barter — Download latest files from GitHub
# ============================================================
# Called by launch_dayz.bat when git is not installed.
# Downloads key scripts and custom mods from the repo.
#
# Exit codes:
#   0  = up to date (or updates applied, no restart needed)
#   99 = launch_dayz.bat was updated — caller should restart
#   1  = could not reach GitHub
# ============================================================

param([string]$Dir)

$raw = "https://raw.githubusercontent.com/mint-morrogh/DayZServer/main"
$selfUpdated = $false

# ---- Test connectivity by downloading launcher ----
try {
    Invoke-WebRequest "$raw/launch_dayz.bat" -OutFile "$Dir\launch_dayz.bat.new" -UseBasicParsing | Out-Null
} catch {
    Write-Host "  [SKIP] Cannot reach GitHub - continuing offline"
    exit 1
}

# ---- Check if launcher changed ----
$old = Get-Content "$Dir\launch_dayz.bat" -Raw -ErrorAction SilentlyContinue
$new = Get-Content "$Dir\launch_dayz.bat.new" -Raw
# Normalize line endings for comparison
if (($old -replace "`r`n","`n") -ne ($new -replace "`r`n","`n")) {
    Move-Item "$Dir\launch_dayz.bat.new" "$Dir\launch_dayz.bat" -Force
    Write-Host "  [UPD] launch_dayz.bat"
    $selfUpdated = $true
} else {
    Remove-Item "$Dir\launch_dayz.bat.new" -Force
    Write-Host "  [OK]  launch_dayz.bat"
}

# ---- Download other scripts ----
foreach ($f in @("install_mods.bat", "update.bat", "bb_update.ps1")) {
    try {
        Invoke-WebRequest "$raw/$f" -OutFile "$Dir\$f" -UseBasicParsing | Out-Null
        Write-Host "  [OK]  $f"
    } catch {
        Write-Host "  [SKIP] $f"
    }
}

# ---- Download MinimapTweak custom mod ----
$mt = "$Dir\@MinimapTweak"
$mtAddons = "$mt\addons"
if (!(Test-Path $mtAddons)) { New-Item -ItemType Directory -Path $mtAddons -Force | Out-Null }

foreach ($f in @("mod.cpp", "meta.cpp")) {
    try { Invoke-WebRequest "$raw/@MinimapTweak/$f" -OutFile "$mt\$f" -UseBasicParsing | Out-Null } catch {}
}
try { Invoke-WebRequest "$raw/@MinimapTweak/addons/MinimapTweak.pbo" -OutFile "$mtAddons\MinimapTweak.pbo" -UseBasicParsing | Out-Null } catch {}
Write-Host "  [OK]  @MinimapTweak"

if ($selfUpdated) { exit 99 } else { exit 0 }
