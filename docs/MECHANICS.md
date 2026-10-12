# Mechanics evidence ledger

See [ORIGINAL_BEHAVIOR.md](ORIGINAL_BEHAVIOR.md) for a subsystem coverage index,
identified US revision/DOL hash, and new static evidence for sequence ordering,
event scheduling, command continuation and wait behavior. That reference keeps
binary evidence separate from public reconstruction, observations and port
proposals. This ledger is chronological: later implementation updates supersede
earlier statements that a feature is missing.

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
freeze the field camera and NPC updates instead of following Mario's airborne
recovery motion. Existing partner follow/camera does not yet implement this.

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
required fidelity work. Camera/follower positions freeze in controller checks;
full original NPC event/animation behavior is not yet reproduced.

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
