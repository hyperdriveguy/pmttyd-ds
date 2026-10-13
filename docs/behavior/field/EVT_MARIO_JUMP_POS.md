# evt_mario_jump_pos

**Address:** `0x800e4c54`  
**Size:** 1984 bytes (496 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Overview

`evt_mario_jump_pos` is a complex event command that performs scripted Mario
movement to a target position. It resolves multiple arguments, computes
trajectories, and integrates movement over multiple frames. This is used for
scripted jump sequences and event-driven movement.

## Disassembly summary

The function:
1. Allocates 144 bytes of stack space for local variables (14 saved floats)
2. Calls `0x8005e0c0` (get current Mario position) — result in f31
3. If argument 4 (r4) is nonzero, clears offset 120 of the work structure
4. Resolves 8 arguments from the event argument structure (at offset 24):
   - `[arg+0]` → float (f28) — target X or speed
   - `[arg+4]` → float (f30) — target Y or distance
   - `[arg+8]` → float (f27) — target Z or height
   - `[arg+12]` → integer (r29) — mode/flags
   - `[arg+16]` → integer (r28) — secondary mode
   - `[arg+20]` → float (f26) — speed parameter
5. Converts f26 from integer representation to float
6. If mode == 0: calls `0x800059cc` to get angle to target
7. If mode == 1: uses provided angle (f28)
8. If mode == 2: calls `0x800059cc` for angle, then `0x8000703c` for
   angle conversion
9. If mode == 3: calls `0x8008ffbc` for target-relative angle
10. Calls `0x8000722c` to compute velocity components from speed and angle
11. Stores velocity at offsets 124-136 of the work structure
12. If mode == 0: also stores Y velocity at offset 416
13. Main integration loop (mode-dependent):
    - Calls `0x8005e0c0` to get current position
    - Checks state flags (bit 7 of state word) for completion
    - Calls `0x8005de4` (position update) with velocity
    - Calls `0x80094020` (collision/movement integration)
    - Decrements counter at offset 128
    - Loop continues until counter reaches zero
14. On completion: sets final position, clears state flags, calls
    `0x801a8ef4` (end-of-movement handler)
15. Returns 0 (yield) or 1 (complete) based on whether movement finished

## Key offsets (work structure)

| Offset | Size | Purpose |
|--------|------|---------|
| 0x00 | 4 | State flags (bit 7 = active, bit 8 = gravity) |
| 0x78 | 4 | Event timer / completion counter |
| 0x7c | 4 | Velocity X |
| 0x80 | 4 | Velocity Y |
| 0x84 | 4 | Velocity Z |
| 0x8c | 4 | Target position X |
| 0x90 | 4 | Target position Y |
| 0x94 | 4 | Target position Z |
| 0x80 | 4 | Frame counter / loop limit |

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
| `0x800a0b0c` | Input processing |
| `0x800a15a8` | State machine |
| `0x800a1684` | State update |

## Critical for porting

This function handles all scripted Mario movement in events. For porting, the
key requirements are:

1. Preserve the mode-dependent angle computation (modes 0-3)
2. Preserve the velocity integration loop and collision handling
3. The counter at offset 128 controls how many frames the movement lasts
4. State flags at offset 0 control whether movement is active
5. The function must yield (return 0) during movement and complete (return 1)
   when done

## Remaining gaps

- Full semantics of all 4 modes and their argument combinations
- Exact collision integration behavior
- How the counter at offset 128 is initialized
- Relationship to the event system's USER_FUNC continuation protocol
- How multiple concurrent jump_pos commands interact
