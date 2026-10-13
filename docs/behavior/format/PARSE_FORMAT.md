# parse_format

**Address:** `0x8026cdf4`  
**Size:** 1352 bytes (338 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Overview

`parse_format` is the format string parser for printf-style output. It
processes format specifiers, extracts conversion parameters, and manages
the formatting pipeline. This is the core parsing routine for all printf-
family functions.

## Key behavior summary

- Parses format string character-by-character
- Handles format specifiers: %d, %i, %u, %x, %X, %o, %f, %e, %g, %c, %s
- Manages width and precision specifiers
- Handles flags: -, +, space, #, 0
- Processes length modifiers: h, l, L, z
- Converts arguments to string representation
- Manages output buffer and position
- Handles padding and alignment

## Critical offsets (format state)

| Offset | Size | Purpose |
|--------|------|---------|
| +0x00 | 4 | Format string pointer |
| +0x04 | 4 | Current position |
| +0x08 | 4 | Output buffer pointer |
| +0x0c | 4 | Buffer position |
| +0x10 | 4 | Argument list pointer |
| +0x14 | 4 | Flags |
| +0x18 | 4 | Width |
| +0x1c | 4 | Precision |

## Called functions

| Address | Purpose |
|---------|---------|
| `0x80036604` | Character classification |
| `0x8002e218` | Sequence/parse helper |
| `0x80094020` | Precision handling |

## Porting notes

This function is critical for all formatted output. For porting:
1. Preserve format string parsing algorithm
2. Preserve all format specifier types and flags
3. Preserve width and precision handling
4. Preserve argument conversion pipeline
5. Preserve output buffer management
6. Preserve padding and alignment behavior

## Remaining gaps

- Full format specifier type definitions
- Width and precision edge cases
- Argument conversion algorithms
- Buffer overflow protection
- Locale and internationalization handling
- Floating point formatting details
