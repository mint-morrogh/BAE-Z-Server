# BAE-Z - TODO

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



### ~~Vehicle Damage: Tune SFE No Vehicle Damage speed threshold~~ — DONE
- SFE: No Vehicle Damage installed and working. Minor collisions no longer ruin parts.

### ~~SitRest: "Sit B" (straight) emote not freezing hunger/thirst~~ — FIXED
- SitB confirmed working. SitRest expanded to cover all rest emotes: SitA, SitB, Campfire Sit, Lying Down, SurvivorAnims SitNew.

### ~~Trader: Split Vehicle Parts into multiple tabs~~ — DONE
- Split single 1300-line Vehicle Parts tab into 6 categories: Engine & Fluids, Vanilla Vehicle Parts, Modded Wheels, 4KBOSSK Parts (Audi-Dodge), 4KBOSSK Parts (Ford-Kamaz), 4KBOSSK Parts (Mitsubishi-Toyota).

---

## Pending (no server stop needed)

---

## Completed

### Enable Inventory In Vehicle + Sit Emotes — DONE
- Custom client+server mod (`-mod=`) replaces removed Workshop mod (3594596641).
- Unlocks inventory in vehicles with dog-menu guard (skips re-lock if scripted menu is open).
- Also overrides `CanManipulateInventory()` to allow inventory during sit emotes (SitA, SitB, SurvivorAnims SitNew).
- Added to `-mod=` in `start_server.bat`, `CUSTOM_MODS` + sync in `launch_dayz.bat`.

### Dogs & Horses not spawning — FIXED
- `AnimalMaxCount` set to 1200 (was 200). Vanilla herds consumed all entity slots. Dogs, horses, wolves, sheep now all spawning. Confirmed in-game Mar 2.

### Roaming Trader: "Nails" currency + missing denominations — FIXED
- Client reads config from `Documents\DayZ\` not server `config/`. Copied config to client profile dir, added ruble denominations, automated sync in `launch_dayz.bat`.

### Trader additions — ALL DONE
- Vehicle wheels: 19 4KBOSSK + 10 LM Planes wheels added to Vehicle Parts
- Building Kits: stable, dog sheds, raft/buggy, vanilla kits added to Misc Trader
- Horse Tack: saddle, bridle, horse bags added to Misc Trader
- Dog Gear: 7 collars, gas mask, 6 vests added to Misc Trader
- M79 + 40mm ammo added to Weapon Trader Grenades category

### Zombie health nerf (PvZmoD) — DONE
- 10 bullet-sponge zombie types reduced to 180 HP / 0.4 headshot resist, matching military zombies. Jacket, Skirt, Priest, Mummy, Santa, Patrol, DoorHouse masters, NightWalker all nerfed. Vehicle/explosion immunity removed from boss types.

### Dog health & damage buff — DONE
- HealthBoost config.cpp: dog HP 4x (800→3200), blood 4x (5000→20000), shock doubled (200→400). Bite damage doubled (80→160 Health, 200→400 Blood, 22→44 Shock). Dog regen 10% every 15s (~2.5 min full heal), horses stay at 5% every 30s.

### Zombie daytime spawn rate increase — DONE
- DayZombieManager `DAY_DESPAWN_CHANCE` reduced from 0.85 to 0.65 (35% survive during day, up from 15%). Compensates for zombies being less tanky after PvZmoD nerf.

### Vanilla animal nominals reduced — DONE
- Reduced vanilla herd nominals in events.xml to leave more room under AnimalMaxCount=1200 cap. Dog nominal reduced from 35→8 (~100 entities). Vanilla animals rebalanced to lower counts.

### Fires burn 3x longer — DONE
- `GetFuelBurnRateMP()` override in CampfireRegen returns `super * 0.333`, reducing fuel consumption to 1/3 speed.

### Crops last 3x longer before spoiling — DONE
- `modded class PlantBase` in HealthBoost sets `m_SpoilAfterFullMaturityTime = 43200` (12 hours, up from default 4 hours).

### CE overtime fixes — DONE
- `Stable_dayz_kit` nominal=0 in types_dayzhorse.xml
- `M79` nominal=0 in types.xml

