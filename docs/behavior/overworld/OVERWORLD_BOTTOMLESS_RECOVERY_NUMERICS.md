# Bottomless recovery numerics (motion33/34 placement and fall)

US G8ME01 disc0 revision0; static contract, no execution or gameplay claim.
Answers the mot_bottomless half of native field/AC-03 request 2
(`docs/DOCUMENTATION_REQUESTS.md`). Pose ownership stays in [recovery
poses](OVERWORLD_MARIO_RECOVERY_POSES.md); only numbers, counters, and
transitions are closed here.

Source: `artifacts/reference/mot_bottomless.disasm` (entry 80091904, 334
lines). Stage = word [Mario+68] (dispatch gate ≤ 12 unsigned), counter =
word [Mario+72]. SDA constants read from the ISO: −27296 = 0.0,
−27292 = −2000.0 (loaded once at entry, dead — f0 never reaches a store),
−27288 = 250.0, −27284 = 13.0, −27280 = 220.0, −27276 = 1.0,
−27272 = 100.0, −27268 = 120.0, −27264 = 300.0, −27260 = 10.0,
−27256 = 8.0, −27252 = 0.34, −27248 = −0.002, −27244 = −0.01,
−27240 = −10.0.

Callees (all `us-symbols` rows): `marioGetPtr`, `marioGetScreenPos`,
`marioChkInScreen`, `movePos`, `marioSetFallPara`, `marioUpdateCamPos`,
`marioNoUpdateCamPos`, `marioChgPose`, `marioChgEvtPose`, `distABf`,
`system_angleABf`, `marioChkLandon` (0x8008dc70, contracted in [landing
leaves](OVERWORLD_SPEED_FALL_LANDING_LEAVES.md)), the fallSpeed-shape inline
integrator, `psndSFXOn` (IDs 185/407 on the land path).

## First-tick init versus per-tick dispatch

- If [Mario+12] low bit is clear → straight to the stage dispatch (stage
  preserved across ticks).
- If set: clear it, [384] = 0.0, stage([68]) = 0, then map-name lane
  selection: `strcmp(currentMapName, …)` against win_03 / win_01 / tik_01
  (read from rodata) picks float pairs (250.0 + lane, 13.0 + lane,
  220.0 + lane) into the working lanes; miss uses [224]/[232].
- Angle setup: `distABf`/`system_angleABf` over the lanes → [712]; the
  −2000.0 SDA load in this block is dead.

## Dispatch (ISO-read table 0x8031021C, index = stage)

0→80091b38, 2→80091bf8, 10→80091cf4, 11→80091d04, 12→80091d4c; 1 and 3–9 go
to the epilogue (no-ops). Stages above 12 hit the unsigned gate and also do
nothing — stage 13 (written by the HP path below) is terminal by gate.

## Stage 0 (swim hold) and stage 2 (fall + land)

- Stage 0: [708]/[712]-driven `movePos` swim step, [140/148] snapshots to
  [236/244], counter-- → on expiry stage = 2 with flag ORs, the
  [Mario+46]-indexed indirect callback, and the tail call.
- Stage 2: inline fall integrator identical in shape to the `fallSpeed`
  body (same SDA clamp slot −27296 = 0.0): [132] += … clamped, [128] +=,
  [124] +=, table+36 snap. Then f30 = min([124], −10.0) and the
  `marioChkLandon(f30, …)` query: nonzero → [144] += result and park it at
  [488]; [488] != 0 → counter = 50, stage = 11. Zero-result path: SFX 185
  then 407, fallthrough.
- Drop-back lanes: when [Mario+552] mismatches the angle residue,
  [124]/[128]/[132]/[136] = 0.34 / −0.002 / −0.01 / 0.0. The port's authored
  −0.001/−0.002/+0.00024/−0.00004 cascade matches only the −0.002 lane;
  replace the other three with these readings.
- Default lanes (r4-sourced): before that test, [124/128/132/136] load from
  [r4+0/4/8/12] where r4 = [r13−28176]+16 when [Mario+46] == 5, else the
  absolute table 0x80416208+16. The mismatch triple overwrites them; on a
  match they survive. Table contents are not read out here.

## Stages 10/11/12 and the HP path

- Stage 10: counter-- → on expiry motion dispatch on halfword [Mario+48]
  (16/17/else shapes with flag updates and the 0x80310030 + [46]·12
  indirect callback table), then the shared tail.
- Stage 11: counter-- → HP path: `pouchDamage(−1)` + live-HP read; nonzero
  → stage = 13 (terminal by gate); zero → message + counter = 12,
  stage = 12.
- Stage 12: counter-- (12 ticks) → the same motion dispatch as stage 10.

## Native-constant reconciliation

Replace: sink/rise/arc velocities, travel step, drop-back jerk cascade
(only −0.002 survives), and any stage-15 handling (no slot exists).
Keep: the countdown-driven stage shapes, the −10.0 snap comparison, and the
50/11 landing handoff. The +0x80/+4 flag bits, [520]-gate producer, SFX-ID
selection rationale, and [46]-table ownership remain open, as do float-chain
interiors beyond the pinned lanes.

## Not established

Lane/flag/table meanings, cadence beyond per-invocation steps, camera/pose
internals, and any gameplay effect.
