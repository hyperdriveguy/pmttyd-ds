# BtlUnit_GetHomePos

**Address:** `0x801289b0`  
**Size:** 28 bytes (7 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
801289b0: c0 03 00 30  	lfs 0, 48(3)
801289b4: d0 04 00 00  	stfs 0, 0(4)
801289b8: c0 03 00 34  	lfs 0, 52(3)
801289bc: d0 05 00 00  	stfs 0, 0(5)
801289c0: c0 03 00 38  	lfs 0, 56(3)
801289c4: d0 06 00 00  	stfs 0, 0(6)
801289c8: 4e 80 00 20  	blr
```

## Behavior

`BtlUnit_GetHomePos` copies the home position (XYZ floats) from offset
48 of the unit structure to three output pointers.

| Item | Value |
| --- | --- |
| Argument | `r3` = pointer to a battle unit structure |
| Output | `f1` = `*(float*)r4` (X coordinate) |
| Output | `f2` = `*(float*)r5` (Y coordinate) |
| Output | `f3` = `*(float*)r6` (Z coordinate) |
| Source | `[r3 + 48]`, `[r3 + 52]`, `[r3 + 56]` |
| Side effects | None |
| Calls | None |

## Interpretation

This function retrieves the home position of a battle unit. The home
position is the unit's default or spawn position in the battle stage.
The three output pointers allow the caller to receive the coordinates
in separate variables.

## Remaining gaps

- Full layout of the battle unit structure
- Caller graph
- Relationship to unit spawn and positioning
