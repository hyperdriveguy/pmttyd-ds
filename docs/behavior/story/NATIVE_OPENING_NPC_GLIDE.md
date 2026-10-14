# Opening Parakarry original NPC glide callback

Scope: US G8ME01 disc0 revision0, original aaa evt_prologue's two calls to
evt_npc_glide_position800512bc/0x5ac. This is the used motion callback owner,
not a complete NPC physics/display/audio driver or original-runtime trace.

## Original request and entry

At aaa file0x1008 delivery requests target(210,25,-30); at0x1108 departure
requests(0,300,-100). Both operands are duration0ms, speed80, height-30,
selector11, flags0. Source delivery starts from explicit(500,500,0); departure
captures the current NPC position after pro_00 completes, without teleporting
to the authored delivery endpoint. Original US message IDs/order/speaker and
request conditions are unchanged.

First callback resolves the NPC and operands, sets motion flag0x10, captures
target at NPC+0x164 and current position at+0x158, resets the elapsed accumulator,
and calculates full3D distance. Squared-distance order is Z²+(X²+Y²), followed
by PPC reciprocal-square-root refinement/rounding. Duration0 uses distance/speed
seconds; speed0 or distance0 returns1 without admitting movement. Used speed80
and both source distances are nonzero. The callback stores duration at+0x1c0,
speed at+0x1b0, returns0 and retains its invocation. Its normal subsequent
callback consumes npcMain's elapsed+0x1a0 and delta+0x1a4 seconds.

## Original float curve and incremental movement

_intplGetFloat800509ec/0x1e0 caps input fractions above1. Selector11's jump
entry80306a50 targets80050b58: single-precision phase PI*fraction,
cos(double phase) rounded to float, then(1-cos)*0.5. PI source8041fda8 is
0x40490fdb,3.1415927410125732; it differs from the camera interpolation owner's
constant. Do not substitute the existing camera curve for this callback.

Let elapsed E, duration D, delta S. If E>=D, mark callback complete. If E>D,
source800515dc..e8 computes S = S - (D-E), with precisely that subtraction
order. It does not simply clip E to D or use a final absolute endpoint. If
adjusted S<=0 it returns2 before applying movement. For the actual finite
positive opening clock this early-return branch is not taken.

Current = curve11(E/D); Previous = curve11((E-S)/D).
For each coordinate the callback adds (Target-Start)*(Current-Previous) into
NPC pending motion+0x1b4/+0x1b8/+0x1bc using single-precision fused multiply-add.
For Y it additionally adds height*(sin(PI*Current)-sin(PI*Previous)); source
operation order computes rounded height*sin(previous), then fused
height*sin(current)-oldHeight and a float addition. The sine calls take rounded
float phase and return double, rounded to float. Height-30 supplies the original
downward arc. This is incremental curved motion; absolute line interpolation
or snapping to the target would change source behavior.

The [original outer loop](../platform/GAME_OUTER_LOOP_AND_EVENT_UPDATE_ORDER.md)
runs evtmgrMain before npcMain. npcMain/npcMoveG consume pending XYZ into actor
position after the callback; rendering is not their time producer. Completion
normally returns1, which [USER_FUNC dispatch](../event/EVENT_DISPATCH_AND_WAIT_CONTINUATIONS.md)
clears and yields, while result2 clears and repeats in the same invocation.
The controller therefore delays its following turn/wait or removal until its
next event update after the final movement is consumed.

## Portable owner and native bridge

[OpeningNpcGlide](../../../source/story/opening_npc_glide.cpp) ports this used domain
and exposes captured source command, start/target/current/pending XYZ,
elapsed/delta/duration, callback result and active state. It accepts elapsed
and delta seconds rather than rendering calls. The house controller invokes
one callback per simulation tick and consumes its pending movement into the
existing Parakarry actor presentation; repeated same-tick advance calls cannot
apply a delta twice. No original assets or pose substitutions are introduced.
The existing native renderer already reads the resulting actor position.

Simulation ticks map to elapsed ticks/60 and delta1/60. Native sqrt/trig and
float/FMA integration are explicit numeric bridges to original PPC/SDK math;
no bit-identical or hardware timing claim is made. Full OS tick accumulation,
pause/slow-mode flags, motion/heading/current-NPC flags, collision/gravity,
move-sound footsteps and NPC registry services remain unbound. Used flags0
excludes the explicit collision early-completion test in this callback; the
portable owner does not invent a collision completion provider.

Under this native clock/math bridge delivery takes419 movement updates and
has duration6.96671772; departure takes265 updates from its actual consumed
position. Source terminal delta arithmetic produces small coordinate overshoot,
e.g. delivery approximately(209.994843,24.9930344,-30.0005665), rather than an
endpoint snap. These are host adapter observations, not measured original
runtime positions. Source targets themselves are preserved exactly.

[Read-only verifier](../../../tools/verify_opening_npc_glide.py) pins four
function hashes (callback, float curve, npcMain, npcMoveG), the187-command
house event hash and43 source request/constant/arithmetic/result anchors.
Tests cover the midpoint original vertical arc, exact-duration result1,
nonintegral terminal subtraction behavior, zero/invalid domain, source command
ownership, delivery/departure gates, completion yield/removal, sparse/dense
updates and repeated rendering cadence. Original-runtime visual/timing
comparison and full native NPC driver fidelity remain open.
