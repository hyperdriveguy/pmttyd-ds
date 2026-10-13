# marioChkWallAround

**Address:** `0x800888d0`
**Size:** 2460 bytes
**Namespace:** `mario_hit.o`
**Evidence class:** B (inspected US bytes/control flow) — **structure-level, not a complete decompilation**

## Overview

`marioChkWallAround` is the final slip correction applied after the wall
probes. Every wall routine that reaches a settled wall calls it to nudge
the planar descriptor off the surface so Mario does not stick. It is the
most heavily vectorised function in the `mario_hit.o` family: it uses
`PSVECAdd`, `PSVECScale`, `PSVECDotProduct` and `PSVECMag` from the SDK.

This document covers the entry gate, the divisor setup and the vector-math
shape. The four probe groups and their result reduction are **not** fully
decoded.

## Signature

| Register | Use | Saved to |
|----------|-----|----------|
| `f2` | angle | `f24` |
| `f3` | scale / length | `f25` |
| `f4` | height / plane | `f26` |
| `r3` | in/out: 3-word planar descriptor | `r22` |
| `r4` | out: corrected descriptor (may be `NULL`) | `r21` |

Stack frame: 608 bytes. Static work descriptor: `0x800c3b78`
(shared with [`searchFrontWall`](SEARCH_FRONT_WALL.md) and
[`marioSlitChkWallAround`](MARIO_SLIT_CHK_WALL_AROUND.md)).

## Koopa view-type gate (`0x8008893c`..`0x8008895c`)

```asm
8008893c: li      27, 8                 ; default
80088944: bl      0x801feeb8           ; kpaGetStageViewType
80088948: cmpwi   3, 0
8008894c: bf      2, 0x80088954
80088950: li      27, 2                 ; override when nonzero
80088954: xoris   3, 27, 32768          ; -(8 or 2)
80088958: lis     0, 17200
8008895c: stw     3, 404(1)            ; stack[0x194] = -8 or -2
```

The probe count is **8** by default and **2** while a Koopa Paratroopa is
active (`kpaGetStageViewType() != 0`). `xoris 3, 27, 32768` negates the
value in place; the stored integer is `-8` or `-2` and later feeds the
angle divisor. This is the same signed-by-bit-flip idiom used in the field
collision code.

## Divisor setup (`0x80088960`..`0x800889a4`)

```asm
80088960: fmr     30, 24                ; r30 (GPR) reused below
80088964: lwz     5, 4(22)              ; desc[1]
80088968: li      29, 0
8008896c: stw     0, 400(1)             ; stack[0x190] = 0.0 (double lo)
80088970: lfd     1, 496(31)            ; work[+0x1f0]  (double)
80088974: lfd     0, 400(1)            ; work[+0x190]  (double)
80088978: lfs     2, 0x80049488
8008897c: fsubs   1, 0, 1              ; work[0x190] - work[0x1f0]
80088980: lwz     30, 0(22)             ; desc[0]
80088984: lwz     28, 8(22)             ; desc[2]
80088988: lfs     0, 0x80049484
8008898c: fdivs   27, 2, 1             ; r27 = c494_88 / (d0 - d1)
```

The divisor is `c494_88 / (workDouble[0x190] - workDouble[0x1f0])`. If the
two doubles are equal this divides by zero — the function has no guard on
that field, which is worth preserving rather than "fixing".

```asm
800889a4: fmuls   31, 1, 25            ; f31 = c494_8c * f3
```

`f31` is then the primary scale for the correction.

## Vector math

The SDK vector calls appear in two clusters:

| Cluster | Calls | Address |
|---------|-------|---------|
| 1 | `PSVECMag`, `PSVECScale`, `PSVECAdd` | `0x80088dc0`, `0x80088dd8`, `0x80088de8` |
| 2 | `PSVECDotProduct`, `PSVECScale`, `PSVECAdd` | `0x800891d8`, `0x80089200`, `0x80089210` |

The second cluster normalises the correction vector (dot product against
itself, scale, add) after the first cluster has produced a raw magnitude.
The final result is written through `r21` (`r4`) when it is non-`NULL`;
both in-tree callers pass `r4 = 0`:

- `marioCheckWall` passes `li 4, 0` at `0x8008ba00`
- `marioSearchFrontWall_staying` passes `li 4, 0` at `0x8008b734`

## Probe groups

`hitCheckVecFilter` (`0x800158a4`) is called **12 times**, in four groups
of three, each preceded by `marioGetPtr`:

| Group | Filter calls | `marioGetPtr` |
|-------|--------------|----------------|
| 1 | `0x80088a8c`, `0x80088ab0`, `0x80088ac4` | `0x80088a70`, `0x80088a94` |
| 2 | `0x80088b74`, `0x80088b98`, `0x80088bac` | `0x80088b58`, `0x80088b7c` |
| 3 | `0x80088ef8`, `0x80088f1c`, `0x80088f30` | `0x80088edc`, `0x80088f00` |
| 4 | `0x80088fe0`, `0x80089004`, `0x80089018` | `0x80088fc4`, `0x80088fe8` |

`hitGetAttr` (`0x80014310`) is called twice per classification block, at
`0x80088c1c`/`0x80088c74` and `0x80089088`/`0x800890e0`. `sincosf`
(`0x80005e8c`) is called at `0x80088a0c`, `0x80088d14`, `0x80088e78` and
`0x80089180` — once per group.

## Called functions

| Address | Symbol | Count | Role |
|---------|--------|-------|------|
| `0x801feeb8` | `kpaGetStageViewType` | 1 | probe-count gate |
| `0x8005e0c0` | `marioGetPtr` | 10 | |
| `0x80005e8c` | `sincosf` | 4 | per-group angle resolution |
| `0x800158a4` | `hitCheckVecFilter` | 12 | the probes |
| `0x80014310` | `hitGetAttr` | 4 | classification |
| `0x8029ec40` | `PSVECMag` | 1 | magnitude |
| `0x8029ec84` | `PSVECDotProduct` | 1 | self-dot for normalisation |
| `0x8029ebc8` | `PSVECScale` | 2 | |
| `0x8029eb80` | `PSVECAdd` | 2 | |

## Porting notes

1. The probe count switch (8 vs 2) is driven by Koopa state, not by a
   collision result. Keep it tied to `kpaGetStageViewType`.
2. `xoris 3, 27, 32768` negates via the integer bit pattern; the stored
   value is `-8` or `-2`. Do not replace with a float negation.
3. The divisor has **no divide-by-zero guard** in the original. A port that
   adds one changes behaviour on the degenerate input.
4. Both known callers pass `r4 = NULL`, so the `r21` write path is
   currently dead in-tree. It still needs to exist for parity.

## Remaining gaps

- The four probe groups' filter construction and the reduction that picks
  the correction vector are not decoded; this is the bulk of the
  function.
- Which of `mario[+0x3c]` (read in `searchFrontWall`) relates to the
  `kpaGetStageViewType` branch here.
- Meaning of the work doubles at `0x800c3b78+0x190` and `+0x1f0`.
- Numeric values of `c494_84`, `c494_88`, `c494_8c`.
- The constant at `0x800c4330` loaded by `lis 0, 17200` is not resolved.
- Whether `f2` (angle) or `f4` (plane) is used for the correction sign.