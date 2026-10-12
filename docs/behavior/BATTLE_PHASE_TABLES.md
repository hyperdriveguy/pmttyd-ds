# Phase actor tables, ordering and move consumption

US G8ME01 revision0, inspected 2026-10-12. B evidence is original DOL instructions
and data. See [baseline](../ORIGINAL_BEHAVIOR.md), [move counts](BATTLE_MOVE_COUNTS.md)
and [event barriers](BATTLE_WAITS_AND_CONCLUSION.md). No original execution or
Switch comparison was performed. Extracts/disassembly remain ignored.
This covers phase table helpers, phase wrapper, and selected actor-selection/
consumption regions. The full btlseqMove and btlseqAct controllers remain open.

## Actor table construction (B)

btlseqPhaseFirstProcess at0x8011b0d0,size0x11c receives battle pointer B.
Clear word B+0x1c70 and byte B+0x1c74. Initialize64 word actor IDs at B+0x120
through+0x21c to−1. Then scan unit slots0..63 ascending:

1. Skip null unit; otherwise write byte255 at unit+0x311, even if it is excluded
   by later checks.
2. If unsigned unit byte+0x20 equals10, require CheckStatus(unit,20)≠0.
   Other state-byte values bypass this gate.
3. Require word unit+0x2c equals BattleGetSeq(B,4), fetched for this unit.
4. Require attributes word unit+0x104 lacks0x80000.
5. Append unit's word ID+0 to the actor table; write unit+0x140=−1.
6. Traverse its part list from unit+0x14 via next pointer part+0. For each part
   whose word+0x1c0≠−1, write byte1 at part+0x4e4.

There is no explicit remaining-count, HP, status27 or allegiance check in this
constructor. Slow can keep a state10 unit in the table for downstream handling.
No part-list bound/cycle test exists locally. After the scan, call
battleSortPhaseMoveTable(B,appendedCount), then battleMakePhaseEvtTable(B).
Their two tables have different membership and ID sources below.

## Actor ordering (B)

battleSortPhaseMoveTable at0x8011af44,size0x18c receives(B,count).
If signed count≤0, count contiguous IDs in B+0x120 until the first−1, up to64.
A positive count is used directly, with no local≤64 cap. Entries must resolve
to valid units for comparisons; no null guards occur in either sort.

Perform two complete adjacent bubble-sort passes:

1. Sort ascending by abs(homeX), obtained through BtlUnit_GetHomePos for both
   units on each comparison. Swap only when left absX>right absX.
2. Sort descending by signed word unit+0x28. Swap only when left priority<right.

Each pass executes shrinking adjacent comparisons; equal keys do not swap.
For finite positions, the second stable pass makes priority the primary key,
absolute home X the secondary key and the original table order the remaining
tie order. Do not sort by current position, signed X, unit ID, or a presumed
combined comparator with different tie handling. The home helper at0x801289b0,
size0x1c reads floats unit+0x30,+0x34,+0x38 into its three outputs.
Floating comparisons use fcmpo; unordered/nonfinite inputs require their source
semantics rather than an assumed host total order.

## Phase-event table (B)

battleMakePhaseEvtTable at0x8011adec,size0x158 initializes64 pairs at B+0x220
with(−1,0). Scan all64 unit slots. Skip null units and attributes0x80000;
there is no actor-table membership, phase word, state-byte, count or HP filter.
Append(slotIndex,key), using the **lookup slot index**, rather than unit+0 ID.

Get home X. If X≥0, key=truncToWord(singlePrecision(X+1000.0f)); otherwise
key=truncToWord(abs(X)). Sort pairs ascending by signed key using adjacent
bubble sort, swapping whole pairs only when left key>right key. Equal keys
retain slot order. Constants are0x80422568=0.0f and0x80422580=1000.0f.
NaN/unordered X follows the original fcmpo/branch and conversion behavior;
no gameplay meaning is assigned to invalid coordinates here.

This produces a separate side/position ordering; actor priority does not enter
this sort. The move controller later scans these pairs for phase-event dispatch,
so merging this table with the actor list would change eligibility and order.

## Phase wrapper and immediate move transitions (B)

btlseqPhase at0x8011c2b4,size0x9c reads channel4:

- Exactly0x04000000: increment channel4, set channel5=0x05000000, then exit.
- Values0x04000001..0x04000005: call btlseqMove(B), then exit.
- Exactly0x04000006: increment channel3, reset channel4=0x04000000, then exit.
- Outside that range: exit without a local transition.

btlseqMove uses channel5−0x05000000 as an unsigned index0..14 through15 words
at0x80363c8c. Its local loop at0x8011c298 redispatches immediately only when
its transition flag is nonzero; otherwise it exits. State0 invokes phase-first
processing, advances channel5 and requests immediate redispatch. Other state's
complete control flow and actor-event interaction remain a separate recovery.
The table is fingerprinted; its hash identifies bytes, not semantic coverage.

## Actor selection region (`0x8011bf34`–`0x8011bfec`, B)

This region of the move controller scans the64 actor table entries in order.
For each entry, skip ID−1, missing unit, signed remaining byte+0x22≤0, and
signed state+0x20=10. If status27 is active, require attribute0x10000; otherwise
skip. This is a different status27 policy from BattlePhaseEndCheck below.

If an otherwise eligible unit has state1, clear state to0 and continue scanning.
Otherwise select it: store its table ID at B+0x420, OR0x1000 into unit+0x27c,
increment channel5 and set channel6=0x06000000. This selected region does not
call CanActStatus. Later action/status handling is not inferred from selection.
The all64-skipped path and valid-actor invariant require the surrounding complete
move controller; do not implement selection in isolation as a null-safe API.

## Count-consumption region (`0x8011b8b0`–`0x8011ba18`, B)

Within btlseqAct, unit U has been fetched from B+0x420. This documents the local
region after preceding action services; its exact entry scheduling remains open.
First normalize signed state byte+0x20:5→6,6→7,7→0,8→9,9→0; others unchanged.
Then branch on the resulting state:

- State0: the local r3=1 test at0x8011b920..b934 is always true; do not turn it
  into an extra condition based on battle flags. If status flag U+0x138 has
  0x02000000, clear it and skip decrement. Otherwise decrement unsigned byte
  U+0x22 and store8. Then if B+0xef4 has0x2000, force remaining count0.
- If the resulting signed remaining count≤0, set state10. If U+0x27c lacks
  0x2000, clear byte U+0x24; otherwise clear that0x2000 bit, leaving+24 intact.
- Other states: resulting3 or4 is cleared to0 without decrement; all other
  nonzero states skip the count-consumption block.

The decrement stores before the signed≤0 test; there is no preliminary clamp.
An input remaining0 becomes255(signed−1) then receives state10.
This helper region supplies a+24 reset missing from the turn-first reset
contract, but full Slow cadence still depends on entry/state producers.

On the exhausted path, further code may write U+0x311=96 if signed membership
returned by BtlUnit_GetBelong is0, U+0x2c=0x04000002, BattlePhaseEndCheck=0,
and battle flags lack0x2000. This is a display/state write, not another move.
The following common tail calls BattleConsumeReserveItem before advancing the
action sequence. Its full post-action event/stage behavior remains open.

## Phase-end predicate (B)

BattlePhaseEndCheck at0x8011bb88,size0x9c uses global battle B, scanning all64
actor IDs at+0x120. Return0 on the first entry resolving to a unit with unsigned
state+0x20≠10, signed remaining+0x22>0 and CheckStatus(unit,27)=0. Otherwise
return1 after all64 entries. Missing entries/units are skipped.
It does not test CanActStatus, HP, phase word or the status27 exception attribute
used in selection. A predicate named phase-end is therefore not equivalent to
counting every selectable actor under a merged eligibility rule.

## Differential obligations and remaining work (P)

Compare equal priorities at different absolute/signed home X, all equal keys,
phase-table count0/−1 counting, an actor-table gap, Slow state10 inclusion,
excluded attribute and part writes, actor ID differing from lookup slot,
phase-event side key truncation, state1 deferred selection, status27 selection
exception versus phase-end rejection, count decrement bypass flag, exhausted
count clearing+24 versus preserving it, and reserved-item consumption timing.
These are proposed comparisons, not executed-original observations.

Remaining: complete move/action controllers and state entry conditions, command/
attack services, phase-event bodies, state-byte/flag writers, table ID invariants,
outer turn progression and original execution traces. This closes local table
and consumption details; complete battle/game equivalence is still unproven.
