# marioMain

**Address:** `0x8005a7e0`  
**Size:** 2528 bytes (632 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Overview

`marioMain` is the primary field player update function, called once per
retrace from `marioStMain`. It dispatches player motion states, processes
input, handles collision queries, integrates movement, and manages battle
transitions. This is the central execution path for overworld Mario
behavior.

## Key behavior summary

- Reads Mario state from `[r13 + 0x19e0]` (SDA global)
- Validates state nonzero; returns early if zero
- Processes input edges from `[r13 + 0x90b0]` (input work) offset 4920,
  masked with 0x100f (input validation gate)
- Checks player mode byte at offset 56; mode 1 or 2 routes to special
  handlers
- Calls `0x80096a08` for state validation
- Sets gravity at `[r30 + 552]` to 1.0 (constant at `0x80428bc8`), or 0.5
  for moon rooms (motion ID 0x1d = 29)
- Calls a series of subsystem functions for movement integration:
  - `0x801a938c` — primary motion dispatch
  - `0x800916f0` — motion parameter update
  - `0x8026d464` (called 6 times with offsets 864-904) — per-frame
    subsystem updates (collision, animation, etc.)
  - `0x801414a0` — post-update handler
  - `0x801966a4` — unit update
  - `0x80253ca4` — parts update
  - `0x8002f878` — cleanup
- Handles battle start: when conditions met, sets state to 5, calls
  `0x8002e218` (seqMain) to initiate battle sequence
- Checks motion ID at offset 46 against thresholds (0x18=24, 0x21=33,
  0x14=20, 0x1a=26, 0x8=8, 0x19=25) to select the appropriate subsystem
  call chain
- Manages save-call via `0x800d3d30` (pouchAddHP) with conditional
  sequence-5 entry
- Final block copies current position (offsets 140/144/148) to save slots
  (272/276/280), increments counter at offset 52

## Critical offsets (Mario state structure)

| Offset | Size | Purpose |
|--------|------|---------|
| 0x00 | 4 | State flags (bit 25 = battle transition) |
| 0x04 | 4 | Motion control flags (bits 12-13, 14-15 = mode routing) |
| 0x0c | 4 | Sub-state flags (bit 12 = gravity override) |
| 0x1c | 4 | Attribute flags (bit 0 = moon room) |
| 0x2e | 2 | Motion ID (decides subsystem chain) |
| 0x34 | 4 | Frame counter |
| 0x38 | 1 | Mode byte (0=normal, 1/2=special) |
| 0x3c | 1 | Sub-mode byte |
| 0x4c | 2 | Stored position X (save slot) |
| 0x50 | 2 | Stored position Y |
| 0x54 | 2 | Stored position Z |
| 0x5e | 2 | Timer counter (decremented; expiry clears flags) |
| 0x8c | 4 | World position X |
| 0x90 | 4 | World position Y |
| 0x94 | 4 | World position Z |
| 0x110-0x118 | 12 | Save position XYZ |
| 0x174-0x17c | 12 | Gravity/physics values (set to 1.0 or 0.5) |
| 0x1e8 | 4 | Transition parameter |
| 0x1f8 | 4 | Sequence control (cleared on transition) |
| 0x20c | 4 | Previous transition parameter |
| 0x2d8 | 2 | Motion timer (decremented; modulo 30 = battle trigger) |
| 0x290 | 4 | Resource pointer (freed on delete) |
| 0x29c | 4 | Secondary resource pointer |
| 0x2b4 | 4 | Tertiary resource pointer |
| 0x314 | 4 | Parts list pointer |
| 0x318 | 4 | Parts update callback |
| 0x31c | 4 | Cleanup function pointer |
| 0x228 | 4 | Gravity value (written each frame) |
| 0x78 | 4 | Event timer |

## Called functions

| Address | Purpose |
|---------|---------|
| `0x80096a08` | State validation |
| `0x801a938c` | Primary motion dispatch |
| `0x800916f0` | Motion parameter update |
| `0x8026d464` | Per-frame subsystem update (6 calls) |
| `0x801414a0` | Post-update handler |
| `0x801966a4` | Unit update |
| `0x80253ca4` | Parts update |
| `0x8002f878` | Cleanup |
| `0x800d3d30` | pouchAddHP (save/battle entry) |
| `0x8002e218` | seqMain (battle transition) |
| `0x800d3300` | Item/resource lookup (3 calls) |
| `0x8005d740` | Post-save handler |
| `0x80057878` | State cleanup (called multiple times) |
| `0x801fec90` | Mode-2 handler |
| `0x801fc1d0` | Mode-1 handler |
| `0x80059a78` | Motion routing |
| `0x80095cb0` | Input processing |
| `0x80093d30` | Floor query |
| `0x8008c904` | Head/ceiling query |
| `0x8008f248` | Movement integration |
| `0x80096e8c` | Bottomless/void check |
| `0x800bc574` | Gravity handler |
| `0x801a924c` | Camera update |
| `0x800d3d30` | pouchAddHP (HP check) |
| `0x80093910` | Motion timer handler |
| `0x80094680` | State transition |
| `0x80095f64` | Final state update |
| `0x801a8ef4` | End-of-frame handler |
| `0x8008dd04` | Resource cleanup |
| `0x8008e694` | Floor search |
| `0x8008c904` | Overhead search |
| `0x8009455c` | Sequence entry |
| `0x8008f0e4` | Motion handler |
| `0x800a0b0c` | Input processing |
| `0x800a16b0` | State machine |
| `0x800a1684` | State update |

## Porting notes

This function is the single most important overworld execution path. For
porting, the key requirements are:

1. Preserve the call order: motion dispatch, subsystem updates, unit update,
   parts update, cleanup
2. The six `0x8026d464` calls at offsets 864-904 represent per-frame
   subsystem updates that must maintain their relative order
3. Battle transition via seqMain must preserve the original sequence
   numbering and parameter passing
4. Gravity setting (1.0 normal, 0.5 moon) must happen every frame
5. The motion ID routing (offset 46) determines which subsystem chain
   executes; each threshold value selects a different behavior path

## Remaining gaps

- Full semantic meaning of each motion ID and its associated subsystem chain
- Exact behavior of `0x801a938c` (primary motion dispatch)
- Meaning of the 6 subsystem update calls at offsets 864-904
- Exact battle transition conditions and parameter construction
- Full input processing chain
