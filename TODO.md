# BAE-Z - TODO

## Needs in-game test (installed 2026-09-06, after full wipe)

### Trader grenades/explosives unsellable + search OOM - FIXED (2026-09-07)
- **Grenades could never be sold.** Every throwable in the Grenades category had `1` in the quantity column. Trader's `DoItemSellChecks` tests `getItemAmount(item) >= amount`, and a grenade/mine is not a Magazine and has no `varQuantityMax`, so `getItemAmount` returns 0 and `0 >= 1` always failed - listed and buyable, but the sell button never found the item. Every other quantity-less item in the file (tools, clothing, optics) uses `*`, which the parser resolves to 0. The 40mm rounds sold fine because they are magazines with an ammo count (confirmed in `TM_TradesLog`). All non-magazine entries switched to `*`.
- **Category moved** from Weapon Trader to **Weapon Supplies** (the ammo trader, where explosives are expected) and renamed `Grenades & Explosives`; the three launchers (M79, ExpansionRPG7, ExpansionLAW) stay on the Weapon Trader as `Launchers`.
- **Nine items added** that spawn as loot but were never sellable: `ClaymoreMine`, `ImprovisedExplosive`, `LandMineTrap`, `RemoteDetonator(+Receiver/Trigger)`, `Grenade_ChemGas`, `nm_Improgren`, `nm_Shrapnelgren`, `nm_IncendiaryGren`. Safe in the safezone: Trader's own `Grenade_Base.InitiateExplosion` / `Grenade_ChemGas.OnExplode` overrides delete them instead of detonating.
- **Search OOM fixed properly**, so the 8 smoke variants removed on 2026-09-05 are back (M18 Yellow/Purple, RDG2 Black, 40mm Smoke Green/Black, M203 Smoke Green/Yellow/Purple). New `@TraderSearchFix` (client+server `-mod=`, `mod_src/TraderSearchFix/`) debounces `TraderMenu.updateItemPreview()` by 300 ms via `CALL_CATEGORY_GUI`, so a burst of keystrokes creates one 3D preview entity instead of one per character. The old workaround only reduced how many items matched; the leak was the per-keystroke `CreateObject()` pile-up against deferred `ObjectDelete()`.
- **Test:** Weapon Supplies > Grenades & Explosives: sell a found grenade and a smoke, buy an M67 and a Claymore. Then type "smoke" one character at a time in the search box at BOTH traders - no crash, and the preview should settle on the selected item.

### Inventory Move Sounds on 1.29 - PATCHED (2026-09-07)
- 1.29 removed vanilla `MagRifle_empty_in_SoundSet`; IMS plays it on every magazine/ammo/grenade move -> `EffectSound ... Invalid sound set` on the client (4x right before the Sep 7 heap-corruption crash). New `@InventoryMoveSoundsFix` (3_Game script, client+server, in both bat files) overrides `EffectSound.SetSoundSet()` and swaps the name for `MagRifle_fill_out_SoundSet`. A config override was tried and rejected: both IMS PBOs share the CfgPatches name `IMS_Sounds`, so it cannot be ordered after the movesounds config (server RPT showed our config processed first).
- **Test:** move a magazine and a grenade in inventory; `%LOCALAPPDATA%\DayZ\script_*.log` must show no `MagRifle_empty_in_SoundSet` error, and a mag handling sound should play.

### Trader dead entries - FIXED (2026-09-07)
- Class check of every Weapon Trader Rifles..Optics entry against all vanilla + mod configs found 24 names that exist nowhere (listed but never buy/sell). Renamed 16 (`AJW__AWM_*` double underscore -> `AJW_AWM_*`, `Expansion_M203_*`/`Expansion_338`/`Expansion_46x30` -> `Ammo_Expansion_*`/`AmmoBox_Expansion_*`, `AJW__Ammo_338`, `AJW__AmmoBox_338mm_10Rnd`, `AJW_Mag_CM_TaranTactical_9Rnd` -> `AJW_Mag_TaranTactical_9Rnd`) and deleted 8 with no real class (`AJW_Optic_LRHS`, `AJW_Optic_LRHS_FDE`, `MassAR15`, `MassPpskn`, `Expansion_W1873`, `Expansion_8mm`, `BAW_Mag_Uzi_35Rnd`, `BAW_Mag_DMR_20rnd`).
- **Test:** Weapon Trader > Sniper/Ammo/Optics: buy an `AJW_AWM_FDE`, an M203 HE round and a .338 box; they should spawn.
- GPS was never in the trader: added `GPSReceiver, *, 80, 40` to Misc Trader > Electronics (between Rangefinder and NVGoggles). **Test:** buy/sell a GPS at Misc Trader.

### KM_BetterFishing - INSTALLED
- Workshop 3744242546 -> `@KM_BetterFishing` (client+server, end of `-mod=` list). Only depends on vanilla DZ_ addons.
- Mods `ActionFishingNew`, `FishingRod`, `CatchingContextFishingRodAction`, vanilla fish classes, knives (`SetActions`), `PlayerBase.EEKilled`, `MissionServer.OnInit` - all call `super`. ZenSkills also mods `CatchingContextFishingRodAction` but different methods (bite chance vs bait consumption); Nemsis also mods the knives via `SetActions` - both chain.
- Loot: `custom/types_kmbetterfishing.xml` (Coast + Village usage, mod ships everything at nominal 0), spawnabletypes block appended to `cfgspawnabletypes.xml`. Trader: Misc **Fishing Supplies**, Consume **Meat** buys the fish/fillets.
- Config generated on first boot at `config/KM_BetterFishing/KM_BetterFishing.json` (tracked); `KM_BetterFishing_PlayerData.json` is XP (ignored, wiped).
- **Test:** buy `KM_FishingRod_Pro` + a rubber worm at Misc Trader, fish at the coast; the worm should survive a catch; new fish should appear; check the fishing XP message. Find a tackle box on the coast. Sell a fish at Consume Trader.

### Food and Drinks - INSTALLED
- Workshop 3570578065 -> `@FoodAndDrinks` (client+server, after `@KM_BetterFishing` in `-mod=`). Single `HM_Food.pbo`, only mods `ModItemRegisterCallbacks`; 33 `Edible_Base` items, no config folder.
- Loot: `custom/types_foodanddrinks.xml` (drinks 12, chips/bars/snacks 8, JD 4, MRE 4 military/hunting); vanilla sodas 56 -> 36 and Crackers/Zagorky/Marmalade/PowderedMilk/Pate/Pajka/Brisket trimmed ~30% in `db/types.xml` so total food stays flat. Trader: Consume **Snacks & Bars** + **Energy Drinks & Booze**.
- **Test:** loot a few houses/supermarkets on the coast - branded cans/chips should show up alongside vanilla ones; eat/drink one (energy drink = 50 energy / 150 water); buy a Monster + Pringles at Consume Trader.

### Batch files must be CRLF - FIXED (2026-09-06)
- `INSTALL_MODS.bat` had LF line endings in the working copy; cmd's `call :label` mis-resumes in LF files, so every run re-executed the mod list 18 times (summaries went 17/17, 35/35, 50/50 ...). Harmless (xcopy /D) but slow. Converted `INSTALL_MODS.bat`, `START_SERVER.bat`, `WIPE_SERVER.bat` to CRLF; git stores LF and `core.autocrlf=true` checks out CRLF, so keep it that way. If a bat starts looping again, check `git ls-files --eol *.bat`.

## Needs in-game test (installed 2026-09-05, after full wipe)

### TP Apoc vehicles (M1025 / Pickup / SUV) - INSTALLED
- Workshop 3737385977 / 3738834788 / 3738839427 -> `@TP_Apoc_M1025`, `@TP_Apoc_Pickup`, `@TP_Apoc_SUV` (client+server, end of `-mod=` list).
- One of each spawns via `VehicleTPApoc` (events.xml) at 8 positions (cfgeventspawns.xml). Spawnable attachments + `custom/types_tpapoc.xml` added. Trader: **TP Apoc Vehicles** / **TP Apoc Parts**.
- **Test:** drive to a spawn position (NWAF south hangars, Balota hangars, Pavlovo, Myshkino, Green Mountain, Novodmitrovsk, Stary tents, Solnichniy factory) and confirm the vehicle is not clipping into a building. Adjust `<pos>` if so.
- **Test:** Humvee turret - get in the gunner seat, load `FYG_762_Ammo`, fire. BDE turret scripts spam `[Camera_Logic] Check N` prints in the client log; harmless.

### Immersive Placing Update - INSTALLED
- Workshop 3753472356 -> `@ImmersivePlacing` (client+server). Mods `PlayerBase`, `ItemBase`, `ActionDeployObject`, `ActionDigGardenPlot`, `Hologram` - all call `super`, chains fine with Expansion.
- **Test:** place an item, rotate/raise it; quick-drop; dig a garden plot; Expansion base building still works.

### Zens Core Mod - NEW DEPENDENCY (2026-09-05)
- Zens Skill Perk Tree and Zens Treasure updated and now require `ZenModCore` (Workshop 3702420204 -> `@ZenModCore`, loaded before `@ZenSkills`). Without it the client fails with "Can't compile Game script module ... Unknown type ZenConfigRegister" and the server aborts at startup.
- Added to INSTALL_MODS.bat / START_SERVER.bat / README. **Everyone must subscribe to it on the Workshop.**

### Stale Workshop PBOs on the server - FIXED (2026-09-05)
- `INSTALL_MODS.bat` only ever added/updated files, so PBOs that a Workshop update renamed or removed stayed behind in the server `@Mod\addons` folder. After the long break this left duplicates (old `Dodge_ChallengerSRTSuperStock_BOSS*.pbo` + new `DodgeChallengerDemon_BOSS*.pbo`, old `Porsche_911RWB_1972_BOSS.pbo`, `cz_optics.pbo`, `0_ZenSkills_Preload.pbo`) -> "Multiple declaration of class" on the client and an ACCESS_VIOLATION crash on the server during startup.
- Stale files deleted; `:install_mod` now removes any `.pbo`/`.bisign` in the server addons folder that the Workshop copy no longer ships (`[STALE]` lines in its output).

### CZ Optics Workshop mod changed contents - VERIFIED OK (2026-09-05)
- Workshop 3571068454 now only ships `BC_ServerPack.pbo` (text config.cpp); the stale `cz_optics.pbo` was an old leftover. All 8 `BC_*_Optic` classes the Weapon Supplies trader sells are defined in the new PBO; nothing references the old file.

### 4KBOSSK update renamed models - HANDLED
- Porsche 911 RWB replaced by Porsche 911 GT3 (`GT3_*`), Challenger moved into `DodgeChallengerDemon_BOSS` (class names unchanged), new models Audi R8, BMW M5 G90, Ford Crown Victoria, Ford Mustang SVT, Shelby Super Snake, Skyline rust variants, lifted Bronco variants. Event/spawnabletypes/types regenerated (29 models, 291 colour variants); trader "Porsche 911 RWB Parts" -> "Porsche 911 GT3 Parts".

### Custom types files were in the wrong folder - FIXED (2026-09-05)
- `cfgeconomycore.xml` loads `<ce folder="custom">` relative to the mission root, but `types_buggy.xml`, `types_harley.xml`, `types_expansion_vehicles.xml`, `types_expansion_weapons.xml` and `types_lmhelicopters.xml` had been committed to `db/custom/` and were never loaded (RPT: `Failed to read types file`). Moved to `mpmissions/dayzOffline.chernarusplus/custom/` together with the new `types_tpapoc.xml`.
- `ExpansionOldBike` no longer exists in DayZ-Expansion-Vehicles; removed from `VehicleExpansionGround`, the types file and the trader.

### Steam Workshop collection
- Add the five new mods to the BAE-Z collection (id 3679580119) so new players get them from the one-click subscribe link.

---

## Pending (server must be stopped)

*(nothing outstanding)*

## Pending (no server stop needed)

### Freeze Mods: Lock all mod versions to prevent Workshop updates from breaking the server
- **Goal:** Snapshot all current working mod versions so Steam Workshop updates can't break anything.
- **How it works:**
  1. Create `FREEZE_MODS.bat` that copies ALL Workshop mod folders (`steamapps/workshop/content/221100/<id>/`) to a local `@Frozen/<ModName>/` directory
  2. Update `LAUNCH_DAYZ.bat` to load client mods from `@Frozen/` paths instead of `!Workshop` symlinks
  3. Update `START_SERVER.bat` `-mod=` and `-serverMod=` to also point at `@Frozen/` copies
  4. Stop running `INSTALL_MODS.bat` after freezing (it would overwrite frozen copies with new Workshop versions)
- **To update a specific mod later:** Re-run `INSTALL_MODS.bat` for just that mod, test it, then copy the updated version into `@Frozen/`
- **To update everything:** Re-run `INSTALL_MODS.bat` for all mods, test, then re-run `FREEZE_MODS.bat` to snapshot the new working set
- **Rollback:** Keep a `@Frozen_backup/` copy before updating so you can restore if something breaks
- **Note:** Our custom patches (SurvivorAnims, etc.) must be re-applied after any mod update — `INSTALL_MODS.bat` already handles this
- **When:** After all current fixes are stable and tested

---








## Completed

### Git tracking audit: settings in, save data out - DONE (2026-09-06)
- Found two config folders tracked under the WRONG name so the mods never read them: Saga Shake Tree reads `config/SagaTreeShake/` (we had `SagaShakeTree/`), Care Packages reads `config/CarePackageV2/` (we had `CarePackagesV2/`, and the real folder was gitignored). Both mods were running on stock defaults. Customised files moved into the real folders, wrong folders removed.
- Now also tracked: `config/ImmersivePlacing/`, `config/LM/` (planes + helis), `config/SFE_TransportDamageControl/`, `config/SobrMods_Signal_Overnight_Stay/`. Runtime data (storage, PlayerDB, logs, Expansion Groups/PlayerData, Saga cooldowns) stays ignored.

### Trader: bought items never arrive on DayZ 1.29 - DONE (2026-09-06)
- Non-stackables (cans, jackets, holsters, belts) were charged but never spawned. Cause: Trader's `TR_Helper.GetItemMaxQuantity()` is `bool` but returns `-1` for classes without `count`/`varQuantityMax`; 1.29 reads that as `true`, so the item was treated as an amount-0 stack and the spawn skipped.
- Fixed by custom `@TraderBuyFix` (`-serverMod`, source `mod_src/TraderBuyFix/`): int max-quantity + vanilla `LocationCreateEntity` spawn, hands/ground fallback, `[TraderBuyFix]` log lines in `config/script_*.log`. Verified in-game: bacon, holster, belt, jacket all arrived.

### 4KBOSSK: random colours + all models - DONE (2026-09-05)
- `Vehicle4KBOSSK` now lists every colour variant of every model (221 children, `max=1 min=1`) so CE spawns one of each model in a random colour. Added NissanGTR, NissanGTRCustom, GMC_BOSS and Mitsubishi Lancer Evo IX (24 models + 3 buggies = nominal 27, 37 positions).
- Fixed pre-existing bugs found while doing it: `Ford_Raptor_MonsterTruck` base class is not spawnable (scope 0) - replaced with its 6 real variants; Civic spawned with `Wheel_Main` (Jeep wheel) instead of `Honda_Civic_Wheel`; MotorHome referenced non-existent `MotorHome_Wheel` (real class `MotorHomeRV_Wheel`). Wheel counts now match each model's slot count (Kamaz 6, Ram 2021 10, etc.).
- `cfgspawnabletypes.xml` and `custom/types_4kbossk.xml` regenerated from the mod's actual configs.

### Trader: Weapon Trader OOM on "smoke" search - DONE (2026-09-05), SUPERSEDED (2026-09-07)
- First pass only reduced how many items matched the search (8 smoke lines removed). Root cause is fixed for real by `@TraderSearchFix` and all 8 lines are back - see the entry at the top of this file.

### Enable Inventory In Vehicle + Sit Emotes — DONE
- Custom client+server mod (`-mod=`) replaces removed Workshop mod (3594596641).
- Unlocks inventory in vehicles with dog-menu guard (skips re-lock if scripted menu is open).
- Also overrides `CanManipulateInventory()` to allow inventory during sit emotes (SitA, SitB, SurvivorAnims SitNew).
- Added to `-mod=` in `START_SERVER.bat`, `CUSTOM_MODS` + sync in `LAUNCH_DAYZ.bat`.

### Dogs & Horses not spawning — FIXED
- `AnimalMaxCount` set to 1200 (was 200). Vanilla herds consumed all entity slots. Dogs, horses, wolves, sheep now all spawning. Confirmed in-game Mar 2.

### Roaming Trader: "Nails" currency + missing denominations — FIXED
- Client reads config from `Documents\DayZ\` not server `config/`. Copied config to client profile dir, added ruble denominations, automated sync in `LAUNCH_DAYZ.bat`.

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
- DayZombieManager `DAY_DESPAWN_CHANCE` reduced from 0.85 to 0.55 (45% survive during day, up from 15%). Compensates for zombies being less tanky after PvZmoD nerf. (0.85→0.65 initial, then 0.65→0.55 bump on Mar 9)

### Vanilla animal nominals reduced — DONE
- Reduced vanilla herd nominals in events.xml to leave more room under AnimalMaxCount=1200 cap. Dog nominal reduced from 35→8 (~100 entities). Vanilla animals rebalanced to lower counts.

### Fires burn 3x longer — DONE
- `GetFuelBurnRateMP()` override in CampfireRegen returns `super * 0.333`, reducing fuel consumption to 1/3 speed.

### Crops last 3x longer before spoiling — DONE
- `modded class PlantBase` in HealthBoost sets `m_SpoilAfterFullMaturityTime = 43200` (12 hours, up from default 4 hours).

### DayZ-Dog: IntroSceneCharacter startup crash — FIXED
- Patched PBO adds null guards around `CreateCharacterPerson` in `CharacterLoad`. No more `Access violation at 0x8` on client startup. Auto-deployed by `LAUNCH_DAYZ.bat` Fix 6 and `install_mods.bat` post-install step.
- Rebuild if DayZ-Dog updates: extract PBO → replace scripts from `mod_src/DayZDogPatch/scripts/` → rebuild with `pbo -b -H "prefix=Dayz_Dog" -P "**/*"`

### CE overtime fixes — DONE
- `Stable_dayz_kit` nominal=0 in types_dayzhorse.xml
- `M79` nominal=0 in types.xml
