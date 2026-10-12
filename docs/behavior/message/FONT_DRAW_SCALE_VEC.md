# FontDrawScaleVec

**Address:** `0x8007773c`  
**Size:** 28 bytes (7 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
8007773c: 80 a3 00 00  	lwz 5, 0(3)
80077740: 80 83 00 04  	lwz 4, 4(3)
80077744: 80 03 00 08  	lwz 0, 8(3)
80077748: 90 ad 1a 90  	stw 5, 6800(13)
8007774c: 90 8d 1a 94  	stw 4, 6804(13)
80077750: 90 0d 1a 98  	stw 0, 6808(13)
80077754: 4e 80 00 20  	blr
```

## Behavior

`FontDrawScaleVec` copies three 32-bit words from the argument structure
to three consecutive SDA global variables.

| Item | Value |
| --- | --- |
| Argument | `r3` = pointer to a 12-byte structure (three words) |
| Side effect 1 | `[r13 + 0x1a90] = [r3 + 0]` (X scale) |
| Side effect 2 | `[r13 + 0x1a94] = [r3 + 4]` (Y scale) |
| Side effect 3 | `[r13 + 0x1a98] = [r3 + 8]` (Z scale) |
| Return | None (void) |
| Calls | None |

## Interpretation

This function sets the font draw scale vector from a supplied 3D scale
structure. The three values are stored as global variables in SDA, making
them available to the font rendering system. The scale controls how text
glyphs are stretched or compressed along each axis.

## Remaining gaps

- Full layout of the font draw state
- Caller graph
- Relationship to `FontDrawScale` (single-value variant)
