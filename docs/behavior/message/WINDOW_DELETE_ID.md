# windowDeleteID

**Address:** `0x8007fa54`  
**Size:** 108 bytes (27 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
8007fa54: 94 21 ff f0  	stwu 1, -16(1)
8007fa58: 7c 08 02 a6  	mflr 0
8007fa5c: 90 01 00 14  	stw 0, 20(1)
8007fa60: 1c 03 00 48  	mulli 0, 3, 72
8007fa64: 93 e1 00 0c  	stw 31, 12(1)
8007fa68: 80 6d 1b 08  	lwz 3, 6920(13)
8007fa6c: 7f e3 02 14  	add 31, 3, 0
8007fa70: a0 1f 00 02  	lhz 0, 2(31)
8007fa74: 54 00 07 ff  	clrlwi. 0, 0, 31
8007fa78: 40 82 00 0c  	bf 2, 0x8007fa84
8007fa7c: 38 60 00 00  	li 3, 0
8007fa80: 48 00 00 2c  	b 0x8007faac
8007fa84: 81 9f 00 40  	lwz 12, 64(31)
8007fa88: 28 0c 00 00  	cmplwi 12, 0
8007fa8c: 41 82 00 10  	bt 2, 0x8007fa9c
8007fa90: 7f e3 fb 78  	mr 3, 31
8007fa94: 7d 89 03 a6  	mtctr 12
8007fa98: 4e 80 04 21  	bctrl
8007fa9c: 38 00 00 00  	li 0, 0
8007faa0: 38 60 00 01  	li 3, 1
8007faa4: b0 1f 00 02  	sth 0, 2(31)
8007faa8: 90 1f 00 28  	stw 0, 40(31)
8007faac: 80 01 00 14  	lwz 0, 20(1)
8007fab0: 83 e1 00 0c  	lwz 31, 12(1)
8007fab4: 7c 08 03 a6  	mtlr 0
8007fab8: 38 21 00 10  	addi 1, 1, 16
8007fabc: 4e 80 00 20  	blr
```

## Behavior

`windowDeleteID` deletes a window by its index. It looks up the window
record, calls the delete callback (if present), clears the state and
data fields, and returns 1 on success or 0 if the window is not active.

| Item | Value |
| --- | --- |
| Argument | `r3` = window index |
| Active check | Halfword at offset 2, tested for nonzero |
| Delete callback | `[record + 64]` (called if active and nonzero) |
| Side effects | State (offset 2) cleared, data (offset 40) cleared |
| Return | `r3` = 1 (success) or 0 (not active) |
| Calls | Indirect callback at `[record + 64]` |

## Interpretation

This function provides indexed window deletion. It first checks if the
window is active, then calls the type-specific delete callback, and
finally clears the record. The return value indicates whether the
deletion was performed.

## Remaining gaps

- Full layout of the 72-byte window record structure
- Type-specific delete callbacks
- Caller graph
