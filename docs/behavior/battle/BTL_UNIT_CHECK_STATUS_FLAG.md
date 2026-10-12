# BtlUnit_CheckStatusFlag

**Address:** `0x801277f8`  
**Size:** 28 bytes (7 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
801277f8: 28 03 00 00  	cmplwi 3, 0
801277fc: 41 82 00 10  	bt 2, 0x8012780c
80127800: 80 03 01 38  	lwz 0, 312(3)
80127804: 7c 03 20 38  	and 3, 0, 4
80127808: 4e 80 00 20  	blr
8012780c: 38 60 00 00  	li 3, 0
80127810: 4e 80 00 20  	blr
```

## Behavior

`BtlUnit_CheckStatusFlag` checks whether a battle unit has a specific
status flag set. Returns nonzero if the flag is set, zero otherwise.

| Item | Value |
| --- | --- |
| Argument | `r3` = pointer to a battle unit structure (may be null) |
| Null check | Returns 0 if `r3 == 0` |
| Return | `r3` = `[r3 + 312] & 0x04` (nonzero if bit 2 is set) |
| Side effects | None |
| Calls | None |

## Interpretation

This function checks bit 2 (mask `0x04`) of the unit's status flags field
at offset 312 (`0x138`). The null check provides safety for cases where
a unit pointer may not be valid. The specific status represented by bit
2 depends on the status flag definitions.

## Remaining gaps

- Full layout of the battle unit structure
- Meaning of bit 2 at offset 312
- Caller graph
- Relationship to other status flag checks
