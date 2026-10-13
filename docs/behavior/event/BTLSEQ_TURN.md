# btlseqTurn

**Address:** `0x80042b94`  
**Size:** 7896 bytes (1974 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Overview

`btlseqTurn` is the main battle turn sequence controller. It manages the
complete turn lifecycle including player command selection, enemy action
execution, turn resolution, and transition to the next turn. This is the
central battle flow orchestrator.

## Key behavior summary

- Manages turn state machine (player phase, enemy phase, resolution)
- Coordinates player command selection and execution
- Schedules and executes enemy actions
- Manages turn counters and phase transitions
- Handles turn-end conditions (victory, defeat, escape)
- Coordinates with damage calculation and status systems
- Manages event script execution during turn

## Critical offsets (battle sequence structure)

| Offset | Size | Purpose |
|--------|------|---------|
| +0x00 | 4 | Turn state |
| +0x04 | 4 | Turn counter |
| +0x08 | 4 | Current phase |
| +0x0c | 4 | Active unit index |

## Called functions

| Address | Purpose |
|---------|---------|
| `0x800fa45c` | BattleActionCommandSetup |
| `0x800fa5b0` | BattleActionCommandManager |
| `0x8005f958` | BattleCalculateDamage |
| `0x800fa1b8` | Defense result handler |
| `0x800d3d30` | pouchAddHP |
| `0x800d3300` | Item/resource lookup |
| `0x8002e218` | seqMain (turn transition) |

## Porting notes

This is the single most complex function in the battle system. For porting:
1. Preserve turn state machine and phase ordering
2. Preserve player command selection pipeline
3. Preserve enemy action scheduling and execution
4. Preserve turn counter management and phase transitions
5. Preserve victory/defeat/escape condition handling
6. Preserve coordination with damage, status, and event systems

## Remaining gaps

- Full turn state machine with all transitions
- Player command selection pipeline details
- Enemy action scheduling algorithm
- Phase transition conditions and timing
- Victory/defeat/escape determination logic
- Event script integration during turn
- RNG consumption and seed management
- Status effect interaction with turn flow
