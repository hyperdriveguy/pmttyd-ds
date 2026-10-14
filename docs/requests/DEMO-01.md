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
  position gate that admits or skips a scene — **partly answered**: the one
  persistent entry predicate located so far is `aaa_00_init_evt`'s opcode-24
  comparison against `GSW[0]` at command 19 (see the
  [state contract](../behavior/story/OPENING_SCENE_STATE_AND_PERSISTENCE.md)).
  *Other scenes' gates are not in these modules or not yet identified.*
- [ ] The persistent writes each scene makes (global story flags, story values,
  save-block first/post-story flags) and their order relative to message and
  scene completion — **partly answered** by [opening scene state and
  persistence](../behavior/story/OPENING_SCENE_STATE_AND_PERSISTENCE.md):
  across `dmo`/`aaa`/`sys` the **only saved story value is `GSW[0]`**, written
  by `evt_prologue2` and `epilogue_evt2` and read (as a gate) by
  `aaa_00_init_evt`; `GSF` is never used by those modules; every `GF`/`GW`
  use is intra-event and context-local, and `LW` is entry-local. Per-command
  operand positions, opcodes and event SHA-256s are tabulated.
  *Save-block 233/236 remain with GSF-01, and the written **values** are not
  recovered — opcode 50 stores `evtGetValue(operand1)`, so the value is not an
  operand.*
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

## 2026-10-13 — ID 3: opening scene state census (static, ISO-read)

[Opening scene state and persistence](../behavior/story/OPENING_SCENE_STATE_AND_PERSISTENCE.md).

Answers the persistent-write and part of the entry-condition asks for the
`dmo` / `aaa` / `sys` modules. Method: `tools/rel_event_inventory.py` reads
each annotated REL event span from the read-only ISO, and each integer
operand is routed through the published
[namespace table](../behavior/event/EVENT_INTEGER_OPERAND_NAMESPACES.md)
(`index = wrap32(operand - base)`, signed32) with opcode direction taken
from the assignment and IF contracts.

**Headline: the opening's cross-scene persistence is one word.**

- `GSW[0]` is the **only** saved story value any opening event touches.
  Written by `aaa/evt_prologue2` cmd 49 and `aaa/epilogue_evt2` cmd 117;
  **read as a gate** by `aaa/aaa_00_init_evt` cmd 19 (opcode 24, the
  compare family). It is the documented index-0 exception and must be
  stored as a **word** at GP+174, not a byte in the GP+578+ array.
- `GSF` (saved flags) is **never** used by these modules. Save-block
  233/236 stay with GSF-01; nothing here claims they are unreached.
- **`GF` (16 uses) and `GW` (4) do not persist.** They belong to the
  current event context, not to BattleWork or the GP save arrays, and the
  manager selects a context per field/battle. Every `GF` use in the opening
  is a set-then-test **within one event**: `demo_evt` writes `GF[0]` at cmd
  294 and reads it at cmd 324; writes `GF[1]` at 295 and reads at 330; writes
  `GF[10]` at cmd 1 and reads at 382. The last is the one most likely to be
  mistaken for a persistent flag, and it is not.
- `LW` (292 uses) is entry-local and needs no persistence.

**Presentation flags are separate** and are not story state: `demo_evt`
issues 10 `evt_mapobj_flag_onoff`, 8 `evt_npc_flag_onoff`, 1
`evt_mobj_flag_onoff`; `evt_fade_set_mapchange_type` appears in every
opening event.

**Branch/join graph is still open.** `sys/evt_start` carries **9**
`evt_bero_mapchange` requests — that is the demo's fan-out — but their target
arguments are raw immediates in the inventory and were not decoded.
Decoding them into harbor → professor → underground → lecture is the next
step and is most of what DEMO-01 still needs.

Also recorded as gaps: opcode direction for **93, 95 and 108**, which carry
the four `GW` operands (claimed as neither reads nor writes); the `gor`,
`muj` and `tik` modules, not scanned in this pass; and the **written
values**, since opcode 50 stores `evtGetValue(operand1)` rather than a
literal operand.

Static only — no execution trace. Event SHA-256s for all ten scanned
spans are tabulated in the contract.

## 2026-10-13 — Bounded persistent flag prerequisite

[GSF notice/pipe/storage delivery](../behavior/story/OPENING_GSF_NOTICE_PIPE_AND_SAVE_STORAGE.md)
now pins233/236 save-notice order,1207 departure/story11 reload predicate and
live/save-copy mapping. Source hashes/operands verified; original execution
not claimed. This supplies part of persistent writes/reentry but does not close
all scenes, joins or the complete playable-demo contract. DEMO-01 stays OPEN.

## 2026-10-13 — Bounded battle-result producer prerequisite

[Original result phase0](../behavior/battle/BATTLE_RESULT_PHASE0_PUBLICATION_AND_PRIORITY.md)
now pins captured record/flag/conclusion priority and no-write path. Source
checks passed; field nonzero wait is not exact victory. Full battle ending,
scene joins, story advance/save-unblock and original execution remain OPEN.
