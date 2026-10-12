# BtlUnit_SetFallAccel

**Address:** `0x8012816c`  
**Size:** 8 bytes (2 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
8012816c: d0 23 01 74  	stfs 1, 372(3)
80128170: 4e 80 00 20  	blr
```

## Behavior

`BtlUnit_SetFallAccel` is a trivial float field setter. It stores the
single-precision floating-point value in register 1 to byte offset 372
(`0x174`) of the pointer in register 3, then returns.

| Item | Value |
| --- | --- |
| Argument | `r3` = pointer to a battle unit structure |
| Argument | `f1` = float value to store (the fall acceleration) |
| Side effect | `[r3 + 372] = f1` (32-bit float store) |
| Return | None (void) |
| Calls | None |

## Interpretation

This function sets the **fall acceleration** field of a battle unit. In the
battle system, units can be launched into the air (e.g., by a Jump attack)
and then fall back down. The fall acceleration controls how quickly the unit
descends. This is a per-unit physics parameter that may vary depending on
the unit type or status effects.

## Remaining gaps

- Full structure layout of the battle unit record
- Caller graph (who sets fall acceleration and when)
- Relationship to the battle physics system
- Default values and how they vary by unit type
