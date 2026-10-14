# Battle action, status and resource code

This guide covers the portable core helpers, not complete attacks, event
scheduling, presentation or native teardown. Original behavior evidence is in
the [battle contracts](../behavior/README.md). Keep callback invocation counts
in simulation history, separate from rendering frames.

## Input and action results

[TimingA](../../source/battle/battle_timing_a.cpp) reads30-word general trigger
history (newest sample0), scans20 samples to reject multiple A presses, then
checks the shorter difficulty window. Seven threshold/sample tables use the
first row unless flag2 requests count-dependent shrinking. Equality stays in
the current row;99999 terminates tables and count>=100000 yields no window.
Result mask0x2 means success; classification1 is pressed/mash failure,2 no press,
3 ordinary success. Special automatic routes need not write classification.
The random generator wraps32-bit LCG state (`1103515245*state+12345`) and extracts
bits16..30; zero-bound draws consume no RNG. Seed ownership remains unresolved.

TimingA main states0/99/1000/1001/1002 mean initialization/idle/finish start/
countdown/retirement. Finish60 decrements on its installation call. Its scratch
block is11 words, with lane5 holding original float-300 bits0xc3960000 and lane8
scalar20. These are display-work initialization values, not attack timing.
[TimingA contract](../behavior/battle/BATTLE_TIMING_A_COMMAND.md).

[Defense](../../source/battle/battle_defense.cpp) owns private history rather
than TimingA's general trigger history. Its inclusive +/-30 stick threshold
publishes main up/down/left/right in bits16..19 and substick in bits20..23.
Global DemoPad uses a different threshold and masks. Virtual repeats wait16
updates, then8; signed-byte counters preserve source wrap. Defense counts
A and B separately over15 samples, even when both are in one history word.
Difficulty is `clamp(3+Unsimplifier-Simplifier,0,6)`; guard windows are
10/9/8/8/7/6/5 samples and superguard5/4/3/3/2/1/1.

Defense results1/2/4/5 mean pressed failure/no press/guard/superguard. Superguard
is tested first only when weaponGate and positive HP allow it. Otherwise guard
uses the same retained input. All terminal routes reset the private pad while
retaining release history and its unwritten tail byte.
[Input/defense contract](../behavior/input/BATTLE_INPUT_AND_DEFENCE.md).

[Hammer](../../source/battle/battle_hammer.cpp) uses general held-left bit0x40000,
not a new press or A edge. State100 installs120 acquisition polls and immediately
processes state1000; charge state1001 decrements countdown before stage/release
logic. Stage increment and release check take mutually exclusive branches.
Final stage is N+1; N+2 means overrun. Difficulty allowances80/70/60/40/20/10/5
compare against `period-countdown`. Original weapon-token period overrides are
25/30 with fallback40; they are callback counts, not seconds.

Hammer results use mask0x1 pending and mask0x2 success; mode mask0x2 permits retry/forced
timeout success. Classifications1/2/3 mean failed release/overrun/successful
release. A forced result still executes the rest of the charge body. Completion
bridge1003 advances to idle1004. Stop1005 installs60 then ends its call;1006
counts down and1007 retires callbacks. Unlike TimingA, installation does not
decrement finish countdown. Handle -1 means unallocated; native charge domain
1..12 protects the twelve-element array.
[Hammer contract](../behavior/battle/BATTLE_HAMMER_STICK_KEEP_LEFT.md).

[Action service wrappers](../../source/battle/battle_action_services.cpp) use
event handler2 to continue,0 to retry ResultAC while result mask0x1 is pending.
Eight parameter lanes resolve before testing keep-existing sentinel-250000000.
StartAC writes main state100; help modes0/2 mean hide/show. Prize calculation
uses original provider byte plus input minus1, clamped0..6; low hit code30
returns -1. It captures work before resolving operands; -1/-5 actor operands
skip unit lookup. These service calls are distinct ownership boundaries.

## Status storage and byte interpretation

[Status implementation](../../source/battle/battle_status.cpp) retains original
unit offsets0x118..0x135. ID narrows to signed8; storage remains raw uint8,
with sign extension at explicit consumers. Original names are annotations from
the [status contract](../behavior/battle/BATTLE_STATUS_STORAGE.md).

| IDs | Meaning / nonordinary storage behavior |
| --- | --- |
| 0 | Allergic; setter admits positive signed duration |
| 1 | Sleep; setter clears auxiliary0x119, recovery preserves it |
| 2,3,4,5,6,7 | Stop, Dizzy, Poison, Confuse, Electric, Dodgy; Poison has separate strength |
| 8,9 | Burn/Freeze cancel the opposite without installing incoming status |
| 10/11 | Huge/Tiny share duration+signed strength; sign selects active ID |
| 12/13 | Attack Up/Down share duration+signed strength |
| 14/15 | Defense Up/Down share duration+signed strength; opposite signs cancel |
| 16 | Charge adds strength into one byte; wrap, signed negative clear, cap99 |
| 17,18 | Flipped, Invisible |
| 19/20 | Fast/Slow cancel opposite and may reset move counters |
| 21,22 | Payback, Hold Fast; Payback admits positive signed duration |
| 23,24 | HP/FP Regen, separate strength bytes |
| 25,26 | Fright/Gale Force have no stored lanes in this helper |
| 27 | Instant Kill/down gate |

Recovery moves signed durations toward zero; dead/HP<=0 units and Charge/Flipped
skip this recovery helper. Setter return0 can still mean cancellation/move-count
mutation. Move bytes0x20..24 are mode,maximum,current,baseline and a retained
counter. Fast doubles with byte wrap; Slow halves counts>=2, otherwise compares
counter against2 or4. Mode10 marks no move. Counter writers and turn cadence
are separate contracts; do not infer an alternating schedule from this helper.
[Move-count contract](../behavior/battle/BATTLE_MOVE_COUNTS.md).

## Resource import and damage accounting

[Unit resources](../../source/battle/battle_unit_resources.cpp) use original
big-endian signed16 pouch values. Mario kind222 reads current HP/FP at0x70/74
and base maxima0x8e/90. Partners224..230 use nonsequential translated party
indices and14-byte pouch records with baseHP+4/currentHP+6. Snapshot index
`kind-224` is different from this party-index translation. HP/FP Plus contributes
5 per count; values narrow signed16 before upper-capping without lower clamp or
healing. Six status-change records initialize type0xff/bytes1..3/word4 but retain
word8. Clearing status calls the original additive Charge reset after zeroing
30 bytes. [Import contract](../behavior/unit/UNIT_STATUS_CLEAR_AND_POUCH_IMPORT.md).

[Direct damage](../../source/battle/battle_damage_application.cpp) clamps requested
HP to[-99,99], but does not clamp FP equivalently. Attacker -5 means none.
Partners borrow the Mario(kind222,belong0) FP owner found in64 slots. Effects,
Pity Flower RNG/recovery and sounds precede resource subtraction. Requested
amounts narrow through signed8 bookkeeping, separately from signed16 resources
and consumed FP. Unit0x40000000/part0x800000 attributes suppress HP writes,
not all prior effects/accounting. Recovery code0x2000 chooses presentation but
does not flip damage sign. Positive requested HP drives action records even
when actual HP writes are blocked. Sleep wake checks positive requested HP/FP,
not measured loss.

Hit codes, reaction patterns and effect parameter tuples remain source values;
the [damage contract](../behavior/battle/BATTLE_DAMAGE_APPLICATION.md) gives
exact masks/argument tables. Effect positions here use original engine units;
field Game geometry uses .01 scale. Final hit-event dispatch runs last with
locally updated lethal/revival code. Missing effect/audio/audience/event providers
must not be collapsed into silent successful native behavior.
