# BattleCalculateDamage

**Address:** `0x8005f958`  
**Size:** 1692 bytes (423 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Overview

`BattleCalculateDamage` is the central damage calculation function for battles.
It applies weapon power, badge modifiers, status effects, defense values, and
RNG to determine final damage. This is the core combat math pipeline.

## Key behavior summary

- Receives attacker, defender, weapon data, and battle context
- Retrieves weapon power from weapon data or power callbacks
- Applies badge modifiers (attack badges, status badges)
- Applies status effects (Poison, Burn, Freeze, etc.)
- Applies defense values from defender unit data
- Applies RNG variance to final damage
- Handles special cases: Superguard, Guard, Pity Flower
- Returns final damage value

## Critical offsets (battle work structure)

| Offset | Size | Purpose |
|--------|------|---------|
| +0x00 | 4 | Attacker unit pointer |
| +0x04 | 4 | Defender unit pointer |
| +0x08 | 4 | Weapon data pointer |
| +0x0c | 4 | Base damage |
| +0x10 | 4 | Final damage (output) |
| +0x14 | 4 | Modifier flags |
| +0x18 | 4 | RNG seed/state |

## Called functions

| Address | Purpose |
|---------|---------|
| `0x80189d30` | weaponGetPowerDefault |
| `0x800d5eb0` | pouchGetPtr |
| `0x800fa1b8` | Defense result handler |
| `0x80094020` | Status effect integration |
| `0x8009455c` | Sequence entry |
| `0x8009412c` | RNG processing |

## Porting notes

This function is critical for faithful combat. For porting:
1. Preserve weapon power calculation pipeline
2. Preserve badge modifier stacking rules
3. Preserve status effect interactions
4. Preserve defense value application order
5. Preserve RNG variance and seed management
6. Preserve special case handling (Guard, Superguard, Pity Flower)

## Remaining gaps

- Full weapon data structure layout
- Badge modifier stacking rules and priorities
- Status effect interaction matrix
- Defense value source and calculation
- RNG algorithm and seed management
- Special case trigger conditions and effects
