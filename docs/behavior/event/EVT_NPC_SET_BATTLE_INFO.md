# evt_npc_set_battle_info

**Address:** `0x8004f0a4`  
**Size:** 100 bytes (25 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Overview

`evt_npc_set_battle_info` is an event command that configures an NPC's battle
information. It resolves two IDs (NPC and battle configuration) and calls the
battle setup function.

## Key behavior summary

- Resolves two arguments from event argument structure (at offset 24):
  - `[arg+0]` — NPC ID
  - `[arg+4]` — battle configuration ID
- Calls `0x80031260` twice to resolve arguments
- Calls `0x8004f5d8` with resolved values for battle setup
- Calls `0x80043d80` for final configuration
- Returns 2 (complete)

## Critical offsets

| Offset | Size | Purpose |
|--------|------|---------|
| +0x18 | 4 | Argument structure pointer |
| +0x00 | 4 | NPC ID |
| +0x04 | 4 | Battle configuration ID |

## Called functions

| Address | Purpose |
|---------|---------|
| `0x80031260` | Resolve argument values (2 calls) |
| `0x8004f5d8` | Battle setup |
| `0x80043d80` | Final configuration |

## Porting notes

This function is critical for NPC battle configuration. For porting:
1. Preserve two-argument resolution order
2. Preserve battle setup and configuration pipeline
3. Handle NPC ID and battle configuration ID appropriately

## Remaining gaps

- Full battle configuration structure layout
- NPC ID to unit mapping
- Relationship to encounter system
- Multi-NPC battle coordination logic
