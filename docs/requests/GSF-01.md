# GSF-01 — Opening story-flag map and save-block story switches

**Status: OPEN.** Part of the [request inbox](../DOCUMENTATION_REQUESTS.md);
validated deliveries and locks are registered in the
[delivery log](DELIVERY_LOG.md). Filed by the native implementation session,
2026-10-13, for the first demo milestone ([milestones](../MILESTONES.md)); the
same flag system gates every later chapter, so this also serves the full-game
end goal. The documentation slice is explicitly requested; no independent
research-agent ownership or completion claim is made.

## Open todos

- [x] Original GSF233/236 save-block writer/order and GSF1207 pipe/Professor
  story11 predicate — [notice/pipe/storage contract](../behavior/story/OPENING_GSF_NOTICE_PIPE_AND_SAVE_STORAGE.md).
  Independently gated236 write, pre-choice flags, GSW4 acceptance write pinned.
- [x] Local GSF bank/GSW0+4 ownership and original record+8 write/restore mapping;
  same contract. Namespace lifetime/IO/native round-trip remains separate.
- [ ] Other notice-flag consumers, exhaustive opening flags/actor admission and
  original cross-scene/reentry/save-load execution.

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

## 2026-10-13 — Notice flags, pipe departure and record mapping

[Bounded GSF contract](../behavior/story/OPENING_GSF_NOTICE_PIPE_AND_SAVE_STORAGE.md)
answers original233/236 notice writer/order,1207 pipe/Professor reload predicate,
GSW0/4 storage and save-copy mapping.233 is set before first message;236 tests
story>=7 independently. Refusal retains live flags but not GSW4 acceptance write.
1207 is independent of story11; switch23 continuation resets banks separately.
No exhaustive flag census or original round-trip claim.

Validation: three DOL spans/112 words/two REL spans/five imports; exact command
nesting/refusal/SET/pipe/Professor operands/eight vectors/masks/record offsets
checked. Switch manifest63 spans/205 anchors and writer/restore pins re-passed.
Links/scoped diff passed. Docs only; no make rerun; source assets outside history.
GSF-01 remains OPEN: other consumers/flags/admission/lifetime/original traces.
