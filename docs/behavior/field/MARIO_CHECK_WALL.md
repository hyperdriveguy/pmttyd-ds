# marioCheckWall

**Address:** `0x8008b790`
**Size:** 684 bytes (171 instructions)
**Namespace:** `mario_hit.o`
**Evidence class:** B (inspected US bytes/control flow — complete body)

## Overview

`marioCheckWall` is the general standing/running wall check. It shares the
gate structure and 14-tick wall-hold record with
[`marioCheckWall_roll`](MARIO_CHECK_WALL_ROLL.md), but differs in three
substantive ways: it has a **byte counter** (`mario[+0x3f]`), a
**name-matched** hit classification using `strncmp`, and an in-place
`movePos` slide. It finishes with `dou10_yoko_yari` ("horizontal
disengage") applied to the published length at `mario[+0x214]`.

This body is documented **completely**.

## Signature

| Register | Use |
|----------|-----|
| `f1` | probe length (saved to `r30`) |
| `f2` | angle (saved to `r31`) |

Return: not set — the epilogue has no `li 3, 0`. `r3` leaks from
`dou10_yoko_yari`. Stack frame: 80 bytes.

## Prologue and gate (`0x8008b7c0`..`0x8008b7e4`)

```asm
8008b7c0: bl      0x8005e0c0           ; marioGetPtr
8008b7c4: lfs     0, 0x8004946c
8008b7cc: lfs     1, 0x80049468
8008b7d0: fcmpu   0, 30, 0            ; f1 vs c494_6c
8008b7d4: stfs    1, 532(3)           ; mario[+0x214] = c494_68  (unconditional)
8008b7d8: bt      2, 0x8008b834        ; f1 < c -> early exit
8008b7dc: lha     0, 80(31)            ; mario[+0x50]
8008b7e0: cmpwi   0, 0
8008b7e4: bf      2, 0x8008b834        ; == 0 -> early exit
```

Identical to the roll variant: `mario[+0x214] = c494_68` is written
**before** the gate.

## Wall-hold re-arm (`0x8008b7e8`..`0x8008b83c`)

```asm
8008b7e8: lfs     0, 460(31)           ; mario[+0x1cc]
8008b7ec: fcmpo   0, 0, 30
8008b7f0: bf      0, 0x8008b808
8008b7f4: stfs    30, 460(31)          ; mario[+0x1cc] = f1
8008b7f8: li      0, 14
8008b7fc: stfs    31, 464(31)          ; mario[+0x1d0] = f2
8008b800: stw     0, 468(31)           ; mario[+0x1d4] = 14
8008b804: b       0x8008b828
8008b808: lwz     3, 468(31)
8008b80c: addic.  0, 3, -1
8008b810: stw     0, 468(31)           ; mario[+0x1d4] -= 1
8008b814: bt      1, 0x8008b828
8008b818: li      0, 0
8008b81c: stw     0, 468(31)           ; clamp to 0
8008b820: stfs    30, 460(31)          ; re-arm time
8008b824: stfs    31, 464(31)          ; re-arm angle   <-- extra vs roll variant
8008b828: lfs     0, 464(31)
8008b82c: stfs    0, 416(31)           ; mario[+0x1a0] = mario[+0x1d0]
```

Same 14-tick re-arm, with one addition: the re-arm-on-expiry path also
restores `mario[+0x1d0]`, and afterwards `mario[+0x1a0]` is published from
`mario[+0x1d0]`.

The early-exit target `0x8008b834` clears the hold:

```asm
8008b834: lfs     0, 0x8004946c
8008b838: stfs    0, 460(31)           ; mario[+0x1cc] = 0
```

## Dispatch on probe length (`0x8008b83c`..`0x8008b858`)

```asm
8008b83c: lfs     0, 0x8004946c
8008b840: fcmpu   0, 30, 0
8008b844: bf      2, 0x8008b85c
8008b848: fmr     1, 31
8008b84c: bl      0x8008b4d8           ; marioSearchFrontWall_staying
8008b850: addi    3, 31, 532
8008b854: bl      0x80089e94           ; dou10_yoko_yari
8008b858: b       0x8008ba10           ; epilogue
```

Long probe (`f1 >= c494_6c`) delegates to the staying search and exits.
This is the "already wedged" path.

## Main probe (`0x8008b85c`..`0x8008b898`)

```asm
8008b85c: lwz     4, 140(31)           ; mario[+0x8c]
8008b860: li      0, 0
8008b864: lwz     5, 144(31)           ; mario[+0x90]
8008b870: addi    3, 1, 12             ; r3 = &stack[12]
8008b874: stw     4, 12(1)
8008b878: addi    4, 1, 8              ; r4 = &stack[8]
8008b87c: stw     5, 16(1)
8008b880: lwz     5, 148(31)           ; mario[+0x94]
8008b884: stw     5, 20(1)
8008b888: stw     0, 528(31)           ; mario[+0x210] = 0
8008b88c: lfs     3, 448(31)           ; f3 = mario[+0x1c0]
8008b890: bl      0x8008a2f0           ; searchFrontWall(&stack12, &stack8)
8008b894: mr.     30, 3                ; r30 = result
8008b898: bt      2, 0x8008b9d4        ; no hit -> copy descriptor back
```

`mario[+0x210]` is a second hit record, cleared before the probe.

## Record latch (`0x8008b89c`..`0x8008b8b4`)

```asm
8008b89c: lwz     0, 528(31)           ; mario[+0x210]
8008b8a0: cmplwi  0, 0
8008b8a4: bt      2, 0x8008b8b0
8008b8a8: stw     0, 484(31)           ; mario[+0x1e4] = mario[+0x210]
8008b8ac: b       0x8008b8b4
8008b8b0: stw     30, 484(31)          ; else mario[+0x1e4] = result
```

The secondary record wins when present.

## Three-way blocked classification (`0x8008b8b4`..`0x8008b934`)

```asm
8008b8b8: bl      0x80014310           ; hitGetAttr
8008b8c0: bl      0x8005e0c0
8008b8c4: rlwinm. 0, 29, 0, 12, 12     ; bit 12  -> blocked
8008b8cc: lwz     0, 0(3)
8008b8d0: rlwinm. 0, 0, 0, 7, 7        ; bit 7 of mario[+0] -> blocked
8008b8d8: li      0, 0                 ; else 0
8008b8e0: rlwinm. 0, 29, 0, 24, 24     ; bit 24 -> clear bit 0, then blocked
8008b8f0: lhz     0, 46(3)             ; mario[+0x2e]
8008b8f4: subfic  0, 0, 25
8008b8f8: cntlzw  0, 0
8008b8fc: srwi    0, 0, 5              ; != 25 -> blocked
```

Byte-for-byte the same idiom used by `marioSlitChkWallAround` and
`marioCheckWall_roll`.

### Blocked branch (`0x8008b908`..`0x8008b930`)

```asm
8008b908: lfs     0, 0x8004946c
8008b90c: fcmpu   0, 30, 0
8008b910: bt      2, 0x8008b91c
8008b914: stw     30, 496(31)          ; mario[+0x1f0] = result
8008b918: stw     30, 500(31)          ; mario[+0x1f4] = result
8008b91c: fmr     1, 30
8008b920: addi    3, 31, 140           ; r3 = &mario[+0x8c]   (IN PLACE)
8008b924: fmr     2, 31
8008b928: addi    4, 31, 148           ; r4 = &mario[+0x94]   (IN PLACE)
8008b92c: bl      0x80005de4           ; movePos
```

This is the slide: unlike every other path in this file, `movePos` writes
**directly into the Mario struct** at `+0x8c` and `+0x94`.

### Unblocked branch (`0x8008b934`..`0x8008b9b8`)

```asm
8008b934: lfs     0, 8(1)
8008b93c: stfs    0, 532(31)           ; mario[+0x214] = distance
8008b940: stw     30, 500(31)          ; mario[+0x1f4] = result
8008b944: bl      0x80014328           ; hitGetName
8008b948: addi    4, 2, -27396         ; r4 = 0x80049504
8008b94c: li      5, 5                 ; r5 = 5
8008b950: bl      0x8026d424           ; strncmp(name, 0x80049504, 5)
8008b954: cmpwi   3, 0
8008b958: li      4, 10
8008b95c: bt      2, 0x8008b964        ; match -> r4 = 10
8008b960: li      4, 20                ; else  r4 = 20
8008b964: lbz     3, 63(31)            ; mario[+0x3f]
8008b968: addi    3, 3, 1
8008b96c: extsb   0, 3
8008b970: stb     3, 63(31)            ; mario[+0x3f]++  (signed byte)
8008b974: cmpw    0, 4
8008b978: bt      0, 0x8008b9b8        ; already == r4 -> no re-test
8008b97c: stb     4, 63(31)            ; mario[+0x3f] = r4
8008b980: mr      3, 30
8008b984: bl      0x80014310           ; hitGetAttr
8008b988: rlwinm. 0, 3, 0, 24, 24      ; bit 24
8008b98c: bf      2, 0x8008b998
8008b990: li      0, 1
8008b994: b       0x8008b9ac
8008b998: bl      0x8005e0c0
8008b99c: lhz     0, 46(3)
8008b9a0: subfic  0, 0, 25
8008b9a4: cntlzw  0, 0
8008b9a8: srwi    0, 0, 5
8008b9ac: cmpwi   0, 0
8008b9b0: bt      2, 0x8008b9b8
8008b9b4: stw     30, 496(31)          ; mario[+0x1f0] = result
```

This is a **hysteresis counter**:

1. Publish the distance into `mario[+0x214]` and the result into
   `mario[+0x1f4]`.
2. Compare the hit object's name against the 5-character literal at
   `0x80049504`. A prefix match selects threshold **10**, otherwise **20**.
3. Increment the signed byte `mario[+0x3f]`.
4. Only when the incremented value **differs** from the threshold is the
   byte clamped to the threshold and a second hit test run. When it
   already equals the threshold, the re-test is **skipped**.

The re-test uses a two-way classification (bit 24 set, **or**
`mario[+0x2e] != 25`) — note it does **not** include the bit-12/bit-7
terms used in the main classification. A hit sets `mario[+0x1f0]`.

## Descriptor commit and correction

Both the no-hit path (`0x8008b9d4`) and the unblocked path
(`0x8008b9b8`) copy `stack[12..20]` back into `mario[+0x8c..+0x94]` with
three `stw`s each:

```asm
8008b9b8: lwz     3, 12(1)
8008b9bc: lwz     0, 16(1)
8008b9c0: stw     3, 140(31)           ; mario[+0x8c] = stack[12]
8008b9c4: stw     0, 144(31)           ; mario[+0x90] = stack[16]
8008b9c8: lwz     0, 20(1)
8008b9cc: stw     0, 148(31)           ; mario[+0x94] = stack[20]
```

Then both paths converge:

```asm
8008b9ec: fmr     1, 30
8008b9f0: lfs     3, 448(31)           ; f3 = mario[+0x1c0]
8008b9f4: fmr     2, 31
8008b9f8: lfs     4, 444(31)           ; f4 = mario[+0x1bc]
8008b9fc: addi    3, 31, 140           ; r3 = &mario[+0x8c]
8008ba00: li      4, 0                 ; r4 = NULL
8008ba04: bl      0x800888d0           ; marioChkWallAround
8008ba08: addi    3, 31, 532
8008ba0c: bl      0x80089e94           ; dou10_yoko_yari(&mario[+0x214])
```

Note the `movePos` slide path branches to `0x8008b9ec`, so **slid** Mario
still receives the `marioChkWallAround` correction and the disengage pass —
but its descriptor was modified in place and is not re-copied.

## Called functions

| Address | Symbol | Count | Role |
|---------|--------|-------|------|
| `0x8005e0c0` | `marioGetPtr` | 4 | |
| `0x8008a2f0` | `searchFrontWall` | 1 | the probe |
| `0x8008b4d8` | `marioSearchFrontWall_staying` | 1 | long-probe delegate |
| `0x80005de4` | `movePos` | 1 | in-place slide |
| `0x80014310` | `hitGetAttr` | 2 | classification |
| `0x80014328` | `hitGetName` | 1 | 5-char name match |
| `0x8026d424` | `strncmp` | 1 | name gate |
| `0x800888d0` | `marioChkWallAround` | 1 | slip correction |
| `0x80089e94` | `dou10_yoko_yari` | 2 | horizontal disengage |

## Mario struct offsets

| Offset | Access | Role |
|--------|--------|------|
| `+0x2e` | halfword | `!= 25` in the hysteresis re-test |
| `+0x3f` | byte (signed) | hysteresis counter, thresholds 10 / 20 |
| `+0x50` | halfword | gate: must be nonzero |
| `+0x8c` / `+0x90` / `+0x94` | word | probe descriptor, slide target, commit target |
| `+0x1a0` | float | published from `mario[+0x1d0]` |
| `+0x1bc` | float | `f4` to `marioChkWallAround` |
| `+0x1c0` | float | `f3` to `searchFrontWall` / `marioChkWallAround` |
| `+0x1cc` / `+0x1d0` / `+0x1d4` | float/float/word | wall-hold time / angle / 14-tick counter |
| `+0x1e4` | word | primary wall record |
| `+0x1f0` | word | slide / hysteresis record |
| `+0x1f4` | word | unblocked hit record |
| `+0x210` | word | secondary hit record, cleared per call |
| `+0x214` | float | published length |

## Porting notes

1. The hysteresis counter is a **signed byte** — `extsb` at `0x8008b96c`
   is load-bearing. Threshold comparison is signed (`cmpw`, not `cmplwi`),
   so a counter that wraps past 127 will not equal 10 or 20 and will keep
   re-testing.
2. The re-test classifier is a **different, smaller** predicate than the
   main classifier. Do not reuse the three-way test here.
3. `movePos` in the blocked path writes in place into the Mario struct;
   no descriptor copy follows.
4. `mario[+0x214] = c494_68` is unconditional and precedes all gates.

## Remaining gaps

- The 5-character literal at `0x80049504` is not read from the ISO; the
  identity of the matching surface type is unknown.
- Numeric values of `c494_68` / `c494_6c` are not extracted.
- `dou10_yoko_yari` (1116 bytes) and `marioChkWallAround` (2460 bytes)
  are undocumented.
- Whether the leaked `r3` from `dou10_yoko_yari` is ever consumed by a
  caller of `marioCheckWall`.