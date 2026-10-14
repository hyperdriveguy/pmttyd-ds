# Item95 Mario motion and control provider

Source: [motion15/stay contract](ITEM95_MARIO_MOTION15_ENTRY_DISPLAY_AND_STAY.md).
Runtime ownership: [Game acquisition](GOOMBA3_GAME_ACQUISITION_BINDING.md).

`field_item_mario` provides finite ordinary requests against the existing
FieldJump, FieldMovement and MarioPresentation owners. Get/stay update the
current/retained motion before the old-motion exit callback, then queue entry.
They do not request a pose immediately. Ordinary rows0/10 require an injected
idle/fall sound-exit provider; rows1/2/3/11/15 have null posts. Missing exits
refuse before mutation. Game candidate admission and pending talk admission
both check the get-motion provider frontier.

A later admitted Mario presentation visit consumes motion15 entry before that
visit's item work. It clears countdown48, counter50, phase44 and horizontal
speed180; it preserves peak counter52, vertical jump state, position and air
flags. AdjustMoveDir writes the display-sector heading and travel direction,
then requests original M_I_U through the existing pose lock/change service.
Repeated non-entry visits do not restart it. Ordinary unmapped M_I_U selects
the front bank and clears the rear routing bit; it has no generated rear pose.

Game `applyFieldItemMarioRequest` handles GetItemMotion, StayMotion and source
nested ControlOff/On using its existing fieldMarioBattle control-depth owner.
Main2 blocks motion15 entry but allows retained presentation. Stay queues
motion0; the later ordinary idle owner chooses its source pose after control
is admitted. A failure/discard hold cannot instantly consume pending stay.

This remains a registered-provider path. Native main still needs actual idle/
fall voice cancellation, movement clear, key/system/camera, windows/effects,
icon and audio registration. The inline source ClearMarioMotion zeros additive
X174/Z17c, horizontal speed180 and combined norm18c; it must not zero additive
Y178 or jump velocity. It is deliberately not treated as a vertical reset.

Special current22, unknown motion-table exit rows and main00100000 heading
aliases remain explicit provider frontiers. Request guards21/25/26 and
main10000000 retain source no-op/speed-clear behavior. This module is invoked
inside the existing admitted Game owner, not a complete MarioMain/init
implementation or original ABI numeric emulation. Native-unit scale remains
.01; heading vectors use the existing float/trig adapter.

Tests in `tests/field_item_mario_tests.cpp` check post order, missing providers,
pending/control gates, preserved unrelated movement, heading writes, pose
lock, no repeated restart, previous16/17 cleanup and special branches. The real
Game Goomba3 fixture now invokes the motion/control provider: the grant visit
still has pending15 and the prior pose; the next Mario visit selects M_I_U
before item ControlOff, cleanup queues stay, then exploration restores idle.
This verifies simulation ownership, not rendered pixels or physical performance.

Validated2026-10-14, commit `7929601c`, isolated review tree
`b6f6a2ea5513a9c218fa0d8fecaf624b5eadf70f`: `make test`229/229 C++ and141/141
Python tests passed; `make all BUILD=build-goomba-acquisition-review
TARGET=ttyd-goomba-acquisition-review` passed. Independent read-only review
confirmed finite request/entry/control ordering and identified the need for
nonnull post readiness, now checked before candidate and pending admission.
The first shared configuration failed because concurrent battle CMake work
referenced an unfinished test file; isolated review excluded that working edit.
No Azahar/Dolphin capture or hardware/pixel/performance claim.

Ignored native artifact `artifacts/ttyd-item-mario-reviewed.3dsx`, SHA256
`bc74a69098924ef1de173469450627c396af62c03a785bc179a720958451fbab`.
Logs `artifacts/item-mario-final-test.log`, `artifacts/item-mario-final-native.log`
and `artifacts/item-mario-source-audit.txt` identify the tested source snapshot.
No new assets/conversion/license changes; uses the existing original front
M_I_U animation pack and Citro3D renderer path.
