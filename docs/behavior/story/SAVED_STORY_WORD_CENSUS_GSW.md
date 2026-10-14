# Game-wide `GSW` saved-word census (all 29 shipped REL modules)

**Applies to:** every operand in every annotated event span of all 29
`rel/*.rel` modules shipped on US G8ME01 revision 0.
**Evidence class:** B — **ISO-read**, static event spans only. **No execution
trace.** Scope is REL event code; it is **not** a claim about the whole binary.

## Scope and method

| Quantity | Value |
|---|---|
| REL modules read | 29 |
| Events parsed | 7290 |
| Commands parsed | 234878 |
| Source | `/home/carson/games/roms/G8ME01.iso`, read-only |

Parsed with `tools/dol_event_inventory.py:parse_commands`, the same reader
[rel_event_inventory.py](../event/EVENT_ENTRY_PACKED_LAYOUT.md) already uses, so the
byte walk and the command-word decoding are identical to the existing
contracts. `GSW` membership uses the documented band
`-180000000 < operand <= -160000000`, base `-170000000`,
`index = wrap32(operand - base)` from
[operand namespaces](../event/EVENT_INTEGER_OPERAND_NAMESPACES.md).

**Writer classification is explicit, not inferred.** An opcode is a writer when
operand0 is a destination. Taken from
[opcode coverage](../event/EVENT_OPCODE_IMPLEMENTATION_COVERAGE.md):

- Writers: `50` `SET resolved`, `51` `SET raw`, `52` `SET float`, `53` `ADD`,
  `54` `SUB`, `55` `MUL`, `56` `DIV`, `57` `MOD`, `58`-`61` float arithmetic,
  `63`-`67` integer reads, `69`-`72` float reads, `73` indexed read, `74`
  `Set user work`, `75` `Set user flags`, `77` `AND resolved`, `78` `AND raw`,
  `79` `OR resolved`, `80` `OR raw`, `81`/`82` conversions, `83`-`86` raw
  address load/store, `87`/`88` resolved destination, `97`-`99` priority /
  speed / type.
- Not writers: `0`-`23` (fetch, loop, wait, string and float comparisons),
  `24`-`31` (integer comparisons, flag tests), `32`/`33` (`ELSE`/`END_IF`),
  `34`-`49` (switch and cases), `62`/`68` (data cursor), `76`
  (allocate), `89`-`96` (call/run/child/delete), `100`-`106` (start/stop/check),
  `107`-`118` (entry construction, inline, brother, no-ops, debug, metadata).

### Correction to an earlier sweep

An earlier pass in this session used only `{50, 51, 53, 54, 55, 56, 57, 58, 63}`
as writers and therefore **missed opcode 79** (`OR resolved`) and `77`/`80`.
That omission hid the only bitfield word in the game. The set above is the
corrected one and is the set behind every number in this contract.

## The map: 31 saved words, 2213 references

| `GSW` index | Writes | Reads | Writer opcodes | Observed character |
|---|---|---|---|---|
| **0** | **427** | **1514** | `50` ×427 | chapter counter, values 2..405 |
| **1420** | **33** | **86** | `50` ×33 | 0..30 counter; no nonzero writer |
| 2 | 1 | 2 | `50` | copy of `LW[2]` |
| 3 | 1 | 2 | `50` | copy of `LW[2]` |
| 33 | 1 | 0 | `50` | copy of `LW[0]` |
| 116 | 1 | 4 | `53` | `+= 1` in `gor` |
| 117 | 1 | 0 | `50` | copy of `LW[1]` |
| 301 | 2 | 2 | `50`, `53` | clear to 0, `+= 1` |
| 501 | 4 | 17 | `50` | tou tournament state |
| 502 | 2 | 0 | `50` | |
| 503 | 2 | 0 | `50` | |
| 504 | 2 | 0 | `50` | |
| 505 | 1 | 2 | `53` | `+= 1` |
| 506 | 1 | 3 | `50` | |
| 651 | 2 | 0 | `50` | `9`, `98` |
| 665 | 0 | 2 | — | read only |
| 666 | 1 | 2 | `50` | `= GSW[665]` |
| 721 | 1 | 3 | `53` | `+= 1` death counter |
| 821 | 1 | 0 | `50` | init to 0 |
| 822 | 1 | 0 | `50` | init to 0 |
| 823 | 1 | 0 | `50` | init to 0 |
| 870 | 6 | 0 | `50` | **train route**, values 0/1/2/3 |
| 1071 | 2 | 4 | `53` ×2 | `+= 1` twice, aji |
| 1121 | 2 | 2 | `50`, `53` | door open state |
| 1122 | 8 | 8 | `53` ×8 | **key count**, `+= 1` ×8 |
| 1123 | 5 | 6 | `50` ×5 | **enemy kill index**, values 0,1,2,3,4 |
| 1124 | 1 | 0 | `50` | copy of `LW[0]` |
| 1221 | 2 | 1 | `50`, `53` | kpa: init 1, `+= 1` on death |
| 1222 | 2 | 3 | `50` ×2 | kpa: `= 180` |
| 1321 | 12 | 8 | `50` ×9, `53` ×3 | **jon floor selector / progress** |
| **1469** | 10 | 6 | `50`, `53`, **`79`** | **the only bitfield word** |

No other `GSW` index is referenced anywhere in the 29 modules.

### Structural findings

1. **`GSW[0]` is the chapter counter and it is enormous.** 427 stores,
   **394 distinct values**, and the values form a near-contiguous band from
   **2 to 405**.
2. **`GSW[1420]` is a second, small counter with a 0..30 domain** — see below.
   It is *not* the boolean I described in an earlier delivery; see
   *Corrections*.
3. **Only 11 of 31 words are ever written by a non-assignment opcode.** `53`
   (`ADD`) appears for 116, 301, 505, 721, 1071, 1121, 1122, 1221, 1321, 1469;
   `79` (`OR`) only for 1469. Everything else is plain `50`.
4. **Several words are pure scratch copies** (`2`, `3`, `33`, `117`, `1124`),
   each written once from an `LW` local. They persist across events but have
   no second writer in REL code.
5. **`GSW[665]` is never written** in REL code; `GSW[666]` copies from it.

## `GSW[0]`: the chapter counter

### The value set

427 opcode-50 stores; 426 have an immediate literal source, 1 does not.
**394 distinct values, spanning 2..405, with exactly ten gaps:**

```
22   36   126   177   186   281   349   351   357   381
```

This is dense enough to establish the numbering as a single game-wide
sequence shared by all areas, not per-area counters. Note that **281 is never
written** even though `gor_03_init_evt` cmd 21 tests `>= 281` — the threshold
is real even though the exact value is never produced.

**The single non-literal store** is `tou2/evt_random_battle2` cmd 83,
`GSW[0] <- LW[0]`, whose cmd 80 is the mirror `LW[0] <- GSW[0]`. It is a
save/restore pair through a local, not an arithmetic advance.

### Chapter map by module

Module ownership of the numbering. Runs are contiguous value ranges; a value
written by two events appears twice.

| Module | Stores | Range | Chapter-value runs |
|---|---|---|---|
| `aaa` | 2 | 2-403 | 2, 403 |
| `tik` | 23 | 12-380 | 12-18, 21, 64, 66-70, 120, 174, 220-221, 271, 347, 378-380 |
| `gon` | 21 | 35-56 | 35, 37-56 |
| `gra` | 10 | 184-210 | 184, 188-193, 203, 207, 210 |
| `hei` | 13 | 23-115 | 23, 23-25, 29-31, 31-34, 63, 115 |
| `win` | 15 | 71-119 | 71-73, 77-87, 119 |
| `gor` | 42 | 3-405 | 3-11, 19-20, 65, 121-125, 173, 175-176, 218, 222-229, 269-270, 272-273, 278-280, 342, 345-346, 348, 350, 405 |
| `kpa` | 6 | 60-344 | 60-61, 116, 217, 343-344 |
| `mri` | 32 | 74-171 | 74-76, 88-112, 118, 127, 170-171 |
| `nok` | 5 | 26-117 | 26-28, 62, 117 |
| `tou` | 47 | 127-172 | 127-130, 130-132, 134-137, 137-138, 138-139, 139-143, 143, 143-144, 144-152, 152-153, 153-158, 158-163, 172 |
| `jin` | 14 | 194-213 | 194-195, 195-201, 208-209, 211-213 |
| `muj` | 31 | 216-277 | 216, 230-242, 251-253, 253-254, 257-261, 268, 274-275, 275-276, 276-277 |
| `usu` | 16 | 178-267 | 178-183, 185, 187, 202, 204-206, 214, 219, 266-267 |
| `dou` | 10 | 243-256 | 243-250, 255-256 |
| `rsh` | 43 | 282-334 | 282, 282-285, 285-290, 290-291, 291-292, 292, 292-309, 309, 327-334 |
| `eki` | 15 | 313-325 | 313-316, 316, 316-325 |
| `hom` | 4 | 310-326 | 310-312, 326 |
| `aji` | 33 | 57-376 | 57-59, 113-114, 165-167, 167-169, 215, 262-265, 340-341, 360-373, 376 |
| `pik` | 8 | 335-375 | 335-339, 355, 374-375 |
| `moo` | 3 | 358-377 | 358-359, 377 |
| `bom` | 6 | 352-356 | 352, 352-356 |
| `tou2` | 4 | 133-178 | 133, 164, 178 (+ the `LW[0]` copy) |
| `las` | 22 | 382-402 | 382-401, 401-402 |
| `end` | 2 | 404 | 404, 404 |
| `sys`, `dmo` | 0 | — | no `GSW[0]` store |

`sys` and `dmo` write no chapter value at all. `dmo` is the demo module and
`sys` holds 3 events.

### The chain ends are now traced

The endpoints of the numbering are individually attributed:

| Value | Writer | Module |
|---|---|---|
| **2** | `evt_prologue2` cmd 49 | `aaa/aaa_00.o` |
| 3 | `enter_gorotsuki_town` cmd 203 | `gor/gor_00.o` |
| 4 | `christine_osoware` cmd 85 | `gor/gor_00.o` |
| 5 | `gundan1000tai_beforebattle` cmd 119 | `gor/gor_00.o` |
| 380 | `mahojin_event7` cmd 517 | `tik/tik_05.o` |
| **382** | `first_evt` cmd 45 | `las/las_00.o` |
| 400 | `koopa_evt` cmd 429 | `las/las_28.o` |
| 401 | `last_evt_3` cmd 319 / `last_evt_3_2` cmd 399 | `las/las_29.o` |
| **402** | **`last_evt_4` cmd 489** | **`las/las_29.o`** |
| **403** | `epilogue_evt2` cmd 117 | `aaa/aaa_00.o` |
| **404** | `unk_evt_end_00004654` cmd 5, `unk_evt_end_00004718` cmd 15 | `end/end_unknown.o` |
| **405** | `return_gorotsuki_town` cmd 206 | `gor/gor_00.o` |

**This resolves the two sentinels.** `402` — the value every opening gate
tests — is written by `las/last_evt_4`, the last of the `las_29` chapter
sequence. `404` is written by the two `end` module events. `403` is the
epilogue. And **405 is written by `gor/return_gorotsuki_town`**, a return to
the starting town, i.e. a post-ending state above `404`.

The earlier delivery's open question — the origin of 402 — is closed, and the
ordering the opening modules *imply* (402 pending → 2 → … → 403) is wrong at
the front: 402 is not a pre-prologue sentinel, it is written at the **end** of
the `las` chapter band, immediately before 403.

**Not claimed:** that 2 is the first chapter, or that values 3..401 form a
strictly linear player route. A value being *writable* does not make the
event reachable, and no execution trace was taken. What is established is the
set of written values and their attributed producers.

### Repeated values

Some values are written by more than one event, which is evidence of branching
or revisits rather than of a single linear spine: 167 (`aji`), 401
(`las/last_evt_3` and `last_evt_3_2`), 316 (`eki`, three times), 404 (`end`,
twice), and 352 (`bom`, twice).

## `GSW[1420]`: a 0..30 counter, and a correction

### Correction

An earlier delivery in this session described `GSW[1420]` as a boolean story
word gated on `== 1` / `!= 1`. That was **accurate about those two sites and
wrong about the word**. Across all 29 modules `GSW[1420]` has **86 read sites
in 13 modules**, compared with opcode mix `24` (`==`) ×75, `25` (`!=`) ×7,
`34` (`SWITCH`) ×3 and `115` (debug) ×1, against **exactly 32 distinct
literals**:

```
0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23
24 25 26 27 28 29 30
```

That is a **contiguous 0..30 domain**, not a two-state flag. The two sites
cited earlier (`gor_01` cmd 126 `== 1`, `gor_02` cmd 63 `!= 1`) are two points
inside that range and nothing more.

### The writers

33 writes, **all opcode 50**. 32 store the literal **0**; one
(`gor/gor_irai_main` cmd 38) stores `LW[11]`.

**No shipped REL event ever writes `GSW[1420]` a value in 1..30.** With the
corrected writer set — which includes `51`, `53`-`57`, `77`-`80` — there is no
`+= 1`, no `OR`, no `SET raw` and no indexed write. The only place that treats
it as a number is the `gor_irai` (tower) pair:

```
gor_irai_tenshu_main  cmd54  LW[10] <- GSW[1420]
                     cmd56  LW[10] -= 1
                     cmd57  LW[11] <- LW[10]
                     cmd65  if LW[10] >= 30            <- the 0..30 ceiling
                     cmd68  LW[11] <- GSW[1420]
                     cmd69  op91 ... GSW[1420]
                     cmd70  GSW[1420] <- 0
```

and `gor_irai_main` cmd 38 `GSW[1420] <- LW[11]`, whose `LW[11]` producer is
inside an `INLINE` block and is not resolved by this sweep (see
[child-entry nesting](../event/EVENT_CHILD_ENTRY_NESTING_AND_FLAT_PARSE.md)).

So the ceiling **30** is independently corroborated by the one event that does
arithmetic on the word. That is the strongest statement the evidence supports.

**Not claimed:** any account of where a 1..30 value comes from. It is outside
the 29 REL modules or reached through a form this sweep does not cover.
Naming a save-block default or calling the comparisons dead would both be
invention, so neither is done here.

### Read distribution

`GSW[1420]` is read in 13 modules — `gor` (35), `pik` (9), `bom` (8), `tou` (8),
`jon` (7), `nok` (4), `tik` (3), `win` (3), `dou` (2), `gon` (2), `mri` (2),
`usu` (2), `rsh` (1). A word consulted across a third of the game with a 0..30
domain is a global sequence, not a local flag.

## `GSW[1469]`: the only bitfield

The only `GSW` word written by opcode 79 (`OR resolved`, documented as
`destination value OR resolved operand1`):

```
rsh/talk_cook@rsh_03.o  cmd42  GSW[1469] |= 1
rsh/talk_cook@rsh_03.o  cmd46  GSW[1469] |= 2
rsh/talk_cook@rsh_03.o  cmd50  GSW[1469] |= 4
```

Three distinct bits in one saved word, all in a single event, at consecutive
command groups. Alongside `win/iri_28_init` cmds 1/6/12/18 (clear, then three
`+= 1`) and `win/iri_28_enemy_dead` / `gor/iri_09_tentyo_talk` (`+= 1`), the
word mixes bit-set and count semantics.

A port that models every `GSW` index as an integer counter will pass the
`+= 1` sites and break the three `|=` sites.

## `GSW[1122]` / `GSW[1123]` / `GSW[870]` / `GSW[1321]`: counters worth naming

These are readable from the writer set alone, so they are stated with their
evidence rather than left open:

- **`GSW[1122]` — key count.** Eight events `las/kagi1_evt` through
  `las/kagi8_evt` each do `GSW[1122] += 1` at cmd 20, 8 reads. The event names
  are the Japanese *kagi* (key). This is a count of eight distinct keys.
- **`GSW[1123]` — enemy kill index.** `las/enemy1_dead_evt` … `enemy4_dead_evt`
  store **1, 2, 3, 4** and `las_17_init_evt` stores 0; 6 reads. This is
  "which enemy was killed", not a count.
- **`GSW[870]` — train route.** Six writes across four modules with values
  0, 1, 2, 3 (`gor/evt_train`, `hom/evt_train_nidome`, `pik/evt_train`,
  `rsh/evt_nidome_bed`), zero reads in REL code.
- **`GSW[1321]` — jon floor selector.** `jon/jon_init_evt` cmds 4-22 store
  `0, 50, 80, 9, 49, 79, 99` in order, and `jon/floor_inc` does `+= 1`,
  `jon/floor_clear` stores 0, `jon/talk_idouya` does `+= 2` then `+= 5`. 8
  reads.

## Porting notes

1. `GSW[0]` must be a **word** and must hold **2..405**. The ten gap values
   must stay unwritten; a port that increments by 1 instead of taking the
   stored literal will produce chapters that never existed.
2. The chapter value is set by **opcode 50 with a literal**, from 426 of 427
   sites. Implement it as an assignment of a constant, not as an increment.
3. Do not normalise the numbering per area. It is one game-wide sequence, and
   `gor` legitimately spans 3..405.
4. `GSW[1469]` needs **bit** semantics for its three `|=` sites. Any other
   `GSW` word written only by `50` and `53` is integer arithmetic.
5. `GSW[1420]` needs a 0..30 domain in whatever type holds it, because the
   ceiling appears in a `>= 30` comparison.
6. Six words (`2`, `3`, `33`, `117`, `1124`, `666`) are single-copy scratch
   words. Keep them persistent — they are read back in later events — but do
   not expect a second writer.

## Remaining gaps

- **No execution trace.** Everything here is which values are written and
  which are read, not which paths execute.
- **Reachability.** A written chapter is not a reachable chapter. The
  producer table does not establish a player route, and the eight `sys`/`dmo`
  modules with no store are not explained.
- **`GSW[1420]`'s nonzero producer**, and whether the 0..30 read sites are
  reachable.
- **Names.** The chapter *titles* are still not recovered; what is new here is
  the **producer event** for each value, which is a stronger anchor than the
  comparison literals but still not a title.
- **`GSW[665]`** has readers but no REL writer.
- **Save-block mapping.** These 30 words are the in-memory names; how they
  serialise into the card record is not covered here.
- Scans of DOL-side (`_main`) events and any non-REL story code are outside
  this census.