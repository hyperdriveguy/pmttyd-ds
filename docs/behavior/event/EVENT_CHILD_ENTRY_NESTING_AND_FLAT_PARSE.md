# Child-entry nesting in a flat event span, and the INLINE_ID / BROTHER_ID closers

**Applies to:** the linear command layout of every annotated REL event span, and
to any scan that walks a span without descending into child entries.
**Evidence class:** B — **ISO-read**, static event spans from all 29 shipped
`rel/*.rel` modules (7290 events, 234878 commands). **No execution trace.**

## Why this contract exists

`tools/dol_event_inventory.py:parse_commands` is a **flat linear walk**. It
reads each command word, derives `count = word >> 16` and `opcode = word & 0xFFFF`,
and advances by `4 + 4*count`. It never descends.

The consequence is that the body of an `INLINE` or `BROTHER` child entry
**appears in the same flat list as its parent's commands**, with no marker
distinguishing them. A command index taken from the flat list is therefore
ambiguous: it may name a parent command or a command belonging to a nested
child entry. Every event census that reports "command *N*" has to answer this
before its indices mean anything.

This contract establishes the nesting structure and the depth test that
resolves the ambiguity.

## The four openers and the two closers

Byte-level layout, straight from the opcode table in
[opcode coverage](EVENT_OPCODE_IMPLEMENTATION_COVERAGE.md):

| Opcode | Role | Closed by |
|--------|------|-----------|
| 107 `INLINE` | child entry inline in the parent stream | `109` |
| 108 `INLINE_ID` | `INLINE` with an explicit ID operand | `109` |
| 110 `BROTHER` | sibling entry in the parent stream | `112` |
| 111 `BROTHER_ID` | `BROTHER` with an explicit ID operand | `112` |
| 109 `END_INLINE` | — | — |
| 112 `END_BROTHER` | — | — |

**There are four openers and only two closers.** `INLINE_ID` does **not** get
its own terminator; it shares `END_INLINE` with `INLINE`, and `BROTHER_ID`
shares `END_BROTHER` with `BROTHER`.

Verified across all 7290 events with an explicit stack. The pairing is exact
and unambiguous — no event produced a cross-family closer:

| Opener | Closer | Count |
|--------|--------|-------|
| 107 `INLINE` | 109 | 2779 |
| 108 `INLINE_ID` | 109 | 120 |
| 110 `BROTHER` | 112 | 837 |
| 111 `BROTHER_ID` | 112 | 106 |
| | **total** | **3842** |

Closer totals `109 = 2899 = 2779 + 120` and `112 = 943 = 837 + 106` match the
opener totals exactly.

## Depth is well defined and never unbalanced

Sweeping all 7290 events with `depth += 1` on 107/108/110/111 and
`depth -= 1` on 109/112:

- **0 events** ended at a non-zero depth, and **0** events drove depth
  negative. Nesting is balanced in every shipped event span.
- Maximum depth reached anywhere: **2**.
- Distribution of each event's maximum depth:

| Max depth | Events |
|---|---|
| 0 | 5850 |
| 1 | 1434 |
| 2 | 6 |

The six depth-2 events are `bom/minnnanokoe_evt`, `gon/negotiation_event`,
`kpa/kpa_01_syuppatu_evt`, `las/switch_1_evt`, `las/switch_2_evt` and
`mri/minnnanokoe_evt`.

**Depth 0 means "a command of this event's own top-level stream."** Depth 1
or 2 means the command belongs to a child entry. That is the test a flat
census has to apply before quoting a command index as a parent command.

## Independent check on the delivered chapter and SWITCH tables

The `GSW[0]` chapter table and its eight opcode-34 ladder tables were written
before this contract existed, using flat indices. Re-running the depth test
over the sixteen cited opening events:

- Every cited `GSW[0]` store — `aaa/evt_prologue2` cmd 49,
  `aaa/epilogue_evt2` cmd 117, `gor/kurihakase_first_talk` cmd 176 — is at
  **depth 0**.
- Both `END_SWITCH` commands and every case header in all eight ladders is at
  **depth 0**, and **no opener or closer marker falls inside any ladder
  span**. For example `gor/gor_00_init_evt` carries its only nested regions at
  cmds 0-11 and 169-171, and the ladders occupy 12-48 and 98-146.

So the previously delivered indices stand. This was verified rather than
assumed, and it is recorded here because the same test has **not** been
applied to any other delivery.

## Porting notes

1. A port must represent child entries as nested, not as a flat command array.
   A flat array cannot tell a parent's `op92 RUN_EVT` from a command inside
   the body of an earlier `INLINE`, and the two have different locals.
2. `INLINE_ID` and `BROTHER_ID` reuse `END_INLINE` / `END_BROTHER`. A port
   that pairs `INLINE_ID` with `END_BROTHER` (or matches by numeric adjacency)
   will mis-nest. Pair on the **family**, not on the opcode number's parity.
3. Do not assume a maximum depth of 1. Six shipped events nest two deep.
4. The [switch scanners](EVENT_SWITCH_STACK_AND_CASE_SCANS.md) run a
   *different* depth rule — they count opcode 34 and opcode 1 as openers
   against opcode 49 — because they are walking switch nesting inside one
   command stream. That rule is **not** the child-entry rule and the two must
   not be merged. In particular a `107`-opened child region is invisible to the
   switch scanners.

## Remaining gaps

- Whether an `INLINE`/`BROTHER` child entry gets its own locals or shares the
  parent's `LW` array is a handler-level question, not a layout question; the
  layout here says nothing about it.
- `op1` ("Common2 tail") is treated as a region opener by the switch scanners.
  Its relationship to the 107/109 child-entry regions is not established.
- The six depth-2 events are identified but their inner structure is not
  tabulated.
- No execution trace, and no claim about entry construction or `BROTHER`
  scheduling order.