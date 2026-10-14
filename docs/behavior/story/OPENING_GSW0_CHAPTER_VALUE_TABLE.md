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
value. Nothing scanned here names it, and — as the write set below shows —
**402 is never written anywhere in the six scanned modules.** It cannot be
the post-epilogue value; see the 402/403 distinction below.

## The `GSW[0]` write set (opcode 50)

Opcode 50 stores `evtGetValue(operand1)` into the destination, so the
written **value** is operand1 resolved — not a hard-coded literal in the
handler. Operand1 is an operand of the same command, so the value *is*
recoverable from the event span. It is: in all three writers operand1 falls
in the `operand > -20000000` immediate/pointer band, which `evtGetValue`
returns **raw**.

A complete sweep for commands whose first operand is the `GSW[0]` marker
(`-170000000`) across all sixteen events finds **54** references, of which
**51 are comparisons** (opcodes 24, 25, 26, 28, 29) and **3 are stores**
(opcode 50). The three stores are:

| Module | Event | Cmd | Opcode | `GSW[0] <-` |
|--------|-------|-----|--------|-------------|
| `aaa` | `evt_prologue2` | 49 | 50 | **2** |
| `aaa` | `epilogue_evt2` | 117 | 50 | **403** |
| `gor` | `kurihakase_first_talk` | 176 | 50 | **9** |

Raw operand words: `0x00000002`, `0x00000193` (403), `0x00000009`. Each is
followed within one command by an opcode-91 fade/map-change pair, so the
store is the last state write before the scene changes:

```
aaa/evt_prologue2        cmd[49] op50 GSW[0] <- 2
                        cmd[50] op91 0 0 0
                        cmd[51] op2  ; cmd[52] op1
aaa/epilogue_evt2        cmd[117] op50 GSW[0] <- 403
                        cmd[118] op91 0 1 10 1 9 300
gor/kurihakase_first_talk cmd[176] op50 GSW[0] <- 9
                        cmd[177] op91 0 1
```

### The 402 / 403 distinction

This is the load-bearing correction. `epilogue_evt2` writes **403**, but
every `== 402` gate in the opening tests **402**. Those are two different
states, so:

- **402 is not "game complete".** The epilogue does not write it.
- **402 is never written at all** in the six scanned modules (`dmo`, `aaa`,
  `sys`, `gor`, `muj`, `tik`). Its source is outside the scanned set — most
  likely a save-load default or a module not covered by the opening sweep.
  **Not claimed:** any specific origin.
- `gor_00_init_evt` cmd 148 additionally tests `!= 404`, so **403 and 404
  are separate adjacent terminal-ish states**, and 404 is likewise never
  written in the scanned set.

The only ordering the event spans themselves support is:

```
  (402, unwritten here)  ->  prologue2 writes 2
                         ->  ...  ->  kurihakase_first_talk writes 9
                         ->  ...  ->  epilogue_evt2 writes 403
```

`kurihakase_first_talk` writing `9` is **not** claimed to be a chapter 9
advance. The counter's own tests show identity comparisons on `2`, `6`, `7`,
`10`, `19`, `23` and threshold comparisons from `56` upward, so a value of
`9` is inside the identity-tested early band; whether the professor scene is
a step *forward* to 9 or a *write-back* of 9 as a scene-local step marker
depends on where `gor/kurihakase_first_talk` is entered from, which is not
established by the span scan. **No execution trace.**

### Porting notes for the write set

1. `GSW[0]` is the **word** exception: `evtSetValue` routes `GSW` through
   `swByteGet`/`swByteSet`, and at index 0 both operate on the complete
   `WORD` at `GP+174` (`EVENT_SAVED_SWITCH_STORAGE.md`,
   `EVENT_ENTRY_PACKED_LAYOUT.md`). A port that stores `GSW[0]` as a byte
   truncates 403 and 404 to distinct-looking values.
2. Opcode 50 must pass operand1 through `evtGetValue`. Here the operands
   are immediates, so a port that hard-codes "write the operand" is
   equivalent **for these three commands only** — it is not equivalent in
   general, and opcode 51 (raw store, no getter) is a separate case.
3. Initialise `GSW[0]` to a state that makes `aaa_00_init_evt` admit, since
   402 is never produced by the scanned event code. If the port picks a
   different sentinel it must change all 10 `402` tests with it.

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
2. `402` is a distinguished sentinel that the scanned event code **only
   ever tests**; `aaa_00_init_evt` admits only `== 402`. Treat `!= 402` as
   a **skip** guard where the scan shows it at the top of an init. Do not
   assume it equals the epilogue's 403 — they are different states.
3. Implement the identity tests (`==`, `!=`) and the threshold tests
   (`>=`, `<`, `<=`) as written. Do not normalise them to a single range
   form.
4. Preserve the decomposed range structure in `gor_02` and `muj_00`.

## Remaining gaps

- **Which chapter each value names.** The numbers are recovered; the
  chapter *titles* are not, and no mapping to area or scene is claimed
  beyond the event each test lives in.
- **The origin of 402 and 404.** Both are tested and neither is written in
  the six scanned modules. Needs a wider module sweep or a save-format read.
- **Whether `kurihakase_first_talk`'s `9` is forward progress.** The value
  is recovered; the direction is not, because the entry path into that
  event is not established.
- **Opcode-34 (SWITCH) case bodies** on `GSW[0]` were not expanded. The
  complete reference list is: `gor_00_init_evt` cmds 12 and 98;
  `gor_02_init_evt` cmds 0 and 112; `gor_03_init_evt` cmds 17, 60 and 158;
  `muj_00_init_evt` cmd 82. (Earlier revisions of this file listed only
  three of these eight.)
- `GSW[1420]`, seen in `gor_01` cmd 126 and `gor_02` cmd 63, has no
  extracted comparison literal.
- No execution trace.