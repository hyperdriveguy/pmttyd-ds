# Mechanics evidence ledger

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
