# Opening area init story-state map: `GSW[0]`, `GSF` and the area exits

**Applies to:** `gor`, `muj`, `tik` opening / area-init events
**Evidence class:** B — **ISO-read**, static event spans and relocated operands only. **No execution trace.**

## What this corrects

The companion [opening scene state](OPENING_SCENE_STATE_AND_PERSISTENCE.md)
states that `GSF` (the saved flag family) is "**never** used by any opening
event". That was true only of the three modules scanned there (`dmo`, `aaa`,
`sys`) and **is wrong as a general claim**. Scanning the area-init events of
`gor`, `muj` and `tik` shows `GSF` is the **dominant** persistent flag family
in the opening areas, with dozens of reads and a dozen writes.

That earlier document's scope line has been narrowed to its actual claim.

## Method

Identical to the companion contract: `tools/rel_event_inventory.py` on the
read-only US ISO, integer operands routed through the published
[namespace table](../event/EVENT_INTEGER_OPERAND_NAMESPACES.md)
(`index = wrap32(operand - base)`, signed32), opcode direction from the
[assignment](../event/EVENT_ASSIGNMENT_ARITHMETIC_AND_DATA_CURSOR.md) and
[IF](../event/EVENT_IF_INTEGER_MASK_AND_CONTINUATIONS.md) contracts.
`WRITE` = opcodes 50, 51, 53..56, 63, 106; `READ` = 12..31, 34, 16, 17, 22.

## `GSW[0]` is the chapter gate, read everywhere

`GSW[0]` is read by **every** area-init event scanned. It is the chapter /
progress switch, not a per-scene local.

| Event | Reads of `GSW[0]` | Command indices |
|-------|-------------------|-----------------|
| `gor/gor_00_init_evt` | 12 | 12, 14, 55, 66, 87, 90, 93, 98, 147, 148, 149 |
| `gor/gor_01_init_evt` | 10 | 1, 5, 14, 44, 47, 54, 60, 85, 100, 106, 167 |
| `gor/gor_02_init_evt` | 11 | 0, 12, 20, 26, 52, 97, 101, 102, 109, 112, 164 |
| `gor/gor_03_init_evt` | 6 | 17, 60, 96, 150, 153, 158 |
| `muj/muj_20_init_evt` | 5 | 29, 56, 61, 92 |
| `muj/muj_00_init_evt` | 9 | 12, 19, 20, 21, 37, 63, 70, 75, 76, 82 |
| `tik/tik_00_init_evt` | 3 | 0, 1, 47 |

Opcode mix is `24` (`==`), `25` (`!=`), `26` (`<`), `28` (`<=`), `29` (`>=`)
and `34` (SWITCH on the resolved value) — so the gate is a **multi-way
comparison**, not a single equality. Any port that tests only `== 0` will
mis-admit chapters.

**A second saved story word appears:** `GSW[1420]`, read at
`gor_01_init_evt` cmd 126 and `gor_02_init_evt` cmd 63. It is not the index-0
exception, so it is a byte in the GP+578+ array — the first evidence that
the opening uses a story word beyond `GSW[0]`.

## The Professor writes `GSW[0]`

This is the writer the `dmo`/`aaa`/`sys` scan did not contain:

| Event | Cmd | Op | Direction |
|-------|-----|----|-----------|
| `gor/kurihakase_first_talk` (`gor_02.o`) | 176 | 50 | **WRITE `GSW[0]`** |

`kurihakase_first_talk` — the professor's first conversation, in
**`gor_02`** — also writes `GF[0]` at cmds 69 and 72 and reads it at cmd 84
under opcode 11. Its `GSW[0]` write at cmd 176 is near the end of a
180-command event and is the **only** `GSW[0]` write found anywhere in the
sixteen events scanned for this contract and its companion.

So the chapter transition is driven by the professor scene, not by the
prologue or epilogue events. The prologue/epilogue writes recorded in the
companion contract (`aaa/evt_prologue2` cmd 49, `aaa/epilogue_evt2` cmd 117)
are therefore **not** the only chapter advance, and a port that treats them
as the sole producer will miss the professor branch entirely.

## Saved-flag (`GSF`) reads and writes

`GSF` indices in the high 1100-1200 range cluster around the opening areas.
Read/written pairs are marked.

| Event | `GSF` writes | `GSF` reads (high band) |
|-------|--------------|--------------------------|
| `gor/gor_00_init_evt` | **1180** (cmd 152), **1178** (cmd 157), **39** (cmd 158) | 1178 (43), 1181 (94), 1180 (151), 1178 (156) |
| `gor/gor_01_init_evt` | **1191** (55, 66), **1189** (61, opnd1), **1190** (62, opnd1) | 1200 (48), 1191 (52), 1189 (53), 1191 (59, 101), 1188 (91), 1196 (143) |
| `gor/gor_02_init_evt` | **1203** (cmd 23) | 1219 (84), 5375 (90), 1221 (132, 137) |
| `gor/gor_03_init_evt` | **1229** (cmd 93) | 1229 (0, 7, 92), 5393 (23), 1230 (30), 1236 (154) |
| `muj/muj_20_init_evt` | — | 3125 (46), 1178 (51) |
| `muj/muj_00_init_evt` | **3125** (cmd 146), **39** (cmd 147) | 3125 (13, 145) |
| `tik/tik_00_init_evt` | **6** (cmd 15), **237** (cmd 16) | — |

**`GSF[39]` is written by both `gor_00_init_evt` and `muj_00_init_evt`** — the
same flag set from two different area inits, so it is a cross-area marker
rather than an area-local one.

The 5300-5600 band appears only as **operand 6/7/8 of opcode-91 user
functions** in every event above (`GSF[5525]`..`[5588]`). Those are almost
certainly **message or actor identifiers passed to a callee**, not flag
accesses by this event; they are listed here only so the next reader does
not re-count them. Direction is undetermined for all of them.

## Area exits recovered

Twelve `evt_bero_mapchange` exits across the scanned events, on top of the
thirteen in the companion contract:

| Event | Cmd | Fade before | Map | Entrance |
|-------|-----|-------------|-----|----------|
| `gor/gor_muj_ship_start` | 79 | — | **`muj_20`** | — |
| `muj/muj_gor_ship_start` | 75 | — | **`muj_20`** | — |
| `muj/evt_prologue` (`muj_20.o`) | 49 | 48 `(1, 17, 800, 16, 800)` | **`sys_01`** | `prologue` |
| `muj/epilogue_evt` (`muj_20.o`) | 20 | 19 `(1, 10, 900, 9, 900)` | **`muj_01`** | — |
| `gor/epilogue_evt` (`gor_00.o`) | 405 | — | **`muj_21`** | — |
| `tik/epilogue_evt` (`tik_00.o`) | 36 | 35 `(1, 10, 900, 9, 900)` | **`moo_04`** | — |

Two things fall out of this:

1. **`sys_01` is entered from `muj_20`'s prologue**, not from `aaa`. The
   companion contract found `sys/evt_start` issuing nine map changes *into*
   `gor_10`, `hei_00`, … and `aaa/evt_prologue2` issuing `muj_20/prologue`.
   Now `muj_20/evt_prologue` issues `sys_01/prologue`. So the `muj_20`
   prologue scene is the join point between the `aaa` prologue and the
   `sys_01` sequence.
2. **`muj_20` is a bidirectional join.** `gor_muj_ship_start` (in `gor_00`)
   and `muj_gor_ship_start` (in `muj_00`) both exit to `muj_20`, i.e. the
   ship voyage is entered from either side and lands on the same map.

## Event fingerprints

| Event | Namespace | Commands | SHA-256 |
|-------|-----------|----------|---------|
| `gor_00_init_evt` | `gor_00.o` | 174 | `feca019e68a4b78036ea454dee444be4287f18b1b2f3c322d82b232190a68496` |
| `gor_01_init_evt` | `gor_01.o` | 177 | `aa2fada48b78e1eb7d6eddd114e1bd875e6dc7744a8d2590bb7cf588db1acb7e` |
| `gor_02_init_evt` | `gor_02.o` | 171 | `649d12889da97f2b7b29aa59f59f58671dfb916195160127f594df927e84109e` |
| `gor_03_init_evt` | `gor_03.o` | 173 | `bb0a5d4f1232e2575d56f2bc93ee4e81cd25570eb188e506985375329afe89b0` |
| `kurihakase_first_talk` | `gor_02.o` | 180 | `9e27be37f4d93218ea9204cf25d17eef07876c130d25f13c8b5470739f545511` |
| `gor_muj_ship_start` | `gor_00.o` | 82 | `fd16e656b51afc2dfebf50e7b86ebbaab8110204bf38245dc57158fb9632fb71` |
| `epilogue_evt` | `gor_00.o` | 408 | `6dc8925f3a866fae269e850e3371687949775b98fe21cd9d4e52c4f63eca12fa` |
| `epilogue_npc_entry` | `gor_00.o` | 85 | `cbc2aeafb7171f54d51e3a873ba4216f704db51e09bbc87deb532e884d1a51ae` |
| `muj_20_init_evt` | `muj_20.o` | 102 | `131ac2bc41c846b6d8ea0c57d026eccde545dd4d80927307109c0bf4f03b31b2` |
| `evt_prologue` | `muj_20.o` | 78 | `f3ba8a3667a705bc6875d19ade724d0dbb934b47a466f2642c660c7f7d3f6024` |
| `epilogue_evt` | `muj_20.o` | 23 | `b3a534e2d34b7421362802465ffe849c9fede1a5aa520c9d51329b5bd497773d` |
| `muj_gor_ship_start` | `muj_00.o` | 78 | `8b7d2c8d0c9ecb4f75e82229627aa4ace991ba62f4db20ff398ca5d614dc25fa` |
| `muj_00_init_evt` | `muj_00.o` | 155 | `a661c38b3110bbe159a5b87c36931b9f99ec7f0ad93bb48489690bffa67626c7` |
| `tik_00_init_evt` | `tik_00.o` | 59 | `1b769a7ca40730b3590e8dff99160267f1ec2f88b68caf8da714afe0942ce45c` |
| `epilogue_evt` | `tik_00.o` | 39 | `189833573d63b49ef4aca884300a2a0bb6cab8212adddb4785438dda233ce228` |

## Porting notes

1. `GSW[0]` gates every opening area init, and it is a **multi-way
   comparison** (`==`, `!=`, `<`, `<=`, `>=`, SWITCH). A single equality
   test will mis-admit chapters.
2. The **professor** scene (`kurihakase_first_talk`, in `gor_02`) is the
   writer of `GSW[0]`. Treating the prologue/epilogue writes as the sole
   chapter advance misses the professor branch.
3. `GSF[39]` is shared across `gor_00` and `muj_00` area inits.
4. `muj_20` is a **bidirectional join**: both `gor_muj_ship_start` and
   `muj_gor_ship_start` exit into it.
5. `GSW[1420]` is a second story word in the opening, stored as a byte
   (not the index-0 word exception).

## Remaining gaps

- **Which `GSW[0]` comparison value corresponds to which chapter.** The
  operand values are literals on the right-hand side of each comparison and
  were not extracted; only the operand *index* and band are tabulated.
- The 5300-5600 `GSF` band in opcode-91 operand positions — very likely
  callee arguments rather than flag accesses, unconfirmed.
- Opcode **11, 52, 93, 95 and 108** directions remain undetermined
  throughout; this affects the `GF`/`GW` rows in the companion contract.
- `GSF` semantics: no flag here is named, and none is mapped to a save-block
  slot. That remains with [GSF-01](../../requests/GSF-01.md).
- `gor_04_init_evt` and the remaining area inits were not scanned.
- No original execution trace; every claim is static.