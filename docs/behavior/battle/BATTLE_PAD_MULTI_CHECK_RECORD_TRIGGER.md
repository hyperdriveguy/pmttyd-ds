# BattlePadMultiCheckRecordTrigger

**Address:** `0x8011a008`  
**Size:** 32 bytes (8 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
8011a008: 1c 63 01 fc  	mulli 3, 3, 508
8011a00c: 54 80 10 3a  	slwi 0, 4, 2
8011a010: 80 8d 1c 70  	lwz 4, 7280(13)
8011a014: 38 63 0f 44  	addi 3, 3, 3908
8011a018: 7c 03 02 14  	add 0, 3, 0
8011a01c: 7c 04 00 2e  	lwzx 0, 4, 0
8011a020: 7c 03 28 38  	and 3, 0, 5
8011a024: 4e 80 00 20  	blr
```

## Behavior

`BattlePadMultiCheckRecordTrigger` loads a word from the battle pad array at a computed offset and extracts bits 0-1 (mask `0x05`).

| Item | Value |
| --- | --- |
| Argument | `r3` = pad index |
| Argument | `r4` | second index (shifted left by 2) |
| Computation | `padArray + index * 508 + 3908 + secondIndex * 4` (word load) |
| Return | `r3` = `word & 0x05` (nonzero if bits 0-1 are set) |
| Side effects | None |
| Calls | None |

## Interpretation

This function checks a specific flag in the battle pad array. The pad array has 508-byte records, and the offset 3908 (`0x0f44`) selects a specific field within the record. The second index (shifted left by 2) selects a word within an array at that field. Bits 0-1 of the selected word indicate whether the "record trigger" multi-check condition is met.

## Remaining gaps

- Full layout of the 508-byte pad record structure
- Meaning of bits 0-1 at offset 3908
- Caller graph
- Relationship to `BattlePadMultiCheckNow` and `BattlePadMultiCheckTrigger`
