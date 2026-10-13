# BattleSetStatusDamage

**Address:** `0x800fcf04`  
**Size:** 4220 bytes (1055 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Overview

`BattleSetStatusDamage` applies status effect damage to a battle unit. It
handles the damage calculation and application for status effects such as
Poison, Burn, and other periodic damage sources. This is the primary status
damage execution path.

## Key behavior summary

- Receives unit pointer and damage amount
- Checks status flags and immunity
- Applies damage to unit HP
- Handles damage modifiers and reductions
- Updates unit state and flags
- Triggers damage events and animations
- Handles unit defeat (HP reaching 0)
- Coordinates with damage display system

## Critical offsets (unit structure)

| Offset | Size | Purpose |
|--------|------|---------|
| +0x00 | 4 | Unit flags |
| +0x04 | 4 | Status flags |
| +0x08 | 4 | Current HP |
| +0x0c | 4 | Max HP |
| +0x10 | 4 | Defense value |
| +0x14 | 4 | Status modifiers |

## Called functions

| Address | Purpose |
|---------|---------|
| `0x800d9848` | Status check |
| `0x8005f958` | BattleCalculateDamage |
| `0x800fa1b8` | Defense result handler |
| `0x800d3d30` | pouchAddHP |
| `0x8002e218` | seqMain (defeat transition) |
| `0x8012984c` | Damage event trigger |
| `0x8011ace8` | Unit state update |
| `0x800f895c` | Status flag update |

## Porting notes

This function is critical for status damage mechanics. For porting:
1. Preserve status flag checking and immunity logic
2. Preserve damage calculation pipeline
3. Preserve HP modification and defeat handling
4. Preserve damage event triggering and animation
5. Preserve coordination with display system
6. Handle all status types (Poison, Burn, Freeze, etc.)
7. Preserve damage modifier stacking rules

## Remaining gaps

- Full status effect type definitions
- Immunity and modifier interaction rules
- Damage event animation timing
- Defeat sequence details
- Status duration and tick management
- Multi-unit status damage coordination
