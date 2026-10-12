# BattlePadMultiCheckTrigger

**Address:** `0x8011a054`  
**Size:** 24 bytes (6 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
8011a054: 1c 63 01 fc  	mulli 3, 3, 508
8011a058: 80 ad 1c 70  	lwz 5, 7280(13)
8011a05c: 38 03 0f 44  	addi 0, 3, 3908
8011a060: 7c 05 00 2e  	lwzx 0, 5, 0
8011a064: 7c 03 20 38  	and 3, 0, 4
8011a068: 4e 80 00 20  	blr
```

## Behavior

`BattlePadMultiCheckTrigger` loads a word from the battle pad array at
a computed offset and extracts bit 2 (mask `0x04`).

| Item | Value |
| --- | --- |
| Argument | `r3` = pad index |
| Computation | `padArray + index * 508 + 3908` (word load) |
| Return | `r3` = `word & 0x04` (nonzero if bit 2 is set) |
| Side effects | None |
| Calls | None |

## Interpretation

This function checks a specific flag in the battle pad array. The pad
array has 508-byte records, and the offset 3908 (`0x0f44`) selects a
specific field within the record. Bit 2 of that field indicates whether
the "trigger" multi-check condition is met. This is similar to
`BattlePadMultiCheckNow` but checks a different field offset.

## Remaining gaps

- Full layout of the 508-byte pad record structure
- Meaning of bit 2 at offset 3908
- Caller graph
- Relationship to `BattlePadMultiCheckNow`
