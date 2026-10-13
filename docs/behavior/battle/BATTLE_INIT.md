# BattleInit

**Address:** `0x800f8f04`  
**Size:** 312 bytes (78 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
800f8f04: 94 21 ff f0  	stwu 1, -16(1)
800f8f08: 7c 08 02 a6  	mflr 0
800f8f0c: 3c 80 00 02  	lis 4, 2
800f8f10: 90 01 00 14  	stw 0, 20(1)
800f8f14: bf c1 00 08  	stmw 30, 8(1)
800f8f18: 7c 7e 1b 78  	mr 30, 3
800f8f1c: 38 64 90 98  	addi 3, 4, -28520
800f8f20: 4b ff fa 15  	bl 0x800f8934
800f8f24: 3c 80 00 02  	lis 4, 2
800f8f28: 90 6d 1c 70  	stw 3, 7280(13)
800f8f2c: 38 a4 90 98  	addi 5, 4, -28520
800f8f30: 7c 7f 1b 78  	mr 31, 3
800f8f34: 38 80 00 00  	li 4, 0
800f8f38: 4b f0 c1 7d  	bl 0x800050b4
800f8f3c: 3c 7f 00 02  	addis 3, 31, 2
800f8f40: 38 00 ff ff  	li 0, -1
800f8f44: 90 03 90 70  	stw 0, -28560(3)
800f8f48: 90 1f 04 24  	stw 0, 1060(31)
800f8f4c: 93 df 27 38  	stw 30, 10040(31)
800f8f50: 48 02 15 3d  	bl 0x8011a48c
800f8f54: 48 11 d9 b5  	bl 0x80216908
800f8f58: 48 0b 12 d1  	bl 0x801aa228
800f8f5c: 7f e3 fb 78  	mr 3, 31
800f8f60: 48 00 16 cd  	bl 0x800fa62c
800f8f64: 4b ff eb 8d  	bl 0x800f7af0
800f8f68: 48 09 cb 2d  	bl 0x80195a94
800f8f6c: 48 00 a1 8d  	bl 0x801030f8
800f8f70: 7f e3 fb 78  	mr 3, 31
800f8f74: 38 80 00 00  	li 4, 0
800f8f78: 38 a0 00 00  	li 5, 0
800f8f7c: 4b ff fa 8d  	bl 0x800f8a08
800f8f80: 7f e3 fb 78  	mr 3, 31
800f8f84: 38 80 00 01  	li 4, 1
800f8f88: 3c a0 01 00  	lis 5, 256
800f8f8c: 4b ff fa 7d  	bl 0x800f8a08
800f8f90: 38 00 00 00  	li 0, 0
800f8f94: 3c 7f 00 02  	addis 3, 31, 2
800f8f98: 90 1f 0f 28  	stw 0, 3880(31)
800f8f9c: 38 80 00 00  	li 4, 0
800f8fa0: 38 a0 00 10  	li 5, 16
800f8fa4: 38 63 90 60  	addi 3, 3, -28576
800f8fa8: 90 1f 17 20  	stw 0, 5920(31)
800f8fac: 90 1f 1b b4  	stw 0, 7092(31)
800f8fb0: 90 1f 1b b8  	stw 0, 7096(31)
800f8fb4: 90 1f 1b c0  	stw 0, 7104(31)
800f8fb8: 90 1f 1b c4  	stw 0, 7108(31)
800f8fbc: 90 1f 1b cc  	stw 0, 7116(31)
800f8fc0: 90 1f 1b d0  	stw 0, 7120(31)
800f8fc4: 90 1f 1b d8  	stw 0, 7128(31)
800f8fc8: 90 1f 1b dc  	stw 0, 7132(31)
800f8fcc: 90 1f 1b e4  	stw 0, 7140(31)
800f8fd0: 90 1f 1b e8  	stw 0, 7144(31)
800f8fd4: 90 1f 1b f0  	stw 0, 7152(31)
800f8fd8: 90 1f 1b f4  	stw 0, 7156(31)
800f8fdc: 90 1f 1b fc  	stw 0, 7164(31)
800f8fe0: 90 1f 1c 00  	stw 0, 7168(31)
800f8fe4: 90 1f 1c 08  	stw 0, 7176(31)
800f8fe8: 90 1f 1c 0c  	stw 0, 7180(31)
800f8fec: 90 1f 1c 14  	stw 0, 7188(31)
800f8ff0: 90 1f 1c 18  	stw 0, 7192(31)
800f8ff4: 90 1f 1c 20  	stw 0, 7200(31)
800f8ff8: 90 1f 1c 24  	stw 0, 7204(31)
800f8ffc: 90 1f 1c 78  	stw 0, 7288(31)
800f9000: b0 1f 00 00  	sth 0, 0(31)
800f9004: 4b f0 c0 b1  	bl 0x800050b4
800f9008: 38 00 00 00  	li 0, 0
800f900c: 90 1f 0e f4  	stw 0, 3828(31)
800f9010: 48 03 06 85  	bl 0x80129694
800f9014: 7f e3 fb 78  	mr 3, 31
800f9018: 4b ff ec 09  	bl 0x800f7c20
800f901c: 48 04 07 65  	bl 0x80139780
800f9020: 48 0a f3 d9  	bl 0x801a83f8
800f9024: 48 10 c0 59  	bl 0x8020507c
800f9028: bb c1 00 08  	lmw 30, 8(1)
800f902c: 80 01 00 14  	lwz 0, 20(1)
800f9030: 7c 08 03 a6  	mtlr 0
800f9034: 38 21 00 10  	addi 1, 1, 16
800f9038: 4e 80 00 20  	blr
```

## Behavior

`BattleInit` initializes the battle system. It allocates the battle work structure, zeroes it, initializes multiple subsystems, and sets up the battle state.

| Item | Value |
| --- | --- |
| Argument | `r3` = pointer to battle configuration |
| Allocation | 28520 bytes via `BattleAlloc` |
| Calls | `0x800f8934` (BattleAlloc), `0x800050b4` (heapZero), plus 10+ initialization functions |
| Side effects | Clears ~20 fields, initializes subsystems |
| Return | None (void) |

## Interpretation

This is the main battle system initialization function. It allocates and zeroes the battle work structure, then calls a series of subsystem initializers to set up units, weapons, items, and other battle data. The function clears approximately 20 fields in the battle work structure and initializes the battle flags to 0.

## Remaining gaps

- Full layout of the battle work structure
- Implementation of all called initialization functions
- Caller graph
- Relationship to battle start and configuration
