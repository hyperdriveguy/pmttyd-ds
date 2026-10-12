# dispCalcZ

**Address:** `0x8001086c`  
**Size:** 148 bytes (37 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
8001086c: 94 21 ff e0  	stwu 1, -32(1)
80010870: 7c 08 02 a6  	mflr 0
80010874: 90 01 00 24  	stw 0, 36(1)
80010878: 93 e1 00 1c  	stw 31, 28(1)
8001087c: 93 c1 00 18  	stw 30, 24(1)
80010880: 7c 7e 1b 78  	mr 30, 3
80010884: 38 60 00 04  	li 3, 4
80010888: 4b ff ea 75  	bl 0x8000f2fc
8001088c: 7c 7f 1b 78  	mr 31, 3
80010890: 7f c4 f3 78  	mr 4, 30
80010894: 38 7f 01 1c  	addi 3, 31, 284
80010898: 38 a1 00 08  	addi 5, 1, 8
8001089c: 48 28 de 09  	bl 0x8029e6a4
800108a0: 38 81 00 08  	addi 4, 1, 8
800108a4: 38 7f 01 5c  	addi 3, 31, 348
800108a8: 7c 85 23 78  	mr 5, 4
800108ac: 48 28 e2 5d  	bl 0x8029eb08
800108b0: c0 42 83 20  	lfs 2, -31968(2)
800108b4: c0 21 00 10  	lfs 1, 16(1)
800108b8: c0 02 83 24  	lfs 0, -31964(2)
800108bc: ec 22 10 7a  	fmadds 1, 2, 1, 2
800108c0: fc 01 00 40  	fcmpo 0, 1, 0
800108c4: 40 80 00 0c  	bf 0, 0x800108d0
800108c8: fc 20 00 90  	fmr 1, 0
800108cc: 48 00 00 14  	b 0x800108e0
800108d0: c0 02 83 28  	lfs 0, -31960(2)
800108d4: fc 01 00 40  	fcmpo 0, 1, 0
800108d8: 40 81 00 08  	bf 1, 0x800108e0
800108dc: fc 20 00 90  	fmr 1, 0
800108e0: c0 02 83 2c  	lfs 0, -31956(2)
800108e4: 80 01 00 24  	lwz 0, 36(1)
800108e8: ec 21 00 32  	fmuls 1, 1, 0
800108ec: 83 e1 00 1c  	lwz 31, 28(1)
800108f0: 83 c1 00 18  	lwz 30, 24(1)
800108f4: 7c 08 03 a6  	mtlr 0
800108f8: 38 21 00 20  	addi 1, 1, 32
800108fc: 4e 80 00 20  	blr
```

## Behavior

`dispCalcZ` computes a Z-depth value for display sorting. It:
1. Calls `0x8000f2fc` with argument 4 (likely getting a camera or
   display context)
2. Calls `0x8029e6a4` with a matrix at offset 284 and a vector at
   stack+8 (likely a matrix-vector multiply)
3. Calls `0x8029eb08` with a result at offset 348 (likely another
   transformation)
4. Computes `result = constant1 * input + constant2` using `fmadds`
5. Clamps the result between two constants using `fcmpo` + `fmr`
6. Multiplies by a final scale constant

| Item | Value |
| --- | --- |
| Argument | `r3` = input value (float) |
| Calls | `0x8000f2fc`, `0x8029e6a4`, `0x8029eb08` |
| Return | `f1` = clamped and scaled Z value |
| Constants | Four floats at `0x80428320`-`0x8042832c` (via r2) |

## Interpretation

This function calculates a Z-depth value for display list sorting. It
transforms the input through a camera/view matrix, clamps the result
to a valid range, and applies a final scaling factor. The constants
at `0x80428320`-`0x8042832c` define the transformation and clamping
bounds.

## Remaining gaps

- Exact values of the four float constants
- Implementation of the called functions
- Caller graph
- Relationship to `dispSort`
