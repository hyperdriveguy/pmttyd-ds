# JUMP-01 — Ordinary Jump follow-up and per-hit event flow

**Status: OPEN (partially answered).** Part of the [request inbox](../DOCUMENTATION_REQUESTS.md); validated deliveries and locks are registered in the [delivery log](DELIVERY_LOG.md).
Request text below is preserved verbatim from the inbox; answers and implementation breadcrumbs are append-only history. Updated 2026-10-12.

## Open todos

- [ ] Complete ordinary Jump event `0x80355cf0` control flow: all
  SetParam/flag/difficulty/start/result/get-result/stop branches, per-hit
  state-bit writers, precheck and damage flags, subsequent-hit
  conditions/count/target-alive gates, inline/child motion and wait
  ownership, bonus/stylish/feedback declaration and cleanup, linked to the
  actual damage/result invocations. Retain raw event operands and addresses;
  provide original timing/state traces separately and do **not** insert the
  harness's impact48/return42 schedule or one-hit policy into the contract.
- [x] Packed-Jump SFX provenance (source-audit request 1) — answered
  ([Jump SFX provenance](../behavior/battle/BATTLE_JUMP_SFX_PROVENANCE.md));
  native follow-ups (spurious `0x802ee470` reads, angleABf/distABf numeric
  providers, both `psndSFXOn_3D` records) are recorded in the history file.
- [ ] Field recovery numeric stages (source-audit request 2) — moved to
  [FIELD-RECOVERY-01.md](FIELD-RECOVERY-01.md).
- [ ] Recovery pose-table domains and pipe paper lifecycle (source-audit
  request 3) — moved to [FIELD-RECOVERY-01.md](FIELD-RECOVERY-01.md).
- [ ] `marioMain` per-retrace body (P0c main-loop/field-dispatch audit) —
  moved to [MAINLOOP-01.md](MAINLOOP-01.md).
- [ ] Original `cardMain`/`swSet` persistence (P1b save/persistence audit) —
  moved to [SAVE-01.md](SAVE-01.md).

Original pouch initialization, upgrade/load/save and bonus-byte writers still
need binding; portable service tests are not original evidence.

## Request text (verbatim)

Please recover the complete ordinary Jump event0x80355cf0 control flow:all
SetParam/flag/difficulty/start/result/get-result/stop branches,per-hit state-bit
writers,precheck and damage flags,subsequent-hit conditions/count/target-alive
gates,inline/child motion and wait ownership,bonus/stylish/feedback declaration
and cleanup. Link existing motion callbacks and original interpreter/pad cadence
to the actual damage/result invocations. Retain raw event operands and addresses.
Provide original timing/state traces separately;do not insert the harness's
impact48/return42 schedule or its temporary one-hit policy into the contract.
The portable services test invalid signed levels via bounded providers;original
pouch initialization,upgrade/load/save and bonus-byte writers still need binding.

Native implementation source audit / documentation request (2026-10-12),JUMP-01:
reviewed the recent native commits that shipped without leaving requests
(b79bbe2 packed Mario Jump services,81c143d water/void/pipe recovery poses,
778add7 idle/low-HP pose scheduling,e1257d8/357a656 field shadows). Local
function bodies are recovered;the asks below target original provenance the
port still lacks. Commit79bbe2 binds packedMarioJumpPositionEvent801093a8 to
native event/unit/memory with explicit single/double/trig providers;mode1
eligibility falls back to packedACPossibility(unit+0xc,kind223,statuses1/2/9/27,
unit+0x138 mask1),which BATTLE_DAMAGE_ORCHESTRATION already covers,and the
motion/numerics/sound-mode local bodies are closed in
BATTLE_MARIO_JUMP_MOTION/NUMERICS/BATTLE_UNIT_SOUND_MODE. Still absent and
requested:1) the three-word block the packed body loads at0x802ee470/474/478
immediately before its SFX request — original contents/roles and whether it is
the standard attack-sound key row supplied to LoadSeMode-type2 sources;
2) the concrete original jump sound-key domain stored at unit+18c by the
sound-mode sources,and the psndSFXOn_3D invocation record for this callback:
point-in-time (same setup invocation as the phase switch or later),the copied
baseline position pointer (unit+154-15c here),return-value handling and whether
any landing/terminal cue follows;3) identity and FPSCR entry state of the Gekko
PS-family sin/cos called by801093a8 — MJP-NUM-04 already requests differential
bits;the symbol/address and rounding mode are the remaining doc ask. The native
port fires play once at setup with baseline154-15c and performs the global reads
without writes;no original audio output/cadence claim exists. No lock reserved;
existing spatial/psndSFX/audio requests stay open. This entry adds no native/
test/CMake change (docs-only;latest recorded host state87 CTest/90 Python and
make all passing).

## Answers and delivery notes

The append-only delivery notes, implementation breadcrumbs and host-gate history for this request live in [JUMP-01_HISTORY.md](JUMP-01_HISTORY.md). Validated deliveries are also registered in the [delivery log](DELIVERY_LOG.md).
