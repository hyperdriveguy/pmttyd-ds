# Pouch coin transaction leaves

US G8ME01 disc0 revision0; static contract, no execution or gameplay claim.
Answers SERVICE-01 todo 2's coin half (`docs/requests/SERVICE-01.md`): the
coin field, deduction/addition order, the insufficient path, and pouch
writes. Shop/inn flows and story gating stay open.

Coin field: pouch+120 (+0x78 `coins`, s16). All three bodies reload the
pouch pointer per phase rather than retaining it.

## pouchAddCoin(delta) / pouchSetCoin(value)

Entries 800d3e18 / 800d3dd4 (23/21 lines, `.elf` pairs present).
`us-usage.json`: mario_pouch.o, 76/68 bytes, fingerprinted. Add stores
[120]+delta; Set stores the raw value; then both run the identical clamp:
below 0 → 0, above 999 → 999 (`bf`-not-taken fallthrough stores — read
branch-if-false carefully). Both return the clamped value in r3. No
overflow guard beyond the clamps is needed at halfword scale.

## coin_chk (newly extracted affordability gate)

Entry 800f1710, 29 lines; `coin_chk.elf`/`.disasm` extracted read-only for
this contract (us-symbols: evt_kinopio.o, 116 bytes). Event callback,
returns 2:

1. `pouchGetCoin()` (0x800d3e64, no-arg leaf) → coin count.
2. Price = word at [table+16] where table = word at [global 0x804165E8]
   ([r13−26936] dereferenced once). Both globals positional.
3. Signed compare (`cmpw`): coins ≥ price → `evtSetValue(entry,
   raw arg0, 1)`; else `evtSetValue(entry, raw arg0, 0)`.

The destination is the raw first event word (no routing on it — the value
1/0 passes through `evtSetValue` routing, so a literal-band destination
stores nothing). No coins move here; deduction (if any) belongs to the
purchase path, which has no reference body — the insufficient path is this
0-write, not a reverted transaction.

## Not established

Price-table ownership/contents, purchase/deduction callers, shop/inn flows,
save mapping, and any gameplay effect.
