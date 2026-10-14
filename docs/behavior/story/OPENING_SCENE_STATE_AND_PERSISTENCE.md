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

`GSF` (the saved flag family) is **never used by these three modules**
(`dmo`, `aaa`, `sys`) — which is all this contract scanned.

> **Correction.** An earlier version of this line read "never used by any
> opening event". That was too broad and is wrong. Scanning the `gor`, `muj`
> and `tik` area-init events shows `GSF` is the **dominant** persistent flag
> family in the opening areas. See
> [OPENING_AREA_INIT_STORY_STATE_MAP](OPENING_AREA_INIT_STORY_STATE_MAP.md).

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

## Branch/join graph — decoded map and entrance identifiers

`evt_bero_mapchange` operands are relocated string pointers, so the
inventory resolves them to resource identifiers. An earlier version of this
document claimed they were undecoded raw immediates; that was wrong.

Every such request in the scanned modules:

| Event | Cmd | Fade immediately before | Map | Entrance |
|-------|-----|--------------------------|-----|----------|
| `sys/evt_start` | 54 | — | `gor_10` | — (operand `0`) |
| `sys/evt_start` | 56 | — | `hei_00` | `dokan_2` |
| `sys/evt_start` | 58 | — | `win_06` | `dokan1` |
| `sys/evt_start` | 60 | — | `tou_20` | — |
| `sys/evt_start` | 62 | — | `usu_00` | `dokan_1` |
| `sys/evt_start` | 64 | — | `muj_20` | — |
| `sys/evt_start` | 66 | — | `rsh_02_a` | — |
| `sys/evt_start` | 68 | — | `bom_00` | `dokan` |
| `sys/evt_start` | 70 | — | `las_00` | `w_bero` |
| `aaa/evt_prologue` | 184 | cmd 183 `(1, 17, 800, 16, 800)` | `gor_10` | — |
| `aaa/evt_prologue2` | 50 | cmd 48 `(1, 49, 0, 50, 0)` | `muj_20` | `prologue` |
| `aaa/epilogue_evt` | 75 | cmd 74 `(1, 10, 900, 9, 900)` | `gor_02` | — |
| `aaa/epilogue_evt2` | 119 | cmd 118 `(1, 10, 1, 9, 300)` | `end_00` | — |

An entrance operand of `0` is a null pointer in the inventory, i.e. "no
entrance", not "entrance zero".

### The `sys/evt_start` route

Nine map changes in ascending command order with no conditional opcode
between them:

```
gor_10 -> hei_00(dokan_2) -> win_06(dokan1) -> tou_20 -> usu_00(dokan_1)
       -> muj_20 -> rsh_02_a -> bom_00(dokan) -> las_00(w_bero)
```

Four of the nine specify an entrance identifier, and three of those four
are `dokan`-family entrances. The identifiers are **resource strings from
the US build**; they are not translated here into area names, and no claim
is made that the order is a linear player route — the commands are
sequential, but whether each waits on a trigger is not established by this
scan.

## Persistent-write ordering relative to scene exit

This is the ordering DEMO-01 asks for, and it is sharp:

| Event | `GSW[0]` write | Fade | Map change |
|-------|----------------|------|------------|
| `aaa/evt_prologue2` | cmd **49** | cmd 48 | cmd 50 |
| `aaa/epilogue_evt2` | cmd **117** | cmd 118 | cmd 119 |

In `evt_prologue2` the `GSW[0]` write is the **immediately preceding
command** to the fade. In `epilogue_evt2` it is two commands before. In
both, the story value is committed **before** the fade and the map change,
so a reload or interrupt during the fade still sees the new value.

That is the same ordering [GSF-01](../../requests/GSF-01.md) pins for the
save-notice flags, so the opening's persistent state is written once,
before the transition, rather than after it.

## Fade parameters

`evt_fade_set_mapchange_type` first argument is a mode flag; the pairs are
`(mode, inKind, inMs, outKind, outMs)` as printed. Notable values:

| Event | Cmd | Parameters |
|-------|-----|------------|
| `sys/evt_start` | 22 | `(0, 19, 500, 18, 500)` |
| `sys/evt_start` | 23 | `(1, 19, 500, 18, 500)` |
| `sys/sys_01_init_evt` | 42, 43 | `(0, 17, 1800, 16, 1800)` then `(1, 17, 1800, 16, 1800)` |
| `aaa/evt_prologue` | 183 | `(1, 17, 800, 16, 800)` |
| `aaa/evt_prologue2` | 48 | `(1, 49, 0, 50, 0)` |
| `aaa/epilogue_evt` | 74 | `(1, 10, 900, 9, 900)` |
| `aaa/epilogue_evt2` | 118 | `(1, 10, 1, 9, 300)` |
| `dmo/demo_evt` | 388 | `(1, 17, 2000, -1, -1)` |
| `dmo/dmo_00_init_evt` | 60 | `(1, -1, -1, -1, 900)` |

`sys/evt_start` cmds 22 and 23 differ **only** in the mode flag (`0` then
`1`) with identical fade kinds and durations; `sys_01_init_evt` cmds 42/43
pair the same way. `-1` appears as "unchanged" in the demo events, but the
`-1` **sentinel semantics were not verified here** and are not claimed.

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
| Branch/join graph | **Answered across the scanned modules**, including the `gor`/`muj`/`tik` exits in the [area-init map](OPENING_AREA_INIT_STORY_STATE_MAP.md). Nine `evt_bero_mapchange` requests in `sys/evt_start` and one each in `evt_prologue`, `evt_prologue2`, `epilogue_evt` and `epilogue_evt2`. Targets decode to map and entrance identifiers — see the graph section below. An earlier version of this document wrongly said they were undecoded raw immediates. |
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

- **Whether the `sys/evt_start` sequence is a linear route.** The nine
  map changes are sequential commands, but nothing in this scan shows
  whether each waits on a trigger, and no claim is made that it is.
- **Opcode direction for 93, 95 and 108**, which carry the four `GW`
  operands. Not determined; those four are claimed as neither reads nor
  writes.
- **`-1` fade sentinel semantics.** Observed in the demo events but not
  verified.
- **`GSF` use outside `dmo`/`aaa`/`sys` is covered by the companion
  area-init contract**, which finds it heavily used in `gor`/`muj`/`tik`.
  What remains open is whether `GSF` is also reached through a user-function
  callee rather than a direct operand: the inventory sees direct operands and
  relocated annotations only, so a callee inside a `USER_FUNC` operand is not
  covered by either scan.
- `gor`, `muj`, `tik` area inits were scanned in the companion
  [area-init map](OPENING_AREA_INIT_STORY_STATE_MAP.md); `gor_04_init_evt`
  and the remaining area inits still are not.
- Exact per-command operand values for the reads and writes above are not
  tabulated — only the destination operand index and band. Opcode 50 writes
  `evtGetValue(operand1)` into `operand0`, so the written *value* is not in
  the operand list and was not recovered.
- Re-entry after reload needs the save-block map (GSF-01).
- No original execution trace. Every claim here is static.