# Retained original Mario mov_pos2 and force start

`SourceMarioMovePosition` executes whole original
`evt_mario_mov_pos2 800e5f50/180` and its complete inline-owned
`marioSetForceMove80095e90/d4` child. It is portable source control/memory logic;
actual field owners, numerical helpers and `marioSetMotion8009455c` remain typed
providers. It neither reuses the legacy linear millisecond movement adapter nor
certifies whole MarioMain, native pose completion or a source field playthrough.

## Callback and suspension semantics

`resume(actualEntry, actualGuestEntryAddress, firstCall, services)` returns an
optional source EVT result. Empty means an inner read/store/child remains held;
retain this cursor and its original operands. Engaged 0 means **this original
USER_FUNC body completed**, with force flag20 still set. The actual VM must call
the body anew on its next real simulation visit, now with firstCall false.
Only after a complete body may `reset()` start that next invocation. Repeated
resume after completion returns the original result without replaying work.
Render attempts must not create new simulation visits or decrement a timer.

Actual event pointer/guest address/ID/callsite and immutable services/user/callback
identities are pinned across inner holds. Lifetime compares both pointer and
shared ownership identity. Current proof precedes every source operation and
lost current poisons the cursor, preventing partial-prefix replay. Recursive
resume/reset is refused. A false store must have no effects. A child returning
unavailable after doing work must retain that work in its actual child owner.
Destroying a held cursor is permitted only with genuine caller retirement or
explicit outer teardown, not a retry policy that replays earlier stores.

Null callbacks hold at their actual reached source boundary, preserving prefix
reads/writes. A poll invocation that never reaches arithmetic/motion can complete
with those callbacks absent. No guessed pointer, output, GP value, numeric result
or guest-stack alias is introduced. All captured stack/register values are
private typed native fields, initially unknown until their real read completes.

## Source order and independently captured owners

Each invocation captures E+18 and the current Mario pointer, then reads/resolves
X, Z and speed separately through the original evtGetFloat80030840 boundary.
The first-call flag is retained even if a later native retry supplies false.
Only first-call executes distance8000722c and angle8000703c. It rereads actual
X/Z for angle after distance, stores travel direction1a4, then reads current
main flags. Flag02000000 applies actual source scale80421b0c=3ecccccd (~0.4),
not half speed. Fresh GP pointer80415fd0 and GP+4 provide signed refresh rate.
The conversion reads actual double bias802e3eb8=4330000080000000; actual source
one at80421ad8 is3f800000. Direction1a4 is read afresh after those constants.

The original scalar sequence is individually single-rounded: rate/GPHz,
one/rate, distance/rate, then addition and fctiwz. Do not algebraically replace
it with `(distance+1)/rate`, assume a clock rate, or cache source constants.
The numeric request records each original instruction/callsite. Actual native
SDK/IEEE providers must supply that domain or remain unavailable; the test
arithmetic provider does not establish Gekko/FPSCR/angle/sqrt bit parity.

Whole force-start independently reloads the Mario pointer. It ORs main20;
stores facing1a0, travel1a4, low16 counter4e and speed180; then reads motion2e.
Motions25/21/22/26 skip the action child. Other motions read dash base188 and
call actual SetMotion with2 for speed>=dash, otherwise1. After that child the
source deliberately writes facing/travel/counter/speed **again**. These stores
must not be deduplicated: the child may mutate them. The parent then polls its
own originally captured actor, which can differ from the force child's fresh
actor. Set flag20 returns0 without position stores; cleared flag20 writes X8c
then Z94 and returns2. Y90 is never overwritten by this callback.

## Actual native integration boundary

Root's field adapter must supply the actual EventNativeRuntime entry address and
actual VM evtGetFloat operands. Real typed Mario storage must expose main0,
motion2e, counter4e, XYZ8c/90/94, speed180, dash base188, facing1a0 and travel1a4.
A finite native source-only view may expose those actual fields; it must not
manufacture a whole Mario allocation, source pointer history or unknown words.
Source-owned/unknown memory must receive first refusal before snapshot fallback.

Original movement and count consumption are separate. `marioMoveMain800594a8`
contains the forced XZ step; actual `marioForceMoveMain80095f64/94` consumes the
signed16 counter and clears20 at its real reached boundary, after movement in
MarioMain8005b0dc. Neither elapsed wall time nor the event's result0 certifies
that consumer. Until that actual/native sim-tick owner is bound, the callback
correctly keeps returning0 while20 remains set. An inner unresolved SetMotion
must not allow a partially committed source force-start to be driven as though
its whole synchronous call had finished. Outer native simulation admission owns
that scheduling fence. No Game integration/activation is included here.

## Validation

An independent immediate reference runs both complete source bodies. Tests compare
full ordered read/store/GetFloat/numeric/action traces and final memory across
normal/poll/scaled/special-motion branches, holding every external lane twice.
Mutations prove fresh angle coordinates, late direction, independent parent/
force pointer capture and required post-action stores. A retained partial action
child completes its own prefix once. Tests also cover result0/new-poll behavior,
null callbacks at reached boundaries, changed service/control-block identity,
reentry and current-loss poisoning. Complete original bodies/constants/ABI are
pinned read-only from the US revision-zero ISO.
