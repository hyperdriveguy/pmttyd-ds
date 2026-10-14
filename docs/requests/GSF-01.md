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
`include/runtime/game.hpp` carries `std::optional<int32_t> originalStory`,
`bool originalFlag1207` ("Original GSF: Frankly has departed through the east
pipe"), `bool originalFlag233`, `bool originalFlag236` ("Save-block first/
post-story notices") and a separate `secondaryFlags` word; `source/runtime/game.cpp`
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

## Verification (native session, 2026-10-13)

Re-checked the [bounded GSF delivery](../behavior/story/OPENING_GSF_NOTICE_PIPE_AND_SAVE_STORAGE.md)
against the read-only US ISO with `tools/inspect_dol_function.py` (output kept
under `/tmp`, not source history), independent of its own manifest:

- `swSet`(`0x800858fc`)/`swGet`(`0x800858b4`) disassembly confirms GP=`*(r13-28496)`
  =`0x80415fd0`, `q=trunc(N/32)`, address=`GP+376(0x178)+4q`, mask=`slw(1,R)`;
  set loads/or/stores one word, get normalizes nonzero to exactly1. So index233/
  236 →`GP+0x194` (mask`0x200`/`0x1000`) and index1207 →`GP+0x20c`
  (mask`0x800000`) match the delivered `gp_word_offset`/mask and
  [SW_SET](../behavior/memory/SW_SET.md).
- `swInit`(`0x80085980`) confirms the five extents and the closing `stw 0,372(3)`
  (word174): memsets at376/`0x400`,1400/`0x800`,3448/`0x40`,3512/`0x400`.
- The 164-byte system-event span `0x8030b4ec` reproduces the delivered order:
  `SET GSF233=1` (`f8405c69`,`00000001`) precedes `evt_msg_print`(`800d284c`),
  and the first `00000021` END_IF closes before the independent `GSW0>=7`
  (`0002001d f5de0180 00000007`) that sets `f8405c6c` (GSF236) — so 233 alone
  never suppresses the 236 write.

Not re-run here: the `gor.rel` 1207/east-pipe and Professor-init spans (REL
extraction was not repeated); they remain the manifest's pinned evidence.
Verified bytes only; no original runtime execution and no native change made.
