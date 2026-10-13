# BattleDamageDirect

**Address:** `0x800feaa8`  
**Size:** 2852 bytes (713 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Overview

`BattleDamageDirect` applies direct damage to a battle unit without going
through the full damage calculation pipeline. It handles immediate HP/FP
reduction, status application, and defeat checking. This is used for
environmental damage, script-driven damage, and other direct damage sources.

## Key behavior summary

- Receives unit pointer and damage amount
- Checks unit validity and damage applicability
- Applies damage directly to HP or FP
- Handles damage modifiers and reductions
- Updates unit state and flags
- Triggers damage events and animations
- Handles unit defeat (HP reaching 0)
- Coordinates with status and display systems

## Critical offsets (unit structure)

| Offset | Size | Purpose |
|--------|------|---------|
| +0x00 | 4 | Unit flags |
| +0x04 | 4 | Status flags |
| +0x08 | 4 | Current HP |
| +0x0c | 4 | Max HP |
| +0x10 | 4 | Current FP |
| +0x14 | 4 | Max FP |
| +0x18 | 4 | Defense modifiers |

## Called functions

| Address | Purpose |
|---------|---------|
| `0x8019f788` | Unit validation |
| `0x8019f400` | Damage application |
| `0x80057878` | State cleanup |
| `0x80096e8c` | Bottomless check |
| `0x800bc574` | Gravity handler |
| `0x801a924c` | Camera update |
| `0x800d3d30` | pouchAddHP |
| `0x8002e218` | seqMain (defeat transition) |
| `0x8012984c` | Damage event trigger |
| `0x8011ace8` | Unit state update |
| `0x800f895c` | Status flag update |

## Porting notes

This function is critical for direct damage mechanics. For porting:
1. Preserve direct damage application pipeline
2. Preserve HP/FP modification and defeat handling
3. Preserve damage event triggering and animation
4. Preserve coordination with status and display systems
5. Handle all damage types (HP, FP, status)
6. Preserve defeat sequence and transition

## Remaining gaps

- Full damage modifier and reduction rules
- Defeat sequence details and timing
- Status application interaction
- Multi-unit damage coordination
- Environmental damage source handling
- Script-driven damage integration
