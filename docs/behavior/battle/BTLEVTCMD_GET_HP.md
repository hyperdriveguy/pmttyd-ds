# btlevtcmd_GetHp

**Address:** `0x801101c4`  
**Size:** 120 bytes (30 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
801101c4: 94 21 ff e0  	stwu 1, -32(1)
801101c8: 7c 08 02 a6  	mflr 0
801101cc: 90 01 00 24  	stw 0, 36(1)
801101d0: bf a1 00 14  	stmw 29, 20(1)
801101d4: 7c 7d 1b 78  	mr 29, 3
801101d8: 83 e3 00 18  	lwz 31, 24(3)
801101dc: 83 cd 1c 70  	lwz 30, 7280(13)
801101e0: 80 9f 00 00  	lwz 4, 0(31)
801101e4: 4b f2 10 7d  	bl 0x80031260
801101e8: 7c 60 1b 78  	mr 0, 3
801101ec: 7f a3 eb 78  	mr 3, 29
801101f0: 7c 04 03 78  	mr 4, 0
801101f4: 48 01 51 f9  	bl 0x801253ec
801101f8: 7c 64 1b 78  	mr 4, 3
801101fc: 2c 04 ff ff  	cmpwi 4, -1
80110200: 40 82 00 0c  	bf 2, 0x8011020c
80110204: 38 60 00 02  	li 3, 2
80110208: 48 00 00 20  	b 0x80110228
8011020c: 7f c3 f3 78  	mr 3, 30
80110210: 4b fe 86 bd  	bl 0x800f88cc
80110214: a8 a3 01 0c  	lha 5, 268(3)
80110218: 7f a3 eb 78  	mr 3, 29
8011021c: 80 9f 00 04  	lwz 4, 4(31)
80110220: 4b f2 0a dd  	bl 0x80030cfc
80110224: 38 60 00 02  	li 3, 2
80110228: bb a1 00 14  	lmw 29, 20(1)
8011022c: 80 01 00 24  	lwz 0, 36(1)
80110230: 7c 08 03 a6  	mtlr 0
80110234: 38 21 00 20  	addi 1, 1, 32
80110238: 4e 80 00 20  	blr
```

## Behavior

`btlevtcmd_GetHp` is an event command that retrieves the current HP
of a unit. It resolves the unit ID, looks up the unit, reads its HP,
and stores the result in an event variable.

| Item | Value |
| --- | --- |
| Argument | `r3` = event argument structure |
| Calls | `0x80031260` (resolve unit ID), `0x801253ec` (get unit), `0x800f88cc` (get unit from table), `0x80030cfc` (set variable) |
| Return | `r3` = 2 (command complete) |

## Interpretation

This event command retrieves the current HP of a specified unit. It
resolves the unit ID, looks up the unit in the battle unit table, reads
the HP value at offset 268 (`0x10c`), and stores it in an event variable.
If the unit is not found, it returns 2 without setting the variable.

## Remaining gaps

- Full layout of the event argument structure
- Implementation of called functions
- Caller graph
