## 2026-10-14 — Original weapon rows, eligibility and party domain

Committed `3b5c7bd`: shared complete local weapon-table builder, target-count
wrapper and eligibility updater. It emits original Jump/Hammer/badge/inventory/
Special rows and delegates party key1 through its real callback/count-address
protocol. Private cursor/count cells, partial descriptors, untouched padding,
late tutorial override, original name/icon order, empty/dummy rows, signed
levels/items/counts, wrapped cursor correction and fresh owner/pointer reads
follow source contracts. Unbound party/update/sample providers retain explicit
port boundaries; no fabricated labels, party rows or success values.

Native launch correction: Goombella uses party domain6/cursor4c8 instead of
Item domain2/cursor51c; Hammer uses its4bc cursor instead of Jump4b0. Physical
navigation/cancellation and original labels/icons stay unchanged. Live Headbonk
checks domain6 and source attack-class count16f55. Generated source tables still
need real native providers and tutorial menu/input integration; this library
is not a completed tutorial menu or gameplay substitute.

Validation: shared make test passed224/224 C++ and138/138 Python tests. Final
focused builder/trio tests passed2/2 after a store-order fix and mutation fixture;
native make all passed/rebuilt 3DSX. Existing read-only ISO verifiers passed
10 bodies/52 data spans/894 builder anchors and20 menu/target bodies/344 anchors.
Tests cover private count mutation, empty/negative cursor preservation, signed
inventory, live levels/captured Special mask, late replacement presentation,
item-pointer read after ordinal/ID stores, captured eligibility count and raw
side addressing into a private target buffer. No Azahar/Dolphin run; no new
game payload or conversion. Scoped commits preserve concurrent gauge/audio/
rendering work. A fixture callback return type was corrected before final checks.
A concurrent subsystem folder migration began after validation (battle files
now under source/battle and include/battle); its combined build is not rechecked
by this step. Preserve that reorganization and use current paths next session.

Next: real source table/party/cost/sampling/message providers in native runtime;
original tutorial class/submenu/input/selection, player action and turn ownership;
source camera, window/effect/audio/audience consumers and physical alternatives
to optional lower-screen audience taps. Full tutorial/battle goal stays active.

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
host library. `python3 tools/check_doc_links.py` checked 1126 Markdown files
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

At the latest documented handoff, native builds and host tests passed; the
most recent exact results are listed under Validation below. This workspace may
contain newer uncommitted feature work, which should be validated by its owner.

- The source-based opening reaches the original arrival checkpoint. Original US
  messages, actors, music selections and assets are retained where bound; full
  renderer/audio parity is unverified.
- The battle harness has original trio attack selection/execution, damage and
  reward work, plus the recent source state6 registration, status-window
  arbitration, slot initialization and HP gauge/display prefix.
- The latest battle boundary is surviving-unit display and pose admission.
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

The newest archived implementation entry reports `make test` passing 222/222
CTest and 138/138 Python tests, and `make all` passing. That validation was for
the recorded HP-gauge/display change at that point in the shared tree. It is not
a claim that the current uncommitted combined checkout has been rebuilt or that
hardware behavior is verified. Check the dated archive before relying on older
counts or emulator captures.

This documentation cleanup changes no runtime code or asset conversion.

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
Concurrent subsystem migration began after the isolated review builds; its
combined new directory layout was not validated by these audio checks.
