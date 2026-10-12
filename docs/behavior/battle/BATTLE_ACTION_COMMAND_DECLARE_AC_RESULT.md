# BattleActionCommandDeclareACResult

**Address:** `0x800fa4f0`  
**Size:** 132 bytes (33 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
800fa4f0: 94 21 ff f0  	stwu 1, -16(1)
800fa4f4: 7c 08 02 a6  	mflr 0
800fa4f8: 2c 05 ff ff  	cmpwi 5, -1
800fa4fc: 3c c3 00 02  	addis 6, 3, 2
800fa500: 90 01 00 14  	stw 0, 20(1)
800fa504: 41 82 00 08  	bt 2, 0x800fa50c
800fa508: 48 00 00 10  	b 0x800fa518
800fa50c: 7c 83 23 78  	mr 3, 4
800fa510: 48 0a 51 61  	bl 0x8019f670
800fa514: 48 00 00 50  	b 0x800fa564
800fa518: 6c a3 80 00  	xoris 3, 5, 32768
800fa51c: 3c 00 43 30  	lis 0, 17200
800fa520: 90 61 00 0c  	stw 3, 12(1)
800fa524: 3c a0 80 2f  	lis 5, -32721
800fa528: c8 45 e0 48  	lfd 2, -8120(5)
800fa52c: 7c 83 23 78  	mr 3, 4
800fa530: 90 01 00 08  	stw 0, 8(1)
800fa534: c0 62 ad 64  	lfs 3, -21148(2)
800fa538: c8 21 00 08  	lfd 1, 8(1)
800fa53c: c0 02 ad 60  	lfs 0, -21152(2)
800fa540: ec 21 10 28  	fsubs 1, 1, 2
800fa544: 90 86 8f fc  	stw 4, -28676(6)
800fa548: ec 03 00 7a  	fmadds 0, 3, 1, 0
800fa54c: d0 06 90 00  	stfs 0, -28672(6)
800fa550: 88 04 00 19  	lbz 0, 25(4)
800fa554: 98 06 90 05  	stb 0, -28667(6)
800fa558: 88 04 00 1a  	lbz 0, 26(4)
800fa55c: 98 06 90 06  	stb 0, -28666(6)
800fa560: 48 0a 51 a9  	bl 0x8019f708
800fa564: 80 01 00 14  	lwz 0, 20(1)
800fa568: 7c 08 03 a6  	mtlr 0
800fa56c: 38 21 00 10  	addi 1, 1, 16
800fa570: 4e 80 00 20  	blr
```

## Behavior

`BattleActionCommandDeclareACResult` declares an action command result.
If the target is -1, it calls a special handler. Otherwise, it converts
the target value to a float, stores it in the battle work structure,
and calls a notification function.

| Item | Value |
| --- | --- |
| Argument | `r3` = pointer to battle work structure |
| Argument | `r4` = pointer to result structure |
| Argument | `r5` = target value (integer, -1 for special case) |
| Calls | `0x8019f670` (special handler), `0x8019f708` (notification) |
| Return | None (void) |

## Interpretation

This function declares an action command result to the battle system.
It handles two cases: a special case when the target is -1, and a normal
case where the target value is converted to a float and stored. The
function also copies two bytes from the result structure to the battle
work structure, likely for tracking or scoring purposes.

## Remaining gaps

- Full layout of the battle work structure
- Full layout of the result structure
- Implementation of the called functions
- Caller graph
