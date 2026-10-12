# pouchSetMaxHP

**Address:** `0x800d3ce8`  
**Size:** 24 bytes (6 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
800d3ce8: 80 8d 1b e0  	lwz 4, 7136(13)
800d3cec: b0 64 00 72  	sth 3, 114(4)
800d3cf0: 80 6d 1b e0  	lwz 3, 7136(13)
800d3cf4: a8 03 00 72  	lha 0, 114(3)
800d3cf8: b0 03 00 8e  	sth 0, 142(3)
800d3cfc: 4e 80 00 20  	blr
```

## Behavior

`pouchSetMaxHP` stores the supplied 16-bit value to the pouch state's
max HP field, then reads it back and stores it to a second location.

| Item | Value |
| --- | --- |
| Argument | `r3` = max HP value (16-bit) |
| Side effect 1 | `[pouch + 114] = r3` (store max HP) |
| Side effect 2 | `[pouch + 142] = sign_extend([pouch + 114])` (copy to second field) |
| Return | None (void) |
| Calls | None |

## Interpretation

This function sets the maximum HP in the pouch state. The value is stored
at offset 114 (`0x72`) and then read back with sign extension and stored
at offset 142 (`0x8e`). The second field may be a shadow copy or a
derived value used by a different subsystem.

## Remaining gaps

- Full layout of the pouch state structure
- Purpose of the second field at offset 142
- Caller graph
