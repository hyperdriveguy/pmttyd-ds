# Battle unit visible-position writer

US G8ME01 revision0; static contract, no execution or gameplay claim.
Closes the `BtlUnit_SetPos` row of
[UNADDRESSED_BINARY_FUNCTIONS.md](../binary/UNADDRESSED_BINARY_FUNCTIONS.md).

## Fingerprint

- Object: `artifacts/reference/behavior/BtlUnit_SetPos.elf` (208 bytes).
- Listing: `artifacts/reference/behavior/BtlUnit_SetPos.disasm` (10 lines).
- Entry `80128c6c`, body 16 bytes; `battle-jump-evidence.json` pins address
  2148699244 (= `0x80128c6c`), size 16, sha256
  `473beb07b076bbf52f19537606519838c337da49a6c284fb1761df1a7e936059`.
- `us-usage.json`: namespace `battle_unit.o`, 44 direct link sites, 19 REL
  imports. `evidence.json` cites it as a branch target (site 2148700468).

## Complete body (4 instructions, no calls, no frame)

```
80128c6c: d0 23 00 3c   stfs 1, 60(3)
80128c70: d0 43 00 40   stfs 2, 64(3)
80128c74: d0 63 00 44   stfs 3, 68(3)
80128c78: 4e 80 00 20   blr
```

Per the PPC EABI, the incoming unit pointer arrives in r3 and the three
single-precision values in f1/f2/f3. The body stores f1 at unit+0x3c (+60),
f2 at unit+0x40 (+64), f3 at unit+0x44 (+68), in X,Y,Z order, then returns.
No load precedes any store, so partial/aliased writes are impossible: each
lane is an independent store. No null/alias validation occurs.

## Offset identity

+60/+64/+68 are the visible-position triple: [unit/part position
queries](BATTLE_UNIT_PART_POSITION_QUERIES.md) attributes U3c/40/44 to visible
`GetPos`, distinct from home U30/34/38. This body writes exactly that triple.
Entry adjacency corroborates the pairing: `BtlUnit_SetPos` spans
80128c6c..80128c7b (16 bytes) and `battle-jump-evidence.json` places
`BtlUnit_GetPos` at 2148699260 (= `0x80128c7c`), the next word.

## Not established

 unit/part lifetime, who supplies the pointer/values, call cadence,
persistence, and any rendered or gameplay effect. The 44 link sites and 19 REL
imports are counts, not a caller census.
