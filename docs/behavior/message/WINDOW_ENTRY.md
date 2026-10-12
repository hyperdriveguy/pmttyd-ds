# windowEntry

**Address:** `0x8007fb0c`  
**Size:** 136 bytes (34 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
8007fb0c: 94 21 ff e0  	stwu 1, -32(1)
8007fb10: 7c 08 02 a6  	mflr 0
8007fb14: 90 01 00 24  	stw 0, 36(1)
8007fb18: 38 00 00 07  	li 0, 7
8007fb1c: 93 e1 00 1c  	stw 31, 28(1)
8007fb20: 93 c1 00 18  	stw 30, 24(1)
8007fb24: 3b c0 00 00  	li 30, 0
8007fb28: 93 a1 00 14  	stw 29, 20(1)
8007fb2c: 7c 7d 1b 78  	mr 29, 3
8007fb30: 83 ed 1b 08  	lwz 31, 6920(13)
8007fb34: 7c 09 03 a6  	mtctr 0
8007fb38: a0 1f 00 02  	lhz 0, 2(31)
8007fb3c: 54 00 07 ff  	clrlwi. 0, 0, 31
8007fb40: 40 82 00 28  	bf 2, 0x8007fb68
8007fb44: 7f e3 fb 78  	mr 3, 31
8007fb48: 38 80 00 00  	li 4, 0
8007fb4c: 38 a0 00 48  	li 5, 72
8007fb50: 4b f8 55 65  	bl 0x800050b4
8007fb54: 38 00 00 01  	li 0, 1
8007fb58: 7f c3 f3 78  	mr 3, 30
8007fb5c: b0 1f 00 02  	sth 0, 2(31)
8007fb60: b3 bf 00 04  	sth 29, 4(31)
8007fb64: 48 00 00 14  	b 0x8007fb78
8007fb68: 3b de 00 01  	addi 30, 30, 1
8007fb6c: 3b ff 00 48  	addi 31, 31, 72
8007fb70: 42 00 ff c8  	bdnz 0x8007fb38
8007fb74: 38 60 ff ff  	li 3, -1
8007fb78: 80 01 00 24  	lwz 0, 36(1)
8007fb7c: 83 e1 00 1c  	lwz 31, 28(1)
8007fb80: 83 c1 00 18  	lwz 30, 24(1)
8007fb84: 83 a1 00 14  	lwz 29, 20(1)
8007fb88: 7c 08 03 a6  	mtlr 0
8007fb8c: 38 21 00 20  	addi 1, 1, 32
8007fb90: 4e 80 00 20  	blr
```

## Behavior

`windowEntry` finds the first inactive window slot, clears it using
`heapZero`, sets its active flag to 1, stores the supplied window ID at
offset 4, and returns the slot index. Returns -1 if no slot is available.

| Item | Value |
| --- | --- |
| Argument | `r3` = window ID (16-bit, stored at offset 4) |
| Search | Iterates 7 slots, checking state at offset 2 |
| Clear | `heapZero(record, 0, 72)` via `0x800050b4` |
| Activation | Sets state (offset 2) to 1, stores ID (offset 4) |
| Return | `r3` = slot index (0-6) or -1 if full |
| Calls | `0x800050b4` (heapZero) |

## Interpretation

This function allocates a window slot for a new window. It scans for
the first inactive slot, clears it, marks it as active, and stores the
window ID. The window ID is used to identify the window type or instance.
If all 7 slots are in use, the function returns -1 to indicate failure.

## Remaining gaps

- Meaning of the window ID stored at offset 4
- Full layout of the 72-byte window record structure
- Caller graph
