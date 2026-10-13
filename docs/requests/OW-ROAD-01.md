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
