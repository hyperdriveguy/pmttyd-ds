# BattleInformationSetResult

**Address:** `0x80114508`  
**Size:** 8 bytes (2 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
80114508: 90 83 00 10  	stw 4, 16(3)
8011450c: 4e 80 00 20  	blr
```

## Behavior

`BattleInformationSetResult` is a trivial field setter. It stores the value
in register 4 to byte offset 16 of the pointer in register 3, then returns.

| Item | Value |
| --- | --- |
| Argument | `r3` = pointer to a battle information structure |
| Argument | `r4` = value to store (the battle result) |
| Side effect | `[r3 + 16] = r4` (32-bit word store) |
| Return | None (void) |
| Calls | None |

## Interpretation

This function sets the **result** field of a battle information record. The
battle information structure tracks the outcome of a battle encounter
(victory, defeat, escape, etc.). The setter is the write counterpart to
`BattleInformationGetResult`.

## Remaining gaps

- Full structure layout of the battle information record
- Caller graph (who sets the result and when)
- Relationship to `BattleInformationGetResult`

## Verified producer/caller delivery (2026-10-13)

[Ending phase0 priority](BATTLE_RESULT_PHASE0_PUBLICATION_AND_PRIORITY.md)
now pins the common producer and all nine direct DOL setter requests. Complete
leaf/scoped ending interpretation does not close every record writer, ending
phase, override meaning or original tutorial execution.
