# btlseqAct

**Address:** `0x8011bb38`  
**Size:** 2220 bytes (555 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Overview

`btlseqAct` is the battle action sequence controller. It manages the execution
of player and enemy actions during battle, including action timing, animation
coordination, and result processing. This is the central action execution
orchestrator.

## Key behavior summary

- Manages action state machine (startup, execution, resolution)
- Coordinates player command execution
- Schedules and executes enemy actions
- Manages action timing and animation
- Handles action results and damage application
- Coordinates with event script system
- Manages action queue and priority
- Handles action interruption and cancellation

## Critical offsets (action sequence structure)

| Offset | Size | Purpose |
|--------|------|---------|
| +0x00 | 4 | Action state |
| +0x04 | 4 | Current action pointer |
| +0x08 | 4 | Action queue |
| +0x0c | 4 | Timing counter |
| +0x10 | 4 | Result code |

## Called functions

| Address | Purpose |
|---------|---------|
| `0x80036604` | State classification |
| `0x8002e218` | Sequence helper |
| `0x80094020` | Timing integration |
| `0x8009455c` | Result processing |
| `0x800f895c` | Action execution |
| `0x8011ace8` | State update |

## Porting notes

This function is critical for battle action execution. For porting:
1. Preserve action state machine and transitions
2. Preserve player command execution pipeline
3. Preserve enemy action scheduling
4. Preserve action timing and animation coordination
5. Preserve result processing and damage application
6. Preserve event script coordination

## Remaining gaps

- Full action state machine with all transitions
- Player command execution details
- Enemy action scheduling algorithm
- Action timing and animation curves
- Result processing pipeline
- Event script integration details
