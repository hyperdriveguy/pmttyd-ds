# msgAnalize

**Address:** `0x80081de0`  
**Size:** 5796 bytes (1449 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Overview

`msgAnalize` is the message text analyzer and layout engine. It processes
message text character-by-character, handling control tokens, computing glyph
advances, managing line breaks, and producing the final display layout. This
is the core text processing function for all dialogue and UI text.

## Key behavior summary

- Processes message text from a buffer, character by character
- Handles control tokens (embedded commands within message text)
- Computes glyph widths and advances using font data
- Manages line wrapping and word boundaries
- Handles special characters: newlines, tabs, spaces
- Coordinates with font system for glyph metrics
- Produces layout records for the display system
- Handles variable-width and fixed-width fonts
- Manages text alignment and justification

## Critical offsets

| Offset | Size | Purpose |
|--------|------|---------|
| +0x00 | 4 | Message buffer pointer |
| +0x04 | 4 | Current position |
| +0x08 | 4 | Line start position |
| +0x0c | 4 | Line width accumulator |
| +0x10 | 4 | Max line width |
| +0x14 | 4 | Font pointer |
| +0x18 | 4 | Layout output pointer |

## Called functions

| Address | Purpose |
|---------|---------|
| `0x80010bc4` | Glyph width lookup |
| `0x8008e694` | Font metrics query |
| `0x80093d30` | Glyph rendering prep |
| `0x8008c904` | Layout record creation |

## Porting notes

This function is critical for faithful text presentation. For porting:
1. Preserve control token parsing and execution
2. Preserve glyph width computation and font coordination
3. Preserve line wrapping and word boundary handling
4. Preserve layout record format for display system
5. Handle both variable-width and fixed-width fonts
6. Preserve text alignment and justification behavior

## Remaining gaps

- Full control token type definitions and semantics
- Font data structure layout
- Layout record format details
- Line wrapping algorithm specifics
- Text alignment and justification rules
- Multi-language and character encoding handling
