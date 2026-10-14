# Pouch item acquisition dispatcher

US G8ME01 disc0 revision0; static contract, no execution or gameplay claim.
Answers ITEM-01 todo 1's slot/count/acquisition core
(`docs/requests/ITEM-01.md`): `pouchGetItem` is the central item-receive
entry, and its array bases, capacities, and routing verify the snapshot
`PouchData` layout below.

Fingerprint: entry 800d50cc, `.elf`/303-line `.disasm`, one `bl` (badge
subroutine 0x800858fc, positional). Pouch pointer throughout is
[r13+7136] (0x1BE0, the shared pointer of the power-callback docs), so
field and battle sides operate on one record — no separate battle pouch
exists at this layer.

## Layout cross-check (snapshot `mario_pouch.h`, JP/US caveat)

- `heldItems[20]` at +0x192: body uses base pouch+402 (= 0x192) with
  2-byte stepping (`lha`/`sth` pairs, ±2 memmove strides). ✓
- `keyItems[121]` at +0xA0: body uses base pouch+160 (= 0xA0) with count
  121. ✓
- Capacity: `pouchCheckItem`-style call (0x800d4ed4) selects count 10
  versus 20 — the header's base-10 / strange-sack-20 rule, recovered from
  the binary rather than assumed. ✓
- Coins cap 999 at +120; the 999 compare/store is in-body.

## Type-id routing (verified branch map)

- 121 → coins (+1 if < 999), return 1.
- 122 → capped counter at a global+8 (increment unless already ≥ 99999),
  identity open; return 1.
- 123 → +112-gated path with a ×14-strided member scan and an unresolved
  call (0x801410e8); return 1.
- 124 → +116/+118 path, return 1. 125 → +154 path. 87 → +156 path.
- 6, 7 / 9, 10, 11, 12 → byte stores at +152/+153 by small decision
  graphs; 8 falls through similarly. 89 → bare accept (return 1, no-op).
- 240–338 → badge subroutine 0x800858fc, then the shared tail.
- 1–120 → key-item insert scan (121 slots at +0xA0). 0 and 339+ take
  separate miss/empty paths. Return 1 on store, 0 on miss
  (`li 3,0` exit after the scan loop exhausts via `bdnz`).

Insertions shift existing halfwords with `bdnz`-bounded memmove loops
(exact bounds per loop in the listing); the compacted region base and the
subroutine's role stay open, as do per-item effect/consumption rules
(ITEM-01 todos 2–4), save-block mapping (SAVE-01), and acquisition sites.

## Not established

Subroutine 0x800858fc internals, memmove-loop bound ownership, effect and
consumption semantics, save format, shop/chest/pickup callers, and any
gameplay effect.
