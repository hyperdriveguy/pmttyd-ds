# btlevtcmd_SpawnUnit

**Address:** `0x801133bc`  
**Size:** 116 bytes (29 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
801133bc: 94 21 ff e0  	stwu 1, -32(1)
801133c0: 7c 08 02 a6  	mflr 0
801133c4: 90 01 00 24  	stw 0, 36(1)
801133c8: bf a1 00 14  	stmw 29, 20(1)
801133cc: 7c 7d 1b 78  	mr 29, 3
801133d0: 83 e3 00 18  	lwz 31, 24(3)
801133d4: 83 df 00 00  	lwz 30, 0(31)
801133d8: 80 9f 00 04  	lwz 4, 4(31)
801133dc: 4b f1 de 85  	bl 0x80031260
801133e0: 80 9f 00 08  	lwz 4, 8(31)
801133e4: 48 01 59 3d  	bl 0x80128d20
801133e8: 28 03 00 00  	cmplwi 3, 0
801133ec: 40 82 00 1c  	bf 2, 0x80113408
801133f0: 7f a3 eb 78  	mr 3, 29
801133f4: 7f c4 f3 78  	mr 4, 30
801133f8: 38 a0 ff ff  	li 5, -1
801133fc: 4b f1 d9 01  	bl 0x80030cfc
80113400: 38 60 00 02  	li 3, 2
80113404: 48 00 00 18  	b 0x8011341c
80113408: 80 a3 00 00  	lwz 5, 0(3)
8011340c: 7f a3 eb 78  	mr 3, 29
80113410: 7f c4 f3 78  	mr 4, 30
80113414: 4b f1 d8 e9  	bl 0x80030cfc
80113418: 38 60 00 02  	li 3, 2
8011341c: bb a1 00 14  	lmw 29, 20(1)
80113420: 80 01 00 24  	lwz 0, 36(1)
80113424: 7c 08 03 a6  	mtlr 0
80113428: 38 21 00 20  	addi 1, 1, 32
8011342c: 4e 80 00 20  	blr
```

## Behavior

`btlevtcmd_SpawnUnit` is an event command that spawns a new battle unit.
It resolves the unit type, calls the spawn function, and stores the
result in an event variable.

| Item | Value |
| --- | --- |
| Argument | `r3` = event argument structure |
| Calls | `0x80031260` (resolve unit type), `0x80128d20` (spawn unit), `0x80030cfc` (set variable) |
| Return | `r3` = 2 (command complete) |

## Interpretation

This event command spawns a new unit in battle. It resolves the unit type
from the event arguments, calls the spawn function to create the unit,
and stores the new unit's pointer in an event variable. If the spawn
fails, it stores -1 instead.

## Remaining gaps

- Full layout of the event argument structure
- Implementation of the spawn function at `0x80128d20`
- Caller graph
