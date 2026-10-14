# Native Goomba4 quest2 item hand-in

The native `GoombaHandIn` owner binds the quest2 arm of the original
[Goomba4 graph](ROGUEPORT_GOOMBA4_QUEST_AND_ITEM_SELECTION.md). Original source
[talk control and attribution](GOOMBA4_QUEST_TALK_CONTROL_AND_POSE_ADMISSION.md)
remain distinct from this native adapter. No original execution observation or
New 2DS XL performance parity is claimed here.

Admission requires the original gor_02 actor key, known quest1420=2, known
GSF5405=1 and live pouch count95>=1. It also requires the original message/font,
selector/icon metadata and finite pouch revision tables. Unknown source state
never becomes zero or ordinary dialogue. The nearest source talk candidate
retains priority when its event cannot execute.

The event prints `irai_02_03` with Goomba4 attribution, waits for original message
completion, opens the retained source table `[95,-1]` selector, and waits for
its closing sequence. The event reads the result before deleting the selector;
mode0 does not publish the selected row index. Result-1 returns without writes.
Every other result prints `irai_02_04` with the same actor, waits for completion,
then removes item95 through the original
[Mario/party parameter revision](../unit/POUCH_PARAMETER_REVISION.md) wrapper
and sets GSF5406=1 regardless of original removal result0/-1. Neither the story
word, quest byte nor GSF5405 changes. Original message bytes/control tokens are
loaded room-first/global; no hand-in prose is invented.

Talk owns depth1; selector entry owns depth2; selector deletion restores depth1;
event return restores depth0. This bounded owner exposes those depths to the
native talk scan and control adapter. It is not the full wrapping original
Mario depth byte or a replacement for other event owners. No party key behavior
is invented; the source party key leaves are no-ops. A failed native resource or
pouch-domain admission keeps the event pending and locked, rather than writing
a fabricated original completion result. The normal admitted runtime retains
its live pouch through the event.

The portable owner advances from simulation ticks, message completion and
selector polling. The Game adapter starts it from existing NPC arbitration and
keeps `Scene::Dialogue` through selector and message boundaries. The existing
Citro3D/Citro2D original font/frame/icon renderer draws the selector. No draw
callback advances the quest or removes inventory. Original BP costs and partner
HP table load once at renderer initialization; revision allocates no heap memory
and scans the same200 equipment slots and seven partner records as source.

Host checks cover original guard knowledge, original message keys/tokens and
speaker, ready-only deletion, cancel and successful hand-in, source removal
failure, unchanged damaged HP/FP, and a native Game checkpoint/proximity/event
entry/cancel cycle. The checkpoint placement and input counts are harness
choices, not original observations. The revision checks include final equipment
slot, duplicate badges, signed BP, all partner records, rank3, signed halfword
wrap and native unknown-domain refusal before removal.

Remaining: quest13 stays unavailable until known GSF5407 storage/migration and
original `M_N_5B`/`M_N_C` pose admission are bound. Existing native checkpoints
still do not serialize the full original pouch, so a completed quest2 hand-in is
not yet a faithful persistent inventory round-trip. Original task acceptance,
other quest stages, full switch banks, global event ownership/status/SFX/RNG and
matched Dolphin/3DS rendering/performance remain separate work.

Follow-up: [quest13 is now bound](NATIVE_GOOMBA4_QUEST13_HAND_IN.md) for explicit known state, with [native v8 persistence](../format/NATIVE_POUCH_CHECKPOINT_V8.md); the earlier unavailable-state limitation above describes the initial quest2 binding.
