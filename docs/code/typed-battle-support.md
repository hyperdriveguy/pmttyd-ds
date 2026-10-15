# Typed movement, targeting and facing support

These APIs compose semantic objects and services alongside the
[packed state](packed-battle-state.md) and [packed motion](packed-battle-motion.md)
implementations. Matching fields do not make their native structs guest ABIs
or establish identical per-word memory traces. Hexadecimal offsets/masks below
are source byte locations; decimal counts/scalars are stated separately.
[Behavior contracts](../behavior/README.md) retain original evidence. Source
simulation visits and recovered coefficients are not measured render timing.

## Counts and output operands

[battle_move_frame_event.cpp](../../source/battle/battle_move_frame_event.cpp)
reads raw Y before converting X, then still resolves/discards integer Y. Raw
speed similarly precedes Z conversion. Visible XYZ is captured after operand
work. Speed zero falls back to unit speed then source fallback. Count truncates
distance/speed, adds one only for positive fused residual, then forces exactly
zero to one; it does not clamp negatives or implement generic `ceil(abs(...))`.
The raw output sentinel `f1194d80` (-250000000) skips writing.

[battle_mario_jump_parameter_event.cpp](../../source/battle/battle_mario_jump_parameter_event.cpp)
instead treats operand 4 as both resolved requested count and raw destination.
Requested zero derives distance/speed without forcing one. Beyond the loaded
source threshold, add truncated extra distance × source coefficient with word
wrap. An explicit request is a signed lower bound on the result. Output is
suppressed only when the **result** equals -250000000; a raw sentinel destination
may still receive a changed result. Do not merge these output predicates.

The typed equivalents in [jump motion](../../source/battle/battle_jump_motion.cpp)
and [move position](../../source/battle/battle_move_position.cpp) use semantic
arguments already captured by their callers. They still resolve/discard integer
Y, but do not claim to reproduce every packed raw-memory prefetch. Jump count's
source threshold is 60 and binary32 extra factor `0.15000000596046448`.

## Motion fields and numeric choices

`MarioJumpUnit` keeps visible, baseline, target and per-axis delta independently.
A visible output never silently becomes the next baseline. Sound-source and
mode arrays are big-endian records of `0x36/0x1c` bytes; the whole native unit
must not be serialized as a source struct. Baseline/target setters skip exact
float -250000000 per axis, distinct from raw EVT output tags.

Typed linear movement captures current position, target, heading/distance and
nonzero-X direction on first call. Zero count derives and forces one, correcting
speed by fused residual; explicit count divides distance. Horizontal integration
uses recovered binary32 tau `6.2831854820251465` and 360 degrees, double sin/cos
then single results. Both trig calls precede destination reads; X store precedes
Z read, so aliased X/Z sees the update. After publishing baseline, wrapped signed
remaining >0 controls sound / returns 0; otherwise snap target, end retained
sound and return 1. There is no render-delta multiplication or new zero-speed
guard.

Typed Mario jump uses these source coefficients and modes:

| Quantity | Meaning |
| --- | --- |
| Binary32 Pi/tau | `3.1415927410125732` / `6.2831854820251465`, not exact host double constants |
| Phase advance | Double FMA factor `0.53*sin(theta)+1` for modes <3, otherwise `0.8*sin(theta)+1`; single result after advance |
| Initial angle / step | 90 degrees; float `360/n` for modes <3, integer division then conversion otherwise |
| Height | `((distance-20)/6)+47`, single rounding after each double operation |
| Modes 2/4 | Height multiplied by source 1.12 / 1.25 |
| Post-trajectory increment | Binary32 `1.7999999523162842` |
| Continuation divisors | 16.5 for height setup, 33 for horizontal continuation, neither a time delta |

Event phase 1 runs trajectory, 2 checks eligibility, 3 continues. First-call mode
1 skips ordinary setup and selects phase 2. Derived zero count returns 2 without
a forced one. Phase 1 advances baseline, computes sine/FMA visible height,
clamps upward when signed remaining <3, then tests **exact zero** after wrapped
decrement. It retains negative self-subtraction rather than folding velocity
reset to zero, preserving signed-zero/NaN behavior. Phase 2 either returns 2 or
sets count one / phase 3 and returns 0 before continuation. The two phase-3 sine
calls are separate callback points. Source event results are 0 block, 1 advance/
yield, 2 advance; they are not ordinary Boolean returns.

## Sound records and borrowed position views

[loadUnitSoundMode](../../source/battle/battle_jump_motion.cpp) resets toggle
`14` and handle `18=-1` before reading source bytes. Source/destination overlap
therefore matters. Mode -2 retains keys/periods; -1 with types 0–3 selects source
offsets `8/18/0/28`, type 2 zeroing periods. Other modes clear keys/timing.
Countdown `12` finally copies initial period `c`.

[battle_movement_sound.cpp](../../source/battle/battle_movement_sound.cpp)
snapshots keys `0/4`. First-only mode starts once at supplied position, then
updates retained handle `18` from fresh unit/part position. Periodic mode
crosses signed countdown zero to -1, toggles parity, reloads `e/10` and triggers
at the original supplied position, without retaining a handle. End clears a
retained handle only after actual Off. The adapter stores the playback-services
object's address; that object must outlive it. Original trigger sentinel and
zero argument are forwarded, not replaced by an audible-success stub.

[battle_position.cpp](../../source/battle/battle_position.cpp) borrows actual
owner fields through pointers. Raw outputs are required and sequential;
relative world calculation captures visible/offset/facing, using X FMA and Y/Z
addition. Absolute mask `10000000` uses absolute coordinates. Hit calculation
captures side factor first, then applies two layers with fresh scale reads:
rounded X/Y product plus FMA, one Z FMA. Pointer outputs can alias; the EVT
privately snapshots XYZ before truncation/stores and skips raw sentinel axes.
Unit and separate part services may independently capture different shared work.
Negative part ID returns head; a single-part head ignores requested definition
ID; longer lists scan IDs. Missing heads/parts require caller admission.

## Candidate buffers and takeoff

[battle_candidate_sampling.cpp](../../source/battle/battle_candidate_sampling.cpp)
and [choice](../../source/battle/battle_candidate_choice.cpp) retain the row/
mask meanings mapped in [packed state](packed-battle-state.md). The native
`0xacc` byte array contains 74 decimal-36-byte rows from `+4`, followed by count/
rank/actor/mask metadata. Its weapon is a **separate semantic pointer**; the byte-zero word is not populated by these typed APIs as a weapon pointer. Reusing the sampler retains
unused bytes. Asserts are domain diagnostics, not complete capacity validation.

Typed sampling can use native `memmove` for private row compaction; it does not
claim the packed sampler's observable per-word service trace. It captures its
own work and per-row masks/part fields. Source bucket formula is
`trunc((800+x)/40)` only for x>-800, upper-capped at 48 and mirrored for negative
direction. Sort Z adds 5, lane splits below -30, and optional directional bias
adds `300*direction`. Signed-16 narrowing precedes sort-key storage. Admission,
attribute, same-unit preference and occlusion remain separate passes. Successful
Monosiri admission queries again; the second pointer is an asserted native
invariant. Pair-swap ordering is not stable sorting.

Typed chooser captures work, writes (-1,0), then publishes first ranked candidate
before weights. Unit output precedes part read and callbacks can mutate rows,
ranks, count or semantic fields. First-row bonus 10 and per-rank +100 precede
flag formulas, with source float coefficients 1.5/.1/.4/.5/2/1/.05 and truncation
after each selected formula. Item/status overrides, repeated FP getters and live
weapon reloads remain explicit. All 74 weights participate in maximum/roulette
selection; strict maximum ties favor row index, roulette stops at `<=0`, and
zero weights are not skipped. There is no invented zero/negative-bound fallback.

[battle_jump_takeoff.cpp](../../source/battle/battle_jump_takeoff.cpp) captures
work before actor/target/part operands, keeps own home Y and target hit XZ, then
samples null weapon with original masks `01001240/01000000` and signed-low-byte
bias. Chooser mode 4 always runs. Positive count may choose a nearer X; requested/
chosen part pointer equality gives offset 60, otherwise 30; sampler swap scalar
reverses sign only for that positive-count branch. Empty count uses +60. Fresh
side reads multiply the offset, and all raw outputs write without sentinel
skipping. The composed overload owns one local buffer per invocation, rereads
count after choosing and must not let buffer/output references escape.

## Selection, facing and target-confirmation order

[battle_selection.cpp](../../source/battle/battle_selection.cpp) preserves the
menu/candidate layouts described in [packed state](packed-battle-state.md).
Current work/acting unit lookup precedes cursor-type rejection. Types 0–8/10/14
have exact cursor fields; 5/14 require kind-dependent actor access. Unsupported
cursor leaves output untouched / returns unavailable; selected weapon token zero
is an engaged result. Captured menu work remains separate from fresh actor work.
The second selected-target halfword loads after the first output callback.

[battle_facing.cpp](../../source/battle/battle_facing.cpp) captures initial battle
for alliance direction while actor/target lookup can recapture globals. Mode
mask 9 bypasses target actor translation, even in combinations with target mode
16. Facing priority is sequential:

| Stage | Direction source / effect |
| --- | --- |
| Default | Actor retained `189`, or part `bd` |
| Mask 4 | Initial battle alliance-row direction `+a` |
| Mask 8 | Motion direction `188`, or part `bc` |
| Mask 16 | Target X minus own X |
| Mask 1 | Literal signed integer X minus own X, overriding target mode |
| Mask `100` | Word negation last |

Zero or unordered delta retains the previous direction. The X helper loads all
three axes, and deliberately adds **unmirrored** relative part offset X; it is
not general part-world FMA/facing calculation. Absolute mask uses part `18`.
ChangeFaceDirection resolves actor then direction, reads current work and narrows
to byte `189` without changing motion `188` or part direction.

[battle_target_selection.cpp](../../source/battle/battle_target_selection.cpp)
implements local Input state 11, not the full outer controller. Provider false
may follow source writes/sounds; there is no local retry cursor or rollback.
Cursor child writes actual mapped scratch; old unit/part is captured before
refresh/count read, then its new rank is retained or reset to zero. Weapon
`64` mask `01000000` permits repeat `80000` then `40000` independently. Each can
emit movement sound for count >=2, even when their net movement cancels.

Cancel `200` takes priority over confirm `100` and returns without audience
polling. It runs sound 2521, end-target, menu 6, clears battle mask 4, help 0,
command pose; fresh type 0/1 and battle masks `200/400` can select menu 1 and
remove enemy light. Confirm instead runs sound 2520, light Off(1,1), menu 29,
end-target and battle mask 4. Type 2 / unit state zero / operation 2 or 3 writes
state 8 or 5 and pays the fixed original weapon before help 2. Audience still
runs afterward: nullopt unavailable, false completed without interruption, true
publishes marker `1c74`, end-target and help 0. A true wrapper result does not
invent outer progress. Finalization calls actual attack selection before clearing
menu/battle flags and light Off(0,0), returning engaged source result 1.
