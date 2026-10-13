# _JUTFont_DrawPos

**Address:** `0x80074bac`  
**Size:** 1244 bytes (311 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Overview

`_JUTFont_DrawPos` renders text at a specified position using the JUT font
system. It handles glyph lookup, positioning, scaling, and coordinate
transformation. This is the low-level text rendering function for the
game's UI system.

## Key behavior summary

- Receives text string, position coordinates, and font data
- Transforms coordinates through current matrix
- Looks up glyphs in font data for each character
- Handles glyph advance and kerning
- Applies scaling and positioning
- Renders through GX (graphics) system
- Manages glyph cache for performance

## Critical offsets

| Offset | Size | Purpose |
|--------|------|---------|
| +0x00 | 4 | Text string pointer |
| +0x04 | 4 | X position |
| +0x08 | 4 | Y position |
| +0x0c | 4 | Z position |
| +0x10 | 4 | Font data pointer |

## Called functions

| Address | Purpose |
|---------|---------|
| `0x80036604` | Glyph classification |
| `0x8002e218` | Coordinate transformation |
| `0x80094020` | State integration |

## Porting notes

This function is critical for UI text rendering. For porting:
1. Preserve coordinate transformation pipeline
2. Preserve glyph lookup and rendering
3. Preserve font data structure access
4. Preserve scaling and positioning
5. Handle GX rendering calls appropriately

## Remaining gaps

- Full font data structure layout
- Glyph cache management details
- Coordinate transformation matrix format
- GX rendering call parameters
- Text alignment and justification rules
