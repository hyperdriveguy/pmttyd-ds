# Native McGoomba reward admission

Game NPC talk now admits the original Goomba3/room0/key42ff5f1f quest2 reward
when saved quest1420==2 and5406 is knowntrue. It reads neither5405 nor5376 for
that source branch. Pouch, complete coin service shape/resource readiness and
Mario Get-motion post readiness must be supplied before candidacy; unsupported
services do not advertise talk. Existing known-state ordinary/acquisition and
Goomfrey hand-in branches retain their separate admission.

[Original reward caller](PORTABLE_GOOMBA3_REWARD_AND_COIN_CALLBACK.md) prints
irai_02_02 with NPC attribution, blocks on coin20 return2, clears1420 then sets
5376 through the owned saved field, and prints sys_request_finish with speaker0.
Game retains one NPC KeyOff from successful begin to final message completion,
then KeyOn. Coin callback adds no inner key/system/control hold. Original text
and control tokens are unchanged; no story dialogue is authored.

Reward event visit follows early camera4/camera5 publication and precedes this
visit's Mario/NPC/item owners. GetMotion can be entered by the same later Mario
visit, unlike parcel's later item-owner request. While the reward has no active
message, Game still visits Mario with zero movement input; pending get/stay
motions are not starved by a bare early return. Talk/wait/scene owners remain
held through coin wait and finish text. Stable callback/window parameters live
independently past coin return2 and retire after actual manager deletion.

Native physical triggers deliberately read the retained producer pressed
snapshot at this earlier EVT visit. This preserves one edge without duplicating
producer update but is a native polling adaptation, not a claim of full original
pad/retrace scheduling. Host tests inject source A explicitly. Existing
[coin services](NATIVE_COIN_GET_PRESENTATION_SERVICES.md) retain their raw-clock
GP38 freeze/NDSP drain/camera-conversion limitations.

Copied GoombaReward/Game snapshots cannot invoke live reward services because
owner identity is retained. Complete service-shape readiness is factored from
FieldCoinGet.bind and shared by Game candidacy. The integrated host fixture uses
actual Game NPC pending/admission, original dialogue, real CoinGetWindow/effect/
icon registry, supplied clock and typed audio/Mario requests. It grants20 once,
releases outer key hold, retires the deleted window despite action1, and encodes
quest0/5376true/120coins in v9. This fixture does not exercise native NDSP itself.

Original quest acceptance, full acquisition→Goomfrey→reward chain and additional
areas remain unfinished. No complete quest/fidelity/hardware performance claim.
Existing original assets and rights remain in [ASSETS](../../ASSETS.md).

## Exact validation and native evidence (2026-10-14)

Committed d7b2c245. Final isolated tree5c0faad74ecd588ac03b1f86848f3e2f3a8faef1
passed sequential make assets-ready, make test -o assets-ready (250/250 C++ and
144/144 Python) and normal make all -o assets-ready. Read-only source/admission
review found no blockers. Logs: ignored artifacts/reward-admission-assets.log,
reward-admission-test.log and reward-admission-build.log. Normal artifact
artifacts/ttyd-reward-admission-reviewed.3dsx SHA256
976e3b28e35d03178d877ee5be8cf409a7b2a0e2e1134576f0d9b51569bac5e1.

DEBUG_SHORT_BOOT only bypasses front end; no reward instrumentation or forced
state transitions were added. Debug artifact SHA256
046e6c6e0ed726f1a2475cf037eaef6b011749cadc9abfaa782d70769e7f14cb.
Isolated tools/emulator_smoke.py run used artifacts/fixtures/goomba3-reward-native.bin
(room0/story8, explicit delivered quest2, soundenabled, original pouch100coins)
and private DSP firmware. Log artifacts/reward-admission-smoke.log passed;
actual saved v9 checkpoint has quest0/5376true/5406true/120coins/HP10/FP5 and both
checksums. Three follow-up A inputs/save retained120coins. User profile/saves were
untouched. Private tracked source audit matches; main timestamp restored for a
future normal build after the debug-only compiler flag.

Inspected captures in artifacts/azahar-goomba3-reward/: reward-stage-04 shows
original compensation NPC dialogue; stage-06 shows original coin403, pink effect7
and You got20coins window; stage-07 shows source system finish; return-saved
shows field/save return. The early first-dialogue capture was still black during
startup and supplies no dialogue evidence. Actual rendered stages/saved state
establish finite native reward execution, not matched original pixel/audio parity
or hardware/stable60 performance. Original audio detached-tail checks, pause/input
clock adaptations and complete acceptance/acquisition/hand-in/reward flow remain open.
No original assets or embedded binaries were committed.
