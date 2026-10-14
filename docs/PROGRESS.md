## 2026-10-14 — Goomba3 portable parcel acquisition

Committed `3b71d849`. Original quest2 acquisition now has a host-testable
print → item entry/force → item service → cleanup → later5405 owner using the
real pouch and registry. Both windows precede the once-only grant; full key
storage stays pending. Duplicate source-name lookup retains initial XYZ and
cleanup deletes all matching entries. The owner is compiled into the native
program; ordinary Game/NPC acquisition and presentation are still unbound.

Validation: isolated review tree41bc4a45 passed `make test`225/225 C++ and141/141
Python tests and `make all BUILD=build-goomba-acquisition-review
TARGET=ttyd-goomba-acquisition-review`. Shared working-tree test failed the
separate trio_attack_original pose assertion (227/228 passed). No new gameplay
render capture or physical performance claim. [Exact validation and artifact](behavior/field/GOOMBA3_PORTABLE_ACQUISITION_OWNER.md).

Next: bind stable Game-owned pouch/switch/talk/registry providers; preserve
source event→Mario→NPC→item→sound→window→effect order and next-Mario-owner
motion15 initialization; connect original camera5 icon, M_I_U, effect/window
and existing native audio/control services; capture acquisition in an isolated
Azahar profile and measure rendering. Full-storage UI and other areas remain
open. The full-game goal remains active.

## 2026-10-14 — Original weapon decision and live tutorial targets

Committed `16baaf3d`: complete shared original setup80122068/6c and decision
80123210/150 wrappers, with typed port service readiness. Decision captures the
actor before rebuild, uses current actor for sampling, preserves signed owner/
direction semantics, writes the live target buffer, resets selection after
sampling, and implements current-count/flags/permutation preference scanning.
Setup preserves table/cursor/UI ordering and final flags OR. Neither body
publishes menu type, creates an attack, consumes resources or advances story.

The tutorial selectWeapon API now rebuilds its real original rows, invokes the
actual packed sampler and exposes target unit/part pairs. Normal Jump selects
Crump2/body1, with HP and roots unchanged. Unavailable rows/overlapping roots
are refused by the admission bridge. Class/submenu/target physical input, type
publication, action registration/timing/impact and turn ownership remain needed;
this target-state binding is not a complete native tutorial or displayed menu.

Validation: shared make test passed226/226 C++ and141/141 Python tests. Final
focused decision fixture passed after adding live count/flag mutation checks.
Native TMPDIR=artifacts/tutorial-admission-tmp make all
BUILD=build-tutorial-menu-layout TARGET=ttyd-2ds-menu-layout passed/rebuilt 3DSX.
Read-only US verifier passed15 bodies,234 anchors,2 spans and16 scoped calls.
Tests cover actor recapture, signed direction, selection reset, first-different/
last-same, live flags/count, permutation position, unresolved services and UI
flag mutation, plus real tutorial Crump target sampling. No Azahar/Dolphin or
menu-render/playability claim; no new game payload/conversion. Scoped commits
preserve concurrent field/render work.

Next: original class/submenu/target input and menu-type publication; source
selected event/root ownership; player command timing, impact and full turns;
original camera/window/effect/audio/audience presentation. Preserve original
art/menu style and physical controls, with optional lower-screen audience taps.
The full tutorial/battle goal remains active.

## 2026-10-14 — Tutorial native weapon providers and field-pouch entry

Implementation is committed in `452a29cd`; contract/asset record in `aff81380`.
The tutorial now binds the complete shared table/eligibility/private-target
helpers to original packed units/weapons, current pouch/equipped queries, icon
leaves, FP/AP costs, actual packed sampling and original US labels. Mapped label
storage copies text/control tokens unchanged and caches original key pointers.
The API exposes source descriptors only after intro/no active phase/attack/hit;
it does not confirm a command, create an attack or advance story/turn state.

The first live menu check exposed bare pouchInit level0, which correctly emits
no ordinary Jump row. Game tutorial entry now uses beginFromPouch when original
field storage exists, copying every entry byte without new-game grants/reset.
Original Mario resource import/revision carries HP/FP/base/max values before
his source init. Tests preserve HP7/FP2 and boots/hammer levels, enabled original
Normal Jump803538a8/icon167 and Hammer80353de8, disabled empty-item row/reason3,
empty locked Special domain, unchanged combat state and entry-copy independence.
Party NPC helper3 remains separate; tutorial party delegation is unbound. Legacy
storage-less saves keep the prior bare-pouch harness. Battle export, full gear
import and class/submenu/input/player action/turn ownership remain incomplete.

Validation: corrected shared make test passed224/224 C++ and141/141 Python tests.
The first Python pass failed a relocated map_visibility include path, then
passed after its correction. Default native make all hit stale pre-migration
shader dependency paths; isolated TMPDIR=artifacts/tutorial-admission-tmp make
all BUILD=build-tutorial-menu-layout TARGET=ttyd-2ds-menu-layout passed and built
ttyd-2ds-menu-layout.3dsx from the new subsystem layout. No Azahar/Dolphin run or
native menu visual/playability claim. No new game payload/conversion. Runtime
changes landed during concurrent commits; current source/history was inspected
and the required bindings/CMake source registration are present in HEAD.

Next: original tutorial class/submenu/input and decision, selected target and
source action registration, action timing/player impact/turns; camera/window/
effect/audio/audience consumers with original assets and physical input. Keep
lower-screen audience taps optional. The full tutorial/battle goal stays active.

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

## 2026-10-14 — Battle part isolation and original pose defaults

Feature commit `83402d8a` preserves three distinct original Mario battle parts
and Goombella's separate primary record. Original `M_Z_1`/`PKR_Z_1` defaults are
converted; private sidecars retain all85/32 source descriptor names and original
model/texture identity. See the [storage/directory contract](behavior/display/NATIVE_BATTLE_PART_STORAGE_AND_POSE_DIRECTORY.md).

Final combined `make test`:224/224 C++ and141/141 Python passed. Native
`make -o assets-ready all BUILD=build-pose-constructor TARGET=ttyd-pose-constructor -j4`
passed after the migration/menu fixes. Isolated immutable Azahar check reached
the original10 Star Point reward/victory and existing field-restoration hold;
no detected exception. Exact baseline, logs and limitations are recorded in
the [archive](PROGRESS_ARCHIVE.md#2026-10-14--battle-part-isolation-and-original-pose-defaults).

Next: retained real battle pose/resource/texture ownership and per-part entry,
then original selection/restart/rate/effect/release services. Display remains
pending at animPoseSetLocalTimeRate8004142c; teardown and field return remain
unbound. No completed-prologue or hardware-parity claim.

## 2026-10-14 — Book page capture alignment and native aspect

Corrected the observed book-page corruption/shift by replacing CPU framebuffer
readback/rotation/Morton repack with direct GPU render-to-texture. Each page
repeats the same old desktop/cover/page/illustration/retained-paper composition
into a256-square VRAM texture with an untilted crop projection matching native
110/0/193/240 (original124/0/386/480). Temporary target deletion after FrameEnd
waits for GPU completion before source captureComplete/background replacement.
The original372-vertex/124-triangle A_1 deformation, four independent owners,
source rune backs,180-tick waits and narration order remain intact.

Book cover/papers now use the entire400x240 top screen. Original vertical FOV/
geometry scale remains; desktop fills at its own aspect with vertical cropping.
Paper placement applies the original capture-rectangle matrix before global
ImageBase T(offset)*S(scale), correcting final zoom alignment. No arbitrary
compensating page shift, new art, dialogue or invented timing was introduced.
Initial aspect/helper work was retained during the concurrent subsystem move;
GPU correction commit4f719013 uses the new include/story and source/display
paths. Original US init was checked read-only; its book offsets remain0.

Validation: isolated frozen frame0 native GPU replacement compares against the
live original page with all193x240 crop pixels within2 RGB levels, mean absolute
channel error0.0003742084 (book-alignment-verified/crop-comparison.json).
Original CPU/RGB/display-readback probes reproduced malformed captured pixels;
those private diagnostic builds are not deliverables. Direct GPU flat probe
(azahar-book-rtt-flat) fixes it. Production animation run
artifacts/azahar-book-four-turns completes all four turns and zoom/Peach handoff;
reviewed settled pages and transition, typical60FPS. No native exception.

Current reorganized source was then freshly compiled with
make -o assets-ready all BUILD=build-book-verified TARGET=ttyd-book-verified
(book-current-clean-all.log), reusing the already converted original packs.
Its actual normal binary artifacts/book-alignment-current/ttyd-2ds.3dsx passes
another isolated full four-turn/zoom/market run (azahar-book-current,
book-current-smoke.log). Reviewed all pages and final transition. Current cold
market admission samples0FPS at t108 and26FPS at t110, then60FPS market; this is
still synchronous scene IO/performance work, not accepted as hitch-free. Page
registration/art remains correct. Coherent prior baseline plus owned book
changes also builds normal and DEBUG_SHORT_BOOT (book-final-native-all.log,
book-debug-short.log). Normal deliverable and hashes are project-local ignored
artifacts; no diagnostic frame0 hold is enabled in production.

Host checks: coherent snapshot make test209/209 CTest and133/133 Python pass
(book-rtt-tests.log). Current standalone Python141/141 pass
(book-current-python-tests.log). Required shared make test was attempted:
first CMake encountered the directory migration mid-edit; existing host cache
then produced24 failures/225 including stale-layout segfaults. Fresh host build
artifacts/book-host-clean removes those segfaults, passing227/228 CTest, with
only unrelated trio_attack_original assertion at tests/trio_attack_tests.cpp:410
(pose word0x14==74/scalar0x44==1) remaining (book-clean-tests.log). Both book
geometry/capture tests pass. First shared make all encountered stale shader
source dependencies after migration; fresh native build above passes. Original
book/capture/camera8 verifier65 anchors pass read-only (book-original-anchors.log).
Scoped whitespace and documentation-link checks pass; logs retained.

No user emulator profile/saves or ISO writes. Remaining: physical New2DS XL
raster/performance comparison; original image-camera/TEV/shadow parity beyond
this documented adapter; cold market admission stall and separate trio test
failure. Full remake/prologue battle progression remains separate unfinished
work. This correction does not revalidate New Game beyond the previous arrival
route or alter its story endpoint.
