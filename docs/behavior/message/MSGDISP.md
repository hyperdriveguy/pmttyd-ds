# msgDisp

**Address:** `0x80050b88`  
**Size:** 2900 bytes (725 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Overview

`msgDisp` is the primary message display function. It processes message text,
handles control tokens, renders glyphs, manages window state, and coordinates
with the animation and font systems. This is the central execution path for all
dialogue and text presentation.

## Key behavior summary

- Called with message ID, window structure, and display parameters
- Reads message text from message table (indexed by message ID)
- Processes text character-by-character through a state machine
- Handles control tokens (embedded commands within message text)
- Coordinates with font rendering system for glyph display
- Manages window geometry, positioning, and animation
- Handles speaker name display and animation
- Manages text scroll timing and advancement

## Critical offsets

| Offset | Size | Purpose |
|--------|------|---------|
| +0x00 | 4 | Window state flags |
| +0x04 | 4 | Current message pointer |
| +0x08 | 4 | Display parameters |
| +0x10 | 4 | Animation state |

## Called functions

| Address | Purpose |
|---------|---------|
| `0x80093d30` | Glyph rendering |
| `0x8008c904` | Window update |
| `0x8005de4` | Text processing |
| `0x8009455c` | Scroll timing |
| `0x8008f0e4` | Input handling |
| `0x80094020` | Animation integration |
| `0x801a8ef4` | End-of-display handler |

## Porting notes

This function is critical for faithful dialogue presentation. For porting:
1. Preserve message text extraction from msg/US files
2. Preserve control token parsing and execution
3. Preserve glyph rendering pipeline and font coordination
4. Preserve window geometry and animation timing
5. Preserve speaker name display and animation
6. Preserve text scroll timing and input handling

## Remaining gaps

- Full message ID table and text extraction
- Control token type definitions and semantics
- Font rendering pipeline details
- Window animation curves and timing
- Speaker animation state machine
- Text scroll rate and boundary conditions
- Multi-language and character encoding handling
