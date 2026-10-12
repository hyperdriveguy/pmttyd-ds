# windowCheckID

**Address:** `0x8007ad50`  
**Size:** 24 bytes (6 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
8007ad50: 1c 63 00 48  	mulli 3, 3, 72
8007ad54: 80 8d 1b 08  	lwz 4, 6920(13)
8007ad58: 38 03 00 02  	addi 0, 3, 2
8007ad5c: 7c 04 02 2e  	lhzx 0, 4, 0
8007ad60: 54 03 07 bc  	rlwinm 3, 0, 0, 30, 31
8007ad64: 4e 80 00 20  	blr
```

## Behavior

`windowCheckID` reads the 16-bit state field at offset 2 of the specified
window record and extracts bit 1 (mask `0x02`).

| Item | Value |
| --- | --- |
| Argument | `r3` = window index |
| Computation | `windowArray + index * 72 + 2` (halfword load) |
| Return | `r3` = `(state >> 0) & 0x02` (nonzero if bit 1 is set) |
| Side effects | None |
| Calls | None |

## Interpretation

This function checks whether a window is **active**. The state field at
offset 2 of the window record uses bit 1 as the active flag. A nonzero
return value means the window is currently active.

## Remaining gaps

- Full layout of the 72-byte window record structure
- Meaning of other bits in the state field
- Caller graph
