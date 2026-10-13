# evt_mario_jump_pos

**Address:** `0x800e4c54`  
**Size:** 1984 bytes (496 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Overview

`evt_mario_jump_pos` is a complex event command that performs scripted Mario
movement to a target position. It resolves multiple arguments, computes
trajectories, and integrates movement over multiple frames. This is used for
scripted jump sequences and event-driven movement.

## Key behavior summary

- Allocates 144 bytes of stack space for local variables (14 saved floats)
- Calls `0x8005e0c0` to get current Mario position
- If argument 4 (r4) is nonzero, clears offset 120 of the work structure
- Resolves 8 arguments from the event argument structure (at offset 24):
  - `[arg+0]` through `[arg+20]` — movement parameters
- Converts parameters from integer representation to float
- Main integration loop (mode-dependent):
  - Mode 0: Direct position computation
  - Mode 1: Angle-based movement
  - Mode 2: Target-relative angle computation
  - Mode 3: Target-relative angle with conversion
- Calls `0x8000722c` to compute velocity components from speed and angle
- Stores velocity at offsets 124-136 of the work structure
- Main integration loop:
  - Calls `0x8005e0c0` to get current position
  - Checks state flags (bit 7 of state word) for completion
  - Calls `0x8005de4` (position update) with velocity
  - Calls `0x80094020` (collision/movement integration)
  - Decrements counter at offset 128
  - Loop continues until counter reaches zero
- On completion: sets final position, clears state flags, calls
  `0x801a8ef4` (end-of-movement handler)
- Returns 0 (yield) or 1 (complete) based on whether movement finished

## Critical offsets (work structure)

| Offset | Size | Purpose |
|--------|------|---------|
| +0x00 | 4 | State flags (bit 7 = active, bit 8 = gravity) |
| +0x78 | 4 | Event timer / completion counter |
| +0x7c | 4 | Velocity X |
| +0x80 | 4 | Velocity Y |
| +0x84 | 4 | Velocity Z |
| +0x8c | 4 | Target position X |
| +0x90 | 4 | Target position Y |
| +0x94 | 4 | Target position Z |
| +0x80 | 4 | Frame counter / loop limit |

## Called functions

| Address | Purpose |
|---------|---------|
| `0x8005e0c0` | Get current Mario position |
| `0x800059cc` | Compute angle to target position |
| `0x8000703c` | Angle conversion |
| `0x8008ffbc` | Target-relative angle computation |
| `0x8000722c` | Velocity components from speed/angle |
| `0x8005de4` | Position update |
| `0x80094020` | Collision/movement integration |
| `0x801a8ef4` | End-of-movement handler |
| `0x8009455c` | Sequence entry |
| `0x8008f0e4` | Motion handler |

## Porting notes

This function is critical for scripted movement. For porting:
1. Preserve mode-dependent angle computation (modes 0-3)
2. Preserve velocity integration loop and collision handling
3. Preserve counter at offset 128 for frame limit
4. Preserve state flags at offset 0 (bits 7 and 8)
5. Handle GX rendering calls appropriately

## Remaining gaps

- Full semantics of all 4 modes and their argument combinations
- Exact collision integration behavior
- How the counter at offset 128 is initialized
- Relationship to event system's USER_FUNC continuation protocol
- How multiple concurrent jump_pos commands interact
