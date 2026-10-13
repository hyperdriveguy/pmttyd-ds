# float2str

**Address:** `0x8026b294`  
**Size:** 1820 bytes (455 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Overview

`float2str` converts a binary64 (double-precision) floating point value to
its decimal string representation. It handles sign, integer part, fractional
part, and exponent formatting. This is the core float-to-string conversion
routine.

## Key behavior summary

- Extracts sign, exponent, and mantissa from binary64 input
- Handles special values: zero, infinity, NaN
- Converts integer part to decimal digits
- Converts fractional part to decimal digits
- Handles scientific notation for large/small values
- Manages precision and rounding
- Produces null-terminated string output

## Critical offsets

| Offset | Size | Purpose |
|--------|------|---------|
| +0x00 | 8 | Input float value |
| +0x08 | 4 | Output buffer pointer |
| +0x0c | 4 | Precision |
| +0x10 | 4 | Flags |

## Called functions

| Address | Purpose |
|---------|---------|
| `0x80036604` | Digit classification |
| `0x8002e218` | Conversion helper |
| `0x80094020` | Precision handling |

## Porting notes

This function is critical for all float-to-string conversions. For porting:
1. Preserve binary64 format parsing
2. Preserve special value handling (zero, inf, nan)
3. Preserve integer and fractional part conversion
4. Preserve scientific notation formatting
5. Preserve precision and rounding behavior

## Remaining gaps

- Full precision and rounding rules
- Scientific notation threshold values
- Special value string representations
- Buffer overflow protection
- Locale and internationalization handling
