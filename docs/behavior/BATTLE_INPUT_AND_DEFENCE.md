# Battle input history and defensive action commands

Baseline: US G8ME01 revision 0, DOL hash and evidence classes in
[ORIGINAL_BEHAVIOR.md](../ORIGINAL_BEHAVIOR.md). Updated 2026-10-12.
All numbered addresses below are US RAM virtual addresses. B denotes original
binary inspection; R denotes public reconstruction; P denotes a port/test
obligation. No Dolphin gameplay timing was measured for this document.

## Why this is an undecompiled behavior priority

At public doldecomp/ttyd revision `62131fc31866f85a12e4817a8dc37f0c25eb5f8c`,
`src/battle/battle_ac.c` implements `BattleActionCommandCheckDefence` only as a
constant `-2` return. The US binary contains 616 bytes at `0x800fa1b8` implementing
a materially different state machine. The entire `battle_ac.c` object is declared
NonMatching. This document specifies that missing routine from its instructions,
plus the input buffer behavior on which it depends.

The pinned [public source](https://github.com/doldecomp/ttyd/blob/62131fc31866f85a12e4817a8dc37f0c25eb5f8c/src/battle/battle_ac.c)
and [pad source](https://github.com/doldecomp/ttyd/blob/62131fc31866f85a12e4817a8dc37f0c25eb5f8c/src/battle/battle_pad.c)
are discovery aids. Evidence artifacts are ignored
`artifacts/reference/behavior/<function>.disasm` and `.elf`.

## Input state layout and update (B)

`BtlPad_WorkUpdate`, `0x8011a0ac`, consumes one channel's global input snapshot.
Its relative fields are:

| Offset | Meaning established by reads/writes |
| --- | --- |
| `0x000`, `0x00c` | Current and prior 12-byte pad snapshots |
| `0x018` | 30 words of newly pressed button history |
| `0x090` | 30 words of repeat-button history |
| `0x108` | 30 words of current/held button history |
| `0x180` | Current release mask; this function does not shift this array |
| `0x1f8`, `0x1f9` | Repeat phase and repeat counter, stored as bytes |

The shift loop (`0x8011a0b0`–`0x8011a0f0`) runs 29 iterations from byte offset
`0x70` downwards. For each of the three shifted arrays it copies old index 28
to 29, then 27 to 28, continuing through 0 to 1. It then writes the new index
0. Thus history index 0 is the latest update and index 29 the oldest retained
entry. A shift in the opposite direction would overwrite the evidence that
defense checks rely on.

**R/B disagreement:** the inspected public C loop uses source index `30-i`
and destination `29-i`. That text is not the US instruction behavior above.
Do not copy that loop as a faithful history implementation. This is a concrete
reason that source existence and a matching claim cannot be inferred from one
another.

The new snapshot takes the physical button field's low 16 bits and signed
main/C-stick coordinates from global input. Analog A/B fields become zero on
this path. The new pressed/repeat/released low-button masks come from the global
input producer, not a local recomputation from the physical-button snapshot.
That producer's sampling/edge rules are a separate contract still to recover.

The update adds virtual buttons with inclusive thresholds:

| Source | Test | Added mask |
| --- | --- | --- |
| Main Y | ≥ 30 / ≤ -30 | `0x00010000` / `0x00020000` |
| Main X | ≤ -30 / ≥ 30 | `0x00040000` / `0x00080000` |
| C-stick Y | ≥ 30 / ≤ -30 | `0x00100000` / `0x00200000` |
| C-stick X | ≤ -30 / ≥ 30 | `0x00400000` / `0x00800000` |

Virtual press edges are `current & ~previous & 0xffff0000`, ORed into the
global pressed mask (`0x8011a28c`–`0x8011a2a4`). Virtual release edges are
`previous & ~current & 0xffff0000`, ORed into the release mask at the end.
Holding a button continuously does not create another press edge here.

## Repeat behavior (B)

For an all-zero current combined mask, clear repeat output, repeat phase and
counter (`0x8011a2b4`–`0x8011a2c0`). For a changed nonzero combined mask,
emit the newly pressed virtual directions, preserve the global physical repeat
bits, set phase 1 and counter 0. For an unchanged nonzero mask, increment the
counter; phase 1's threshold is 16, and subsequent threshold is 8. On reaching
the threshold, OR the held virtual directions into repeat output, set phase 2
and reset the counter (`0x8011a310`–`0x8011a364`).

The comparison uses the whole combined mask. Adding a physical button while
holding a stick direction changes the mask and restarts this repeat phase.
The 16/8 delays govern virtual directions; the original global producer remains
responsible for physical repeat bits. Do not impose this loop on every UI button
without inspecting that producer and the particular menu's callers.

`BtlPad_WorkInit` (`0x8011a38c`) zeroes both pad snapshots, all 30 entries in
each of the three shifted arrays, and the repeat phase/counter. It does **not**
clear the release array in the inspected instruction range. This difference
should be retained in analysis even if an implementation initializes all memory
at object construction for safety.

## Defense input context and index

The defense routine uses battle work's dedicated action-command pad at
`battle+0x1d1c`, not necessarily the general menu pad. Its history reader,
`BattleACPadCheckRecordTrigger` (`0x800fa1a0`), returns
`word(battle+0x1d34+4*historyIndex) & buttonMask`.

It uses mask `0x100` (A) and `0x200` (B). The same entry with both A and B
counts as **two** occurrences in the anti-mashing scan. Held masks and repeat
masks are not used by this routine. The exact battle-loop placement of pad
updates relative to attack impact still needs tracing; history indices are not
automatically equivalent to video frames or a symmetric window around impact.

Defense difficulty is `clamp(3 + byte(unit+0x306) - byte(unit+0x305), 0, 6)`.
R names these fields Unsimplifier and Simplifier counts respectively. The loads,
arithmetic and clamp are B at `0x800fa200`–`0x800fa234`. This routine does not
read the stored attack action-command difficulty.

By comparison, `BattleActionCommandSetDifficulty` (`0x800fa12c`) stores the
supplied base difficulty in `battle+0x1cbc`, applies the same badge adjustment
to the **supplied base**, clamps to 0..6, and stores `battle+0x1cbd`.

## Defense windows (B)

Two seven-byte tables are loaded directly by the US routine:

| Difficulty index | A entries checked, table `0x802ee018` | B entries checked, table `0x802ee020` |
| --- | --- | --- |
| 0 | 10 | 5 |
| 1 | 9 | 4 |
| 2 | 8 | 3 |
| 3, no difficulty badges | 8 | 3 |
| 4 | 7 | 2 |
| 5 | 6 | 1 |
| 6 | 5 | 1 |

A window of 8 means history indices **0 through 7**, not 0 through 8. The
separate anti-mash scan includes indices **0 through 14** for both buttons.
These values supersede guesses about this function, but do not establish that
defense is enabled in the introductory Crump script.

## Complete local decision order (B)

Inputs: target unit pointer; second argument with byte at `+0x13` controlling
eligibility for the B/result-5 branch; action-command pad history; signed
halfword override counter at `battle+0x1d18`. Output is stored in
`battle+0x1cb0` and returned as an integer. Interpret the second argument's
byte as a gate until its full weapon layout/callers have been verified.

1. Clear result to 0 and compute the clamped difficulty and both windows.
2. Attempt the result-5 branch only if argument byte `+0x13 != 0` and signed
   unit HP at `+0x10c > 0` (`0x800fa238`–`0x800fa25c`).
3. Within that eligible branch, a positive override counter is decremented and
   returns result 5 through the common cleanup. Otherwise, unit byte `+0x307`
   being nonzero also selects 5. R calls that byte AutoCommand; its legitimate
   game writers are not yet established here.
4. Otherwise scan the B window. If any B edge exists, count each A and B edge
   separately across the 15-entry anti-mash span. A total less than 2 selects 5;
   a total of 2 or more falls through to normal defense handling.
5. If result 5 was not selected, a positive override counter is decremented and
   selects result 4 (`0x800fa330`–`0x800fa34c`). This counter branch can apply
   even when the eligibility gate in step 2 failed.
6. Otherwise count A/B occurrences across indices 0..14. At least 2 selects
   result 1. With fewer than 2, scan the A window: an A edge selects result 4.
7. If no A edge is in that window, a positive anti-mash count selects result 1;
   a zero count selects result 2.
8. Every return path calls `BtlPad_WorkInit` on the action-command pad
   (`0x800fa400`–`0x800fa404`), then returns the stored result.

The conventional names are result 4 = guard and result 5 = superguard; local
behavior is specified numerically to avoid importing unverified downstream
damage semantics. `BattleCheckDamage` reads the stored result: at `0x800fb0a4`
it tests 5 specially, and at `0x800fb364` it distinguishes values above 3.
The surrounding damage/status/counterattack branches are not fully documented
yet, so this document does not assert a universal damage reduction/reflection.
Result 1 distinguishes attempted/rejected timing from result 2's absence of a
recent A/B edge on the ordinary path. Result 0 is transient initialization,
not a terminal return of the inspected ordinary branches.

## Derived boundary cases (P tests; not O recordings)

At default difficulty, with counter 0 and AutoCommand byte 0:

| History before evaluation | Eligibility gate | Expected local result |
| --- | --- | --- |
| No A/B in indices 0..14 | Either | 2 |
| Only A at index 7 | Either | 4 |
| Only A at index 8 | Either | 1 |
| Only A at index 15 | Either | 2 |
| Only B at index 2 | Eligible, HP positive | 5 |
| Only B at index 3 | Eligible, HP positive | 1 |
| Only B at index 0 | Ineligible | 1 |
| A and B together at index 0 | Eligible, HP positive | 1 |
| A at 0 and A at 14 | Either | 1 |
| A at 0 and A at 15 | Either | 4 |
| Positive override counter, no edges | Eligible, HP positive | 5, decrement once |
| Positive override counter, no edges | Ineligible | 4, decrement once |

All cases also reset the pad history. Thus adjacent hits cannot blindly reuse
the same history unless inputs were recorded anew. A dedicated caller may skip
this function or populate the override; those contexts must be traced before
generalizing this result to all attacks or tutorial sequences.

## Reproduction and open boundaries

```sh
python3 tools/inspect_dol_function.py BattleActionCommandCheckDefence --symbols artifacts/reference/us_symbols.csv --output artifacts/reference/behavior/BattleActionCommandCheckDefence.elf
python3 tools/inspect_dol_function.py BtlPad_WorkUpdate --symbols artifacts/reference/us_symbols.csv --output artifacts/reference/behavior/BtlPad_WorkUpdate.elf
python3 tools/verify_behavior_contracts.py
```

The verifier checks table contents and selected instruction anchors. It does not
execute the decision routine, verify all callers, or prove a port correct. The
51-function metadata report includes fingerprints, not a completion percentage.

Still required: attack-specific action-command implementations, impact/input
sampling order, tutorial restrictions, override counter writers, all damage
modifiers, statuses, multi-hit defense reset policy, partner commands, stylish
actions, audience/bingo/SP consequences and observed timing acceptance.
