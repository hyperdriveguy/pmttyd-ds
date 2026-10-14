# Opening scene state: which values each scene writes, reads and persists

**Applies to:** the `dmo` → `aaa` / `sys` opening modules
**Evidence class:** B — **ISO-read**, static event spans and relocated operands only. **No execution trace.**

## Why this document exists

[requests/DEMO-01.md](../../requests/DEMO-01.md) asks for the opening
sequence's entry conditions, persistent writes, branch/join graph and
re-entry behaviour. This contract covers one part of that: **which
encoded state each opening scene actually touches, and which of those
survives a scene change.**

It is the cross-scene piece the existing harbor bindings do not supply.
[STORY_HARBOR_OPENING_BINDINGS](STORY_HARBOR_OPENING_BINDINGS.md) is the
per-scene message/event input; this is the state layer over the same
modules.

## Method

`tools/rel_event_inventory.py` reads each annotated REL event span from
the read-only US ISO, parses the big-endian command words, and annotates
relocated operands with their DOL symbol names. Each event's SHA-256 is
reported and repeated below.

Integer operands are then routed through the published namespace table in
[EVENT_INTEGER_OPERAND_NAMESPACES](../event/EVENT_INTEGER_OPERAND_NAMESPACES.md):
for each band, `index = wrap32(operand - base)`, signed32 throughout.

Opcode direction uses
[EVENT_ASSIGNMENT_ARITHMETIC_AND_DATA_CURSOR](../event/EVENT_ASSIGNMENT_ARITHMETIC_AND_DATA_CURSOR.md)
and [EVENT_IF_INTEGER_MASK_AND_CONTINUATIONS](../event/EVENT_IF_INTEGER_MASK_AND_CONTINUATIONS.md):

| Class | Opcodes | Meaning |
|-------|---------|---------|
| WRITE | 50, 51, 53, 54, 55, 56, 63, 106 | ends in `evtSetValue` |
| READ | 12..31, 34, 16, 17, 22 | compare / mask / switch predicates |
| other | everything else | **direction not determined here** |

## Result: only ONE value persists across scenes

**`GSW[0]` is the only saved story word any opening event touches**, and it
is touched three times:

| Event | Cmd | Opcode | Direction |
|-------|-----|--------|-----------|
| `aaa/evt_prologue2` | 49 | 50 | **WRITE** |
| `aaa/epilogue_evt2` | 117 | 50 | **WRITE** |
| `aaa/aaa_00_init_evt` | 19 | 24 | **READ** |

`GSF` (the saved flag family) is **never used** by any opening event.
Per this request's own scope, save-block 233/236 are filed separately in
[GSF-01](../../requests/GSF-01.md); this contract says nothing about them
and does not claim they are unreached.

`GSW[0]` is the documented **index-0 exception**: it is a GP-owned WORD at
GP+174, not a byte in the GP+578+ array
([EVENT_ENTRY_PACKED_LAYOUT](../event/EVENT_ENTRY_PACKED_LAYOUT.md),
[EVENT_SAVED_SWITCH_STORAGE](../event/EVENT_SAVED_SWITCH_STORAGE.md)).
A port must store it as a word.

### The gate shape

`aaa_00_init_evt` — the module's init event — **reads** `GSW[0]` at command
19 with opcode 24 (an equality/comparison predicate in the 24..29 family).
It does not write it. That is the shape of a persistent entry predicate:
a later init event gates on a value an earlier scene left behind, and
does not clear it.

The two writers are `evt_prologue2` and `epilogue_evt2`, both in `aaa`.
`evt_prologue` and `dmo/demo_evt` never write `GSW`.

## Result: GF/GW do not persist

| Family | Occurrences in the opening | Owner |
|--------|---------------------------|-------|
| `GF` | 16 | **current EVENT CONTEXT** |
| `GW` | 4 | **current EVENT CONTEXT** |
| `LW` | 292 | this `EventEntry`, local |
| `LSW` / `LSF` | 0 | — |
| `UF` / `UW` / `LF` | 0 | — |

Per the namespace contract, **`GF`/`GW` belong to the current event context,
not to BattleWork directly or to the GP save arrays**, and `LW` belongs to
the individual entry. The manager selects a context (`0x803D8D30` for field,
`0x803D8DD0` for battle), so a context change discards them.

Every `GF` occurrence in the opening is a **set-then-test inside one event**:

| Event | Writes | Reads |
|-------|--------|-------|
| `dmo/demo_evt` | `GF[10]` cmd 1; `GF[0]` cmd 294; `GF[1]` cmd 295; `GF[0]` cmd 301; `GF[1]` cmd 309 | `GF[0]` cmd 324; `GF[1]` cmd 330; `GF[10]` cmd 382 |
| `aaa/evt_prologue` | `GF[1]` cmd 35; `GF[1]` cmd 71 | `GF[1]` cmd 47 |
| `aaa/evt_prologue2` | `GF[0]` cmd 35 | `GF[0]` cmd 40 |
| `aaa/epilogue_evt2` | `GF[0]` cmd 91; `GF[0]` cmd 100 | — |

In `demo_evt` the write at cmd 294 precedes the read at cmd 324, and the
write at cmd 295 precedes the read at cmd 330 — in each case **within the
same event, and in order**. `GF[10]` is written at cmd 1 and read at cmd
382, which is the longest intra-event span and the one most likely to be
mistaken for a persistent flag.

`GW[10]` appears twice in `demo_evt` under opcodes **93** and **95**, and
`GW[0]` under 93 and 108. Those four are direction-**undetermined** (see
gaps); they are not claimed as reads or writes here.

`LW` is by far the most used (292 occurrences) and is entirely
entry-local — it carries no cross-scene state and needs no persistence.

## Presentation flags are separate

Independently of the operand bands, the opening events use the
relocated map/npc flag services:

| Event | `evt_mapobj_flag_onoff` | `evt_npc_flag_onoff` | `evt_mobj_flag_onoff` |
|-------|--------------------------|---------------------|------------------------|
| `dmo/demo_evt` | 10 | 8 | 1 |
| `dmo/dmo_00_init_evt` | 1 | 12 | 1 |
| `aaa/evt_prologue` | 6 | 4 | — |
| `aaa/evt_prologue2` | 3 | 2 | — |
| `aaa/aaa_00_init_evt` | 2 | — | — |
| `aaa/epilogue_evt` | — | 1 | — |
| `aaa/epilogue_evt2` | — | 2 | — |
| `aaa/epilogue_npc_entry` | — | 2 | — |

These drive **map-object and NPC presentation** (which actor is visible,
which prop is in which state). They are not story state and are not part
of the persistent branch graph. `evt_bero_mapchange` and
`evt_fade_set_mapchange_type` are the scene-exit requests; the latter
appears in every opening event, and `sys/evt_start` carries **9**
`evt_bero_mapchange` requests.

## Scene exit requests

| Event | `evt_fade_set_mapchange_type` | `evt_bero_mapchange` |
|-------|--------------------------------|----------------------|
| `sys/evt_start` | 2 | **9** |
| `sys/sys_01_init_evt` | 2 | — |
| `dmo/demo_evt` | 1 | — |
| `dmo/dmo_00_init_evt` | 1 | — |
| `aaa/evt_prologue` | 1 | — |
| `aaa/evt_prologue2` | 1 | — |
| `aaa/epilogue_evt` | 1 | — |
| `aaa/epilogue_evt2` | 1 | — |

`sys/evt_start`'s nine map-change requests are the branch fan-out the
demo needs. Their target arguments are raw immediates in this inventory,
so **the branch graph itself is not recovered here** — see gaps.

## Event fingerprints (all read-only from the US ISO)

| Event | REL SHA-256 | Commands |
|-------|-------------|----------|
| `dmo/demo_evt` | `3e3f3b88dd5e9c8268dfdb58cf0df3120b0548814225041879830c3e637bc6c0` | 392 |
| `dmo/dmo_00_init_evt` | `517da311a31b22dbb73a74189d18fe40d7bcef08de5734b13a44f445a5fbc77a` | 63 |
| `aaa/evt_prologue` | `833162e1181ca1e25faef38ecaaa34dfba08de0ad72c95cd6693f87a5461cf5f` | 187 |
| `aaa/evt_prologue2` | `6a8878ef482a27ca82943e8f359e35d0264dd2515978900935aaa64f26bd042f` | 53 |
| `aaa/aaa_00_init_evt` | `84ea96d4663383773864f42c2a93024f9eb04b2669a2811c4bdb2d6a1d55ac63` | 35 |
| `aaa/epilogue_evt` | `c06739e3dd529dfd82279c178574a5ef25ab1a84585a1d513b5015bee89b6075` | 78 |
| `aaa/epilogue_evt2` | `4b778c6bf2ad64fad360af26a2aeb1053eaafaf89fabf48275a012450d25a4f6` | 122 |
| `aaa/epilogue_npc_entry` | `fdf327a6d75d9631d4a41668d01b562b3c1948f7edca50b76545c6705300f314` | 16 |
| `sys/sys_01_init_evt` | `de944bd9a79ab6a5940f83c6a0af357a99ba07c9341c6405c74f66c581ec6be2` | 47 |
| `sys/evt_start` | `65bf047866322730d1a7e03623b243ce0158e76f7531507bb4941bccb7be349a` | 74 |

REL digests: `dmo.rel` 26328 bytes
`3e4089507ce78d9b0ebf74c6d05cc801095ff393fe82bc6407c9f6f9a03656b4`.

## What this answers in DEMO-01

| DEMO-01 todo | Status here |
|---|---|
| Persistent writes per scene | **Partly answered.** `GSW[0]` is the only saved story word in these modules, written by `evt_prologue2` and `epilogue_evt2`, read by `aaa_00_init_evt`. `GF`/`GW`/`LW` are context or entry local and do not persist. Save-block 233/236 remain with GSF-01. |
| Entry conditions per scene | **Partly answered.** `aaa_00_init_evt` gates on `GSW[0]`. Other scenes' gates are not in these modules or not identified. |
| Branch/join graph | **Not answered.** Nine `evt_bero_mapchange` requests in `sys/evt_start` are the fan-out, but their target arguments are raw immediates and were not decoded. |
| Re-entry behaviour | **Not answered.** Requires the save-block map (GSF-01) and an execution trace. |

## Porting notes

1. Store `GSW[0]` as a **word** (GP+174), not a byte.
2. Do not persist `GF`/`GW` across a context change. The opening's many
   `GF` uses are intra-event set-then-test pairs, and treating them as
   persistent flags would create state the original does not keep.
3. `LW` carries no cross-scene state; it does not need saving.
4. The `GF[10]` pair in `demo_evt` (write cmd 1, read cmd 382) is the one
   that most looks like a persistent flag and is not.

## Remaining gaps

- **Branch/join graph.** `evt_bero_mapchange` target arguments are raw
  immediates in the inventory; decoding them into the harbor → professor →
  underground → lecture graph is the next step and is what most of
  DEMO-01 still needs.
- **Opcode direction for 93, 95 and 108**, which carry the four `GW`
  operands. Not determined; those four are claimed as neither reads nor
  writes.
- Whether `GSF` is genuinely unused in the opening, or reached through a
  user-function callee rather than a direct operand. The inventory sees
  direct operands and relocated annotations only, so a callee inside a
  `USER_FUNC` operand is not covered by this scan.
- `gor`, `muj`, `tik` and the remaining opening modules were not scanned
  in this pass; only `dmo`, `aaa` and `sys`.
- Exact per-command operand values for the reads and writes above are not
  tabulated — only the destination operand index and band. Opcode 50 writes
  `evtGetValue(operand1)` into `operand0`, so the written *value* is not in
  the operand list and was not recovered.
- Re-entry after reload needs the save-block map (GSF-01).
- No original execution trace. Every claim here is static.