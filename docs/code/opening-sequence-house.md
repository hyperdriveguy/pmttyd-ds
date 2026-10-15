# New Game sequencing, house and audio requests

Implementation: [sequence](../../source/story/opening_sequence.cpp),
[house](../../source/story/opening_house_scene.cpp),
[glide](../../source/story/opening_npc_glide.cpp),
[object binding](../../source/story/opening_house_objects.cpp),
[sound requests](../../source/story/opening_house_sounds.cpp),
[PCM player](../../source/story/opening_house_pcm.cpp).
Evidence: [opening flow](../behavior/story/NATIVE_OPENING_FLOW.md),
[house requests](../behavior/story/NATIVE_OPENING_HOUSE_PRESENTATION.md),
[glide](../behavior/story/NATIVE_OPENING_NPC_GLIDE.md),
[sounds](../behavior/audio/NATIVE_OPENING_HOUSE_SOUNDS.md).
This is a bounded native sequencing/presentation adapter, not REL execution.

| Part | Original payload / speaker identity |
| --- | --- |
| House | pro_00 / `0x468` Parakarry; pro_01..03 / `0x474` Luigi |
| Letter | pro_04 / source0 |
| HouseReturn | pro_05 / `0x474` |
| MapDisplay -> Voyage -> Prologue | Their separate request/admission owners |
| Wake | mac_0_012, mac_0_013, mac_0_013_01 / source0 |
| Arrival | mac_0_014, mac_0_015 / source0 |

Load validation groups 11 payloads by resource; its pro_05-before-pro_04 array is
not runtime story order. Room resources are aaa_00 for house/return/map, gor_10
for letter/wake, gor_00 for arrival, then unambiguous global fallback. An ambiguous
room key blocks substitution. Original IDs/raw tokens remain unchanged; missing
or unsupported text fails. Native terminal input is an explicit presentation
adaptation, not a rewritten payload. Local resources publish after validation;
the house sound catalog has its own earlier publication boundary.

Public sequence time is milliseconds. Scene motion generally uses
floor(ms*60/1000), while heading/wake take milliseconds. Binding scene services
requires actual resource/frame admission before progression. Admission shifts
per-part deadline by loading delay, then restarts only House/Voyage/Arrival/Wake;
other helper clocks retain entry time. BGM/map-transfer clocks advance even while
waiting admission. Part matching does not provide async-generation protection:
caller must retire stale producers. HouseReturn uses authored snapshots rather
than rerunning the house script. Inter-scene 1000 ms pacing is native presentation,
not measured original timing.

Scene clocks continue while a message owns input. Exact closed message index
releases its source gate. Story2 requires actual map exit presentation; Voyage
entry paper continues concurrently. Story3 is the arrival parent release, not
full ship-child completion or direct battle entry. Catalog music requests do not
prove audible readiness; repeated same-track polling preserves existing fades.
See [voyage/arrival](opening-voyage-arrival.md) and
[map/heading/wake](opening-map-heading-wake.md).

House actor slots 0/1/2 mean Mario/Luigi/Parakarry, separate from field IDs.
Its request enum is native, not EVT opcodes. Millisecond waits round upward to
whole 60 Hz ticks; explicit wait(1)/wait(2) remain frame requests. Move/glide/
message/wait block the root, while parallel movement, camera and door interpolate
independently. Source pose names/endpoints and command REL offsets are provenance,
not guest CPU function pointers. Sparse advances replay each simulation tick.

Walking is a native constant-speed80 bridge, preserving Mario/Luigi height.
Parakarry uses incremental selector11 glide with speed80/height-30. It preserves
source terminal subtraction order, negative previous curve phase, fused arc
differences and additions to retained position; it does not snap to an inferred
endpoint. Result1 clears/yields an extra invocation, including zero distance;
result2 has continue semantics in its reached terminal branch. Source float/
sqrt/trigonometric equality and full NPC/collision ownership remain separate.

Music request starts at 90 ticks; foreground trees interpolate over240. Door and
tree publications retain Q1024 after interpolation and again after the source
division. `PMHOBJ02` house bindings have four tags: door/postbox/ki2/ki1, finite
3x4 world matrices and globally unique converted vertex IDs. Apply recomputes
retained local geometry. Door parameter multiplies568/1024, quantizes, then
rotates; it is not already final degrees. Other parameters divide10 into local
translation, with opposite tree signs. Zero writes original XYZ, avoiding drift.
These writes update converted geometry, not field collision or GPU admission.

Sound position is captured before NPC delta consumption. Flight waits180 ticks,
then 14 calls12 ticks apart, at least one holding call24 ticks apart, and 17 final
calls12 ticks apart. Departure is checked after a holding wait; it does not cancel
a current sample or end the inline when glide ends. Position X/Y truncates Q1024,
Z is authored-100. Direct sparse sound calls reuse one passed XYZ for overdue
requests; the house's per-tick replay supplies historical samples.

`PMHSND01` validates original cue identities/table972/973/568/589, four mono WAV
headers/exact lengths and transformed object-position metadata. Cue indexes0..3
are distinct from original SFX/FX/macro/sample IDs. ENV on/off/LPF and BGM-scope
remain explicit unbound records, not fabricated ambience or completed spatial audio.

PCM presentation uses four stable linear PCM16 clips and eight wave buffers on
NDSP10..17. Channel18 remains outside that bank for title's detached owner.
Sample size bound512K frames is native. Table volume/pan divide127 into stereo;
resolved position is required but full calc3D/MusyX behavior is not implemented.
Keep the player at a stable address while queued; explicit shutdown clears channels
before freeing PCM. There is no automatic destructor cleanup or safe live copy.

PCM update keys owner pointer plus beginTick epoch. Mute clears voices and advances
the request cursor, so unmute does not replay discarded requests. Failed play
also advances cursor and counts failure: false is a report, not retained retry.
ENV/scope increment unbound count. Original source requests remain in their trace;
this bounded player does not acknowledge an original ambience engine.
