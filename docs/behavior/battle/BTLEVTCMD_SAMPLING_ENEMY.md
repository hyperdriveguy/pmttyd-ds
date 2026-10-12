# btlevtcmd_SamplingEnemy

**Address:** `0x8010b664`  
**Size:** 176 bytes (44 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
8010b664: 94 21 ff e0  	stwu 1, -32(1)
8010b668: 7c 08 02 a6  	mflr 0
8010b66c: 90 01 00 24  	stw 0, 36(1)
8010b670: bf 61 00 0c  	stmw 27, 12(1)
8010b674: 7c 7b 1b 78  	mr 27, 3
8010b678: 83 a3 00 18  	lwz 29, 24(3)
8010b67c: 83 ed 1c 70  	lwz 31, 7280(13)
8010b680: 80 9d 00 00  	lwz 4, 0(29)
8010b684: 3b 9f 04 28  	addi 28, 31, 1064
8010b688: 4b f2 5b d9  	bl 0x80031260
8010b68c: 7c 60 1b 78  	mr 0, 3
8010b690: 7f 63 db 78  	mr 3, 27
8010b694: 7c 04 03 78  	mr 4, 0
8010b698: 48 01 9d 55  	bl 0x801253ec
8010b69c: 80 9d 00 04  	lwz 4, 4(29)
8010b6a0: 7c 7e 1b 78  	mr 30, 3
8010b6a4: 7f 63 db 78  	mr 3, 27
8010b6a8: 4b f2 5b b9  	bl 0x80031260
8010b6ac: 80 9d 00 08  	lwz 4, 8(29)
8010b6b0: 7c 7d 07 74  	extsb 29, 3
8010b6b4: 7f 63 db 78  	mr 3, 27
8010b6b8: 4b f2 5b a9  	bl 0x80031260
8010b6bc: 57 a0 18 38  	slwi 0, 29, 3
8010b6c0: 81 03 00 68  	lwz 8, 104(3)
8010b6c4: 7c 64 1b 78  	mr 4, 3
8010b6c8: 7c 7f 02 14  	add 3, 31, 0
8010b6cc: 55 00 00 c7  	rlwinm. 0, 8, 0, 3, 3
8010b6d0: 88 63 00 0a  	lbz 3, 10(3)
8010b6d4: 7c 03 00 d0  	neg 0, 3
8010b6d8: 7c 09 07 74  	extsb 9, 0
8010b6dc: 41 82 00 0c  	bt 2, 0x8010b6e8
8010b6e0: 1c 09 ff ff  	mulli 0, 9, -1
8010b6e4: 7c 09 07 74  	extsb 9, 0
8010b6e8: 80 e4 00 64  	lwz 7, 100(4)
8010b6ec: 7f 83 e3 78  	mr 3, 28
8010b6f0: 7f c5 f3 78  	mr 5, 30
8010b6f4: 7f a6 eb 78  	mr 6, 29
8010b6f8: 4b ff 3e c9  	bl 0x800ff5c0
8010b6fc: bb 61 00 0c  	lmw 27, 12(1)
8010b700: 38 60 00 02  	li 3, 2
8010b704: 80 01 00 24  	lwz 0, 36(1)
8010b708: 7c 08 03 a6  	mtlr 0
8010b70c: 38 21 00 20  	addi 1, 1, 32
8010b710: 4e 80 00 20  	blr
```

## Behavior

`btlevtcmd_SamplingEnemy` is an event command that samples enemy units
for targeting. It resolves the unit ID, looks up the unit, computes
sampling parameters based on the unit's properties, and calls a sampling
function.

| Item | Value |
| --- | --- |
| Argument | `r3` = event argument structure |
| Calls | `0x80031260` (resolve unit ID) x3, `0x801253ec` (get unit), `0x800ff5c0` (sampling function) |
| Return | `r3` = 2 (command complete) |

## Interpretation

This event command performs enemy sampling for battle targeting. It
resolves the unit, computes sampling parameters (including a negation
operation on a byte value), and calls a sampling function to determine
valid targets. The sampling is used for random or weighted target
selection.

## Remaining gaps

- Full layout of the event argument structure
- Implementation of the sampling function at `0x800ff5c0`
- Caller graph
- Relationship to `btlevtcmd_ChoiceSamplingEnemy`
