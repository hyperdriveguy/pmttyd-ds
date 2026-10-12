# btlevtcmd_ChoiceSamplingEnemy

**Address:** `0x8010b5e4`  
**Size:** 128 bytes (32 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
8010b5e4: 94 21 ff e0  	stwu 1, -32(1)
8010b5e8: 7c 08 02 a6  	mflr 0
8010b5ec: 90 01 00 24  	stw 0, 36(1)
8010b5f0: bf 81 00 10  	stmw 28, 16(1)
8010b5f4: 7c 7c 1b 78  	mr 28, 3
8010b5f8: 80 8d 1c 70  	lwz 4, 7280(13)
8010b5fc: 83 e3 00 18  	lwz 31, 24(3)
8010b600: 3b c4 04 28  	addi 30, 4, 1064
8010b604: 80 9f 00 00  	lwz 4, 0(31)
8010b608: 4b f2 5c 59  	bl 0x80031260
8010b60c: 7c 64 1b 78  	mr 4, 3
8010b610: 83 bf 00 04  	lwz 29, 4(31)
8010b614: 83 ff 00 08  	lwz 31, 8(31)
8010b618: 7f c3 f3 78  	mr 3, 30
8010b61c: 80 84 00 7c  	lwz 4, 124(4)
8010b620: 38 a1 00 0c  	addi 5, 1, 12
8010b624: 38 c1 00 08  	addi 6, 1, 8
8010b628: 4b ff 35 a1  	bl 0x800febc8
8010b62c: 80 a1 00 0c  	lwz 5, 12(1)
8010b630: 7f 83 e3 78  	mr 3, 28
8010b634: 7f a4 eb 78  	mr 4, 29
8010b638: 4b f2 56 c5  	bl 0x80030cfc
8010b63c: 80 a1 00 08  	lwz 5, 8(1)
8010b640: 7f 83 e3 78  	mr 3, 28
8010b644: 7f e4 fb 78  	mr 4, 31
8010b648: 4b f2 56 b5  	bl 0x80030cfc
8010b64c: bb 81 00 10  	lmw 28, 16(1)
8010b650: 38 60 00 02  	li 3, 2
8010b654: 80 01 00 24  	lwz 0, 36(1)
8010b658: 7c 08 03 a6  	mtlr 0
8010b65c: 38 21 00 20  	addi 1, 1, 32
8010b660: 4e 80 00 20  	blr
```

## Behavior

`btlevtcmd_ChoiceSamplingEnemy` is an event command that performs
choice-based enemy sampling. It resolves the unit ID, calls a sampling
function, and stores the results in event variables.

| Item | Value |
| --- | --- |
| Argument | `r3` = event argument structure |
| Calls | `0x80031260` (resolve unit ID), `0x800febc8` (sampling function), `0x80030cfc` (set variable) x2 |
| Return | `r3` = 2 (command complete) |

## Interpretation

This event command performs choice-based sampling of enemy units. It
resolves the unit, calls a sampling function that returns two values,
and stores both in event variables. This is used for more complex target
selection logic in battle events.

## Remaining gaps

- Full layout of the event argument structure
- Implementation of the sampling function at `0x800febc8`
- Caller graph
- Relationship to `btlevtcmd_SamplingEnemy`
