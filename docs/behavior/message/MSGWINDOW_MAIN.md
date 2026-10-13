# msgWindow_Main

**Address:** `0x8011bb38`  
**Size:** 2052 bytes (513 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Overview

`msgWindow_Main` is the main message window update function. It manages
window state, handles input, coordinates text display, and manages window
lifecycle. This is the central execution path for all dialogue windows.

## Key behavior summary

- Manages window state machine (opening, active, closing)
- Handles input (advance, cancel, navigate)
- Coordinates text display and scroll
- Manages window geometry and positioning
- Handles speaker name display
- Manages window animation and transitions
- Coordinates with message system

## Critical offsets (window structure)

| Offset | Size | Purpose |
|--------|------|---------|
| +0x00 | 4 | Window state flags |
| +0x04 | 4 | Current position |
| +0x08 | 4 | Display parameters |
| +0x0c | 4 | Animation state |

## Called functions

| Address | Purpose |
|---------|---------|
| `0x80036604` | State classification |
| `0x8002e218` | Sequence helper |
| `0x80094020` | Input handling |
| `0x8009455c` | Display update |

## Porting notes

This function is critical for dialogue presentation. For porting:
1. Preserve window state machine
2. Preserve input handling (advance, cancel, navigate)
3. Preserve text display and scroll coordination
4. Preserve window geometry and positioning
5. Preserve speaker name display
6. Preserve animation and transition handling

## Remaining gaps

- Full window state machine with all transitions
- Input handling details and timing
- Text display and scroll algorithm
- Window geometry and positioning rules
- Speaker name display format
- Animation curves and timing
