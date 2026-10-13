# weaponGetPowerFromMarioHammerLv

**Address:** `0x80189c38`  
**Size:** 120 bytes (30 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
80189c38: 94 21 ff e0  	stwu 1, -32(1)
80189c3c: 7c 08 02 a6  	mflr 0
80189c40: 90 01 00 24  	stw 0, 36(1)
80189c44: bf a1 00 14  	stmw 29, 20(1)
80189c48: 7c 7d 1b 78  	mr 29, 3
80189c4c: 7c 9e 23 78  	mr 30, 4
80189c50: 80 ad 1c 70  	lwz 5, 7280(13)
80189c54: 80 05 1c b8  	lwz 0, 7352(5)
80189c58: 54 1f ff fe  	rlwinm 31, 0, 31, 31, 31
80189c5c: 4b f4 c2 55  	bl 0x800d5eb0
80189c60: 88 03 00 99  	lbz 0, 153(3)
80189c64: 7c 03 07 74  	extsb 3, 0
80189c68: 38 03 ff ff  	addi 0, 3, -1
80189c6c: 54 00 08 3c  	slwi 0, 0, 1
80189c70: 7c 1f 02 14  	add 0, 31, 0
80189c74: 2c 00 00 06  	cmpwi 0, 6
80189c78: 41 80 00 08  	bt 0, 0x80189c80
80189c7c: 38 00 00 05  	li 0, 5
80189c80: 88 9d 03 00  	lbz 4, 768(29)
80189c84: 54 00 10 3a  	slwi 0, 0, 2
80189c88: 7c 7e 02 14  	add 3, 30, 0
80189c8c: 28 04 00 00  	cmplwi 4, 0
80189c90: 80 63 00 20  	lwz 3, 32(3)
80189c94: 41 82 00 08  	bt 2, 0x80189c9c
80189c98: 7c 63 22 14  	add 3, 3, 4
80189c9c: bb a1 00 14  	lmw 29, 20(1)
80189ca0: 80 01 00 24  	lwz 0, 36(1)
80189ca4: 7c 08 03 a6  	mtlr 0
80189ca8: 38 21 00 20  	addi 1, 1, 32
80189cac: 4e 80 00 20  	blr
```

## Behavior

`weaponGetPowerFromMarioHammerLv` calculates the Hammer weapon power based
on Mario's Hammer level. It reads the level from the pouch, clamps the
index, looks up a power value from a table, and adds a bonus if present.

| Item | Value |
| --- | --- |
| Argument | `r3` = pointer to weapon data |
| Argument | `r4` = pointer to power output table |
| Level source | `[pouch + 153]` (signed byte) |
| Index calculation | `(level - 1) * 2 + (battleWork[7352] & 1)`, clamped to max 5 |
| Return | `r3` = `outputTable[index * 4] + bonus` (bonus from weapon data offset 768) |
| Calls | `0x800d5eb0` (pouchGetPtr) |

## Interpretation

This function calculates the power of Mario's Hammer attack based on his
Hammer level. The level is read from the pouch state and used to index into
a power table. The index is clamped to a maximum of 5 (representing 6
levels, 0-5). An additional bonus value from the weapon data is added to
the table lookup result if present. This is the Hammer counterpart to
`weaponGetPowerFromMarioJumpLv`.

## Remaining gaps

- Exact layout of the power output table
- Meaning of the flag at battle work offset 7352
- Full weapon data structure layout
- Caller graph
- Relationship to `weaponGetPowerFromMarioJumpLv`
