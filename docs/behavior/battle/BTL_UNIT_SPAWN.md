# BtlUnit_Spawn

**Address:** `0x80128d20`  
**Size:** 232 bytes (58 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
80128d20: 94 21 ff f0  	stwu 1, -16(1)
80128d24: 7c 08 02 a6  	mflr 0
80128d28: 90 01 00 14  	stw 0, 20(1)
80128d2c: bf c1 00 08  	stmw 30, 8(1)
80128d30: 7c 9e 23 78  	mr 30, 4
80128d34: 83 ed 1c 70  	lwz 31, 7280(13)
80128d38: 48 00 02 a9  	bl 0x80128fe0
80128d3c: 7c 66 1b 79  	mr. 6, 3
80128d40: 40 82 00 08  	bf 2, 0x80128d48
80128d44: 48 00 00 b0  	b 0x80128df4
80128d48: 57 c0 07 bd  	rlwinm. 0, 30, 0, 30, 30
80128d4c: 41 82 00 50  	bt 2, 0x80128d9c
80128d50: 38 60 00 00  	li 3, 0
80128d54: 38 00 00 40  	li 0, 64
80128d58: 98 66 00 20  	stb 3, 32(6)
80128d5c: 80 a6 00 00  	lwz 5, 0(6)
80128d60: 7c 09 03 a6  	mtctr 0
80128d64: 2c 05 ff ff  	cmpwi 5, -1
80128d68: 41 82 00 88  	bt 2, 0x80128df0
80128d6c: 38 83 01 20  	addi 4, 3, 288
80128d70: 7c 1f 20 2e  	lwzx 0, 31, 4
80128d74: 2c 00 ff ff  	cmpwi 0, -1
80128d78: 40 82 00 10  	bf 2, 0x80128d88
80128d7c: 7c 7f 1a 14  	add 3, 31, 3
80128d80: 90 a3 01 20  	stw 5, 288(3)
80128d84: 48 00 00 6c  	b 0x80128df0
80128d88: 7c bf 21 2e  	stwx 5, 31, 4
80128d8c: 7c 05 03 78  	mr 5, 0
80128d90: 38 63 00 04  	addi 3, 3, 4
80128d94: 42 00 ff d0  	bdnz 0x80128d64
80128d98: 48 00 00 58  	b 0x80128df0
80128d9c: 57 c0 07 ff  	clrlwi. 0, 30, 31
80128da0: 41 82 00 50  	bt 2, 0x80128df0
80128da4: 38 60 00 00  	li 3, 0
80128da8: 38 00 00 40  	li 0, 64
80128dac: 98 66 00 20  	stb 3, 32(6)
80128db0: 38 60 00 fc  	li 3, 252
80128db4: 80 a6 00 00  	lwz 5, 0(6)
80128db8: 7c 09 03 a6  	mtctr 0
80128dbc: 2c 05 ff ff  	cmpwi 5, -1
80128dc0: 41 82 00 30  	bt 2, 0x80128df0
80128dc4: 38 83 01 20  	addi 4, 3, 288
80128dc8: 7c 1f 20 2e  	lwzx 0, 31, 4
80128dcc: 2c 00 ff ff  	cmpwi 0, -1
80128dd0: 40 82 00 10  	bf 2, 0x80128de0
80128dd4: 7c 7f 1a 14  	add 3, 31, 3
80128dd8: 90 a3 01 20  	stw 5, 288(3)
80128ddc: 48 00 00 14  	b 0x80128df0
80128de0: 7c bf 21 2e  	stwx 5, 31, 4
80128de4: 7c 05 03 78  	mr 5, 0
80128de8: 38 63 ff fc  	addi 3, 3, -4
80128dec: 42 00 ff d0  	bdnz 0x80128dbc
80128df0: 7c c3 33 78  	mr 3, 6
80128df4: bb c1 00 08  	lmw 30, 8(1)
80128df8: 80 01 00 14  	lwz 0, 20(1)
80128dfc: 7c 08 03 a6  	mtlr 0
80128e00: 38 21 00 10  	addi 1, 1, 16
80128e04: 4e 80 00 20  	blr
```

## Behavior

`BtlUnit_Spawn` creates a new battle unit. It calls the unit constructor,
then registers the unit's type ID in the battle work's unit type table.

| Item | Value |
| --- | --- |
| Argument | `r3` = (implicit from constructor) |
| Argument | `r4` = unit type ID |
| Return | `r3` = pointer to spawned unit, or 0 on failure |
| Calls | `0x80128fe0` (unit constructor) |
| Side effects | Registers type ID in unit type table (64 entries) |

## Interpretation

This function spawns a new battle unit by calling the unit constructor with
the supplied type ID. After construction, it registers the type ID in the
battle work's unit type table, which tracks which types are currently in
use. The function handles two cases based on a flag in the type ID:
normal registration (bit 1 clear) and reverse registration (bit 0 set).

## Remaining gaps

- Full layout of the unit structure
- Implementation of the unit constructor at `0x80128fe0`
- Caller graph
- Relationship to `BtlUnit_Delete`
