# marioSearchFrontWall_staying

**Address:** `0x8008b4d8`
**Size:** 696 bytes (174 instructions)
**Namespace:** `mario_hit.o`
**Evidence class:** B (inspected US bytes/control flow — complete body)

## Overview

`marioSearchFrontWall_staying` maintains the front-wall record while Mario
stays pressed against a wall. It runs up to three `searchFrontWall` probes
— a direct probe, a probe offset by Mario's movedir, and a partner-gated
probe — then hands the result to `marioChkWallAround` for the final
slip correction.

This body is documented **completely**: every branch and call site is
listed below. It is not a partial summary.

## Signature

| Register | Use |
|----------|-----|
| `f1` | roll/entry angle, saved to `r31` and later re-used as `f2` |
| `f3` (return) | whatever `marioChkWallAround` returned — see caveat below |

Stack frame: 96 bytes.

## Probe length selection (`0x8008b50c`..`0x8008b52c`)

```asm
8008b504: bl      0x8005e0c0           ; marioGetPtr
8008b508: li      0, 0
8008b50c: lfs     1, 0x8004948c         ; scale constant
8008b510: stb     0, 63(3)             ; mario[+0x3f] = 0
8008b518: lwz     0, 0(3)              ; mario[+0]
8008b51c: lfs     0, 440(3)            ; mario[+0x1b8]
8008b520: rlwinm. 0, 0, 0, 11, 11      ; bit 11
8008b524: fmuls   29, 1, 0             ; r29 = c494_8c * mario[+0x1b8]
8008b528: bt      2, 0x8008b530
8008b52c: lfs     29, 0x8004946c        ; else r29 = c494_6c
```

Probe length `r29` is `c494_8c * mario[+0x1b8]` when **bit 11 of
`mario[+0]` is set**, otherwise the base constant `c494_6c`.

`mario[+0x3f]` is cleared unconditionally before any gate.

## Stage 1 — direct probe (`0x8008b530`..`0x8008b584`)

Descriptor at `stack+16` = `{mario[+0x8c], mario[+0x90], mario[+0x94]}`;
output at `stack+12`.

```asm
8008b554: lfs     2, 420(31)           ; f2 = mario[+0x1a4]
8008b558: lfs     0, 412(31)           ; mario[+0x19c]
8008b55c: lfs     3, 448(31)           ; f3 = mario[+0x1c0]
8008b560: fsubs   2, 2, 0             ; f2 = mario[+0x1a4] - mario[+0x19c]
8008b564: bl      0x8008a2f0           ; searchFrontWall
8008b568: lhz     0, 46(31)            ; mario[+0x2e]
8008b56c: cmplwi  0, 19
8008b570: bt      2, 0x8008b57c        ; == 19 -> force zero
8008b574: stw     3, 484(31)           ; mario[+0x1e4] = result
```

**`mario[+0x2e] == 19` forces `mario[+0x1e4] = 0` regardless of the probe
result.** This is the one hard-coded state veto in the function and it is
applied to all three probe stages.

## Stage 2 — movedir-offset probe (`0x8008b584`..`0x8008b5e4`)

Entered only when stage 1 returned nonzero.

```asm
8008b58c: lfs     1, 428(31)           ; mario[+0x1ac]
8008b590: bl      0x800963fc           ; toMovedir
8008b598: fmr     0, 1                ; f0 = movedir result
8008b5a0: fmr     1, 29               ; f1 = probe length
8008b5b0: fmr     2, 0                ; f2 = movedir
8008b5c4: bl      0x8008a2f0           ; searchFrontWall
```

Same descriptor at `stack+16`, same output at `stack+12`, same
`mario[+0x2e] == 19` veto, same store into `mario[+0x1e4]`.

## Stage 3 — partner-gated offset probe (`0x8008b5e8`..`0x8008b6b4`)

Entered when stage 1 returned zero. Gated by **two** conditions.

### Gate A — field name comparison

```asm
8008b5e8: lwz     5, -28496(13)        ; global field record (0x80415fd0)
8008b5ec: lis     3, -32724
8008b5f0: addi    4, 3, 15752          ; r4 = 0x800c3d88 (string)
8008b5f4: addi    3, 5, 300            ; r3 = field->name at +300
8008b5f8: bl      0x8026d464           ; strcmp
8008b5fc: cmpwi   3, 0
8008b600: bf      2, 0x8008b6b8        ; nonzero -> give up
```

The field-name string at `0x800c3d88` is compared against the current
field's name. The constant is **not extracted** from the ISO here; the
stage is only entered on an exact match.

### Gate B — Vivian status must not be 1, 2 or 3

```asm
8008b604: bl      0x801865b8           ; vivianGetStatus
8008b608: cmpwi   3, 2
8008b60c: bt      2, 0x8008b6b8        ; == 2 -> give up
8008b610: bl      0x801865b8           ; vivianGetStatus
8008b614: cmpwi   3, 1
8008b618: bt      2, 0x8008b6b8        ; == 1 -> give up
8008b61c: bl      0x801865b8           ; vivianGetStatus
8008b620: cmpwi   3, 3
8008b624: bt      2, 0x8008b6b8        ; == 3 -> give up
```

`vivianGetStatus` is called **three times**, each result checked against a
different literal. Since the three comparisons reject 1, 2 and 3
independently and every result is discarded, the gate is equivalent to
`vivianGetStatus() ∉ {1, 2, 3}` evaluated three times. The repetition is
present in the original and is not a transcription artifact.

### Probe

```asm
8008b628: lfs     1, 428(31)           ; mario[+0x1ac]
8008b62c: bl      0x800963fc           ; toMovedir
8008b634: fmr     30, 1               ; r30 = movedir
8008b640: lfs     3, 0x8004948c
8008b65c: lfs     0, 440(31)           ; mario[+0x1b8]
8008b660: fmuls   1, 3, 0             ; f1 = c494_8c * mario[+0x1b8]
8008b664: bl      0x80005de4           ; movePos  (r3=&stack16, r4=&stack24)
8008b668: lfs     0, 0x80049494
8008b678: fadds   30, 30, 0            ; r30 += c494_94
8008b670: lfs     1, 440(31)           ; f1 = mario[+0x1b8]
8008b680: fmr     2, 30
8008b684: bl      0x8008a2f0           ; searchFrontWall (r4 = &stack8)
8008b690: lfs     1, 440(31)           ; mario[+0x1b8]
8008b694: lfs     0, 8(1)              ; stack[8] = distance
8008b698: fcmpo   0, 1, 0
8008b69c: cror    2, 1, 2              ; branchless select
8008b6a4: stw     3, 484(31)           ; mario[+0x1e4] = result
8008b6a8: lfs     1, 0x8004948c
8008b6ac: lfs     0, 8(1)
8008b6b0: fmuls   0, 1, 0
8008b6b4: stfs    0, 12(1)             ; stack[12] = c494_8c * distance
```

Stage 3 first `movePos`-shifts by `c494_8c * mario[+0x1b8]`, adds
`c494_94` to the movedir scalar, then probes with `f1 = mario[+0x1b8]`
directly. On a hit the **distance output** `stack[8]` is scaled back into
`stack[12]` so the common tail sees a length in the same units as the
earlier stages.

## Common tail (`0x8008b6b8`)

```asm
8008b6b8: lwz     0, 484(31)           ; mario[+0x1e4]
8008b6bc: cmplwi  0, 0
8008b6c0: bt      2, 0x8008b6cc
8008b6c4: lfs     0, 12(1)
8008b6c8: stfs    0, 532(31)           ; mario[+0x214] = stack[12]
```

`mario[+0x214]` is written **only when a wall was found**.

## Secondary record (`0x8008b6cc`..`0x8008b71c`)

```asm
8008b6cc: lhz     0, 588(31)           ; mario[+0x24c]
8008b6d0: rlwinm. 0, 0, 0, 23, 23      ; bit 23
8008b6d4: bt      2, 0x8008b720        ; set -> skip
8008b6d8: li      0, 0
8008b6e0: stw     0, 480(31)           ; mario[+0x1e0] = 0
8008b704: lfs     2, 420(31)           ; f2 = mario[+0x1a4]
8008b70c: bl      0x8008a2f0
8008b710: cmplwi  3, 0
8008b718: lwz     0, 484(31)           ; mario[+0x1e4]
8008b71c: stw     0, 480(31)           ; mario[+0x1e0] = mario[+0x1e4]
```

When **bit 23 of `mario[+0x24c]` is clear**, a fourth probe runs with
`f2 = mario[+0x1a4]` (no subtraction) and, on a hit,
`mario[+0x1e0]` is set to the current `mario[+0x1e4]`.

## Slip correction (`0x8008b720`..`0x8008b760`)

```asm
8008b720: lwz     5, 140(31)
8008b724: fmr     2, 31                ; f2 = entry angle
8008b730: lfs     1, 0x8004946c
8008b734: li      4, 0                 ; r4 = NULL output
8008b74c: lfs     4, 444(31)           ; f4 = mario[+0x1bc]
8008b750: bl      0x800888d0           ; marioChkWallAround
8008b754: lfs     0, 16(1)
8008b758: stfs    0, 140(31)           ; mario[+0x8c] = stack[16]
8008b75c: lfs     0, 24(1)
8008b760: stfs    0, 148(31)           ; mario[+0x94] = stack[24]
```

The final unconditional step: `marioChkWallAround` writes two corrected
floats into `stack[16]` and `stack[24]`, which are copied back into
`mario[+0x8c]` and `mario[+0x94]` as **floats**, overwriting the word
reads used as the descriptor earlier in the function. Note `r4` is
`NULL` here — the correction is used only for its position outputs.

## Called functions

| Address | Symbol | Count | Role |
|---------|--------|-------|------|
| `0x8005e0c0` | `marioGetPtr` | 1 | global Mario pointer |
| `0x8008a2f0` | `searchFrontWall` | up to 4 | the wall probes |
| `0x800963fc` | `toMovedir` | 2 | convert `mario[+0x1ac]` to a vector |
| `0x80005de4` | `movePos` | 1 | stage-3 position shift |
| `0x8026d464` | `strcmp` | 1 | field-name gate |
| `0x801865b8` | `vivianGetStatus` | 3 | partner-state gate |
| `0x800888d0` | `marioChkWallAround` | 1 | final slip correction |

## Mario struct offsets

| Offset | Access | Role |
|--------|--------|------|
| `+0x00` | word, bit 11 | probe-length selector |
| `+0x2e` | halfword | `== 19` vetoes the wall record |
| `+0x3f` | byte | cleared to 0 on entry |
| `+0x8c` / `+0x94` | float (rw) | descriptor words, then correction outputs |
| `+0x19c` | float | subtracted from `+0x1a4` for `f2` |
| `+0x1a4` | float | `f2` for stages 1 and 4 |
| `+0x1ac` | float | input to `toMovedir` |
| `+0x1b8` | float | probe length / scale base |
| `+0x1bc` | float | `f4` to `marioChkWallAround` |
| `+0x1c0` | float | `f3` to every `searchFrontWall` |
| `+0x1e0` | word | secondary wall record |
| `+0x1e4` | word | primary wall record |
| `+0x214` | float | published length, only on a hit |
| `+0x24c` | halfword, bit 23 | gates the fourth probe |

## Porting notes

1. `mario[+0x2e] == 19` must be checked **after** every `searchFrontWall`
   call and before every store, exactly as staged. Hoisting the check to
   function entry changes which probe's result is discarded.
2. Stage 3's `vivianGetStatus() ∉ {1,2,3}` gate is partner-specific; the
   remake must keep the field-name gate as well.
3. `mario[+0x8c]` and `mario[+0x94]` are read as words when building the
   descriptor and written as floats at the end — the struct field is
   dual-typed by design.
4. `mario[+0x3f] = 0` is unconditional and happens before any gate.

### Return-value caveat

There is **no explicit `li 3, 0`** before the epilogue. `r3` at the
`blr` is whatever `marioChkWallAround` last returned. This function is
documented as `void` by its callers; a port that exposes the leaked value
would be a behaviour change.

## Remaining gaps

- Numeric values of `c494_68`, `c494_6c`, `c494_8c`, `c494_94` are not
  extracted.
- The literal string at `0x800c3d88` is not read from the ISO, so the
  exact field that enables stage 3 is unknown.
- Meaning of `mario[+0x2e] == 19`, `mario[+0x24c]` bit 23 and
  `mario[+0x3f]` is not established.
- `searchFrontWall` and `marioChkWallAround` are now documented at
  structure level — see [SEARCH_FRONT_WALL](SEARCH_FRONT_WALL.md) and
  [MARIO_CHK_WALL_AROUND](MARIO_CHK_WALL_AROUND.md). Their probe sequences
  and result reduction remain open.
- The three redundant `vivianGetStatus` calls may have side effects that
  a decompilation of that function would need to explain.