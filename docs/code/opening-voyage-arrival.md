# Voyage, arrival ship and field-departure ownership

Implementation: [Voyage](../../include/story/opening_voyage_scene.hpp),
[logo/local callbacks](../../include/story/opening_voyage_callbacks.hpp),
[arrival](../../include/story/opening_harbor_scene.hpp),
[jump](../../source/story/opening_harbor_jump.cpp),
[field child](../../include/story/opening_field_departure.hpp),
[ship bindings](../../source/story/opening_ship_objects.cpp).
Evidence: [Voyage callbacks](../behavior/story/NATIVE_OPENING_VOYAGE_CALLBACKS.md),
[arrival requests](../behavior/story/NATIVE_OPENING_HARBOR_SHIP.md).
All clocks here are simulation adapters; rendering publishes its actual resource/
final-frame acknowledgements without advancing motion.

Voyage retains source globals(-2000,0,-3000), then approach-1800->0 starts after
240 ticks. Ship delta is zero at that reseed, preventing a fabricated wheel step.
Input eligibility begins841 ticks, independently of logo resource/state readiness.
Sparse advance delivers a physical edge only to its final tick, never every
replayed update. Native track8/9 choice uses live1141 threshold before acceptance,
then freezes that branch using acceptance time. It is not always a current-time
music switch or an original BGM enum.

Logo resource checks precede states, with at most one invocation per tick,
including visits that find resources unavailable. Interpolation states increment
the same counter separately for alpha and Y: first pair uses1/2, last179/180,
leaving entry alpha254/Y0. Then global tick%15==0 admits input state. A/B/START
mask0x1300 requires external eligibility; acceptance and full logo cleanup are
separate publications. Retained prompt helpers have no used source registration;
their existence does not justify inventing a visible Voyage prompt.

Acceptance immediately begins departure0->1800. At +240 ticks request heading/
white, then require +288 **and** actual alpha255 presentation. Logo's own fade is
a separate owner. Rudder/wheel/chimney/screw publications quantize original
interpolation, division and additions separately; >360-only wrap remains.
Sway starts after90 ticks/1500 ms. Local transform retains pivot Y+/-6000,
phi=(5*globalX)/-6000, source rotation order and four Q1024 writes. Native trig/
projection/camera/audio/effect services remain distinct evidence requirements.

Arrival uses explicit incoming GW8 (`work8`, fresh native0), not a guessed restored
height. It waits180 ticks, approaches -600->-270 over120 via curve0, then docks
-270->-75 over120 via curve4. First message at480; second first-close+120. Source
messages014/015 keep literal speaker0. Current camera/rocking/parts use source
requests with a bounded native single-tick owner, not a complete REL scheduler.

After015 closes, jump begins +60 ticks with integer targetY=trunc(GW8), duration
1800 ms and height100. Used no-hit jump computes coefficient-4*height, X/Z s*d/n
then addition, Y nested fused quadratic. It saves/clears flag0x80000, snaps at
elapsed>=duration before division, and ORs the saved bit into **current** flags.
Duration0 completes immediately; this is different from the incremental house
glide and does not add collision or forced gravity state.

Paper capture requests at +84 ticks; admitted capture starts original0..60 sample
clock. Elapsed60 plus final presentation restores yaw0. Departure needs +288 and
paperDone, then starts next tick. Actual renderer owns valid capture/final-frame
claims; a timer alone cannot establish them. Its legs last120/360 ticks, wakeX
remains-475, parent story3 publishes at180 while ship child lasts480. Speed/rocking
pause during dummy jump, and rocking holds amplitude through each segment.

`OpeningFieldDeparture` copies the still-live child only after story3/paper end.
It anchors field ticks to the existing source clock, preserving progress rather
than restarting departure. Native room1 is harbor; unload ends this local child.
Parent completion does not therefore mean all ship geometry/resources can retire.

`PMHOBJ02` ship pack has seven hierarchy groups, finite invertible3x4 matrices,
and per-group unique vertex IDs. Parent/child overlap is intentional. Child axes
X/Z/Z/Y/X consume operands0/1/1/2/3. Apply computes child world positions, maps
through ship inverse, then composes root rock/yaw/translation; wake bypasses those
ship-child/rock transforms. Voyage has distinct wake Y/yaw policy from harbor.

Remap validates all index bounds before mutation, then rebuilds scratch/membership;
it does not prove uniqueness of arbitrary supplied mesh remaps. Const apply uses
mutable vertex scratch, so callers serialize writers. It does not mutate motion
clocks. Local/source translations divide10; converted mesh/GPU ownership and actual
original material/camera parity remain separate. See
[sequence/house](opening-sequence-house.md) and [transitions](opening-map-heading-wake.md).
