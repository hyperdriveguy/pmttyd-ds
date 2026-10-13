# __dec2num

**Address:** `0x80266834`  
**Size:** 4224 bytes (1056 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Overview

`__dec2num` is the decimal string to binary64 (double-precision float)
conversion function. It parses decimal text representation and produces
the corresponding IEEE 754 double-precision value. This is the core
numeric parsing routine used by scanf and other input functions.

## Key behavior summary

- Parses decimal text: optional sign, integer part, fractional part,
  optional exponent
- Handles special values: "inf", "infinity", "nan" (case-insensitive)
- Produces IEEE 754 binary64 result
- Manages precision and rounding
- Handles overflow/underflow conditions
- Returns status code indicating success/failure

## Critical offsets (work structure)

| Offset | Size | Purpose |
|--------|------|---------|
| +0x00 | 4 | Input string pointer |
| +0x04 | 4 | Current parse position |
| +0x08 | 4 | Integer part accumulator |
| +0x0c | 4 | Fraction part accumulator |
| +0x10 | 4 | Exponent value |
| +0x14 | 4 | Sign flag |
| +0x18 | 4 | Status/result code |

## Called functions

| Address | Purpose |
|---------|---------|
| `0x80036664` | Digit classification |
| `0x8002e218` | Sequence/parse helper |
| `0x80094020` | Precision handling |

## Porting notes

This function is critical for numeric input parsing. For porting:
1. Preserve decimal text parsing algorithm
2. Preserve special value handling (inf, nan)
3. Preserve IEEE 754 binary64 output format
4. Preserve overflow/underflow behavior
5. Preserve status code return values
6. Handle both positive and negative values
7. Handle scientific notation (e.g., "1.23e-4")

## Remaining gaps

- Full precision and rounding rules
- Overflow/underflow threshold values
- Special value string matching details
- Status code definitions and meanings
- Locale and internationalization handling
- Error recovery and partial parse behavior
