# DayZ Private Server Setup Guide

A private DayZ co-op server configured for small groups (2-4 players) on Chernarus. Designed for a relaxed survival experience with quality-of-life mods, faster day/night cycles, and customized zombie spawns.

## Quick Start

### Prerequisites

- [DayZ](https://store.steampowered.com/app/221100/DayZ/) (game)
- [DayZ Server](https://store.steampowered.com/app/223350/DayZ_Server/) (free, under Steam Library > Tools)
- All mods listed below subscribed in the Steam Workshop

### Setup

1. Install **DayZ Server** from Steam (Library > Tools)
2. Clone this repo into your DayZ Server directory, or copy the config files over your installation
3. Subscribe to all required mods in the Steam Workshop (see [Mods](#mods))
4. Launch DayZ once so Steam downloads the mod files
5. Copy each mod's Workshop folder into the server directory with the correct `@Name` (see [Mod Installation](#mod-installation))
6. Copy each mod's `.bikey` file from their `keys/` folder into the server's `keys/` folder
7. Edit `server_settings.json` to customize your server (see [Settings Patcher](#settings-patcher))
8. Double-click `apply_settings.bat` to apply your settings
9. Double-click `start.bat` to launch the server
10. Connect via DayZ > Servers > LAN, or Direct Connect to `127.0.0.1:2302`

## Settings Patcher

Instead of manually editing multiple config files scattered across the server, **all important settings live in one file**: `server_settings.json`.

### How to use

1. Open `server_settings.json` in any text editor
2. Change whatever you want
3. Double-click `apply_settings.bat`
4. Restart your server

That's it. The patcher reads your settings and writes them to the correct config files automatically.

### What you can configure

| Section | What it controls | Config file it patches |
|---|---|---|
| `server` | Name, password, admin password, max players | `serverDZ.cfg` |
| `time` | Day/night speed, start time, persistence | `serverDZ.cfg` |
| `network` | Voice chat, 3rd person, crosshair | `serverDZ.cfg` |
| `gameplay` | Stamina, base damage, map features, base building rules | `cfggameplay.json` |
| `zombies` | Spawn rates per category (city, coastal, military, wilderness, etc.) | `db/events.xml` |
| `lootEconomy` | Global zombie/animal caps, loot damage, food decay, respawn timers | `db/globals.xml` |
| `zenSkills` | Skill book and EXP booster spawn rates | `custom/types_zenskills.xml` |

### Example: Common tweaks

**Change your password:**
```json
"server": {
    "password": "mynewpassword",
    "adminPassword": "myadminpass"
}
```

**Make nights shorter (or remove them):**
```json
"time": {
    "acceleration": 6,
    "nightAcceleration": 12
}
```
Higher `nightAcceleration` = shorter nights. Set `acceleration` to 1 for real-time.

**Disable all zombies in the wilderness:**
```json
"zombies": {
    "wilderness": { "nominal": 0, "min": 0, "max": 0 }
}
```

**Double zombies at military bases:**
```json
"zombies": {
    "military":     { "nominal": 100, "min": 50, "max": 500 },
    "militaryHard": { "nominal": 100, "min": 50, "max": 500 }
}
```

**Show your position on the map (easier navigation):**
```json
"gameplay": {
    "showPlayerOnMap": true,
    "mapWithoutItem": true
}
```

**Relaxed base building (place anywhere):**
```json
"gameplay": {
    "relaxedBaseBuilding": true
}
```

**More stamina, less punishing weight:**
```json
"gameplay": {
    "staminaMax": 200.0,
    "staminaWeightThreshold": 12000.0,
    "sprintStaminaModifier": 0.5
}
```

**More EXP boosters on the map:**
```json
"zenSkills": {
    "expBoostInjector": { "nominal": 20, "min": 10 }
}
```

### Passwords

The default password is `mintmorrogh`. Change it in `server_settings.json` under the `server` section, then run `apply_settings.bat`.

To use admin commands in-game, open chat and type `#login <your admin password>`.

## Server Configuration Details

### Time Settings

| Setting | Default | Effect |
|---|---|---|
| `startTime` | 2000/07/01/08/00 | Server starts at 8:00 AM in summer |
| `acceleration` | 6 | Full day/night cycle in ~4 real hours |
| `nightAcceleration` | 4 | Nights pass in ~22 minutes |
| `persistent` | true | Time saves between restarts |

### Zombie Spawns

Zombie spawns have been customized for a small-group experience:

| Category | Nominal / Min / Max | Notes |
|---|---|---|
| City (regular) | 50 / 25 / 250 | Full vanilla density |
| City (coastal/Tier 1) | 12 / 6 / 60 | 25% of vanilla — lighter starts |
| Village (coastal/Tier 1) | 12 / 6 / 25 | 25% of vanilla |
| Wilderness (forest) | 5 / 2 / 15 | Very rare — forests are mostly clear |
| Military | 50 / 25 / 250 | Full vanilla density |
| All others (police, medic, etc.) | 50 / 25 / 100 | Vanilla defaults |

### Loot Economy

| Setting | Default | What it does |
|---|---|---|
| `zombieMaxCount` | 1000 | Max zombies alive server-wide |
| `animalMaxCount` | 200 | Max animals alive server-wide |
| `lootDamageMin/Max` | 0.0 / 0.82 | Condition range for spawned loot |
| `foodDecay` | 1 | Food spoilage (1=on, 0=off) |
| `respawnLimit` | 20 | Max loot respawn cycles per restart |
| `flagRefreshFrequency` | 432000 | Territory flag refresh interval (seconds) |
| `flagRefreshMaxDuration` | 3456000 | Max time a flag stays active (seconds) |

## Mods

All mods must be installed on both the **server** and **client**.

| Mod | Workshop ID | Description |
|---|---|---|
| [CF (Community Framework)](https://steamcommunity.com/sharedfiles/filedetails/?id=1559212036) | 1559212036 | Required dependency for other mods |
| [Trader](https://steamcommunity.com/sharedfiles/filedetails/?id=1590841260) | 1590841260 | NPC traders, safe zones, buy/sell items |
| [Mass's Many Item Overhaul](https://steamcommunity.com/sharedfiles/filedetails/?id=1566911166) | 1566911166 | Adds weapons, clothing, crafting recipes |
| [UnlimitedRun](https://steamcommunity.com/sharedfiles/filedetails/?id=2040872847) | 2040872847 | Unlimited stamina for sprinting |
| [VanillaPlusPlusMap](https://steamcommunity.com/sharedfiles/filedetails/?id=1623711988) | 1623711988 | Enhanced map with markers and details |
| [GoreZ](https://steamcommunity.com/sharedfiles/filedetails/?id=1648967877) | 1648967877 | Enhanced blood and gore effects |
| [Inventory Move Sounds](https://steamcommunity.com/sharedfiles/filedetails/?id=2444247391) | 2444247391 | Adds sounds when moving items in inventory |
| [No Sprinting Zombies](https://steamcommunity.com/sharedfiles/filedetails/?id=1816010715) | 1816010715 | Zombies walk and jog but don't sprint |
| [Zenarchist's Skills](https://steamcommunity.com/sharedfiles/filedetails/?id=3601119520) | 3601119520 | Skill perk tree — survival, crafting, hunting, gathering |

### Mod Installation

Workshop mods download to `Steam\steamapps\workshop\content\221100\`. Copy each folder into the server directory and rename:

| Workshop Folder | Rename To |
|---|---|
| `1559212036` | `@CF` |
| `1590841260` | `@Trader` |
| `1566911166` | `@Mass'sManyItemOverhaul` |
| `2040872847` | `@UnlimitedRun` |
| `1623711988` | `@VanillaPlusPlusMap` |
| `1648967877` | `@GoreZ` |
| `2444247391` | `@Inventory-Move-Sounds` |
| `1816010715` | `@No-Sprinting-Zombies` |
| `3601119520` | `@ZenSkills` |

### Zenarchist's Skills — Loot Spawns

Skill-related items spawn across the map (configurable in `server_settings.json` under `zenSkills`):

| Item | Default Spawns | Locations |
|---|---|---|
| EXP Boost Injector | 10 (min 5) | Medical, Military, Police, Towns |
| Perk Reset Injector | 4 (min 2) | Medical, Military |
| Survival Book | 6 (min 3) | Everywhere |
| Crafting Book | 6 (min 3) | Everywhere |
| Hunting Book | 6 (min 3) | Everywhere |
| Gathering Book | 6 (min 3) | Everywhere |

Items respawn automatically when the count drops below the minimum.

## Launching the Server

1. Double-click `apply_settings.bat` (if you changed any settings)
2. Double-click `start.bat`
3. Wait 1-2 minutes for the server to load
4. **Do not press any keys** in the command prompt — any keypress kills the server
5. The server auto-restarts every 4 hours

## Connecting

- **LAN:** DayZ > Servers > LAN tab
- **Direct Connect:** `<server IP>:2302`
- **Password:** Whatever you set in `server_settings.json` (default: `mintmorrogh`)

## File Structure

```
DayZServer/
├── server_settings.json         # <-- EDIT THIS: All server settings in one place
├── apply_settings.bat           # Double-click to apply settings
├── apply_settings.ps1           # PowerShell patcher (called by .bat)
├── serverDZ.cfg                 # Main server config (patched automatically)
├── start.bat                    # Launch script with mod list
├── whitelist.txt                # Player whitelist (disabled by default)
├── keys/                        # Mod signature keys (.bikey)
├── @CF/                         # Community Framework mod
├── @Trader/                     # Trader mod
├── @Mass'sManyItemOverhaul/     # Item overhaul mod
├── @UnlimitedRun/               # Unlimited stamina mod
├── @VanillaPlusPlusMap/         # Enhanced map mod
├── @GoreZ/                      # Gore effects mod
├── @Inventory-Move-Sounds/      # Inventory sound mod
├── @No-Sprinting-Zombies/       # Zombie behavior mod
├── @ZenSkills/                  # Skill perk tree mod
└── mpmissions/
    └── dayzOffline.chernarusplus/
        ├── cfggameplay.json     # Gameplay settings (patched automatically)
        ├── cfgeconomycore.xml   # Economy config (references custom types)
        ├── custom/
        │   └── types_zenskills.xml  # ZenSkills item spawns (patched automatically)
        └── db/
            ├── events.xml       # Zombie spawns (patched automatically)
            └── globals.xml      # Loot economy (patched automatically)
```

## Wiping the Server

To start fresh, delete everything inside:
```
mpmissions/dayzOffline.chernarusplus/storage_1/
```

## Troubleshooting

- **Server crashes on launch:** Check the latest `.RPT` log in the `config/` folder. Usually a mod incompatibility — remove mods from the `-mod=` line in `start.bat` one at a time to isolate.
- **Can't connect:** Make sure your client has the exact same mods enabled. Mod mismatch = connection refused.
- **No loot spawning:** Usually an XML syntax error. Validate your XML files at [codebeautify.org/xmlvalidator](https://codebeautify.org/xmlvalidator).
- **Zombies not spawning:** Same as above — check `events.xml` for syntax errors.
- **Settings not applying:** Make sure you ran `apply_settings.bat` after editing `server_settings.json`, then restarted the server.
