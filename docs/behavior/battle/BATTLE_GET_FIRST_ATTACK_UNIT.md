# BattleGetFirstAttackUnit

**Address:** `0x800feaf8`  
**Size:** 208 bytes (52 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
800feaf8: 94 21 ff e0  	stwu 1, -32(1)
800feafc: 7c 08 02 a6  	mflr 0
800feb00: 90 01 00 24  	stw 0, 36(1)
800feb04: bf 61 00 0c  	stmw 27, 12(1)
800feb08: 7c 7b 1b 78  	mr 27, 3
800feb0c: 7c 9c 23 78  	mr 28, 4
800feb10: 7c bd 2b 78  	mr 29, 5
800feb14: 3b c0 00 00  	li 30, 0
800feb18: 7f 63 db 78  	mr 3, 27
800feb1c: 7f c4 f3 78  	mr 4, 30
800feb20: 4b ff 9d ad  	bl 0x800f88cc
800feb24: 7c 7f 1b 79  	mr. 31, 3
800feb28: 41 82 00 10  	bt 2, 0x800feb38
800feb2c: 88 1f 00 0c  	lbz 0, 12(31)
800feb30: 2c 00 00 01  	cmpwi 0, 1
800feb34: 41 82 00 10  	bt 2, 0x800feb44
800feb38: 3b de 00 01  	addi 30, 30, 1
800feb3c: 2c 1e 00 40  	cmpwi 30, 64
800feb40: 41 80 ff d8  	bt 0, 0x800feb18
800feb44: 80 7b 27 38  	lwz 3, 10040(27)
800feb48: 80 03 00 08  	lwz 0, 8(3)
800feb4c: 2c 00 00 09  	cmpwi 0, 9
800feb50: 41 82 00 50  	bt 2, 0x800feba0
800feb54: 40 80 00 4c  	bf 0, 0x800feba0
800feb58: 2c 00 00 07  	cmpwi 0, 7
800feb5c: 40 80 00 24  	bf 0, 0x800feb80
800feb60: 2c 00 00 01  	cmpwi 0, 1
800feb64: 40 80 00 08  	bf 0, 0x800feb6c
800feb68: 48 00 00 38  	b 0x800feba0
800feb6c: 7f 63 db 78  	mr 3, 27
800feb70: 4b ff 9c 39  	bl 0x800f87a8
800feb74: 7f e0 fb 78  	mr 0, 31
800feb78: 7c 7f 1b 78  	mr 31, 3
800feb7c: 48 00 00 30  	b 0x800febac
800feb80: 7f 63 db 78  	mr 3, 27
800feb84: 4b ff 9c 25  	bl 0x800f87a8
800feb88: 7c 64 1b 78  	mr 4, 3
800feb8c: 7f 63 db 78  	mr 3, 27
800feb90: 4b ff 9b 65  	bl 0x800f86f4
800feb94: 7f e0 fb 78  	mr 0, 31
800feb98: 7c 7f 1b 78  	mr 31, 3
800feb9c: 48 00 00 10  	b 0x800febac
800feba0: 7f 63 db 78  	mr 3, 27
800feba4: 4b ff 9c 05  	bl 0x800f87a8
800feba8: 7c 60 1b 78  	mr 0, 3
800febac: 93 fc 00 00  	stw 31, 0(28)
800febb0: 90 1d 00 00  	stw 0, 0(29)
800febb4: bb 61 00 0c  	lmw 27, 12(1)
800febb8: 80 01 00 24  	lwz 0, 36(1)
800febbc: 7c 08 03 a6  	mtlr 0
800febc0: 38 21 00 20  	addi 1, 1, 32
800febc4: 4e 80 00 20  	blr
```

## Behavior

`BattleGetFirstAttackUnit` searches through up to 64 battle units to
find the first enemy unit (belong == 1). It returns the unit pointer
and a secondary value through output parameters.

| Item | Value |
| --- | --- |
| Argument | `r3` = pointer to battle work structure |
| Output | `r4` = pointer to store unit pointer |
| Output | `r5` = pointer to store secondary value |
| Search | Iterates 64 units, checking belong byte at offset 12 |
| Return | `r3` = unit pointer, or 0 if not found |
| Calls | `0x800f88cc` (get unit from table), `0x800f87a8` (get unit type), `0x800f86f4` (get unit property) |

## Interpretation

This function finds the first enemy unit in the battle unit table. It
iterates through up to 64 units, checking each one's belong byte to
determine if it is an enemy. The function also retrieves additional
unit properties based on the unit type. The results are stored in the
output parameters for use by the caller.

## Remaining gaps

- Full layout of the battle work structure
- Meaning of the secondary value
- Caller graph
- Relationship to other target selection functions
