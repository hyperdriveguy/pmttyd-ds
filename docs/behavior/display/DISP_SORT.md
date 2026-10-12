# dispSort

**Address:** `0x80010b5c`  
**Size:** 52 bytes (13 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
80010b5c: 94 21 ff f0  	stwu 1, -16(1)
80010b60: 7c 08 02 a6  	mflr 0
80010b64: 3c 80 80 01  	lis 4, -32767
80010b68: 38 a0 00 04  	li 5, 4
80010b6c: 90 01 00 14  	stw 0, 20(1)
80010b70: 38 c4 0b 90  	addi 6, 4, 2960
80010b74: 80 6d 16 f4  	lwz 3, 5876(13)
80010b78: 80 8d 16 f8  	lwz 4, 5880(13)
80010b7c: 4b ff 59 81  	bl 0x800064fc
80010b80: 80 01 00 14  	lwz 0, 20(1)
80010b84: 7c 08 03 a6  	mtlr 0
80010b88: 38 21 00 10  	addi 1, 1, 16
80010b8c: 4e 80 00 20  	blr
```

## Behavior

`dispSort` is a wrapper that calls a sort function at `0x800064fc` with
the display work sort array, display work count, element size (4 bytes),
and a comparison function at `0x80010b90`.

| Item | Value |
| --- | --- |
| Sort array | `r3` = `[r13 + 0x16f4]` (display work sort array) |
| Element count | `r4` = `[r13 + 0x16f8]` (display work count) |
| Element size | `r5` = 4 |
| Comparator | `r6` = `0x80010b90` |
| Calls | `0x800064fc` (sort function) |
| Return | None (void) |

## Interpretation

This function sorts the display work array using a generic sort routine.
The display work array contains rendering commands or display list entries
that need to be ordered by depth or priority. The comparator at `0x80010b90`
defines the sort ordering.

## Remaining gaps

- Implementation of the sort function at `0x800064fc`
- Implementation of the comparator at `0x80010b90`
- Layout of the display work sort array
- Caller graph
