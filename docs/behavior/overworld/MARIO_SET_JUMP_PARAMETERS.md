# Mario jump parameter installation

US G8ME01 disc0 revision0; static contract, no execution or gameplay claim.
`marioSetJumpPara` at `0x800941bc` (`mario_motion.o`, size `0x1ec`; listing
`artifacts/reference/setJump.disasm`, 129 lines). The pinned snapshot has **no**
definition for it (see [reference/source gap](#source-coverage)). Existing
prose only records one call site in
[Airborne and landing](OVERWORLD_AIRBORNE_AND_LANDING.md).

## Effect

Writes the four consecutive single-precision slots at
`Mario+0x7c / +0x80 / +0x84 / +0x88` (unnamed in the pinned MarioWork header)
from a motion-indexed jump table, then conditionally runs the Koopa
jump-parameter leaf. No value is returned (void body, `blr` epilogue).

## Dispatch

`motion = lhz [Mario+46]` (`+0x2E`, `currMotionId`). The code computes
`index = motion − 3` and, when `0 ≤ index ≤ 22`, jumps through the 23-entry
table at `0x80310250` (`lis 0x8031` + `addi 592`). All other motions fall
through to the shared tail.

| Motion | Name | Handler |
| --- | --- | --- |
| 3 | `MARIO_MOTION_JUMP` | flag-gated table-B block (`0x8009421c`) |
| 4 | `MARIO_MOTION_JUMP_NPC` | fixed tuple (`0x80094308`) |
| 5 | `MARIO_MOTION_JUMP_SW` | table A (`0x800941f4`) |
| 6, 7, 8, 9, 10, 11, 13…19, 21, 23 | all others in range | tail only (`0x8009437c`) |
| 12 | `MARIO_MOTION_UPSTAIRS` | flag-gated table-B block |
| 20 | `MARIO_MOTION_DAMAGE_TOGE` | flag-gated table-B block |
| 22 | `MARIO_MOTION_FORCE_RESET` | table B then gravity override (`0x8009432c`) |
| 24 | `MARIO_MOTION_PARTY_USE` | flag-gated table-B block |
| 25 | `MARIO_MOTION_KPA_SWIM` | flag-gated table-B block |

## Tuples

Table A at `0x80416208` (`lis 0x8041` + `lfsu 25096`): `(2.5, -0.1, 0.02, -0.01)`.
Table B at `0x80416110` (`addi r13, -28176`): `(8.0, -0.84, 0.08, -0.02)`.

| Route | Stored `+0x7c / +0x80 / +0x84 / +0x88` |
| --- | --- |
| Table A (motion 5) | `(2.5, -0.1, 0.02, -0.01)` → tail |
| Motion 4 | `(1.0, -0.039, -0.01, -0.003)` → tail |
| Flag-gated block, no scale | table B, `(8.0, -0.84, 0.08, -0.02)` |
| Flag-gated block, scaled | `0.5 ×` table B, `(4.0, -0.42, 0.04, -0.01)` |
| Gravity override (flag-gated) | `(2.0, -0.003, 0.029, -0.003)` |
| Motion 22 after table B | `(2.7, -0.002, 0.016, -0.002)` |

## Flag-gated block (motions 3, 12, 20, 24, 25)

1. `trigFlags = word [Mario+12]` (`+0x0C`). Test `rlwinm. 0,0,0,15,15` → mask
   `0x00010000` (`MARIO_TRIG_FLAG_SET_JUMP_PARAMS`).
2. If **clear**, test `flags` bit `rlwinm. 0,0,0,19,19` → mask `0x00001000`
   (`MARIO_FLAG_PRE_BATTLE_PHYSICS`). If that is **set**, jump straight to the
   shared tail and leave the tuples untouched.
3. Otherwise: when `SET_JUMP_PARAMS` is set, store table B unscaled if
   `PRE_BATTLE_PHYSICS` is set and `0.5 ×` table B otherwise; when
   `SET_JUMP_PARAMS` is clear, store `0.5 ×` table B. Both routes then run the
   gravity override.
4. Gravity override: compare `float [Mario+552]` (`+0x228`, `playerGravity`)
   with `1.0` (constant `0x804209B4`); on inequality store
   `(2.0, -0.003, 0.029, -0.003)`.

Constants: `2.0` at `0x80420A44`, `-0.003` at `0x80420A48`, `0.029` at
`0x80420A4C`, `-0.039` at `0x80420A50`, `-0.01` at `0x804209D4`, `0.5` at
`0x80420A1C`, `2.7` at `0x80420A54`, `0.016` at `0x80420A58`, `-0.002` at
`0x804209D0`, `1.0` at `0x804209B4`.

## Shared tail (`0x8009437c`)

Reload `currMotionId`; if it differs from 8 (`MARIO_MOTION_JUMP_3`), load the
signed byte at `Mario+60` (`+0x3C`, `characterId`) and, when it equals 2, call
`kpaSetJumpPara` (`0x801fe638`, koopa.o) with the current Mario pointer. This
tail is also the default route for out-of-range motions, so the leaf can run
without any tuple store in this function.

## Callers (direct DOL `bl` census, 6 sites)

| Site | Caller |
| --- | --- |
| `0x8009b31c` | `mot_jumpSmall` |
| `0x8009b51c` | `mot_jump` |
| `0x8009b790` | `mot_jumpNpc` |
| `0x8009bba8` | `mot_jumpSw` |
| `0x8009bf58` | `mot_jumpStand` |
| `0x8009c2f0` | `marioPreJump` |

## Not established

Tuple slot meanings (initial/augment/velocity/accumulator roles), whether the
flags gate is also set by non-event writers, `kpaSetJumpPara` internals, and
per-call cadence. `MARIO_TRIG_FLAG_SET_JUMP_PARAMS`/`PRE_BATTLE_PHYSICS`
identities come from the pinned header enum, not from a US execution trace.
