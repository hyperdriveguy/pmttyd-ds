# BattleInformationGetResult

**Address:** `0x80114500`  
**Size:** 8 bytes (2 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
80114500: 80 63 00 10  	lwz 3, 16(3)
80114504: 4e 80 00 20  	blr
```

## Behavior

`BattleInformationGetResult` is a trivial field getter. It loads the 32-bit
word at byte offset 16 from the pointer in register 3 and returns it in
register 3.

| Item | Value |
| --- | --- |
| Argument | `r3` = pointer to a battle information structure |
| Return | `r3` = word at `[r3 + 16]` (the battle result) |
| Side effects | None |
| Calls | None |

## Interpretation

This function reads the **result** field of a battle information record. The
battle information structure tracks the outcome of a battle encounter
(victory, defeat, escape, etc.). The getter is the read counterpart to
`BattleInformationSetResult`.

## Remaining gaps

- Full structure layout of the battle information record
- Caller graph (who reads the result and when)
- Relationship to `BattleInformationSetResult`

## Verified producer/caller delivery (2026-10-13)

[Ending phase0 priority](BATTLE_RESULT_PHASE0_PUBLICATION_AND_PRIORITY.md)
now pins the common producer and all nine direct DOL setter requests. Complete
leaf/scoped ending interpretation does not close every record writer, ending
phase, override meaning or original tutorial execution.
