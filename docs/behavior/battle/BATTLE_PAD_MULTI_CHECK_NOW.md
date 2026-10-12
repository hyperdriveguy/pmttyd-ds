# BattlePadMultiCheckNow

**Address:** `0x80119fc8`  
**Size:** 24 bytes (6 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
80119fc8: 1c 63 01 fc  	mulli 3, 3, 508
80119fcc: 80 ad 1c 70  	lwz 5, 7280(13)
80119fd0: 38 03 10 34  	addi 0, 3, 4148
80119fd4: 7c 05 00 2e  	lwzx 0, 5, 0
80119fd8: 7c 03 20 38  	and 3, 0, 4
8011fdc: 4e 80 00 20  	blr
```

## Behavior

`BattlePadMultiCheckNow` loads a word from the battle pad array at a
computed offset and extracts bit 2 (mask `0x04`).

| Item | Value |
| --- | --- |
| Argument | `r3` = pad index |
| Computation | `padArray + index * 508 + 4148` (word load) |
| Return | `r3` = `word & 0x04` (nonzero if bit 2 is set) |
| Side effects | None |
| Calls | None |

## Interpretation

This function checks a specific flag in the battle pad array. The pad
array has 508-byte records, and the offset 4148 (`0x1034`) selects a
specific field within the record. Bit 2 of that field indicates whether
the "now" (current frame) multi-check condition is met. This is likely
used for input timing validation during action commands.

## Remaining gaps

- Full layout of the 508-byte pad record structure
- Meaning of bit 2 at offset 4148
- Caller graph
- Relationship to `BattlePadMultiCheckTrigger`
