# Hit check filter (filtered raycast with outputs)

US G8ME01 disc0 revision0; static contract, no execution or gameplay claim.
Closes the open `hitCheckFilter` callee flagged in [bero
placement](../overworld/OVERWORLD_BERO_NORMAL_PLACEMENT.md).

Fingerprint: entry 8001577c, `.elf`/`hitCheckFilter.disasm` (80 lines).
`us-usage.json`: hitdrv.o, 296 bytes. Sole call: `hitVec`/`hitCheckVecFilter`
0x800158a4 ([support/slopes](../overworld/OVERWORLD_SUPPORT_SLOPES_AND_STEPS.md)),
whose nonzero-ness is the return.

## Argument shape (8 integer + 6 float registers)

r3 = query owner (copied through calls, also used for the +0 flag word the
bero caller tests... no — this body never tests it; reported positionally),
r4–r6 and r8–r10 = six output float pointers ([r25],[r26],[r27],[r29],[r30],
[r31]), r7 = extra float-input pointer, f1–f6 = float inputs. Table words
come from two absolute tables (0x802C0000−2120/−2108).

## Order

1. Marshal: table words + bit-copied float args (via `stfs`/`lwz` int
   round-trips, preserving exact bits) into the stack query struct at sp+32.
2. `hitVec(query)`; null result → return 0 (`li 3,0` path).
3. Else copy six result floats ([sp+68/72/76/80/84/88]) through the six
   output pointers and return `hitVec`'s nonzero value (fall-through with
   r3 untouched since the call).

The r7 input contributes only `lfs [r7]` into the query; its meaning is
open. No output is written on the miss path.

## Not established

Query owner/flag roles, table contents, output-lane meanings, `hitVec`
internals beyond the cited doc, cadence, and any gameplay effect.
