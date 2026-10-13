# evt_bero_mapchange

**Address:** `0x8007aad0`  
**Size:** 176 bytes (44 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Overview

`evt_bero_mapchange` is the event command that initiates a room/map transition.
It resolves two arguments and calls the sequence change function.

## Key behavior summary

- Resolves two arguments from event argument structure (at offset 24):
  - `[arg+0]` — target map/entrance ID
  - `[arg+4]` — secondary parameter
- If transition type argument (r4) is nonzero, calls `seqMain` with the
  resolved values to initiate the sequence change
- Returns 0 (yield) to allow the transition to complete

## Critical offsets

| Offset | Size | Purpose |
|--------|------|---------|
| +0x18 | 4 | Argument structure pointer |
| +0x00 | 4 | Target map/entrance ID |
| +0x04 | 4 | Secondary parameter |

## Called functions

| Address | Purpose |
|---------|---------|
| `0x80031260` | Resolve argument values (2 calls) |
| `0x8002e218` | seqMain (sequence change) |

## Porting notes

This function is critical for room transitions. For porting:
1. Preserve argument resolution order (map ID first, entrance ID second)
2. Preserve the nonzero transition type gate
3. Preserve sequence change parameter passing
4. Handle map/entrance ID mappings appropriately

## Remaining gaps

- Full sequence table and map/entrance ID mappings
- Relationship to fade system (fadeEntry called from seqMain)
- How the secondary parameter affects the transition
- Door/pipe vs road transition differentiation
