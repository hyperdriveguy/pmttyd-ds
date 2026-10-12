# BattleACPadCheckRecordTrigger

**Address:** `0x800fa1a0`  
**Size:** 24 bytes (6 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
800fa1a0: 80 ad 1c 70  	lwz 5, 7280(13)
800fa1a4: 54 60 10 3a  	slwi 0, 3, 2
800fa1a8: 7c 65 02 14  	add 3, 5, 0
800fa1ac: 80 03 1d 34  	lwz 0, 7476(3)
800fa1b0: 7c 03 20 38  	and 3, 0, 4
800fa1b4: 4e 80 00 20  	blr
```

## Behavior

`BattleACPadCheckRecordTrigger` loads a word from the battle pad array
at a computed offset and extracts bit 2 (mask `0x04`).

| Item | Value |
| --- | --- |
| Argument | `r3` = pad index |
| Computation | `battleWork + index * 4 + 7476` (word load) |
| Return | `r3` = `word & 0x04` (nonzero if bit 2 is set) |
| Side effects | None |
| Calls | None |

## Interpretation

This function checks a specific flag in the battle pad array. The offset
is computed as `index * 4 + 7476`, suggesting an array of 32-bit words
within the battle work structure. Bit 2 of the selected word indicates
whether the "record trigger" condition is met. This is likely used for
input timing validation during action commands.

## Remaining gaps

- Full layout of the battle work structure
- Meaning of bit 2 at offset 7476
- Caller graph
- Relationship to `BattlePadMultiCheckNow` and `BattlePadMultiCheckTrigger`
