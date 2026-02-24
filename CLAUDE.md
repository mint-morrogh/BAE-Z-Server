# DayZ Server Modding Reference

## Project: Blood & Barter

- 2-player co-op LAN server (Chernarus)
- Git repo: https://github.com/mint-morrogh/DayZServer
- PBOs built with `dayz-dev-tools` (pip install dayz-dev-tools)
- Mod source in `mod_src/`, packed PBOs in `@ModName/addons/`

---

## Creating a Server-Side Mod

Server-side mods load via `-serverMod=@ModName` (clients don't need them).
Client+server mods use `-mod=@ModName` (both sides need them).

### Required File Structure

```
@ModName/
  mod.cpp              # Mod registration (loaded by engine)
  meta.cpp             # Mod metadata
  addons/
    ModName.pbo        # Packed binary object (the actual mod)

mod_src/
  ModName/
    config.cpp         # Text config (packed directly into PBO — NOT rapified)
    Scripts/
      4_World/         # Script phase folder
        MyScript.c     # EnforceScript source
```

### mod.cpp — Keep It Plain

```cpp
class ModName
{
};
```

**DO NOT** use `class ModName: ModInfo` — `ModInfo` is not available for
server-side mods and causes "undefined base class ModInfo" error.

### meta.cpp

```cpp
class ModName
{
    name = "ModName";
    author = "Author Name";
    version = "1.0";
};
```

### config.cpp (Packed as text into PBO)

```cpp
class CfgPatches
{
    class ModName
    {
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] = {"DZ_Data"};
    };
};
class CfgMods
{
    class ModName
    {
        dir = "ModName";
        picture = "";
        action = "";
        hideName = 1;
        hidePicture = 1;
        name = "ModName";
        credits = "";
        author = "";
        version = "1.0";
        extra = 0;
        type = "mod";
        dependencies[] = {"World"};
        class defs
        {
            class worldScriptModule
            {
                value = "";
                files[] = {"ModName/Scripts/4_World"};
            };
        };
    };
};
```

### Prefix

The PBO prefix (e.g., `ModName`) is passed via `-H "prefix=ModName"` when
packing with dayz-dev-tools. It must match the `dir` value in CfgMods and
the script path references in config.cpp.

---

## PBO Format (Critical Quirks)

PBOs are the container format for DayZ mod files. The order of sections is exact
and the engine is unforgiving about it.

### Correct byte layout:

```
1. Product Entry (21 bytes)
   - filename: \0  (1 byte, empty string)
   - packing:  0x56657273  ("Vers" magic, little-endian)
   - original_size: 0
   - reserved: 0
   - timestamp: 0
   - data_size: 0

2. Properties (immediately after product entry!)
   - key\0value\0 pairs (e.g., "prefix\0ModName\0")
   - terminated by empty key: \0

3. File Entry Headers (one per file)
   - filename\0  (null-terminated, backslashes for paths)
   - packing: 0 (uncompressed)
   - original_size: <uint32>
   - reserved: 0
   - timestamp: 0
   - data_size: <uint32>  (same as original_size for uncompressed)

4. Terminator Entry (21 bytes)
   - filename: \0
   - packing: 0
   - original_size: 0
   - reserved: 0
   - timestamp: 0
   - data_size: 0

5. File Data
   - Concatenated file contents in same order as headers
```

### SHA-1 Checksum (required):
After the file data, every valid PBO ends with:
- `\0` (1 byte null separator)
- SHA-1 hash (20 bytes) of everything before it (sections 1-5)

dayz-dev-tools handles this automatically.

### Common PBO mistakes:

- **Missing SHA-1 checksum**: PBOs must end with `\0` + 20-byte SHA-1 hash.
  All proper tools (dayz-dev-tools, AddonBuilder, armake2) include this.
- **Using a broken rapifier**: Our custom `rapify.py` produces bad config.bin
  that hangs the engine. Always pack text `config.cpp` instead.
- **Properties in the wrong place**: Properties MUST be between the product
  entry and the file entries.
- All uint32 fields are little-endian.
- Filenames use backslashes (`Scripts\4_World\File.c`).

---

## Config Format

The DayZ engine can read BOTH text `config.cpp` and binary `config.bin`.
It checks the first bytes — if it sees `\0raP` magic, it treats as binary;
otherwise it parses as text.

**We pack text config.cpp directly into PBOs.** Do NOT use `rapify.py` —
it produces broken binary configs that hang the engine. If binary config.bin
is ever needed, use the official `CfgConvert.exe` from DayZ Tools on Steam:
`CfgConvert.exe -bin -dst config.bin config.cpp`

**If both config.bin and config.cpp exist in a PBO, the .bin is IGNORED.**

---

## Build Commands

```bash
# Pack PBO using dayz-dev-tools (pip install dayz-dev-tools)
# -b = no-convert (pack text config.cpp as-is, skip rapification)
cd mod_src/ModName
pbo -b -H "prefix=ModName" @ModName/addons/ModName.pbo config.cpp Scripts/4_World/MyScript.c
```

**DO NOT use rapify.py** — our custom rapifier produces broken config.bin
that hangs the DayZ engine. Pack text `config.cpp` directly instead; the
engine reads it natively. Use `dayz-dev-tools` (installed via pip) for
PBO packing — it handles checksums and format correctly.

---

## EnforceScript Modding Notes

### Script phases (folder names matter):
- `1_Core` — engine core
- `2_GameLib` — game library
- `3_Game` — game logic
- `4_World` — world entities (zombies, players, items)
- `5_Mission` — mission logic

### `modded class` keyword:
Use `modded class ClassName` to extend/override existing classes. This chains
with other mods (e.g., PvZmoD also modding ZombieBase). Always call `super`
on overridden methods.

**CRITICAL: One `modded class` per class per `-serverMod` list.** If two
separate server-side mods (loaded via `-serverMod=`) both contain
`modded class ZombieBase`, the script compiler deadlocks and the server
hangs silently during startup (no errors, no RPT output). The same class
CAN be modded by a `-mod=` (client+server) mod AND a `-serverMod=` mod
without issue — the conflict is only between two `-serverMod=` entries.

**Workaround:** Merge all `modded class ZombieBase` logic into a single
server-side mod. Different classes (e.g., `ZombieBase` in one mod and
`FireplaceBase` in another) work fine as separate server mods.

### Server-only code guard:
```cpp
if (!GetGame().IsServer())
    return;
```

### Deferred deletion pattern:
Don't delete entities directly in `EEInit()` — use CallLater:
```cpp
GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DeferredDespawn, 150, false);
```
The 150ms delay avoids issues during entity initialization.

### Useful APIs:
- `GetGame().GetWorld().GetDate(year, month, day, hour, minute)` — in-game time
- `GetGame().ObjectDelete(obj)` — remove entity from world
- `Math.RandomFloat01()` — random 0.0–1.0
- `GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(func, ms, repeat)`

---

## Central Economy (CE) Key Files

All under `mpmissions/dayzOffline.chernarusplus/db/`:

- **types.xml** — Item spawn rules. `nominal`/`min` = count on map. `quantmin`/`quantmax` = fill percentage (0–100) for magazines, ammo, containers.
- **events.xml** — Zombie/animal spawn events. `nominal`/`min` = active count. Events use `Infected*` prefix (not `Zombie*`). No day/night variants exist.
- **Custom type files** (e.g., `custom/types_4kbossk.xml`) — loaded via `cfgeconomycore.xml`.

### CE behavior:
- Items/zombies spawn independently at random valid locations — no per-building linking.
- CE tries to maintain `nominal` counts. If a mod despawns zombies, CE respawns more, creating equilibrium at a reduced effective count.
- `quantmin`/`quantmax` are percentages: a 30-round mag with quantmin=40 spawns with ~12 rounds minimum.

---

## PvZmoD CustomisableZombies

Config files in `config/PvZmoD_CustomisableZombies_Profile/`:

- **Globals.xml** — Server-wide zombie ratios (speed, health, strength, vision, size).
  - `Zombies_Size_Activated Day="0" Night="0"` disables random giant zombies.
- **Characteristics.xml** — Per-zombie-type overrides (~2500 lines).
  - Vision `Night="1.0"` = vanilla detection range.
- PvZmoD reads actual in-game time, so Sleep Till Morning (clock advance) updates zombie behavior immediately.

---

## Server Config Notes

- `start.bat` line 19: `-mod=` for client+server mods, `-serverMod=` for server-only mods.
- `serverDZ.cfg` — main server configuration.
- `verifySignatures = 0` — disabled for LAN; custom server mods are unsigned.
  Set to 2 only if all PBOs have matching .bisign/.bikey files.
- Storage wipe: delete contents of `storage_0/` and `storage_1/` for fresh characters/world.
- Server auto-restarts every 4 hours (timeout 14390 in start.bat).
- `config/DataCache/` — compiled script cache. Delete `cache.ch` and
  `cache_lock` after crashes to force clean recompilation.

---

## PBO Build Tools

**Primary: dayz-dev-tools** (Python, `pip install dayz-dev-tools`).
This is our standard PBO packer — handles checksums, format, and packing.

Alternatives (if ever needed):
- **AddonBuilder** (DayZ Tools on Steam): Official GUI tool with signing.
- **CfgConvert** (DayZ Tools): Official rapifier for config.cpp → config.bin.
- **armake2** (Rust, `cargo install armake2`): Open-source packer + signer.
- **pboProject** (Mikero): Professional-grade, handles binarization and signing.

**DO NOT use `mod_src/rapify.py` or `mod_src/pack_pbo.py`** — rapify.py
produces broken config.bin that silently hangs the DayZ engine. These files
are kept for reference only.
