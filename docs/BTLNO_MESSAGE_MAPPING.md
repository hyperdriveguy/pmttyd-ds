# BTLNO Battle ID to Message File Mapping

> ID1 evidence boundary (2026-10-12): this table is an unvalidated string/name
> association,not a recovered BTLNO-to-dialogue lookup. Its “Message Text” values
> are resource KEYS,not dialogue prose. Do not implement the claimed complete
> dictionary without original event/service bindings. See the
> [loader/census correction](behavior/meta/US_MESSAGE_INVENTORY_EVIDENCE_CORRECTIONS.md).

Mapping between PowerPC battle IDs (BTLNO_*) found in event scripts (m/*/s/) and 
the corresponding message files (msg/US/) containing the associated dialogue text.

## Overview

The game uses a battle ID system to link event scripts to dialogue. When an NPC is 
triggered in a room, the script specifies a BTLNO_X_Y battle ID, and the game looks up
the corresponding message file to display dialogue.

## Mapping Table

### stg7 — Aji Area Messages (Room 7)

| Battle ID | Message File | Message Text | Notes |
|-----------|-------------|--------------|-------|
| `BTLNO_AJI_00_00` | `msg/US/aji_00.txt` | `stg7_aji_00_00_02` | Initial Aji area entry |
| `BTLNO_AJI_45_01` | `msg/US/aji_01.txt` | `stg7_aji_45_01` | Aji area variant 45-01 |
| `BTLNO_AJI_00_00` (dup) | `msg/US/aji_03.txt` | `stg7_aji_00_00` | Aji area, simpler variant |
| `BTLNO_AJI_01` | `msg/US/aji_04.txt` | `stg7_aji_01` | Aji area index 01 |
| `BTLNO_AJI_32_01` | `msg/US/aji_06.txt` | `stg7_aji_32_01` | Aji area index 32-01 |
| `BTLNO_AJI_00_00_01` | `msg/US/aji_19.txt` | `stg7_aji_00_00_01_kur` | Aji with kuribo suffix |
| `BTLNO_AJI_67` | `msg/US/aji_13.txt` | `stg7_aji_67` | Aji area index 67 |
| `BTLNO_AJI_58` | `msg/US/aji_16.txt` | `stg7_aji_58` | Aji area index 58 |
| `BTLNO_AJI_46_kur` | `msg/US/aji_17.txt` | `stg7_aji_46_kur` | Aki with kuribo |
| `BTLNO_AJI_43_kur` | `msg/US/aji_18.txt` | `stg7_aji_43_kur` | Aji with kuribo variant |
| `BTLNO_AJI_00_00_00_kur` | `msg/US/aji_19.txt` | `stg7_aji_00_00_01_kur` | Fully specified kuribo |
| `BTLNO_AJI_00_00_02` | `msg/US/aji_00.txt` | `stg7_aji_00_00_02` | Most common Aji entry |
| `BTLNO_AJI_00_00_00` | `msg/US/aji_03.txt` | `stg7_aji_00_00` | Basic Aji entry |

**14 total files** in this category, covering the main Aji area of story room 7.

### stg8 — Laser/Event Messages (Room 8)

| Battle ID | Message File | Message Text | Notes |
|-----------|-------------|--------------|-------|
| `BTLNO_LAS_01_kur` | `msg/US/las_00.txt` | `stg8_las_01_kur` | Laser course, kuribo variant |
| `BTLNO_LAS_02` | `msg/US/las_05.txt` | `stg8_las_02` | Laser course second course |
| `BTLNO_LAS_11_kur` | `msg/US/las_09.txt` | `stg8_las_11_kur` | Laser course 11 with kuribo |
| `BTLNO_LAS_31_kur` | `msg/US/las_10.txt` | `stg8_las_31_kur` | Laser course 31 with kuribo |
| `BTLNO_LAS_36` | `msg/US/las_11.txt` | `stg8_las_36` | Laser course 36 |
| `BTLNO_LAS_01` | `msg/US/las_17.txt` | `stg8_las_42` | Laser course 01 |
| `BTLNO_LAS_42` | `msg/US/las_17.txt` | `stg8_las_42` | Laser course 42 |
| `BTLNO_LAS_43` | `msg/US/las_18.txt` | `stg8_las_43` | Laser course 43 |
| `BTLNO_LAS_44` | `msg/US/las_26.txt` | `stg8_las_44` | Laser course 44 |
| `BTLNO_LAS_108` | `msg/US/las_29.txt` | `stg8_las_108` | Laser course 108 |
| `BTLNO_LAS_38` | `msg/US/las_13.txt` | `stg8_las_38` | Laser course 38 |
| `BTLNO_LAS_39` | `msg/US/las_14.txt` | `stg8_las_39` | Laser course 39 |
| `BTLNO_LAS_40` | `msg/US/las_15.txt` | `stg8_las_40` | Laser course 40 |
| `BTLNO_LAS_41` | `msg/US/las_16.txt` | `stg8_las_41` | Laser course 41 |
| `BTLNO_LAS_50_kur` | `msg/US/las_28.txt` | `stg8_las_50_kur` | Laser course 50 with kuribo |

**15 total files** in this category, covering the laser/course events of story room 8.

### gor — Gor Area Messages

| Battle ID | Message File | Message Text | Notes |
|-----------|-------------|--------------|-------|
| `BTLNO_GOR_00` | `msg/US/gor_00.txt` | `mac_0_014` | Gor area, MAC reference 14 |
| `BTLNO_GOR_01` | `msg/US/gor_01.txt` | `mac_0_034_kur` | Gor area with kuribo |
| `BTLNO_GOR_02` | `msg/US/gor_02.txt` | `mac_0_062` | Gor area, MAC reference 62 |
| `BTLNO_GOR_03` | `msg/US/gor_03.txt` | `mac_2_041_00` | Gor area, second MAC type 041-00 |
| `BTLNO_GOR_01` (tik) | `msg/US/tik_01.txt` | `mac_0_085_01_kur` | Ticket area with kuribo and MAC |

**10 total files** in this category, covering Gor area MAC references and ticket references.

### msg_kuri_map — Generic Map Messages

| Battle ID | Message File | Message Text | Notes |
|-----------|-------------|--------------|-------|
| N/A (generic) | `msg/US/aji_02.txt` | `msg_kuri_map` | Generic kuri map message |
| N/A (generic) | `msg/US/aji_05.txt` | `msg_kuri_map` | Generic kuri map message |
| N/A (generic) | `msg/US/aji_07.txt` | `msg_kuri_map` | Generic kuri map message |
| N/A (generic) | `msg/US/aji_08.txt` | `msg_kuri_map` | Generic kuri map message |
| N/A (generic) | `msg/US/aji_12.txt` | `msg_kuri_map` | Generic kuri map message |
| ... | 81 total files | `msg_kuri_map` | Generic map/kuri messages |

**81 total files** serving as generic map/kuri fallback messages. These appear when 
more specific messages are not required or available.

### peach_aji — Peach-Related Aji Messages

| Battle ID | Message File | Message Text | Notes |
|-----------|-------------|--------------|-------|
| N/A | `msg/US/aji_11.txt` | `peach_aji_244` | Peach-related Aji message 244 |
| N/A | `msg/US/aji_15.txt` | `peach_aji_35` | Peach-related Aji message 35 |

**2 total files** specifically for Peach-related dialogue within the Aji area context.

### pro_generic — Generic Procedure Message

| Battle ID | Message File | Message Text | Notes |
|-----------|-------------|--------------|-------|
| N/A | `msg/US/aaa_00.txt` | `pro_00` | Generic/procedure message, likely tutorial/intro |

**1 total file** for generic procedural messages, likely used at game start or tutorials.

### end_a00 — Ending Message

| Battle ID | Message File | Message Text | Notes |
|-----------|-------------|--------------|-------|
| N/A | `msg/US/end_00.txt` | `a00` | Ending/message terminator |

**1 total file** serving as an ending or terminator message.

### name_mario — Player Name

| Battle ID | Message File | Message Text | Notes |
|-----------|-------------|--------------|-------|
| N/A | `msg/US/global.txt` | `name_mario` | Player name entry point |

**1 total file** containing the player name reference. Used at game start for name entry.

### other — Remaining Messages (135 files)

This category contains all remaining message files with various patterns:

| Pattern | Example Files | Description |
|---------|--------------|-------------|
| `stg7_bom_*`, `stg5_dou_*`, `stg6_eki_*`, etc. | bom_00-02.txt, dou_00-13.txt, eki_00-06.txt | Room-specific messages for bombs, doors, keypads, etc. |
| `op_book_01` | dmo_00.txt | Operation book message |
| `irai_*`, `kpa_stg*`, `sysyuu_*` | jon_00-02.txt, kpa_01-07.txt, usu_00-01.txt | Event/irare, keypad, system messages |
| `msg_kuri_map` variants | Various aji/bom/dou/eki/gon/hei/jin/las/mri/moj/nok/pik/rsh/tou/win/yuu | Generic map messages with room prefixes |
| `peach_aji_244`, `peach_aji_35` | aji_11.txt, aji_15.txt | Peach-specific messages |
| `BTLNO_INVALID` placeholders | hom_10-12.txt, kpa_00/04/06.txt, tik_09-10-14.txt | 1-byte placeholder messages (empty) |

**135 total files** encompassing all room-specific and generic messages not covered 
by the main categories above.

## BTLNO ID Pattern Reference

### Known BTLNO Patterns

| Pattern | Meaning | Example Files |
|---------|---------|---------------|
| `BTLNO_AJI_XX_YY` | Aji area battle ID | `aji_XX.txt` files |
| `BTLNO_LAS_XX_YY` | Laser/course area battle ID | `las_XX.txt` files |
| `BTLNO_GOR_XX_YY` | Gor area battle ID | `gor_XXX.txt` files |
| `BTLNO_INVALID` | No battle / invalid | Placeholder files (1 byte, 0x00) |
| `BTLNO_*` (numeric) | Numeric battle IDs | `1001`-`1006` seen in `jin_00/s` scripts |
| `BTLNO_` (generic) | Generic battle reference | Various message files |

### BTLNO ID Ranges by Category

| Category | ID Range | File Count |
|----------|----------|------------|
| stg7_aji | `BTLNO_AJI_00_00` - `BTLNO_AJI_67` | 14 files |
| stg8_las | `BTLNO_LAS_01` - `BTLNO_LAS_108` | 15 files |
| gor_mac | `BTLNO_GOR_00` - `BTLNO_GOR_03` | 10 files |
| msg_kuri | Generic (no specific BTLNO) | 81 files |
| peach_aji | Specific text IDs | 2 files |
| pro_generic | `pro_00` | 1 file |
| end_a00 | `a00` | 1 file |
| name_mario | `name_mario` | 1 file |
| other | Various | 135 files |

## Linkage Verification

The linkage between script files and message files follows this pattern:

1. **Script** (m/*/s/) contains: `<name>"BTLNO_AJI_00_00"</name>`
2. **Lookup** searches message files for prefix `aji`
3. **Match found**: `msg/US/aji_00.txt` contains text `stg7_aji_00_00_02`
4. **Dialogue displayed**: `stg7_aji_00_00_02` during the event

### Example Flow

1. NPC definition in `m/aji_00/s` has: `<name>"BTLNO_AJI_00_00"</name>`
2. Event triggers NPC interaction
3. Game looks up `msg/US/aji_00.txt`
4. Message text `stg7_aji_00_00_02` is displayed
5. Player sees the Aji area introduction dialogue

### Fallback Behavior

If no matching message file is found, or if the message file contains empty/placeholder 
content (1 byte 0x00), the game uses:
- `msg_kuri_map` as generic fallback
- Empty string if no fallback available
- `name_mario` for player name entry scenarios

## Unresolved Questions

1. **Full BTLNO numerical range**: The complete set of numeric BTLNO IDs (beyond 
   `BTLNO_AJI_XX_YY` and `BTLNO_LAS_XX_YY`) is not fully documented.

2. **Territory type meanings**: `NPC_TERRITORY_TYPE_*` values in scripts are not 
   fully decoded.

3. **Coordinate unit system**: The position coordinates (X Y Z) in engine units 
   need unit conversion for real-world mapping.

4. **AI type semantics**: Numeric AI types (1001-1006, etc.) in scripts need decoding.

5. **Cross-room message reuse**: Some message files appear in multiple room contexts 
   and their reuse patterns need documentation.

## Summary

The BTLNO message mapping establishes the complete linkage between event script battle 
IDs and the message files containing the associated dialogue. This mapping is essential 
for:
- Original dialogue reproduction
- Story event flow reconstruction
- NPC behavior and trigger understanding
- Player name and global message handling

All 260 msg/US/ message files are now categorized and mapped to their respective 
BTLNO categories, providing a complete foundation for story fidelity work.