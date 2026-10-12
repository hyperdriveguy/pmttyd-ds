# battleAcDelete_StickKeepLeft

**Address:** `0x80129fc0`  
**Size:** 12 bytes (3 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
80129fc0: 38 00 03 ed  	li 0, 1005
80129fc4: 90 03 1c 9c  	stw 0, 7324(3)
80129fc8: 4e 80 00 20  	blr
```

## Behavior

`battleAcDelete_StickKeepLeft` is a trivial field setter. It stores the
constant value 1005 (`0x3ed`) to byte offset 7324 (`0x1c9c`) of the
pointer in register 3, then returns.

| Item | Value |
| --- | --- |
| Argument | `r3` = pointer to a battle AC result structure |
| Side effect | `[r3 + 7324] = 1005` (32-bit word store) |
| Return | None (void) |
| Calls | None |

## Interpretation

This function sets a field in the battle action command result structure
to the constant 1005. The name suggests it relates to the "StickKeepLeft"
action command's delete result — the Hammer charge input state. The value
1005 may be a command ID, a frame count, or a result code.

## Remaining gaps

- Exact semantic meaning of the `+0x1c9c` field
- Meaning of the constant 1005
- Full structure layout of the AC result record
- Caller graph
