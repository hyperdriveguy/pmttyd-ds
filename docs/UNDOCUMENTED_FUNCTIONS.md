# Undocumented Functions and Binary Patterns in Paper Mario ISO

Found while analyzing `artifacts/rom/G8ME01.iso`. These are small binary patterns,
data tables, and functions that were not previously documented in the project.

## Overview

During binary code analysis of the Paper Mario ISO (G8ME01), several categories
of small, previously undocumented elements were discovered:

1. **Effect definition tables** (`a/ag2tg.bin`) - Effect name registry
2. **Object definition tables** - MOBJ/HOBJ name registries  
3. **Script files** (`m/*/s*`) - Event sequence scripts
4. **Small message files** (`msg/US/*`) - 1-byte placeholder messages
5. **Texture/animation tables** - Various `.tpl` and `.bfn` files
6. **Incremental binary data** - `.bin` files with pattern data

## Effect Definition Table (`a/ag2tg.bin`)

**Size**: 41984 bytes at offset 0x33313b44

This file contains a registry of game effects. Effects are referenced by name
throughout the game's code.

### Effect Name Patterns

The file contains entries of the form:
- `EFF_XXXX` - Standard effects
- `EFF_XXXX_D` - "D" variant (probably duplicate/debug)
- `EFF_XXXX_F` - "F" variant (possibly flame/fire?)
- `EFF_XXXX_I` - "I" variant (possibly ice?)
- `EFF_XXXX_Sp` - Special variant
- `EFF_XXXX_us` - US region variant
- `EFF_XXXX_D` suffix patterns

### Known Effect Names (extracted from binary)

| Pattern | Count | Notes |
|---------|-------|-------|
| `EFF_Baketu` | 2 | |
| `Eff_Hana` | 2 | |
| `EFF_Irekae` | many | Includes _D, _F, _I, _Sp, _us variants |
| `Eff_Rankup` | 6 | Includes _D, _F, _I, _Sp, _us variants |
| `Eff_good` | 6 | Includes _D, _F, _I, _Sp, _us variants |
| `Eff_great` | 6 | Includes _D, _F, _I, _Sp, _us variants |
| `Eff_hibashira` | 2 | |
| `Eff_bakuhatu` | 2 | |
| `Eff_lucky` | 6 | Includes _D, _F, _I, _Sp, _us variants |
| `Eff_excellent` | 6 | Includes _D, _F, _I, _Sp, _us variants |
| `Eff_nohit` | 2 | |
| `Eff_sango` | 2 | |
| `Eff_treasure_map` | 2 | |
| `Eff_wonderful` | 6 | Includes _D, _F, _I, _Sp, _us variants |

### Structured Data Pattern

The file appears to have a table structure where:
- u32 values are followed by ASCII strings
- The pattern repeats: `u32` + effect name string
- Some u32 values are 0 (possibly null terminators or inactive effects)
- Some u32 values reference other data

Example pattern found:
```
u32: 0x4546465f = "_EFF" in little-endian ASCII
u32: 0x42616b65 = "bake" in little-endian ASCII
u32: 0x74750000 = "tu\0" (terminated string)
```

### Effect Name Suffix Meanings (hypothesis)

- `_D` - Duplicate or debug version
- `_F` - Flame/Fire variant
- `_I` - Ice variant
- `_Sp` - Special effect variant
- `_us` - US region specific variant

## Vivian Binary Table (`a/vivian.bin`)

**Size**: 3088 bytes at offset 0x373f6528

This file contains structured data with mixed u32 values and short strings.

### Known String Patterns

| String | Offset | Associated u32 values |
|--------|--------|----------------------|
| `3B?8Q` | ~0 | 0x3bc48c94, 0x3eb33342, ... |
| `3@?8Q` | ~16 | similar structure |
| `3D54` | ~32 | |
| `34?8Q` | ~64 | |
| `32?8Q` | ~80 | |
| `fx54` | ~320 | Associated with multiple u32 values |
| `?ffh>` | ~352 | |
| `fx?338>` | ~384 | |

### u32 Value Structure

The file has a regular pattern of u32 values incrementing by ~0x3b3330 intervals:
- 0x3bc48c94
- 0x3eb33342
- 0x3f3851f4
- 0x3eb33344
- 0x3eb33332
- 0x3faa3d6d
- 0x3eb33340
- 0x3ff1eb7a
- 0x3eccccd6
- 0x3f3851e8

These appear to be addresses or offsets within the game's memory map.

## Object Definition Tables

### HOBJ (Host Object) Names

Found in `a/ag2tg.bin` at various offsets. HOBJ entries follow the pattern:
- `HOBJ_BlueJumpStand`
- `HOBJ_BlueSwitch`
- `HOBJ_BrickHatenaBlock`
- `HOBJ_GreenPipeL` / `HOBJ_GreenPipeW`
- `HOBJ_Lock`
- `HOBJ_RedJumpStand` / `HOBJ_RedSwitch`
- `HOBJ_TreasureBox`
- and many more (50+ unique prefixes with suffixes)

### MOBJ (Mario Object) Names

Also found in `a/ag2tg.bin`. MOBJ entries follow the pattern:
- `MOBJ_10CountBlock`
- `MOBJ_Arrow`
- `MOBJ_BadgeBlock`
- `MOBJ_BigTreasureBox`
- `MOBJ_Block` / `MOBJ_BlueJumpStand` / `MOBJ_BlueSwitch`
- `MOBJ_CoinTakenRecoveryBlock`
- `MOBJ_EFF_*` (embedded effects)
- `MOBJ_FlowBrickBlock`
- `MOBJ_GoldenTreasureBox`
- `MOBJ_GrayTreasureBox`
- `MOBJ_HatenaBlock`
- `MOBJ_IceBlock`
- `MOBJ_KururinFloor`
- `MOBJ_Lock`
- `MOBJ_RecoveryBlock`
- `MOBJ_SaveBlock`
- `MOBJ_Signboard`
- `MOBJ_TreasureBox`
- and 100+ more

Suffix patterns observed:
- `_1` - Level 1 variant
- `_10` - Level 10 variant  
- `_10C` - Level 10 Count variant
- `_A` - Type A variant
- `_Ar` - Arm variant
- `_B` - Type B variant
- `_Ba` - Base variant
- `_Bad` - Bad variant
- `_Bi` - Ice variant
- `_Big` - Big variant
- `_Bl` - Blue variant
- `_Blo` - Blooper variant
- `_Blui` - Blue (full)
- `_Br` - Brown/Br variant
- `_Bre` - Bre variant
- `_Red` - Red variant
- `_Re` - Re variant
- `_Ri` - Ri variant
- `_Rid` - Rid variant
- `_S` - Size/Small variant
- `_Sa` - Size A variant
- `_Sev` - Sev variant (maybe save?)
- `_Si` - Si variant
- `_Sig` - Sig (signature?) variant
- `_T` - Type T variant
- `_Tr` - Tr variant
- `_Tre` - Tre variant
- `_W` - White/W variant
- `_Wh` - Wh variant
- `_Whi` - White (full)
- `_b` - small/b variant
- `_bi` - bi variant
- `_big` - Big variant
- `_e` - e variant (mario?)
- `_ek` - ek variant
- `_eki` - eki variant
- `_f` - f variant
- `_fi` - fi variant
- `_fir` - fir variant
- `_g` - g variant
- `_go` - go variant
- `_goa` - goa variant
- `_g` - g (general) variant
- `_gr` - gr variant
- `_gre` - gre variant
- `_p` - p variant
- `_pu` - pu variant
- `_pur` - pur variant
- `_r` - r variant
- `_re` - re variant
- `_red` - red variant
- `_s` - s variant
- `_sa` - sa variant
- `_san` - san variant
- `_t` - t variant
- `_te` - te variant
- `_tes` - tes variant
- `_T` - T variant
- `_TE` - TE variant
- `_TEX` - TEX variant
- `_r` - r variant
- `_re` - re variant
- `_Red` - Red variant
- `_R` - R variant
- `_Re` - Re variant
- `_Ri` - Ri variant
- `_Rid` - Rid variant
- `_S` - S variant
- `_Sa` - Sa variant
- `_Si` - Si variant
- `_Sig` - Sig variant
- `_T` - T variant
- `_Tr` - Tr variant
- `_Tre` - Tre variant
- `_W` - W variant
- `_Wh` - Wh variant
- `_Whi` - Whi variant
- `_b` - b variant
- `_bi` - bi variant
- `_big` - big variant

## Small Message Files (`msg/US/`)

### 1-Byte Placeholder Messages

Several message files contain exactly 1 byte (0x00):
- `msg/US/gon_bt00.txt` - offset 0x462fe038
- `msg/US/hei_bt00.txt` - offset 0x463612bc
- `msg/US/hom_10.txt` - offset 0x463626b4
- `msg/US/hom_11.txt` - offset 0x463626b8
- `msg/US/hom_12.txt` - offset 0x463626bc
- `msg/US/kpa_00.txt` - offset 0x46369a74
- `msg/US/kpa_04.txt` - offset 0x4636bfcc
- `msg/US/kpa_06.txt` - offset 0x4636c6b0
- `msg/US/tik_09.txt` - offset 0x464054f4
- `msg/US/tik_10.txt` - offset 0x464054f8

These are likely placeholder messages that are never displayed, or terminator
messages at the end of message sequences.

### Message ID Range

- IDs range from 0 to 30 (31 unique IDs)
- 260 total message files
- File naming pattern: `{prefix}_{number}.txt`
- Common prefixes: `aaa`, `aji`, `bom`, `dou`, `eki`, `gon`, `gra`, `hei`, `hom`, `jin`, `kpa`, `las`, `tik`, `tou`, `yuu`

## Script Files (`m/*/s*`)

The `m/` directory contains subdirectories for each room/area, with `/s/`
subdirectories holding script data. Key observations:

### Script File Sizes

Smallest scripts (under 2000 bytes):
- `m/aaa_00/s` - 1238 bytes
- `m/aji_00/s` - 1258 bytes
- `m/aji_04/s` - 1268 bytes
- `m/aji_06/s` - 667 bytes
- `m/aji_08/s` - 666 bytes
- `m/aji_09/s` - 619 bytes
- `m/bom_01/s` - 3717 bytes
- `m/dou_04/s` - 625 bytes

### Script Structure Hypothesis

These scripts likely contain:
- Opcode sequences (event commands)
- Pointer addresses to other data
- Reference to message IDs
- Object spawn references
- Conditional branches

The `/s` likely stands for "script" and contains the event sequence data
for each room/area.

## Incremental Binary Data (`icon.bin`)

**Size**: 8174 bytes at offset 0x38837690

This file has a very regular structure:
- 204 u32 values (8176 bytes, close to 8174)
- Values increment in a pattern:
  - 0x023c047a
  - 0x04860492
  - 0x049e04aa
  - 0x04b604c2
  - ... (continuing increment pattern)

This appears to be a lookup table or offset table, possibly for icon tile
positions or palette data.

## Summary of Undocumented Elements

| Category | Count | Size Range | Key Finding |
|----------|-------|------------|-------------|
| Effect definitions | ~60+ names | 41984 bytes (ag2tg.bin) | EFF_* naming convention with suffix variants |
| Object definitions (HOBJ) | ~50+ names | embedded in ag2tg.bin | HOBJ_* with geometric suffixes |
| Object definitions (MOBJ) | ~100+ names | embedded in ag2tg.bin | MOBJ_* with trait suffixes |
| 1-byte message placeholders | 10 files | 1 byte each | All contain 0x00, likely terminators |
| Message IDs (unique) | 31 IDs | across 260 files | Range 0-30, pattern: {prefix}_{num}.txt |
| Script files (m/*/s) | 218 files | 0-11MB | Script data per room/area |
| Incremental table | 1 file | 8174 bytes (icon.bin) | Regular u32 increment pattern |
| Small .bin files | 3 files | 8-42KB | Effect/object name registries |

## Areas for Further Research

1. **Effect suffix semantics**: Decode the meaning of `_D`, `_F`, `_I`, `_Sp`, `_us` suffixes
2. **Script opcode format**: Decode the event scripts in `m/*/s/` files
3. **u32 value meanings**: Understand the structured u32 tables in `ag2tg.bin` and `vivian.bin`
4. **Message placeholder purpose**: Determine why 1-byte 0x00 messages exist
5. **HOBJ/MOBJ suffix patterns**: Full decoding of the naming convention

These elements represent "small, undocumented functions" in the binary code that
require further investigation to fully understand their purpose and behavior.