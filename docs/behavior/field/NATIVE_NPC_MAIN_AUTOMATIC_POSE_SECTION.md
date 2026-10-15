# Retained original NPC Main automatic pose selection

The [entry binding](NATIVE_NPC_MAIN_ENTRY_AND_CLOCK_COMPOSITION.md) now composes
[movement](NATIVE_NPC_MAIN_MOVEMENT_SECTION.md) and the complete source automatic
pose subsection80049fbc..8004a160. The [parent control flow](ORIGINAL_NPC_MAIN_PARENT_CONTROL_FLOW.md)
and [child ABI audit](ORIGINAL_NPC_MAIN_CHILD_ABI_AND_NUMERIC_FRONTIER.md) remain
authoritative. Completion of this subsection reaches unfinished Detection,
never full Main completion, actor traversal or history publication.

## Source numeric graph

Fresh mainflags400 clear bypasses the subsection. Otherwise read fresh dt1a4,
then source floatzero8041fc7c. Only dt>zero enters; unordered dt bypasses.
The retained movement deltaX and deltaZ feed separate binary32 squares followed
by a binary32 addition. Do not replace this with an FMA or a double norm.

For q>zero request the original reciprocal-square-root estimate as an explicit
numeric primitive. Then load source binary64 half802c13c8 and three802c13d0.
Three times, with each multiply rounded to binary64:

1. square=seed*seed;
2. scaled=half*seed;
3. correction=NEGATE(fused(q*square-three));
4. seed=scaled*correction.

The source fnmsub negation boundary matters for signed zero and NaN signs.
Finally round q*seed to binary64, then to binary32. Positive infinity follows
this path; it is not short-circuited to a square root of infinity.

For q not>zero load source doublezero802c13d8. Negative q uses source canonical
NaN8041e3c8, raw7fffffff. Otherwise inspect the stored binary32 exponent and
fraction; only NaN loads that same canonical word. Other classes retain q.

Reload dt1a4 AFTER the norm path, then load idle threshold8041fcdc (3dcccccd).
Divide norm/dt in binary32. Strict speed<idle selects phase1. Otherwise retain
descriptor actor28, load descriptor3c, multiply by the entry's retained factor
in binary32, and use strict speed<threshold for phase2, otherwise phase3.
Unordered comparisons therefore select run unless the earlier idle test passed.

## Animation and phase order

Idle reads phase108 before descriptor28; equal1 skips all animation work.
Otherwise capture descriptor, read handle104, read descriptor10 and request
animPoseSetAnim80041170(handle,selector,1). Walk/run retain the descriptor from
threshold calculation across their phase108 check, then read handle104 and
name18/1c from that descriptor. A later descriptor-word mutation must not cause
a new descriptor lookup. The name field itself is read at its original later
point. Equal selected phase skips the child AND phase store.

The source ignores the animation child's return. Only after child completion
publish phase108=1/2/3. A missing provider holds after earlier source operations;
successful child effects are never replayed when the subsequent phase store
holds. Repeated completed-subsection calls are effect-free. Services, actor,
delta bits and retained-factor bits must match the captured invocation.

## Actual native ownership and remaining numeric boundary

advanceAutomaticPose first resumes the existing retained entry and movement
cursors through the same owner/VM/pose alias. State10-clear movement already
reaches Detection and does not invent a delta or enter auto-pose. Otherwise
the new cursor captures actual movement delta and entry factor. The animation
request uses the exact shared BattlePosePool, not a catalog or shadow namespace.

[Original selector semantics](../display/ANIMATION_SELECTOR_AND_RESTART.md)
are signed: negative raw selectors are source string addresses; nonnegative
selectors are indices, including null=>index0. The native binding admits a
bounded mapped string for negative selectors and calls selectName; it uses
selectIndex for nonnegative selectors. Original missing-name/out-of-range
fallback0 and force1 restart remain intact. No evaluation/Main call is added.
A read-only explicit clock provider must supply actual source timestamp
quotients ([clock contract](../display/ANIMATION_LOCAL_TIME_AND_CLOCK_REQUESTS.md));
missing clock admission holds without pose or phase effects. Whole typed child
admission is synchronous, not a claim of instruction-level interrupt replay.

The numeric seed has no implicit fallback. nativeFieldNpcReciprocalSqrtEstimate
is an opt-in 1/sqrt(double) adapter; it is NOT the original Gekko estimate.
The core still executes all three source refinements. Gekko frsqrte seed,
FPSCR/GQR, exception/subnormal/NaN-payload parity and original runtime traces
remain open. The core allocates no memory; source double operations retain
correct arithmetic boundaries rather than substituting a cheaper float graph.
The existing ARMv6K hard-float toolchain handles scalar operations; physical
New2DSXL performance has not been measured. Ordinary Area activation still
requires real movement/Mario/clock/resource services and the remaining parent.

## Evidence and checks

Original constants are copied unchanged through field_npc_actor_assets.py:
802c13c8 length18hex SHA2567b478c1af21c79d171ad581ffd7ac318fde7808030822ef0ed03d174e918d38f;
8041e3c8 length4 SHA25624ae0d93f1af72addc019182fae1ab44547a1e84758785745f4358373eab1960;
8041fcdc length4 SHA25631cf8391f9bfd8eedef79bcf6b13f2db5f20089185538b317e01e372d5b8b750.
Original source bytes/listings/packs remain ignored and local; rights unchanged.

Tests cover an independent idle trace, every-primitive holds across phase/gate/
exception paths, separate-square-versus-FMA operands, an independent rational
rounding oracle for exactly three Newton rounds, fresh postseed dt, retained
descriptor/fresh name, child mutation before phase publication, phase equality,
zero/negative/NaN dt, NaN threshold/speed, positive infinity and absent providers.
Shared native integration uses an actual original c_kuri_h pose, named and raw
numeric selectors, missing clock, forced same-clip restart, real VM pose aliases,
and unchanged MainCount/parent frontier. Clock/allocation/gravity providers in
that test are explicitly fixtures. See PROGRESS for frozen-tree/full/ARM gates.

Next source load is actor11c at8004a160: detection/interrupts, display/material/
shadow, late history and field tail remain mandatory. No subsection result
acknowledges or skips them.
