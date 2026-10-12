# windowInit

**Address:** `0x8007fd84`  
**Size:** 112 bytes (28 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
8007fd84: 94 21 ff f0  	stwu 1, -16(1)
8007fd88: 7c 08 02 a6  	mflr 0
8007fd8c: 38 60 00 00  	li 3, 0
8007fd90: 38 80 01 f8  	li 4, 504
8007fd94: 90 01 00 14  	stw 0, 20(1)
8007fd98: 4b fb 03 59  	bl 0x800300f0
8007fd9c: 90 6d 1b 08  	stw 3, 6920(13)
8007fda0: 38 00 00 00  	li 0, 0
8007fda4: b0 03 00 02  	sth 0, 2(3)
8007fda8: 90 03 00 28  	stw 0, 40(3)
8007fdac: b0 03 00 4a  	sth 0, 74(3)
8007fdb0: 90 03 00 70  	stw 0, 112(3)
8007fdb4: b0 03 00 92  	sth 0, 146(3)
8007fdb8: 90 03 00 b8  	stw 0, 184(3)
8007fdbc: b0 03 00 da  	sth 0, 218(3)
8007fdc0: 90 03 01 00  	stw 0, 256(3)
8007fdc4: b0 03 01 22  	sth 0, 290(3)
8007fdc8: 90 03 01 48  	stw 0, 328(3)
8007fdcc: b0 03 01 6a  	sth 0, 362(3)
8007fdd0: 90 03 01 90  	stw 0, 400(3)
8007fdd4: b0 03 01 b2  	sth 0, 434(3)
8007fdd8: 90 03 01 d8  	stw 0, 472(3)
8007fddc: 90 0d 1a f8  	stw 0, 6904(13)
8007fde0: 90 0d 1a fc  	stw 0, 6908(13)
8007fde4: 80 01 00 14  	lwz 0, 20(1)
8007fde8: 7c 08 03 a6  	mtlr 0
8007fdec: 38 21 00 10  	addi 1, 1, 16
8007fdf0: 4e 80 00 20  	blr
```

## Behavior

`windowInit` allocates the window array (504 bytes = 7 records x 72 bytes)
using `heapAlloc`, stores the pointer in SDA, clears all 7 window records,
and zeroes two additional SDA globals.

| Item | Value |
| --- | --- |
| Allocation | 504 bytes (7 x 72), via `0x800300f0` (heapAlloc) |
| Window array pointer | Stored at `[r13 + 0x1b08]` |
| Records cleared | 7 (state at offset 2, data at offset 40) |
| Additional globals cleared | `[r13 + 0x1af8]` and `[r13 + 0x1afc]` |
| Return | None (void) |
| Calls | `0x800300f0` (heapAlloc) |

## Interpretation

This function initializes the window system by allocating the window
array and clearing all records. The two additional SDA globals at
`0x1af8` and `0x1afc` are likely window-related counters or state
variables that are also reset during initialization.

## Remaining gaps

- Meaning of the globals at `0x1af8` and `0x1afc`
- Full layout of the 72-byte window record structure
- Caller graph
