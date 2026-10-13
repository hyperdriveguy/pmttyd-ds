# Interpolation and party run/stop event leaves

US G8ME01 disc0 revision0; static contracts, no execution, cadence or gameplay
claim. Closes `evt_sub_intpl_init` (`0x80056120`, `evt_sub.o`, size `0x98`, 44
lines), `evt_sub_intpl_get_value` (`0x80056064`, size `0xbc`, 53 lines),
`evt_party_run` (`0x800eae70`, `evt_party.o`, size `0x48`, 24 lines) and
`evt_party_stop` (`0x800eaeb8`, size `0x48`, 24 lines) from the
[unaddressed register](../binary/UNADDRESSED_BINARY_FUNCTIONS.md). None has a
definition in the pinned snapshot (rechecked with `tools/reference_source_gap.py`).

All four are event callbacks that return 2 (done) and take the event entry in
`r3`; `args = word[entry+24]`.

## Interpolation state layout

`evt_sub_intpl_init` and `evt_sub_intpl_get_value` share one record inside the
event entry:

| Offset | Role |
| --- | --- |
| `+0xC8` (200) | start value, resolved from operand 0 |
| `+0xCC` (204) | end value, resolved from operand 1 |
| `+0xD0` (208) | frame count, resolved from operand 2 |
| `+0xD4` (212) | current step counter, starts at 0 and increments per query |
| `+0xD8` (216) | limit value, resolved from operand 3 |
| `+0x9C` (156) | integer output of the interpolation |
| `+0xA0` (160) | completion flag |

## evt_sub_intpl_init (0x80056120, 44 lines)

Resolves the four operands in order with `evtGetValue` (`0x80031260`), loading
each operand word fresh (`lwz 4, [args+k]` before every call) and keeping the
results in `r29`…`r31` and `r3`. Then stores start/end/limit and writes 0 to
the step counter. Returns 2. There is no gate, no range check and no null check;
operands may be re-read differently if their source variables change between
calls.

## evt_sub_intpl_get_value (0x80056064, 53 lines)

1. Loads `end = [entry+204]` and `frames = [entry+208]`, converts both to
   binary64 with the `0x43300000`-bias double at `0x802C1860` (loaded with
   `lis 0x802C` + `addi 6240`), producing `f1 = (double)start` and
   `f2 = (double)frames`.
2. Calls `intplGetValue` (`0x80006c8c`, `system.o`, size `0x3b0`) with
   `r3 = start`, `r4 = step counter`, `r5 = [entry+216]` (limit),
   `r6 = end`, `f1`, `f2`. Its result stays in `f1`.
3. `fctiwz` converts the result toward zero, and the low word of the stored
   pair is written to `[entry+156]` — the integer output.
4. `cmpw [entry+212], [entry+216]` (`bt 0`): when the step counter is **below**
   the limit, `[entry+160] = 1`; otherwise `[entry+160] = 0`.
5. `[entry+212] = step counter + 1`; return 2.

The counter increments unconditionally, including past the limit, so the
completion flag rather than the counter is the documented end signal.

## evt_party_run (0x800eae70) and evt_party_stop (0x800eaeb8)

Both are identical except for the callee:

1. `args = word[entry+24]`; `r3 = 1` as the default slot index.
2. `r0 = word[args+0]`; `cmpwi 0,0`; `bf 2` skips the next step. When the raw
   first operand word is zero, `r3 = word[0x8041ead0]` (`r13+7088`) — the
   current party slot index. When it is nonzero the index stays 1.
3. `r3 = partyGetPtr(r3)` (`0x800bf1b4`, `party.o`): index 0 yields NULL,
   otherwise `word[0x8041ead8 + 4*index]` (the two-entry pointer table that
   `partyInit` clears).
4. `cmplwi 3,0`; `bt 2` skips the tail call when the party pointer is NULL.
5. Tail call `partyRun` (`0x800becdc`) or `partyStop` (`0x800becb8`), both
   `party.o`, with the resolved party pointer. The event body then ignores
   their returns and returns 2.

So a zero operand selects the current party slot, a nonzero operand selects slot
1, and a null pointer silently skips the request.

## Not established

- `intplGetValue` internals (its own body is not covered here), the operand
  semantic identities, and whether `frames == 0` is guarded upstream.
- Caller census for all four bodies and party run/stop cadence.
- Whether the current-index word at `0x8041ead0` can name slot 2 from event
  scripts.
