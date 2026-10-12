# windowMain

**Address:** `0x8007f9ec`  
**Size:** 104 bytes (26 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
8007f9ec: 94 21 ff f0  	stwu 1, -16(1)
8007f9f0: 7c 08 02 a6  	mflr 0
8007f9f4: 90 01 00 14  	stw 0, 20(1)
8007f9f8: 93 e1 00 0c  	stw 31, 12(1)
8007f9fc: 93 c1 00 08  	stw 30, 8(1)
8007fa00: 3b c0 00 00  	li 30, 0
8007fa04: 83 ed 1b 08  	lwz 31, 6920(13)
8007fa08: a0 1f 00 02  	lhz 0, 2(31)
8007fa0c: 54 00 07 ff  	clrlwi. 0, 0, 31
8007fa10: 41 82 00 1c  	bt 2, 0x8007fa2c
8007fa14: 81 9f 00 3c  	lwz 12, 60(31)
8007fa18: 28 0c 00 00  	cmplwi 12, 0
8007fa1c: 41 82 00 10  	bt 2, 0x8007fa2c
8007fa20: 7f e3 fb 78  	mr 3, 31
8007fa24: 7d 89 03 a6  	mtctr 12
8007fa28: 4e 80 04 21  	bctrl
8007fa2c: 3b de 00 01  	addi 30, 30, 1
8007fa30: 3b ff 00 48  	addi 31, 31, 72
8007fa34: 2c 1e 00 07  	cmpwi 30, 7
8007fa38: 41 80 ff d0  	bt 0, 0x8007fa08
8007fa3c: 80 01 00 14  	lwz 0, 20(1)
8007fa40: 83 e1 00 0c  	lwz 31, 12(1)
8007fa44: 83 c1 00 08  	lwz 30, 8(1)
8007fa48: 7c 08 03 a6  	mtlr 0
8007fa4c: 38 21 00 10  	addi 1, 1, 16
8007fa50: 4e 80 00 20  	blr
```

## Behavior

`windowMain` iterates over all 7 window records. For each active window,
it calls the update callback stored at offset 60 of the record.

| Item | Value |
| --- | --- |
| Window array base | `[r13 + 0x1b08]` |
| Records processed | 7 (indices 0-6) |
| Active check | Halfword at offset 2, tested for nonzero |
| Update callback | `[record + 60]` (called if active and nonzero) |
| Return | None (void) |
| Calls | Indirect callback at `[record + 60]` |

## Interpretation

This is the main window update loop. It scans all 7 window slots and
invokes the update callback for each active window. The callback at
offset 60 is a function pointer that handles the window's per-frame
update logic (animation, input, rendering, etc.).

## Remaining gaps

- Full layout of the 72-byte window record structure
- Type-specific update callbacks
- Caller graph
