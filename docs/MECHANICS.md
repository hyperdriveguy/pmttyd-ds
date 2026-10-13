# Mechanics evidence ledger

Original-game mechanics evidence for the US GameCube G8ME01 binary. This ledger keeps the status, the provisional parameter warning and the observation backlog; long-form dated evidence sections live in the chapters below. New Dolphin or original-runtime observations are appended to this ledger (or to the matching chapter) with revision, inputs and frame references. Static source/DOL inspection must stay distinguished from emulator observation.

## Evidence chapters

| Chapter | Contents |
| --- | --- |
| [Camera](mechanics/CAMERA.md) | Camera collision/shift, authored zone geometry, straight paths, retained targets, native camera pipeline binding entries |
| [Field movement, collision and roads](mechanics/FIELD_AND_ROADS.md) | ROAD contact, head/ceiling/floor queries, slope/stair/support filters, air momentum, wall movement, launch arbitration, movement rates |
| [Mario presentation](mechanics/MARIO_PRESENTATION.md) | Facing/mesh presentation, sparse animation playback, idle/key service ownership, grounded shadow request |

## Status

Dolphin booted the original ISO in an isolated profile and captured its initial
health/safety screen. No gameplay capture has yet established timing or exact
tutorial rules.
The code is a playable development harness, not a verified faithful recreation.
Keep this distinction in future reports. Do not treat test expectations as
observations of the original game.

## Provisional parameters

`BattleRules` centralizes Crump HP 5, attack 1, jump impact tick 48 with five
preceding accepted ticks, hammer release tick 60 +/-6 ticks, enemy impact tick 54
with five preceding guard ticks. A successful jump or hammer deals 2; failure
currently deals 1. Guard reduces damage to zero. Early jump/guard attempts cannot
be repaired by mashing. A mushroom heals 5 HP and consumes a turn. These values
and the permissibility of items/hammer/guard in the introductory encounter need
original-game validation. Defeat retry heals Mario; this is prototype behavior.

## Required observations

Observation backlog flagged as todos; each item links to the detailed
evidence or requirement below. Record original-game observations in this
ledger with revision, inputs, frames and profile details — static source/DOL
inspection is not an observation.

Top-level captures:

- [ ] Docks encounter capture in Dolphin (isolated profile): mandatory versus
  later combat, initial stats, available commands, jump landing window,
  hammer hold/release, guard/superguard availability, enemy attack, victory
  rewards, dialogue and whether defeat is possible normally.
- [ ] Rogueport plaza capture: movement, camera, jumping, partner follow,
  interaction range and transitions.
- [ ] Harbor stair/port traversal measured in Dolphin (0.1 risers, wall foot
  clearance, per-node collision attributes); the current port rule is
  provisional and unmeasured.
- [ ] Harbor water/fall/recovery sequence observed in Dolphin (mot_damage
  timing, HP penalty, safe-return selection); source-derived stage values are
  not acceptance evidence.
- [ ] Road fade timing/choreography and the original exit event's mov_pos2
  travel and wait observed before replacing provisional native transitions.

Detailed observation requirements:

- [ ] [Ordinary road scripted travel](#ordinary-road-scripted-travel)
- [ ] [Normal-Mario wall attribute filtering](#normal-mario-wall-attribute-filtering)
- [ ] [Normal wall clearance and sampling heights](#normal-wall-clearance-and-sampling-heights)
- [ ] [Wall triangle slices](#wall-triangle-slices)
- [ ] [Ordinary movement rates](#ordinary-movement-rates)
- [ ] [Ordinary field jump integration](#ordinary-field-jump-integration)
- [ ] [Landing floor ray and world-height update order](#landing-floor-ray-and-world-height-update-order)
- [ ] [World-height landing integration and unsupported edges](#world-height-landing-integration-and-unsupported-edges)
- [ ] [Bottomless/void detection and return (2026-10-12)](#bottomlessvoid-detection-and-return-2026-10-12)


Use the US GameCube ISO in Dolphin with an isolated profile. Record exact game
revision, controller mappings, input timestamps, video frames, and context.
Capture the docks encounter and distinguish mandatory tutorial behavior from
later normal combat. Confirm initial stats, available commands, jump landing
window, hammer hold/release logic, guard/superguard availability, enemy attack,
victory rewards, dialogue, and whether defeat is possible normally.

Also capture Rogueport plaza movement, camera, jumping, partner follow behavior,
interaction range and transitions. Reference Luigi's Mansion 3DS for handheld
text size, menu readability and first-party presentation, without assuming its
implementation details.

Touch menus are an adaptation: button access remains available. Timing-meter
assists, enemy HP display and replacement dialogue are development aids to
remove or revise after fidelity comparison.

Harbor traversal evidence: source collision stairs contain0.1 native-unit
risers after the established0.1 map conversion scale. Native wall foot clearance
is now0.25, matching the prior ground-ascent allowance. This is a provisional
port rule that enables original stair geometry; it has not been measured in
Dolphin. Solid dock floor at y0.15 is traversed and retraced by host/Azahar
checks. Original water/fall/recovery behavior and per-node collision attributes
remain unfinished; do not treat walking on all exported surfaces as accepted.

Original US DOL floor attribute evidence (inspection, not Dolphin observation):
marioLandOn0x80092c94 gets current floor hitGetAttr at80092de4;80092df4 tests
PPC bit22 (mask0x200). If set and current motion !=31, it calls
set_damage_root_ypos0x8013b600 using Mario Y at80092dfc/80092e00 and enters
motion31 at80092e58–80092e68, with additional state checks in between. Original
marioMotTbl index31 maps to mot_damage0x8013ae04. This proves a damage-motion
branch, not full recovery timing, penalty, effects, or safe-return selection.
Next inspect mot_damage/mot_bottomless and observe falling from harbor in
Dolphin before reproducing the complete sequence.

Water-damage trace continuation (original US DOL bytes, not emulator observation):
* mot_damage8013ae04 uses Mario state+0x44 and counter+0x48. Original jump table
  at8036ed4c maps states0/1/10/11/12/13/14/20/30/31/32 to
  8013af84/8013affc/8013b028/8013b06c/8013b350/8013b3c4/8013b3e0/
  8013b438/8013b544/8013b554/8013b5b8.
* State30 assigns counter50 then falls through into state31 decrement. At
  expiry8013b564–8013b578, character byte+0x3c==0 calls pouchAddHP(-1), then
  pouchGetHP is checked against0. A positive HP path assigns state32/counter12,
  immediately decrements it, and eventually calls marioChgMot(0). Zero HP
  takes state33 instead. Do NOT substitute a minimum-1-HP penalty.
* pouchAddHP800d3d3c itself clamps between0 and max HP. These are literal
  counters and control flow, NOT measured complete recovery frame timings.
* Return selection at8013b114 checks saved hit pointer+0x208 and uses
  hitGetDamageReturnPos80013ef0 when present. Otherwise map-specific fallbacks
  (muj_00/gor_00/gor_04) and an additional saved-vector branch apply. Need trace
  writers of+0x208 before choosing which floor should be remembered natively.
* evt_hit_damage_return_set8007458c iterates16-byte name-pointer+XYZ records.
  Sentinel Y=-1000 calls hitObjGetPos8001449c. hitObjGetPos averages transformed
  leaf hit-object centers through hitObjGetPosSub800145ec; it is NOT the player
  position or a guessed bounding box of the whole room. Resolve only after
  retaining the full original hit hierarchy and leaf centers.
* hitGrpDamageReturnSet80013efc finds named hit group; hitDamageReturnSet
  80013fc4 sets attribute0x40000000 and binds the return record to the owner
  and descendants, preserving owner siblings when initially called with flag0.
  These init mutations are currently missing from native contact attributes.
* __init_registers8000539c establishes r2=80427440 and r13=8041cf20, useful for
  resolving original small-data constants without guessing.

Original init table calls are outside IF/SWITCH blocks at gor_00:0x2f364 and
gor_02:0x4d6d0 (END_SWITCH is opcode49). Harbor table0x2c594 has5 entries,
east0x4c9d4 has12; all17 source entries and5 sentinels are retained by extraction.
Harbor authored A_yuka_03 return=(84,15,220), A_yuka_04=(475,15,-180); units are
original GameCube engine units. Later _mapLoad inspection below corrects their
conversion to native units:0.01, because the original model-to-engine scale is10.
These points do not authorize teleporting from every water tile indiscriminately.

Original coordinate-space correction: _mapLoad80028f18 at800294dc loads the
constant from r2-31484=8041f944 (read original DOL float10.0), creates uniform
PSMTXScale10 at800294ec, and passes that matrix to mapEntry at800294fc and
hitEntry at80029514. The native renderer/collision currently uses map-file
model units times0.1; event engine-unit literals therefore use0.1/10=0.01.
Harbor A_yuka_03 authored(84,15,220) becomes(.84,.15,2.2), A_yuka_04 becomes
(4.75,.15,-1.8). This fixes the prior incorrect0.1 event-unit assumption before
any recovery teleport was implemented. Model-space leaf centers already baked
into native coordinates must NOT receive the extra division.

Original _hitEnt: mesh-owner center=(bboxMin+bboxMax)/2; no-parts owner uses
local translation at80018af4–80018b08. hitObjGetPosSub averages transformed
leaf centers, excluding the group's top-level siblings. Native conversion now
retains all collision hierarchy owners (including A_ground_01) and applies
ordered init bindings to the owner/descendants. Dynamic hit transforms and
repeat event execution remain outside the current baked init state.

Saved return-hit writer recovered: scanned original DOL text for stw+0x208,
then inspected marioMoveMain800594a8. At80059714 it reads current floor+0x1e8;
null skips the latch.80059724 calls hitGetAttr;80059728 tests PPC bit1 (mask
0x40000000).80059730/80059734 copies that floor to saved-hit+0x208 before
horizontal movement calculations. Other floors do not clear it. Native now
remembers the same eligible contact at the start of its movement tick and
resolves its authored position through the active room's binding map. Original
jump/contact and transformation behavior remain broader physics work.

Normal ground-query filter: marioSearchGround8008e694 chooses chkfilterVec
800915e4 except special motion28/Vivian or roll-state cases. The default callback
first accepts any0x00080080 bit, then rejects bit4 unless0x00800000 is set.
Later0x80000000/name-dependent handling does not occur in the current five raw
floor inventories. Native applies the recovered bit4 rule; currently excludes
two west-room trigger floors. It is not a full partner/transformation/wall filter.
Ship_Change02's0x8000 is NOT rejected by this normal bit filter.

Actual original-harbor host path right30/diagonal80/down80 crosses Ship_Change02
after A_yuka_04, then A_mizu water. The last eligible contact therefore yields
Ship_Change02 automatic return(5.5,.05,-1.9), retaining it while over water.
Initial test expectation A_yuka_04 was contradicted by that source floor path
and corrected; do not force a preferred floor or point. This is verification of
native contact/latch inputs, not an observed original harbor water sequence.

Further water trace: mot_damage initialization calls marioNoUpdateCamPos
801a8f34 at8013ae90 and npcStopForEvent80043cd8 at8013af5c; completion calls
marioUpdateCamPos801a8f60 and npcStartForEvent80043bc8. Water recovery must
retain follow publication via display10000 while camMain continues. The older
camera-freeze inference was incorrect and is superseded by native8ee75c4.
npcStopForEvent flags selected event NPCs; it does not skip the NPC manager or
universally stop every actor/partner.

Native water controller now implements ordinary mot_damage stages0/1/10/11/
12/13/14/20/31/32/33: sink -6 engine units/tick until below root-160, hold40,
rise20 decreasing by1 with POSITIVE minimum10 (not negative terminal velocity),
then travel speed8 for integer-truncated horizontal-distance/8 ticks. No exact
XZ teleport is introduced: remaining distance is less than one travel step.
Zero-distance travel uses10-tick branch14. Fall uses original marioJumpData
+0x10 four floats(-.1,-.2,.024,-.004), jerk clamped nonpositive, terminal-18
then mot_damage's additional-10 fall-step cap. Values were read from original
DOL data; original source headers are not substituted for runtime values.

Landing falls through assigning50 and decrementing on that same tick, yielding
49 remaining hold ticks. HP-1 then positive HP assigns12 and decrements on the
same tick, yielding11 remaining ticks. HP0 enters state33 instead of healing.
Current zero-HP UI reloads a valid existing checkpoint without changing its HP;
original game-over presentation remains unfinished. Original gor_00 fallback
(0,0,-200) maps to(0,0,-2), gor_04(400,35,-100) maps to(4,.35,-1); other packed
ordinary maps use zero vector. Special state/saved-vector/character cases remain
unsupported. Normal water trigger requires current prototype jump to finish.

Source-derived mechanics pass portable/native checks but are NOT Dolphin-
observed acceptance. Original water splash/chomp effects, pose sequences, sound
macros, final landing ray, body widths and exact float/timing boundaries remain
required fidelity work. Source retained follow targets and camera-manager
advancement replace the earlier frozen-camera checks; full original NPC/partner
event and animation behavior is not yet reproduced.

Normal gravity parameter confirmed: original marioMain8005a944 writes1.0 to
Mario+0x228 each frame (DOL constant80420008). Only listed moo moon rooms
replace it with0.5 at8005a9d8; current Rogueport rooms use1.0. marioResetGravity
80096988 also writes1.0 (80420a74). Thus the implemented normal fall table
selection is source-supported for this slice, rather than inferred from zeroed
initial memory. Special gravity overrides remain out of scope for these rooms.

Road fade source evidence: marioStInit8006fc48/4c/54 sets type9 incoming,
300ms duration, type10 outgoing; stored gp+0xf8/fc/100/104 and second-group
copies. seq_mapChangeMain80007d04/08/0c reads outgoing gp type/duration and
calls fadeEntry; resets default type10/duration300 at80007d34–80007d54.
Default color at8041f4cc is original RGBA000000ff (black). evt_fade_in80136114–
128 uses type9 and original black color80422a80. fadeMain jump table8030468c
case9 points800120b8 and case10 points800121c0, using linear elapsed/duration
progress with inverse for incoming. Native uses18 fixed60Hz ticks, extending
black coverage to both 2DS screens. Complete timing/choreography is not yet
Dolphin-observed.

Road event data: bero_case_exec_road8030dac8 disables controls and calls
evt_mario_wait_landon at8030db30 before scripted movement. Native now waits
for its prototype jump to finish before fading/loading. Original exit event
also performs mov_pos2 travel and150ms wait before mapchange; that movie travel
is still missing from native transitions.

Further distance evidence for next implementation: bero_get_position_normal
80079b74 uses source hit-object center when position100000 sentinel is set,
and source constant30 at80420518 for directional start offset. End offset
uses30 plus supplied float length; mode1 reverses direction by180 degrees.
Road exit caller supplies FLOAT150 to both get_position_normal and
evt_mario_mov_pos2. mov_pos2 divides that rate by gp framerate (150/60=2.5
engine units/tick, native0.025); tick count uses(distance+1)/rate and final XZ
assignment. Confirm full initial-position/entry-event sequence and supporting
floor/ray behavior before replacing provisional native .9 inset/.7 sensor.
Road entry event sets party homing distances20 then50 engine units; current
follow model and entry placement have not yet adopted those event modes.

### Ordinary road scripted travel

Read-only US DOL inspection: `evt_bero_get_info_length` (8007a76c)
returns default30 engine units for Road type0 length-1, not150.
`evt_bero_initial_pos` (8030f060) calls `bero_get_position_normal` in
mode1 with that entry length. Its start is marker center minus30 in the
inward direction; its endpoint is center plus30. With engine/native scale
0.01 this is 0.3 outside to0.3 inside, traveling0.6.
`evt_mario_mov_pos2` (800e5f50), called with FLOAT150 by
`evt_bero_info_run_road` (8030e97c), uses150 engine units/s divided by
NTSC60:0.025 native/tick. Tick count truncates(distance+1)/speed and the
final tick snaps XZ to the endpoint. Ordinary entry takes24 ticks.
`bero_case_exec_road` (8030dac8) waits for landing, computes mode0
position with length150 (1.5 native outside), travels at the same rate,
then waits150ms (9 ticks) before map change. The road-ending party homing
uses FLOAT50 (0.5 native).

Native implementation now follows these distances and rate. Remaining
limitations: .7-native trigger radius, fixed support height during scripted
travel, simplified camera, stationary companion on departure/co-located on
arrival, omitted first-entry branches and original party choreography.
Normal collision body dimensions and sensor activation still require source
recovery; these changes do not establish faithful general movement physics.

### Normal-Mario wall attribute filtering

US DOL `marioHitCheck` (80090e80; callback setup80090f8c–80090f98)
passes `chkfilterVec` (800915e4) to its ordinary wall checks.
The callback first accepts attributes with mask0x00080080, then rejects
bit0x4 unless bit0x00800000 is set. This is the same ordinary-Mario attribute
rule already applied to floors. Native wall packs now retain flags/source owner
IDs and apply this rule at query time rather than making sensor walls solid.
`tools/verify_wall_attributes.py` compares every exported record directly to
original drawmode bytes, and asserts no bit31 cases exist in these five packs.
It finds81 rejected records among5,264 records. Other transformation callbacks,
name-dependent high-bit cases, runtime event flag changes, swept triangle/body
collision and original body dimensions remain unfinished.

### Normal wall clearance and sampling heights

`marioInit`8005b72c/734/774 stores original width20,height37,current width20
(engine units). Original constants80420088=20,804200c0=37.
`marioHitCheckWidth`800916f0 restores current width to base width for ordinary
Mario. `marioCheckWall`8008b88c passes current width as f3 to
`searchFrontWall`8008a2f0. That helper retains f3 asf26 and multiplies it by
0.5 at8008a544: ordinary forward clearance is10 engine units, not20 or28.
It adds movement distance to this clearance at8008a558.
The ordinary grounded branch8008a4dc–8008a4f4 uses two Y offsets:
8042092c=26.6400013 and804208b4=11. Native scale0.01 gives clearance0.1
and ray heights0.2664/0.11. Nominal height37 is not itself the wall-ray height.
`python3 tools/verify_actor_collision.py` independently reads these constants
and key instruction sites from the read-only ISO (no original code execution).

Native queries now use that clearance and the two height samples, including
jumpHeight when testing Mario's airborne wall clearance. Original force/ray
movement, triangle intersections, contact-normal sliding and other Mario-state
sample tables remain incomplete. XZ segments still summarize each triangle's
full min/maxY, which can overestimate collision at sloped edges. Native floor
step/query allowance0.25 and prototype jump trajectory remain provisional.
Partner shares this normal-Mario proxy rather than its own recovered collision.

### Wall triangle slices

Replaced native min/max-height rectangle approximation with horizontal slices
through each retained original wall triangle at the recovered normal-Mario ray
heights. This removes collision in empty triangular bbox regions and retains
actual sloped edges and vertex endpoints. Source triangle positions are preserved
through conversion instead of deduplicating different triangles by their longest
XZ segment. This is a geometry correction within the current clearance query;
it does not reproduce original swept direction rays, backface policy, contact
normal sliding or dynamic meshes. Angled wall selection still uses the existing
near-vertical normal threshold and needs separate original-ray coverage work.

### Ordinary movement rates

US `marioInit`8005b6e0–8005b6f0 stores walk1.0 (80420008) and ordinary
Mario dash2.25 (804200b8) at offsets184/188. The alternate character branch
uses dash2.0; it is not the current Mario target. `mot_walk`80087aa4 loads
walk speed; `mot_dash`800877c4 loads dash speed and stores the adjusted speed
at180. `marioMoveMain`80059718 loads180 before ordinary wall/movement
resolution. Source forced/air movement also has stick-dependent drift branches;
those are not reproduced by this rate correction.
`N_marioGetMovementSpeed` / mot_dash use raw stick X²+Y²>3025 (55²) to
classify dash; equal55 is walk. `marioGetStick` caps magnitude at70 (80420a64).
Native Circle Pad normalized magnitude maps onto that70-unit range, selecting
walk at<=55/70 and dash above it. Native rates are0.01 and0.0225 per60Hz tick;
input direction is normalized so diagonal travel does not increase speed.

Original motion-state delay/hysteresis, airborne drift, special slow status,
moon/transform rules and partner-specific movement remain unfinished. Native
selection changes immediately; this adaptation preserves rates/threshold but is
not the full original movement state machine. Circle Pad dead zone is still the
existing18-unit device mapping. The source verifier now checks the speed floats
and original load/store/threshold sites as well as collision constants.

### Ordinary field jump integration

US marioJumpData80416110 contains ascent(8,-.84,.08,-.02), followed at+16
by fall(-.1,-.2,.024,-.004); fields are velocity,acceleration,jerk,jerk increment.
SetJumpPara dispatch for ordinary state3 (80310250) selects8009421c; ordinary
branch80094280 copies the ascent tuple. marioJump80093db0–80093dec updates
jerk,then acceleration,then velocity; gravity divisor is1 in Rogueport.
At rising velocity and counter>=4, released A adds-0.526 (80420a38).
When velocity crosses zero, marioJump replaces parameters with the fall tuple
and integrates it once on the transition tick. It adds velocity to world Y at
80093ff0–80093ffc. Subsequent fall uses marioGetFallSpd80094098: jerk clamps
nonpositive, terminal velocity-18 clears derivatives. Native units multiply all
four derivative values and release increment by0.01.

Portable FieldJump now follows this integration/hold rule. On static equal-height
support, tap lands after21 updates after begin, held after27; four-tick heights
are0.17882 and0.1946. These are source-derived native tests, not Dolphin timing
observations. Current game still uses relative support height and simple landing
clamp; original floor search, ceilings, dynamic support, alternate jump types,
gravity overrides and airborne horizontal drift remain unfinished.
Startup detail: mot_jump initializes countdown+0x48=2,phase+0x44=0 and
increments jump counter+0x50 on the begin frame. Later calls decrement the
countdown; expiry sets phase10 before jump counter increment. marioJump requires
phase>=10. FieldJump begins counter1, skips the next update at counter2, and
first integrates at counter3. Released-A reduction starts at counter4 (second
integration), not four integrations after launch. This refines the intermediate
22/26-tick estimates that omitted startup/counter ordering.

### Landing floor ray and world-height update order

US `marioFall`800939dc–80093a14 calls marioChkLandon with current vertical
velocity, stores the returned floor/contact and snaps world Y when a hit exists;
otherwise it adds velocity to world Y. Another ordinary fall branch calls at
80093bd0 and has attribute-dependent acceptance; preserve that branch when
integrating, rather than snapping every candidate. marioChkLandon8008dc70
forwards the velocity as both scalar bounds to marioSearchGround8008e694.
At8008e780–8008e7a4, the ray extends abs(v)+0.01 down and abs(v)+5 up
(engine units), with constants80420964/804208c0. Origin Y is worldY+up
(8008e8a4), ray length is up+down (8008e8ac). Native center bounds are
worldY-abs(v)-0.0001 through worldY+abs(v)+0.05.

Normal routine probes five positions (loop termination8008f044): center then
four rotated offsets, radius0.375*base width (80420978=0.375), normally7.5
engine units or0.075 native. Floor selection also includes slope and snap-distance
logic; a returned raw triangle intersection is not automatically a complete
landing decision. Original normal vs transformed filters differ.

Added landingGround center-ray primitive with these source bounds and preserved
original attributes/return bindings. This is prerequisite geometry work, not
integration of the original floor state machine. Game still uses legacy support
queries and relative jump height. Next integrate world Y, all five probes and
source candidate/contact acceptance, then edge falling and ceiling resolution.

### World-height landing integration and unsupported edges

Game now keeps takeoff world Y independent of later support, advances the source
jump/fall derivatives without clamping to the takeoff plane, and queries the floor
from the pre-displacement world Y. Accepted contact snaps to that floor; missed
contact clears the current floor ID and continues vertical displacement. Footprint
uses center plus four rotated offsets at normal radius0.075. Highest candidate
within the recovered generic11-engine-unit snap distance is selected. Orientation
uses the current native movement direction; original angle/slope adjustment and
special attribute branches are not fully reproduced.

Grounded movement retains legacy support stepping. When that query misses,
footprint support is checked; loss of all support starts the original fall tuple
instead of rejecting XZ movement. This lets Mario drop from the harbor entrance
onto its lower dock; walking back into the vertical drop cannot recover the upper
road, so stairs are required. A valid floor contact is not discarded merely because
horizontal wall clearance fails; original wall push-out/sliding remains necessary.
Save requests and START writes are suppressed while airborne.

Grounded0.25-up/0.5-down band, ceiling handling, slope filtering, missing-floor
void recovery, partner physics and shadow placement remain provisional/incomplete.
The new host unequal-floor cases are behavior tests, not Dolphin observations.

### Bottomless/void detection and return (2026-10-12)

US marioBottomless80096e8c compares world Y against80420aa8=-2000 engine
units (native-20), inclusive. Source follows optional BG/reload branches, otherwise
restores stored bottomless spawn and both partners then selects motion34.
N_marioSetBottomlessRespawnPosOnBeroEntry stores supplied position with37 units
added to Y (80420aa4). mot_bottomless probes a point300 units above its return
target, raises that point10 units while onscreen, traverses horizontally at8
units/tick, then falls using normal fall tuple with-10 per-step cap. On landing,
wait50 includes same-frame decrement; HP subtract1 follows, with12 injury ticks
for surviving Mario. Source verifier checks threshold/offset/screen/travel floats
and landing/injury counter instruction sites.

Native implements threshold, validated return target, offscreen return/travel/drop
and shared landing/damage/defeat delays. Offscreen probe uses current handheld
camera geometry (shared with renderer), preserving source300/10 increments.
Native camera recenters on return target; companion is grounded during the movie
and regrouped on completion because original NPC gravity is not yet implemented.
Return prefers a valid original remembered recovery binding, then native room/load
entry. Original dynamic fallback selection, reload-enabled/BG branches and exact
camera/partner choreography remain incomplete; these are adaptations, not claims
of full source equivalence or Dolphin-observed timing.

## 2026-10-12 — Native partner follow verification (not Dolphin evidence)

Read-only US binary checks verify the ordinary ground-homing jump-history and
wall-contact counter branches, plus the existing jump/fall/history/screen
recovery contracts. See [native follow and evidence boundaries](behavior/partner/FIELD_PARTNER_NATIVE_FOLLOW.md).
No new original-runtime observation was made. Source-derived coefficients are
static evidence; trail scheduling, terrain/path adaptation and native spacing
are not measured original trajectories.

Portable scenarios cover stationary and moving-history jumps, retained spacing,
gap crossing, ledge falls, thin walls, stale-path correction, persistent blockage,
capacity pressure and screen recovery. The native Game binding is included.
Isolated Azahar movement/save captures and original Goombella air/landing poses
are native verification only. Final captures are under ignored
`artifacts/azahar-partner-follow-spacing/`; profiles and saves are isolated.
Full partner floor/front-wall/placement and original sound-service composition,
Dolphin trajectory comparison and hardware behavior remain open.

## 2026-10-13 — Harbor partner fall and distance correction

Recovered ordinary ground-homing distance and history/overlap rules directly
from read-only US bytes; see [ground-follow contract](behavior/partner/FIELD_PARTNER_GROUND_FOLLOW_DISTANCES.md).
The45/50 values govern speed decisions, not a fixed resting gap. Target activity
normally stays nonzero at idle through the dash-base fallback. Native history
now consumes within13+speed original units and uses the source overlap box.

The old native harbor descent missed valid floor support, continued below the
dock and eventually used root-copy recovery. Source fall queries five probes
before gravity, uses homing loss acceleration-1 (stay loss-1.5), damps horizontal
speed0.2 and uses1.3*retained speed. Native regression at two deck positions now
admits an actual jump, lands at0.15 and records zero resets over300 ticks.
Temporary isolated native CSV tracing also showed minimum partnerY0.15 and zero
resets. Tracing was removed from the release build. These are native observations
and static original contracts; no original Dolphin timing/trajectory was measured.

Final isolated captures in `artifacts/azahar-partner-harbor-final/` sample the
brief fall window. `partner-dock-side-view.png` shows grounded Goombella beside
Mario after a small lateral reveal; a straight-line view can naturally hide her
behind Mario. No depth-test override, fabricated motion cue or generated art was
introduced. Original-runtime trajectories, full terrain/placement/state-service
composition and hardware behavior remain open.
