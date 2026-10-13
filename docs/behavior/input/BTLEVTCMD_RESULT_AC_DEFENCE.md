# btlevtcmd_ResultACDefence

**Address:** `0x80107694`  
**Size:** 144 bytes (36 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
80107694: 94 21 ff f0  	stwu 1, -16(1)
80107698: 7c 08 02 a6  	mflr 0
8010769c: 90 01 00 14  	stw 0, 20(1)
801076a0: bf c1 00 08  	stmw 30, 8(1)
801076a4: 7c 7e 1b 78  	mr 30, 3
801076a8: 83 e3 00 18  	lwz 31, 24(3)
801076ac: 80 9f 00 00  	lwz 4, 0(31)
801076b0: 4b f2 9b b1  	bl 0x80031260
801076b4: 7c 60 1b 78  	mr 0, 3
801076b8: 7f c3 f3 78  	mr 3, 30
801076bc: 7c 04 03 78  	mr 4, 0
801076c0: 48 01 dd 2d  	bl 0x801253ec
801076c4: 7c 64 1b 78  	mr 4, 3
801076c8: 80 6d 1c 70  	lwz 3, 7280(13)
801076cc: 4b ff 12 01  	bl 0x800f88cc
801076d0: 80 9f 00 04  	lwz 4, 4(31)
801076d4: 7c 7f 1b 78  	mr 31, 3
801076d8: 7f c3 f3 78  	mr 3, 30
801076dc: 4b f2 9b 85  	bl 0x80031260
801076e0: 7c 60 1b 78  	mr 0, 3
801076e4: 7f e3 fb 78  	mr 3, 31
801076e8: 7c 04 03 78  	mr 4, 0
801076ec: 4b ff 2a cd  	bl 0x800fa1b8
801076f0: bb c1 00 08  	lmw 30, 8(1)
801076f4: 38 60 00 02  	li 3, 2
801076f8: 80 01 00 14  	lwz 0, 20(1)
801076fc: 7c 08 03 a6  	mtlr 0
80107700: 38 21 00 10  	addi 1, 1, 16
80107704: 4e 80 00 20  	blr
```

## Behavior

`btlevtcmd_ResultACDefence` is an event command that triggers the defense
action command result. It resolves the unit ID, looks up the unit, and calls
the defense result handler.

| Item | Value |
| --- | --- |
| Argument | `r3` = event argument structure |
| Calls | `0x80031260` (resolve unit ID) x2, `0x801253ec` (get unit), `0x800f88cc` (get unit from table), `0x800fa1b8` (defense result handler) |
| Return | `r3` = 2 (command complete) |

## Interpretation

This event command processes the defense result during battle. It resolves two
arguments from the event argument structure (at offset 24): the unit ID and
a secondary value. It looks up the unit in the battle unit table and calls
the defense result handler at `0x800fa1b8`. The return value of 2 indicates
the command is complete (not blocked).

This is a critical function for porting battle mechanics as it handles the
defense action command result, which determines whether a defense was
successful and applies the appropriate damage reduction.

## Remaining gaps

- Implementation of the defense result handler at `0x800fa1b8`
- Full semantics of the two resolved arguments
- Relationship to the defense timing windows and difficulty tables
- How the defense result is applied to damage calculation
