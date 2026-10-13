# Hit damage-return sets

US G8ME01 disc0 revision0; static contracts, no execution or gameplay claim.
Closes `hit_damage_return_set`, `hitGrpDamageReturnSet`, and
`hitDamageReturnSet` in the [unaddressed-functions
register](../binary/UNADDRESSED_BINARY_FUNCTIONS.md).

Callee identities from `us-symbols`/`us-usage.json` rows: `memcpy` 0x8000519c
(Runtime, [used elsewhere](../display/DISPLAY_SORT_IMPLEMENTATION.md)),
`_mapAlloc` 0x8002ff58 ([heap allocator](../battle/BATTLE_ALLOC.md)),
`mapGetWork` 0x8002b438 ([map work getter](../effect/EFFECT_POOL_ACQUISITION_AND_MAP_PUBLICATION.md)),
`hitObjGetPos` 0x8001449c (hitdrv.o, 336 bytes), `strcmp` 0x8026d464 (MSL).
The three bodies call each other in a chain: the event callback reaches
`hitGrpDamageReturnSet`, which reaches `hitDamageReturnSet`.

## hit_damage_return_set (8007458c, 60 lines): event entry, copy, per-record sweep

Event callback (entry block at [entry+24], `evtGetValue`, return 2).
`us-symbols` has no row for the lowercase spelling; harbor-opening and
lecture JSONs name it without a contract.

1. If incoming r4 == 0, return 2 with no further work (the resolved operand
   is still computed first via `evtGetValue([block+0])`).
2. Scan 16-byte records from the resolved word for the first null head word;
   r3 counts from 1 (so an immediately-null list yields count 1 and size 16).
3. `_mapAlloc([r13+6376], count*16)`, publish the pointer at [r13+6736],
   `memcpy` the records over. Both globals cited positionally.
4. Walk the copy with stride 16 while [rec+0] != 0: if float [rec+8] equals
   the SDA constant (r2−28784, `fcmpu`, value not recovered), call
   `hitObjGetPos(rec+4)` then `hitGrpDamageReturnSet([rec+0], rec)`.
5. Return 2.

The float equality gate and the SDA constant's value are the open items;
the copy pointer's consumers are not traced here.

## hitGrpDamageReturnSet (80013efc, 56 lines): name search, single dispatch

`us-usage.json`: hitdrv.o, 200 bytes, 1 link site. Args positional
(r3 = key string, r4 = context). Order:

1. `mapGetWork()` (ignores both args — it reads globals) → work pointer.
2. If key is null (`cmplwi r25,0` taken-equal), skip straight to the epilogue
   with r3 untouched since the `mapGetWork` call — so the null-key return is
   that call's residue, not a clean 0. Only the match path constructs a
   return deliberately.
3. Outer records stride 376, count at [work+0]; inner records stride 228,
   count at [outer+344], array at [outer+348].
4. Inner match: halfword [rec+0] has bit 0x100 set (`rlwinm.` mask 24..24)
   AND `strcmp(name-at-[[rec+8]], key)` == 0.
5. First match calls `hitDamageReturnSet(record, context, 0)` and returns its
   value. Exhaustion returns a stale register (last `strcmp`/`mapGetWork`
   residue) — no clean 0/−1 miss code exists on that path.

## hitDamageReturnSet (80013fc4, 217 lines): marking tree walk

`us-usage.json`: hitdrv.o, 844 bytes, 17 link sites. Recursive over a binary
tree with children at [node+216]/[node+220]; every visited node gets
[node+4] |= 0x40000000 (`oris` 16384) and [node+176] = incoming r4. Children
are invoked as (child, r4-unchanged, 1). The three-level unrolling is manual
inlining — recursion with r5=1 continues past it with identical marking.

- Entry with r5 == 0 (the `hitGrpDamageReturnSet` call shape) walks the whole
  tree **except** the entry node's +220 subtree; the phase-2 block
  (80014178–800142fc, same shape) covers that subtree only when r5 != 0.
- Null children are skipped via `cmplwi` guards at each level; no depth
  counter or cycle guard exists in the body.
- No return value is constructed: the epilogue falls through with stale r3,
  and the sole caller ignores it.

The meanings of the 0x40000000 bit, the +176 slot, and the +216/+220 links
are not established here.

## Not established

Record-list provenance, name-table ownership, SDA constant values, tree
lifetime/ownership, and any damage-world effect. Counts (17 link sites, lecture
names) are not caller censuses.
