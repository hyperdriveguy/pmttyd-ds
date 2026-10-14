# Pouch item removal (table select, scan, compaction)

US G8ME01 disc0 revision0; static contract, no execution or gameplay claim.
Extends [pouch acquisition](POUCH_GET_ITEM_DISPATCH.md) with the removal
half for ITEM-01 todo 1. The US body matches the pinned snapshot
`pouchRemoveItem` (`src/mario/mario_pouch.c:651`) branch for branch; member
names below are the snapshot's, with the JP/US caveat.

Fingerprint: entry 800d4c38, `.elf`/173-line `.disasm`, no `bl` (leaf).
Pouch pointer is [r13+7136] throughout, same single record as acquisition.

## Table selection (verified offsets)

| Type range | table1 | table2 | maximum | valid |
| --- | --- | --- | --- | --- |
| 121–235 (held) | pouch+402 (+0x192 `heldItems`) | none (r30 = 0) | 10, or 20 when the 0x800d4ed4 check call returns nonzero | 1 |
| 1–120 (key) | pouch+160 (+0xA0 `keyItems`) | none | 121 | 1 |
| 240–338 (badge) | pouch+506 (+0x1FA `badges`) | pouch+906 (+0x38A `equippedBadges`) | 200 | 1 |
| else (0, 236–239, 339+) | — | — | — | 0 → return FALSE (0) |

The 10/20 held capacity re-derives the strange-sack rule independently of
the acquisition body. Badge tables are the only dual-table case.

## Scan and compaction

Linear scan for the first slot equal to the type. On a hit: clear the slot
(and the twin slot when table2 exists), then memmove-shift the tail down by
one halfword with `bdnz`-bounded copy loops (single- and dual-table
variants are unrolled separately in-body), and return TRUE (1). Miss (loop
exhausts the count) returns FALSE without writing. The scan compares with
`cmpw` after an `lha` load; the found-index arithmetic feeds both the clear
and the shift, so exactly one entry is removed per call.

## Not established

Item-effect/consumption rules, save mapping, shop/chest callers, table
lifetimes, and any gameplay effect. The 0x800d4ed4 check call's own
contract is shared with the acquisition body and stays open.
