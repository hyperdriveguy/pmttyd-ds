# btlevtcmd_onoff_battleflag

**Address:** `0x801137ec`  
**Size:** 96 bytes (24 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
801137ec: 94 21 ff f0  	stwu 1, -16(1)
801137f0: 7c 08 02 a6  	mflr 0
801137f4: 90 01 00 14  	stw 0, 20(1)
801137f8: 93 e1 00 0c  	stw 31, 12(1)
801137fc: 83 e3 00 18  	lwz 31, 24(3)
80113800: 80 9f 00 00  	lwz 4, 0(31)
80113804: 4b f1 da 5d  	bl 0x80031260
80113808: 2c 03 00 01  	cmpwi 3, 1
8011380c: 80 7f 00 04  	lwz 3, 4(31)
80113810: 80 8d 1c 70  	lwz 4, 7280(13)
80113814: 40 82 00 14  	bf 2, 0x80113828
80113818: 80 04 0e f4  	lwz 0, 3828(4)
8011381c: 7c 00 1b 78  	or 0, 0, 3
80113820: 90 04 0e f4  	stw 0, 3828(4)
80113824: 48 00 00 10  	b 0x80113834
80113828: 80 04 0e f4  	lwz 0, 3828(4)
8011382c: 7c 00 18 78  	andc 0, 0, 3
80113830: 90 04 0e f4  	stw 0, 3828(4)
80113834: 80 01 00 14  	lwz 0, 20(1)
80113838: 38 60 00 02  	li 3, 2
8011383c: 83 e1 00 0c  	lwz 31, 12(1)
80113840: 7c 08 03 a6  	mtlr 0
80113844: 38 21 00 10  	addi 1, 1, 16
80113848: 4e 80 00 20  	blr
```

## Behavior

`btlevtcmd_onoff_battleflag` is an event command that sets or clears battle flags. It resolves a flag ID, then either sets or clears bits 0-1 of the battle flags field based on the flag ID.

| Item | Value |
| --- | --- |
| Argument | `r3` = event argument structure |
| Calls | `0x80031260` (resolve flag ID) |
| Side effect | If flag ID == 1: `battleFlags |= 3`; else: `battleFlags &= ~3` |
| Return | `r3` = 2 (command complete) |

## Interpretation

This event command toggles battle flags. When the flag ID is 1, it sets bits 0-1 of the battle flags field (at offset 3828). For any other flag ID, it clears bits 0-1. This is used to enable or disable specific battle behaviors or conditions.

## Remaining gaps

- Full layout of the event argument structure
- Meaning of bits 0-1 at offset 3828
- Caller graph
