# evt_bero_mapchange

**Address:** `0x8007aad0`  
**Size:** 176 bytes (44 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
8007aad0: 94 21 ff e0  	stwu 1, -32(1)
8007aad4: 7c 08 02 a6  	mflr 0
8007aad8: 90 01 00 24  	stw 0, 36(1)
8007aadc: 93 e1 00 1c  	stw 31, 28(1)
8007aae0: 93 c1 00 18  	stw 30, 24(1)
8007aae4: 7c 9e 23 78  	mr 30, 4
8007aae8: 93 a1 00 14  	stw 29, 20(1)
8007aaec: 7c 7d 1b 78  	mr 29, 3
8007aaf0: 83 e3 00 18  	lwz 31, 24(3)
8007aaf4: 80 9f 00 00  	lwz 4, 0(31)
8007aaf8: 4b fb 67 69  	bl 0x80031260
8007aafc: 80 9f 00 04  	lwz 4, 4(31)
8007ab00: 7c 7f 1b 78  	mr 31, 3
8007ab04: 7f a3 eb 78  	mr 3, 29
8007ab08: 4b fb 67 59  	bl 0x80031260
8007ab0c: 2c 1e 00 00  	cmpwi 30, 0
8007ab10: 41 82 00 14  	bt 2, 0x8007ab24
8007ab14: 7f e4 fb 78  	mr 4, 31
8007ab18: 7c 65 1b 78  	mr 5, 3
8007ab1c: 38 60 00 03  	li 3, 3
8007ab20: 4b fb 36 f9  	bl 0x8002e218
8007ab24: 80 01 00 24  	lwz 0, 36(1)
8007ab28: 38 60 00 00  	li 3, 0
8007ab2c: 83 e1 00 1c  	lwz 31, 28(1)
8007ab30: 83 c1 00 18  	lwz 30, 24(1)
8007ab34: 83 a1 00 14  	lwz 29, 20(1)
8007ab38: 7c 08 03 a6  	mtlr 0
8007ab3c: 38 21 00 20  	addi 1, 1, 32
8007ab40: 4e 80 00 20  	blr
```

## Behavior

`evt_bero_mapchange` is an event command that initiates a map/room transition.
It resolves two arguments and calls the sequence change function.

| Item | Value |
| --- | --- |
| Argument | `r3` = event argument structure |
| Argument | `r4` = transition type (nonzero triggers sequence change) |
| Calls | `0x80031260` (resolve) x2, `0x8002e218` (seqMain) |
| Return | `r3` = 0 (yield) |

## Interpretation

This event command triggers a room/map transition. It resolves two values from
the event argument structure (at offset 24): the target map/entrance ID and
a secondary parameter. If the transition type argument (r4) is nonzero, it
calls `seqMain` with the resolved values to initiate the sequence change.

The function returns 0, which in the event system means the command is blocked
(will continue on the next eligible invocation after the sequence change
completes).

## Critical for porting

This is one of the most important functions for porting as it handles:
1. Room-to-room transitions (Rogueport plaza ↔ harbor ↔ east ↔ west ↔ station)
2. Sequence change initiation with proper parameter passing
3. The relationship between event scripts and the sequence system

## Remaining gaps

- Full sequence table and map/entrance ID mappings
- Relationship to fade system (fadeEntry called from seqMain)
- How the secondary parameter affects the transition
- Door/pipe vs road transition differentiation
