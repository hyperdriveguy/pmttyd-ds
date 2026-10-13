# OW-ROAD-01 — Rogueport road triggers and transitions

**Status: OPEN (roads contract delivered).** Part of the [request inbox](../DOCUMENTATION_REQUESTS.md); validated deliveries and locks are registered in the [delivery log](DELIVERY_LOG.md).
Filed by the native overworld implementation session, 2026-10-12, as one of the four "Native delivery and further subsystem requests" blockers (shared batch text in [OW-PARTNER-01](OW-PARTNER-01.md), heading kept in [OW-MOVE-01](OW-MOVE-01.md)), and moved here from `OVERWORLD_DOCUMENTATION_REQUESTS.md` during the inbox merge; request text is preserved verbatim. The documentation slice was explicitly requested; no independent research-agent ownership or full-game completion claim is made. Updated 2026-10-12.

## Open todos

- [ ] Bind the used NPC yaw/display matrix and tribe scale/height/pose owners
  for Professor c_kuri_h. Actual bank clip D_1 differs from the tribe's d_1
  literal; preserve case and recover selector/fallback behavior. Current
  native story8 S_1 rendering adapts billboard facing/clock/scalar, while
  first-talk/fence scene execution remains open.

- [ ] Follow the used outer map-object flag2/0x800 and inner0x800/0x1000
  consumers through draw/fade/material requests. Native indoor cutaways
  provisionally suppress the outer display subtree while entered; replace
  this adapter with the recovered source fade and restoration behavior.

- [ ] Actual Rogueport road event trigger shapes/directions/conditions, story
  gates, arrival/departure camera and party ownership, and door/pipe/event
  transitions. Native distance<.7 and toward-marker dot gate, cooldown60 and
  reciprocal endpoint extrapolation need shipped caller/input bindings.
- [ ] Exact node contacts and source registration attribute overlays must
  replace radius/dot/cooldown guesses; preserve documented story callbacks and
  distinguish the station north train door from an ordinary road.
- [ ] Dynamic NPC/block classification, special attributes/modes, original
  numeric services and source execution traces remain unbound.

## Request text (verbatim)

- OW-ROAD-01: actual Rogueport road event trigger shapes/directions/conditions,
  story gates, arrival/departure camera and party ownership, and door/pipe/event
  transitions. Native distance<.7 and toward-marker dot gate, cooldown60 and
  reciprocal endpoint extrapolation need shipped caller/input bindings.

## Answers and delivery notes (append-only history)

2026-10-13 scoped native documentation delivery:
[Rogueport interior doors](../behavior/overworld/OVERWORLD_ROGUEPORT_INTERIOR_DOORS.md)
recovers plaza inn/bar and professor-house door records, exact display/collision
groups, case9 contact+A admission, type3 hit-normal movement points, input/camera
and visibility switch order, and professor story8/9 callbacks. These interiors
remain in gor_01/gor_02; they have no separate BERO room/entry. Upper-floor fades,
full NPC/item visibility and complete story scene binding remain open.

Next native binding: [Rogueport roads](../behavior/overworld/OVERWORLD_ROGUEPORT_ROADS.md),
delivered2f88fc4. Exact node contacts and source registration attribute overlays
must replace radius/dot/cooldown guesses; preserve documented story callbacks
and distinguish the station north train door from an ordinary road. Camera
tracking/yaw/screen projection recovery was requested from the documentation
agent to replace fixed framing/.07 smoothing/default on-screen assumptions.
Dynamic NPC/block classification, special attributes/modes, original numeric
services, camera/control projection owners and source execution traces remain
unbound. ID1 owns partner terrain/motion documentation independently.

2026-10-13 follow-up source story delivery:
[Professor first conversation and fence](../behavior/story/PROFESSOR_FIRST_TALK_AND_FENCE.md)
recovers story8 open and story9 close callback chronology, all29 original
first-talk resource IDs/speakers, party `_kur` expansion, required message
controls, Professor model c_kuri_h, Mario poses, temporary party1 conversion,
source fence paper/display/collision removal and story10 lecture handoff.
The existing lecture contract retains its numeric choices and story11 return.
No original gameplay execution or complete prologue fidelity claim is made.

2026-10-13 ordinary Professor display follow-up:
[NPC yaw/matrix and pose fallback](../behavior/display/NPC_ORDINARY_YAW_MATRIX_AND_POSE_FALLBACK.md)
binds evt_npc_set_ry to NPC+144, ordinary draw's negative revised yaw and
scale2, NPC matrix inputs/order, animPosePeraOn camera-facing producer,
tribe30/30 collision dimensions and exact missing lowercase d_1→bank0 Z_1
fallback. Actual bank case discrepancy is retained. Native camera/pose/material
presentation and original runtime comparisons remain open.

2026-10-13 first-talk visual/timing follow-up:
[Professor item/pose/hop/turn services](../behavior/story/PROFESSOR_ITEM_POSE_JUMP_AND_TURNS.md)
binds item0x60 to icon314/icon.tpl image319, M_C_3 to ordinary field e_mario,
Professor hop225ms/apex30 and exact party/Mario200ms blocking turn ownership.
Negative party duration-1 differs from0; source interpolation is not generic
shortest arc. Original icon drawing/NPC pose/numeric/runtime fidelity remain open.

2026-10-13 temporary Professor/fence admission follow-up:
[Temporary party and fence admission](../behavior/partner/FIELD_PROFESSOR_TEMPORARY_PARTY_AND_FENCE_ADMISSION.md)
binds story9 house close contact+A callback to fence event, kind11 model/poses
and constructor mode2, exact set_party_pos ownership, secondary history's
Goombella-versus-Mario source selection and clear(1)→partyKill2 restoration.
Fence admission is the door exit, not a newly invented fence proximity trigger.
Original follow trajectories/paper/collision/native runtime fidelity remain open.

2026-10-13 fence capture/paper follow-up:
[Fence paper capture and animation](../behavior/display/PROLOGUE_FENCE_PAPER_CAPTURE_AND_ANIMATION.md)
recovers OFF_d_saku source format/base mesh/A_1 seven keys and140frame span,
image clock/loop-ratio completion, previous-frame unsignedpixel capture bounds,
literal source aspect arithmetic and bounded64×48screen-plane matrix request.
Existing generic parser evaluates A_1; static Z_1 needs a single-key adapter.
Full capturetexture/depth/FOV/material/shadow/native runtime owners remain open.

2026-10-13 lecture field visibility/backend follow-up:
[Lecture actor visibility and backend boundary](../behavior/story/LECTURE_FIELD_ACTOR_VISIBILITY_AND_BACKEND_BOUNDARY.md)
pinsNPC0x80 draw admission, separate0x8000 pose-facing snap, retained NPC hop
pose ownership, partykill0/previous-position restoration and actualsetup3
coordinator/player/defense lesson graph. Accepted battle_start must retain its
source ownership until the distinct backend exists; decline follows original
choice/story11 branch. Native lesson/runtime fidelity remains open.

2026-10-13 Professor reload/reentry follow-up:
[Story11 reload and lecture reentry](../behavior/story/PROFESSOR_STORY11_RELOAD_AND_LECTURE_REENTRY.md)
recovers unconditional Professorrecord/actor init, exactstory11 flag1207-dependent
outside position versushidden position, originalpipeout flagwriter and scoped
unload/contextreset plusfreshstory10 crossingpoll admission. Staticstory8/10-only
catalog visibility cannot reproduce story11 reload. Saveflag/teardown/runtime
composition remains open.

2026-10-13 used speaker pose follow-up:
[Field talk/stay ownership](../behavior/message/FIELD_SPEAKER_TALK_STAY_POSE_OWNERSHIP.md)
binds original Generalcountdown/latch andresult0/1 stayrequests to concrete
NPCforce1 talk/staystrings andpartyselector7/1 helpers. Printcreation/active
window is not talkpose admission; explicitGoombellaNPC andprintparty remain
different owners. Original cadence/rendered/runtime comparison remains open.

2026-10-13 underground pipe and first-arrival follow-up:
[Pipe route](../behavior/overworld/ROGUEPORT_UNDERGROUND_PIPE_ROUTE.md) binds
exactgor_02/dokan↔tik_01/dokan_2 records/case6stickY>30 versuscase10frontcontact,
commonadmissiongates, source60unit pipe lengths and20unit horizontaloffset,
start/end geometry and BOTHparty restoration, Franklyslot1 creation/flag1207.
[First underground arrival/trio](../behavior/story/UNDERGROUND_FIRST_ARRIVAL_AND_GOOMBA_TRIO.md)
contains complete161command encounter,62commandinit and11commandarrival,
original4actor maptag visibility/placements/c_kuribotribe poses, tenorderedUS
message bindings/hash/controlcensus including094_01 fromtik_04, concurrency
and actualsetup4battleboundary. Prologuestory<=11 starts crossingpollX>=-100;
sourcepostbattle advances12 only after correctbackendreturn. Remaining bounded
requests: casecontactproducer and originalruntime, pipepaperpose/fade/camera/audio,
setup4unit/group/coordinator/result graph, moving-floor/bomb services and
message dynamic/shake/STICKicon renderedparity. No nativecode edited, no
originalruntime/test claim.
