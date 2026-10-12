# dispInit

**Address:** `0x80010c70`  
**Size:** 100 bytes (25 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
80010c70: 94 21 ff f0  	stwu 1, -16(1)
80010c74: 7c 08 02 a6  	mflr 0
80010c78: 38 60 00 00  	li 3, 0
80010c7c: 38 80 40 00  	li 4, 16384
80010c80: 90 01 00 14  	stw 0, 20(1)
80010c84: 48 01 f4 6d  	bl 0x800300f0
80010c88: 90 6d 16 f0  	stw 3, 5872(13)
80010c8c: 38 60 00 00  	li 3, 0
80010c90: 38 80 10 00  	li 4, 4096
80010c94: 48 01 f4 5d  	bl 0x800300f0
80010c98: 90 6d 16 f4  	stw 3, 5876(13)
80010c9c: 38 80 00 00  	li 4, 0
80010ca0: 80 6d 16 f0  	lwz 3, 5872(13)
80010ca4: 38 a0 40 00  	li 5, 16384
80010ca8: 4b ff 44 0d  	bl 0x800050b4
80010cac: 80 6d 16 f4  	lwz 3, 5876(13)
80010cb0: 38 80 00 00  	li 4, 0
80010cb4: 38 a0 10 00  	li 5, 4096
80010cb8: 4b ff 43 fd  	bl 0x800050b4
80010cbc: 38 00 00 00  	li 0, 0
80010cc0: 90 0d 16 f8  	stw 0, 5880(13)
80010cc4: 80 01 00 14  	lwz 0, 20(1)
80010cc8: 7c 08 03 a6  	mtlr 0
80010ccc: 38 21 00 10  	addi 1, 1, 16
80010cd0: 4e 80 00 20  	blr
```

## Behavior

`dispInit` allocates two display work buffers using `heapAlloc` (at
`0x800300f0`), zeroes them using `heapZero` (at `0x800050b4`), and
initializes the display work count to zero.

| Item | Value |
| --- | --- |
| Buffer 1 | 16384 bytes, stored at `[r13 + 0x16f0]` |
| Buffer 2 | 4096 bytes, stored at `[r13 + 0x16f4]` |
| Count | Stored at `[r13 + 0x16f8]`, initialized to 0 |
| Calls | `0x800300f0` (heapAlloc) x2, `0x800050b4` (heapZero) x2 |
| Return | None (void) |

## Interpretation

This function initializes the display subsystem by allocating two work
buffers and clearing them. The first buffer (16KB) is the display work
array, and the second (4KB) is the sort array. Both are zeroed before
use. The display work count starts at 0.

## Remaining gaps

- Implementation of `heapAlloc` at `0x800300f0`
- Implementation of `heapZero` at `0x800050b4`
- Caller graph
