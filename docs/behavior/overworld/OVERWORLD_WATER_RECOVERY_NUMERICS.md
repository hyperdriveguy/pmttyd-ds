# Water recovery numeric stages (mot_damage)

US G8ME01 disc0 revision0; static contract, no execution or gameplay claim.
Answers the numeric half of native field/AC-03 request 2 (mot_damage water
motion31 stages) in `docs/DOCUMENTATION_REQUESTS.md`. Pose ownership stays
in [recovery poses](OVERWORLD_MARIO_RECOVERY_POSES.md); only numbers,
counters, and transitions are closed here.

Source: `artifacts/reference/mot_damage.disasm` (entry 8013ae04, 517
lines). Stage = word [Mario+68], counter = word [Mario+72] (established by
the dispatch shape below, not by names). SDA constants read from the ISO:
−18656 = 0.0, −18632 = 1.0, −18628 = 10.0, −18624 = 500.0, −18620 = 8.0,
−18616 = 0.125, −18612 = −10.0, −18592 = −6.0, −18580 = 160.0,
−18576 = 0.3, −18572 = 20.0, −18568 = 50.0. Integer/float register files
are kept distinct throughout (an `lfs` never feeds a `stw` counter).

## Dispatch (ISO-read table 0x8036ED4C, index = stage, gate ≤ 32 unsigned)

0→8013af84, 1→8013affc, 10→8013b028, 11→8013b06c, 12→8013b350,
13→8013b3c4, 14→8013b3e0, 20→8013b438, 30→8013b544, 31→8013b554,
32→8013b5b8; all other stages → epilogue (no-op). The native port's
"stage15" has no slot here — unlisted stages are no-ops, so any stage-15
behavior in the port is authored, not original.

## Sink and entry (stages 0–1)

- Entry prologue: if [Mario+124] == 0.0, [124] = −6.0 and [128/132/136] =
  0.0 (sink-velocity init on a zeroed lane; nonzero lanes pass through
  untouched). A prior revision stated this guard backwards — corrected
  after the native session's independent re-read. SFX 404 path and
  bgmode-gated positioning follow per surrounding calls (not decoded
  here).
- Stage 0 (8013af84): [144] += ([696] − 160.0); if [144] ≤ 0 (NaN-safe
  `cror`) call 0x80094020; [124] += −6.0; [128] = [132] = [136] = 0.0.
- Stage-0 exit (in-line): [124] = 0.3, stage = 1, counter = 40.
- Stage 1 (8013affc): counter--; while > 0 return; on expiry stage = 0,
  [708] = 20.0 float. So the 40-tick hold the port attributes to "rise"
  is, in the original, stage 1's countdown back to stage 0.

## Rise/travel (stages 10/11, 12, 13, 14, 20)

Slot discipline (re-verified instruction by instruction): [68] = stage,
[72] = counter. An earlier revision mixed the two in several bullets below;
the corrected mapping is authoritative.

- Stage 10 (8013b028): counter++; on reaching exactly 2 → SFX 405,
  stage = 11, else straight to the rise core (8013b06c). Two-tick arm.
- Stage 11 core: [144] += [708]; [708] −= 1.0 (the 10.0-floor branch
  stores the identical value, so unconditional in ordered cases). [144]
  vs [696] NaN-safe compare selects the land-check branch: bgmode != 1 →
  vectored splash call (0x8022645c) then SFX 186; == 1 → SFX 186 only.
- Rise ceiling (8013b0fc, same stage): if [144] ≥ [696] + 500.0, the
  [520]-gated branch selects a position triple (via 0x80013ef0 or the
  map-name `strcmp` chain) into the stack slots, then the bit6-gated splash
  call; [704] = lane quotient via `fdivs` (the travel-rate computation —
  a measured divisor, not the port's 0.08).
- Stage 12 (8013b350): [240] += [704]; [140/148] snapshot to [236/244]
  after the 0x80005de4 call; counter-- → stage = 13 on expiry, plus
  [Mario+0] |= 0x80 and the 0x8009412c call. On that transition [72] is
  set to 1, so stage 13 runs exactly one tick.
- Stage 13 (8013b3c4): counter-- → stage = 20 on expiry.
- Stage 14 (8013b3e0): [240] += [704] again; snapshots again; counter-- →
  on expiry flag OR, 0x8009412c call, [Mario+4] low-bit clear, stage = 20.
- Stage 20 (8013b438): `marioGetFallSpd` body call (0x80094098, fall
  integrator), [520]-gated branch, then the `marioChkLandon` landing query
  (0x8008dc70, contracted) with the bgmode-gated K(−18568 = 50.0) compare
  selecting the hit-event path versus the [488]-parked miss path.

## Landing/HP chain (stages 30/31/32/33)

- Stage 30 (8013b544): counter = 50 (`li 3, 50` → [72]), stage = 31
  (`li 0, 31` → [68]). (Matches the poses doc's landing handoff modulo
  its "count48" notation.)
- Stage 31 (8013b554): counter--; while > 0 return; on expiry read live
  HP byte; `pouchDamage(-1)` call (0x800d3d3c) when the sign test passes;
  then `pouchGetHP`-style read (0x800d3d30): nonzero → stage 33 written to
  the counter slot... precisely `li 0, 33` → [68] (stage 33); zero → fall
  to the 32 path. **This is the HP-penalty tick**: −1 pouch damage applied
  exactly at stage-31 expiry, before the branch.
- Stage 32 (8013b5b8): counter-- → on expiry `marioChgMot(0)`
  (0x8009455c) then the shared tail call; the 12-count preset
  (`li 3, 12` → [72]) with counter slot 32 (`li 0, 32` → [68]) sets the
  12-tick window the poses doc observes (11 after immediate decrement).
- Stage 33: pose-table/cleanup tail (poses doc owns the M_D_2 call).

## Stage-12 counter provenance (closed)

The open "[72] preset" question is answered: inside the travel-setup region,
a lane float is truncated (`fctiwz`), stored, reloaded, and sign-extended to
halfword (`extsh`) before `stw 0, 72(31)` (8013b2a4–ac). So stage 12's
countdown is a computed travel-tick count, not a stale inheritance — the
earlier "stale from stage 10" hypothesis is withdrawn. Which lane feeds the
truncate sits among the [140/148]-derived position triple (not reduced
here), and the [72]==0 bypass (forcing counter 10 with a [704] recompute
and an r0-stale stage write) is reported as written, not rationalized.

## Not established

Float-chain interiors of stages 12/14/20 (exact lane roles), the 0x80005de4
/0x8009412c/0x80094020 callees, [520]-gate producer, +0x80/+4 flag meanings,
SFX-ID selection beyond the pinned IDs (404/405/186/203/407), travel
distance/0.08-style ratios (no 0.08-class constant appears in the SDA set —
the port's divisor is authored), jerk-rate staging (no jerk lane found in
this body; the port's −0.001/−0.002/+0.00024/−0.00004 cascade is authored),
and any timing beyond per-invocation countdown steps.
