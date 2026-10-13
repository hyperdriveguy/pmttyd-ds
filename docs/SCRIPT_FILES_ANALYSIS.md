# Script Files Analysis (m/*/s/)

Analysis of the XML-based event script files from Paper Mario ISO (G8ME01).

## Overview

The `m/*/s/` directory contains event script files for each room/area in the game.
These files use an XML-like format to define NPC (Non-Player Character) behavior,
including placement, AI type, battle parameters, and territory settings.

## File Structure

The `m/` directory has the following subdirectory patterns:

| Subdirectory | Description | Example File Count |
|-------------|-------------|-------------------|
| `c/` | Code/data | Varies by room |
| `d/` | Data | Varies by room |
| `s/` | **Scripts** (NPC definitions) | Varies by room (0-100+) |
| `t/` | Textures | Varies by room |

### Script File Sizes

| Room | File Size | Notes |
|------|-----------|-------|
| `m/aaa_00/s` | 1,238 bytes | Minimal NPC definitions |
| `m/aji_00/s` | 1,258 bytes | Minimal NPC definitions |
| `m/gon_00/s` | 0 bytes | Empty/no NPCs |
| `m/hei_00/s` | 1,868 bytes | Standard NPC definitions |
| `m/jin_00/s` | 10,695 bytes | Complex NPC definitions |
| `m/mri_00/s` | Varies | Multiple NPCs |
| `m/tik_00/s` | Varies | Ticket-related NPCs |

## XML Format Structure

The scripts use a simple XML-like tag format:

```xml
<Npc>
   <name>"BTLNO_AJI_00_00"</name>
   <tribe>"NPC_TERRITORY_TYPE_NOTHING"</tribe>
   <position>0 -1000 0 </position>
   <dir>270</dir>
   <aitype></aitype>
   <territory_type>NPC_TERRITORY_TYPE_NOTHING</territory_type>
   <territory_base>0 0 0</territory_base>
   <territory_loiter>0 0 0</territory_loiter>
   <search_range>0</search_range>
   <search_angle>0</search_angle>
   <homing_range>0</homing_range>
   <homing_angle>0</homing_angle>
   <btl_setup_no>BTLNO_INVALID</btl_setup_no>
</Npc>
```

### Tag Meanings

| Tag | Meaning | Typical Values |
|-----|---------|----------------|
| `<name>` | NPC battle ID | `BTLNO_AJI_00_00`, `BTLNO_INVALID`, `BTLNO_*` |
| `<tribe>` | Territory type | `NPC_TERRITORY_TYPE_NOTHING`, `NPC_TERRITORY_TYPE_*` |
| `<position>` | X Y Z coordinates | `0 -1000 0`, `150 100 100`, etc. |
| `<dir>` | Direction angle | `90`, `180`, `270`, `360`, etc. |
| `<aitype>` | AI type | Usually empty, sometimes numeric |
| `<btl_setup_no>` | Battle setup number | `BTLNO_INVALID`, `BTLNO_AJI_00_00`, etc. |
| `<territory_>` | Territory parameters | Base, loiter, homing ranges/angles |
| `<search_>` | Search parameters | Range and angle |
| `<homing_>` | Homing parameters | Range and angle |

### NPC Battle IDs (btl_setup_no)

Known battle ID patterns:
- `BTLNO_INVALID` - Invalid/no battle
- `BTLNO_AJI_00_00` - Area AJI index 00-00
- `BTLNO_AJI_45_01` - Area AJI index 45-01
- `BTLNO_*` - General battle ID pattern
- Numerical IDs: `1001`, `1002`, `1003`, `1004`, `1005`, `1006` (seen in jin_00/s)

### Territory Types

- `NPC_TERRITORY_TYPE_NOTHING` - No territory restriction
- Other `NPC_TERRITORY_TYPE_*` values exist (not fully decoded)

### AI Types

- Usually empty/`undefined`
- Sometimes numeric values (e.g., `1001`, `1002`, `1003`, `1004`, `1005`, `1006`)

## Script Content Analysis

### Common Patterns Across Rooms

| Pattern | Frequency | Description |
|---------|-----------|-------------|
| `<position>0 -1000 0</position>` | Very common | Default "far away" position |
| `<dir>270</dir>` | Very common | Default facing direction (west) |
| `<name>"BTLNO_INVALID"</name>` | Common | NPCs that don't trigger battle |
| `<name>"BTLNO_AJI_XX_YY"` | Common | NPCs that trigger specific battles |
| `<territory_type>NPC_TERRITORY_TYPE_NOTHING</territory_type>` | Common | No territory restrictions |
| Coordinate values like `0 -1000 0` | Common | Default "off-screen" position |
| Coordinate values like `150 100 100` | Common | In-room position |

### Room-Specific Observations

- **`m/aaa_00/s`** (1,238 bytes): Minimal NPCs, all with default positions
- **`m/aji_00/s`** (1,258 bytes): Similar to aaa_00, minimal definitions
- **`m/gon_00/s`** (0 bytes): No NPC definitions (empty file)
- **`m/hei_00/s`** (1,868 bytes): Standard NPC definitions similar to aaa_00
- **`m/jin_00/s`** (10,695 bytes): Complex with 788 unique strings, includes battle IDs 1001-1006
- **`m/gon_01/s`** (237,440 bytes): Very large, likely contains many NPC definitions

### NPC Territory Types (partial)

| Territory Type | Meaning (hypothesis) |
|---------------|----------------------|
| `NPC_TERRITORY_TYPE_NOTHING` | No restrictions, NPC roams freely |
| Other types | Likely restrict NPC to specific areas/paths |

## Relationship to Message Files

The NPC battle IDs in the script files (`BTLNO_AJI_XX_YY`, etc.) correspond to message IDs in the `msg/US/` directory. For example:

- Script has `<name>"BTLNO_AJI_00_00"</name>` → Message file `aji_00.txt` contains `stg7_aji_00_00_02`
- Script has `<name>"BTLNO_AJI_45_01"</name>` → Message file `aji_01.txt` contains `stg7_aji_45_01`
- Script has `<name>"1001"</name>` → Message file mapping varies

This establishes the link between:
1. **Event scripts** (m/*/s/) - NPC definitions and battle triggers
2. **Message files** (msg/US/) - Dialogue/text references
3. **Room positions** - Where NPCs appear in the game world

## Areas for Further Research

1. **Full XML schema**: Decode complete tag structure and all possible attributes
2. **Battle ID mapping**: Map `BTLNO_*` IDs to actual battle encounters and message texts
3. **Territory type meanings**: Decode `NPC_TERRITORY_TYPE_*` values
4. **AI type meanings**: Decode numeric AI type values (1001-1006, etc.)
5. **Coordinate system**: Understand the unit system for position coordinates (engine units?)
6. **NPC behavior patterns**: How territory types affect NPC movement and behavior
7. **Cross-reference with REL files**: Link NPC definitions to actual in-game entity placements

## Summary

The `m/*/s/` script files define NPC behavior using a compact XML format. Each file contains one or more `<Npc>` entries with properties for:
- Name/battle ID (linking to msg/US/ messages)
- Position (X, Y, Z coordinates in engine units)
- Direction (angle in degrees)
- AI type (numeric or empty)
- Territory type and parameters
- Search and homing parameters
- Battle setup number

These scripts serve as the event-driven definition of who appears in each room, where they stand, and what happens when the player interacts with them (typically triggering a battle with associated dialogue from msg/US/).