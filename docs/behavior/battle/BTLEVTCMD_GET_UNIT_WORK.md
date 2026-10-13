# btlevtcmd_GetUnitWork

**Address:** `0x8010f828`  
**Size:** 132 bytes (33 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
8010f828: 94 21 ff e0  	stwu 1, -32(1)
8010f82c: 7c 08 02 a6  	mflr 0
8010f830: 90 01 00 24  	stw 0, 36(1)
8010f834: bf 81 00 10  	stmw 28, 16(1)
8010f838: 7c 7c 1b 78  	mr 28, 3
8010f83c: 83 a3 00 18  	lwz 29, 24(3)
8010f840: 80 9d 00 00  	lwz 4, 0(29)
8010f844: 4b f2 1a 1d  	bl 0x80031260
8010f848: 7c 60 1b 78  	mr 0, 3
8010f84c: 7f 83 e3 78  	mr 3, 28
8010f850: 7c 04 03 78  	mr 4, 0
8010f854: 48 01 5b 99  	bl 0x801253ec
8010f858: 80 9d 00 04  	lwz 4, 4(29)
8010f85c: 7c 7e 1b 78  	mr 30, 3
8010f860: 7f 83 e3 78  	mr 3, 28
8010f864: 4b f2 19 fd  	bl 0x80031260
8010f868: 7c 7f 1b 78  	mr 31, 3
8010f86c: 83 bd 00 08  	lwz 29, 8(29)
8010f870: 80 6d 1c 70  	lwz 3, 7280(13)
8010f874: 7f c4 f3 78  	mr 4, 30
8010f878: 4b fe 90 55  	bl 0x800f88cc
8010f87c: 57 e0 10 3a  	slwi 0, 31, 2
8010f880: 7c 83 02 14  	add 4, 3, 0
8010f884: 7f 83 e3 78  	mr 3, 28
8010f888: 80 a4 02 18  	lwz 5, 536(4)
8010f88c: 7f a4 eb 78  	mr 4, 29
8010f890: 4b f2 14 6d  	bl 0x80030cfc
8010f894: bb 81 00 10  	lmw 28, 16(1)
8010f898: 38 60 00 02  	li 3, 2
8010f89c: 80 01 00 24  	lwz 0, 36(1)
8010f8a0: 7c 08 03 a6  	mtlr 0
8010f8a4: 38 21 00 20  	addi 1, 1, 32
8010f8a8: 4e 80 00 20  	blr
```

## Behavior

`btlevtcmd_GetUnitWork` is an event command that retrieves a unit's work value. It resolves the unit ID, looks up the unit, reads its work value, and stores the result in an event variable.

| Item | Value |
| --- | --- |
| Argument | `r3` = event argument structure |
| Calls | `0x80031260` (resolve unit ID) x2, `0x801253ec` (get unit), `0x800f88cc` (get unit from table), `0x80030cfc` (set variable) |
| Return | `r3` = 2 (command complete) |

## Interpretation

This event command retrieves a unit's work value (a per-unit variable used for battle logic). It resolves the unit ID, looks up the unit in the battle unit table, reads the work value at offset 536 (`0x218`), and stores it in an event variable.

## Remaining gaps

- Full layout of the event argument structure
- Implementation of called functions
- Caller graph
