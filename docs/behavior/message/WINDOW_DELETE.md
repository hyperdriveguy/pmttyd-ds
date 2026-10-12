# windowDelete

**Address:** `0x8007bac0`  
**Size:** 76 bytes (19 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
8007bac0: 94 21 ff f0  	stwu 1, -16(1)
8007bac4: 7c 08 02 a6  	mflr 0
8007bac8: 90 01 00 14  	stw 0, 20(1)
8007facc: 93 e1 00 0c  	stw 31, 12(1)
8007fad0: 7c 7f 1b 78  	mr 31, 3
8007fad4: 81 83 00 40  	lwz 12, 64(3)
8007fad8: 28 0c 00 00  	cmplwi 12, 0
8007fadc: 41 82 00 0c  	bt 2, 0x8007fae8
8007fae0: 7d 89 03 a6  	mtctr 12
8007fae4: 4e 80 04 21  	bctrl
8007fae8: 38 00 00 00  	li 0, 0
8007faec: 38 60 00 01  	li 3, 1
8007faf0: b0 1f 00 02  	sth 0, 2(31)
8007faf4: 90 1f 00 28  	stw 0, 40(31)
8007faf8: 83 e1 00 0c  	lwz 31, 12(1)
8007fafc: 80 01 00 14  	lwz 0, 20(1)
8007fb00: 7c 08 03 a6  	mtlr 0
8007fb04: 38 21 00 10  	addi 1, 1, 16
8007fb08: 4e 80 00 20  	blr
```

## Behavior

`windowDelete` calls the window's delete callback (if present), then
clears the window's state and data fields. Returns 1 to indicate success.

| Item | Value |
| --- | --- |
| Argument | `r3` = pointer to window record |
| Callback | `[r3 + 64]` (delete callback, called if nonzero) |
| Side effects | State (offset 2) cleared, data (offset 40) cleared |
| Return | `r3` = 1 (success) |
| Calls | Indirect callback at `[r3 + 64]` |

## Interpretation

This function deletes a window by first invoking its type-specific delete
cleanup callback, then resetting the record's state and data fields. The
return value of 1 indicates the deletion was successful. The callback
at offset 64 is a function pointer stored in the window record.

## Remaining gaps

- Full layout of the 72-byte window record structure
- Type-specific delete callbacks
- Caller graph
