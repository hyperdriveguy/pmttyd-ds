# `calc3D` — positional attenuation and pan

**Address:** `0x800dc6d4`
**Size:** 1268 bytes (317 instructions)
**Namespace:** `pmario_sound.o`
**Evidence class:** B (inspected US bytes/control flow + **ISO-read named constants**)

## Overview

`calc3D` is the spatial model behind positional SFX. It resolves the
source position (optionally relative to the listener), computes a
distance, and produces a three-byte **`PAN`** record: attenuation,
pan and volume.

Unlike the surrounding contracts, the numeric constants here are **not**
guesses: every `lfd` resolves to a **named** symbol, and all values were
read from the read-only ISO.

## ABI

| Register | Role | Saved to |
|----------|------|----------|
| `r3` | **flags** (bits 7 and 6 select position handling) | tested in place |
| `r4` | `pssfxlist` row pointer — reads `+0x00`, `+0x04`, `+0x08` | — |
| `r5` | out0 | `r28` |
| `r6` | out1 | `r29` |

Working registers: `r27` = `0x803df188` (audio global), `r10` = `0x802CC740`,
`r31` = `0x802DC740` (base for the `lfd` constants). Stack frame: 128 bytes.

Return: the **32-bit word** of the `PAN` record.

## Source position resolution (`0x800dc6f4`..`0x800dc77c`)

Three words are copied from the row into `stack[12/16/20]`:

```asm
800dc6f4: lwz     9, 0(4)             ; row[+0x00]
800dc6fc: lwz     8, 4(4)             ; row[+0x04]
800dc704: lwz     7, 8(4)             ; row[+0x08]
```

Then two independent flag tests on `r3`:

```asm
800dc700: rlwinm. 0, 3, 0, 7, 7      ; bit 7
800dc72c: bt      2, 0x800dc760       ; bit 7 CLEAR -> skip the add
800dc730: lfs     1, 12(1)
800dc734: lfs     0, 48(27)           ; global[+0x30]
800dc740: fadds   4, 1, 0
800dc74c: fadds   2, 3, 2            ; with global[+0x34]
800dc758: fadds   0, 1, 0            ; with global[+0x38]
```

**bit 7 = "add listener position"** — the row position is offset by the
listener at `global+0x30..+0x38`.

```asm
800dc760: rlwinm. 0, 3, 0, 6, 6      ; bit 6
800dc764: bt      2, 0x800dc780       ; bit 6 CLEAR -> skip the replace
800dc768: lwz     4, 48(27)           ; global[+0x30]
800dc770: lwz     0, 56(27)           ; global[+0x38]
800dc774: stw     4, 12(1)
800dc77c: stw     0, 20(1)
```

**bit 6 = "replace with listener"** — the three words are overwritten with
the listener position outright.

So the flags word controls whether the row position is absolute, relative
to the listener, or replaced by it.

## Distance (`0x800dc780`..`0x800dc7d4`)

```asm
800dc784: lfs     1, 48(27)           ; listener x
800dc78c: lfs     2, 56(27)           ; listener z
800dc790: lfs     30, 60(3)           ; f30 = global[+0x3c]
800dc794: lfs     3, 12(1)            ; source x
800dc798: lfs     4, 20(1)            ; source z
800dc79c: bl      0x800dcbc8           ; planar distance helper
800dc7a0: fsubs   2, 1, 30            ; d = distance - global[+0x3c]
800dc7a8: fabs    2, 2
800dc7b4: bf      2, 0x800dc7d4
800dc7b8: fcmpo   0, 1, 30
800dc7c4: fadds   1, 1, 0            ; or
800dc7d0: fsubs   1, 1, 0            ; ... fsubs with a constant
```

The raw distance has the scalar `global+0x3c` subtracted, then an absolute
value / comparison pair (`cror` branchless select) folds a signed
correction on one side of the listener. The helper at `0x800dcbc8` is the
planar 2D distance primitive; its own body is **not** decoded here.

## Attenuation by reciprocal square root (`0x800dc840`..`0x800dc8c0`)

The squared distance is formed, then `frsqrte` is refined by **three
Newton-Raphson iterations** using the ISO-read constants:

```asm
800dc87c: frsqrte 1, 30               ; x ~= 1/sqrt(d2)
800dc880: lfd     3, 30040(31)         ; A = 0.5
800dc884: lfd     2, 30048(31)         ; B = 3.0
800dc888: fmul    0, 1, 1              ; x*x
800dc88c: fmul    1, 3, 1              ; A * x
800dc890: fnmsub  0, 30, 0, 2          ; B - d2 * (x*x)
800dc894: fmul    1, 1, 0              ; x = A*x * (B - d2*x*x)
```
repeated three times, then:
```asm
800dc8b8: fmul    30, 30, 0
800dc8bc: frsp    30, 30
```

The recurrence is the standard `x ← 0.5·x·(3 − d²·x²)`, and the final
`frsp` narrows the double-precision result to single. Preserving the three
iterations and the `frsp` is required for bit-exact attenuation.

## The zero-distance and negative cases (`0x800dc874`..`0x800dc8dc`)

```asm
800dc874: fcmpo   0, 30, 0
800dc878: bf      1, 0x800dc8c4        ; d2 > 0 -> normal path
800dc8c4: lfd     0, 30056(31)         ; C = 0.0
800dc8c8: fcmpo   0, 30, 0
800dc8cc: bf      0, 0x800dc8dc        ; d2 == 0 -> skip
800dc8d0: lis     3, -32702
800dc8d4: lfs     30, -7224(3)         ; 0x8041e3c8 = __float_nan
```

At `d2 <= 0` the result is forced to **`__float_nan`**, the named symbol at
`0x8041E3C8` (`.sdata`, bits `7fffffff`). A zero-distance sound is
therefore deliberately poisoned rather than clamped.

## Float-classifier (`0x800dc8dc`..`0x800dc944`)

The value is round-tripped through `stfs`/`lwz` and its IEEE-754 exponent
is inspected directly:

```asm
800dc8e0: lis     0, 32640             ; 0x7F800000
800dc8e4: lwz     4, 8(1)
800dc8e8: rlwinm  3, 4, 0, 1, 8
800dc8ec: cmpw    3, 0
800dc8f0: bt      2, 0x800dc904
800dc8f4: bf      0, 0x800dc934
800dc8f8: cmpwi   3, 0
800dc8fc: bt      2, 0x800dc91c
800dc900: b       0x800dc934
800dc904: clrlwi. 0, 4, 9              ; mantissa bit 9
  -> li 0, 1  or  li 0, 2
800dc91c: clrlwi. 0, 4, 9
  -> li 0, 5  or  li 0, 3
800dc934: li      0, 4
```

Five cases (1..5) are selected by the exponent field and the sign of the
mantissa. Only case **1** acts:

```asm
800dc938: cmpwi   0, 1
800dc93c: bf      2, 0x800dc948        ; case != 1 -> skip
800dc940: lis     3, -32702
800dc944: lfs     30, -7224(3)         ; force __float_nan
```

Cases 2, 3, 4 and 5 all converge without changing `f30`; case 1 replaces it
with `__float_nan`. The classifier's exact domain (which IEEE-754 class
maps to which case) is **not** fully derived here — the exponent/mantissa
tests are recorded, the semantic labels are not.

## Pan window (`0x800dc7f0`..`0x800dc83c` and `0x800dc9d8`..`0x800dcab0`)

Two identical clamp idioms, both producing a **7-bit** result:

```asm
800dc7f4: extsh   0, 3
800dc7f8: cmpwi   0, 64
800dc7fc: bf      0, 0x800dc80c
800dc800: subfic  0, 3, 127            ; 127 - x   (lower clamp)
800dc80c: cmpwi   0, 196
800dc810: bf      1, 0x800dc820
800dc814: addi    0, 3, -196
800dc818: subfic  0, 0, 196            ; 196 - ... (upper clamp)
800dc820: addi    0, 3, -64            ; subtract the minimum distance
800dc824: extsh.  30, 0
800dc82c: li      30, 0                ; clamp low
800dc83c: li      30, 127              ; clamp high
```

The distance is clamped to the window **[64, 196]**, then 64 is subtracted,
giving **0..127**. The `64` is a minimum audible distance: anything nearer
reads as fully attenuated. The `subfic 0, 3, 127` / `subfic 0, 0, 196`
sequence is branch-free saturation, and `extsh` makes the clamp
**16-bit signed**.

The second occurrence (`0x800dc9d8`..`0x800dcab0`) clamps a value into
**1..127** (note the low clamp is 1, not 0) before it is used as a
multiplier: `mulli 3, 3, 90` scales it to a 0..90 range.

## Angle and phase (`0x800dc948`..`0x800dca98`)

```asm
800dc948: lfd     0, 30064(31)         ; D = 3.14  (pi)
800dc94c: fmul    1, 0, 31             ; angle * pi
800dc950: frsp    1, 1
800dc954: bl      0x80271ea8            ; cos()
800dc960: fmuls   1, 30, 1             ; * attenuation
800dc96c: bf      1, 0x800dc974
800dc970: fmr     2, 0                 ; take magnitude
800dc9f4: lfd     2, 29984(31)         ; E = 2^52 + 2^31
800dca00: lfs     0, -23400(2)
800dca04: addze   0, 0
800dca08: xoris   0, 0, 32768          ; negation via integer bit flip
800dca14: fsubs   1, 1, 2
800dca1c: fdivs   1, 1, 0
800dca20: bl      0x80272410            ; sin()
800dca2c: fnmsubs 0, 0, 1, 0
```

`D = 3.14` is the pi approximation (read from the ISO as
`0x40091eb851eb851f`). The angle is scaled by it, `cos` produces the lateral
component, and `sin` produces a second phase term. `E = 2^52 + 2^31`
(`0x4330000080000000`) is the classic **double-to-int rounding magic
constant**; the `addze` / `xoris` pair negates the integer bit pattern
before `lfd` reloads it as a double. This is the same `0x43300000` word
that appears as a raw `lis 0, 17200` literal elsewhere — now explained.

## Output: the `PAN` record (`0x800dcb74`..`0x800dcba0`)

```asm
800dcb74: stb     0, 7160(13)          ; 0x8041eb18  primary
800dcb7c: stb     28, 7160(13)         ; alternate primary
800dcb80: lbz     0, 7160(13)
800dcb84: cmplwi  0, 127
800dcb8c: li      0, 127
800dcb90: stb     0, 7160(13)          ; clamp primary to 127
800dcb94: addi    3, 13, 7160
800dcb98: stb     30, 1(3)             ; PAN[+1] = pan
800dcb9c: stb     5, 2(3)              ; PAN[+2] = volume
800dcba0: lwz     3, 7160(13)          ; return the word
```

The output global is the named symbol **`PAN`** (`PAN$4553`) at
`0x8041EB18`, in **`.sbss`**.

| Offset | Size | Role |
|--------|------|------|
| `+0` | 1 | primary (attenuation/distance), clamped to 127 |
| `+1` | 1 | pan |
| `+2` | 1 | volume |

```cpp
struct PAN {          // 0x8041EB18, 3 significant bytes of a word
    uint8_t primary;  // +0, clamped to 127
    uint8_t pan;      // +1
    uint8_t volume;   // +2
};                     // returned as the full 32-bit word
```

The alternate primary at `0x800dcb7c` is taken when `r29`'s bit 16 is set
(`clrlwi. 0, 29, 16`), i.e. when `r6` (out1) carries that flag.

## Named constants — all read from the ISO

| Access | Address | Symbol | Value |
|--------|---------|--------|-------|
| `lfd 30040(r31)` | `0x802E3C98` | `double_0p5_802e3c98` | **0.5** |
| `lfd 30048(r31)` | `0x802E3CA0` | (adjacent) | **3.0** |
| `lfd 30056(r31)` | `0x802E3CA8` | (adjacent) | **0.0** |
| `lfd 30064(r31)` | `0x802E3CB0` | (adjacent) | **3.14** |
| `lfd 29984(r31)` | `0x802E3C60` | `double_to_int_802e3c60` | **2^52 + 2^31** |
| `lfd 29992(r31)` | `0x802E3C68` | (adjacent) | **2^52** |
| `lfs -7224(r3)` | `0x8041E3C8` | `__float_nan` | **NaN** (`7fffffff`) |

Raw bits, read directly: `0.5` = `3fe0000000000000`, `3.14` =
`40091eb851eb851f`, `double_to_int` = `4330000080000000`, `2^52` =
`4330000000000000`.

The eight `r2`-relative `lfs` constants are now **resolved**: `r2` is
`0x8042B038` (see
[SMALL_DATA_R2_BASE_AND_FLOAT_POOLS](../binary/SMALL_DATA_R2_BASE_AND_FLOAT_POOLS.md),
where the field pool independently corroborates the base via pi/4).

| Offset | Value | Use in this function |
|--------|-------|---------------------|
| `-23412(r2)` | `0.03125` (1/32) | scale in the `srawi 0,3,6` / magic-double divide |
| `-23408(r2)` | `0.0` | loaded at `0x800DC868` |
| `-23404(r2)` | `0.0` | loaded beside it |
| `-23400(r2)` | **NaN** | the d2<=0 poison constant |
| `-23396(r2)` | `0.0174533` (**pi/180**) | degrees-to-radians scale |
| `-23392(r2)` | `0.0078125` (1/128) | 7-bit quantisation scale |
| `-23388(r2)` | `0.03125` (1/32) | scale |
| `-23384(r2)` | `0.0` | **divisor** at `0x800DC98C` — see below |
| `-23380(r2)` | `6.283185` (**2*pi**) | multiplies `f2` before the divide |
| `-23376(r2)` | `78.75` | compared against `fabs(f31)` |
| `-23372(r2)` | `0.349` (**pi/9**) | multiplied by `mulli 29,100` |

So the "3.14" seen in the `lfd` table is a separate constant from the
`2*pi` and `pi/180` in the `r2` pool; both exist.

**Flagged, unresolved:** `-23384(r2)` is `0.0` and sits in the divisor
position of `fdivs 0, 3, 2` at `0x800DC98C`, preceded by
`fcmpo 0, 1, 0` at `0x800DC988` against `-23376(r2) = 78.75`. The pool
value is not in doubt and the instruction order is unambiguous, so whether
that branch is reachable or is guarded so the division never executes was
**not** determined here.

## Porting notes

1. The `frsqrte` refinement is **three** iterations of
   `x ← 0.5·x·(3 − d²·x²)` plus a final `frsp`. Fewer iterations or a
   missing `frsp` changes every attenuation value.
2. A zero or negative squared distance yields **NaN**, deliberately. Do not
   add a guard; propagate the NaN as the original does.
3. Pan is clamped to the window **[64, 196]** then offset by 64 — the 64 is
   a minimum audible distance, not an arbitrary bias.
4. All three output fields are **bytes**, and the function returns the
   containing 32-bit word.
5. The flags word (`r3` bit 7 = add listener, bit 6 = replace with
   listener) selects how the source position is derived. Both tests are
   independent.
6. Use `3.14`, not `M_PI`. The value is literally 3.14 in the binary.

## Remaining gaps

- The helper at `0x800dcbc8` (the planar distance primitive) is called but
  not decoded.
- The exact IEEE-754 class → case 1..5 mapping in the classifier, and
  which of cases 2..5 are reachable in practice.
- What `r5`/`r6` (out0/out1) are used for by the caller; this function
  writes nothing through them and only reads `r29`'s bit 16.
- The `pssfxlist` row fields `+0x00`, `+0x04`, `+0x08` that supply the
  source position.
- `global+0x3c`'s meaning — it is subtracted from every distance.
- No original execution trace.