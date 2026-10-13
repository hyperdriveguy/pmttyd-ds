# marioCheckWall_roll

**Address:** `0x8008af78`
**Size:** 1376 bytes
**Namespace:** `mario_hit.o`
**Evidence class:** B (inspected US bytes/control flow)

## Overview

`marioCheckWall_roll` handles wall detection while Mario is in the roll
motion. It maintains a small "last seen front wall" record on the Mario
struct, probes for a wall ahead, and decides whether to commit to the
staying-in-front-wall follow-up (`marioSearchFrontWall_staying`).

## Signature

| Register | Use |
|----------|-----|
| `f1` | time / probe duration (held in `r29`) |
| `f2` | angle / facing (held in `r30`) |

Return value: not set on all paths observed — the visible exits are plain
`b` to the epilogue at `0x8008b4a4` / `blr` at `0x8008b4d4`.

## Prologue and gate

```asm
8008afa8: fmr     29, 1                 ; f1 -> r29
8008afac: fmr     30, 2                 ; f2 -> r30
8008afb0: bl      0x8005e0c0            ; marioGetPtr
8008afb4: lfs     0, 0x8004946c         ; threshold constant A
8008afb8: mr      31, 3                 ; r31 = Mario
8008afbc: lfs     1, 0x80049468
8008afc0: fcmpu   0, 29, 0             ; compare f1 against constant A
8008afc4: stfs    1, 532(3)            ; mario[+0x214] = constant B
8008afc8: bt      2, 0x8008b01c        ; if f1 < A -> skip
8008afcc: lha     0, 80(31)            ; mario[+0x50] (halfword)
8008afd0: cmpwi   0, 0
8008afd4: bf      2, 0x8008b01c        ; if == 0 -> skip
```

Two gates precede the body: `f1 >= constant@0x8004946c` **and**
`mario[+0x50] != 0`. Both must hold; otherwise control falls to
`0x8008b01c`.

## Wall-hold timer (`0x8008afd8`..`0x8008b020`)

A three-field record at `mario+0x1cc`..`mario+0x1d4`:

```asm
8008afd8: lfs     0, 460(31)           ; mario[+0x1cc]  (float)
8008afdc: fcmpo   0, 0, 29
8008afe0: bf      0, 0x8008aff8        ; if constant > f1 -> decrement branch
8008afe4: stfs    29, 460(31)          ; mario[+0x1cc] = f1
8008afe8: li      0, 14
8008afec: stfs    30, 464(31)          ; mario[+0x1d0] = f2
8008aff0: stw     0, 468(31)           ; mario[+0x1d4] = 14
...
8008aff8: lwz     3, 468(31)
8008affc: addic.  0, 3, -1
8008b000: stw     0, 468(31)           ; mario[+0x1d4] -= 1
8008b004: bt      1, 0x8008b024        ; if result != 0 -> continue
8008b008: li      0, 0
8008b00c: stw     0, 468(31)           ; clamp to 0
8008b010: stfs    29, 460(31)          ; re-arm mario[+0x1cc]
```

Behavior: when `mario[+0x1cc] < f1` the record is **re-armed** with the
new time and angle and a counter of **14**. Otherwise the counter is
decremented and, on reaching zero, the record is re-armed again. This is a
14-tick hold-off that keeps the roll wall contact alive across frames.

## Front-wall search (`0x8008b310`)

```asm
8008b310: lwz     5, 140(31)           ; r5 = mario[+0x8c]
8008b314: fmr     1, 29                ; f1 = f1 arg
8008b318: lwz     0, 144(31)           ; r4 -> later stack+8
8008b31c: fmr     2, 30                ; f2 = f2 arg
8008b320: addi    3, 1, 36             ; r3 = &stack[36]
8008b324: addi    4, 1, 8              ; r4 = &stack[8]
8008b328: stw     5, 36(1)
8008b32c: stw     0, 40(1)
8008b330: lwz     0, 148(31)           ; mario[+0x94]
8008b334: stw     0, 44(1)
8008b338: lfs     3, 448(31)           ; f3 = mario[+0x1c0]
8008b33c: bl      0x8008a2f0           ; searchFrontWall
8008b340: mr.     30, 3
8008b344: bt      2, 0x8008b448        ; if result == 0 -> skip
8008b348: stw     30, 484(31)          ; mario[+0x1e4] = result
```

`searchFrontWall` receives an 3-word descriptor at `stack+36`
(`mario[+0x8c]`, `mario[+0x90]`, `mario[+0x94]`), an output at `stack+8`,
the two float arguments, and `mario[+0x1c0]` as a fourth float. A nonzero
result is latched into `mario[+0x1e4]`.

## Hit classification

Immediately after the search, the same status-bit family used by
[the slit probe](MARIO_SLIT_CHK_WALL_AROUND.md) is applied:

```asm
8008b34c: bl      0x80014310           ; hitGetAttr
8008b354: bl      0x8005e0c0           ; marioGetPtr
8008b358: rlwinm. 0, 29, 0, 12, 12     ; bit 12
8008b360: lwz     0, 0(3)
8008b364: rlwinm. 0, 0, 0, 7, 7        ; bit 7 of mario[+0]
```

## Exit to the staying search

```asm
8008b2d4: lfs     0, 0x8004946c
8008b2d8: fcmpu   0, 29, 0
8008b2dc: bf      2, 0x8008b2ec
8008b2e0: fmr     1, 30
8008b2e4: bl      0x8008b4d8           ; marioSearchFrontWall_staying
8008b2e8: b       0x8008b4a4           ; -> epilogue
```

The same `f1 >= constant A` gate reappears here. When it passes, the
second float argument is forwarded to `marioSearchFrontWall_staying` and
the function returns immediately — no further processing.

## Called functions

| Address | Symbol | Role |
|---------|--------|------|
| `0x8005e0c0` | `marioGetPtr` | global Mario pointer |
| `0x800158a4` | `hitCheckVecFilter` | 3 vector filter probes |
| `0x80014310` | `hitGetAttr` | attribute mask readback |
| `0x8008a2f0` | `searchFrontWall` | main front-wall query (2404 bytes) |
| `0x8008b4d8` | `marioSearchFrontWall_staying` | follow-up wall search (696 bytes) |
| `0x8008b8c0`+0x0 | `marioCheckWall` | (in tail range) |
| `0x80005de4` | `movePos` | position advance |

## Mario struct offsets touched

| Offset | Access | Notes |
|--------|--------|-------|
| `+0x00` | word, bit 7 tested | global mode flag |
| `+0x50` | halfword | gate: must be nonzero |
| `+0x8c` | word | descriptor word 0 |
| `+0x90` | word/float | descriptor word 1; float written at `0x8008b2d0` |
| `+0x94` | word | descriptor word 2 |
| `+0x180` | float | zeroed when `mario[+0x1f8] != 0` |
| `+0x1c0` | float | passed as `f3` to `searchFrontWall` |
| `+0x1cc` | float | wall-hold time |
| `+0x1d0` | float | wall-hold angle |
| `+0x1d4` | word | wall-hold counter, armed to 14 |
| `+0x1e4` | word | latched `searchFrontWall` result |
| `+0x1f8` | word | controls the `+0x180` reset |
| `+0x214` | float | set to constant `0x80049468` on every call |

## Porting notes

1. `mario[+0x214]` is written **unconditionally** before the gate check —
   a port that skips this when the gates fail will diverge.
2. The 14-tick re-arm is a tick counter, not a timer; keep it in
   simulation ticks per the project convention.
3. The staying-search exit is an early `b` to the epilogue; it must not
   fall through to the trailing work.
4. The `mario[+0x90]` float write at `0x8008b2d0` (`1.0 - x`) is part of
   the roll wall-normal correction, not a scratch write.

## Remaining gaps

- Numeric values of the `lfs` constants at `0x80049468` / `0x8004946c`
  are not extracted; the second one is the activation threshold.
- The three `hitCheckVecFilter` descriptors are built from stack data that
  has not been fully mapped.
- Whether `mario[+0x1e4]` is consumed by the roll animation driver or by
  `marioChgPose` dispatch.
- `searchFrontWall` and `marioChkWallAround` are now documented at
  structure level — see [SEARCH_FRONT_WALL](SEARCH_FRONT_WALL.md) and
  [MARIO_CHK_WALL_AROUND](MARIO_CHK_WALL_AROUND.md). `marioChgWallAround`
  in that older note is a transcription error for `marioChkWallAround`.
- Callers of `marioCheckWall_roll` are not audited.