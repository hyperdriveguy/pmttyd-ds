# Battle unit status check

US G8ME01 disc0 revision0; static contract, no execution or gameplay claim.
Closes the open `BtlUnit_CheckStatus` callee flagged in [unit-query
leaves](BATTLE_UNIT_QUERY_EVENT_LEAVES.md) and used by [hit
precheck](BATTLE_HIT_PRECHECK.md) and damage calculation.

Fingerprint: entry 80127814, `.elf`/`BtlUnit_CheckStatus.disasm` (37 lines).
`us-usage.json`: battle_unit.o, 124 bytes. Callee `BtlUnit_GetStatus`
0x80127f34 (battle_unit.o, 452 bytes; no prose contract — open) is identified
by the damage-calculation annotation, consistent with this body's two
stack-byte outputs.

## Complete body

`BtlUnit_CheckStatus(unit, status)`:

1. Null unit (`mr.` + taken-equal branch) → return 0. No other validation.
2. `BtlUnit_GetStatus(unit, &stk9, &stk8)` — two output bytes; only the
   first (`stk+9`) is ever read below. Its return value is discarded.
3. If signed byte stk+9 != 0 → return 1.
4. Else if sign-extended `status` != 6 → return 0.
5. Else return ([unit+759] != 0) ? 1 : 0 (`lbz` + null test).

Return domain is exactly {0,1} on every path. The second GetStatus byte,
the `status` values other than the 6-comparison, and the +759 flag's meaning
are not established here. Callers branch on ==0 versus nonzero (the
CheckStatus leaf writes 0/1 accordingly), so no other return shape exists to
discover.

## Not established

`BtlUnit_GetStatus` internals, status-code table, +759 ownership, cadence,
and any gameplay effect.
