# __minus_dec

**Address:** `0x800d2d60`  
**Size:** 1276 bytes (319 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Overview

`__minus_dec` performs decimal record subtraction. It computes the difference
between two decimal number representations, handling sign, borrow, and
normalization. This is the core decimal arithmetic subtraction routine.

## Key behavior summary

- Receives two decimal record pointers (minuend and subtrahend)
- Compares magnitudes to determine result sign
- Performs digit-by-digit subtraction with borrow propagation
- Handles sign determination and result normalization
- Manages leading zero suppression
- Returns result in decimal record format

## Critical offsets (decimal record structure)

| Offset | Size | Purpose |
|--------|------|---------|
| +0x00 | 4 | Sign flag |
| +0x04 | 4 | Exponent |
| +0x08 | 4 | Digit count |
| +0x0c | 4 | Digit array pointer |

## Called functions

| Address | Purpose |
|---------|---------|
| `0x8007ad50` | Digit classification |
| `0x80094020` | Precision handling |

## Porting notes

This function is critical for decimal arithmetic. For porting:
1. Preserve decimal record format and layout
2. Preserve digit-by-digit subtraction algorithm
3. Preserve borrow propagation logic
4. Preserve sign determination and normalization
5. Preserve leading zero suppression

## Remaining gaps

- Full decimal record structure layout
- Digit array format and encoding
- Borrow propagation edge cases
- Sign determination rules
- Normalization algorithm details
