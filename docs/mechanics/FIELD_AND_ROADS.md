# Field movement, collision and road evidence

ROAD contact/departure, head-query and airborne/landing bindings, head-contact policy, ceiling and floor queries, slope/stair/support filters, air momentum, wall movement, launch arbitration and direction-dependent movement rates. Split out of the [mechanics ledger](../MECHANICS.md) on 2026-10-12; sections are verbatim.

## 2026-10-12 — Native ordinary ROAD contact and concurrent departure

ROAD1 metadata replaces the old marker-radius trigger with named collision
owner IDs. Registered roads apply the source runtime00800000 attribute overlay;
ordinary MoveMain refreshes its separate pass-through contact on moving and
staying passes. Case10 admission uses that actual nonnull identity and the
documented global gate adapter, without an input/radius/dot test or60-tick
cooldown. Cases enable after arrival completion. Mode1 arrival uses center+30u
to center-30u and a raw downward query from markerY, retaining markerY on miss;
it does not add a query-origin offset, apply Mario's attr4 filter, reject slopes
or quantize the returned Y. Native geometry remains scale.01.

Generic departure's inline waits for ordinary grounded support and moves toward
center+150u; its parent150ms+2frames continuation requests mapchange independently.
The native60Hz timer and existing fade bridge preserve this separation but are
not the complete original EVT credit/clock/key-nesting/sequencer implementation.
During an airborne wait, input is neutral and ordinary retained travel, gravity,
head/wall/floor collision continue. Departure does not freeze XZ or wait for
travel completion before starting the parent timer. The ordinary150units/s
movement service and final snap remain simulation-tick based.

Original GSW0 is now an optional full signed word in the native version3 save;
legacy/prototype victory cannot infer it. Unknown story or source callbacks for
HarborOut5/PartnerArrival6/CoinArrival7 remain explicit MissingStory/Custom
dependencies and are not silently executed as generic travel. gor_03 debug
arrival clears system bit40 through an explicit unsaved native adapter. Full
case-event IDs/BeroEXEC, scene execution, runtime HitObj active flags, authored
camera zones and original arithmetic equivalence remain unbound. These contracts
come from the linked docs/behavior road verifiers, not new Dolphin observations.


## 2026-10-12 — Native ordinary head-query and low-ceiling ownership

Native31f6cb8 binds the recovered no-NPC head contract, superseding provisional
head notes below. Each ordered probe selects its nearest solid ray before the
head cone gate; first admitted triangle returns contact or near miss, without
searching later feet for a better contact. Raw normalized ray normals write
projected tilts even before rejection, preserve overall inclination, and all-miss
queries preserve the old record. Actual velocity gates the returned node; separate
near clearance0..5 original units can still set the contact flag. Contact point
and proposed root use the documented fixed-bias/fused/truncated local formulas.

Low-clearance arbitration can request idle/walk/dash before the motion3/6 test,
leaving just-integrated root and tuple/flags/counters untouched on that call.
Matching previous walk/dash instead permits the ordinary pending-fall response.
Next ground entry owns clearing;walk/dash retain tuple/counter, so front-step
requires counter0. Source alias check confirms only local Y is written by the
normal head leaf; rootX copy-back is an identity. Head contact/clearance/flag
are separate native outputs. Static source verifier pins26 bodies/185 anchors/
19 spans; no new Dolphin observations or hardware comparisons are claimed.

The original-mesh plaza roof regression now checks rejected114.93-degree rear
face/admitted132.97-degree center and first-center near miss before contact.
Derived native peak.307 replaces the former.12..15 range from the incomplete
solver;neither is an original gameplay observation. Geometry/trig/scaled floats
remain adaptations;dynamic NPC/block,pose/audio,special modes/root locks and
full camera/input/world lifecycle ownership remain open.

## 2026-10-12 — Native ordinary airborne/landing binding

Static US contract OVERWORLD_AIRBORNE_AND_LANDING.md, recovered by the
documentation agent in35f82d8/a4df746/ce29235, supersedes older provisional
native startup, fall-counter and landing-query notes below. Native97fd570 and
db71cca separate current/previous motion, pending entry, active jump/fall flags,
phase/countdown, signed16 motion/peak counters, takeoff height/base and current
vertical tuple. Apex requests10 late while retaining the jump flag and integrates
the new fall tuple once; active fall enters next invocation without resetting
counters or delaying integration on its camera phase. No-contact ascent retains
its support node until the fall owner clears/queries it. Landing uses the same
five-foot SearchGround with D=V and V=V, query-owned slope outputs and the.1
original-unit ray tail; ground's separately rounded metadata is not rewritten
by landing. LandOn resets counters/ordinary tuple and requests ground motion
late. Drift mask0x80 clears on landing, damping bypass0x20 persists. Idle entry
snapshots actual root height into takeoff height. Fall horizontal movement occurs
before base damping, which therefore changes the next movement invocation.

These are static reconstruction and native regression results, not new Dolphin
observations. Native default on-screen state and bypass/key/control inputs are
explicit adapters pending original projection/control owners. Host scaled float,
geometry/trig/quantization remain numerical adaptations. Full low-ceiling
ground arbitration and head-query tilt/cone/near-miss rules are now documented
by ce29235 but remain to be bound; dynamic block/NPC/special state/attribute,
pose/sound/camera owners and original runtime/hardware comparisons remain open.

See [ORIGINAL_BEHAVIOR.md](../ORIGINAL_BEHAVIOR.md) for a subsystem coverage index,
identified US revision/DOL hash, and new static evidence for sequence ordering,
event scheduling, command continuation and wait behavior. That reference keeps
binary evidence separate from public reconstruction, observations and port
proposals. This ledger is chronological: later implementation updates supersede
earlier statements that a feature is missing.

## Ordinary head-contact policy (US source, not yet active geometry query)

Read-only DOL tracing: marioSearchHead8008c1a8 and marioChkOverhead8008c904.
Normal height37/base width20 engine units; five center/rotated probes, radius
.5*(width-.5)=9.75 engine units (.0975 native). Upward ray starts at rootY+
half height and has length height+vertical velocity. Collision callback
chkFilterAttr2 (800916c4), normal marioHitCheck2, accepts !(attrs&4) OR
(attrs&0x00800000); unlike floor/wall,0x00080080 is not an override.

Search contact Y=trunc(hitY*100+height/2)/100 (engine units). The100 constant
is80420900, not1000. Normal contact response requires actor vertical velocity
>=-10 and rootY+height>=rounded contact. Overhead root is
trunc((contactY-height)*10+.5)/10-1, with truncation toward zero. This means
ceiling45 gives contact45.18 and root7.2 after clearance. Root may additionally
clamp to saved takeoffY (+0x120); the separate peak setter800968f0 writes+0x12c.
On bump source clears jump derivatives, selects falling motion, installs normal
fall parameters. Sound342 is requested; native SFX mapping remains incomplete.

include/head_collision.hpp isolates verified attributes/contact math. It is
not wired to gameplay until geometry selection is faithful. Existing ground
pack preserves attrs/source node and vertices but omits collision cull mode,
and selects only |normalY|>=.65. Original hitCheckVecFilter800158a4 reads node
drawmode byte1; mode1 rejects dot(triangle normal,ray direction)>=0
(800161c4–80016210). Other modes bypass that test. Before integrating, preserve
this metadata and recover original triangle-normal orientation; do not infer
ceiling solely from height or floor-pack winding. Sloped/dynamic surfaces and
transformed Mario branches require further work. Source inspection artifacts
are ignored; regenerate with tools/inspect_dol_function.py and us_symbols.csv.

## Active static ceiling queries (2026-10-12)

The previous head-policy section is now integrated for ordinary field jumping.
Closer tracing of hitCalcVtxPosition800165a8 recovered the collision-specific
strip order: even triple(n,n+2,n+1), odd(n,n+1,n+2). This is opposite GX render
winding; hit_topology preserves source order separately. Source edge construction
80016990–800169cc computes(v2-v0) cross(v0-v1), equivalent to standard cross
for these collision triples. Parity/swaps/cross call bytes are checked in the
actor verifier. Previous floor/wall packs keep their existing geometry policy.

New complete per-room .head pack retains all source strip triangles, attributes,
source node and original drawmode byte1. No normalY slope threshold is applied.
Upward query uses lowest intersected plane from root+height/2 to that origin+
height+velocity; mode1 culls normal dot upward>=0. Five center/rotated probes
use source9.75 radius and original head-specific filter. A reached contact
uses verified rounding/clearance, preserves takeoff minimum, resets to normal
fall parameters. Source ordinary check runs after jump integration; native
does likewise. Head geometry/index participate in transactional room rollback.

Host analytical tests cover low/sloped ceiling, one-sided winding, two-sided
underside, footprint rotation, source filter differences and corrupted pack.
Original plaza(-6.7,.1) fixture limits held jump to .12–.15 native units (observed
host maximum.137), lands without damage/warp. Native repeated jumps check landed
save/position/HP; capture alone does not measure source timing or original
Dolphin fidelity. Dynamic collision, transformed actor branches, jump/hammer
block events, SFX342 and exact falling/takeoff flag variants remain unfinished.

## Source update order and distinct landing callers (2026-10-12)

marioMain8005a7e0 distinguishes ascent from fall. Ascent branch8005affc
calls marioJump,8005b000 overhead,8005b004 horizontal move; conditional
marioChkGnd2 follows8005b034. Fall branch8005b04c moves, then overhead
8005b074, then marioFall8005b078. Native ascent now integrates jump/head
before XZ wall queries; falling retains XZ before fall gravity. New ledge falls
start gravity the following tick. Split integrateVertical/finishVertical keeps
landing selection after XZ, retaining existing search behavior. This fixes
stale-height wall clearance while rising; analytical low-wall regression blocks
standing but permits the same movement on the first integrated ascent tick.

Do not confuse marioChkGnd2 with marioChkLandon8008dc70. ChkGnd2 normal
search passes constant.1engine as first argument (804208ec) and current velocity
as second; it reads current actorY and accepts -10<=rootY-floorY<0. It skips
when jump flag is set (8008f100–8008f104), so it is not the normal falling
landing implementation. Native falling instead corresponds to marioFall's
ChkLandon call80093bd0, which copies velocity into BOTH search arguments
8008dca8/8008dcb0. Bounds are therefore abs(v)+.01engine below and abs(v)+5
above pre-displacement root. ChkLandon is a predictive floor search before
fall displacement; this remains the native falling ray. The caller distinction
is now source-byte verified. A fixed small-depth replacement was experimentally
rejected by high-to-low regression because it skipped the floor; no such
replacement remains in runtime.

Remaining mismatch: falling overhead still follows derivative integration in
native advanceVertical rather than original pre-gravity overhead. Exact source
fall-search acceptance, slope/attribute branches, floor face culling and ascent
to falling transition sequence need further work. Grounded overhead checks and
legacy grounded stepping are unchanged. No Dolphin measurement/hardware claim.

## Falling overhead phase and clamp correction (2026-10-12)

Native falling now follows move, pre-gravity head check, fall derivative update,
predictive floor selection. marioMain8005b074 calls marioChkOverhead before
marioFall8005b078; fall derivatives begin80093b54. A falling bump clears jump
parameters/installs the normal fall tuple before the same tick's fall update.
The new regression inserts a ceiling at the pre-gravity head height during
descent: source contact clamps root then normal first fall displacement is-.003
native. Integrating before head search would miss this contact after downward
motion. A second regression falls below a higher takeoff floor, bumps overhead,
and confirms root stays below takeoff rather than teleporting upward.

Original overhead distinguishes ray extension from velocity: when ascent flag
is clear8008ca4c supplies10engine (.1 native) to the head query, while contact
acceptance8008c6a8 reads actual actor velocity. Native six-argument headCollision
uses explicit ray extension with actual-velocity cutoff preserved. Existing
five-argument query retains extension=velocity for ascent/test callers.
Overhead takeoff minimum is bypassed when fall flag bit14 is set
8008cbe4/8008cbe8; native falling bumps now use ceiling clamp directly. Ascent
retains takeoff minimum until its fall transition. Apex-to-fall ray extension
also switches to.1 when derivatives enter falling mode. Source sites verified
by tools/verify_actor_collision.py, not measured Dolphin behavior.

Grounded head check remains absent. Grounded step band, floor slope/attribute/
face filtering, exact special actor states, contact block events/SFX and party
physics remain unfinished. The generic.11 floor footprint snap threshold is
not complete source fall-search acceptance.

## Normal floor-candidate vertical acceptance (2026-10-12)

Replaced the generic symmetric.11-native footprint threshold with traced
marioSearchGround8008ef24–8008eff4 branches. Let d=rootY-floorY in native
units. If d>=.02 (2engine,804208f0), candidate is accepted only when
d<=abs(current vertical velocity). Otherwise abs(d)<=.11 (11engine,
804208b4). Below-root candidates at least2engine down may supply distance/angle
outputs without becoming a selected floor when outside fall reach; native
currently uses the acceptance decision only. Above-root and very close floors
use the11engine snap rule. Highest accepted candidate among five probes remains
selected. This permits terminal-speed18engine landings beyond generic11 while
avoiding an early snap to an unreachable lower surface. Analytical boundary
tests and height3.14→0 terminal-speed landing validate the native policy.

Byte verifier checks constant loads/branch sites and underlying source values.
This is source evidence, not measured Dolphin acceptance. Earlier attribute/
slope angle rejection branches remain incomplete; existing ground pack still
uses near-horizontal geometry selection and lacks source face-cull metadata.
Grounded.25-up/.5-down stepping remains unchanged. Grounded source call may
use different reach arguments and must not be replaced using this falling
caller alone. Raw research tool additions in the worktree belong to separate
work and are not part of this feature.

## Ordinary floor slope filter (2026-10-12)

Source marioSearchGround8008eb60 loads50degrees from80420954; special flag/
motion16 paths load75 from80420950. Ordinary native Mario now rejects the
nearest solid floor plane with slope>=50, before candidate vertical acceptance.
Original8008ed34 compares angle to limit and8008ed40 clears the hit pointer
when it reaches/exceeds limit. The current implementation uses atan2(horizontal
normal length,abs(normalY)); face orientation is deliberately separate pending
source cull metadata in the floor pack. It does not implement75degree special
actor states. Source angleABf8000703c uses a lookup-based angle calculation;
native standard atan2 can differ near the exact boundary. Do not claim bit-exact
source angle arithmetic. Source call computes angle from(horizontal normal,
-negative normalY); the downward ray's oriented hit normal matters.

Offline floor export no longer discards normals with |normalY|/length<.65.
That cutoff was stricter than the50degree limit and removed49.5–50degree
surfaces. All nondegenerate projected surfaces with ratio>1e-6 now survive
conversion; runtime chooses nearest solid ray hit then checks its slope.
Consequently steep geometry can block a query instead of allowing a lower
plane to win. Core tests cover49.7degree import,50.1degree rejection and steep
over-flat masking. Original full-scan oracle independently compares normal
cone via squared cosine, not the runtime atan2 helper. This proves query/index
consistency on the original packs, not original face culling or Dolphin fidelity.

Next floor format must preserve original cull byte and collision winding.
Use source hit_topology/head_geometry ordering, not render topology; mode1
rejects downward ray when normalY<=0. Other modes are two-sided and source
orients the hit normal. Grounded stepping and slope animation remain unfinished.

## Active downward face culling (2026-10-12)

New .ground records preserve source hit_topology winding and source drawmode
byte1. hitCheckVecFilter tests mode==1 and rejects dot(normal,ray)>=0. For
downward rays this is normalY<=0; native rejects these triangles BEFORE nearest
solid-hit selection. Modes0/2/3 are treated as two-sided, matching the source
mode1 condition. The slope helper uses absolute normalY, equivalent to the
slope of a two-sided hit oriented upward; it does not expose signed contact
normal for animations yet. Floor/head meshes now share original strip ordering
while render geometry retains GX ordering.

Analytical tests cover one-sided upper underside passing through to lower
front-facing floor, reversed upper winding becoming solid, two-sided upper
face acceptance, invalid cull/truncated pack retention and rollback carrying
cull metadata. Legacy packs remain two-sided for compatibility. Original
full-scan floor oracle includes face rejection before selecting nearest plane
then independent slope cone selection. Both original-room checks and native
harbor stair traversal pass; they do not establish full Dolphin/hardware fidelity.

Remaining grounded.25/.5 band, original angle lookup arithmetic/special states,
signed normal/animation outputs, wall push-out/sliding, dynamic contacts and
original block/SFX events need further work.

## Grounded height bounds and upstairs motion (2026-10-12)

Recovered marioChkFrontStep8008926c, normal caller8008f9e8 and motion12
entry803100c0→mot_upstairs8009b158. Forward probe distance is base width20
*.265=5.3engine (.053native); origin root+half height18.5; ray descends through
that half-height plus11+.1 engine units. Caller accepts rise5..11engine
inclusive and surface angle<=30 (80420928). mot_upstairs clears jump parameters,
sets counter3, velocity=(targetY-rootY)/3 and adds velocity on each of3 updates.
Original source sites/constants are byte-verified. No Dolphin timing capture yet.

Active native movement now blocks center-query rise>.11; drops>=.02 require
foot support or start falling rather than snapping through the legacy.5 band.
Rises.05.. .11 on slope<=30 enter FieldStep, which updates actual root height
over3 simulation ticks. Movement continues with wall queries at interpolated Y;
head check follows horizontal movement during step and can cancel to falling.
Saves/warps/jump/interact are guarded during interpolation; lifecycle reset
clears the state. Smaller rises/near drops still snap. Arrival/loader query
retains broad groundAt band separately; partner still uses legacy stepping.

Important adaptation: current trigger uses candidate floor under movement,
not the original forward probe. Full marioChkGnd reach/quantization/distance/
angle outputs and motion-specific XZ behavior remain incomplete. Step shadow
and camera follow native root interpolation; original stair pose/choreography
is unmeasured. Ceiling cancellation followed by floor acquisition still needs
matched source/geometry testing. Do not describe this as complete grounded
physics. Host tests cover3 updates, save/jump suppression during rise, blocking
.2 ledge and falling from.08 ledge after foot support ends.

## Active forward stair probe (2026-10-12)

Replaced candidate-floor stair activation with a query after horizontal move.
Probe center=p+normalized move direction*.053native (20engine*.265). Downward
ray begins root+.185 and ends root-.111, from half-height18.5 plus11+.1engine
reach. It uses existing source attribute/face filtering and nearest-hit policy.
Ordinary upstairs caller requires5..11engine rise and slope<=30; native starts
the same3-tick FieldStep from the current root at this stage. Movement does
not snap into a5engine-or-larger rise before this query. Core anticipation
fixture begins atx.07 on lower floor, advances topositive x while forward ray
sees upperx<=0 surface, and enters upstairs before crossing the edge.

Raw frontStepGround exposes height/slope, not original drop-distance/angle
outputs or all marioChkFrontStep state branches. Applying the caller rise and
angle checks preserves ordinary stair activation; other consumers remain
unimplemented. Native direction is normalized floating input, while source
rounds its movement angle; exact source-angle arithmetic is still pending.
The query's50degree surface rejection is stricter than raw front output but
all accepted ordinary steps must already satisfy30degrees. Special transformed
actor states/force vectors, grounded contact quantization/five-probe ray,
stair-specific XZ speed and low-ceiling cancellation remain unfinished.

## Active five-probe grounded support (2026-10-12)

Normal marioChkGnd8008f838 passes11engine search depth and current actor
vertical velocity to marioSearchGround. For ordinary stationary/walking state
(v=0), native groundedFootprint uses root-.1101 to root+.05. Five rotated
center/cardinal probes use baseWidth20*.375=7.5engine radius; source nearest
solid/face/slope filtering precedes vertical candidate acceptance. Highest
accepted result across ALL probes wins, rather than center-first fallback.

Game now keeps XZ movement separate from support: wall checks/coarse tall-ledge
guard run during movement; forward stair activation and five-foot selection
run after full XZ. Selected raw height and contact update once. Missing support
starts falling. Broad groundAt remains for entry/loader/partner and the coarse
unwalled-ledge guard; its.25/.5 band no longer snaps main player's root.
Forward probe still observes pre-support root, preserving source stair order.
Edge fixture on lower floor now adopts a higher valid side-probe floor even
when center sees lower ground. Rotated footprint and5engine upward reach are
tested, alongside existing stairs/drop/landing cases.

Important quantization distinction: source8008f898 writes rounded selected
height to actor+0x1c4 (floor cache);8008fae4/8008fae8 assign raw search output
to actual actorY+0x90. Native root remains raw; a separate source floor cache
is not currently represented. Applying that quantization directly to root
would be incorrect, especially for negative heights due truncation semantics.
Byte verifier now proves both stores. Nonzero residual vertical velocities/
special flags, exact angle rounding, full cache/contact outputs, partner probe
policy, grounded head checks and source wall push-out remain unfinished.

## CORRECTION — GameCube overhead response motion gate (2026-10-12)

This supersedes the earlier falling/upstairs clamp interpretation in this file
and commits048ff7a/7857a24. Reviewing the complete response branch shows
8008cc9c reads motion ID;8008cca0 accepts3,8008cca8 accepts6;8008ccac
branches past response for all other IDs. Table80310030 identifies0 stay,
1 walk,2 dash,3 jump,6 jumpStand,10 fall,12 upstairs. Source overhead still
queries/stores contact for grounded/falling/upstairs, but these ordinary motions
do NOT clear jump parameters, force falling or assign clamped actualY here.
The previous tests proved our implementation behavior, not this source gate.

Native normal jump now applies response only while representing motion3;
apex transition/falling maps to10 and records query without response. Upstairs
and standing/walking similarly record overheadContact only. The bool is a
current-frame diagnostic, not a complete original head-object pointer/cache.
Grounded contact runs after horizontal move and before floor selection; falling
query retains pre-gravity placement. The source gate helper also documents6
for future jumpStand; that ability is not implemented yet. Jump-only takeoff
minimum/clearance remains intact for ordinary ascent. Original SFX342 mapping
and earlier special-mode/attribute branches are still incomplete.

New synthetic tests explicitly distinguish query from response: low roof does
not interrupt ordinary standing/walking/upstairs; contacted falling trajectory
matches an unobstructed control tick; normal rising jump still bumps/lands.
Source verifier proves response comparisons/skip instruction and motion-table
pointers. Do not infer Switch parity: README's active documentation reference
is Switch1.0.0 static research, whose corresponding field contract remains
untraced. This corrects the existing GameCube-native prototype comparison.

## Ordinary air momentum and corrected jump startup (2026-10-12)

The [overworld movement contract](../behavior/overworld/OVERWORLD_MARIO_MOVEMENT.md) supersedes
this ledger's earlier first-integration-at-counter3 interpretation. mot_jump's
launch call decrements countdown2 to1 and records counter1 with phase0; the next
call enters phase10/counter2. marioJump gates on phase<10, not counter<3. Released
A shortening still begins at counter4. Native FieldJump now integrates on the
first update after begin. With flat equal-height support its standalone tap/held
landing predictions are22/26 updates after begin, and four-update heights are
.22922/.245 native units. These are host source-derived predictions, not observed
original-game timing. The native launch enters air movement before horizontal
collision and preserves a zero-height launch invocation without consuming an
extra derivative update; it still performs the overhead query on that call.

Plain ordinary-air movement retains base speed/travel direction and adds current
stick drift (capped70, times .015 times .25, converted at scale.01), then caps
combined norm at ordinary dash speed. Neutral input preserves running momentum;
full same-direction input on a walking jump gives .012625 per native tick rather
than selecting dash. FieldMovement supplies this portable plain/no-badge/scale1
policy to Game. Existing native vector/hypot math represents original angle/trig
services; bit-exact numerical behavior is not established. Simultaneous idle/
threshold/transform jump arbitration, movement-rate/slopes, wall deflection,
modifiers, physical input conversion and original execution traces remain open.
No Dolphin observation or art/audio conversion was performed for this change.

Probe orientation is separate from current input and combined displacement:
nonzero retained base selects travel+1a4; zero selects facing+1ac through
 toMovedir. Ordinary native probes now keep retained travel during air drift,
with native left/right facing as their zero-base conversion. The nonzero ground
heading-offset and special motion19 branches remain unbound. An analytical
ceiling-edge regression distinguishes retained-right probes from diagonal
current-stick probes. The documentation verifier now pins19 containing bodies,
29 anchors and six constants for this local movement/probe scope.

## Directional wall movement and surrounding correction (2026-10-12)

OVERWORLD_WALL_MOVEMENT.md now supplies the original ordinary moving-wall
contract. The prior circle endpoint/X-then-Z player policy is superseded for
loaded collision rooms by whole-vector forward/side queries and two surrounding
correction passes. Forward heights remain26.6400013/11 original units, adding
.1 when the signed motion counter is nonzero. Around uses11 and bodyHeight-1
(normally36); its two passes use8 directions when view flag+14 bit0 is clear,
otherwise2. Ordinary init clears that flag; later view writers remain open.

Forward normals/distances use source.001-unit signed/FMA rounding. Closest tie
retains the first ray; slide is half the full displacement's tangent component.
The +45 guard stops on first hit, while the -45 guard uses its final ray token,
including null. Attribute pass-through is a separate wrapper decision. Around
filters pass-through hits before closest selection, caps only the first summed
push (ordinary2.5, falling5 for width20), then admits half the UNcapped second
push only for positive dot with the capped first. Staying probes disposable
positions, so neutral ground input does not move Mario through this wrapper.

Native FieldWall policy uses original engine units; its complete .head-mesh ray
provider preserves source winding/cull/attributes and admits oblique geometry
without the legacy .walls slope cutoff. The provider scales into cached native
geometry and performs float triangle intersections; original hit-driver math,
scene traversal, dynamic contacts/named NPC owners and hazard services remain
unbound. No original execution or numerical equivalence is claimed. Normal
contact metadata/distance are separate from the blocked/sliding position result.
Native fallback geometry/placeholder bounds and partner eligibility queries remain
separate adaptations. Around can keep the body on an upper ledge when an adjacent
wall lies inside its clearance; the old overlap fixture's lower-landing expectation
was superseded with a clearance/support regression, with float-height tolerance.

## Plain launch arbitration and published field input (2026-10-12)

OVERWORLD_MOTION_ARBITRATION.md closes the plain idle/walk/dash simultaneous
A/input request paths. A trigger is not consumed by the repeated dispatcher:
threshold/neutral requests can replace pending jump, but the next ground
callback retries A. Current neutral/walk/dash base and current input heading
therefore finish BEFORE surviving jump entry. Native Game now selects this
plain ground state before begin while discarding ground displacement, then
uses air displacement on the zero-height launch call. Standing/current weak
input .5 launches with .0113125 native displacement, not drift-only.0013125.
All nine idle/walk/dash-to-neutral/weak/strong paths and current-heading turns
have source-derived host regression coverage. Airborne/landing-call A triggers
are no longer replayed after physics; ordinary late landing requests dispatch
next call, and a new grounded trigger is required for another launch.

Physical Explore input now reads published signed-byte GP axes and A held/edge
words after the existing PhysicalInputProducer update. The source integer
squared-radius>3025 classification is preserved even for equality33²+44²=55²;
raw byte55 remains walk when the preceding native float exceeded55/70 slightly.
The producer's global input-clear chord also reaches field movement. Native
Circle Pad device mapping and per-axis18 deadzone remain existing adaptations;
this consumer does not add a new radial deadzone. SDK sampling/clamp, system/
contact/talk/action/pose gates, external motion, transformation/badge/background
modifiers and original runtime comparisons remain open. The plain semantic
reduction is not the complete original Mario motion/pose/audio dispatcher.

## Direction-dependent movement rate after the air cap (2026-10-12)

OVERWORLD_DIRECTION_MOVEMENT_RATE.md distinguishes directional gain from terrain
slope scaling. marioMoveMain calls marioGetMoveRate with retained travel heading
+1a4 after the ordinary air dash cap, then multiplies the combined norm before
wall movement (unless roll flag0x01000000). Rate is fused1+
.19000005722045898*abs(cos((retainedHeading-cameraOffset)*pi/180)); there is no
nonzero-base gate and no second dash cap. Native FieldMovement now applies this
gain after base/drift/cap composition and before the wall solver. Its cached
unit-vector/trig representation remains a numerical adaptation; heading offset0
corresponds to this port's current fixed Z-facing camera, not a recovered constant
original Rogueport camera yaw. The source pre-camera offset owner is documented,
but its actual camera-zone/script binding remains unimplemented.

In this explicit camera domain, X travel retains base rate, depth gains about19%
and45-degree diagonal gains about13.44%. A depth dash after the cap is approximately
.026775 native per tick. Older normalized-diagonal/no-direction-gain ledger notes
are superseded;1/2.25 original rates describe retained BASE speeds. Host tests
cover axial/diagonal gain, camera-offset rotation, retained heading versus current
orthogonal air input, post-cap ordering and native depth displacement. No original
measured speed/camera/trig trace is claimed. Terrain angle-record scaling, stair
retained travel/third-call reentry, support-loss motion10 versus11 and their tuple/
drift ownership are separate pending contracts and implementation tasks.
