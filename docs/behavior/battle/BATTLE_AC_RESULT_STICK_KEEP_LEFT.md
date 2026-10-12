# battleAcResult_StickKeepLeft

**Address:** `0x8012a174`  
**Size:** 8 bytes (2 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
8012a174: 80 63 1c b8  	lwz 3, 7352(3)
8012a178: 4e 80 00 20  	blr
```

## Behavior

`battleAcResult_StickKeepLeft` is a trivial field getter. It loads the
32-bit word at byte offset 7352 (`0x1cb8`) from the pointer in register 3
and returns it in register 3.

| Item | Value |
| --- | --- |
| Argument | `r3` = pointer to a battle AC (action command) result structure |
| Return | `r3` = word at `[r3 + 7352]` |
| Side effects | None |
| Calls | None |

## Interpretation

This function reads a field from the battle action command result structure.
The offset `0x1cb8` is large, suggesting this is a field in a complex
sub-structure or an array element. The name suggests it relates to the
"StickKeepLeft" action command result — the Hammer charge input state where
the player holds left on the control stick.

## Remaining gaps

- Exact semantic meaning of the `+0x1cb8` field
- Full structure layout of the AC result record
- Caller graph
- Relationship to the Hammer weapon branch system
