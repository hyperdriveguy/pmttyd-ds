# Native coin-get presentation services

Binds the [original coin callback](GOOMBA3_COIN20_PRESENTATION_CLOCK_WINDOW_AND_RETIREMENT.md)
to Game-owned stable FieldCoinGet and CoinGetWindow owners. Renderer publishes
service callbacks each frame; active FieldCoinGet retains its own callback set
and borrowed parameter storage. Game invocation binds only on firstCall, refuses
retained reentry, and does not turn rebinding into a new reward. Original Goomba3
reward EVT admission remains a separate next integration step.

## Finite resources and source order

Readiness requires room0, original Mario M_I_U animation and texture, original
message/icon/effect resources, Mario motion-post audio, original coin row219
NDSP provider, pouch owner, free icon slot and published early camera5 snapshot.
Coin/acquisition windows and shared effect must be vacant. Coin admission does
not require parcel key-item capacity or row217 audio. Shared resource preparation
reuses the same texture, effect shader, vertex pool and original atlas; no new
art, geometry or GPU allocation per animation frame.

Callback binds type7/payloadcamera5 and original coin403 namediraiCoin. Source
request ordering remains in the portable callback; native provider translates
Get/Stay to existing Mario motion requests, Off3800 to retained field70/mute,
On201 to original row219, ResumeA0 before Off201 fade, and coin grant to original
pouchAddCoins. Source flags/delete/duplicate allocation stay in FieldItemRegistry.
No direct coin shortcut, sound80/system-level toggle or extra control gate is
introduced. Required outer NPC key hold belongs to the future reward caller.

Window manager runs after sound and before effect, then independently observes
actual deletion to retire callback parameters. Return2 already permits source
quest completion; manager retirement does not resume the finished VM callback.
Native overlay draws the original coin window ahead of other retained windows;
existing camera5 field pass draws original rotating coin and pink effect7.

## Heading and clock adaptation

Placement samples Mario then original NpcAngleTable angle(eyeX,eyeZ,atX,atZ),
following cam4 heading114 described by [camera lifecycle](../camera/OVERWORLD_CAMERA_LIFECYCLE_AND_PROJECTION.md).
Native eye/at coordinate scaling still carries original engine subtraction and
rounding limits. Renderer's atan2 yaw and Mario movement heading snapshot are
not substituted for this service.

libctru svcGetSystemTick is268123480Hz (local3ds/os.h SYSCLOCK_ARM11, independent
of New3DS CPU multiplier). Raw high/low paired with hardwareFrequency4*SYSCLOCK
preserves callback divisor268123, low32 wrap and strict signed200/500ms comparisons.
This is explicitly a best-effort native raw wall-clock adapter. Original
[GP38 admitted delta publication](../platform/EVENT_RETRACE_TIME_PUBLICATION.md)
freezes under GP10/exactwinCheck while baseline advances. Those event-global
pause gates are not yet bound; raw absolute native time is not source GP38 parity.
Original owner cadence/native audio detached-tail checks remain unresolved.

## Validation scope

Game/real-window host integration checks borrowed lifetime and failed reentry;
source resource review identified and corrected free-icon/camera5 readiness.
Native compilation verifies actual libctru/Citro3D service wiring. Native reward
admission, screenshot/sound capture, matched Dolphin/hardware parity and stable
performance remain unverified. Existing original packs are reused unchanged;
asset licensing/provenance stays in [ASSETS](../../ASSETS.md). No payload is committed.

## Exact final validation (2026-10-14)

Committed50f5ceb1. Isolated final tree037a54a0b71d081f75639085b1b96bd7a924097f
passed make test -o assets-ready:250/250 C++ and144/144 Python, and normal
make all -o assets-ready passed. Sequential make assets-ready passed before
checks; final source changes do not alter converters/assets. Logs:
artifacts/coin-provider-assets.log, coin-provider-final-test.log and
coin-provider-final-build.log. Source audit matches final review tree.
Reviewed artifacts/ttyd-coin-provider-reviewed.3dsx SHA256
e5d86d111bc4e85d716c2596bcfd4619bd04a485ffb04e1fa4551ae1363a8d47.
Read-only final provider review found no remaining finite-scope blockers after
free-slot/camera checks. Concurrent Game/battle work was excluded from owned
patch staging and preserved. No new native emulator/hardware run; provider
compilation and host ownership checks do not prove rendered reward fidelity.
