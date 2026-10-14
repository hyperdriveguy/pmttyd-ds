# Small-data base register `r2` and the shared float constant pools

**Applies to:** the whole US G8ME01 binary
**Evidence class:** B — `r2` **verified**, constants **ISO-read**

## `r2` = `0x8042B038`

Many functions load `float`/`double` constants with `r2`-relative
addressing (`lfs N, -23412(r2)`). Until now this project recorded those
operands only as offsets, because the base was unknown and several
documents carried invented absolute addresses.

**The base is `0x8042B038`, verified rather than guessed.**

### How it was verified

The constant pools are plain IEEE-754 runs in the loaded image. Given a
candidate base `R`, the values at `R + offset` must all be sensible
numeric literals. Two independent semantic anchors pin `R` exactly:

1. **Field.** `marioSlitChkWallAround` sweeps **8 evenly spaced
   directions** around Mario and documents its angle increment as the
   `lfs` at `-27524(r2)`. At `R = 0x8042B038` that word is
   **`0.7853981853` = π/4**. Eight steps of 45° sum to exactly 360°, which
   is what the loop provably does. The neighbouring words are 16, 40, 24,
   12, 20, 25, 2, 1/32, 0.01, -0.01, 1.4, 1.2, 10, 30, 270, 90, 60, -60,
   100, -100, 35, 5, -0.3, -0.7, -1.5, 300, 698, 185, 80, 15 — the
   degree/probe vocabulary of field collision.
2. **Audio.** The `pmario_sound.o` pool at the same base yields
   π/180 (`0.0174533`), 1/128, 1/32, 2π (`6.283185`), π/9 (`0.349`),
   π/30 (`0.1047`), 360, 255, 30, -30, 6, 8, 78.75, 1.0, 0.01, 0.1 — a
   trigonometry and audio-scale pool.

A candidate base 4 bytes higher puts 2.0 where π/4 must be and fails the
field anchor, so the alignment is unambiguous.

**`r2` is shared across object files.** The field (`mario_hit.o`) and
audio (`pmario_sound.o`) pools are two windows into one contiguous
constant region at `0x8042449c`..`0x8042550c`, not per-file bases.

### How it was read

The DOL is opened read-only, its section table is parsed, and each
constant is read as a 4-byte IEEE-754 single (or 8-byte double) at the
mapped file offset. No bytes were copied into the repository.

## mario_hit.o field pool (`0x8042449c`..`0x80424548`)

| Offset | Address | Value |
|--------|---------|-------|
| `-27548(r2)` | `0x8042449c` | 16 |
| `-27544(r2)` | `0x804244a0` | 40 |
| `-27540(r2)` | `0x804244a4` | 24 |
| `-27536(r2)` | `0x804244a8` | 12 |
| `-27532(r2)` | `0x804244ac` | 20 |
| `-27528(r2)` | `0x804244b0` | 25 |
| `-27524(r2)` | `0x804244b4` | 0.7853981853 |
| `-27520(r2)` | `0x804244b8` | 2 |
| `-27516(r2)` | `0x804244bc` | 0.03125 |
| `-27512(r2)` | `0x804244c0` | 0.009999999776 |
| `-27508(r2)` | `0x804244c4` | -0.009999999776 |
| `-27504(r2)` | `0x804244c8` | 1.399999976 |
| `-27500(r2)` | `0x804244cc` | 1.200000048 |
| `-27496(r2)` | `0x804244d0` | 10 |
| `-27492(r2)` | `0x804244d4` | 30 |
| `-27488(r2)` | `0x804244d8` | 14940.59863 |
| `-27484(r2)` | `0x804244dc` | 0 |
| `-27480(r2)` | `0x804244e0` | 270 |
| `-27476(r2)` | `0x804244e4` | 90 |
| `-27472(r2)` | `0x804244e8` | 0 |
| `-27468(r2)` | `0x804244ec` | 6 |
| `-27464(r2)` | `0x804244f0` | -60 |
| `-27460(r2)` | `0x804244f4` | 60 |
| `-27456(r2)` | `0x804244f8` | 1 |
| `-27452(r2)` | `0x804244fc` | -1 |
| `-27448(r2)` | `0x80424500` | -100 |
| `-27444(r2)` | `0x80424504` | 0.1000000015 |
| `-27440(r2)` | `0x80424508` | 0.05000000075 |
| `-27436(r2)` | `0x8042450c` | 35 |
| `-27432(r2)` | `0x80424510` | 5 |
| `-27428(r2)` | `0x80424514` | -0.3000000119 |
| `-27424(r2)` | `0x80424518` | 100 |
| `-27420(r2)` | `0x8042451c` | 25 |
| `-27416(r2)` | `0x80424520` | 110 |
| `-27412(r2)` | `0x80424524` | 10 |
| `-27408(r2)` | `0x80424528` | -0.6999999881 |
| `-27404(r2)` | `0x8042452c` | -1.5 |
| `-27400(r2)` | `0x80424530` | 300 |
| `-27396(r2)` | `0x80424534` | 698 |
| `-27392(r2)` | `0x80424538` | 185 |
| `-27388(r2)` | `0x8042453c` | 20 |
| `-27384(r2)` | `0x80424540` | 40 |
| `-27380(r2)` | `0x80424544` | 80 |
| `-27376(r2)` | `0x80424548` | 15 |
| `-27372(r2)` | `0x8042454c` | 0.002499999944 |

## pmario_sound.o audio pool (`0x804254bc`..`0x8042550c`)

| Offset | Address | Value |
|--------|---------|-------|
| `-23420(r2)` | `0x804254bc` | 0.1000000015 |
| `-23416(r2)` | `0x804254c0` | 255 |
| `-23412(r2)` | `0x804254c4` | 0.03125 |
| `-23408(r2)` | `0x804254c8` | 0 |
| `-23404(r2)` | `0x804254cc` | 0 |
| `-23400(r2)` | `0x804254d0` | nan |
| `-23396(r2)` | `0x804254d4` | 0.01745329238 |
| `-23392(r2)` | `0x804254d8` | 0.0078125 |
| `-23388(r2)` | `0x804254dc` | 0.03125 |
| `-23384(r2)` | `0x804254e0` | 0 |
| `-23380(r2)` | `0x804254e4` | 6.283185482 |
| `-23376(r2)` | `0x804254e8` | 78.75 |
| `-23372(r2)` | `0x804254ec` | 0.3490658402 |
| `-23368(r2)` | `0x804254f0` | 6 |
| `-23364(r2)` | `0x804254f4` | 360 |
| `-23360(r2)` | `0x804254f8` | -30 |
| `-23356(r2)` | `0x804254fc` | 8 |
| `-23352(r2)` | `0x80425500` | 30 |
| `-23348(r2)` | `0x80425504` | 0.104719758 |
| `-23344(r2)` | `0x80425508` | 1 |
| `-23340(r2)` | `0x8042550c` | 0.009999999776 |
| `-23336(r2)` | `0x80425510` | 1.151500263e-26 |

## Named constants already resolved by symbol

| Offset | Address | Symbol | Value |
|--------|---------|--------|-------|
| (none in these pools) | `0x802E3C98` | `double_0p5_802e3c98` | 0.5 |
| | `0x802E3CA0` | — | 3.0 |
| | `0x802E3CA8` | — | 0.0 |
| | `0x802E3CB0` | — | 3.14 |
| | `0x802E3C60` | `double_to_int_802e3c60` | 2^52 + 2^31 |
| | `0x802E3C68` | — | 2^52 |
| | `0x8041E3C8` | `__float_nan` | NaN (`7fffffff`) |

## Consequences for existing contracts

- Every `N(r2)` operand previously marked "value not extracted" in
  `docs/behavior/field/` and `docs/behavior/audio/` can now be read off
  the two tables above.
- The fabricated `0x800494xx` addresses published earlier in this project
  are superseded; they were this pool at the wrong base.
- `lis 0, 17200` = `0x43300000` is the **high half** of the
  `double_to_int` constant, as recorded in
  [positional attenuation and pan](../audio/POSITIONAL_SFX_ATTENUATION_AND_PAN.md).

## Open item flagged, not resolved

In `calc3D`, `-23384(r2)` is **0.0** and is loaded into the divisor
position at `0x800DC98C` (`fdivs 0, 3, 2`). The neighbouring word is 2π,
and the instruction ordering is unambiguous, so the pool value is not in
doubt. Whether that branch is genuinely reachable, or is guarded by the
`fcmpo` at `0x800DC988` in a way that prevents the division, was **not**
determined here. Flagged rather than resolved.

## Remaining gaps

- Constants outside the two ranges tabulated above; the region either side
  was not enumerated.
- Whether the pool is contiguous beyond these windows.
- No original execution trace; the pools are read from static bytes only.
