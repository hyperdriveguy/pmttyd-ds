# windowReInit

**Address:** `0x8007fb94`  
**Size:** 68 bytes (17 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
8007fb94: 80 6d 1b 08  	lwz 3, 6920(13)
8007fb98: 38 00 00 00  	li 0, 0
8007fb9c: b0 03 00 02  	sth 0, 2(3)
8007fba0: 90 03 00 28  	stw 0, 40(3)
8007fba4: b0 03 00 4a  	sth 0, 74(3)
8007fba8: 90 03 00 70  	stw 0, 112(3)
8007fbac: b0 03 00 92  	sth 0, 146(3)
8007fbb0: 90 03 00 b8  	stw 0, 184(3)
8007fbb4: b0 03 00 da  	sth 0, 218(3)
8007fbb8: 90 03 01 00  	stw 0, 256(3)
8007fbbc: b0 03 01 22  	sth 0, 290(3)
8007fbc0: 90 03 01 48  	stw 0, 328(3)
8007fbc4: b0 03 01 6a  	sth 0, 362(3)
8007fbc8: 90 03 01 90  	stw 0, 400(3)
8007fbcc: b0 03 01 b2  	sth 0, 434(3)
8007fbd0: 90 03 01 d8  	stw 0, 472(3)
8007fbd4: 4e 80 00 20  	blr
```

## Behavior

`windowReInit` clears all 7 window records in the window array. Each
record is 72 bytes, and the function zeroes the state field (offset 2,
halfword) and a data field (offset 40, word) for each record.

| Item | Value |
| --- | --- |
| Window array base | `[r13 + 0x1b08]` |
| Records cleared | 7 (indices 0-6) |
| Bytes per record | 72 (`0x48`) |
| Fields cleared | State (offset 2, 16-bit) and data (offset 40, 32-bit) |
| Return | None (void) |
| Calls | None |

## Interpretation

This function resets the window system to its initial state. It clears
the active flag and data pointer for all 7 window records, effectively
closing all windows. The stride between records is `0x48` bytes, and the
function unrolls all 7 iterations.

## Remaining gaps

- Full layout of the 72-byte window record structure
- Meaning of the data field at offset 40
- Caller graph
