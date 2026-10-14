# Action-class ring draw lifecycle and icon routing

2026-10-14 static US G8ME01 revision0. Complete local bodies implemented:
DrawMainMenu801160e4/0xb94 and DrawMainIcon80119abc/0x1c0. The
[verifier](../../../tools/verify_battle_act_draw.py) pins both bodies, linked
GetRingCenter/GetRingOffset identities, the angle table, state jump table,
source constants and r2 initialization. No Dolphin observation or original GPU
parity is claimed. [UI Init/End](ACT_SELECT_UI_ALLOCATION_AND_NATIVE_BINDING.md)
provides this owner's work/arrays. Camera, center/offset, matrix SDK,
interpolation, circle/bubble raster, party button and pinch mark retain their
own linked-provider contracts; this is not blanket recovery of those bodies.

DrawMainMenu captures current-global battle, its W=B+1c78 owner and C=WORD(W)
before the camera8 request. Those captures survive a provider changing B's
owner pointer. W+0c modes0/2 count row states3 against a retained class count;
when all match, write mode1. Mode1 opens BYTE(W+14), compares fresh selection
to W+18 and detects adjacent selection including both wraps. Rotation changes
to mode2, closes input, saves the current selection, stores each old order at
W+70 and updates order at W+84 with one source wrap. Rows become state4/5,
counters0. Resume mode10 changes to1 and opens input immediately. Other modes
have no outer transition. W+1c is cleared before the row loop.

Every row loop-head freshly reads signed C+8. Row states are:

- 0: decrement delay; when nonpositive, set state1/counter0. No local transform
  or position is produced on this call.
- 1: enable flag1; advance angular entry using source12/rate, clamped to the
  retained table endpoint. Center/offset produces position with Z=offsetY.
  Rotate the matrix around x, using original270+90*counter/(rate*10/60),
  reset that angle to0 at360 and convert to radians. Selected row translation
  adds20 to X and sets flag2. Only after both endpoint and integer-duration
  gates pass, write state3/counter-1; then fresh counter increment makes0.
- 3: stable table position; selected translation adds20 and sets flag2; others
  retain their flags. Parameter0, no counter increment.
- 4/5: interpolate angle from saved order toward new order in opposite
  directions. The wrap endpoint/start0 becomes360 as appropriate. Reaching the
  endpoint writes state3/counter-1, but still finishes this rotation branch.
  Translate, parameter0, clear flag2, increment counter.
- 10: set flags1/2; ring center at angle0. Interpolate X from-60 and Y from55
  using mode4 and rate*12/60; translate with X+20. Parameter uses mode0 from360
  to0. Increment then compare strictly greater than duration for state3.
- 11: set flag1, clear flag2, use the table ring position. Scale X/Y by
  0.1*counter with Z scale1; write state3 when scale>=1 while still drawing this
  branch. Parameter0; increment counter.
- 2/6/7/8/9 and values above11: no row-local writes or transform production.

The original angle table has three five-float rows for counts3/4/5, indexed
as base+count*20+order*4-60. Constants are recovered data, not measured timing.
Integer products wrap32 before signed division; scalar operations use float
rounding and fused expressions where the source does. Matrix and transcendental
SDK equivalence remains the provider's responsibility. Source draw counters are
advanced by this callback; the native owner must invoke it once per simulation
tick, not once per rendered frame.

DrawMainIcon freshly resolves the active unit from current-global B+420 and
selects Mario cursorB+1bb4 when kind222, otherwiseB+1bc0. It captures initial
position Ys and row indices, sorts descending using the source swap loop
(equal values do not trigger swaps), then freshly tests count per draw.
Flag1 admits a draw; flag2 routes to DrawMainIconHukidasi, otherwise to
DrawMainIconCircle. Each row address is rows+20*index. Matrix, position and
parameter are passed without substituting a selected-row or enabled-row guess.
An initial tie can be reordered by later swaps; this is not a generic stable
sort guarantee.

The typed port exposes unwritten source stack elements as optional values.
It does not manufacture identity matrices/positions for omitted states or
clamp invalid source counts. A linked consumer that requires an unwritten
value reports an explicit provider frontier. A false return can follow earlier
memory/draw effects; do not restart the callback as a transactional retry.

Tests exercise delayed entry, original readiness progression, both rotation
directions, resume/scaling, empty/unsupported states, camera frontier and
actual descending icon routing with inactive-row suppression. Native class
rendering/normal Game activation remain open; the development readiness
acknowledgment has not been removed until this lifecycle has a complete native
presentation owner. The
[two-screen requirement](../display/NATIVE_BATTLE_HUD_AND_SUBMENU.md)
continues to place the original menu on the upper battle scene and audience
continuation on the lower screen.

The circle/bubble linked consumer bodies and private original surfaces now have
[their own complete local contract](CLASS_CIRCLE_AND_SPEECH_BUBBLE_DRAW.md).
Native GX/SDK/icon/font providers and renderer activation remain open.
