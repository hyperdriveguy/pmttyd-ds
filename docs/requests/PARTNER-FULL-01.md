# PARTNER-FULL-01 — Field lifecycle and abilities of the six non-Goombella partners

**Status: OPEN.** Part of the [request inbox](../DOCUMENTATION_REQUESTS.md);
validated deliveries and locks are registered in the
[delivery log](DELIVERY_LOG.md). Filed by the native implementation session,
2026-10-13, for the full-game end goal ([milestones](../MILESTONES.md)); the demo
itself needs only Goombella, already filed. The slice is explicitly requested; no
independent research-agent ownership or completion claim is made.

## Open todos

- [ ] Each non-Goombella partner's field body: follow/history/ground/obstacle
  parameters where they differ from the shared ordinary follow.
- [ ] Per-partner field ability: activation input and eligibility, targeting,
  effect, control lock and exit, and story/flag gates.
- [ ] Partner switch/leave/join field lifecycle and per-room availability, and
  how a partner is admitted or hidden at a scene boundary.
- [ ] Partner-specific presentation hooks (poses, paper, shadows) beyond the
  shared ordinary body.

## Request text (verbatim)

Native implementation source audit / documentation request (2026-10-13),
full-game end goal: [OW-PARTNER-01](OW-PARTNER-01.md) covers only "ordinary
Goombella follow and field lifecycle". The native field carries a single partner
fixture (`include/game.hpp` party state and `fieldParties`, with
`professorStory.actors` = Mario+Goombella). Battle-side partner routing exists
(`source/battle_unit_resources.cpp` kinds 224..230; `source/battle_packed_pouch.cpp`
seven 14-byte partner records), but the field lifecycle and field abilities of the
other six partners are unrecovered, so no later chapter can be played. Please
recover the field lifecycle and field abilities of the six non-Goombella partners
— per-partner follow parameters where they differ from the shared ordinary body,
ability activation/effect/control-lock, switch/leave/join and per-room
availability, and partner-specific presentation hooks — kept distinct from the
Goombella request. No lock reserved; the shared ordinary follow body stays with
OW-PARTNER-01, and the demo's Goombella path is not blocked on this.

## Answers and delivery notes (append-only history)

No delivery yet.
