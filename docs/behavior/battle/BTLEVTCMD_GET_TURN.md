# btlevtcmd_get_turn

**Address:** `0x801137b8`  
**Size:** 52 bytes (13 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
801137b8: 94 21 ff f0  	stwu 1, -16(1)
801137bc: 7c 08 02 a6  	mflr 0
801137c0: 90 01 00 14  	stw 0, 20(1)
801137c4: 80 83 00 18  	lwz 4, 24(3)
801137c8: 80 ad 1c 70  	lwz 5, 7280(13)
801137cc: 80 84 00 00  	lwz 4, 0(4)
801137d0: a8 a5 00 00  	lha 5, 0(5)
801137d4: 4b f1 d5 29  	bl 0x80030cfc
801137d8: 80 01 00 14  	lwz 0, 20(1)
801137dc: 38 60 00 02  	li 3, 2
801137e0: 7c 08 03 a6  	mtlr 0
801137e4: 38 21 00 10  	addi 1, 1, 16
801137e8: 4e 80 00 20  	blr
```

## Behavior

`btlevtcmd_get_turn` is an event command that retrieves the current turn number. It reads the turn from the battle work structure and stores it in an event variable.

| Item | Value |
| --- | --- |
| Argument | `r3` = event argument structure |
| Calls | `0x80030cfc` (set variable) |
| Return | `r3` = 2 (command complete) |

## Interpretation

This event command retrieves the current turn number from the battle work structure (at offset 0) and stores it in an event variable. This is used in battle event scripts for turn-dependent logic.

## Remaining gaps

- Full layout of the event argument structure
- Implementation of the set variable function
- Caller graph
