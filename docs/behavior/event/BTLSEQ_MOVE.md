# btlseqMove

**Address:** `0x8011c264`  
**Size:** 1680 bytes (420 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Overview

`btlseqMove` is the battle move sequence controller. It manages the execution
of movement actions during battle, including approach, positioning, and
retreat sequences. This is the central movement orchestrator for battle
actions.

## Key behavior summary

- Manages move state machine (startup, approach, execution, resolution)
- Coordinates unit positioning and movement
- Handles move timing and animation
- Manages move results and damage application
- Coordinates with damage and status systems
- Handles move interruption and cancellation

## Critical offsets (move sequence structure)

| Offset | Size | Purpose |
|--------|------|---------|
| +0x00 | 4 | Move state |
| +0x04 | 4 | Current action pointer |
| +0x08 | 4 | Timing counter |
| +0x0c | 4 | Result code |

## Called functions

| Address | Purpose |
|---------|---------|
| `0x800f8a08` | Move validation |
| `0x800f895c` | Action execution |
| `0x8005f958` | BattleCalculateDamage |
| `0x8012984c` | Damage event trigger |
| `0x8011ace8` | State update |

## Porting notes

This function is critical for battle movement execution. For porting:
1. Preserve move state machine and transitions
2. Preserve unit positioning and movement coordination
3. Preserve move timing and animation handling
4. Preserve result processing and damage application
5. Preserve coordination with damage and status systems

## Remaining gaps

- Full move state machine with all transitions
- Unit positioning and movement algorithm
- Move timing and animation curves
- Result processing pipeline
- Damage and status system integration
