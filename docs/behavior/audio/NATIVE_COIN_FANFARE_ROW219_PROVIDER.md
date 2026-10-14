# Native original coin fanfare row219 provider

Implements the fresh group1 admission described by the [original resource and
retirement contract](GOOMBA3_COIN_FANFARE_ROW219_RESOURCE_AND_NONLOOP_RETIREMENT.md).
Code commit c3b72f6d shares existing retained music slots/envelopes between parcel
row217/track18 and coin row219/track19. Compatibility APIs named keyItem refer to
this shared group1 owner. Coin preparation and admission are explicit APIs;
Game does not yet invoke the complete reward callback.

## Resource and provider

The read-only ISO sound/stream/ff_get4_32k.stm yields original stereo32000 PCM,
65352 frames, nonlooping because COMMON restart is FFFFFFFF. Readiness checks
44-byte WAV layout, exact frames/channels/rate/size and explicit no.loop sidecar.
Both sound modes route track19 to romfs:/field-item-get-coin-fanfare.wav.
Row219 retains base volume80, pan64 and global-volume bypass. The native mix
uses original 1/127 scaling for both item profiles, avoiding generic field gain.
No generated cue, resampling or timer-based EOS substitution is introduced.

Preparation requires DSP/resource readiness, vacant shared group1/tail and a
free music slot. It retains original field music on group0, muted when necessary,
and does not evict it or reset its PCM cursor. On executes the existing source
synchronous BGMMain before later sound-owner visits. Shared envelopes and actual
producer EOS plus queued/playing NDSP drain govern retirement. Source same-ID
reuse/replacement and arbitrary sound-group callers remain outside this fresh
admission binding. A source clip duration is not a native retirement timer.

## Validation and limits

Final isolated source tree17d43265e3d7d23205b213676c824d2af2128f7f:
sequential make assets-ready passed, make test -o assets-ready passed249/249 C++
and143/143 Python, and normal make all -o assets-ready passed. Logs are ignored
artifacts/coin-audio-assets.log, coin-audio-test.log and coin-audio-build.log.
Reviewed normal artifacts/ttyd-coin-audio-reviewed.3dsx SHA256 is
886742a5c7002bef7fe72e53701183ef3873c77f26f5cf2beba70f16e23714cb.
Tests verify profile routing, exact original source hash, decoded PCM metadata
and nonloop sidecar. Read-only provider review found no blocking fresh-admission
mismatch; this is not full reward or complete sound-system parity.

Two private DEBUG_SHORT_BOOT Azahar probes used isolated profiles, explicit
sound-enabled quest fixture and private DSP firmware. Natural playback admission
and EOS/vacancy were observed; field slot0 stayed open and its PCM cursor never
reset. Checkpoints were independently byte-compared to the fixture and match.
Both probes FAILED their detached-tail assertion: no track19 tail was observed.
The first requested resume/Off201 after45 simulation visits; the second after1
visit plus60 explicit sound-owner calls in a burst. The burst is a controlled
owner-count probe, not original or native physical cadence. Neither validates a
1000ms physical fade, detached-tail drain, audible fidelity or hardware speed.
Do not infer a library refill defect or change production code from these traces.

Evidence logs: artifacts/coin-audio-native-probe.log and
coin-audio-native-burst-probe.log; isolated traces live in their corresponding
azahar-coin-audio[-burst]-probe SDMC directories. Original first probe SHA256
87307448ea3ce5aa843a18a6109a06f866f635efc42a88417a479f2f33c44cc2;
burst artifact SHA256
405f21b504a11892b444e889104509da9121755e69b27a0a28fcfffea0169c56.
Instrumented sources were restored to the reviewed tree with a tracked-file
audit. User profiles/saves were untouched; no payload or native binary committed.

Next: matched native/source clock and NDSP telemetry for the failed tail check;
borrowed window retirement on actual deletion, switch5376 persistence and native
Game reward admission. Original acceptance and full acquisition→Goomfrey→reward
validation remain open. See [coin callback](../field/GOOMBA3_COIN20_PRESENTATION_CLOCK_WINDOW_AND_RETIREMENT.md).
