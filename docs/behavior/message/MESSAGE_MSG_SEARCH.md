# Message search (two sorted tables plus fallback)

US G8ME01 disc0 revision0; static contract, no execution or gameplay claim.
Closes the open `msgSearch` callee flagged in [action-class
table](../battle/BATTLE_MAKE_ACT_CLASS_TABLE.md) (20 existing mentions, no contract).

Fingerprint: entry 80081b8c, `.elf`/`msgSearch.disasm` (62 lines).
`us-usage.json`: msgdrv.o, 224 bytes. Sole library call: `strcmp`
0x8026d464 (MSL).

## Complete body

`msgSearch(key)` — only r3 is loaded; r4/r5 residues from the caller are
never read, so the table-builder's single-argument calls are well-formed:

1. For table index t = 0, 1: record = word at [global−28272] + 16·t, where
   the global is the word at [r13−28272] (distinct from gp at [r13−28496]).
   Skip the table when [rec+12] is null.
2. Binary-search the sorted array: entries base = [rec+0], count =
   [rec+8], midpoint = (lo+hi+1)/2 via the sign-safe
   `srwi`/`add`/`srawi` idiom, entry stride 8. `strcmp([base+8·mid], key)`:
   equal → return (word at [rec+4]) + (word at [matched+4]); less →
   lo = mid−1; greater → hi = mid+1; continue while hi ≥ lo (signed).
3. Both tables missed → fallback: index = word at [gp+364];
   return word at ([r13−28268] + 4·index).

There is no miss-null path: exhaustion returns the gp fallback word, whose
null-ness is the caller's problem. The two tables' contents, the +4/+12
fields, the gp fallback table, and the returned values' meanings are not
established here.

## Not established

Table ownership/lifetimes, key provenance, returned-pointer contracts,
cadence, and any gameplay effect.
