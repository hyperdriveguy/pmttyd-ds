# pouchSetMaxFP

**Address:** `0x800d3c50`  
**Size:** 24 bytes (6 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
800d3c50: 80 8d 1b e0  	lwz 4, 7136(13)
800d3c54: b0 64 00 76  	sth 3, 118(4)
800d3c58: 80 6d 1b e0  	lwz 3, 7136(13)
800d3c5c: a8 03 00 76  	lha 0, 118(3)
800d3c60: b0 03 00 90  	sth 0, 144(3)
800d3c64: 4e 80 00 20  	blr
```

## Behavior

`pouchSetMaxFP` stores the supplied 16-bit value to the pouch state's
max FP field, then reads it back and stores it to a second location.

| Item | Value |
| --- | --- |
| Argument | `r3` = max FP value (16-bit) |
| Side effect 1 | `[pouch + 118] = r3` (store max FP) |
| Side effect 2 | `[pouch + 144] = sign_extend([pouch + 118])` (copy to second field) |
| Return | None (void) |
| Calls | None |

## Interpretation

This function sets the maximum FP (Flower Points) in the pouch state. The
value is stored at offset 118 (`0x76`) and then read back with sign
extension and stored at offset 144 (`0x90`). The second field may be a
shadow copy or a derived value used by a different subsystem.

## Remaining gaps

- Full layout of the pouch state structure
- Purpose of the second field at offset 144
- Caller graph
