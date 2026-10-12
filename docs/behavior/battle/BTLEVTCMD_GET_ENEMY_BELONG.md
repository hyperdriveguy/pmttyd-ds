# btlevtcmd_GetEnemyBelong

**Address:** `0x8010eed4`  
**Size:** 112 bytes (28 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
8010eed4: 94 21 ff f0  	stwu 1, -16(1)
8010eed8: 7c 08 02 a6  	mflr 0
8010eedc: 90 01 00 14  	stw 0, 20(1)
8010eee0: bf c1 00 08  	stmw 30, 8(1)
8010eee4: 7c 7e 1b 78  	mr 30, 3
8010eee8: 83 e3 00 18  	lwz 31, 24(3)
8010eeec: 80 9f 00 00  	lwz 4, 0(31)
8010eef0: 4b f2 23 71  	bl 0x80031260
8010eef4: 7c 60 1b 78  	mr 0, 3
8010eef8: 7f c3 f3 78  	mr 3, 30
8010eefc: 7c 04 03 78  	mr 4, 0
8010ef00: 48 01 64 ed  	bl 0x801253ec
8010ef04: 7c 64 1b 78  	mr 4, 3
8010ef08: 83 ff 00 04  	lwz 31, 4(31)
8010ef0c: 80 6d 1c 70  	lwz 3, 7280(13)
8010ef10: 4b fe 99 bd  	bl 0x800f88cc
8010ef14: 48 01 85 41  	bl 0x80127454
8010ef18: 7c 60 1b 78  	mr 0, 3
8010ef1c: 7f c3 f3 78  	mr 3, 30
8010ef20: 7c 05 03 78  	mr 5, 0
8010ef24: 7f e4 fb 78  	mr 4, 31
8010ef28: 4b f2 1d d5  	bl 0x80030cfc
8010ef2c: bb c1 00 08  	lmw 30, 8(1)
8010ef30: 38 60 00 02  	li 3, 2
8010ef34: 80 01 00 14  	lwz 0, 20(1)
8010ef38: 7c 08 03 a6  	mtlr 0
8010ef3c: 38 21 00 10  	addi 1, 1, 16
8010ef40: 4e 80 00 20  	blr
```

## Behavior

`btlevtcmd_GetEnemyBelong` is an event command that retrieves the enemy
belong status of a unit. It resolves the unit ID, looks up the unit,
checks its belong status, and stores the result in an event variable.

| Item | Value |
| --- | --- |
| Argument | `r3` = event argument structure |
| Calls | `0x80031260` (resolve unit ID), `0x801253ec` (get unit), `0x800f88cc` (get unit from table), `0x80127454` (BtlUnit_GetEnemyBelong), `0x80030cfc` (set variable) |
| Return | `r3` = 2 (command complete) |

## Interpretation

This event command checks whether a unit is an enemy and stores the result
in an event variable. It is used in battle event scripts to determine team
membership for conditional logic.

## Remaining gaps

- Full layout of the event argument structure
- Implementation of called functions
- Caller graph
