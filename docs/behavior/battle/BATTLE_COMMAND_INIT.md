# BattleCommandInit

**Address:** `0x80125380`  
**Size:** 24 bytes (6 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
80125380: 38 00 00 00  	li 0, 0
80125384: 90 03 17 1c  	stw 0, 5916(3)
80125388: 80 03 0e f4  	lwz 0, 3828(3)
8012538c: 60 00 00 80  	ori 0, 0, 128
80125390: 90 03 0e f4  	stw 0, 3828(3)
80125394: 4e 80 00 20  	blr
```

## Behavior

`BattleCommandInit` clears a field at offset 5916 and sets bit 7
(mask `0x80`) in a flags field at offset 3828 of the battle command
structure.

| Item | Value |
| --- | --- |
| Argument | `r3` = pointer to a battle command structure |
| Side effect 1 | `[r3 + 5916] = 0` (clear field) |
| Side effect 2 | `[r3 + 3828] |= 0x80` (set bit 7) |
| Return | None (void) |
| Calls | None |

## Interpretation

This function initializes a battle command structure. It clears a
sub-state or counter field at offset 5916, then sets bit 7 of the
flags field at offset 3828. Bit 7 likely indicates that the command
is active or has been initialized. This is called when a new action
command begins execution.

## Remaining gaps

- Full layout of the battle command structure
- Meaning of bit 7 at offset 3828
- Meaning of the field at offset 5916
- Caller graph
