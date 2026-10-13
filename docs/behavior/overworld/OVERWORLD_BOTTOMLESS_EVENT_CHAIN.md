# Bottomless event chain

US G8ME01 disc0 revision0; static contracts, no execution or gameplay claim.
Closes `bottomlessBero` and `bottomlessEvt` in the [unaddressed-functions
register](../binary/UNADDRESSED_BINARY_FUNCTIONS.md), continuing the
bottomless flag/position stores in [speed/fall/landing
leaves](OVERWORLD_SPEED_FALL_LANDING_LEAVES.md) ([Mario+736] = 1,
[Mario+740/744/748] = arg triple).

## bottomlessEvt (8007ac7c, 51 lines): event entry, triple conversion

Event callback, returns 2. Resolves three event words via `evtGetValue`,
converts each signed integer to float with the 0x43300000-bias double trick
against the table double at 0x802C3058 (the same constant bero_length's leaf
uses), and tail-calls `bottomlessBero` (0x80096de4, same-TU `<.text+0x1c168>`
edge) with the three floats. No destination write, no result test.

## bottomlessBero (80096de4, 48 lines): mixed store triple

Float args f1/f2/f3 retained across the `marioGetPtr` call. Stores, by exact
register/stack traffic:

- [Mario+740] = single(f1) bits (integer load/store path).
- [Mario+744] = single(floatword([0x802C3EA8]) + f2) — the table word is
  **live** here, unlike the [speed doc](OVERWORLD_SPEED_FALL_LANDING_LEAVES.md)'s
  all-dead case: f0 survives from the `fadds` to the `stfs` because only
  integer ops intervene.
- [Mario+748] = f3 bits.

The SDA load (`lfs 0, −27036(r2)`) is dead (f0 is overwritten by the `fadds`
before any store reads it). Table base 0x802C3EA8 comes from
`lis` 0x802C0000 + 16040; contents unrecovered.

## Contrast with bottomlessPos

Same frame shape and same three destination offsets, but every table/stack
interaction differs in liveness: `bottomlessPos` overwrites each loaded slot
with `stfs` before reading (pure arg triple), while `bottomlessBero` mixes
one live table word into the middle lane. Do not merge the two bodies.

## Not established

Table contents, offset meanings, SDA values, call cadence, and any gameplay
effect.
