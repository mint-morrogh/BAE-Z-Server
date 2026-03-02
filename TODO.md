# Blood & Barter - TODO

## Pending (server must be stopped)

### Vehicles: Random colors + all models for 4KBOSSK spawns
- **Issue:** Currently each of the 20 vehicles spawns as 1 fixed color (e.g. always a Black Audi, always a Blue BMW). Also missing several available models entirely (NissanGTR non-Nismo, NissanGTRCustom, GMC_BOSS, Mitsubishi Lancer Evo IX).
- **Goal:** Still only 1 of each vehicle model on the server, but the color is randomized each spawn. Add the missing models too.
- **How it works:** In `events.xml`, replace each single-color child with ALL color variants for that model, all sharing the same `max=1 min=1`. The CE picks one randomly from the pool per model.
- **Config location:** `mpmissions/dayzOffline.chernarusplus/db/events.xml` → `Vehicle4KBOSSK` event
- **Also needs:** `cfgspawnabletypes.xml` entries for any new models added, and `types_4kbossk.xml` entries with nominal=0 for new models

#### Current → New child entries per model:
Replace each single child with all color variants. Example for Audi:
```xml
// BEFORE: 1 fixed color
<child lootmax="0" lootmin="0" max="1" min="1" type="Audi_RS6_ABT_Black" />

// AFTER: all 9 colors, CE picks 1 randomly
<child lootmax="0" lootmin="0" max="1" min="1" type="Audi_RS6_ABT_Black" />
<child lootmax="0" lootmin="0" max="1" min="1" type="Audi_RS6_ABT_White" />
<child lootmax="0" lootmin="0" max="1" min="1" type="Audi_RS6_ABT_Red" />
<child lootmax="0" lootmin="0" max="1" min="1" type="Audi_RS6_ABT_Blue" />
<child lootmax="0" lootmin="0" max="1" min="1" type="Audi_RS6_ABT_Yellow" />
<child lootmax="0" lootmin="0" max="1" min="1" type="Audi_RS6_ABT_Green" />
<child lootmax="0" lootmin="0" max="1" min="1" type="Audi_RS6_ABT_Pink" />
<child lootmax="0" lootmin="0" max="1" min="1" type="Audi_RS6_ABT_Teal" />
<child lootmax="0" lootmin="0" max="1" min="1" type="Audi_RS6_ABT_MidNightBlue" />
```
Do the same for all 20+ models.

#### All available color variants by model:
- **Audi_RS6_ABT** (9): Black, White, Red, Blue, Yellow, Green, Pink, Teal, MidNightBlue
- **BMW_M3** (7): White, Black, MidNightBlue, Green, Orange, Red, Blue
- **Chevrolet_NapalmNova** (14): Black, White, Red, Blue, Pink, Green, Yellow, Orange, Purple, CamoGreen, CamoRed, NightFrost, Reptile, Skull, Topo
- **Dodge_ChallengerSRT** (7): Black, Blue, DarkBlue, Green, Orange, Red, White
- **dodgechargerhellcat** (7): black, blue, green, red, pink, lightblue, white
- **DodgeRam_BOSS** (22): Black, Blue, BlueTiger, CamoGreen, ChinaLake, Cyan, DUDB, Green, Mocha, ModernWoodland, Orange, Pink, Red, RedTiger, Rust, Skulls, TopoGraphic, TopoTrip, VectorTrails, White, Woodland_Blue, Woodland_Nightfrost, Woodland_Sand, camogreen
- **Ford_Bronco** (5): Black, Blue, Hunter, Rust, White
- **Ford_Mustang_Shelby_GT500** (8): Black, Blue, DarkBlue, Green, Orange, Pink, Red, White
- **Ford_Raptor_MonsterTruck** (7): Black (base), Baja, Halo, Raptor, Roush, USA, WideOpen
- **Civic** (7): White, Red, Blue, Black, Yellow, Green, RedCustom
- **Jeep_GladiatorF9_BOSS** (25): Black, Brown, Cyan, Green, Orange, Pink, Red, White, DarkRed, camo_abominable, camo_black, camo_blue, camo_blue2, camo_bluetiger, camo_dragonmoss, camo_flecktarn, camo_green, camo_hunter, camo_nightseas, camo_orange, camo_pinkpython, camo_red, camo_redtiger, camo_skulls, camo_topotrip, camo_vanished
- **Kamaz_TyphoonK** (8): Black, CamoGreen, CamoGreen2, CamoGreen3, Green, RedCamo, Rusty, White
- **MotorHome** (4): Apo, GreenCamo, Rust, White
- **Nissan_Skyline** (6): Black, Blue, Orange, Pink, Red, White
- **NissanGTRNismo** (4): Black, DarkPurple, Red, White
- **Porsche_911RWB** (7): Black, Blue, Green, Orange, Pink, Red, White
- **Supra** (8): Black, Blue, FastF, Green, Lighting, Red, Sponge, White
- **Tahoe** (1+): Brown (Lifted_Tahoe variant exists)
- **ToyotaRunner** (14): Black, blue, pink, red, white, yellow, camoblack, camoblue, camogreen, camopink, camopurple, camored, camowhite

#### New models to add (not currently spawning):
- **NissanGTR** (6 colors): Blue, Green, Orange, Pink, Red, White
- **NissanGTRCustom** (4 colors): Black, Blood, LB, Pink
- **GMC_BOSS** (check for colors)
- **Mitsubishi_LancerEvolutionIX_BOSS** (check for colors)
- **DodgeRam_3500** (check for colors — currently spawning Grey but may have more)

#### When implementing:
- Update `nominal` in events.xml to match new total model count (20 + new models)
- Update `max` in events.xml to keep more positions than vehicles for randomness
- Add types.xml entries (nominal=0) for any new models
- Add cfgspawnabletypes.xml entries for any new models (matching wheel types)
- **Requires server restart** (events.xml is loaded at startup)

### SitRest: Add SurvivorAnims "SitNew" emote support
- **Issue:** Sit crossed (vanilla SitA) freezes hunger/thirst, but "sit straight" does not
- **Root cause:** The "sit straight" emote comes from `@SurvivorAnims` mod, not vanilla SitB. It uses emote ID `5501` (`EMOTE_SASitNew`) instead of vanilla `EmoteConstants.ID_EMOTE_SITB` (15)
- **Found in:** `@SurvivorAnims/addons/SurvivorAnims.pbo` → `Scripts/3_Game/constants.c` (defines `EMOTE_SASitNew = 5501`)
- **Fix ready:** `mod_src/SitRest/Scripts/4_World/SitRest.c` already updated to check `id == 5501`
- **To deploy:** Stop server, rebuild PBO, restart:
  ```bash
  cd mod_src/SitRest
  pbo -b -H "prefix=SitRest" ../../@SitRest/addons/SitRest.pbo config.cpp Scripts/4_World/SitRest.c
  ```

### Trader: Add modded vehicle & plane parts for sale — DONE
### Trader: Add "Building Kits" category to Misc Trader — DONE
### Trader: Add "Horse Tack" category to Misc Trader — DONE
### Trader: Add "Dog Gear" category to Misc Trader — DONE
### Stable_dayz_kit and M79 nominal=0 — DONE

---

## Pending (no server stop needed)

### Roaming Trader: Still showing "Nails" as currency instead of Rubles
- **Issue:** The MWGSM Roaming Trader UI shows "Nails" as currency despite server config being set to MoneyRuble. This has persisted through 3 fix attempts because we were only editing the server-side config.
- **Root cause:** Client/server config split. The mod loads its config independently on both sides using `$profile:` which resolves to different directories:
  - **Server:** `$profile:` = `config/` (from `-profiles=config` in start_server.bat) → reads `config/MWGSM_RoamingTrader/MWGSM_RoamingTraderConfig.json` → has `MoneyRuble1` (correct)
  - **Client:** `$profile:` = `C:\Users\Harrison\Documents\DayZ\` → file doesn't exist there → mod constructor calls `SetDefaults()` which hardcodes `CurrencyItem = "Nail"`
- **Found in:** `@MWGSM_Trader/addons/MWGSM_RoamingTrader.pbo` → `scripts/3_Game/MWGSM_RoamingTrader/MWGSM_Config.c` line 155: `CurrencyItem = "Nail"` (hardcoded default)
- **Why previous fixes didn't work:** We kept editing the server config at `config/MWGSM_RoamingTrader/MWGSM_RoamingTraderConfig.json`, but the client never reads that file. The actual transactions use rubles server-side (that part works), but the client UI displays "Nails" because it uses its own local defaulted config.
- **Additional bug:** `AlternateCurrencies` is empty `[]` in the config, meaning only `MoneyRuble1` (1-ruble notes) are accepted as payment. Higher denominations (5, 10, 25, 50, 100) are ignored even though their exchange rates are defined.

#### Fixes needed (3 things):

**1. Copy config to client profile directory:**
```bash
mkdir -p "C:/Users/Harrison/Documents/DayZ/MWGSM_RoamingTrader/"
cp config/MWGSM_RoamingTrader/MWGSM_RoamingTraderConfig.json "C:/Users/Harrison/Documents/DayZ/MWGSM_RoamingTrader/"
```

**2. Add higher ruble denominations to AlternateCurrencies in the config:**
Change in BOTH server and client copies:
```json
"AlternateCurrencies": ["MoneyRuble5", "MoneyRuble10", "MoneyRuble25", "MoneyRuble50", "MoneyRuble100"],
```

**3. Automate the client sync in `launch_dayz.bat`:**
Add a step to copy the config to the client profile dir on every launch, so future edits propagate automatically.

---

## Completed

### Dogs & Horses not spawning — FIXED (multi-stage)
- **Root causes:** Missing `cfgeventspawns.xml` entries for both AnimalWildDog and AnimalWildHorse, plus broken event children config (min=1 on 35 breeds = impossible constraint)
- **Ambient approach failed:** Dog territory zones have `dmax=0` (designed for Herd only), so Ambient territory produced zero spawns
- **Final fix:** Reverted to Herd type with `DZWolfGroupBeh` (matching mod author's reference exactly), max=5, nominal=35, children min=0/max=1. DayzDog.log confirmed dogs were working with this structure on Feb 23-24.
- **Horses:** Set children min=1 to guarantee non-empty herds, nominal=35 for ~7 herds across map
- **Bears:** Enabled with nominal=4

### Trader additions — ALL DONE
- Vehicle wheels: 19 4KBOSSK + 10 LM Planes wheels added to Vehicle Parts
- Building Kits: stable, dog sheds, raft/buggy, vanilla kits added to Misc Trader
- Horse Tack: saddle, bridle, horse bags added to Misc Trader
- Dog Gear: 7 collars, gas mask, 6 vests added to Misc Trader
- M79 + 40mm ammo added to Weapon Trader Grenades category

### CE overtime fixes — DONE
- `Stable_dayz_kit` nominal=0 in types_dayzhorse.xml
- `M79` nominal=0 in types.xml

