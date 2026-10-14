# GSF-01 — Opening story-flag map and save-block story switches

**Status: OPEN.** Part of the [request inbox](../DOCUMENTATION_REQUESTS.md);
validated deliveries and locks are registered in the
[delivery log](DELIVERY_LOG.md). Filed by the native implementation session,
2026-10-13, for the first demo milestone ([milestones](../MILESTONES.md)); the
same flag system gates every later chapter, so this also serves the full-game
end goal. The documentation slice is explicitly requested; no independent
research-agent ownership or completion claim is made.

## Open todos

- [ ] The global story-flag (GSF) IDs and bit semantics that the opening sequence
  reads and writes, with the scene that triggers each write.
- [ ] The save-block first/post-story notice flags (native provisional
  `originalFlag233`/`originalFlag236`) and their original ownership.
- [ ] The story-switch bank ownership and how live GSF state maps onto the save
  block on save/load (the file format and state machine stay with
  [SAVE-01](SAVE-01.md)).
- [ ] The opening flag writes that gate field-actor/NPC visibility and scene
  admission, so native gating uses original flags rather than native booleans.

## Request text (verbatim)

Native implementation source audit / documentation request (2026-10-13), first
demo milestone: the native story gating uses provisional flag values.
`include/game.hpp` carries `std::optional<int32_t> originalStory`,
`bool originalFlag1207` ("Original GSF: Frankly has departed through the east
pipe"), `bool originalFlag233`, `bool originalFlag236` ("Save-block first/
post-story notices") and a separate `secondaryFlags` word; `source/game.cpp`
still prints "Development notice: NPC event unavailable" when an NPC event is
unbound, and the harbor/professor scene owners advance on native stage counters.
The opening scenes' gates need the original flag identities, not native booleans.
Please recover the global story-flag map the opening sequence uses — flag IDs,
bit semantics, the scene that writes each, the save-block first/post-story notice
flags and the story-switch bank ownership — so native story gating and the save
round-trip use original values. The save file format and state machine remain
with [SAVE-01](SAVE-01.md); this request is the live flag semantics those writes
use. No lock reserved; cross-scene persistence is [DEMO-01](DEMO-01.md) and
battle-result publication is [RESULT-01](RESULT-01.md).

## Answers and delivery notes (append-only history)

No delivery yet.
