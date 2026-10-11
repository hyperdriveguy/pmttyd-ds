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
