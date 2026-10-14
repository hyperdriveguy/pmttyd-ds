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
  `aaa_00_init_evt`; every `GF`/`GW` use is intra-event and context-local,
  and `LW` is entry-local. Per-command operand positions, opcodes and event
  SHA-256s are tabulated.
  *Superseded in scope by the [area-init map](../behavior/story/OPENING_AREA_INIT_STORY_STATE_MAP.md),
  which shows `GSW[0]` read by every `gor`/`muj`/`tik` area init, the
  **professor** as a `GSW[0]` writer, `GSW[1420]` as a second story word, and
  `GSF` heavily used — so the earlier "GSF never used" claim is narrowed to
  those three modules. Save-block 233/236 remain with GSF-01, and written
  **values** are not recovered — opcode 50 stores `evtGetValue(operand1)`.*
- [ ] The branch/join graph across harbor → professor → underground → lecture and
  back to the field, including each branch's requirement and where scenes join —
  **graph structure answered** for the scanned modules by
  [the state contract](../behavior/story/OPENING_SCENE_STATE_AND_PERSISTENCE.md):
  nine-step `sys/evt_start` route and four `aaa` exits, all with decoded map
  and entrance identifiers, plus `GSW[0]` written before each transition.
  *Whether the nine are a linear route, and each branch's **requirement**
  (its gate), remain open — the gates are not in the scanned modules or not
  identified, and the professor/lecture/underground legs are `gor`/`muj`/
  `tik`, which were not scanned.*
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

**Branch/join graph is still open** — but the reason given there was
wrong, and is corrected below.

Also recorded as gaps: opcode direction for **93, 95 and 108**, which carry
the four `GW` operands (claimed as neither reads nor writes); the `gor`,
`muj` and `tik` modules, not scanned in this pass; and the **written
values**, since opcode 50 stores `evtGetValue(operand1)` rather than a
literal operand.

Static only — no execution trace. Event SHA-256s for all ten scanned
spans are tabulated in the contract.

## 2026-10-13 — ID 3: gor/muj/tik area-init state map

[Opening area init story-state map](../behavior/story/OPENING_AREA_INIT_STORY_STATE_MAP.md).
Scanned the fifteen `gor`, `muj` and `tik` opening / area-init events with
the same method as the previous delivery, and **corrected an over-broad
claim in it**.

**Correction.** The previous delivery said `GSF` is "**never** used by any
opening event". It was never used by `dmo`/`aaa`/`sys` — all that scan
covered — but across the area inits `GSF` is the **dominant** persistent
flag family. The claim is now scoped to its actual evidence.

**`GSW[0]` is the chapter gate, read by every area init**, with a
multi-way opcode mix — `24` (`==`), `25` (`!=`), `26` (`<`), `28` (`<=`),
`29` (`>=`) and `34` (SWITCH). Read counts: `gor_00` 12, `gor_01` 10,
`gor_02` 11, `gor_03` 6, `muj_00` 9, `muj_20` 5, `tik_00` 3. A port that
tests only `== 0` will mis-admit chapters.

**The Professor writes `GSW[0]`.** `kurihakase_first_talk` (in **`gor_02`**)
writes it at command 176 — the only `GSW[0]` write across the sixteen events
scanned in both deliveries. The chapter transition is therefore driven by
the professor scene; treating the prologue/epilogue writes as the sole
producer would miss the professor branch entirely.

**A second story word:** `GSW[1420]`, read at `gor_01` cmd 126 and `gor_02`
cmd 63. Not the index-0 exception, so it is a byte in the GP+578+ array.

**`GSF` map.** Writes: `gor_00` → 1180, 1178, **39**; `gor_01` → 1191,
1189, 1190; `gor_02` → 1203; `gor_03` → 1229; `muj_00` → 3125, **39**;
`tik_00` → 6, 237. **`GSF[39]` is written by both `gor_00` and `muj_00`**, so
it is a cross-area marker rather than an area-local one.

**Six further area exits**, resolving two joins:

- `gor/gor_muj_ship_start` cmd 79 and `muj/muj_gor_ship_start` cmd 75 both
  exit to **`muj_20`** — the ship voyage is a **bidirectional join** on that map.
- `muj/evt_prologue` cmd 49 exits to **`sys_01` / entrance `prologue`**, so
  `muj_20`'s prologue is the join between the `aaa` prologue and the `sys_01`
  sequence.
- `muj/epilogue_evt` → `muj_01`; `gor/epilogue_evt` → `muj_21`;
  `tik/epilogue_evt` → `moo_04`.

**Not resolved:** which comparison value on each `GSW[0]` test corresponds to
which chapter — only the operand index and band are tabulated, not the
literals. The `GSF` 5300-5600 band appears solely in opcode-91 operand
positions and is very likely callee arguments rather than flag accesses.
Neither is claimed.

## 2026-10-13 — ID 3 correction: the branch graph WAS decoded
## 2026-10-13 — ID 3 correction: the branch graph WAS decoded

The reply above says the `evt_bero_mapchange` targets were "raw immediates
in the inventory and were not decoded". **That was wrong.** The inventory
*does* resolve relocated string operands and emit them as `identifier`; I
had read only the `raw` field. The graph is decoded:

`sys/evt_start`, nine map changes in ascending command order:

```
gor_10 -> hei_00(dokan_2) -> win_06(dokan1) -> tou_20 -> usu_00(dokan_1)
       -> muj_20 -> rsh_02_a -> bom_00(dokan) -> las_00(w_bero)
```

Plus one exit each in `aaa/evt_prologue` (`gor_10`),
`aaa/evt_prologue2` (`muj_20` / entrance `prologue`),
`aaa/epilogue_evt` (`gor_02`) and `aaa/epilogue_evt2` (`end_00`).

**And the ordering ask is answered.** `GSW[0]` is written **immediately
before** the fade in `evt_prologue2` (write cmd 49, fade cmd 48 — adjacent;
map change cmd 50) and two commands before in `epilogue_evt2` (write cmd 117,
fade 118, map change 119). In both, the story value is committed **before**
the fade and the transition, matching the save-notice ordering GSF-01 pins.
So the opening's persistent state is written once, ahead of the scene
change, not after it.

What is **not** claimed: whether the nine `sys/evt_start` map changes are a
linear player route. They are sequential commands, but this scan does not
show whether each waits on a trigger. The identifiers are reported as
original resource strings and are not translated into area names.

Also newly recorded: the full `evt_fade_set_mapchange_type` parameter sets.
`sys/evt_start` cmds 22/23 and `sys_01_init_evt` cmds 42/43 each pair two
fades differing **only** in the mode flag (`0` then `1`) with identical
kinds and durations. `-1` appears in the demo fades as what looks like an
"unchanged" sentinel, but that semantics was **not verified** and is not
claimed.

## 2026-10-13 — Bounded persistent flag prerequisite## 2026-10-13 — Bounded persistent flag prerequisite

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
