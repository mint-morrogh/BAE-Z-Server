# Blood & Barter — DayZ Private Server

A private DayZ co-op server configured for small groups (2-4 players) on Chernarus. Designed for a relaxed survival experience with quality-of-life mods, faster day/night cycles, and customized zombie spawns.

### Features

- **31 mods + 2 custom server-side mods** — all preconfigured and ready to go
- **GPS minimap** — on-screen minimap with player position via DayZ Expansion
- **Companion dogs** — 17 breeds of tameable dogs, equip collars/vests/gas masks, build dog houses
- **Rideable horses** — 5 horse colours, saddles, bridles, saddlebags, buildable stables, walk/trot/gallop/jump/swim
- **42 driveable vehicles** — tiered rarity: common trucks/SUVs (3 each), mid-tier sports cars (2 each), rare exotics (1 each)
- **Flyable planes** — DC-3, Spitfire, Cessna 180, Catalina seaplane, Tigermoth, Stuntplane (14 planes across the map)
- **Care package supply drops** — military, medical, survival, and building supplies parachute in every 60 minutes with zombie guards
- **700+ crafting recipes** — weapons, armor, ammo, vehicles, bushcraft, NBC gear, and more via Nemsis Craftingpack
- **Advanced cooking** — 30+ recipes for dishes, soups, sausages, and cheese via CookZ
- **Skill tree** — earn XP and unlock perks in survival, crafting, hunting, and gathering
- **Treasure hunting** — find photos, travel to the location, dig up randomized loot stashes
- **NPC traders** — buy and sell items at safe zones
- **Roaming military trader** — a helicopter trader lands every 60 minutes near players, stays 30 minutes, trades weapons/ammo/medical/attachments for Roubles
- **Day/night zombie system** — ~85% of zombies culled during daytime (6am-8pm), full spawns at night with rare sprinters
- **Half-damage zombies** — zombie strength set to 0.5x vanilla day and night, gear lasts much longer
- **Zombie kill drops** — zombies drop loot when killed: food/bandages from civilians, ammo from military/police, medical supplies from doctors
- **Campfire health regen** — players near a lit fire slowly regenerate health (+2) and blood (+5) every 10 seconds
- **Abundant wildlife** — 80 deer, 100 roe deer, 50 goats, 40 each of cow/pig/sheep, 20 wild boar, plus boosted foxes, hares, and hens
- **Generous loot** — doubled canned food/drinks, 1.5x snacks and candy, tripled cooking pots, enabled crab cans
- **Boosted foraging** — doubled mushroom spawns under trees, boosted fruit drops from apple/pear/plum trees
- **Shake fruit trees** — hold action on fruit trees to shake loose 2-4 apples/pears/plums (75% drop chance, 30-min cooldown per tree)
- **Pristine loot** — items spawn Pristine to Worn only (no more Damaged/Badly Damaged spawns)
- **45-day item persistence** — dropped items last 45 real days on the ground, survive server restarts
- **Indestructible bases** — base structures and storage containers cannot be damaged
- **Reduced shoe damage** — crawler zombie boot damage reduced from 5.0 to 1.0
- **Sleep till morning** — if all players lie down to sleep at night, time skips to dawn
- **Unlimited stamina** — no stamina drain while sprinting
- **One-file configuration** — all server settings in `server_settings.json`, applied with a single click
- **Auto-restart** — server automatically restarts every 4 hours
- **63 modded backpacks** — 13 backpack models in multiple camo/colour variants, found at military, hunting, town, village, and farm locations
- **Full persistence** — bases, vehicles, and inventory survive restarts

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
| `zombieBehavior` | Zombie speed, health, damage, and vision — separate day/night values | PvZmoD config XMLs |
| `animals` | Wild dog and horse herd spawn counts | `db/events.xml` |
| `horseGear` | Saddle, bridle, saddlebag, and stable kit spawn rates | `custom/types_dayzhorse.xml` |
| `dogGear` | Collar, vest, gas mask, and dog house kit spawn rates | `custom/types_dayzdog.xml` |
| `planes` | Per-model plane spawn counts (DC-3, Spitfire, Cessna, etc.) | `custom/types_lmplanes.xml` |
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

**Make zombies sprint during the day too:**
```json
"zombieBehavior": {
    "speedClampMaxDay": 3.0,
    "speedRatioDay": 1.0
}
```

**Make zombies vanilla strength at all times:**
```json
"zombieBehavior": {
    "healthRatioDay": 1.0,
    "strengthRatioDay": 1.0,
    "visionRatioDay": 1.0
}
```

**More wild dogs and horses:**
```json
"animals": {
    "wildDogs":   { "nominal": 12, "min": 3, "max": 10 },
    "wildHorses": { "nominal": 15, "min": 5, "max": 8 }
}
```

**More saddles and bridles (easier to find horses):**
```json
"horseGear": {
    "saddle": { "nominal": 10, "min": 5 },
    "bridle": { "nominal": 10, "min": 5 }
}
```

**Remove all planes:**
```json
"planes": {
    "dc3":        { "nominal": 0, "min": 0 },
    "stuntplane": { "nominal": 0, "min": 0 },
    "catalina":   { "nominal": 0, "min": 0 },
    "tigermoth":  { "nominal": 0, "min": 0 },
    "tigermothMk2": { "nominal": 0, "min": 0 },
    "tigermothMk3": { "nominal": 0, "min": 0 },
    "spitfire":   { "nominal": 0, "min": 0 },
    "cessna":     { "nominal": 0, "min": 0 }
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

Zombie spawns are set ~33% above half-vanilla baseline. A custom server-side mod (`@DayZombieManager`) culls ~85% of spawns during daytime (6am-8pm), leaving only ~15% of zombies during the day. Full +33% density at night. The same mod also handles zombie kill drops.

| Category | Nominal / Min / Max | Notes |
|---|---|---|
| City (regular) | 33 / 16 / 120 | ~Half vanilla by day, +33% at night |
| City (coastal/Tier 1) | 8 / 4 / 30 | Light — easier coastal starts |
| Village | 27 / 13 / 50 | Moderate |
| Village (coastal/Tier 1) | 8 / 4 / 15 | Light |
| Wilderness (forest) | 3 / 1 / 8 | Very rare — forests are mostly clear |
| Military | 33 / 16 / 120 | Dangerous at night |
| All others (police, medic, etc.) | 27 / 13 / 50 | Reduced |

### Loot Economy

| Setting | Default | What it does |
|---|---|---|
| `zombieMaxCount` | 500 | Max zombies alive server-wide |
| `animalMaxCount` | 200 | Max animals alive server-wide |
| `lootDamageMin/Max` | 0.0 / 0.2 | Loot spawns Pristine to Worn only |
| `foodDecay` | 1 | Food spoilage (1=on, 0=off) |
| `respawnLimit` | 20 | Max loot respawn cycles per restart |
| `flagRefreshFrequency` | 432000 | Territory flag refresh interval (seconds) |
| `flagRefreshMaxDuration` | 3456000 | Max time a flag stays active (seconds) |

## Mods

### Client + Server Mods

These mods must be installed on both the **server** and **client**.

| Mod | Workshop ID | Description |
|---|---|---|
| [CF (Community Framework)](https://steamcommunity.com/sharedfiles/filedetails/?id=1559212036) | 1559212036 | Required dependency for other mods |
| [DayZ-Expansion-Core](https://steamcommunity.com/sharedfiles/filedetails/?id=2291785308) | 2291785308 | Core framework for DayZ Expansion |
| [Dabs Framework](https://steamcommunity.com/sharedfiles/filedetails/?id=2545327648) | 2545327648 | Script/GUI framework dependency for Expansion |
| [DayZ-Expansion](https://steamcommunity.com/sharedfiles/filedetails/?id=2116151222) | 2116151222 | Expansion content — items, UI, and world enhancements |
| [DayZ-Expansion-Navigation](https://steamcommunity.com/sharedfiles/filedetails/?id=2792984722) | 2792984722 | Navigation scripts required by GPS minimap |
| [Expansion Minimap Override](https://steamcommunity.com/sharedfiles/filedetails/?id=3626138230) | 3626138230 | GPS minimap overlay with player position |
| [Trader](https://steamcommunity.com/sharedfiles/filedetails/?id=1590841260) | 1590841260 | NPC traders, safe zones, buy/sell items |
| [Mass's Many Item Overhaul](https://steamcommunity.com/sharedfiles/filedetails/?id=1566911166) | 1566911166 | Adds weapons, clothing, crafting recipes |
| [UnlimitedRun](https://steamcommunity.com/sharedfiles/filedetails/?id=2040872847) | 2040872847 | Unlimited stamina for sprinting |
| [VanillaPlusPlusMap](https://steamcommunity.com/sharedfiles/filedetails/?id=1623711988) | 1623711988 | Enhanced map with markers and details |
| [GoreZ](https://steamcommunity.com/sharedfiles/filedetails/?id=1648967877) | 1648967877 | Enhanced blood and gore effects |
| [Inventory Move Sounds](https://steamcommunity.com/sharedfiles/filedetails/?id=2444247391) | 2444247391 | Adds sounds when moving items in inventory |
| [PvZmoD Customisable Zombies](https://steamcommunity.com/sharedfiles/filedetails/?id=2051775667) | 2051775667 | Configurable zombie speed, health, damage, vision — day/night variants |
| [Zenarchist's Skills](https://steamcommunity.com/sharedfiles/filedetails/?id=3601119520) | 3601119520 | Skill perk tree — survival, crafting, hunting, gathering |
| [CZ Optics](https://steamcommunity.com/sharedfiles/filedetails/?id=3571068454) | 3571068454 | Additional optics and scopes |
| [PercentageHUD](https://steamcommunity.com/sharedfiles/filedetails/?id=3443562573) | 3443562573 | Shows health, blood, hunger, thirst, and stamina as percentages on the HUD |
| [Zen's Treasure](https://steamcommunity.com/sharedfiles/filedetails/?id=3426979799) | 3426979799 | Photo-based treasure hunting — find photos, travel to the location, dig up randomized loot stashes |
| [Care Packages V2](https://steamcommunity.com/sharedfiles/filedetails/?id=2691041685) | 2691041685 | Randomized care package drops with notifications, locked containers, and custom loot |
| [Nemsis Craftingpack All-in-One](https://steamcommunity.com/sharedfiles/filedetails/?id=3606014796) | 3606014796 | 500+ items, 700+ crafting recipes — weapons, armor, vehicles, bushcraft, ammo crafting, and more |
| [LMs Planes](https://steamcommunity.com/sharedfiles/filedetails/?id=3639695989) | 3639695989 | Flyable planes — DC-3, Cessna 180, Spitfire, Catalina seaplane, Tigermoth, Stuntplane |
| [CookZ](https://steamcommunity.com/sharedfiles/filedetails/?id=3302732231) | 3302732231 | Advanced cooking — 30+ recipes for dishes, soups, sausages, marmalades, cheese |
| [CookZ Realistic Packaging](https://steamcommunity.com/sharedfiles/filedetails/?id=3566508757) | 3566508757 | Realistic textures for CookZ food packaging |
| [Sleep Till Morning](https://steamcommunity.com/sharedfiles/filedetails/?id=3578708533) | 3578708533 | Skip to dawn when all players lie down to sleep at night |
| [4KBOSSKVehicles](https://steamcommunity.com/sharedfiles/filedetails/?id=3369325490) | 3369325490 | 21 driveable vehicles — muscle cars, trucks, SUVs, sports cars, monster truck, motorhome |
| [DayZ-Dog](https://steamcommunity.com/sharedfiles/filedetails/?id=2471347750) | 2471347750 | Tameable companion dogs — 17 breeds, collars, vests, gas masks, dog houses |
| [Survivor Animations](https://steamcommunity.com/sharedfiles/filedetails/?id=2918418331) | 2918418331 | Animation dependency required by DayZ Horse |
| [DayZ Horse](https://steamcommunity.com/sharedfiles/filedetails/?id=3295021220) | 3295021220 | Rideable horses — 5 colours, saddles, bridles, saddlebags, stables, walk/trot/gallop/jump/swim |
| [MWGSM Roaming Trader](https://steamcommunity.com/sharedfiles/filedetails/?id=3636799682) | 3636799682 | Helicopter-based roaming trader that lands near players with weapons, ammo, medical, and attachments |
| [Saga Shake Tree](https://steamcommunity.com/sharedfiles/filedetails/?id=3545040196) | 3545040196 | Shake fruit trees to drop apples, pears, and plums — configurable drop rates and cooldowns |
| [Pack Complete — Backpacks FREE](https://steamcommunity.com/sharedfiles/filedetails/?id=3648464156) | 3648464156 | 13 backpack models with 63 camo/colour variants — tactical, military, medical, and civilian packs |

### Mod Installation

Workshop mods download to `Steam\steamapps\workshop\content\221100\`. Copy each folder into the server directory and rename:

| Workshop Folder | Rename To |
|---|---|
| `1559212036` | `@CF` |
| `2291785308` | `@DayZ-Expansion-Core` |
| `2545327648` | `@DabsFramework` |
| `2116151222` | `@DayZ-Expansion` |
| `2792984722` | `@DayZ-Expansion-Navigation` |
| `3626138230` | `@ExpansionMinimap` |
| `1590841260` | `@Trader` |
| `1566911166` | `@Mass'sManyItemOverhaul` |
| `2040872847` | `@UnlimitedRun` |
| `1623711988` | `@VanillaPlusPlusMap` |
| `1648967877` | `@GoreZ` |
| `2444247391` | `@Inventory-Move-Sounds` |
| `2051775667` | `@PvZmoD` |
| `3601119520` | `@ZenSkills` |
| `3571068454` | `@CZOptics` |
| `3443562573` | `@PercentageHUD` |
| `3426979799` | `@ZenTreasure` |
| `2691041685` | `@CarePackages` |
| `3606014796` | `@NemsisCraftingpack` |
| `3639695989` | `@LMsPlanes` |
| `3302732231` | `@CookZ` |
| `3566508757` | `@CookZRealisticPackaging` |
| `3578708533` | `@SleepTillMorning` |
| `3369325490` | `@4KBOSSKVehicles` |
| `2471347750` | `@DayZDog` |
| `2918418331` | `@SurvivorAnims` |
| `3295021220` | `@DayZHorse` |
| `3636799682` | `@MWGSM_Trader` |
| `3545040196` | `@SagaShakeTree` |
| `3648464156` | `@GelyaBackpacks` |

### Gelya Backpacks — Pack Complete

13 backpack models with 63 colour/camo variants spawn across Chernarus alongside vanilla backpacks. Each variant has 2 copies on the map (min 1), except single-variant models and medpacks which spawn slightly more.

| Backpack | Variants | Locations |
|---|---|---|
| Backpack 5.11 | 5 (Black, Tan, DPM, Multicam, Flectarn) | Military, Hunting (Tier 2-4) |
| BEAR Bag | 6 (Black, Tan, Urban Spetsnaz, GreenTortilla, PartizanM, TokioESP) | Military, Hunting (Tier 2-4) |
| Beta 2 | 8 (Black, Green, Tan, Urban Spetsnaz, GreenTortilla, HexcamArid, RussianEMR, SnowUrban) | Military, Town (Tier 2-4) |
| Blood Silence | 9 (COD, Flectarn, GreenTortille, HexcamArid, Multicam, RussianEMR, Tan, Tigerstripe, Vegetato) | Military, Hunting (Tier 3-4) |
| Camel Backpack | 7 (Black, BlueCamo, KuwaitiPolice, Urban Spetsnaz, ERDL, HexcamArid, M90 Night) | Hunting, Town, Farm (Tier 1-3) |
| F5 Switchblade | 10 (Black, Green, GreenTortilla, M90 Night, MTZ16, Multicam, RussianEMR, Swapol, Tan, Woodland) | Military, Town (Tier 2-4) |
| Gunslinger | 7 (Black, BLR, Green, GreenTortilla, Tan, USCM, Woodland 90s) | Military, Hunting, Town (Tier 2-4) |
| Medpack | 2 (Black, EMR) | Medical, Town (Tier 1-3) |
| Sport Bag | 1 | Town, Village (Tier 1-2) |
| Stalker Bag | 1 | Hunting, Village, Farm (Tier 2-3) |
| Wild Bag | 1 | Hunting, Village, Farm (Tier 1-3) |
| Adventure Bag | 5 (Black, DPM, GorkaE, Jigsaw, Zelt) | Town, Village, Hunting (Tier 1-3) |
| Assault G3 | 1 | Military (Tier 3-4) |

Backpack spawns are configured in `custom/types_gelyabackpacks.xml`.

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

## Dynamic Events

### Static Events (events.xml)

| Event | Vanilla | This Server | Effect |
|---|---|---|---|
| Helicopter crashes | 3 | 5 (min 1) | More crash sites with military loot |
| Military convoys | 5 | 8 (min 2) | More ambushed convoys on roads |
| Contaminated zones | 2-4 | Unchanged | Dynamic NBC zones |

### MWGSM Roaming Trader

A military helicopter trader lands near players every 60 minutes and stays for 30 minutes. The trader sells weapons, ammunition, medical supplies, and attachments for **Roubles** (found as loot around the map). Players are notified with coordinates when the trader arrives and warned 5 minutes before departure.

| Setting | Value |
|---|---|
| Spawn interval | Every 60 minutes |
| Duration | 30 minutes |
| Currency | Roubles |
| Stock | 150 items (weapons, ammo, medical, attachments) |
| Guaranteed stock | FAL, AKM, M4A1 |
| Spawn distance | 500-2000m from players |

Config at `config/MWGSM_RoamingTrader/MWGSM_RoamingTraderConfig.json`.

### Care Packages

Randomized supply drops land across the map every 60 minutes. Players are notified in-game with the drop location and distance. Packages are **locked** on drop — press F to open.

| Package Type | Container | Loot |
|---|---|---|
| Military | Typhon camo | 1-3 guns (FAL, AKM, SVD, M4, Mosin, SKS, VSS, Scout) + ammo, mags, optics, NVGs |
| Medical | Red | 8-15 medical items — bandages, morphine, antibiotics, iodine, saline, blood bags |
| Survival | Green | 10-18 food/drink items — canned food, sodas, water, rice + hunting knife or shotgun |
| Tools & Building | Black | 8-14 items — nails, pliers, hacksaw, rope, duct tape, code lock, planks + axes |

17 drop locations spread across Chernarus. 6 zombies guard each drop. Edit loot, locations, and timing in `config/CarePackagesV2/config.json`.

### Nemsis Craftingpack

Adds 500+ craftable items and 700+ recipes. Crafting is vanilla-style — no workbenches needed. Players discover recipes by finding **recipe sketches** in the world, which can be collected in a notebook.

Crafting categories: bushcrafting, makeshift weapons, ammo crafting, armor, improvised NBC gear, vehicles, raiding tools, signs, writing, repairing, and more.

Item spawns are configured in `custom/types_nemsis.xml`.

> **Note:** The Airbrushing addon (`nm_Crafting_Part_Airbrushing.pbo`) was removed from `@NemsisCraftingpack/addons/` because it references classes from a newer version of Mass's Many Item Overhaul than this server uses. This only removes item painting/recoloring — all 700+ crafting recipes still work.

### LMs Planes

Flyable planes spawn across the map at industrial areas and farms (removed town/village spawns to avoid clipping into buildings). Find spare wheels at industrial locations to repair them.

| Plane | Count | Notes |
|---|---|---|
| DC-3 | 2 | Large transport plane |
| Stuntplane | 2 | Aerobatic biplane |
| Catalina | 2 | Amphibious seaplane — lands on water |
| Tigermoth | 2 | Classic biplane |
| Tigermoth MK2 | 1 | Variant |
| Tigermoth MK3 | 1 | Variant |
| Spitfire | 2 | WWII fighter |
| Cessna 180 | 2 | Utility plane |
| **Total** | **14** | Keep under 15-30 for performance |

Plane spawns are configured in `custom/types_lmplanes.xml`.

### 4KBOSSKVehicles

42 driveable vehicles spawn across the map with tiered rarity — common everyday vehicles are easy to find, rare exotics take effort to locate:

| Vehicle | Count | Tier | Location |
|---|---|---|---|
| Honda Civic | 3 | Common | Town, Village |
| Ford Bronco | 3 | Common | Farm, Village |
| Toyota 4Runner | 3 | Common | Farm, Village |
| Dodge Ram 3500 | 3 | Common | Farm, Industrial |
| Chevy Tahoe | 3 | Common | Town, Farm |
| MotorHome | 3 | Common | Town, Farm |
| Chevrolet Napalm Nova | 3 | Common | Town, Farm |
| Audi RS6 ABT | 2 | Medium | Town, Industrial |
| BMW M3 | 2 | Medium | Town, Industrial |
| Dodge Challenger SRT | 2 | Medium | Town, Industrial |
| Dodge Charger Hellcat | 2 | Medium | Town, Industrial |
| Nissan Skyline | 2 | Medium | Town, Industrial |
| Jeep Gladiator F9 | 2 | Medium | Farm, Industrial |
| Ford Mustang Shelby GT500 | 2 | Medium | Town, Industrial |
| Dodge Ram (camo) | 2 | Medium | Farm, Military |
| Porsche 911 RWB | 1 | Rare | Town, Industrial |
| Toyota Supra MK IV | 1 | Rare | Town, Industrial |
| Nissan GTR Nismo | 1 | Rare | Town, Industrial |
| Ford Raptor Monster Truck | 1 | Rare | Farm, Industrial |
| Kamaz Typhoon K | 1 | Rare | Military |

Vehicle spawns are configured in `custom/types_4kbossk.xml`. The mod includes 163 color variants — change colors by swapping class names in the types file.

### CookZ

Adds 30+ craftable food recipes — dishes, soups, sausages, marmalades, and cheese. All items are **crafted only** (nothing spawns in the world). Recipes use vanilla DayZ ingredients and cooking tools.

CookZ Realistic Packaging replaces the default food textures with realistic-looking packaging. It's a visual-only addon — no gameplay changes.

CookZ config auto-generates in `config/CookZ/` on first server start. Item definitions are in `custom/types_cookz.xml`.

### DayZ-Dog — Companion Dogs

17 breeds of tameable wild dogs roam Chernarus. Tame them by feeding raw meat or bones, then command them to follow, stay, or attack. Once tamed, dogs can be equipped with collars, K9 vests, and even gas masks.

Build a dog house from kits found at farms and villages to give your companion a home. Dog houses persist through restarts.

| Item | Count | Locations |
|---|---|---|
| Collars (7 colours) | 3 each (min 1) | Towns, Villages |
| K9 Vests (6 types) | 3 each (min 1) | Police stations |
| Gas Mask | 3 (min 1) | Military (Tier 3-4) |
| Dog House Kit (small) | 3 (min 1) | Villages, Towns, Farms |
| Dog House Kit (large) | 3 (min 1) | Villages, Towns, Farms |

Dog accessories are configured in `custom/types_dayzdog.xml`. The mod generates a server config at `config/Dayz-Dog/` on first start where you can adjust dog health and behaviour.

### DayZ Horse — Rideable Horses

5 horse colours (Brown, White, Gray, Dark Gray, Palomino) spawn in herds across Chernarus fields and farms. Approach a horse with a bridle to tame it, then equip a saddle to ride. Horses can walk, trot, gallop, jump fences, and swim.

Attach saddlebags for mobile storage. Build a stable from kits found at farms to shelter your horse between sessions.

| Item | Count | Locations |
|---|---|---|
| Saddle | 5 (min 3) | Farms, Villages |
| Bridle | 5 (min 3) | Farms, Villages |
| Horse Bags (saddlebags) | 4 (min 2) | Farms, Villages |
| Stable Kit | 3 (min 1) | Farms, Villages |

Horse tack is configured in `custom/types_dayzhorse.xml`. Requires the **Survivor Animations** mod as a dependency (included and loaded automatically).

### PvZmoD — Customisable Zombies

Fully configurable zombie behaviour system with independent day and night settings. **Daytime is forgiving** — fewer zombies that are slow, easier to kill, and have poor eyesight, letting players explore and loot in relative safety. **Nighttime is dangerous** — 33% more zombies with full vanilla vision, full strength, and rare sprinters, making darkness a real threat.

| Setting | Day | Night | Effect |
|---|---|---|---|
| Spawn count | ~85% culled (~15% survive) | +33% more than baseline | Custom server mod thins the herd by day |
| Speed | Walk/jog only | Walk/jog + rare sprinters | Day capped at run (2.5), night allows sprint (3.0) |
| Speed ratio | 0.75x | 0.95x | Further scales down — most zombies walk by day |
| Health | 0.7x (30% easier kills) | 1.0x (vanilla) | Daytime zombies go down fast |
| Damage | 0.5x (half vanilla) | 0.5x (half vanilla) | Gear lasts much longer from zombie fights |
| Vision | 0.65x (35% shorter) | 1.0x (vanilla) | Easier sneaking during day, full sight at night |
| Size | Normal (disabled) | Normal (disabled) | No random size scaling — all zombies player-sized |

All zombie settings are configurable in `server_settings.json` under `zombieBehavior` and applied automatically by the settings patcher. Changes can also be applied in-game without a restart by pressing **Numpad 4**.

Config files are auto-generated at `config/PvZmoD_CustomisableZombies_Profile/` on first server start.

#### Zombie Health Rebalance

The toughest common zombies (police, military, soldiers) have been rebalanced so they're still noticeably tougher than civilians but not tedious melee sponges. Boss types and walkers are unchanged.

| Zombie | Old HP | New HP | Day knife hits (0.7x) |
|---|---|---|---|
| Civilians / NBC Yellow / Paramedics | 33-100 | No change | 2-6 |
| Gamedev (Dean Hall) | 150 | 120 | 5-6 |
| NBC Grey, Police | 200 | 140 | 6-7 |
| Spec Force, Negan, Soldiers | 250 | 160 | 7-8 |
| Heavy Soldiers, Officer Convoy | 300 | 180 | 8-9 |
| Boss types (Santa, Priest, etc.) | 400-1000 | No change | By design |

### Zombie Kill Drops

Built into the `@DayZombieManager` server mod. Zombies have a chance to drop loot when killed, with different loot tables based on zombie type:

| Zombie Type | Drop Chance | Loot |
|---|---|---|
| Civilian (default) | 25% | Food (apple, pear, plum, canned food), bandages, batteries, matches |
| Military (soldiers) | 30% | Ammo boxes (5.56, 7.62, 9mm, 5.45), AK magazines, compass |
| Police | 30% | Pistol ammo (9mm, .45), pistol magazines, handcuffs, flashlight |
| Medical (doctor/nurse/paramedic) | 35% | Bandages, morphine, antibiotics, saline, vitamins, disinfectant |

Loot spawns on the ground at the zombie's death location. Source code in `mod_src/DayZombieManager/`.

### Campfire Health Regen

Custom server-side mod (`@CampfireRegen`). Players within 5 metres of a burning fire slowly regenerate health and blood:

- **+2 Health** per 10 seconds (max 100)
- **+5 Blood** per 10 seconds (max 5000)

Sitting by a campfire for 5 minutes heals ~60 health and ~150 blood. Not a replacement for medical items, but a quality-of-life reward for the campfire moment.

Source code in `mod_src/CampfireRegen/`.

### Sleep Till Morning

When **all players** on the server lie down to sleep at night, time fast-forwards to dawn. No more waiting around during nighttime. Uses the vanilla "Lie Down" emote — no fatigue system required.

### DayZ Expansion — GPS Minimap

Adds a GPS minimap to the HUD showing your position and surroundings. Requires three dependency mods that also add expanded items, UI enhancements, and framework utilities.

| Mod | Role |
|---|---|
| DayZ-Expansion-Core | Core framework (load first) |
| Dabs Framework | Script/GUI support library |
| DayZ-Expansion | Main content — items, UI, world enhancements |
| DayZ-Expansion-Navigation | Navigation scripts required by GPS overlay |
| Expansion Minimap Override | GPS minimap overlay (load last) |

Load order matters — Core and Dabs must load before Expansion, Navigation must load after Expansion, and the Minimap Override must load last. This is already configured in `start.bat`.

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
├── config/                      # Server profiles dir (-profiles=config)
│   ├── CarePackagesV2/
│   │   └── config.json          # Care package loot, locations, timing
│   ├── CookZ/                   # Auto-generated on first start
│   ├── Dayz-Dog/                # Auto-generated — dog health & behaviour config
│   ├── PvZmoD_CustomisableZombies_Profile/  # Auto-generated on first start
│   │   ├── PvZmoD_CustomisableZombies_Globals.xml           # Speed/health/damage ratios (patched)
│   │   ├── PvZmoD_CustomisableZombies_Characteristics.xml   # Per-zombie-type vision/speed/HP
│   │   └── PvZmoD_CustomisableZombies_HELP.txt              # Full docs for all settings
│   ├── MWGSM_RoamingTrader/
│   │   └── MWGSM_RoamingTraderConfig.json  # Roaming trader currency, prices, stock, timing
│   ├── SagaShakeTree/
│   │   └── settings.json        # Fruit drop chances, shake duration, tree cooldowns
│   └── *.RPT                    # Server crash/debug logs
├── @CF/                         # Community Framework mod
├── @DayZ-Expansion-Core/        # Expansion core framework
├── @DabsFramework/              # Script/GUI framework
├── @DayZ-Expansion/             # Expansion content
├── @DayZ-Expansion-Navigation/  # Navigation scripts for GPS
├── @ExpansionMinimap/           # GPS minimap overlay
├── @Trader/                     # Trader mod
├── @Mass'sManyItemOverhaul/     # Item overhaul mod
├── @UnlimitedRun/               # Unlimited stamina mod
├── @VanillaPlusPlusMap/         # Enhanced map mod
├── @GoreZ/                      # Gore effects mod
├── @Inventory-Move-Sounds/      # Inventory sound mod
├── @PvZmoD/                     # Customisable zombie behaviour
├── @ZenSkills/                  # Skill perk tree mod
├── @CZOptics/                   # Additional optics and scopes
├── @PercentageHUD/              # HUD percentage indicators
├── @ZenTreasure/                # Photo-based treasure hunting
├── @CarePackages/               # Care package supply drops
├── @NemsisCraftingpack/         # 500+ craftable items
├── @LMsPlanes/                  # Flyable planes
├── @CookZ/                      # Advanced cooking recipes
├── @CookZRealisticPackaging/    # Realistic food textures
├── @SleepTillMorning/           # Skip night when all players sleep
├── @4KBOSSKVehicles/            # 21 driveable vehicles
├── @DayZDog/                    # Companion dogs (17 breeds)
├── @SurvivorAnims/              # Animation dependency for horse mod
├── @DayZHorse/                  # Rideable horses (5 colours)
├── @MWGSM_Trader/               # Roaming military helicopter trader
├── @SagaShakeTree/              # Shake fruit trees for apples/pears/plums
├── @GelyaBackpacks/             # 13 backpack models (63 variants)
├── @DayZombieManager/           # Custom server-side mod — zombie culling + kill drops
│   └── addons/
│       └── DayZombieManager.pbo # Daytime culling + loot drops on kill
├── @CampfireRegen/              # Custom server-side mod — campfire healing
│   └── addons/
│       └── CampfireRegen.pbo    # Health/blood regen near lit fires
├── mod_src/                     # Source code for custom mods
│   ├── DayZombieManager/        # Zombie manager source (culling + kill drops)
│   ├── CampfireRegen/           # Campfire regen source
│   ├── pack_pbo.py              # PBO packer tool
│   └── rapify.py                # config.cpp to config.bin converter
└── mpmissions/
    └── dayzOffline.chernarusplus/
        ├── cfggameplay.json     # Gameplay settings (patched automatically)
        ├── cfgeconomycore.xml   # Economy config (references custom types)
        ├── custom/
        │   ├── types_zenskills.xml    # ZenSkills item spawns (patched automatically)
        │   ├── types_zentreasure.xml  # Zen's Treasure item spawns
        │   ├── types_nemsis.xml       # Nemsis Craftingpack item spawns
        │   ├── types_lmplanes.xml     # LMs Planes vehicle spawns
        │   ├── types_cookz.xml        # CookZ crafted item definitions
        │   ├── types_4kbossk.xml      # 4KBOSSKVehicles spawn config
        │   ├── types_dayzdog.xml     # Dog accessory spawns
        │   ├── types_dayzhorse.xml   # Horse tack spawns
        │   └── types_gelyabackpacks.xml  # Gelya Backpacks spawn config
        ├── env/
        │   ├── dog_territories.xml   # Wild dog spawn zones
        │   └── wild_horse_territories.xml  # Horse grazing zones
        └── db/
            ├── types.xml        # Item spawn rules (food, gear, tools — heavily customized)
            ├── events.xml       # Zombie/animal spawns (patched automatically)
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
- **Dogs/horses not spawning:** Make sure both `dog_territories.xml` and `wild_horse_territories.xml` exist in the `env/` folder, and that `cfgenvironment.xml` references them. Check the events in `db/events.xml` for `AnimalWildDog` and `AnimalWildHorse` entries.
- **Dog health too low:** After first server start, check `config/Dayz-Dog/` for a generated config file where you can increase dog health values.
