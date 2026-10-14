# Original opening house sound request and PCM consumers

US G8ME01 disc0 revision0, read-only source evidence. This binds the original
house SFX requests to a bounded native primary-sample PCM presentation. Original
MusyX, complete positional mixing and environment scheduling remain separate.
No dialogue, event/message ordering, fabricated sound or synthetic tone is added.

## Independent flight sound event and stop owner

Original aaa evt_prologue sets WW1=0 at0xf00 after Parakarry's position request
and starts an independent inline0xf0c..0x1004. Its exact program is:

1. WAIT_MSEC3000; repeat14 times: query Parakarry position, request
   SFX_EVT_OPN_PARETTA_MOVE1 at0xf38 using queried X/Y and literal Z=-100,
   output operand0; WAIT_MSEC200.
2. Repeat indefinitely: query/request same key at0xf80; WAIT_MSEC400;
   if WW1==1, break. This check is AFTER the wait; setting the word does
   not immediately cancel the currently sounding sample or skip this loop's
   first request.
3. Repeat17 times: query/request same key at0xfdc; WAIT_MSEC200; end inline.

The root writes WW1=1 at0x10fc only AFTER original pro_00 completes, immediately
before departure glide. The source inline continues its fixed final17 calls
independently of that glide's movement completion. The portable owner retains
both fixed loop counts, wait-before-first-request and post-wait stop test.
Simulation waits3000/200/400ms become180/12/24 ticks. Source original OS-time/
event-manager cadence is not claimed identical to this60Hz clock adapter.

Original evt_npc_get_position publishes through evtSetFloat before the SFX
callback reads the local words, so queried X/Y are Q1024 truncated. The house
captures positions before its NPC delta consumption, respecting the original
[events-before-NPC outer order](../platform/GAME_OUTER_LOOP_AND_EVENT_UPDATE_ORDER.md).
Render calls do not schedule sounds; repeated same-tick calls do not duplicate
requests. Sparse scene advancement still executes every simulation tick.

## Source postbox, door and environment requests

SFX_EVT_POST_SHAKE1 at0x1060/0x1308 fires once BEFORE each3-repeat postbox
translation shake, not once per displacement or frame. SFX_DOOR_OPEN1 fires
at0x118c/0x1438; SFX_DOOR_SHUT1 at0x1228/0x15c4. Each uses original
map-object get_position output before its door interpolation starts. All
SFX3D requests have final output operand0, meaning no returned handle store.
The original callback resolves XYZ and calls psndSFXOn_3D, then returns2.

mapObjGetPos/mapObjGetPosSub average the transformed centers of leaf nodes
under the addressed root, excluding root siblings. _mapEnt gives mesh leaves
local bbox midpoint, hierarchy-only leaves their local translation. Source
original world matrices and extra transforms precede the get-position
publication. The exporter retains S_ie_door's single leaf and S_posuto's three
leaves as root-local mean plus original root matrix. Runtime applies source
current door rotation/postbox translation, scales map units to engine units,
and publishes Q1024. Offline matrix/mean and native trig arithmetic remain
numeric adapters; exact PS matrix/update-clock equality is not established.

The original environment requests remain explicit records:

| Command | Operation | Original operands |
| --- | --- | --- |
|0xd10|ENV on|0x110, ENV_OPN_AAA1|
|0xd20|BGM scope|0,1|
|0x1590|ENV off|0x4800|
|0x159c|ENV LPF|0,800|

The interior ENV off/LPF precede the final door-close request after the source
one-frame waits. ENV_OPN_AAA1 is original environment name index1 in
psenvlistname8032da1c. Its five28-byte rows at8032b690..8032b71b refer to
SFX_AMB_BIRD1/2/3 and SFX_AMB_LEAF1/2. Original ENVOn_f_d/ENVMain own the
random scheduling, timed controls and active pool. Their actual RNG/clock/
MusyX ownership has no consumer in this patch: records stay unbound rather
than fabricating bird timing or acknowledging a working ambience engine.
The original BGM scope record is also unbound; existing BGM playback is separate.

## Original assets and native PCM presentation

[Exporter](../../../tools/opening_house_sound_assets.py) resolves original
sound-table index, FX object and sample separately, verifies one direct sample
per selected macro and retains table flags/parameters and source provenance:

| Request | SFX index | FX ID | Macro object | Sample | Rate/frames |
| --- | --- | --- | --- | --- | --- |
| Flight |972|1278|1435|1176|16000/4614|
| Postbox |973|527|620|479|16000/8174|
| Door open |568|931|1062|840|16000/12764|
| Door shut |589|942|1073|851|16000/12365|

Original DSP ADPCM becomes mono PCM16 with no resampling or introduced loops.
Ignored opening-house-sounds.bin is404 bytes (PMHSND01); four WAV companions
are approximately75KB. The sound metadata/resource loader admits all four
PCM headers/sizes before the opening sequence starts. Cache validation checks
all products/hashes. No original payload or compiled pack enters source history.

[OpeningHouseSounds](../../../source/opening_house_sounds.cpp) retains source
requests/control values and schedules the independent inline.
[OpeningHousePcmPlayer](../../../source/opening_house_pcm.cpp) consumes only
SFX PCM presentation: lazy source sample load, original rate, source table
volume/pan, independent overlapping wave buffers and actual native queue calls.
NDSP channels10..17 avoid both BGMstreams0..1 and the primary eight clips2..9.
Mute/scene exit clears its voices; shutdown clears before freeing its own data.
Load/position/voice-admission failures return false and increment failures;
they do not acknowledge a request as played. ENV/scope records remain in the
source owner and increment an unbound count, without successful consumer claims.
The source story never waits for this PCM adapter's queue admission.

Audio::updateOpeningHousePcm and its one main-loop call are separate from
existing BGM/fade/field-NPC audio. Source volume/pan map to native stereo gains;
full calc3D listener/distance/surround, global fades/percentages, priorities,
MusyX macro delay/pitch/random/envelope/child voices and original sound handles
remain unbound. This is original primary-sample playback, not an exact original
MusyX or positional-audio reconstruction. Flight's macro includes additional
commands20/25 as well as STARTSAMPLE; those commands are not rendered here.

## Evidence and validation scope

[Read-only verifier](../../../tools/verify_opening_house_sounds.py) pins the
original187-command house event hash, seven source service hashes and24
SFX/loop/WW1/environment anchors. Host tests cover fixed counts, source stop
ordering, final tail, continued holding messages, two postbox/four door calls,
source parameters, position availability, full scene progression and original
sample identities/PCM headers/nonzero wave data/hash/cache/leaf metadata.
No Azahar audible test, Dolphin trace or physical-hardware playback was performed
for this isolated branch. Parent native integration/audible verification remains
necessary; full original audio fidelity and the overall prologue remain incomplete.
