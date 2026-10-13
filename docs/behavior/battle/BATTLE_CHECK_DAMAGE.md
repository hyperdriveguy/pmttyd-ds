# BattleCheckDamage

**Address:** `0x800fb9cc`  
**Size:** 2560 bytes (640 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Overview

`BattleCheckDamage` validates and processes damage before application. It
checks damage eligibility, applies modifiers, determines damage type, and
routes to the appropriate damage handler. This is the gatekeeper function
for all damage in the battle system.

## Key behavior summary

- Receives attacker, defender, and damage data
- Validates damage eligibility (immunity, status, flags)
- Applies damage modifiers (badges, status effects, defense)
- Determines damage type (HP, FP, status)
- Routes to appropriate damage handler
- Handles special cases (Superguard, Guard, Pity Flower)
- Coordinates with RNG system for variance
- Returns damage result code

## Critical offsets (damage data structure)

| Offset | Size | Purpose |
|--------|------|---------|
| +0x00 | 4 | Attacker unit pointer |
| +0x04 | 4 | Defender unit pointer |
| +0x08 | 4 | Base damage amount |
| +0x0c | 4 | Damage type flags |
| +0x10 | 4 | Modifier flags |
| +0x14 | 4 | Result code (output) |

## Called functions

| Address | Purpose |
|---------|---------|
| `0x800fdfd4` | Damage validation |
| `0x800f8a08` | Modifier application |
| `0x8012984c` | Damage event trigger |
| `0x8011ace8` | Unit state update |
| `0x800f895c` | Status flag update |
| `0x8009455c` | Sequence entry |
| `0x8009412c` | RNG processing |
| `0x80094020` | Status integration |

## Porting notes

This function is critical for damage validation and routing. For porting:
1. Preserve damage eligibility checking
2. Preserve modifier application order
3. Preserve damage type determination
4. Preserve routing to appropriate handlers
5. Preserve special case handling (Guard, Superguard, Pity Flower)
6. Preserve RNG coordination for variance

## Remaining gaps

- Full modifier stacking rules and priorities
- Immunity and status interaction matrix
- Special case trigger conditions
- RNG variance algorithm
- Result code definitions and meanings
- Multi-damage type handling
