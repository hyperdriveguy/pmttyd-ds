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

## Opcode-34 SWITCH ladders on `GSW[0]`

All eight `SWITCH` sites on `GSW[0]` use opcode **34** (resolved operand), not
35 (raw). Ladder boundaries below come from applying the documented
next-case scanner (`EVENT_SWITCH_STACK_AND_CASE_SCANS.md`): start past the
SWITCH at depth 1, collect opcodes 36..47 at depth 1, stop at the `END_SWITCH`
49 that returns depth to 0. All eight sites terminated cleanly within the
annotated span, so each table below is the **complete** ladder.

### `gor/gor_02_init_evt` cmd 0 — the grouped form

| Cmd | Case | Value(s) |
|-----|------|----------|
| 1-7 | `43` `CASE_OR` ×7 | 18, 64, 120, 174, 221, 271, 347 |
| 8-9 | *body* | two `op91` calls |
| 10 | `46` `CASE_END` | — |
| 11 | `42` `DEFAULT` | body 12-15 tests `GSW[0] != 402` |
| 16 | `49` `END_SWITCH` | — |

This is exactly the documented OR/`CASE_END` shape: seven single-operand OR
cases, a two-command body, then `CASE_END`. A mismatch on OR *n* scans to OR
*n+1*; a match latches `-1` and the remaining OR mismatches preserve it, so
the body runs once if **any** of the seven matched.

`gor_02_init_evt` cmd 112 is the same seven chapters in the plain form:
`op36 CASE_EQUAL` on **18, 64, 120, 174, 221, 271, 347**, each with a one-command
`op92` body, `END_SWITCH` at 127, **no DEFAULT**. The two sites are an
independent cross-check that the chapter set is `{18, 64, 120, 174, 221, 271,
347}` — one ladder written as an OR group, the other as an equality chain,
with identical membership.

### `gor/gor_03_init_evt` cmd 17 — a complete partition

| Cmd | Case | Value |
|-----|------|-------|
| 18 | `47` `BETWEEN` | 278..280 |
| 19-20 | *body* | |
| 21 | `41` `>=` | 281 |
| 22-25 | *body* | includes cmd 23 `GSF[5393] == 1` |
| 26 | `49` `END_SWITCH` | no DEFAULT |

`278..280` then `>= 281` tiles `[278, +inf)` with no gap and no overlap, so
`GSW[0] < 278` falls out of the switch doing nothing at all. There is no
`DEFAULT` and no `op33` reset on that path.

### `gor/gor_03_init_evt` cmd 60 — an ascending `<` ladder

| Cmd | Case | Value | | Cmd | Case | Value |
|-----|------|-------|---|-----|------|-------|
| 61 | `38` `<` | 56  | | 71 | `38` `<` | 339 |
| 63 | `38` `<` | 112 | | 73 | `38` `<` | 373 |
| 65 | `38` `<` | 164 | | 75 | `38` `<` | 401 |
| 67 | `38` `<` | 215 | | 77 | `42` `DEFAULT` | — |
| 69 | `38` `<` | 261 | | 79 | `49` `END_SWITCH` | |

Strictly ascending, each body a single `op91`. Because an accepted case writes
state 0 and later cases then take the `state<=0` path, the ladder is
**first-match-wins**, not a set of independent tests. A port that evaluates
all eight and takes the last hit admits the wrong chapters.

### `gor/gor_03_init_evt` cmd 158 — `<` ladder with state writes

| Cmd | Case | Value | Body |
|-----|------|-------|------|
| 159 | `38` `<` | 268 | one `op91` |
| 161 | `38` `<` | 279 | cmd 162 `LW[15] <- 1`; cmd 163 `op91 … GW[12]`; cmd 164 `LW[15] <- 0`; cmd 165 `op91 … GW[13]` |
| 166 | `38` `<` | 282 | empty |
| 167 | `42` `DEFAULT` | — | one `op91` |
| 169 | `49` `END_SWITCH` | | |

Note the 279 body **resets `LW[15]` to 0** after using it, and there is an
empty 282 body — a non-match at 282 does not by itself change state.

### `gor/gor_00_init_evt` cmd 12 — a ladder that is *not* ascending

| Cmd | Case | Value | Body note |
|-----|------|-------|-----------|
| 13 | `47` `BETWEEN` | 0..3 | cmd 14 re-tests `GSW[0] != 2` |
| 21 | `47` `BETWEEN` | 4..6 | two `op91` |
| 24 | `36` `==` | 217 | |
| 28 | `36` `==` | 402 | cmd 30 `op12` on `LW[0]` |
| 38 | `36` `==` | 268 | **after** the 402 case |
| 42 | `42` `DEFAULT` | — | cmd 43 tests `GSF[1178] != 1` |
| 48 | `49` `END_SWITCH` | | |

The equality cases are ordered 217, 402, 268 — **not** ascending, and 402
sits between two lower chapters. A port that sorts or binary-searches this
ladder has changed the source. Here the outcome happens to be order
independent (the three constants are distinct and the cases are disjoint),
but that is a property of these three values, not of the mechanism.

The `0..3` body re-testing `GSW[0] != 2` means the band is split again
inside itself: chapters 0, 1 and 3 take the body and 2 does not.

### `gor/gor_00_init_evt` cmd 98 — the largest ladder

`op36 ==` on 2, 3, 4, 5, 217; `op47 BETWEEN` 172..229; `op36 ==` on 402, 268,
404; `END_SWITCH` at 146. **No `DEFAULT`.** Ordering is again non-monotonic
(217, then 172..229, then 402, then 268, then 404). Chapters matching none of
the nine cases — including 0, 1 and 6 through 171 — leave the switch with no
effect. Three of the bodies (the `== 3`, `== 4` and `== 5` cases) set
`LW[0] <- 1` via `op50`; the `== 2` body does not.

### `muj/muj_00_init_evt` cmd 82 — equality chain plus a threshold

`op36 ==` on 231, 256, 257, 259, 260, 267, then `op41 >=` 268 at cmd 135;
`END_SWITCH` at 151. **No `DEFAULT`.** Because the last case is `>= 268`, this
ladder covers `[268, +inf)` wholesale while 231-267 is handled by exact
identity only: chapters 232-255, 258 and 261-266 get no case and no effect.
The `>= 268` body is the largest in the opening — 8 `op91` calls plus writes
to `GSF[3125]` and `GSF[39]`.

### Cross-cutting findings

1. **Three dispatch forms for the same test.** The opening uses grouped `OR`
   43, plain equality 36, `BETWEEN` 47, `<` 38 and `>=` 41 on `GSW[0]`
   directly, and SWITCH 34 on top of all of them. A port that implements
   only `==` will mis-admit every ladder.
2. **First-match-wins is real.** `gor_03` cmd 60 is the clean case: an
   ascending `<` ladder where evaluating all eight and taking the last
   accepted branch gives a different chapter set.
3. **`GSF[5393]` is a genuine flag read.** It appears as operand0 of
   `op24` (`== 1`) at `gor_03` cmd 23, i.e. in a *destination/first* operand
   position, which the namespace contract routes to
   `index = wrap32(-129994607 + 130000000) = 5393` in `GSF`. So at least this
   member of the 5300-5600 band **is** a flag access. This does **not**
   generalise: in opcode-91 operand positions the same numeric band is a
   callee argument, and the two roles must be distinguished per opcode.
   `GSF` band base is `-130000000`, half-width `10000000`, matching
   `EVENT_INTEGER_OPERAND_NAMESPACES.md`.
4. **Other non-chapter operands inside the switch bodies:** `GSF[1178]`
   (`gor_00` cmd 43), `GSF[3125]` and `GSF[39]` (`muj_00` cmds 146-147),
   `LW[0]` and `LW[15]`, `GW[12]` and `GW[13]`. `GSF[39]` appearing again
   confirms it as the cross-area marker already found in `gor_00` and
   `muj_00` inits.

### Porting notes for the ladders

1. Reproduce the ladder as an **ordered** list with first-match-wins, not as
   a set of independent predicates.
2. Reproduce the exact case opcode. `OR`/`AND` groups latch `-1`/`-2` and are
   terminated by `CASE_END` 46; a plain chain of `CASE_EQUAL` 36 has no
   terminator. These are not interchangeable even when the membership sets
   match — which is why the `gor_02` cmd 0 / cmd 112 pair is a useful test
   pair rather than a redundant one.
3. `BETWEEN` 47 resolves **both** endpoints and accepts `L<=V<=H`; endpoints
   are inclusive and are not swapped.
4. Do not add a `DEFAULT` where the source has none, and do not add an
   `END_SWITCH` body where the ladder falls out with no state change.

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
- **Opcode-34 (SWITCH) case bodies are now expanded** — all eight sites
  (`gor_00` cmds 12, 98; `gor_02` cmds 0, 112; `gor_03` cmds 17, 60, 158;
  `muj_00` cmd 82), see the ladder section. What is *not* recovered is the
  runtime effect of each body: the `op91`/`op92`/`op94` service operand
  meanings are decoded elsewhere but were not cross-referenced per body
  here.
- `GSW[1420]`, seen in `gor_01` cmd 126 and `gor_02` cmd 63, has no
  extracted comparison literal.
- No execution trace.