# btlevtcmd_GetFirstAttackTarget

**Address:** `0x8010b418`  
**Size:** 120 bytes (30 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
8010b418: 94 21 ff e0  	stwu 1, -32(1)
8010b41c: 7c 08 02 a6  	mflr 0
8010b420: 90 01 00 24  	stw 0, 36(1)
8010b424: 38 81 00 0c  	addi 4, 1, 12
8010b428: 38 a1 00 08  	addi 5, 1, 8
8010b42c: bf 81 00 10  	stmw 28, 16(1)
8010b430: 7c 7c 1b 78  	mr 28, 3
8010b434: 80 dc 00 18  	lwz 6, 24(28)
8010b438: 80 6d 1c 70  	lwz 3, 7280(13)
8010b43c: 83 e6 00 00  	lwz 31, 0(6)
8010b440: 83 c6 00 04  	lwz 30, 4(6)
8010b444: 4b ff 36 b5  	bl 0x800feaf8
8010b448: 80 61 00 08  	lwz 3, 8(1)
8010b44c: 48 01 d8 4d  	bl 0x80128c98
8010b450: 80 81 00 08  	lwz 4, 8(1)
8010b454: 7c 7d 1b 78  	mr 29, 3
8010b458: 7f 83 e3 78  	mr 3, 28
8010b45c: 80 a4 00 00  	lwz 5, 0(4)
8010b460: 7f e4 fb 78  	mr 4, 31
8010b464: 4b f2 58 99  	bl 0x80030cfc
8010b468: 7f 83 e3 78  	mr 3, 28
8010b46c: 7f c4 f3 78  	mr 4, 30
8010b470: 7f a5 eb 78  	mr 5, 29
8010b474: 4b f2 58 89  	bl 0x80030cfc
8010b478: bb 81 00 10  	lmw 28, 16(1)
8010b47c: 38 60 00 02  	li 3, 2
8010b480: 80 01 00 24  	lwz 0, 36(1)
8010b484: 7c 08 03 a6  	mtlr 0
8010b488: 38 21 00 20  	addi 1, 1, 32
8010b48c: 4e 80 00 20  	blr
```

## Behavior

`btlevtcmd_GetFirstAttackTarget` is an event command that finds the first
attack target. It calls `BattleGetFirstAttackUnit` to find the first
enemy unit, then `BtlUnit_GetBodyPartsId` to get the body part ID, and
finally sets two event variables with the results.

| Item | Value |
| --- | --- |
| Argument | `r3` = event argument structure |
| Calls | `0x800feaf8` (BattleGetFirstAttackUnit), `0x80128c98` (BtlUnit_GetBodyPartsId), `0x80030cfc` (set variable) x2 |
| Return | `r3` = 2 (command complete) |

## Interpretation

This event command resolves the first attack target for a battle event.
It searches for the first enemy unit, retrieves its body part ID, and
stores both values in event variables for later use. The command is
used in battle event scripts to determine which unit to target.

## Remaining gaps

- Full layout of the event argument structure
- Implementation of called functions
- Caller graph
- Relationship to other target selection commands
