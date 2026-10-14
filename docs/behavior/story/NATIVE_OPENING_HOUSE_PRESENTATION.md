# Native opening house presentation

This bounded presentation controller follows original US revision0
`aaa.rel` `evt_prologue`, file0xcf0..0x1814,187 commands,
SHA256 `833162e1181ca1e25faef38ecaaa34dfba08de0ad72c95cd6693f87a5461cf5f`.
It refines the [ordered opening adapter](NATIVE_OPENING_FLOW.md); it does not
execute the original REL graph or close every source service. Evidence is
static read-only inspection, not a Dolphin timing observation.

`OpeningSequence.house` owns portable actor placements, current requested poses,
pose start ticks, visibility, camera eye/aim, interior presentation selection,
and the four message admission gates. Its three actors are Mario, Luigi and
Parakarry. Positions remain engine XYZ; renderers apply native scale0.01.
Animation sampling uses simulation tick minus the actor's `poseTick`. Luigi's
idle/talk requests are L_5/L_7, then L_8/L_9; explicit walking and letter-carrying
requests use original L_6 and L_11. The local original animation pack includes
all six poses. Actor movement/heading and automatic Mario M_W_1/M_S_1 selection
remain native presentation bridges, not recovered driver equality.

The source BGM starts after a1500ms wait in an independent inline. It therefore
must not be delayed until the first message, or impose a root1500ms wait.
The root camera starts at eye(145,122,884), aim(145,108,-180), requests the
same aim/eyeZ717 over5000ms, then waits4000ms before Parakarry's
(500,500,0)→(210,25,-30) delivery. Source glide requests include speed80,
height−30 and interpolation selector11. The controller retains the authored
endpoints and native constant80 engine units/second; its straight interpolation
omits the original glide arc and timing implementation.

After delivery, two500ms waits surround the source postbox shake, and `pro_00`
is admitted with the original Parakarry speaker. Advancing that message starts
his departure to(0,300,-100). Luigi then follows the source outside route:
(20,30,-150)→(63,30,-110)→(195,30,0)→(225,30,-75), followed by the letter
pickup/return route through(195,30,0),(63,30,-110),(25,30,-155). Original
L_11 is requested while carrying the letter, then L_6 on the final entry.
The final entry runs independently of the source500ms root wait/camera cut.

The interior cut uses eye(−345,154,96), aim(−32,75,−217), retains the two
source single-frame waits and subsequent500ms root wait, then admits `pro_01`.
Completing it starts the source2000ms camera request to
(−376,154,65)/(−63,75,−247). Luigi passes(23,30,−156), then returns to
(−52,30,−275). Mario's separate approach starts at(−155,30,−320), passes
(−125,30,−290), and ends at(−85,30,−310). Original500ms precedes `pro_02`;
original300ms precedes the L_8/L_9 request and `pro_03`; another300ms follows
that payload before the existing letter transition.

Message progression remains original pro_00/pro_01/pro_02/pro_03; no payload,
speaker or story write is invented. Each gate remains blocked until its active
message finishes. The new controller advances every elapsed simulation tick,
including missed ticks, and rejects backward time. Render calls do not advance
movement or WAIT_FRAME counters. Source millisecond waits use the smallest
whole60Hz tick count that does not end early; this is a scoped native clock
adapter, not proof of the original WAIT_MSEC owner's exact scheduling.

The native outside map binds original door/postbox transforms, including the
500ms curve11 door requests, source Q1024 truncation and concurrent closing.
House camera requests now use recovered curve11 endpoints/durations. Source
clock conversion, host/native trig and movement remain adaptations; original
numeric/image equivalence is not claimed. Finite original foreground tree
parallax now consumes the same source curve. Sound/effect/environment consumers,
letterbox/fade ownership and complete NPC flags/
automatic-pose services remain unbound. Initial actor
visibility/heading and map readiness are native bridges. HouseReturn, letter,
map, voyage, wake and arrival still use the separately documented adapter.

`tools/verify_opening_house_requests.py` verifies the original event hash/count
and31 source camera, actor, speed, wait and pose anchors. Host tests check
message admission/order, retained endpoints, original waiting ticks, concurrent
Mario approach, simulation cadence independence, and original pose-pack sampling.
Full original-runtime image/timing/material equality and hardware profiling
remain open.

## Foreground tree parallax consumer (2026-10-13)

Original evt_prologue's independent inline at0xe18..0xeb4 calls
intpl_msec_init(11,0,1500,4000), publishes LW0, divides that value by10,
translates sai_ki2 on+X at0xe50, negates LW0, translates sai_ki1 at0xe78,
waits one frame, and stops when the interpolation's completion word clears.
This is a finite opening camera parallax translation; it is not a repeating
ambient sway. aaa_00_init_evt clears flag2 on both roots at0x2ab8/0x2ad0;
these requests do not hide them through visibility bit1. The previous exporter
omitted both roots, losing their original geometry and the scene consumer.

OpeningHouseScene.treeX now follows original selector11 over240 simulation
ticks, retaining both evtSetFloat Q1024 truncations (interpolation publication
and division). The endpoint150 remains after the inline ends. The existing
original [interpolation adapter](../../../include/field/field_camera_events.hpp)
preserves recovered constants/order but native trig/clock parity remains open.
Sparse simulation calls replay the same curve; rendering does not advance it.

The exterior pack now retains both exact source subtrees and exports PMHOBJ02
bindings ordered S_ie_door, S_posuto, sai_ki2, sai_ki1. Original map world
matrices/local vertices remain intact. The renderer applies additional local
X translations +treeX/10 and −treeX/10, as original mapObjTranslate divides
engine inputs by10 and the source matrix builder postconcatenates the additional
matrix. Authored tree scale1.2 remains, so this is not a flat world-space offset.
Door and postbox behavior is unchanged. Every application recomputes from the
original geometry; repeated calls do not accumulate translation. Switching
to the interior pack releases the exterior trees and their bindings.

Converted exterior vertices increase25626→31656 by restoring6030 original tree
vertices. Four roots have6912 bindings:594 door,288 postbox,3015 each tree.
PMHOBJ02 is193776 bytes. Other map textures/materials/order and the interior
variant remain unchanged. The opening converter fingerprint forces rebuild;
an obsolete two-root pack is rejected rather than silently losing the consumer.

Source verifier now checks23 object/interpolation/axis/constants anchors
(including two tree roots, arithmetic, one-frame wait and flag2 clears), alongside
original door event/matrix function hashes. Tests verify curve start/midpoint/
endpoint/hold and Q1024 publication, sparse/dense progression, opposite roots,
repeated-application/reset behavior, actual original counts and invalid packs.
This is static/source/host/build evidence; native screenshots, original-runtime
comparison, complete flag2/display ownership and hardware profiling remain open.
Flight/postbox/door requests now have [source scheduling and original PCM
presentation](../audio/NATIVE_OPENING_HOUSE_SOUNDS.md). Environment/fade and
complete original NPC physics/display/audio services still need consumers.
Parakarry's used glide callback has a portable owner consumed by the existing
native actor position renderer.
