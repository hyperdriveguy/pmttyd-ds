# Message Files Analysis (msg/US/)

> ID1 evidence correction (2026-10-12): this draft's message-ID/identifier-only
> and prefix/story interpretations are unvalidated and contradicted by the
> [US loader/census correction](behavior/meta/US_MESSAGE_INVENTORY_EVIDENCE_CORRECTIONS.md).
> Actual resources contain key/text pairs;numeric filename suffixes are not message IDs.

Analysis of the US English message files from Paper Mario ISO (G8ME01).

## Overview

The `msg/US/` directory contains 260 message files with identifiers used throughout the game's event system. These files contain reference strings rather than full dialogue text - they appear to be message IDs or event identifiers that reference actual dialogue content.

## File Statistics

| Metric | Value |
|--------|-------|
| Total message files | 260 |
| Empty/placeholder files | 11 (1 byte each, containing 0x00) |
| Large files (>10 KB) | 37 |
| Small files (<100 bytes) | 146 |

## Empty/Placeholder Files

11 files contain exactly 1 byte (0x00), likely serving as placeholders or terminators:

| File | Size | Purpose |
|------|------|---------|
| `gon_bt00.txt` | 1 | Battle gate placeholder |
| `hei_bt00.txt` | 1 | Header/battle placeholder |
| `hom_10.txt` | 1 | Home/field placeholder |
| `hom_11.txt` | 1 | Home placeholder |
| `hom_12.txt` | 1 | Home placeholder |
| `kpa_00.txt` | 1 | Keypad placeholder |
| `kpa_04.txt` | 1 | Keypad variant |
| `kpa_06.txt` | 1 | Keypad variant |
| `tik_09.txt` | 1 | Ticket placeholder |
| `tik_10.txt` | 1 | Ticket placeholder |
| `tik_14.txt` | 1 | Ticket placeholder |

## Text Content Analysis

The non-empty message files contain identifier strings following patterns:

### Prefix Distribution (Top 20)

| Prefix | Count | Description |
|--------|-------|-------------|
| `msg` | 81 | Generic message references |
| `stg7` | 21 | Story room 7 entries |
| `stg2` | 21 | Story room 2 entries |
| `stg5` | 15 | Story room 5 entries |
| `stg1` | 15 | Story room 1 entries |
| `stg6` | 15 | Story room 6 entries |
| `stg8` | 15 | Story room 8 entries |
| `stg3` | 13 | Story room 3 entries |
| `stg4` | 10 | Story room 4 entries |
| `mac` | 10 | MAC/room identifier references |
| `irai` | 8 | "Irai" (dialogue/event) references |
| `kpa` | 6 | Keypad/control references |
| `tik` | 4 | Ticket/message references |
| `sysyuu` | 4 | "System/routine" references |
| `pro` | 2 | Procedure/progress references |
| `peach` | 2 | Peach-related references |
| `op` | 1 | Operation/procedure |
| `a00` | 1 | Entry ID a00 |
| `name` | 1 | Player name reference |
| `gor` | 1 | Gor related |

### Room-Based Naming Convention

Files are organized by story room (stg1-stg8), with each room having multiple message IDs:

- **stg1**: Initial area (typically starting area)
- **stg2**: Mid-game area
- **stg3**: Later area
- **stg4**: Late game area
- **stg5**: End-game area
- **stg6**: Final dungeon/area
- **stg7**: Final areas/boss rushes
- **stg8**: Post-game/credits

### Identifier Format

Message identifiers follow the pattern: `[room]_[category]_[index]_[variant]`

Examples:
- `stg7_aji_00_00_02` - Story room 7, area type Aji, index 00, variant 00, type 02
- `stg7_aji_45_01` - Story room 7, area type Aji, index 45, variant 01
- `msg_kuri_map` - Generic message for Kur map
- `mac_0_014` - MAC reference, ID 14
- `mac_0_034_kur` - MAC reference with "kur" suffix, ID 34
- `op_book_01` - Operation book reference
- `powerdown_00` - Power down event

### Special Suffix Patterns

| Suffix | Meaning (hypothesis) | Frequency |
|--------|----------------------|-----------|
| `_kur` | Kuribo/character related | Several |
| `_viv` | Vivian-related | A few |
| `_kur` | Kuribo | Common in later rooms |
| `_e00`, `e01`, `e02` | Entry variants | Common |
| `_00`, `_01`, `_02` | Variant indices | Very common |

### Largest Files

| File | Size | Content Pattern |
|------|------|-----------------|
| `global.txt` | 239847 | Contains "name_mario" - player name entry |
| `tik_00.txt` | 149321 | Ticket/macro references |
| `gor_02.txt` | 128972 | MAC references with `mac_0_*` IDs |
| `gor_01.txt` | 96245 | MAC references |
| `gor_03.txt` | 83314 | `mac_2_041_00` format |
| `tou_03.txt` | 71993 | Story room 3 entries |
| `gor_00.txt` | 59142 | Initial MAC references |
| `tou_01.txt` | 52356 | Story room 1 entries |
| `tou_08.txt` | 50863 | Story room 8 entries |
| `tou_10.txt` | 43933 | Story room 10+ entries |

### `global.txt` Analysis

The `global.txt` file (239847 bytes) is the largest message file and contains:
- `name_mario` - Player name entry point
- Likely contains all global game state messages
- Used as a reference table for player name and global messages

## Message ID Mapping

Message IDs range from 0 to 30 (31 unique IDs), with files named:
- `{prefix}_{id}.txt` pattern (e.g., `aji_00.txt`, `gon_00.txt`, `tik_00.txt`)

ID ranges per prefix:
- `aji_XX`: IDs 0-19
- `gon_XX`: IDs 0-13
- `hei_XX`: IDs 0-10
- `hom_XX`: IDs 0-12
- `jin_XX`: IDs 0-11
- `kpa_XX`: IDs 0-7
- `las_XX`: IDs 0-30 (largest set)
- `moo_XX`: IDs 0-7
- `mri_XX`: IDs 0-19
- `muj_XX`: IDs 0-119 (very broad range)
- `nok_XX`: IDs 0-1
- `pik_XX`: IDs 0-12
- `rsh_XX`: IDs 0-8
- `tik_XX`: IDs 0-21
- `tou_XX`: IDs 0-50
- `usu_XX`: IDs 0-1
- `win_XX`: IDs 0-6
- `yuu_XX`: IDs 0-3

## Areas for Further Research

1. **Dialogue text mapping**: How message IDs reference actual spoken dialogue
2. **Control tokens**: Any special formatting or control sequences in the messages
3. **Room event mapping**: Precise mapping of message IDs to story events
4. **Variant differences**: Meaning of the suffix/index variants (e.g., `_00` vs `_01` vs `_02`)
5. **Global vs room-specific**: Distinction between `global.txt` and room-specific files
6. **The "name_mario" entry**: Purpose and structure of the global player name message

## Summary

The msg/US/ message system uses a structured identifier format rather than containing full dialogue text. The identifiers follow room-based naming (`stg1`-`stg8`) with category prefixes and index numbers. Eleven 1-byte placeholder files likely serve as markers or terminators in the event system. The largest file (`global.txt`, 239KB) contains the player name entry and global messages.