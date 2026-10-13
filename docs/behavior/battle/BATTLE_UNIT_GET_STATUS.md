# Battle unit status-byte dispatch

US G8ME01 disc0 revision0; static contract, no execution or gameplay claim.
Closes the open `BtlUnit_GetStatus` callee flagged in [unit status
check](BATTLE_UNIT_CHECK_STATUS.md).

Fingerprint: entry 80127f34, `.elf` 644 bytes, 119-line `.disasm`, zero `bl`
(leaf apart from the return). `us-usage.json`: battle_unit.o, 452 bytes.
Jump table 0x803642D8 (28 slots for statuses 0–27) read from the read-only
ISO through the DOL section map.

## Entry protocol

`BtlUnit_GetStatus(unit, status, outA, outB)` — r3/r4/r5/r6. r7/r8 are
zeroed on entry, so outputs default to 0. Either output pointer may be null:
the exit stores r7 only if r5 != 0 and returns early (conditional `bclr`)
when r6 == 0, else stores r8. No validation of `unit` here (the CheckStatus
wrapper owns the null gate).

## Dispatch

Signed `status` > 27 → skip to the exit with both outputs 0. Otherwise the
28-slot table selects (all offsets from `unit`, all byte loads):

| Status | Outputs (r7, r8) |
| --- | --- |
| 0 | (298, 298) |
| 1 | (280, 280) |
| 2 | (282, 282) |
| 3 | (283, 283) |
| 4 | ([284], [285]) straight pair |
| 5 | (286, 286) |
| 6 | (287, 287) |
| 7 | (288, 288) |
| 8 | (289, 289) |
| 9 | (290, 290) |
| 10 | [292] > 0 → (291, [292]); else (0, 0) |
| 11 | [292] < 0 → (291, [292]); else (0, 0) |
| 12 | [294] > 0 → (293, [294]); else (0, 0) |
| 13 | [294] < 0 → (293, [294]); else (0, 0) |
| 14 | [296] > 0 → (295, [296]); else (0, 0) |
| 15 | [296] < 0 → (295, [296]); else (0, 0) |
| 16 | (297, 297) |
| 17 | (299, 299) |
| 18 | (300, 300) |
| 19 | (303, 303) |
| 20 | (304, 304) |
| 21 | (301, 301) |
| 22 | (302, 302) |
| 23 | ([305], [306]) straight pair |
| 24 | ([307], [308]) straight pair |
| 25, 26 | (0, 0) — slots target the exit prologue directly |
| 27 | (309, 309) |

"r7 copy" = `mr r7, r8` after the load, so both outputs carry the same byte
(single-lane cases). Straight pairs load two adjacent bytes. Only the six
paired cases (10–15) test a value; everything else is an unconditional lane
copy. Note the non-monotonic lane order (298 served at status 0, 300 shared
by statuses 18 and 21, 302 only at 22) — the table above is machine-verified
against the listing and the ISO-read jump table, not inferred sequential. The CheckStatus
wrapper reads only the first output and returns 1 on nonzero — consistent
with these lanes being presence/count bytes, but their game meanings are
not established here.

## Not established

Status-code table, lane meanings, unit lifetime, cadence, and any gameplay
effect. The jump-table/branch mapping above is complete and verified
against the listing.
