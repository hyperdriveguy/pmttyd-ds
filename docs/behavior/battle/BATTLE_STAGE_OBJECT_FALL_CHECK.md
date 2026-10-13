# BattleStage_ObjectFallCheck

**Address:** `0x80147644`  
**Size:** 96 bytes (24 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
80147644: 94 21 ff f0  	stwu 1, -16(1)
80147648: 7c 08 02 a6  	mflr 0
8014764c: 90 01 00 14  	stw 0, 20(1)
80147650: 93 e1 00 0c  	stw 31, 12(1)
80147654: 80 6d 1c 70  	lwz 3, 7280(13)
80147658: 80 03 0e f4  	lwz 0, 3828(3)
8014765c: 3f e3 00 02  	addis 31, 3, 2
80147660: 54 00 04 e7  	rlwinm. 0, 0, 0, 19, 19
80147664: 41 82 00 0c  	bt 2, 0x80147670
80147668: 38 60 00 00  	li 3, 0
8014766c: 48 00 00 24  	b 0x80147690
80147670: 3c 60 80 37  	lis 3, -32713
80147674: 38 80 00 0a  	li 4, 10
80147678: 38 63 71 bc  	addi 3, 3, 29116
8014767c: 38 a0 00 00  	li 5, 0
80147680: 4b ee fe 2d  	bl 0x800374ac
80147684: 80 03 01 5c  	lwz 0, 348(3)
80147688: 90 1f 82 c8  	stw 0, -32056(31)
8014768c: 80 63 01 5c  	lwz 3, 348(3)
80147690: 80 01 00 14  	lwz 0, 20(1)
80147694: 83 e1 00 0c  	lwz 31, 12(1)
80147698: 7c 08 03 a6  	mtlr 0
8014769c: 38 21 00 10  	addi 1, 1, 16
801476a0: 4e 80 00 20  	blr
```

## Behavior

`BattleStage_ObjectFallCheck` checks whether a stage object should fall. It tests bit 10 (mask `0x0400`) of the battle flags. If clear, returns 0. If set, calls a stage object function and returns its result.

| Item | Value |
| --- | --- |
| Return | `r3` = 0 if bit 10 clear, else result of stage object function |
| Calls | `0x800374ac` (stage object function) |
| Side effect | Stores result to `[battleWork + 0x82c8]` |

## Interpretation

This function checks whether a stage object (e.g., a platform or prop) should fall during battle. It tests a specific flag in the battle flags field. If the flag is set, it calls a stage object function to determine the fall behavior and stores the result. This is used for dynamic stage elements that can change state during battle.

## Remaining gaps

- Full layout of the battle work structure
- Meaning of bit 10 at offset 3828
- Implementation of the stage object function at `0x800374ac`
- Caller graph
