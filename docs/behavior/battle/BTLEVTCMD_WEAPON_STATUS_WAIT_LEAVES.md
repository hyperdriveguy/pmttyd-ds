# Battle event leaves: weapon level, AC difficulty, announce, queue, status window and wait

US G8ME01 disc0 revision0; static contracts, no execution, cadence or gameplay
claim. Closes `btlevtcmd_GetWeaponActionLv` (`0x801058e0`, `battle_event_cmd.o`,
size `0x48`, 24 lines), `btlevtcmd_AcSetDifficulty` (`0x80107b38`, size `0x38`,
38 lines), `btlevtcmd_AnnounceSetParam` (`0x801abc10`, size `0x30`, 30 lines),
`btlevtcmd_AfterReactionEntry` (`0x80105a78`, size `0x68`, 32 lines),
`btlevtcmd_StatusWindowOnOff` (`0x80103e34`, size `0x7c`, 37 lines) and
`btlevtcmd_StartWaitEvent` (`0x8010a26c`, size `0x64`, 31 lines) from the
[unaddressed register](../binary/UNADDRESSED_BINARY_FUNCTIONS.md). None has a
definition in the pinned snapshot (rechecked with `tools/reference_source_gap.py`).

Shared pipeline for all six: `args = word[entry+24]`, operand words are resolved
with `evtGetValue` (`0x80031260`), ids with `BattleTransID` (`0x801253ec`) and
units with `BattleGetUnitPtr` (`0x800f88cc`) from the shared battle pointer
`B = word[0x8041eb90]` (`r13+7280`). Every body returns 2 and ignores the return
of its callee; none has a first-call gate or null-unit check.

## btlevtcmd_GetWeaponActionLv (0x801058e0, 24 lines)

1. `src = evtGetValue(args[0])`.
2. `level = lbz [src+110]` (`+0x6E`), an unsigned byte read from the resolved
   pointer's structure.
3. `evtSetValue(entry, args[4], level)` (`0x80030cfc`).

The destination word is the **raw** second operand, not a resolved one — the
inverse of the usual resolved-destination pattern.

## btlevtcmd_AcSetDifficulty (0x80107b38, 38 lines)

1. `r30 = evtGetValue(args[0])` (unit id, kept across the next call).
2. `r29 = evtGetValue(args[4])` (difficulty value).
3. `unit = BattleGetUnitPtr(B, BattleTransID(entry, r30))`.
4. `BattleActionCommandSetDifficulty(B, unit, r29)` (`0x800fa12c`,
   `battle_ac.o`, 0x30 bytes) — arguments `r3 = B`, `r4 = unit`, `r5 = value`.

## btlevtcmd_AnnounceSetParam (0x801abc10, 30 lines)

1. `r30 = B + 0x18C8C` (`addis 2` + `addi -29556`): a records array inside the
   battle work with 8-byte stride.
2. `index = evtGetValue(args[0])`; `value = evtGetValue(args[4])`.
3. `word[r30 + 4*index + 4] = value` — the store is at `+4`, i.e. the value word
   of the indexed record, never at `+0`.

There is no bounds check on `index` and no sentinel test; the value word is
overwritten unconditionally.

## btlevtcmd_AfterReactionEntry (0x80105a78, 32 lines)

1. `key = BattleTransID(entry, evtGetValue(args[0]))`.
2. `value = evtGetValue(args[4])`.
3. `BattleAfterReactionEntry(key, value)` (`0x800f7a78`) — see
   [the queue contract](BATTLE_AFTER_REACTION_QUEUE.md). Its return (entry
   pointer for duplicate/full, record address otherwise) is discarded here.
4. Return 2.

## btlevtcmd_StatusWindowOnOff (0x80103e34, 37 lines)

`mode = evtGetValue(args[0])`, then a signed four-way dispatch:

| Resolved `mode` | Store |
| --- | --- |
| `1` | `word[B+10056]` (`+0x2748`) = 1 |
| `0` | `word[B+10056]` = 0 |
| `3` | `word[B+10052]` (`+0x2744`) = 1 |
| anything else (including `2` and negatives) | nothing |

Then `BattleStatusWindowCheck()` (`0x800f7514`) is always called and its boolean
return is discarded. Those two words are the same words that
[BattleStatusWindowCheck](BATTLE_STATUS_WINDOW_CHECK_AND_NOMOVE_X.md) tests in
steps 4 and 5 of its gate, both of which force the checkpoint result to `0`:
`mode 1` blocks the status window, `mode 0` clears that block, and `mode 3`
blocks it through the second word. That reading is a static inference from the
gate order, not an executed observation.

## btlevtcmd_StartWaitEvent (0x8010a26c, 31 lines)

1. `unit = BattleGetUnitPtr(B, BattleTransID(entry, evtGetValue(args[0])))`.
2. If `word[unit+652]` (`+0x28C`) is nonzero, call `BattleRunWaitEvent(unit)`
   (`0x801296e0`, `battle_unit_event.o`, 0x6c bytes); otherwise do nothing.
3. Return 2.

This is the counterpart to `btlevtcmd_StopWaitEvent`, which deletes the id
parked at `unit+656` and clears that slot. `+652` and `+656` are neighbouring
words in the same unit record, so the start/stop pair reads as
"run-if-registered / delete-and-clear".

## Not established

- Operand semantics (which slot is unit id, value or destination), caller census
  and cadence for all six.
- `BattleActionCommandSetDifficulty`, `BattleRunWaitEvent`, the record layout at
  `B+0x18C8C`, and the announced parameter index meaning.
- Any on-screen effect of the status-window block words.
