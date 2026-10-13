# searchFrontWall

**Address:** `0x8008a2f0`
**Size:** 2404 bytes
**Namespace:** `mario_hit.o`
**Evidence class:** B (inspected US bytes/control flow) — **structure-level, not a complete decompilation**

## Overview

`searchFrontWall` is the shared front-wall probe used by every Mario wall
routine. It takes a 3-word planar descriptor, resolves an angular ray with
`sincosf`, runs a sequence of `hitCheckVecFilter` probes with partner and
field-name gates, and returns the hit handle (or 0).

This document covers the entry gates, the bypass path, the partner
interaction block and the vector math. The middle of the function — the
per-angle probe sequence and its result reduction — is **not** fully
decoded. That work is listed under Remaining gaps.

## Signature

| Register | Use | Saved to |
|----------|-----|----------|
| `f1` | probe length | `f24` |
| `f2` | angle | `f25` |
| `f3` | height / plane | `f26` |
| `r3` | in/out: 3-word planar descriptor (`&desc[0..2]`) | `r30` |
| `r4` | out: distance / normal output | `r20` |

Stack frame: 592 bytes. Static work descriptor: `0x800c3b78`
(`lis 5,-32724` + `addi 31,5,15224`), shared with
[`marioSlitChkWallAround`](MARIO_SLIT_CHK_WALL_AROUND.md).

## Gate 1 — Vivian must be in state 2 (`0x8008a35c`..`0x8008a374`)

```asm
8008a35c: bl      0x8005e0c0           ; marioGetPtr
8008a360: mr      24, 3                ; r24 = Mario
8008a364: bl      0x801865b8           ; vivianGetStatus
8008a368: cmpwi   3, 2
8008a36c: bf      2, 0x8008a378
8008a370: li      3, 0
8008a374: b       0x8008ac00           ; -> return 0
```

**If `vivianGetStatus() != 2` the function returns 0 immediately.** This is
the first thing every caller pays for.

## Angle resolution (`0x8008a378`..`0x8008a384`)

```asm
8008a378: fmr     1, 25                ; f1 = angle
8008a37c: addi    3, 1, 20             ; r3 = &stack[20]
8008a380: addi    4, 1, 16             ; r4 = &stack[16]
8008a384: bl      0x80005e8c           ; sincosf
```

`sincosf(angle, &stack[20], &stack[16])` — the GameCube wrapper at
`0x80005e8c` writes sin to the first out-pointer and cos to the second.
`stack[20]` is sin, `stack[16]` is cos.

## Gate 2 — status flag and field name (`0x8008a388`..`0x8008a3ac`)

```asm
8008a388: li      3, 16
8008a38c: bl      0x8005bf3c           ; marioChkSts(16)
8008a390: cmpwi   3, 0
8008a394: bt      2, 0x8008a404        ; no flag -> main probe path
8008a398: lwz     3, -28496(13)        ; field record
8008a39c: addi    4, 31, 536           ; r4 = 0x800c3d88 (string)
8008a3a0: addi    3, 3, 300            ; field->name
8008a3a4: bl      0x8026d464           ; strcmp
8008a3ac: bf      2, 0x8008a404        ; name mismatch -> main probe path
```

Status bit 16 must be set **and** the field name must match the literal at
`0x800c3d88` for the bypass to be considered. This is the **same field
literal** that gates stage 3 of
[`marioSearchFrontWall_staying`](MARIO_SEARCH_FRONT_WALL_STAYING.md) —
that function and this one are gated on one specific field.

## Bypass path (`0x8008a3b0`..`0x8008a400`)

```asm
8008a3b0: lfs     0, 540(24)           ; mario[+0x21c]
8008a3b4: lfs     1, 0x8004947c
8008a3b8: fabs    0, 0
8008a3bc: fcmpo   0, 0, 1
8008a3c0: cror    2, 1, 2              ; branchless select
8008a3c4: bt      2, 0x8008a3dc
8008a3c8: lfs     0, 548(24)           ; mario[+0x224]
8008a3cc: fabs    0, 0
8008a3d4: cror    2, 1, 2
8008a3d8: bf      2, 0x8008a404        ; second test fails -> main path
8008a3dc: lfs     1, 20(1)             ; sin
8008a3e0: li      3, 0
8008a3e4: lfs     0, 0(30)             ; desc[0]
8008a3e8: fmadds  0, 24, 1, 0          ; desc[0] + probeLength * sin
8008a3ec: stfs    0, 0(30)
8008a3f0: lfs     1, 16(1)             ; cos
8008a3f4: lfs     0, 8(30)             ; desc[2]
8008a3f8: fmadds  0, 24, 1, 0          ; desc[2] + probeLength * cos
8008a3fc: stfs    0, 8(30)
8008a400: b       0x8008ac00
```

Two absolute-value comparisons against `c494_7c` on `mario[+0x21c]` and
`mario[+0x224]`, each using the `cror`-based branchless select. If both
pass, the descriptor is advanced along the ray by the full probe length and
the function returns 0 with **no collision test at all** — a straight-line
extrapolation.

## Main probe path entry (`0x8008a404`)

```asm
8008a404: lbz     0, 60(24)            ; mario[+0x3c]
8008a408: li      29, 2
8008a40c: cmpwi   0, 2
8008a410: bf      2, 0x8008a480
8008a414: lwz     0, 20(24)            ; mario[+0x14]
```

## Partner interaction block (`0x8008a448`..`0x8008a4b0`)

```asm
8008a448: bl      0x801ff028           ; kpaGetLevel
8008a48c: bl      0x8005e0c0           ; marioGetPtr
8008a4a0: bl      0x80141050           ; marioGetPartyId
8008a4a4: bl      0x800bf1b4           ; partyGetPtr
8008a4b0: bl      0x800be4c4           ; partyGetHeight
```

This block queries the current party: Koopa Paratroopa level, the party id
attached to Mario, the partner pointer and its height. It is the only place
in the function that reaches into partner state, and it sits between the
gate block and the first `hitCheckVecFilter` group.

## Probe groups

`hitCheckVecFilter` (`0x800158a4`) is called **9 times**, in three groups
of three, each preceded by a fresh `marioGetPtr`:

| Group | Filter calls | `marioGetPtr` |
|-------|--------------|----------------|
| 1 | `0x8008a600`, `0x8008a624`, `0x8008a638` | `0x8008a5e4`, `0x8008a608` |
| 2 | `0x8008a974`, `0x8008a998`, `0x8008a9ac` | `0x8008a958`, `0x8008a97c` |
| 3 | `0x8008aae0`, `0x8008ab04`, `0x8008ab18` | `0x8008aac4`, `0x8008aae8` |

Groups 1 and 2 are separated by a `chkBalloon` call at `0x8008a7f0` and an
`sincosf` at `0x8008a8b4`; group 3 is preceded by another `sincosf` at
`0x8008aa4c`. The three-group shape mirrors the three-label pattern seen
in the slit and roll probes.

## Called functions

| Address | Symbol | Count | Role |
|---------|--------|-------|------|
| `0x8005e0c0` | `marioGetPtr` | 7 | |
| `0x80005e8c` | `sincosf` | 3 | angle -> ray components |
| `0x801865b8` | `vivianGetStatus` | 1 | gate 1 |
| `0x8005bf3c` | `marioChkSts` | 1 | gate 2 |
| `0x8026d464` | `strcmp` | 1 | field-name gate |
| `0x801ff028` | `kpaGetLevel` | 1 | partner query |
| `0x80141050` | `marioGetPartyId` | 1 | partner query |
| `0x800bf1b4` | `partyGetPtr` | 1 | partner query |
| `0x800be4c4` | `partyGetHeight` | 1 | partner query |
| `0x8012ef34` | `chkBalloon` | 1 | balloon clearance |
| `0x800158a4` | `hitCheckVecFilter` | 9 | the probes |

## Porting notes

1. Gate 1 is a hard early return. Any port that lets the function proceed
   without Vivian in state 2 will diverge immediately.
2. The bypass path **writes to the caller's descriptor in place** and
   returns 0 — callers that keep using the descriptor after a zero return
   see advanced coordinates. This is relied upon by
   `marioSearchFrontWall_staying` and `marioCheckWall`.
3. The bypass requires a specific field name, status bit 16, and two
   bounded Mario floats. All four conditions must hold.
4. `sincosf` is the engine's own wrapper (`0x80005e8c`), not libm
   `sincosf`; preserve the sin/cos out-parameter order.

## Remaining gaps

- The per-angle descriptor construction and the 9-probe result reduction
  are not decoded; this is the bulk of the function.
- Meaning of `mario[+0x21c]` / `mario[+0x224]` (both compared by absolute
  value against `c494_7c`) and of status bit 16.
- The literal field name at `0x800c3d88` is not read from the ISO.
- Numeric values of `c494_7c` and the other `lfs` constants.
- Whether the three probe groups differ in filter mask or only in origin.
- The `r20` out-parameter is written in the decoded regions but its layout
  is not established.