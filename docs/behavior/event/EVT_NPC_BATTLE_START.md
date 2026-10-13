# evt_npc_battle_start

**Address:** `0x8004efbc`  
**Size:** 184 bytes (46 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Overview

`evt_npc_battle_start` is the event command that initiates an NPC battle
encounter. It resolves the NPC ID, sets up the battle, and triggers the
battle sequence transition.

## Key behavior summary

- Validates battle configuration pointer (null check)
- If valid, calls `0x800bed44` for battle validation
- Resolves two arguments from event argument structure (at offset 24):
  - `[arg+0]` — NPC ID
  - `[arg+4]` — battle type/flags
- Calls `0x80114500` with NPC ID for battle setup
- Calls `0x8004f5d8` with battle configuration
- Calls `0x80043d80` for battle initialization
- Returns 0 (yield) or 2 (complete) based on battle start result

## Critical offsets

| Offset | Size | Purpose |
|--------|------|---------|
| +0x18 | 4 | Argument structure pointer |
| +0x00 | 4 | NPC ID |
| +0x04 | 4 | Battle type/flags |

## Called functions

| Address | Purpose |
|---------|---------|
| `0x800bed44` | Battle validation |
| `0x80031260` | Resolve argument values (2 calls) |
| `0x80114500` | NPC battle setup |
| `0x8004f5d8` | Battle configuration |
| `0x80043d80` | Battle initialization |

## Porting notes

This function is critical for battle encounters. For porting:
1. Preserve battle validation pipeline
2. Preserve two-argument resolution order
3. Preserve NPC and battle setup pipeline
4. Coordinate with sequence system for battle transitions

## Remaining gaps

- Full battle configuration structure layout
- NPC ID to encounter mapping
- Relationship to sequence transition system
- Multi-NPC and boss battle coordination
