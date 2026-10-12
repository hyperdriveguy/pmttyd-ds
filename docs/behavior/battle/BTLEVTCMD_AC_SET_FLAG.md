# btlevtcmd_AcSetFlag

**Address:** `0x80107ad4`  
**Size:** 24 bytes (6 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
80107ad4: 80 a3 00 18  	lwz 5, 24(3)
80107ad8: 38 60 00 02  	li 3, 2
80107adc: 80 8d 1c 70  	lwz 4, 7280(13)
80107ae0: 80 05 00 00  	lwz 0, 0(5)
80107ae4: 90 04 1c c4  	stw 0, 7364(4)
80107ae8: 4e 80 00 20  	blr
```

## Behavior

`btlevtcmd_AcSetFlag` loads a pointer from offset 24 of the argument,
dereferences it to get a value, and stores that value at offset 7364
(`0x1cc4`) of the battle work structure. Returns 2.

| Item | Value |
| --- | --- |
| Argument | `r3` = pointer to a structure with a pointer at offset 24 |
| Load | `r5` = `[r3 + 24]`, then `r0` = `[r5]` |
| Side effect | `[battleWork + 7364] = r0` |
| Return | `r3` = 2 |
| Calls | None |

## Interpretation

This function copies a flag value from an event argument to the battle
work structure. The value is loaded through a double indirection: first
a pointer is read from offset 24 of the argument, then the value at
that pointer is stored in the battle work's AC flag field. The return
value of 2 indicates the command is complete (not blocked).

## Remaining gaps

- Full layout of the battle work structure
- Meaning of the flag at offset 7364
- Caller graph
- Relationship to the event command system
