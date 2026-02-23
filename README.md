# Mint Mundane DayZ Server

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
7. Set your passwords in `serverDZ.cfg` (see [Passwords](#passwords))
8. Double-click `start.bat` to launch the server
9. Connect via DayZ > Servers > LAN, or Direct Connect to `127.0.0.1:2302`

### Passwords

Edit `serverDZ.cfg` and change these two lines to your own passwords:

```
password = "mintmorrogh";          // Password players enter to join
passwordAdmin = "mintmorrogh";     // Admin password for in-game commands
```

To use admin commands in-game, open chat and type `#login <your admin password>`.

## Server Configuration

### General Settings (`serverDZ.cfg`)

| Setting | Value | Description |
|---|---|---|
| `hostname` | Mint Mundane Server | Server name shown in browser |
| `maxPlayers` | 4 | Max concurrent players |
| `disable3rdPerson` | 0 | Third-person view enabled |
| `disableCrosshair` | 0 | Crosshair enabled |
| `verifySignatures` | 2 | Full mod signature verification |
| `forceSameBuild` | 1 | Clients must match server version |
| `enableCfgGameplayFile` | 1 | Loads cfggameplay.json settings |
| `storageAutoFix` | 1 | Auto-repairs corrupted persistence files |

### Time Settings

| Setting | Value | Effect |
|---|---|---|
| `serverTime` | 2000/07/01/08/00 | Starts at 8:00 AM (summer) |
| `serverTimeAcceleration` | 6 | Full day/night cycle in ~4 real hours |
| `serverNightTimeAcceleration` | 4 | Nights pass in ~22 minutes |
| `serverTimePersistent` | 1 | Time saves between restarts |

### Gameplay Settings (`mpmissions/dayzOffline.chernarusplus/cfggameplay.json`)

The gameplay config is loaded via `enableCfgGameplayFile = 1` in serverDZ.cfg. Edit `cfggameplay.json` to customize stamina, map behavior, base building rules, temperatures, and more.

### Zombie Spawns (`mpmissions/dayzOffline.chernarusplus/db/events.xml`)

Zombie spawns have been customized for a small-group experience:

| Zombie Type | Nominal / Min / Max | Notes |
|---|---|---|
| City (regular) | 50 / 25 / 250 | Full vanilla density |
| City (coastal/Tier 1) | 12 / 6 / 60 | 25% of vanilla — lighter starts |
| Village (coastal/Tier 1) | 12 / 6 / 25 | 25% of vanilla |
| Solitude (wilderness/forest) | 5 / 2 / 15 | Very rare — forests are mostly clear |
| Military | 50 / 25 / 250 | Full vanilla density |
| All others (police, medic, etc.) | Unchanged | Vanilla defaults |

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

Skill-related items spawn across the map via `mpmissions/dayzOffline.chernarusplus/custom/types_zenskills.xml`:

| Item | Spawns | Locations |
|---|---|---|
| EXP Boost Injector | 10 (min 5) | Medical, Military, Police, Towns |
| Perk Reset Injector | 4 (min 2) | Medical, Military |
| Survival Book | 6 (min 3) | Everywhere |
| Crafting Book | 6 (min 3) | Everywhere |
| Hunting Book | 6 (min 3) | Everywhere |
| Gathering Book | 6 (min 3) | Everywhere |

Items respawn automatically when the count drops below the minimum.

## Launching the Server

1. Double-click `start.bat`
2. Wait 1-2 minutes for the server to load
3. **Do not press any keys** in the command prompt — any keypress kills the server
4. The server auto-restarts every 4 hours

## Connecting

- **LAN:** DayZ > Servers > LAN tab > "Mint Mundane Server"
- **Direct Connect:** `<server IP>:2302`
- **Password:** Whatever you set in `serverDZ.cfg` (default: `mintmorrogh`)

## File Structure

```
DayZServer/
├── serverDZ.cfg                 # Main server config
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
        ├── cfggameplay.json     # Gameplay settings (stamina, map, building)
        ├── cfgeconomycore.xml   # Economy config (references custom types)
        ├── custom/
        │   └── types_zenskills.xml  # ZenSkills item spawn config
        └── db/
            └── events.xml       # Zombie spawn configuration
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
