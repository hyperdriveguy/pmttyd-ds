# Additional optimization implementation — 2026-10-14

This implements three high-priority paths from the
[further audit](FURTHER_OPTIMIZATION_AUDIT.md), plus bounded book-cover allocation
cleanup. It preserves authored draw order, simulation/action-command timing,
original assets, dialogue conditions and native resource synchronization.

## Changes

- **Actor metadata:** `Renderer::fieldActors` references immutable catalog
  records, rejects model/story/visibility mismatches first, then copies only
  mutable position/yaw/pose presentation fields. Talk-rule vectors, wandering
  data and other strings remain catalog-owned. Live NPC, scripted professor,
  save-object pose, candidate facing and pose-clock override order is retained.
- **Book composition:** a five-element fixed array replaces the temporary
  vector. Cover/page/illustration/spine/back ordering and source face/blend
  predicates remain unchanged. This removes the small per-frame allocation.
- **Single-bin surfaces:** if both axes select exactly one bin and all three
  original UVs satisfy its four planes, skip polygon clipping and append the
  same three vertices using the existing normalization/atlas arithmetic.
  Ordered comparisons keep NaN UVs on the old path. Bin validation, output
  ceilings, colors, vertex order and fallback partial-output behavior remain.
- **House audio:** an eight-bit mask retains ownership of channels actually
  queued by this player, including DONE voices. Cleanup clears only those
  channels and retires the mask; repeated idle/muted updates issue no clears.
  PCM remains loaded for re-entry. Request cursor/epoch handling, failed
  admission, voice reuse/limits and per-play setup are unchanged.

No assembly, fast-math, simulation-rate changes, geometry approximation or new
threading was needed. The dominant changes remove repeated work rather than
alter numerical operations. Existing ARM build target and compiler flags remain.

## Evidence

The frozen pre-change clipping implementation lives in
`tests/actor_surface_reference.hpp`. Differential tests compare return values,
complete appended vertex/color bytes and unchanged prefixes across wrap modes,
UV boundaries/signed zero, NaNs/infinities, invalid cells/wraps, output limits,
fallback partial overflow and 12,000 randomized cases. Original pack probing
checks 221,707 triangle appends across 40 packs, including loader validation
and first-pose frames 0/17/45: no mismatches. Original-save-block surface and
animation tests also pass.

A host `-O2` synthetic microbenchmark reuses vectors and alternates execution
order over five samples of one million eligible appends. Median original
184.620 ms versus optimized 48.7615 ms gives 3.79× throughput for that kernel.
It does not establish whole-game or physical New 2DS XL performance.
Sources/results: `artifacts/optimization-pass2/surface-clip-bench.*` and
`surface-original-all-probe.log`.

Current catalog loop counts (`actor-copy-probe.log`) show the old east-room
loop copied 28 full records, including 16 nonempty talk-rule vectors and 120
rule elements, per render invocation; plaza copied 14/8/60 respectively. The
new loop copies none of those full records/vectors. These are static catalog
counts, not measured allocator traffic or guaranteed frame rates.

The host NDSP stub executes the actual house player with synthetic two-sample
PCM files and checks sample bytes, rates/mix, allocation failure, DONE reuse,
eight-voice overflow, repeated mute/idle, re-entry, epoch restart, cursor rewind,
retained samples and repeated shutdown. Strict compilation and ASan/UBSan pass;
LeakSanitizer was disabled because this environment uses ptrace. Integrated
CTest uses the isolated stub include directory, never replacing native headers.

## Remaining paths

Next measure whole-loop frame percentiles, native allocation counts and audio
mutex wait times. Then ordinary checkpoint save-worker integration, cold PCM
pre-admission, immutable actor-time caching, collision-query workspace reuse
and transactional music admission remain candidates. Do not relax floating
point arithmetic or GPU pointer lifetimes to reduce a benchmark number.
No original asset format/conversion choices changed in this pass; existing
private pack provenance remains in [ASSETS.md](../ASSETS.md).

## Final validation and handoff

- `TMPDIR="$PWD/artifacts/optimization-pass2/tmp" make test` passed **224/224
  CTest and 138/138 Python tests** (`test-final.log`). Earlier attempts caught
  a missing test source being created by another session, then its callback
  returning `int` for a `uint32_t` function pointer. The required explicit
  return annotation was added to that session's test working file; it was not
  swept into these commits. Full rerun passed.
- `make all` passed (`all.log`). Implementation commits are `532203b` (actor
  metadata/book workspace), `bd5b111` (surface path/oracle) and `fbd726a` (audio
  ownership/stub test). Separate Git indices excluded concurrent staged work.
- Immutable native snapshot `artifacts/optimization-pass2/snapshot/ttyd-2ds.3dsx`
  SHA256: `f46e8b552ee45854c0a53a30cd61124d6d1d4b682d902c7806f0ebb8c7216f6a`.
  Shared native/host builds included ongoing unrelated feature edits; the
  retained binary identifies the exact emulator subject.
- Isolated fresh opening route passed through house/mail/map/voyage/arrival to
  the expected 1536-byte room1/storyKnown1/story3 checkpoint. Final harbor and
  intermediate house/map captures reviewed. Profile/captures:
  `artifacts/azahar-optimization-pass2-opening`; log `opening-smoke.log`.
- A separate isolated normal-frontend route loaded the legacy east/story8
  fixture and ran `tests/azahar_field_actor_talk.py`: original Mouser window
  opened, talk onset did not save, dialogue returned to field and explicit save
  retained room2/story8 with valid checksum. Reviewed dialogue and return
  captures. Profile/captures: `artifacts/azahar-optimization-pass2-field`; route
  wrapper under pass2 artifacts and result in `field-smoke.log`.
- Existing local DSP firmware was copied only into the isolated test SD roots.
  Neither route observed a native exception. No audible-fidelity, complete
  original MusyX or physical hardware performance claim follows from this.
  User emulator profiles/saves and ISO were unchanged. No new private asset
  payload or firmware was committed. Scoped diff and report-link checks passed.

The selected optimization scopes are complete. Further audit paths remain
separate work; the full remake is unfinished. Hardware before/after percentiles
are still required to quantify stutter reduction.
