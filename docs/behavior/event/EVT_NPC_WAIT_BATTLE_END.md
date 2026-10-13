# evt_npc_wait_battle_end

**Address:** `0x8004efbc`  
**Size:** 184 bytes (46 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Overview

`evt_npc_wait_battle_end` is an event command that waits for the current
battle to complete. It checks battle state and returns appropriate status
codes to control event flow during battle sequences.

## Key behavior summary

- Validates battle state pointer (null check)
- If valid, calls `0x800bed44` for battle state validation
- Resolves two arguments from event argument structure (at offset 24):
  - `[arg+0]` — NPC ID
  - `[arg+4]` — battle type/flags
- Calls `0x80114500` with NPC ID
- Calls `0x8004f5d8` with battle configuration
- Calls `0x80043d80` for final setup
- Returns 0 (yield) or 2 (complete) based on battle state

## Critical offsets

| Offset | Size | Purpose |
|--------|------|---------|
| +0x18 | 4 | Argument structure pointer |
| +0x00 | 4 | NPC ID |
| +0x04 | 4 | Battle type/flags |

## Called functions

| Address | Purpose |
|---------|---------|
| `0x800bed44` | Battle state validation |
| `0x80031260` | Resolve argument values (2 calls) |
| `0x80114500` | NPC battle setup |
| `0x8004f5d8` | Battle configuration |
| `0x80043d80` | Final setup |

## Porting notes

This function is critical for battle event coordination. For porting:
1. Preserve battle state validation pipeline
2. Preserve two-argument resolution order
3. Preserve NPC and battle configuration setup
4. Handle return code semantics appropriately

## Remaining gaps

- Full battle state structure layout
- Battle configuration format details
- Relationship to encounter system
- Multi-NPC battle coordination logic
