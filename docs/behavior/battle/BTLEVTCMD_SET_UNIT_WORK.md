# btlevtcmd_SetUnitWork

**Address:** `0x8010f7a4`  
**Size:** 132 bytes (33 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
8010f7a4: 94 21 ff e0  	stwu 1, -32(1)
8010f7a8: 7c 08 02 a6  	mflr 0
8010f7ac: 90 01 00 24  	stw 0, 36(1)
8010f7b0: bf a1 00 14  	stmw 29, 20(1)
8010f7b4: 7c 7f 1b 78  	mr 31, 3
8010f7b8: 83 c3 00 18  	lwz 30, 24(3)
8010f7bc: 80 9e 00 00  	lwz 4, 0(30)
8010f7c0: 4b f2 1a a1  	bl 0x80031260
8010f7c4: 7c 60 1b 78  	mr 0, 3
8010f7c8: 7f e3 fb 78  	mr 3, 31
8010f7cc: 7c 04 03 78  	mr 4, 0
8010f7d0: 48 01 5c 1d  	bl 0x801253ec
8010f7d4: 80 9e 00 04  	lwz 4, 4(30)
8010f7d8: 7c 7d 1b 78  	mr 29, 3
8010f7dc: 7f e3 fb 78  	mr 3, 31
8010f7e0: 4b f2 1a 81  	bl 0x80031260
8010f7e4: 80 9e 00 08  	lwz 4, 8(30)
8010f7e8: 7c 7e 1b 78  	mr 30, 3
8010f7ec: 7f e3 fb 78  	mr 3, 31
8010f7f0: 4b f2 1a 71  	bl 0x80031260
8010f7f4: 7c 7f 1b 78  	mr 31, 3
8010f7f8: 80 6d 1c 70  	lwz 3, 7280(13)
8010f7fc: 7f a4 eb 78  	mr 4, 29
8010f800: 4b fe 90 cd  	bl 0x800f88cc
8010f804: 57 c0 10 3a  	slwi 0, 30, 2
8010f808: 7c 83 02 14  	add 4, 3, 0
8010f80c: 38 60 00 02  	li 3, 2
8010f810: 93 e4 02 18  	stw 31, 536(4)
8010f814: bb a1 00 14  	lmw 29, 20(1)
8010f818: 80 01 00 24  	lwz 0, 36(1)
8010f81c: 7c 08 03 a6  	mtlr 0
8010f820: 38 21 00 20  	addi 1, 1, 32
8010f824: 4e 80 00 20  	blr
```

## Behavior

`btlevtcmd_SetUnitWork` is an event command that sets a unit's work value. It resolves the unit ID, looks up the unit, and stores the supplied value in the unit's work field.

| Item | Value |
| --- | --- |
| Argument | `r3` = event argument structure |
| Calls | `0x80031260` (resolve unit ID) x3, `0x801253ec` (get unit), `0x800f88cc` (get unit from table) |
| Return | `r3` = 2 (command complete) |

## Interpretation

This event command sets a unit's work value (a per-unit variable used for battle logic). It resolves the unit ID, looks up the unit in the battle unit table, and stores the supplied value at offset 536 (`0x218`). This is the write counterpart to `btlevtcmd_GetUnitWork`.

## Remaining gaps

- Full layout of the event argument structure
- Implementation of called functions
- Caller graph
