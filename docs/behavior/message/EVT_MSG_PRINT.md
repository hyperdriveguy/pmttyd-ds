# _evt_msg_print

**Address:** `0x800d284c`  
**Size:** 136 bytes (34 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Overview

`_evt_msg_print` is the event command that displays a message in a dialogue
window. It resolves four arguments (message ID, speaker, animation, and
window type) and calls the message display function at `0x800d28d4`.

## Key behavior summary

- Resolves four arguments from event argument structure (at offset 24):
  - `[arg+0]` — message ID
  - `[arg+4]` — speaker
  - `[arg+8]` — animation
  - `[arg+12]` — window type
- Calls `0x800d28d4` (message display function) with resolved values
- Returns 0 (yield) to allow message display to complete

## Critical offsets

| Offset | Size | Purpose |
|--------|------|---------|
| +0x18 | 4 | Argument structure pointer |
| +0x00 | 4 | Message ID |
| +0x04 | 4 | Speaker |
| +0x08 | 4 | Animation |
| +0x0c | 4 | Window type |

## Called functions

| Address | Purpose |
|---------|---------|
| `0x80031260` | Resolve argument values (4 calls) |
| `0x800d28d4` | Message display function |

## Porting notes

This function is critical for event-driven dialogue. For porting:
1. Preserve four-argument resolution order
2. Preserve message display function call
3. Handle message ID, speaker, animation, and window type appropriately
4. Preserve return code semantics (0 = yield)

## Remaining gaps

- Full message ID table and text extraction
- Speaker name display format
- Animation and window type mappings
- Token processing and control code interpretation
