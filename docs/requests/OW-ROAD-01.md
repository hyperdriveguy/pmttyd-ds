# OW-ROAD-01 — Rogueport road triggers and transitions

**Status: OPEN (roads contract delivered).** Part of the [request inbox](../DOCUMENTATION_REQUESTS.md); validated deliveries and locks are registered in the [delivery log](DELIVERY_LOG.md).
Filed by the native overworld implementation session, 2026-10-12, as one of the four "Native delivery and further subsystem requests" blockers (shared batch text in [OW-PARTNER-01](OW-PARTNER-01.md), heading kept in [OW-MOVE-01](OW-MOVE-01.md)), and moved here from `OVERWORLD_DOCUMENTATION_REQUESTS.md` during the inbox merge; request text is preserved verbatim. The documentation slice was explicitly requested; no independent research-agent ownership or full-game completion claim is made. Updated 2026-10-12.

## Open todos

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

Next native binding: [Rogueport roads](../behavior/overworld/OVERWORLD_ROGUEPORT_ROADS.md),
delivered2f88fc4. Exact node contacts and source registration attribute overlays
must replace radius/dot/cooldown guesses; preserve documented story callbacks
and distinguish the station north train door from an ordinary road. Camera
tracking/yaw/screen projection recovery was requested from the documentation
agent to replace fixed framing/.07 smoothing/default on-screen assumptions.
Dynamic NPC/block classification, special attributes/modes, original numeric
services, camera/control projection owners and source execution traces remain
unbound. ID1 owns partner terrain/motion documentation independently.
