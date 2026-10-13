# BtlUnit_Delete

**Address:** `0x80128e08`  
**Size:** 472 bytes (118 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
80128e08: 94 21 ff e0  	stwu 1, -32(1)
80128e0c: 7c 08 02 a6  	mflr 0
80128e10: 90 01 00 24  	stw 0, 36(1)
80128e14: bf 61 00 0c  	stmw 27, 12(1)
80128e18: 7c 7d 1b 79  	mr. 29, 3
80128e1c: 83 ed 1c 70  	lwz 31, 7280(13)
80128e20: 40 82 00 0c  	bf 2, 0x80128e2c
80128e24: 38 60 00 00  	li 3, 0
80128e28: 48 00 01 a4  	b 0x80128fcc
80128e2c: 3b c0 00 00  	li 30, 0
80128e30: 7f e3 fb 78  	mr 3, 31
80128e34: 7f c4 f3 78  	mr 4, 30
80128e38: 4b fc fa 95  	bl 0x800f88cc
80128e3c: 7c 03 e8 40  	cmplw 3, 29
80128e40: 41 82 00 10  	bt 2, 0x80128e50
80128e44: 3b de 00 01  	addi 30, 30, 1
80128e48: 2c 1e 00 40  	cmpwi 30, 64
80128e4c: 41 80 ff e4  	bt 0, 0x80128e30
80128e50: 2c 1e 00 40  	cmpwi 30, 64
80128e54: 41 80 00 0c  	bt 0, 0x80128e60
80128e58: 38 60 00 00  	li 3, 0
80128e5c: 48 00 01 70  	b 0x80128fcc
80128e60: 80 7d 02 90  	lwz 3, 656(29)
80128e64: 2c 03 00 00  	cmpwi 3, 0
80128e68: 41 82 00 10  	bt 2, 0x80128e78
80128e6c: 4b f0 d7 f9  	bl 0x80036664
80128e70: 38 00 00 00  	li 0, 0
80128e74: 90 1d 02 90  	stw 0, 656(29)
80128e78: 80 7d 02 9c  	lwz 3, 668(29)
80128e7c: 2c 03 00 00  	cmpwi 3, 0
80128e80: 41 82 00 10  	bt 2, 0x80128e90
80128e84: 4b f0 d7 e1  	bl 0x80036664
80128e88: 38 00 00 00  	li 0, 0
80128e8c: 90 1d 02 9c  	stw 0, 668(29)
80128e90: 80 7d 02 b4  	lwz 3, 692(29)
80128e94: 2c 03 00 00  	cmpwi 3, 0
80128e98: 41 82 00 10  	bt 2, 0x80128ea8
80128e9c: 4b f0 d7 c9  	bl 0x80036664
80128ea0: 38 00 00 00  	li 0, 0
80128ea4: 90 1d 02 b4  	stw 0, 692(29)
80128ea8: 38 00 00 40  	li 0, 64
80128eac: 80 dd 00 00  	lwz 6, 0(29)
80128eb0: 38 60 00 00  	li 3, 0
80128eb4: 38 80 ff ff  	li 4, -1
80128eb8: 7c 09 03 a6  	mtctr 0
80128ebc: 38 a3 01 20  	addi 5, 3, 288
80128ec0: 7c 1f 28 2e  	lwzx 0, 31, 5
80128ec4: 7c 00 30 00  	cmpw 0, 6
80128ec8: 40 82 00 08  	bf 2, 0x80128ed0
80128ecc: 7c 9f 29 2e  	stwx 4, 31, 5
80128ed0: 38 63 00 01  	addi 3, 3, 1
80128ed4: 42 00 ff e8  	bdnz 0x80128ebc
80128ed8: 80 1d 00 1c  	lwz 0, 28(29)
80128edc: 54 00 00 43  	rlwinm. 0, 0, 0, 1, 1
80128ee0: 41 82 00 2c  	bt 2, 0x80128f0c
80128ee4: 88 9d 00 0e  	lbz 4, 14(29)
80128ee8: 80 7f 27 38  	lwz 3, 10040(31)
80128eec: 7c 80 07 75  	extsb. 0, 4
80128ef0: 80 63 00 0c  	lwz 3, 12(3)
80128ef4: 41 80 00 18  	bt 0, 0x80128f0c
80128ef8: 7c 80 07 74  	extsb 0, 4
80128efc: 80 9d 03 08  	lwz 4, 776(29)
80128f00: 54 00 10 3a  	slwi 0, 0, 2
80128f04: 7c 63 02 14  	add 3, 3, 0
80128f08: 90 83 00 5c  	stw 4, 92(3)
80128f0c: 7f a3 eb 78  	mr 3, 29
80128f10: 48 06 d7 95  	bl 0x801966a4
80128f14: 7f a3 eb 78  	mr 3, 29
80128f18: 48 12 ad 8d  	bl 0x80253ca4
80128f1c: 80 7d 03 14  	lwz 3, 788(29)
80128f20: 28 03 00 00  	cmplwi 3, 0
80128f24: 41 82 00 24  	bt 2, 0x80128f48
80128f28: 81 9d 03 18  	lwz 12, 792(29)
80128f2c: 28 0c 00 00  	cmplwi 12, 0
80128f30: 41 82 00 14  	bt 2, 0x80128f44
80128f34: 7f a3 eb 78  	mr 3, 29
80128f38: 7d 89 03 a6  	mtctr 12
80128f3c: 4e 80 04 21  	bctrl
80128f40: 48 00 00 08  	b 0x80128f48
80128f44: 4b fc f9 c5  	bl 0x800f8908
80128f48: 80 7d 03 1c  	lwz 3, 796(29)
80128f4c: 28 03 00 00  	cmplwi 3, 0
80128f50: 41 82 00 10  	bt 2, 0x80128f60
80128f54: 4b f0 69 25  	bl 0x8002f878
80128f58: 38 00 00 00  	li 0, 0
80128f5c: 90 1d 03 1c  	stw 0, 796(29)
80128f60: 83 7d 00 14  	lwz 27, 20(29)
80128f64: 3b 80 ff ff  	li 28, -1
80128f68: 48 00 00 38  	b 0x80128fa0
80128f6c: 80 1b 04 e8  	lwz 0, 1256(27)
80128f70: 2c 00 ff ff  	cmpwi 0, -1
80128f74: 41 82 00 08  	bt 2, 0x80128f7c
80128f78: 93 9b 04 e8  	stw 28, 1256(27)
80128f7c: 80 7b 01 c0  	lwz 3, 448(27)
80128f80: 2c 03 ff ff  	cmpwi 3, -1
80128f84: 41 82 00 18  	bt 2, 0x80128f9c
80128f88: 38 80 00 00  	li 4, 0
80128f8c: 38 a0 00 00  	li 5, 0
80128f90: 4b f1 7a 99  	bl 0x80040a28
80128f94: 80 7b 01 c0  	lwz 3, 448(27)
80128f98: 4b f1 23 39  	bl 0x8003b2d0
80128f9c: 83 7b 00 00  	lwz 27, 0(27)
80128fa0: 28 1b 00 00  	cmplwi 27, 0
80128fa4: 40 82 ff c8  	bf 2, 0x80128f6c
80128fa8: 80 7d 00 14  	lwz 3, 20(29)
80128fac: 4b fc f9 5d  	bl 0x800f8908
80128fb0: 7f a3 eb 78  	mr 3, 29
80128fb4: 4b fc f9 55  	bl 0x800f8908
80128fb8: 7f e3 fb 78  	mr 3, 31
80128fbc: 7f c4 f3 78  	mr 4, 30
80128fc0: 38 a0 00 00  	li 5, 0
80128fc4: 4b fc f8 f1  	bl 0x800f88b4
80128fc8: 38 60 00 01  	li 3, 1
80128fcc: bb 61 00 0c  	lmw 27, 12(1)
80128fd0: 80 01 00 24  	lwz 0, 36(1)
80128fd4: 7c 08 03 a6  	mtlr 0
80128fd8: 38 21 00 20  	addi 1, 1, 32
80128fdc: 4e 80 00 20  	blr
```

## Behavior

`BtlUnit_Delete` removes a battle unit from the unit table. It searches
for the unit, frees its resources, clears its entry, and removes it from
the unit's part list.

| Item | Value |
| --- | --- |
| Argument | `r3` = pointer to unit to delete (may be null) |
| Return | `r3` = 1 on success, 0 if unit not found or null |
| Calls | `0x800f88cc` (find unit in table), `0x80036664` (free resource) x3, `0x801966a4` (update unit), `0x80253ca4` (update parts), `0x800f8908` (BattleFree) x2, `0x800f88b4` (remove from table) |
| Side effects | Frees 3 resources, clears 64 table entries, updates unit and parts |

## Interpretation

This function deletes a battle unit and all its associated resources. It
first searches the unit table to find the unit's index, then frees three
resources (likely pose, display, and AI data), clears the unit's entry in
the table, updates the unit and its parts, and finally frees the unit
structure itself. The function returns 1 on success or 0 if the unit
was not found.

## Remaining gaps

- Full layout of the unit structure
- Meaning of the three freed resources
- Implementation of the called functions
- Caller graph
- Relationship to `BtlUnit_Spawn`
