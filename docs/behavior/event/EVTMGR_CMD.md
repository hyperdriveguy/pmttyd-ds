# evtmgrCmd

**Address:** `0x800316b8`  
**Size:** 5048 bytes (1262 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Overview

`evtmgrCmd` is the event VM command interpreter. It decodes big-endian 32-bit
command words (opcode in low half, argument count in high half), dispatches to
119 opcode handlers via a table at `0x80304fd0`, and manages handler results
and return codes. This is the central execution engine for all event scripts.

## Key behavior summary

- Reads command word from `gp` (event work pointer); opcode and argument
  count stored in byte fields
- Advances to next command by `4 + 4*count`; clears blocked marker on fetch
- Dispatches via jump table at `0x80304fd0` (119 entries)
- Handler result protocol:
  | Result | Effect |
  |--------|--------|
  | 0 | Blocked; return 0 |
  | 1 | Yield; advance to next; return 0 |
  | 2 | Continue; advance; invoke next in same call |
  | 3 | Redispatch immediately |
  | 255 | Return -1 to event manager (end event) |
  | Negative | Return 1 to event manager (stop manager pass) |

- Opcode 2 (DELETE): invokes event deletion then returns 255
- Opcode 1: reaches default status path
- Opcode 9 (WAIT_FRAME): first evaluation stores counter and marks initialized;
  zero completes immediately; nonzero decrements, blocks, then yields
- Opcode 10: calls `evt_wait_msec` at `0x80035884`; records duration and
  lifetime; converts to integer milliseconds; yields on completion
- Opcode 91 (USER_FUNC): resolves and caches callback on first call, removes
  first argument, sets blocked marker, invokes `(event, 1)`; resumption
  invokes `(event, 0)` without reinitializing

## Critical offsets (event work structure)

| Offset | Size | Purpose |
|--------|------|---------|
| +0x0a | 1 | Blocked marker |
| +0x00 | 4 | Current command pointer |

## Dispatch table

Located at `0x80304fd0`; 119 entries covering opcodes 0-118. Each entry
is a 4-byte function address.

## Porting notes

This function is the single most critical event execution path. For porting:
1. Preserve the big-endian command word format (opcode low, count high)
2. Preserve the 119-entry dispatch table and all handler addresses
3. Preserve the result protocol (0/1/2/3/255/negative)
4. USER_FUNC caching must persist per-command (not per-invocation)
5. WAIT_FRAME and wait_msec semantics must be preserved with source's
   millisecond-to-tick conversion

## Remaining gaps

- Full implementation of all 119 opcode handlers
- Numeric edge cases (64-bit timer wrap, negative waits)
- Memory management for cached USER_FUNC state
- Interaction with child/brother event lifecycle
- Pause and system-level control integration
