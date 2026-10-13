# marioSearchHead_roll

**Address:** `0x8008bc70`
**Size:** 1336 bytes
**Namespace:** `mario_hit.o`
**Evidence class:** B (inspected US bytes/control flow)

## Overview

`marioSearchHead_roll` searches for open space in front of Mario while he
is rolling. It probes five directions around the roll heading, advancing
the position with `movePos`, then runs a vector filter against the
resulting descriptor and reduces the hits to a scalar clearance value
written through an out-pointer.

## Signature

| Register | Use |
|----------|-----|
| `f1` | roll angle (saved to `r28`, later reused as a temp) |
| `r3` | in: pointer to a 2-float source position (`src[0]`, `src[1]`) |
| `r4` | in/out: pointer to the 3-float static work descriptor at `0x800c3c5c`, **and** the out-pointer for the result float |

Return: **`0` on every path** (`li 3, 0` at `0x8008c170` is the single
exit). The meaningful output is the float stored at `0(r27)` in the loop
tail, where `r27` is the saved `r4`.

## Static work buffers

```asm
8008bcb4: lis     5, -32724            ; 0x800c0000
8008bcb8: lis     4, -32724
8008bcc4: addi    30, 5, 15440         ; r30 = 0x800c3c50
8008bccc: addi    31, 4, 15452         ; r31 = 0x800c3c5c
```

Two 12-byte globals are used as scratch:
`0x800c3c50` (3 words, indexed by iteration) and `0x800c3c5c`
(3 words, also indexed by iteration). They accumulate the per-direction
probe results.

## Direction dispatch (5 iterations)

Loop head `0x8008bcdc`, back-edge `0x8008c16c`:

```asm
8008c164: addi    28, 28, 1
8008c168: cmpwi   28, 5
8008c16c: bt      0, 0x8008bcdc
```

Inside the loop the iteration index `r28` selects the probe vector through
a comparison chain:

```asm
8008bcdc: cmpwi   28, 2
8008bce0: bt      2, 0x8008bd80         ; idx == 2 -> dedicated case
8008bce4: bf      0, 0x8008bcf8         ; idx  >  2
8008bce8: cmpwi   28, 0
8008bcec: bt      2, 0x8008bd08         ; idx == 0 -> zero vector
8008bcf0: bf      0, 0x8008bd18         ; idx  >  0
8008bcf4: b       0x8008bec4            ; idx  == 1 -> generic
8008bcf8: cmpwi   28, 4
8008bcfc: bt      2, 0x8008be58         ; idx == 4 -> dedicated case
8008bd00: bf      0, 0x8008bec4         ; idx  >  4 -> generic
8008bd04: b       0x8008bde8            ; idx  == 3 -> dedicated case
```

So there are **dedicated cases for indices 0, 2, 3 and 4**; index 1 and
the out-of-range index 5 fall through to the shared path at `0x8008bec4`.

| Index | Behaviour |
|-------|-----------|
| 0 | `stack[12] = 0.0`, `stack[8] = 0.0` — no displacement |
| 1 | generic path |
| 2 | dedicated (`0x8008bd80`) |
| 3 | dedicated (`0x8008bde8`) |
| 4 | dedicated (`0x8008be58`) |
| 5 | generic path (unreachable while the bound is 5) |

## Displacement case (index 1 shape), `0x8008bd18`

```asm
8008bd18: lfs     1, 0x800494e4
8008bd1c: lfs     0, 420(29)           ; mario[+0x1a4]
8008bd20: lfs     2, 0x800494a8
8008bd24: fadds   1, 1, 0
8008bd28: lfs     0, 0x80049494
8008bd2c: fmuls   1, 2, 1
8008bd30: fdivs   28, 1, 0            ; f28 = (c1 + mario[0x1a4]) * c2 / c3
8008bd34: fmr     1, 28
8008bd38: bl      0x80272410           ; sin
8008bd3c: frsp    2, 1
8008bd40: lfs     0, 0x8004950c
8008bd48: fmuls   0, 0, 2             ; cos(f28) * c
8008bd4c: stfs    0, 12(1)
8008bd50: bl      0x80271ea8           ; cos
8008bd54: frsp    0, 1
8008bd5c: lfs     1, 0x800494b8
8008bd68: fneg    0, 0
8008bd6c: fmuls   0, 2, 0             ; -sin(f28) * c
8008bd70: stfs    0, 8(1)
8008bd74: lfs     2, 420(29)           ; f2 = mario[+0x1a4]
8008bd78: bl      0x80005de4           ; movePos
```

Each probe builds a 2D displacement (`stack+8`, `stack+12`) as
`c * (sin(t), -cos(t))` with `frsp` rounding to single precision between
the trig call and the scale, then calls `movePos` with
`f3 = mario[+0x1a4]`. `sin` and `cos` are the MSL `s_sin.c` / `s_cos.c`
implementations at `0x80272410` / `0x80271ea8`.

**Porting note:** the `frsp` instructions at `0x8008bd3c` and
`0x8008bd54` are deliberate single-precision roundings. Reproducing them
is required for bit-exact probe vectors.

## Descriptor assembly (`0x8008becc`..`0x8008bf44`)

After the loop the two static scratch buffers are folded into a
stack filter descriptor: three words from `0x800c3c5c` go to
`stack+0x58..0x60`, three words from `0x800c3c50` to `stack+0x4c..0x54`
and again to `stack+0x10..0x18`, and two floats
(`stack[8] + stack[12]`) are stored into `stack+0x1c` / `stack+0x20`.

## Label selection and filter run (`0x8008bf48`)

```asm
8008bf48: bl      0x8005e0c0           ; marioGetPtr
8008bf4c: lhz     0, 46(3)             ; mario[+0x2e]
8008bf50: cmplwi  0, 28
8008bf54: bf      2, 0x8008bf6c
  ; == 28 -> label string at 0x80091538
8008bf6c: bl      0x8005e0c0
8008bf70: lwz     0, 0(3)              ; mario[+0]
8008bf74: rlwinm. 0, 0, 0, 7, 7        ; bit 7
  ; bit7 set  -> label string at 0x80091504
  ; bit7 clear-> label string at 0x800915e4
8008bfa0: cmplwi  3, 0
8008bfa4: bf      2, 0x8008bfb0
8008bfa8: li      3, 0
8008bfac: b       0x8008c0fc
```

Three debug label strings are copied into `stack+0x40` by
`hitCheckVecFilter`, selected by `mario[+0x2e] == 28` and bit 7 of
`mario[+0]` — the same two-way partition used by the slit and roll wall
probes.

## Clearance reduction (`0x8008bfb0`..`0x8008c160`)

```asm
8008bfc0: lfs     1, 52(1)
8008bfc4: lfs     0, 0x8004946c
8008bfcc: fcmpo   0, 1, 0
8008bfd4: cror    2, 1, 2              ; branchless select
8008bfdc: lfs     0, 0x8004948c        ;   value A
8008bfe4: lfs     0, 0x80049490        ;   value B
8008bfe8: lfs     3, 0x80049468
8008bfec: lis     0, 17200             ; 0x800C4330
8008bff0: stw     0, 136(1)
8008bff8: fmadds  0, 3, 1, 0           ; c68 + hitDistance
8008bffc: lfs     4, 56(1)
8008c000: lfd     1, 0x800c3d68        ; 8-byte constant
8008c004: fctiwz  2, 0                 ; truncate to integer
8008c00c: fcmpo   0, 4, 0
8008c018: xoris   0, 0, 32768          ; sign-bit flip
8008c024: fsubs   0, 0, 1
8008c028: fdivs   0, 0, 3             ; / constant
```

The `cror 2, 1, 2` is a **branchless select** between the constants at
`0x8004948c` and `0x80049490`, chosen by whether `stack[52]` compares
equal to `0x8004946c`. The reduction then computes
`(c68 + hitDistance)` truncated with `fctiwz`, and a scaled residual
`(f0 - c) / c3`. The `xoris 0, 0, 32768` at `0x8008c018` is the standard
PPC idiom for **negating a float via its integer representation**.

Final tail:

```asm
8008c158: fcmpo   0, 2, 0
8008c15c: bf      1, 0x8008c164        ; if truncated == 0 -> skip store
8008c160: stfs    0, 0(27)             ; *r4 = clearance
```

The result is written **only when the truncated value is nonzero**.

## Called functions

| Address | Symbol | Count |
|---------|--------|-------|
| `0x8005e0c0` | `marioGetPtr` | 4 |
| `0x80005de4` | `movePos` | 5 (once per loop iteration) |
| `0x80272410` | `sin` (MSL `s_sin.c`) | 5 |
| `0x80271ea8` | `cos` (MSL `s_cos.c`) | 5 |
| `0x800158a4` | `hitCheckVecFilter` | 3 (label selection + front clearance) |

## Porting notes

1. The loop bound is the literal `cmpwi 28, 5`; the dispatch chain only
   has dedicated arms for 0, 2, 3 and 4, so index 1 and index 5 share the
   generic path. Preserving the chain shape matters.
2. The out-pointer write is conditional on the truncated value being
   nonzero — an unconditional store will differ.
3. The result is returned in `r3 = 0`; callers must read the out-pointer.
4. The two static buffers at `0x800c3c50` / `0x800c3c5c` are scratch, not
   save state; they are written unconditionally each call.

## Remaining gaps

- Values of the `lfs` constants at `0x80049494`, `0x800494a8`,
  `0x800494b8`, `0x800494e4`, `0x8004950c` (probe radius, angle scale,
  sin/cos multipliers) are not extracted.
- Semantics of dedicated directions 2, 3 and 4 are only known from their
  placement in the dispatch chain, not decoded.
- The 8-byte double constant at `0x800c3d68` and the word constant at
  `0x800c4330` are not resolved.
- The three label strings at `0x80091538` / `0x80091504` / `0x800915e4`
  are not read from the ISO.
- Callers of `marioSearchHead_roll` are not audited.