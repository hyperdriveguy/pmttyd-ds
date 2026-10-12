# BattleActionCommandStart

**Address:** `0x800fa450`  
**Size:** 12 bytes (3 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
800fa450: 38 00 00 64  	li 0, 100
800fa454: 90 03 1c 9c  	stw 0, 7324(3)
800fa458: 4e 80 00 20  	blr
```

## Behavior

`BattleActionCommandStart` is a trivial field setter. It stores the
constant value 100 (`0x64`) to byte offset 7324 (`0x1c9c`) of the
pointer in register 3, then returns.

| Item | Value |
| --- | --- |
| Argument | `r3` = pointer to a battle AC result structure |
| Side effect | `[r3 + 7324] = 100` (32-bit word store) |
| Return | None (void) |
| Calls | None |

## Interpretation

This function sets a field in the battle action command result structure
to the constant 100. The name suggests it marks the start of an action
command. The value 100 may be a command ID, a frame count, or a state
marker. This is the same field offset as `battleAcDelete_StickKeepLeft`
but with a different constant.

## Remaining gaps

- Exact semantic meaning of the `+0x1c9c` field
- Meaning of the constant 100
- Full structure layout of the AC result record
- Caller graph
