# Pinch threshold flags and transition callbacks

Scope: complete `BtlUnit_CheckPinchStatus`, US G8ME01 revision 0,
`80126e00..80126eff` (0x100 bytes). Static DOL recovery; no original runtime
measurement. [Evidence and source constants](BATTLE_PINCH_AUDIENCE_REQUESTS_AND_SOURCE_THRESHOLDS.md)
and [parent all-unit scan](BATTLE_PINCH_ALL_UNIT_SCAN_AND_WARNING_CADENCE.md)
complete the local chain. No native implementation is changed.

## Inputs and storage

Input r3 is a valid unit U; r4 is retained suppression flag F. There are no null
checks. WORD(U+0x10) points to the current kind definition D. HP at U+0x10c
and maximum HP at U+0x108 are signed 16-bit values; thresholds D+0x0d and
D+0x0c are unsigned bytes. Flags occupy WORD(U+0x138). The function does not
return a defined scalar status: r3 remains incidental load/service state and
its parent ignores it.

## Ordered first transition: bit 0x20000000

At 80126e18..34, capture D and HP, then test `HP <= U8(D+0x0d)` and
`HP < S16(U+0x108)`. If either fails, clear only bit 0x20000000 when present.
If both pass and the bit is already set, preserve it and skip the callback.
On a newly qualifying transition, set the bit before any callback. Only F==0
and a fresh WORD(U+8)==222 call `BattleAudience_Case_MarioDanger` (80126e60).
Other actors still receive the flag, and nonzero F suppresses the callback
rather than suppressing the update. Leaving the threshold emits no callback.

## Ordered second transition: bit 0x10000000

At 80126e7c, reload D and HP after the entire first phase. Test
`HP <= U8(D+0x0c)` and `HP < S16(U+0x108)` with a fresh maximum HP load.
Failure clears only 0x10000000. An already set bit produces no callback.
A newly qualifying transition stores 0x10000000, then reloads flags. Only
when 0x20000000 is now clear, F==0, and a fresh kind is 222 does it call
`BattleAudience_Case_MarioPinch` (80126ed0).

The first callback may alter definition, HP, maximum HP, kind or flags before
the second phase. A native implementation must preserve these read boundaries
rather than evaluate both thresholds from one entry snapshot. Existing other
flag bits survive each OR or clear; clearing masks are 0xdfffffff/0xefffffff.

## Required edge behavior

There is no positive-HP, side, status, percentage, clamp or actor-kind gate on
the flag updates. Zero and negative HP can qualify. Full HP fails even when
below a definition threshold. With the original Mario/party thresholds 1 and
5, HP 1 below maximum sets both flags but requests only the first callback;
HP 2..5 below maximum sets only the second. Moving from HP 2 to HP 1 triggers
the newly entered first callback although the second flag was already set.
Moving back to HP 2 clears the first bit without repeating the second callback.
Repeated calls at the same qualifying HP do not repeat callbacks. F!=0 still
sets bits, so a later F==0 call need not emit a callback for that existing bit.
These examples assume stable services and the source thresholds, not a universal
threshold policy for every actor definition.

## Port acceptance boundaries

Reproduce signed HP comparisons against unsigned byte thresholds, first-phase
mutation before second-phase reads, entry-only callbacks and independent bit
clears. Positive HP filtering belongs to the parent warning scan. Do not infer
that symbol names describe conventional UI danger/peril labels: original symbols
and sound keys are retained exactly. Audience target adjustment internals and
runtime presentation remain separate documentation work.
