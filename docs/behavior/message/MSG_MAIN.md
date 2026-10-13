# msgMain

**Address:** `0x8008460c`  
**Size:** 1832 bytes (458 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Overview

`msgMain` is the main message system update function. It coordinates message
window lifecycle, text processing, display updates, and input handling. This
is the central execution path for all dialogue and text presentation.

## Key behavior summary

- Manages message window state machine
- Coordinates text processing and display
- Handles input (advance, cancel, navigate)
- Manages window geometry and positioning
- Coordinates with animation and font systems
- Handles message queue and priority
- Manages text scroll and advancement timing

## Critical offsets (message state)

| Offset | Size | Purpose |
|--------|------|---------|
| +0x00 | 4 | Message state flags |
| +0x04 | 4 | Current message pointer |
| +0x08 | 4 | Display parameters |
| +0x0c | 4 | Animation state |
| +0x10 | 4 | Window structure pointer |

## Called functions

| Address | Purpose |
|---------|---------|
| `0x800d9848` | Message text processing |
| `0x80094020` | State integration |
| `0x8009455c` | Display update |
| `0x8008f0e4` | Input handling |
| `0x8009412c` | RNG processing |

## Porting notes

This function is critical for dialogue presentation. For porting:
1. Preserve message state machine and transitions
2. Preserve text processing coordination
3. Preserve input handling (advance, cancel, navigate)
4. Preserve window geometry and positioning
5. Preserve animation and font system coordination
6. Preserve message queue and priority management

## Remaining gaps

- Full message state machine with all transitions
- Text processing coordination details
- Input handling timing and edge cases
- Window geometry and positioning rules
- Animation and font system integration
- Message queue and priority management
