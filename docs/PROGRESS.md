# Project progress and handoff

Updated 2026-10-14. The dated implementation record is preserved in
[PROGRESS_ARCHIVE.md](PROGRESS_ARCHIVE.md). This page is the concise current
handoff; consult the archive when exact historical validation or commit context
is needed.

## Documentation and repository organization

Added subsystem maps at `source/README.md`, `include/README.md`,
`tests/README.md`, and `tools/README.md`; they explain the existing filename
prefixes and connect code/test searches to behavior contracts. The runtime
source and public headers now live in subsystem directories while retaining
module basenames. CMake and devkitARM discovery include the grouped directories;
text references were updated to the current source paths. Tests and utilities
remain filename-grouped and their indexes document that convention. The local
generated asset tree is grouped by room/feature; its layout and privacy rules
are indexed in `docs/ASSETS.md`.

The dated asset ledger is preserved in `docs/ASSETS_ARCHIVE.md`; the current
`docs/ASSETS.md` is the short source/storage/license guide. The speculative
render proposal inbox is retired; `docs/OPTIMIZATION_REQUESTS.md` points to
completed evidence and the current unmeasured-candidate audit. The active
behavior research inbox remains `docs/DOCUMENTATION_REQUESTS.md`.

Validation: fresh `make BUILD=build-structure-check all` passed and produced the
native 3DSX; `cmake -S . -B /tmp/pm-port-structure-cmake
-DCMAKE_CXX_COMPILER=/usr/bin/g++` configured successfully and
`cmake --build /tmp/pm-port-structure-cmake --target ttyd_core -j4` built the
host library. `python3 tools/check_doc_links.py` checked 1130 Markdown files
with 0 broken links; `git diff --check` passed. A first build in the reused
default directory failed on stale dependency paths from the old layout; the
fresh build passed after Makefile directory discovery was anchored to TOPDIR.
`make test` was not run. No source behavior or asset conversions changed; build
preparation regenerated/reused ignored packs only.

## Current objective

Build the native New 2DS XL remake toward the D1 Rogueport opening demo and the
G1 full-game goal in [milestones](MILESTONES.md). The remake is unfinished.
The current native slice has an original-content opening through Rogueport
arrival and connected outdoor rooms, but the original Crump tutorial battle
still lacks complete action, display, teardown, field-return and persistence
ownership. Do not treat harness completion as original-game completion.

## Current state

The latest full host-test counts are preserved in the archive. This session's
combined native and host-library build results are recorded below; all
organization and follow-up changes are committed.

- The source-based opening reaches the original arrival checkpoint. Original US
  messages, actors, music selections and assets are retained where bound; full
  renderer/audio parity is unverified.
- The battle harness has original trio attack selection/execution, damage and
  reward work, source state6 registration, status-window arbitration, slot
  initialization, HP gauge/display prefix, original weapon rows and a partial
  tutorial weapon-menu builder. Tutorial input/selection and full encounter
  flow remain incomplete.
- The latest battle boundary is surviving-unit display and pose admission, with
  source default pose identity and non-overlapping native part records retained.
  Original field/battle pose-pool provisioning, remaining status/icon/part
  consumers, battle teardown, field restoration and save/result owners remain
  open. See [HP gauge/display scope](behavior/battle/NATIVE_HP_GAUGE_HISTORY_AND_DISPLAY_PREFIX.md)
  and the [US completion register](behavior/meta/US_COMPLETION_AND_PRIORITIES.md).
- The latest optimization pass implements actor metadata copy filtering,
  single-bin surface clipping equivalence, bounded book workspace and queued
  house-audio cleanup. Hardware performance remains unmeasured; see the
  [optimization audit](optimization/FURTHER_OPTIMIZATION_AUDIT.md).

## Next work

1. Complete original pose-pool/resource admission and the live per-unit,
   queued-display and status/icon/part consumers.
2. Bind ordered battle teardown and field return, then the real result and save
   owners required by D1.
3. Continue the original tutorial menu/input/action sequence and validate the
   complete opening chain without development dialogue or injected outcomes.
4. Profile whole-loop performance on New 2DS XL before making hardware claims.

The active research backlog is indexed in
[DOCUMENTATION_REQUESTS.md](DOCUMENTATION_REQUESTS.md). Recovered subsystem
contracts are indexed in [behavior/README.md](behavior/README.md). Asset source,
conversion, and licensing notes remain in [ASSETS.md](ASSETS.md).

## Latest recorded code validation

The combined reorganized checkout passed `make BUILD=build-structure-check all`,
CMake configure and `ttyd_core` build, the 1130-file Markdown link audit, and
`git diff --check`. `make test` was not run this session. The most recent full
test counts remain the dated results in the archive; they predate this combined
reorganization. No hardware behavior is claimed. Check the archive before
relying on older test counts or emulator captures.

The repository reorganization preserves runtime behavior. Separate feature
commits in this session add the opening-book layout correction, battle part and
pose identity handling, and tutorial weapon-menu construction; none is a claim
of full original battle completion.

## 2026-10-14 — Item-get audio providers

Committed original primary/secondary BGM envelopes, detached fanfare tail and
retained field-music resume (`1a4a5ee`, `ec3ee1f`). Both steps passed isolated
211 CTests/135 Python tests and native builds. Original-mode Azahar confirmed
logical vacancy before tail release and cursor90112→90112 across NULLresume.
The final capture observed Azahar57FPS/native36FPS in its interval; physical
performance and steady integration throughput remain unverified. Native pickup
print/entry/item-main/control/windows/pouch/5405 integration is next.
[Exact validation, artifacts, limitations and next work](behavior/audio/NATIVE_ITEM_GET_AUDIO_VALIDATION_2026_10_14.md)
retain the detailed record while this handoff/archive reorganization proceeds.
The combined reorganized checkout was subsequently rebuilt in this session; the
audio measurements above remain the earlier isolated results.

## Session commits (2026-10-14)

Repository organization is committed in `8e70c746`. Separate follow-up commits
cover the opening-book layout (`b97fa9c6`), battle-part/pose identity
(`83402d8a`), CMake registrations (`b3455044`), message-asset order
(`7f5245f6`), tutorial weapon menu and pouch import (`452a29cd`), recovered
behavior contracts (`9390b332`), generated-build ignores (`b43651bf`), and the
renderer notice (`de2dd009`). Earlier independent agent changes are preserved
in their own commits.
