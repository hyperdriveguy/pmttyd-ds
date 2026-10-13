# parse_format@printf.c

**Address:** `0x800813d4`  
**Size:** 1284 bytes (321 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Overview

`parse_format` is the printf format string parser. It processes format
specifiers character-by-character, extracting flags, width, precision,
length modifiers, and conversion specifiers. This is the core parsing engine
for all printf-family output.

## Key behavior summary

- Parses format string from current position
- Handles flags: `-`, `+`, space, `#`, `0`
- Extracts width (numeric or `*` for argument)
- Extracts precision (`.` followed by numeric or `*`)
- Handles length modifiers: `h`, `l`, `L`, `z`
- Identifies conversion specifiers: `d`, `i`, `u`, `x`, `X`, `o`, `f`, `e`,
  `g`, `c`, `s`, `p`, `n`
- Updates format state structure
- Returns updated position in format string

## Critical offsets (format state)

| Offset | Size | Purpose |
|--------|------|---------|
| +0x00 | 4 | Flags |
| +0x04 | 4 | Width |
| +0x08 | 4 | Precision |
| +0x0c | 4 | Length modifier |
| +0x10 | 4 | Conversion specifier |

## Porting notes

This function is critical for all formatted output. For porting:
1. Preserve format string parsing algorithm
2. Preserve all flag and specifier types
3. Preserve width and precision handling
4. Preserve length modifier support
5. Handle both numeric and `*` argument-based width/precision

## Remaining gaps

- Full flag combination edge cases
- Width and precision overflow handling
- Argument list coordination details
- Buffer overflow protection
- Locale and internationalization handling
