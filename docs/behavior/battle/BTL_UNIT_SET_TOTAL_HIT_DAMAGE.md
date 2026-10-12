# BtlUnit_SetTotalHitDamage

**Address:** `0x80127434`  
**Size:** 12 bytes (3 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
80127434: 7c 80 07 34  	extsh 0, 4
80127438: 90 03 02 64  	stw 0, 612(3)
8012743c: 4e 80 00 20  	blr
```

## Behavior

`BtlUnit_SetTotalHitDamage` is a trivial field setter with sign extension.
It sign-extends the 16-bit value in register 4 to 32 bits and stores it
at byte offset 612 (`0x264`) of the pointer in register 3.

| Item | Value |
| --- | --- |
| Argument | `r3` = pointer to a battle unit structure |
| Argument | `r4` = 16-bit signed value (sign-extended to 32-bit) |
| Side effect | `[r3 + 612] = sign_extend(r4)` (32-bit word store) |
| Return | None (void) |
| Calls | None |

## Interpretation

This function sets the **total hit damage** field of a battle unit. The
sign extension allows negative damage values (e.g., for healing or
damage reduction). The field at offset `0x264` tracks the cumulative
damage dealt by this unit during the current turn or battle.

## Remaining gaps

- Full structure layout of the battle unit record
- Caller graph
- Relationship to damage calculation and application
