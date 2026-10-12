# btlevtcmd_GetMaxHp

**Address:** `0x801100cc`  
**Size:** 120 bytes (30 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
801100cc: 94 21 ff e0  	stwu 1, -32(1)
801100d0: 7c 08 02 a6  	mflr 0
801100d4: 90 01 00 24  	stw 0, 36(1)
801100d8: bf a1 00 14  	stmw 29, 20(1)
801100dc: 7c 7d 1b 78  	mr 29, 3
801100e0: 83 e3 00 18  	lwz 31, 24(3)
801100e4: 83 cd 1c 70  	lwz 30, 7280(13)
801100e8: 80 9f 00 00  	lwz 4, 0(31)
801100ec: 4b f2 11 75  	bl 0x80031260
801100f0: 7c 60 1b 78  	mr 0, 3
801100f4: 7f a3 eb 78  	mr 3, 29
801100f8: 7c 04 03 78  	mr 4, 0
801100fc: 48 01 52 f1  	bl 0x801253ec
80110100: 7c 64 1b 78  	mr 4, 3
80110104: 2c 04 ff ff  	cmpwi 4, -1
80110108: 40 82 00 0c  	bf 2, 0x80110114
8011010c: 38 60 00 02  	li 3, 2
80110110: 48 00 00 20  	b 0x80110130
80110114: 7f c3 f3 78  	mr 3, 30
80110118: 4b fe 87 b5  	bl 0x800f88cc
8011011c: a8 a3 01 08  	lha 5, 264(3)
80110120: 7f a3 eb 78  	mr 3, 29
80110124: 80 9f 00 04  	lwz 4, 4(31)
80110128: 4b f2 0b d5  	bl 0x80030cfc
8011012c: 38 60 00 02  	li 3, 2
80110130: bb a1 00 14  	lmw 29, 20(1)
80110134: 80 01 00 24  	lwz 0, 36(1)
80110138: 7c 08 03 a6  	mtlr 0
8011013c: 38 21 00 20  	addi 1, 1, 32
80110140: 4e 80 00 20  	blr
```

## Behavior

`btlevtcmd_GetMaxHp` is an event command that retrieves the maximum HP
of a unit. It resolves the unit ID, looks up the unit, reads its max HP,
and stores the result in an event variable.

| Item | Value |
| --- | --- |
| Argument | `r3` = event argument structure |
| Calls | `0x80031260` (resolve unit ID), `0x801253ec` (get unit), `0x800f88cc` (get unit from table), `0x80030cfc` (set variable) |
| Return | `r3` = 2 (command complete) |

## Interpretation

This event command retrieves the maximum HP of a specified unit. It
resolves the unit ID, looks up the unit in the battle unit table, reads
the max HP value at offset 264 (`0x108`), and stores it in an event
variable. If the unit is not found, it returns 2 without setting the
variable.

## Remaining gaps

- Full layout of the event argument structure
- Implementation of called functions
- Caller graph
