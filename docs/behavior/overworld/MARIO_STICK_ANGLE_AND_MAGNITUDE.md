# Mario stick query: world angle and 70-capped magnitude

US G8ME01 disc0 revision0; static contract, no execution, cadence or original
gameplay claim. `marioGetStick` at `0x80095af8` (`mario_sbr.o`, size `0x1b8`;
listing `artifacts/reference/getStick.disasm`, 116 lines). The pinned snapshot
`decomp-62131fc31866f85a12e4817a8dc37f0c25eb5f8c` has **no** definition for it
(rechecked with `tools/reference_source_gap.py`, which pairs listing names and
canonical `us_symbols.csv` symbols with snapshot definitions).

Partial related coverage (not duplicated here): the heading-offset producer and
the movement rate that consumes `+0x198` are in
[Directional movement rate](OVERWORLD_DIRECTION_MOVEMENT_RATE.md); the
magnitude field is listed in
[Ordinary overworld movement](OVERWORLD_MARIO_MOVEMENT.md). This document is the
first complete contract for the query body itself.

## Argument ABI and outputs

Two pointers, no null check, both always written:

| Arg | Register | Writes | Field |
| --- | --- | --- | --- |
| `out0` | `r3` → `r29` | world-axes stick angle | `Mario+0x198` (current stick angle) |
| `out1` | `r4` → `r30` | stick magnitude | `Mario+0x194` (current stick magnitude) |

The single direct DOL caller is `marioMain` at `0x8005abe8`
(`marioMain+0x408`): `addi 3,30,408` (`0x198`) and `addi 4,30,404` (`0x194`).

## Complete read/store order

1. `marioGetPtr` (`0x8005e0c0`) into `r31`.
2. `flags = word [Mario+0]`. Test `rlwinm. 0,3,0,26,26` → mask `0x0020`
   (`MARIO_FLAG_FORCED_MOVEMENT`). If set: `*out0 = float [Mario+420]`
   (`+0x1A4`, retained travel heading) and `*out1 = float [Mario+384]`
   (`+0x180`, base speed), then `blr`. Forced movement therefore replaces the
   stick pair with retained travel/base speed; nothing else runs.
3. Otherwise `x = extsb [Mario+594]` (`+0x252`, `wStickDir1`) into `r27`.
4. Vertical source selection:
   - test `rlwinm. 0,3,0,6,6` → mask `0x02000000` (`MARIO_FLAG_BG_MODE`).
   - `invertY = 1` when BG_MODE is clear, or when BG_MODE is set and
     `kpaGetStageViewType` (`0x801feeb8`, koopa.o) returns 0;
     `invertY = 0` when BG_MODE is set and that call returns nonzero.
   - `y = invertY ? (s8)[Mario+595]` (`+0x253`, `wStickDir2`) : 0`. The
     negation at `80095b7c` and the later `neg 28,28` at `80095b9c` cancel, so
     the loaded byte is used unchanged.
   - If `yoshiGetStatus` (`0x800b6e94`, party_yoshi.o) returns 2, `y = 0`.
5. Both axes are converted to binary64 with the `0x43300000`-bias double at
   `0x802C3EC0` (`fsub` bias subtraction), producing `f3 = (double)x`,
   `f4 = (double)y`.
6. `distABf` (`0x8000722c`, system.o) is called with `f1 = 0.0`, `f2 = 0.0`,
   `f3 = x`, `f4 = y`, i.e. the planar distance from the origin; `f31` receives
   its result. `f0 = 70.0` (constant at `0x80420A64`, word `428C0000`, read
   with displacement `r2−27100`). `fcmpo 0,31,0` compares the distance with
   `f0`, and `cror 2,1,2` folds GT into CR0.EQ so that EQ means
   "distance ≥ 70.0"; the `bf 2` skips the following `fmr 31,0` only when the
   distance is smaller, so **magnitude = min(distance, 70.0)**.
7. `angleABf` (`0x8000703c`, system.o) with the same four doubles returns the
   bearing in `f1`.
8. Zero-magnitude branch: the `mfcr`/`rlwinm 0,0,3,31,31`/`xori`/`cntlzw`/
   `rlwinm. 0,0,27,5,31` sequence at `80095c34–c48` leaves CR0.EQ clear only
   when `dist == 0.0`, so the `lfs 1, [Mario+416]` at `80095c50` executes in
   that case: **`angle = [Mario+0x1A0]` (current view direction) when the stick
   is centered**, otherwise the computed bearing.
9. `f1 = angle + float [Mario+412]` (`+0x19C`, camera-owned heading offset
   published by `marioPreCamera`, see the movement-rate contract), then the two
   wrap loops `while (f1 < 0.0f) f1 += 360.0f` and
   `while (f1 > 0.0f) f1 -= 360.0f` (constants at `0x80420A68`, word
   `43B40000`; the second loop's GT test is the `cror 2,1,2`-assembled ≥).
   This is the same shape as the snapshot's `revise360`.
10. `*out0 = f1`, `*out1 = f31`; epilogue.

## Field and constant register

| Address | Word | Meaning |
| --- | --- | --- |
| `0x80420A60` | `00000000` | 0.0, distance/angle seed and loop reference |
| `0x80420A64` | `428C0000` | 70.0, magnitude cap |
| `0x80420A68` | `43B40000` | 360.0, wrap modulus |

r2 base `0x80427440` is set by startup `0x80005418/1c`; r13 base
`0x8041cf20` by `0x80005420/24`.

## Not established

- Cadence: the caller asks once per `marioMain` pass; no frame/tick claim.
- `kpaGetStageViewType` / `yoshiGetStatus` bodies are separate modules.
- Whether the BG_MODE or riding-Yoshi Y-drop has a visible gameplay effect is
  not claimed here.
- `Mario+0x1A4` is the retained travel heading per existing prose; the
  forced-movement path copies it unchanged and does not re-wrap it.
