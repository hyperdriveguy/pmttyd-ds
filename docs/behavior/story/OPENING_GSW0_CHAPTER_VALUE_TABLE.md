# `GSW[0]` chapter value table for the opening modules

**Applies to:** `GSW[0]` as compared by the sixteen scanned opening / area-init events
**Evidence class:** B — **ISO-read**, static event spans only. **No execution trace.**

## What this is

The companion
[area-init map](OPENING_AREA_INIT_STORY_STATE_MAP.md) established that
`GSW[0]` is read by every opening area init through a **multi-way** set of
comparison opcodes, and recorded that the comparison *literals* were not
extracted. This contract extracts them.

Every `GSW[0]` comparison across the scanned events, with its right-hand
side, is tabulated below.

## The sentinel: 402

`402` appears in **all seven** events that reference it, and only ever as
an equality or inequality against `GSW[0]`:

| Event | Tests |
|-------|-------|
| `aaa/aaa_00_init_evt` | `== 402` (cmd 19) |
| `gor/gor_00_init_evt` | `!= 402` (87, 147) |
| `gor/gor_01_init_evt` | `== 402` (14, 167) |
| `gor/gor_02_init_evt` | `!= 402` (12), `== 402` (164) |
| `gor/gor_03_init_evt` | — |
| `muj/muj_20_init_evt` | `== 402` (cmd 92) |
| `muj/muj_00_init_evt` | — |
| `tik/tik_00_init_evt` | `!= 402` (0), `== 402` (47) |

Totals: **6 × `== 402`** and **4 × `!= 402`** — by far the most-tested
value in the whole opening.

`402` is used as a plain literal, not through an operand band, so it is a
raw immediate on the right-hand side of the comparison. It is not a saved
value being mirrored.

**What is established:** `aaa_00_init_evt` — the prologue module's init —
**admits only when `GSW[0] == 402`**, and every opening area init treats
402 as a distinguished state, several of them as a skip guard
(`!= 402`).

**What is not established:** that 402 is the demo milestone's completion
value. The tests are consistent with 402 meaning "prologue complete", but
nothing scanned here names it, and no execution trace shows a write of 402.
The **only** `GSW[0]` writer found across all sixteen events is
`kurihakase_first_talk` cmd 176, whose written **value** is not an operand
(opcode 50 stores `evtGetValue(operand1)`), so which chapter the professor
advances *to* is not recovered.

## Per-event tables

### `aaa/aaa_00_init_evt` (prologue module init, 35 cmds)

| Cmd | Test |
|-----|------|
| 19 | `GSW[0] == 402` |

### `gor/gor_00_init_evt` (174 cmds)

| Cmd | Test |
|-----|------|
| 14 | `!= 2` |
| 55 | `>= 277` |
| 66 | `!= 217` |
| 87 | `!= 402` |
| 90 | `== 269` |
| 93 | `== 123` |
| 147 | `!= 402` |
| 148 | `!= 404` |
| 149 | `>= 268` |

### `gor/gor_01_init_evt` (177 cmds)

| Cmd | Test |
|-----|------|
| 1 | `== 341` |
| 5 | `== 400` |
| 14 | `== 402` |
| 44 | `== 6` |
| 47 | `== 7` |
| 54 | `>= 56` |
| 60 | `>= 56` |
| 85 | `< 223` |
| 100 | `< 223` |
| 106 | `< 19` |
| 167 | `== 402` |

### `gor/gor_02_init_evt` (171 cmds)

| Cmd | Test |
|-----|------|
| 12 | `!= 402` |
| 20 | `>= 268` |
| 26 | `== 7` |
| 52 | `< 23` |
| 97 | `>= 10` |
| 101 | `>= 7` |
| 102 | `<= 10` |
| 109 | `== 10` |
| 164 | `== 402` |

### `gor/gor_03_init_evt` (173 cmds)

| Cmd | Test |
|-----|------|
| 96 | `== 172` |
| 150 | `== 121` |
| 153 | `== 123` |

### `muj/muj_20_init_evt` (102 cmds)

| Cmd | Test |
|-----|------|
| 29 | `== 215` |
| 56 | `== 229` |
| 61 | `== 230` |
| 92 | `== 402` |

### `muj/muj_00_init_evt` (155 cmds)

| Cmd | Test |
|-----|------|
| 12 | `>= 268` |
| 19 | `!= 231` |
| 20 | `!= 267` |
| 21 | `== 260` |
| 37 | `>= 382` |
| 63 | `<= 255` |
| 70 | `< 260` |
| 75 | `>= 257` |
| 76 | `< 258` |

### `tik/tik_00_init_evt` (59 cmds)

| Cmd | Test |
|-----|------|
| 0 | `!= 402` |
| 1 | `!= 377` |
| 47 | `== 402` |

## Distinct chapter values

Across all sixteen events, `GSW[0]` is compared against **34 distinct
values**:

```
  2   6   7  10  19  23  56 121 123 172 215 217 223 229 230 231
257 258 260 267 268 269 277 341 377 382 400 402 404
```

The distribution is informative. Values below 25 (`2`, `6`, `7`, `10`,
`19`, `23`) cluster in `gor_01` and `gor_02` and are tested with `==`/`!=`
and short ranges — an **early-game** band. Values 121-277 and 341-404 are
tested with `>=`/`<` as well as `==`, i.e. **range** predicates — a
later-game band where the game needs to ask "at least chapter N".

`GSW[0]` therefore behaves as a **monotonic chapter counter**: early
chapters are tested for identity, later ones by threshold.

## Range decompositions

Two events decompose a single range test across several commands rather
than issuing one comparison:

`gor_02_init_evt` cmds 97, 101, 102, 109 give `>= 7`, `>= 10`, `<= 10`,
`== 10` — an identity test on 7/10 written as a threshold sandwich.

`muj_00_init_evt` cmds 63, 70, 75, 76 give `<= 255`, `< 260`, `>= 257`,
`< 258` — a band around 257/258.

A port that emits each opcode as an independent C comparison must
reproduce this structure; collapsing `<= 255` and `< 260` into one range
test would change which chapters are admitted.

## Porting notes

1. Store `GSW[0]` as a **word** (the index-0 exception), not a byte.
2. `402` is the prologue-complete sentinel; `aaa_00_init_evt` admits only
   `== 402`. Treat `!= 402` as a **skip** guard where the scan shows it at
   the top of an init.
3. Implement the identity tests (`==`, `!=`) and the threshold tests
   (`>=`, `<`, `<=`) as written. Do not normalise them to a single range
   form.
4. Preserve the decomposed range structure in `gor_02` and `muj_00`.

## Remaining gaps

- **Which chapter each value names.** The numbers are recovered; the
  chapter *titles* are not, and no mapping to area or scene is claimed
  beyond the event each test lives in.
- **The written values.** Opcode 50 writes `evtGetValue(operand1)`, so the
  value stored to `GSW[0]` by `kurihakase_first_talk` cmd 176 — and the
  values written by the two `aaa` events — are not operands and were not
  recovered. Without them the chapter *transition* is unproven.
- The `GSW[0]` tests at `gor_00` cmds 12 and 98 and `gor_02` cmd 0 use
  opcode 34 (SWITCH on the resolved value) and are not in the tables above;
  their case bodies were not expanded.
- `GSW[1420]`, seen in `gor_01` cmd 126 and `gor_02` cmd 63, has no
  extracted comparison literal.
- No execution trace; `402` is inferred as a sentinel from usage alone.