# __ieee754_pow

**Address:** `0x8027067c`  
**Size:** 2096 bytes (524 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Overview

`__ieee754_pow` is the IEEE 754 compliant power function (x^y). It computes
the result of raising a base to an exponent with proper handling of special
cases, rounding, and error conditions. This is the standard library power
function used throughout the game.

## Key behavior summary

- Computes x^y for binary64 (double-precision) inputs
- Handles special cases: 0^0, 0^negative, 1^anything, negative^integer
- Handles infinity and NaN inputs
- Manages overflow and underflow conditions
- Performs correct rounding per IEEE 754
- Returns result with proper sign and exponent

## Critical offsets

| Offset | Size | Purpose |
|--------|------|---------|
| +0x00 | 8 | Base (x) |
| +0x08 | 8 | Exponent (y) |
| +0x10 | 8 | Result (output) |

## Called functions

| Address | Purpose |
|---------|---------|
| `0x80036604` | Special case classification |
| `0x8002e218` | Computation helper |
| `0x80094020` | Precision handling |

## Porting notes

This function is critical for all power computations. For porting:
1. Preserve IEEE 754 compliance
2. Preserve special case handling
3. Preserve overflow/underflow behavior
4. Preserve correct rounding
5. Handle all edge cases (0, 1, inf, nan)

## Remaining gaps

- Full special case handling matrix
- Overflow/underflow threshold values
- Rounding mode details
- Precision and accuracy guarantees
- Performance optimization opportunities
