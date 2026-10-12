# Random numbers and deterministic replay

US G8ME01 revision 0; 2026-10-12. B = inspected original binary;
P = derived validation/implementation requirement. Identity and provenance are
in [ORIGINAL_BEHAVIOR.md](../ORIGINAL_BEHAVIOR.md).

## Verified integer generator (B)

The US `rand` routine is at `0x8026c324`, length `0x20`. Its eight PowerPC
instructions read a 32-bit state from `r13+0x14a0`, multiply by `0x41c64e6d`,
add `0x3039`, retain the low 32 bits and store the new state. The return value
is bits 16..30 of the new state:

```text
state = (state * 1103515245 + 12345) modulo 2^32
return (state >> 16) & 32767
```

The multiplier is assembled by instructions at `0x8026c324/0x8026c32c`;
the addend is at `0x8026c334`; the final rotate/mask is at `0x8026c33c`.
Given the established US `r13=0x8041cf20`, storage is `0x8041e3c0`. The
returned range is 0..32767, while the state retains all 32 bits. This is not
a claim about the initial seed, entropy source, or all game randomness.

Avoid relying on the host C library's `rand()`: its algorithm and state ownership
need not match. Unsigned 32-bit arithmetic is an explicit way to reproduce
the original wrap without host signed-overflow behavior.

## Bounded integer helper (B)

`irand`, `0x80005da0`, length `0x44`, takes a signed integer bound:

- Bound 0 returns 0 immediately, **without calling rand or changing its state**.
- Otherwise it calls rand once, then uses signed division/multiplication to
  subtract the quotient times the bound from the result.

For positive bounds this is `randResult % bound`, with no rejection sampling
and no second random draw. The output range is 0..bound-1 when bound ≤ 32768;
when bound is larger, some outputs are unreachable because the generator only
returns 15 bits. Modulo reduction can be biased when the bound does not divide
32768. Replacing it with a more uniform distribution would change behavior.

The original instructions do not reject a negative bound. Its intended callers
and valid domain still need inventory; do not add gameplay meaning to negative
bounds on the strength of the helper alone. For reproducibility, count actual
generator invocations, not calls to irand: zero-bound calls consume no draw.

## Derived reference vectors (P, not observed game execution)

Starting with explicit state 1, the formula above yields:

| Call | New state | Return |
| --- | --- | --- |
| 1 | `0x41c67ea6` | 16838 |
| 2 | `0x967eb0e7` | 5758 |
| 3 | `0x2781e494` | 10113 |
| 4 | `0xc46b9b3d` | 17515 |
| 5 | `0xf94bdf32` | 31051 |

These vectors were calculated from the recovered recurrence, not captured from
an original battle. With the same starting state, irand(10) predicts 8 and
advances once. irand(0) predicts 0 and leaves the state at 1, so a following
irand(10) still predicts 8.

## What remains to establish full-game randomness

Original message rendering has confirmed shared-RNG consumers: each admitted
dynamic+shake or standalone shake glyph calls irand(10000) twice, X then Y,
while history/skipped glyphs bypass those draws. See [glyph effects](MESSAGE_GLYPH_EFFECTS.md).
Rendering frequency is therefore an original observable dependency for these
paths; isolating visual randomness into a new independent generator would change
the established behavior. Actual draw cadence and other renderer consumers
remain to be recovered before a full replay policy can be specified.

This arithmetic is sufficient to implement these two routines, but not to
reproduce the game's complete random behavior. Still required:

1. Trace seeding, reseeding and any direct writers of `0x8041e3c0`, including
   new game, title/demo, loaded saves and emulator initialization.
2. Recover `randf` (`0x80137a10`) and other random helpers; determine whether
   they share this state or have independent state and how many draws they use.
3. Inventory call sites in battle, audience, drops, encounters, NPC/effects,
   shops, minigames and scripts. Preserve conditional draw order and avoid
   consuming extra draws for a new render frame or optional visual effect.
4. Establish whether and how original save records preserve random state.
   Do not serialize an assumed seed field merely because a replay benefits
   from having one.
5. Pair matched original/native traces with seed-before, call site, arguments,
   result, seed-after and simulation tick. Distinguish a changed draw order
   from a changed RNG formula when results diverge.

`tools/verify_behavior_contracts.py` checks eight relevant instruction anchors.
The function fingerprint report includes rand and irand; neither tool executes
the original generator or proves its consumers are documented.
