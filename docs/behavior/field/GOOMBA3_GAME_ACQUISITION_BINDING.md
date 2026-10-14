# Game acquisition binding and snapshot ownership

Complements [portable acquisition owner](GOOMBA3_PORTABLE_ACQUISITION_OWNER.md)
and [source field order](GOOMBA3_ITEMGET_FIELD_SERVICE_AND_EVENT_RESUME_ORDER.md).

Game now owns the acquisition and registry alongside its original pouch,
quest switches and FieldActorTalk. `bindFieldItemGetPresentation` registers
presentation callbacks after restore. The native main/renderer/audio adapter
has not registered them yet; default native acquisition remains unavailable.
This step binds NPC admission and runtime ownership, not playable presentation.

## Runtime visits

Ordinary candidate scanning accepts plaza Goomba3's original quest2 acquisition
branch only with a real pouch and all required presentation callbacks ready.
The pending NPC talk visit invokes the acquisition owner with the live actor
root and original US message, sets dialogue/key hold and preserves attribution.
Readiness-only or missing callback sets cannot advertise or start acquisition.

At the start of a subsequent Game simulation visit, the event owner runs before
Mario/NPC work. Item work runs after the Mario presentation and NPC owners at
the end of that visit, consuming physical source A/B triggers (or existing host
input mapping). It does not run inside candidate admission or the dialogue
print callback. The message-ending visit keeps dialogue active while the event
still owns acquisition. The following event visit can enter/force the item;
that visit's item owner can grant it. Item cleanup does not write5405. A later
event visit writes5405 and returns to exploration; the NPC wait can then end.
Native source sound/window/effect consumers remain next to bind in their order.

The blocked acquisition branch suppresses ordinary move/warp/save handling;
its requested get-item motion must survive until the following Mario owner.
[Motion15 and stay](ITEM95_MARIO_MOTION15_ENTRY_DISPLAY_AND_STAY.md) pins the
pending motion, heading, source control-off gate and front M_I_U bank. That
native motion provider is still pending. No fabricated waiting text or quest
conditions substitute for missing services.

## Snapshot policy

Game is copied for existing renderer/transaction snapshots. An active
acquisition retains raw pointers to its original Game-owned services and item
entry, including an internal callback context. Those pointers cannot be copied
as an independently executable event.

`GoombaAcquisition::begin` records its owner identity. Copied/moved acquisition
state can be read for presentation but both `eventMain` and `itemMain` refuse
to advance unless the identity still matches `this`, before dereferencing any
retained owner pointers. The original Game remains the live owner. This guard
is not rebasing, active snapshot continuation, or an acquisition save format.
After a restore/reset, native presentation services must be registered again.
Do not change pouch/switch/registry storage during a live acquisition.

## Verification scope

The original Goomba3 Game fixture in `tests/goomba3_talk_tests.cpp` now tests
actual candidate admission and talk dispatch, missing callbacks, original
`irai_02_00`, print-before-entry, one actual pouch grant, retained dialogue,
cleanup-before5405, eventual exploration/key release, and copied Games during
printing and item get without mutation of the live Game. Its injected
presentation callbacks do not prove native pose/window/effect/sound rendering.

Validated 2026-10-14, code commit `4bd12c2f`, isolated review tree
`d12eec2123a6137ebd53da2661f393c721f252df`: `make test`225/225 C++ and141/141
Python tests; native `make all BUILD=build-goomba-acquisition-review
TARGET=ttyd-goomba-acquisition-review` passed. Independent read-only review
found the snapshot pointer hazard, then confirmed the advancement guard rejects
copied owner contexts before dereferencing retained pointers. Final tests
include both printing and getting snapshot regressions.

Ignored artifact `artifacts/ttyd-goomba-acquisition-game-reviewed.3dsx`, SHA256
`ca3a6adc9ed5f7b7bb99b1ae5c3749ace5f4a51adba538187a9422cd6bf0a0fc`.
Logs `artifacts/goomba-acquisition-game-final-test.log`,
`artifacts/goomba-acquisition-game-final-native.log` and
`artifacts/goomba-acquisition-game-source-audit.txt` identify the tested tree.
No Azahar/Dolphin gameplay capture, new asset conversion or physical performance
measurement. Original extracted actor/message/geometry assets are reused.
