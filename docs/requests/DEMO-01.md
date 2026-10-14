# DEMO-01 — Opening demo: entry conditions, persistent writes and re-entry

**Status: OPEN.** Part of the [request inbox](../DOCUMENTATION_REQUESTS.md);
validated deliveries and locks are registered in the
[delivery log](DELIVERY_LOG.md). Filed by the native implementation session,
2026-10-13, to scope the next native step — the first demo milestone
([milestones](../MILESTONES.md)) — while full-game playability stays the end
goal. The documentation slice is explicitly requested; no independent
research-agent ownership or completion claim is made.

## Open todos

- [ ] Exact entry conditions for each opening scene (new file through the first
  field re-entry): story values, prior-scene completion, flags, and any input or
  position gate that admits or skips a scene.
- [ ] The persistent writes each scene makes (global story flags, story values,
  save-block first/post-story flags) and their order relative to message and
  scene completion.
- [ ] The branch/join graph across harbor → professor → underground → lecture and
  back to the field, including each branch's requirement and where scenes join.
- [ ] Re-entry behavior: what the field shows after a completed scene, after a
  reload at each checkpoint, and on re-approaching a finished trigger.

## Request text (verbatim)

Native implementation source audit / documentation request (2026-10-13), first
demo milestone: the completion assessment
([US completion](../behavior/meta/US_COMPLETION_AND_PRIORITIES.md)) records that
no complete specification exists "of even one original playable story encounter
from entry conditions through persistent results and repeat-entry behavior", and
its P0a row asks for "conditions, joins, control locks and persistent writes;
original branch/re-entry trace" for the opening harbor→Crump→Rogueport graph.
The native port wires the harbor/professor/underground/lecture scenes as separate
native owners (`HarborStory`, `HarborEncounter`, `ProfessorStory`) whose runtime
story values and flags are provisional (`Game::originalStory`, `originalFlag1207`,
`originalFlag233`, `originalFlag236` in `include/game.hpp`), and `source/game.cpp`
still reports development notices for unbound dialogue and NPC events. Please
recover, as one contract, the opening sequence's exact entry conditions, per-scene
persistent writes, branch/join conditions and re-entry behavior, so the demo can
be built from the original graph instead of the native scene owners. The per-scene
message/event bindings already delivered for the harbor module
([harbor opening bindings](../behavior/story/STORY_HARBOR_OPENING_BINDINGS.md))
are the input; what is missing is the cross-scene state and re-entry contract. No
lock reserved; battle-result publication is filed separately as
[RESULT-01](RESULT-01.md) and the story-flag/save-block map as
[GSF-01](GSF-01.md).

## Answers and delivery notes (append-only history)

No delivery yet.
