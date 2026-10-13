# msgWindow_Disp

**Address:** `0x800808ec`  
**Size:** 1756 bytes (439 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Overview

`msgWindow_Disp` is the message window display function. It renders glyphs,
handles text layout, manages window coordinates, and produces the final
visual output for dialogue windows. This is the rendering execution path
for all text presentation.

## Key behavior summary

- Renders glyphs from font data
- Handles text layout and positioning
- Manages window coordinates and clipping
- Handles text color and styling
- Coordinates with font rendering system
- Manages glyph cache and batching
- Handles special characters and tokens

## Critical offsets (window display structure)

| Offset | Size | Purpose |
|--------|------|---------|
| +0x00 | 4 | Window state flags |
| +0x04 | 4 | Glyph buffer pointer |
| +0x08 | 4 | Font pointer |
| +0x0c | 4 | Layout parameters |

## Called functions

| Address | Purpose |
|---------|---------|
| `0x8003c450` | Glyph rendering |
| `0x80094020` | State integration |
| `0x8009455c` | Display update |

## Porting notes

This function is critical for text rendering. For porting:
1. Preserve glyph rendering pipeline
2. Preserve text layout and positioning
3. Preserve window coordinates and clipping
4. Preserve text color and styling
5. Preserve font rendering system coordination
6. Preserve glyph cache and batching

## Remaining gaps

- Full glyph rendering pipeline details
- Text layout and positioning algorithm
- Window coordinates and clipping rules
- Text color and styling system
- Font rendering system integration
- Glyph cache and batching strategy
