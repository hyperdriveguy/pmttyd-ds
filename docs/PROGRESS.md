## 2026-10-14 — Original audience member dispatch, motion and three idle reactions

Commit `bb51dc1d` replaces the native positive-audience generic controller stub
with the whole local80192a0c dispatcher and motion tail. Its21 original state
calls retain typed unavailable providers. Whole WaitHurimuki80192164,
WaitKubiKatamuke80192054 and WaitJump80191f20 now run with the existing original
packed SetAnimation/ChangeStatus helpers. Native GetFront consumes retained
camera4 eye/at through the existing source dot-product helper. Main Wait and
reactions4..20 still suspend at their real function addresses, with no successful
no-op controller. This increment does not yet advance the normal positive-crowd
loop beyond its first idle decision.
[Control contract and source pins](behavior/battle/AUDIENCE_MEMBER_CONTROL_AND_IDLE_REACTIONS.md).

Motion keeps axis-by-axis position/velocity single stores, updated-X facing,
negative-height reset before integration, type11 reset, type2 single phase wrap,
narrowed sine and final offset stores. Native double libm sine remains a declared
SDK numeric adapter, not Gekko math parity. Turn/tilt inclusive phase boundaries,
signed timer arithmetic and jump fallthrough/repeat/landing ordering follow the
read-only US DOL. No action-command timing, controls, story/dialogue, original
menu/HUD art, camera cadence, lower-screen replay or unit/model scale changed.

Exact validation used isolated `artifacts/battle-lens-qwwrj2n7`, refreshed from
frozen tracked base `3bd2bc4fc5c39f7e0535b17415ada803307acfd8` plus owned source,
CMake and tests. Unrelated live animation/render/field work was excluded. First
focused executable passed. First full run passed296/298 C++ tests; two newer
field loaders rejected older private snapshot packs (trouble_board_native and
field_bero_info). After that run and the native build terminated, regenerated
private door_data, bero_info and trouble_board_event packs with that snapshot's
matching extractors and the read-only ISO. Extraction exited0
(`artifacts/audience-actor-assets-refresh.log`); no extracted/compiled data was
committed. An initial final-source copy command used the snapshot cwd and failed
before writing; corrected to absolute root/snapshot paths before final builds.

Cleaned newly introduced misleading-indentation warnings and added a combined
state/dispatcher test through all three type7 jumps and landings. Final
`MAKEFLAGS=-j4 make test` exited0:298/298 C++ and173/173 Python
(`artifacts/audience-actor-test-final.log`). Tests also cover all21 targets,
low-byte front selection, inclusive tilt/turn phases, state-provider holds before
motion, earlier stores retained before missing sine, negative height/type11,
exactly one phase wrap, offsets, retained hover/NaN facing, and unchanged original
pack constants. Final `TMPDIR="$PWD/artifacts/tmp" make all -o assets-ready
BUILD=build-battle-lens TARGET=ttyd-battle-lens` exited0
(`artifacts/audience-actor-native-final.log`); assets-ready bypass uses the private
validated packs, including the refreshed field closures. Both initial native and
final native builds exited0; remaining compiler warnings predate this increment.
Owned diff checks passed. Scoped temporary-index commit preserved other threads'
changes. No new emulator/visual acceptance or hardware-performance claim.

Next: port whole Wait80192260 and its actual helpers, retaining exact FPR0
instruction dataflow and resolving/explicitly representing the low-ratio branch's
caller FPR29 dependency rather than inventing zero. Then close reactions/entry/
item controllers and original normal Main/outer camMain/camDraw cadence so the
inherited field view advances into battle framing. Harmful-item telegraphs,
physical X plus optional audience taps, original class/turn/menu choreography,
tutorial followup hits, field return and actual voice playback remain open. The
tutorial and real battle goal remains active and incomplete.

## 2026-10-14 — Published native field view survives shared camera4 battle entry

Previous turn made progress by removing an artificial mode2 lens-hook gate.
Commit `7b818a14` now bridges the current native field frame into retained
camera4 and inherits its published matrices at actual battle construction.
Availability is explicit native metadata: boot has no view; real CameraManager
view publication or an accepted field candidate marks it available. The field
bridge converts eye/at/view translation and projection translation to source
units, preserves flags/roll/extra/up/aspect/viewport, rejects invalid candidates
atomically, and refuses battle mode2. Full original history remains unproven.
[publication contract](behavior/camera/NATIVE_FIELD_CAMERA4_PUBLICATION_AND_BATTLE_INHERITANCE.md).

Normal Game field ticks stop updating the field camera while retained mode2 owns
it. Removed all three per-battle-tick diagnostic field-lens setter calls; they
had repeatedly overwritten the battle-owned lens. Fresh runtime adoption uses an
already published snapshot and marks history incomplete, rather than inventing
an identity/boot view. The world/audience prefer the retained published eye/at/up
instead of an unpublished controller frame, and can use the actual shared Game
bank before attack runtime creation. Lower replay remains a single preparation.
Updated the older camera contract's unconditional missing-view statement.

Tests cover same normalized projected point across native/source units,
retained field preservation, invalid/degenerate-frame rejection, mode2 refusal,
real constructor/shared bank identity/depth inheritance, and actual Game tick
publication/ownership. First focused test build caught a mistaken fixture API
name (`Game::step` corrected to `tick`); the next run caught a projection oracle
that had omitted the view transform. Corrected the oracle to the actual
view-then-project order; no implementation arithmetic changed for that mistake.
Focused test then exited0. Full `make test` exited0:292/292 C++ and171/171 Python
(`artifacts/field-camera-publication-test.log`), retaining intentional missing-
depth/resume and real ending camera/queue tests.

Exact validation used prior isolated `artifacts/battle-lens-qwwrj2n7` with its
validated source-lens/mode2 work plus owned hunks, excluding unrelated concurrent
Game/trio changes. Native `TMPDIR="$PWD/artifacts/tmp" make all -o assets-ready
BUILD=build-battle-lens TARGET=ttyd-battle-lens` exited0 after normal main was
restored (`artifacts/field-camera-publication-native-final.log`).
`-o assets-ready` reuses validated private original packs. Owned diff checks
passed. Temporary-index commit preserves unrelated edits and appends only the
owned CMake/index fragment. No asset/ISO/compiled pack was committed.

Isolated50-second Azahar run exited0 without a native exception detected. Its
ignored fixture takes a real Game field tick, then passes that same bank into
actual source eight-member battle initialization with no manual lens/depth hook.
`artifacts/azahar-field-camera-handoff/data/azahar-emu/sdmc/field-camera-handoff.txt`
records active1/shared1/published1/mode2/depth1, FOV25/near1/far32768. Inspected
`artifacts/azahar-field-camera-handoff/audience-source-69.png.png` shows the stage
and existing two-screen floor replay, but still at the inherited field framing.
The normal battle outer camera cadence remains blocked by unbound providers, so
this capture is handoff proof, **not accepted battle framing**. The8-member
capture reports about59 FPS; it is not comparable to the200-member benchmark.
The forced story/pouch/main and trace code were removed before final build.
Profiles are generated/isolated and never touch user saves/settings; approved
sandbox escalation provides local display access.

Next: bind the original normal battle Main/outer camMain/camDraw cadence at its
actual provider boundaries so the inherited view advances into battle framing.
Complete audience type controls/reactions before harmful-item telegraphs,
physical X and optional audience taps. Source pose-root/part drawing, original
class/turn/menu choreography, tutorial followups/field return, full-crowd
performance and actual voice playback remain open. No controls, dialogue, story
grant or unit/model scale was changed by this increment. The full tutorial/real
battle goal remains active and incomplete.

## 2026-10-14 — Actual mode2 camera publication no longer needs a manual lens hook

Previous turn made progress by consuming the retained camera4 lens in the native
world and validating visible framing. This turn traced the actual retained-camera
Main provider. Commit `800e16ef` removes its false requirement that the optional
native `setCameraDepthLensAdapter` diagnostic hook be present. The real retained
camera already owns FOV, near/far and projection type. Admitted mode2 can now run
the existing battle-frame producer, copy eye/at/up, calculate distance/yaw and
publish projection/View through the original ordered CameraManager continuation.
No new camera calls are inserted into another cadence. Flag2/unsupported-mode/
unavailable-frame gates and the separate camDraw boundary remain intact.
[mode2 contract](behavior/camera/NATIVE_MODE2_RETAINED_LENS_PUBLICATION.md).

Updated the existing real ending integration path to change retained camera4
fields directly (FOV31, near2/far20000, member aspect1.7), with no manual lens
hook. Its actual callback publishes `cameraPerspective(31,1.2666666507720947,2,
20000)`, preserving the member aspect as required by the source fixed-aspect
mode2 rebuild. Tests verify shared camera identity, unchanged queue20/shadow3
collection, published view/depth, effect-camera copies, subsequent original
provider holds and preserved published projection after a later explicit
diagnostic lens change. This replaces the obsolete expected artificial hold at
cam3dMain8000ea68; the real next hold is camDraw8000f454.

Exact validation used prior isolated `artifacts/battle-lens-qwwrj2n7`, frozen
base7351519229ea1481b063104033c125b1a3aaae2c plus committed lens work and this
owned increment. Unrelated concurrent trio/test edits remain in the live tree;
only exact owned hunks were committed with a temporary index. `make test` exited0:
291/291 C++ and171/171 Python (`artifacts/camera-producer-test.log`), including
`trio_attack_original`'s actual ending/camera continuation. Native
`TMPDIR="$PWD/artifacts/tmp" make all -o assets-ready BUILD=build-battle-lens
TARGET=ttyd-battle-lens` exited0 (`artifacts/camera-producer-native.log`).
`-o assets-ready` uses the snapshot's previously validated private original packs.
Owned `git diff --check` passed. No new graphics fixture or emulator acceptance
claim was made for this producer-only increment; prior captures do not prove its
normal gameplay route. No assets, compiled packs, controls, dialogue, timing,
unit values or original controller math were changed.

Limitations/next: this closes an actual ending-path publication gate, not initial
pre-display field-matrix history. `dispCalcZ` needs the preceding real field
camera4 publication before the first battle display queue can be collected.
Bind that retained field/view ownership and the normal camMain/camDraw cadence,
then validate source pose-root/part presentation and normal battle admission.
Complete audience type controls/reactions before harmful-item telegraphs,
physical X and optional audience taps. Original class/turn/menu choreography,
tutorial followups/field return, full-crowd performance and real voice playback
remain open. The full tutorial/real battle goal stays active and incomplete.

## 2026-10-14 — Trio world consumes its retained source camera4 lens

Previous turn made progress by wiring Mario's published transform and verifying
nonidentity fields in an isolated graphics probe. This turn committed `95a4d8b9`:
the independent trio world reads the retained CameraManager camera4 FOV and
near/far fields through the existing TrioAttack owner. It no longer reconstructs
a42-degree lens with near.1/far80. Source boot25/1/32768 becomes native
25/.01/327.68, using the existing world-distance conversion. The helper rereads
live fields without modifying the owner; perspective/range/finite checks reject
unsupported native preparation. Handheld aspect and centered lower continuation
remain explicit adaptations, and actor/model scale is unchanged. Updated the
older scene contract so it no longer describes the retired lens.
[lens contract](behavior/display/TRIO_SOURCE_CAMERA4_LENS_CONSUMPTION.md).

This is live lens consumption, not original cached GX projection or flag0x40
refresh cadence. View-producer availability and provisional native eye/at fallback
are unchanged; original published eye/at/up still take precedence. Original
projection-extra/roll/offset, complete camera framing and raster parity remain
unproven. No controls, source dialogue or simulation timing were changed.

Exact validation used new frozen snapshot `artifacts/battle-lens-qwwrj2n7`,
base7351519229ea1481b063104033c125b1a3aaae2c plus owned changes, excluding
unrelated worktree edits. New tests verify actual source boot lens, subsequent
owner changes, distance conversion, source-state preservation, unsupported
projection and invalid values. Existing seam/crop/depth/W tests remain green.
Initial `make test` passed287/291 C++ checks; copied old private packs lacked
current door/Bero/map-flag dependencies. Copied the ignored original symbol table
into the snapshot, then regenerated private packs with its frozen converters:
`tools/door_data_assets.py`, `tools/bero_info_assets.py`,
`tools/trouble_board_event_assets.py` (each --iso read-onlyG8ME01 path) and
`tools/build_assets.py` (same default read-only ISO). The first converter attempt
lacked that ignored symbol table; retry after providing it succeeded. The next
host run passed289/291 before map-flag sidecars were rebuilt. No root/user packs
were changed. Final `make test` exited0:291/291 C++ and171/171 Python
(`artifacts/battle-lens-test-final.log`).
`TMPDIR="$PWD/artifacts/tmp" make all -o assets-ready BUILD=build-battle-lens
TARGET=ttyd-battle-lens` exited0 after normal main was restored and the regenerated
private RomFS repackaged (`artifacts/battle-lens-native-final.log`).
`-o assets-ready` reuses those validated packs. Owned `git diff --check` passed.

Isolated50-second Azahar run exited0 without a native exception detected, using
the explicit source-initialized200-member development pouch and its own generated
profile. Inspected `artifacts/azahar-battle-lens/audience-source-69.png.png` shows
the tighter source-lens framing, stage actors at larger projected size, and a
substantial crowd section on the lower screen with continuing floor/window art.
The inspected emulator status reports30 FPS. Other captures share that directory.
This proves that fixture's visible native lens change, not a hardware benchmark,
normal-story admission or a complete real battle. No fixture main or source
population grant remains in normal code. No original pixels, compiled packs or
ISO were committed. Local display access used approved sandbox escalation.

Next: bind original camera4 view production and published projection/refresh
cadence, then verify normal battle actor/pose-root/part presentation. Bind audience
type controls and reactions before harmful-item telegraphs, physical X and
optional audience taps. Continue full-crowd performance work without dropping
members or simulation visits. Original class/turn/menu choreography, tutorial
followups/field return and real voice playback remain open. The full requested
tutorial/real battle goal remains active and incomplete.

## 2026-10-14 — Mario consumes actual published battle transforms

Previous turn made progress through alias-preserving RAM and quad preparation
optimizations with an inspected full-crowd capture near30 FPS. This turn traced
actor geometry and found a concrete native consumer omission. Commit `63dd401f`
passes Mario's published battle actor record to `characterModel`, as already done
for Goombella/enemies, and applies the existing recovered camera/flag-dependent
battle reflection policy. Mario now consumes published unit scale, rotation,
pivot and display offset. Field/story rendering retains its ordinary path and
the lower view replays the same prepared vertices. No default scale, model data,
input, dialogue or simulation timing was changed.
[transform contract](behavior/display/MARIO_TRIO_PUBLISHED_TRANSFORM_CONSUMPTION.md).

A host diagnostic (`artifacts/actor_bounds.cpp`/`artifacts/actor_bounds`) loaded the
existing original compiled idle packs. Mario M_S_1 has306 vertices and bounds
(-10.7977,-1.6582,-1)..(7.34854,21.8018,1); Goombella PKR_S_1 has390 vertices and
bounds(-5.32982,-.197732,-.3)..(9.60646,14.5269,.5). The ordinary native root
adapter already uses.02 (documented scalar2 times unit conversion.01). No
arbitrary enlargement was introduced; original battle pose-root/camera parity
still requires evidence. Read current `CameraManager::initializeBoot`: its
recovered source camera4 lens is25 degrees, near1/far32768 engine units. The
independent renderer's provisional42-degree fallback still does not consume that
actual lens owner; bind it next rather than treating actor size as established.

Exact validation used the prior isolated `artifacts/trio-scene-cgzs5cy_` snapshot
with committed replay/performance changes plus this owned renderer hunk, excluding
unrelated concurrent work. `make test` exited0:285/285 C++ and168/168 Python
(`artifacts/mario-transform-test.log`). Normal native
`TMPDIR="$PWD/artifacts/tmp" make all -o assets-ready BUILD=build-trio-scene
TARGET=ttyd-trio-scene` exited0 after all fixture/trace sources were restored
(`artifacts/mario-transform-native-final-clean.log`). `-o assets-ready` reuses
validated private original packs. Owned `git diff --check` passed. A restore
attempt used the wrong working-directory path; waited for that build to finish,
restored all three files with absolute paths and rebuilt the clean normal target.

Two isolated50-second Azahar probe runs exited0 without native exceptions. The
first pre-launch override did not visibly establish transform consumption and
is not counted as a positive result. The second ignored fixture applies actual
packed Mario fields after launch and explicitly publishes them. Its trace
`artifacts/azahar-mario-transform-published/data/azahar-emu/sdmc/mario-transform.txt`
records scale2,2,1; rotation0,0,25; offset30,0,0; actual unit84101000 and packed
words40000000/41c80000/41f00000. Inspected
`artifacts/azahar-mario-transform-published/audience-source-39.png.png` visibly
shows Mario enlarged, tilted and shifted versus the baseline. This proves
consumption of that published fixture, not normal publication cadence or a real
attack/turn. It uses an explicit8-member development pouch and a generated
emulator profile, never user saves/settings. All packed overrides, manual
publication and trace code were removed before the final native build. No
original asset or compiled pack was committed. Display access used approved
sandbox escalation.

Next: consume the actual source camera4 lens and recover/bind its view producer;
validate normal battle actor publication, pose roots/parts and source transforms.
Profile remaining full-crowd cost and original animation/display cadence. Bind
audience type controls/reactions and harmful-item telegraphs with physical X
plus optional tapping. Complete original class/turn/menu choreography, tutorial
followups/field return and real sound voice playback. Normal-story admission and
complete tutorial/real battles remain unproven; the full goal stays active.

## 2026-10-14 — Full audience reaches about30 FPS without removing members

Previous turn made concrete progress through two-screen scene replay and visible
original crowd rows; the200-member fixture reported about15 FPS. This turn
committed `5f648111` (plain-RAM halfword/word access) and `c131e00d` (ordinary-UV
triangle emission). RAM spans wholly within one region and outside every live
semantic extent now use direct big-endian byte lanes. Live aliases, sparse lanes,
cross-region spans, failed-read output preservation and atomic rejected stores
retain the existing facade. No pointer is cached across mapping changes.
[RAM contract](behavior/event/NATIVE_PLAIN_RAM_WORD_FAST_PATH.md).

The audience backend now emits ordinary in-range UV quads as the same two
triangles without temporary clipping polygons. Out-of-range poses retain the
existing full geometry/UV-boundary tessellation, including original1.075 end
poses. Texture mapping, RGBA, winding, source member visits/requests and simulation
ticks are unchanged. Tests cover all four original templates from802fa024,
retained area1600, atlas bounds and nontrivial RGBA.
[triangle contract](behavior/display/AUDIENCE_IN_RANGE_TRIANGLE_EMISSION.md).

Exact validation used prior isolated snapshot `artifacts/trio-scene-cgzs5cy_`
(frozen base0ba9e2c4 plus the committed replay/framing and owned optimizations),
excluding unrelated concurrent work. First `make test` passed285/285 C++ and
168/168 Python (`artifacts/audience-ram-test.log`). Added mapped final-address
coverage, rebuilt/ran `event_address_space_tests` successfully
(`artifacts/audience-ram-boundary.log`); tests compare byte-visible state with
unaligned halfword/word reads/stores across adjacent RAM, holes, live aliases and
the32-bit ceiling, preserving all existing bytes after rejected stores.
After both optimizations, final normal-source `make test` exited0 with285/285 C++
and168/168 Python (`artifacts/audience-quad-test.log`).
`TMPDIR="$PWD/artifacts/tmp" make all -o assets-ready BUILD=build-trio-scene
TARGET=ttyd-trio-scene` exited0 with normal main restored
(`artifacts/audience-quad-native-final.log`). `-o assets-ready` uses the already
validated private original packs. Owned `git diff --check` passed.

Isolated50-second Azahar runs used the same explicit source-initialized200-member
pouch fixture and generated profiles, never user saves/settings. Both exited0
without a native exception detected. The RAM-only capture
`artifacts/azahar-audience-ram/audience-source-99.png.png` reports30 FPS, compared
with the prior inspected15 FPS capture. The combined optimization capture
`artifacts/azahar-audience-quad/audience-source-99.png.png` also reports30 FPS,
retaining the visible original audience across the seam, original dialogue and
window frames. This is an emulator status observation for this fixture, not a
native hardware benchmark or original raster parity claim. The polygon shortcut
has no demonstrated additional FPS gain here. Normal main contains no forced
pouch/population fixture. Display access used approved sandbox escalation. No
asset pixels, compiled packs or ISO content were committed.

Actor scale was inspected but not changed: ordinary root transforms already use
.02 (documented original scalar2 times native unit conversion.01). The small
actors still need original asset bounds/unit initialization/camera evidence;
increasing scale without that evidence would not establish fidelity. Next work:
profile remaining full-crowd cost and original display/animation cadence; verify
battle actor transforms and recover/bind original camera4 framing. Bind audience
type controls and reactions before harmful-item telegraphs, physical X and
optional audience taps. Original class/turn choreography, normal-story battle
admission, tutorial followups/field return and real voice playback remain open.
The full tutorial/real battle goal remains active and incomplete.

## 2026-10-14 — Trio stage and audience continue onto the lower screen

Committed `58d95166`: the independent native trio world now captures the exact
submitted stage-map ranges (after source visibility filtering), texture
sampler descriptors, cull/depth masks and prepared actor ranges. The lower
screen replays sky, opaque stage, actors, translucent stage and original audience
with the existing centered320x240 clip continuation. It does not call original
animation/event/control/draw producers again or rewrite geometry. Readiness
resets at synchronized frame admission, so failed top preparation cannot replay
stale GPU data. Field proxy actors/NPCs, field shadows, departure drawing and
field item-get effects are suspended while the trio owns the stage. Existing
command/target rows retain their original assets and layout; harness action
notices have original type0 frames for readability over the scene. Physical
input and action timing were not changed.

Until the original camera4 producer publishes a view, the native world and
Audience GetFront now share the existing tutorial camera adapter: eye(0,2.8,5.5),
at(0,1,-.4), FOV42, near.1/far80. The suspended field camera no longer frames the
battle. Original published eye/at/up still take precedence; these fallback values
are provisional presentation constants, not measured original behavior. See
[replay and framing contract](behavior/display/TRIO_LOWER_SCENE_PREPARED_DRAW_REPLAY.md).

Validation used isolated snapshot `artifacts/trio-scene-cgzs5cy_`, frozen at
`0ba9e2c4c61ea081f96aebfec73fcb68156c222c` plus owned changes. Concurrent NPC and
shadow changes were excluded from the feature snapshot/commit and retained in
the live worktree. Initial native compilation caught a concurrent NPC call
copied without its pending declaration; that unrelated call was removed from
the snapshot. Initial host run passed283/285 C++ checks but failed missing Garf
and Borodo catalogue assertions: copied private packs were stale relative to
frozen source. Regenerated only the isolated private catalogue with
`python3 tools/field_actor_assets.py --iso /home/carson/games/roms/G8ME01.iso`
(read-only ISO; no root/user assets changed). Final `make test` exited0:
285/285 C++ and168/168 Python (`artifacts/trio-scene-test-fixed.log`). The later
camera fallback changes are native-only; portable test inputs were unchanged.
`TMPDIR="$PWD/artifacts/tmp" make all -o assets-ready BUILD=build-trio-scene
TARGET=ttyd-trio-scene` exited0 after normal main was restored
(`artifacts/trio-scene-native-final.log`). `-o assets-ready` uses validated private
original packs in the snapshot. Owned `git diff --check` passed.

Two isolated50-second Azahar runs exited0 without a native exception detected.
The first (`artifacts/azahar-trio-scene`) established floor continuation but
exposed the field camera compressing the audience into the upper view. After
correcting the fallback, inspected
`artifacts/azahar-trio-scene-camera/audience-source-39.png.png` shows original
crowd rows crossing the screen seam into the lower view and the floor continuing
behind original window frames. Additional captures are in the same directory.
The ignored fixture explicitly supplies a200-member source pouch, runs actual
original population initialization and shares that pouch with its HUD owner.
It grants nothing in normal story flow; all forced-main code was removed. No
original pixels or compiled packs were committed, and generated emulator
profiles never touch user settings/saves. Display socket access used approved
sandbox escalation. The full-crowd capture reports about15 FPS: performance
needs work and is not accepted as a smooth real battle. Source actor framing,
normal-story admission and original camera/GX raster parity remain unproven.

Next: verify native battle actor geometry/scale and bind original camera4
framing/producer cadence; reduce full-crowd cost without dropping original
members or simulation visits. Bind original audience type controls/reactions
before harmful-item telegraphs, physical X interruption and optional audience
taps. Finish original class-ring/menu choreography, turn cadence, tutorial
followups/field return and actual sound voice playback. This establishes the
requested lower scene direction but does not finish tutorial/real battles;
the full goal remains active.

## 2026-10-14 — Native trio startup reaches visible original audience

The previous turn made progress through audience rendering, tests and a stack
correction; its black-screen fixture remained unresolved. This turn narrowed the
stall to `TrioAttackRuntime::initialize` before its RAM-registration completion
marker. Original pouch creation, intro, command setup and two physical confirm
operations had returned. Native address-space admission was enumerating every
byte through all semantic providers for large unmapped RAM spans.

The native adapter now rejects existing RAM intersections with 64-bit half-open
intervals and asks conservative interval probes of the four live mapping owners.
If any live extent intersects, it retains the old readable-byte admission scan,
including sparse/unreadable aliases. Otherwise no byte enumeration is necessary.
Read/write ordering, packed bytes, allocation contents, script import transactions,
original story and controls are unchanged. This is native storage policy, not a
new recovered original VM check. See
[admission contract](behavior/event/NATIVE_RAM_ADMISSION_INTERVAL_PREFILTER.md).

Evidence: the ignored positive-pouch/source eight-member fixture now reaches RAM,
script, allocator, camera, unit, gauge, pose, resource, callback, spotlight,
audience, slot and attack completion markers. Its isolated50-second Azahar run
exited0 with no native exception detected. Inspected capture
`artifacts/azahar-audience-range/audience-source-39.png.png` shows the actual
source-created audience with original frame assets on the upper stage; emulator
status reports58 FPS at that capture. Later captures and trace live under the
same isolated artifact directory. This establishes visible native audience
output for that fixture; it does not prove normal-story admission, original
camera/raster parity or a complete battle. The visible trio lower screen is
still the plain native menu and upper camera framing remains wrong.

Exact validation used `artifacts/audience-draw-3v804l5x`, the prior isolated source
snapshot with committed audience/heap changes plus this owned increment.
Regression tests compare registration with the previous readable-byte oracle
at unaligned entry/context/GP/RAM boundaries, runtime holes and the32-bit ceiling,
and test an unreadable context pointer lane alongside readable adjacent bytes.
The trace-enabled first `make test` run passed283 C++ and167 Python tests.
After removing all ignored startup/init trace code and restoring normal main,
final `make test` exited0 with283/283 C++ and167/167 Python tests
(`artifacts/audience-range-test-normal.log`).
`TMPDIR="$PWD/artifacts/tmp" make all -o assets-ready
BUILD=build-audience-draw-review TARGET=ttyd-audience-draw-review` exited0
(`artifacts/audience-range-native-normal.log`), using already generated private
original packs. The normal source contains no forced pouch/population/story
fixture. `git diff --check` passed on owned changes. Scoped incremental commit
preserves unrelated concurrent work; no extracted pixels or compiled assets are
committed. Emulator runs use generated project-local profiles, never user saves
or settings. Local display socket access required approved sandbox escalation.

Next: extend the trio lower screen with a replay of the continuing stage/audience
scene and original menu presentation, without another source animation/simulation
visit. Recover/bind original camera4 framing and verify the normal battle route.
Bind audience actor/type controls and reactions before harmful-item telegraphs,
physical X interruption and optional audience taps. Original class/turn cadence,
tutorial followups/field return and actual sound voice playback remain open.
The full tutorial/real battle objective remains active and incomplete.

## 2026-10-14 — Original audience draw and native stack correction

Implemented and committed `a4c2fc37`: whole original audience draw/GetFront
branches, normals, profile scaling, pivot/rotation matrices, source RGBA/UV
selection and retained flip register behavior. The native backend consumes actual
source-created members and original animation frame bytes. Opaque/translucent
passes share a simulation-timestamp animation guard; tutorial lower scene replays
prepared vertices without another animation visit. This does not populate the
fresh tutorial audience or replace the trio lower menu with scene imagery.
Original TPL CLAMP/linear sampling now uses exact edge texels in atlas gutters;
UV-boundary tessellation preserves source end-pose overshoot without atlas bleed.
No original asset pixels or compiled packs were committed. See
[draw contract](behavior/battle/AUDIENCE_DRAW_NORMALS_FLIP_AND_NATIVE_ATLAS_CLAMP.md)
and [asset conversion record](ASSETS.md).

Validation used the isolated frozen snapshot
`artifacts/audience-draw-3v804l5x` (base
`eb3dd161a98252ce0e96639be9a52ff12a5f1301`, plus owned changes), preserving
concurrent unrelated work. `make test` exited0:283/283 C++ and167/167 Python
checks (`artifacts/audience-draw-test-api-fix.log`). Tests include actual8-member
native initialization/draw packets, same-timestamp animation deduplication,
negative exact-multiple retained flip behavior, guest texture reuse, clamp
geometry area and all224 original frame interiors/gutters. Read-only ISO source
verifier pinned9 bodies and4 data spans (`artifacts/audience-draw-source.log`).
`TMPDIR="$PWD/artifacts/tmp" make all -o assets-ready
BUILD=build-audience-draw-review TARGET=ttyd-audience-draw-review` exited0 with
normal entry restored (`artifacts/audience-draw-native-final-normal.log`).
`-o assets-ready` uses the already generated private original packs in this frozen
snapshot. First native attempts caught a missing camera type include and the
3DS SDK `quad` macro collision; corrected include and renamed backend callback
`emitQuad`. The initial upper-warp test expectation was corrected to source40;
no source height constant was changed.

Native graphics fixture testing is **not a visible audience acceptance pass**.
`tools/emulator_smoke.py` used separate project-local profiles and an ignored
explicit positive-pouch fixture, never user saves/profile or normal story grants.
Xvfb initially failed from sandbox socket access; approved display escalation
allowed testing. First fixture raised an undefined instruction with SP below the
main stack and LR in actorNodeMatrix. Disassembly showed main reserving28596
bytes plus saved registers from the default32768-byte stack. Commit `7ff6b866`
places Renderer in a unique heap owner, reducing that reservation to11476 bytes
plus saved registers. Subsequent10- and50-second isolated runs exited0 with no
native exception detected, but captures remained black/App0FPS. An ignored
startup trace reached renderer initialization, required asset loads, game
allocation and frontend completion, then stopped within the forced trio fixture
setup before its completion marker. Artifacts:
`artifacts/azahar-audience-draw-heap`, `artifacts/azahar-audience-draw-long`,
`artifacts/azahar-audience-draw-trace`. No visible original raster fidelity,
normal-story battle admission or successful first draw is claimed. Diagnostic
main changes were removed and the normal native target rebuilt successfully.

Remaining work: locate the fixture stall between pouch/intro/menu/attack setup,
then prove actual source audience GPU output. Bind original audience main/type
controllers and reaction/throw telegraphs; keep physical X interruption alongside
optional audience taps. Reflow the trio lower screen to the continuing scene and
original menu. Finish original battle scheduler/class menu/turn cadence, tutorial
followups and field return; public sound allocation still has no actual voice
playback. Native math/world-camera adapters are explicitly not original Gekko/GX
numerical/raster equality. No controls, dialogue or story grants were changed by
this increment. Full tutorial/real battles remain incomplete; the goal stays
active.

## 2026-10-14 — Native audience Sound1 and real pending public slots

Commite5045a47 implements whole original __psndSFXOn pending allocation and
AudienceGetInfo1/2/Sound1/2. Typed public slots retain original lookup/gates,
last-free search, strict priority reclaim, defaults/flags, separately-rounded
delay arithmetic, retained coordinates/engine, low16 preference/generation and
actual handle encoding. Native assigned audience requests now invoke the real
Info1→default positional On→volume chain against session-owned publicSfx.
[Pending ownership and limits](behavior/audio/AUDIENCE_SOUND_START_AND_PUBLIC_PENDING_ALLOCATION.md).

The explicit positive development pouch now completes native begin: actual8
original members, noise handle0127, slot39/generation1, original key3925,
volume101/row-volume107, pending flag1 and engine-1. The actual audience audio
request retains that handle. This closes the previous Sound1 allocation frontier
through original source behavior; no backend voice/channel or known-key-1 result
is substituted. Fresh empty setup4 remains the original8c0 domain. The source
story producer of8c is not inferred from this manually supplied test input.

Correction: the original noise/clap rows are effects (noise FX159..162,
clap153..156, descriptor200000xx), not SoundSS streams. Earlier handoff language
calling the missing service a stream was too specific. Original name/table/
constant data was already present in private DOL packs. No additional original
PCM, music, artwork or compiled asset extraction occurred this increment.
Actual psndSFXMain dispatch, original macro/sample/voice backend execution,
spatial updates and audible NDSP output still require binding. Original pending
Check can report occupancy before a voice exists; it does not prove playback.

Validation: frozen artifacts/audience-sound-dlqk0uau, base
93388a913750f0cd1a591f13d74d154adc653905 plus only this increment. Final
make test passes280/280 C++ and167/167 Python checks
(artifacts/audience-sound-test-final.log). New checks cover null-position/stale
engine retention, defaults, preference/generation wrap, full name scan/miss,
mute/cooldown/story gates, raw flags, equal-priority exhaustion, strict reclaim/
failed active stops, missing conversion retaining its published prefix, zero
volume still starting/setting volume, real-1 retained through Sound1, Sound2
member position and actual pending allocation. Native positive startup asserts
real public registry and audience state; the prior unavailable-audio assertion
was updated only after the real producer was implemented and native begin passed.
An initial test expected source63 rather than actual GetInfo1 constant250; the
source-data implementation was correct and that test expectation was corrected.

Required native make all -o assets-ready BUILD=build-audience-sound-review
TARGET=ttyd-audience-sound-review passes using the prepared private original
packs (artifacts/audience-sound-native-final.log), producing170357496-byte3DSX.
Source verifier passes10 bodies,5 spans and r2 pin
(artifacts/audience-sound-source.log). No Azahar/hardware/audio-output parity
run occurred. Other concurrent source changes were excluded from the frozen
increment and preserved during scoped commits. No action-command/render cadence,
input policy, UI style, original dialogue or story writes were changed.

Next: bind actual effect pending dispatch/original macro/sample playback and
public slot lifetime updates; retain genuine backend stop failures. Bind
original audience Act/control and DispAudience801a38f8 to real prepared member
records/frames on both scene views, plus positive target changes. Real harmful
throw telegraphs/interrupt targeting must precede optional taps with physical X.
Normal tutorial admission, source class-ring readiness/menu drawing, command/
turn scheduling, successful Jump voice and battle teardown/field return remain
incomplete. The full tutorial/real-battle goal remains active.

## 2026-10-14 — Original audience population, placement and animation owners

Commit9ec56730 implements whole local SettingAudience, weighted front/back and
uniform seat allocation, cached-count front/back routing, Entry/Sub, SetAnim,
ChangeStatus, Delete and GetAnimEnd bodies. Original setup modes, override types,
RNG request ordering, allocation/sorting/residual/guest rules, paired reservation,
retained/current pointer queries, stage Y, source placement FMA and animation
pointers are preserved. The native trio Setting provider now invokes this general
producer with inherited RNG and finite conversion. Fresh pouch8c=0 retains the
verified empty setup4 result; no story grant or invented crowd is introduced.
[Ownership/domain contract](behavior/battle/AUDIENCE_POPULATION_SEATS_ENTRY_AND_ANIMATION_OWNERS.md).

Positive testing exposed an actual missing original sdata dependency. Both harbor
and trio asset producers now import only the hash-pinned immutable audience
scripts/item-table extent8041c638/14e8 (182 source symbols), preserving original
addresses, frames, delays and loops. Native mutable globals remain excluded.
Both original producers ran successfully in the private snapshot
(artifacts/audience-population-trio-assets.log and
artifacts/audience-population-harbor-assets.log). Validated PMEVT products and
trio metadata were published atomically to ignored RomFS for later sessions;
no original payload is committed. See [asset provenance](ASSETS.md#2026-10-14--original-audience-animation-and-item-table-data).

Validation: frozen source artifacts/audience-population-3ae2fdxi, base
a6b5e3bbc1c756568855c5b64bf98ed3f2e36fee plus only this increment. Final
make test passes277/277 C++ and166/166 Python checks
(artifacts/audience-population-test-final.log). New checks compare20 empty seeds
to the prior verified graph/RNG; exercise all seven finite setup modes and12
valid overrides; actual8-member construction;30 original interpreter advances
and resulting original atlas-frame lookup; paired neighbors/status/deletion;
occupied/reserved rejections; animation force/retention; allocator endpoints;
missing providers; and original retained record versus fresh Sub after RNG
changes current battle. Both PMEVT spans match their exact source hash and omit
native mutable globals. Source verifier passes17 bodies,9 data spans and r2 pin
(artifacts/audience-population-source.log).

Required native make all -o assets-ready BUILD=build-audience-population-review
TARGET=ttyd-audience-population-review passes using privately prepared original
packs (artifacts/audience-population-native-final.log), producing170335596-byte
3DSX. Actual byte searches find both whole PMEVT packs and verify each new
5352-byte audience span (artifacts/audience-population-native-embedding.json).
No emulator/visible audience or original GPU/hardware parity run was performed.

The explicit positive native-pouch probe still fails at existing Sound1/8019fcec,
after actual total8, desired8 and capacity50 publication. An ignored diagnostic
confirmed this boundary; diagnostic source was restored and touched to force
actual recompilation before final full checks. The final tests preserve the
known-key unavailable-audio boundary, rather than supplying a fake voice or-1
known-key result. The whole Init test independently verifies member/count/window/
texture publication and the actual assigned noise key/right+left count before
that request. Early fixture corrections fixed nonexistent test helper names
and selected the real PMEVT file rather than its PMHTUT metadata; no behavior
assertion or original data was relaxed to force gameplay completion.

Next: bind original Sound1/positional stream creation to real original crowd
noise so nonempty initialization can finish. Then bind original active audience
state control, positive target adjustments and DispAudience801a38f8 with its
actual camera/material/UV rules to both scene views. Real throw telegraphs and
target interruption must precede optional audience taps with physical X. Source
story pouch8c admission remains to prove. Normal tutorial admission, live class
ring/readiness and command/turn scheduling, successful Jump voice and full
battle teardown/field return remain incomplete. The full goal stays active.

## 2026-10-14 — Original audience atlas and native frame lookup

Commit728660e0 prepares all224 original audience frames:205 normal and19 Luigi.
The read-only ISO producer pins both original bank hashes, preserves unchanged
TPL payloads and packs64x64 decoded frames into1024x1024 RGBA8 without resizing.
68px cells retain2px transparent padding; native texture allocation is4MiB.
Catalogs preserve bank handles1/2 and original indices. Optional native frame
lookup validates the whole fixed layout before publishing, rejects unknown/
out-of-range indices and retains its previous catalog on failed reload. No
population, animation table, gameplay input, dialogue or menu style is changed.
[Asset paths, format and rights](ASSETS.md#2026-10-14--original-audience-frame-atlas).

Validation: frozen source artifacts/audience-atlas-nt50yyto, base
d6ce4c9be1231d654013aef88813433149fcc09e plus only this increment. Final
make test passes275/275 C++ and163/163 Python checks
(artifacts/audience-atlas-test-final.log). Checks compare all224 atlas crops to
original TPL decoding, verify all bank/index/UV mappings and transparent padding,
and exercise malformed, truncated, trailing-data and invalid lookup domains.
Required native make all -o assets-ready BUILD=build-audience-atlas-review
TARGET=ttyd-audience-atlas-review passes with prepared private packs
(artifacts/audience-atlas-native-final.log), producing170305752-byte3DSX.
Byte searches verify actual embedding of the7188-byte catalog and505909-byte
T3X (artifacts/audience-atlas-native-embedding.json). Atlas PNG was inspected;
no emulator/GPU draw or hardware performance claim follows.

The first host process terminated with143 without a compiler diagnostic; its
interrupted link left a zero-byte event_if_opcodes executable. A resumed full
run also identified the shared stale PMDOOR1/two-door pack, incompatible with
committed three-door PMDOOR2 source. The frozen original door converter regenerated
the pack privately; no source/assertion change was made. Initial native packaging
failed because the shared transient romfs/soundtrack-switch directory disappeared.
After both processes ended, the snapshot acquired its own copied RomFS, received
the regenerated door pack, and the empty executable was removed for actual
relink. Final full checks and native packaging then passed. Shared emulator
profiles/saves were untouched. Original/compiled assets and evidence remain
ignored; incremental commit contains source and documentation only.

Next: implement source positive-capacity audience population/Entry and animation
selection, then bind original audience draw and prepared texture on the continued
scene. The current fresh setup4 tutorial has source pouch8c=0/capacity0; resource
availability must not manufacture occupants. Original draw801a38f8, population
801a60c4 and Entry801a3400 remain the next containing bodies to bind. Actual
harmful-item telegraphs/target interruption must precede optional bottom-screen
taps with a physical X equivalent. Live class-ring camera/SDK/palette/footer
binding and once-per-simulation-tick readiness, normal Game tutorial admission,
command/turn scheduling and successful Jump voice remain incomplete. The active
full tutorial/real-battle goal is not complete.

## 2026-10-14 — Tutorial scene continues onto the lower screen

Commit645042f0 extends the native original harbor tutorial scene into a centered
320x240 lower view beneath the unchanged400x240 upper view. Original stage,
empty seating/floor and actors share the same camera/simulation snapshot and
pixel scale; lower width crops40px from each side, vertical view continues240px.
The lower pass reuses exact top actor vertex ranges without rewriting their
arena or reevaluating animation. Top HUD/messages/curtain stay on the top screen.
Both targets have actual depth buffers; top world matrix is restored afterward.
This is native layout adaptation, not recovered original camera framing. See
[two-screen contract](behavior/display/NATIVE_BATTLE_TWO_SCREEN_SCENE.md).

Validation: final make test in artifacts/battle-dual-view-lvo3xlzi, base
94707d140518015c36f5ebc94299c8926cba4b05 plus only this increment, passes272/272 C++ and159/159
Python checks (artifacts/battle-dual-view-test-final.log). Required native
make all -o assets-ready BUILD=build-dual-view-review
TARGET=ttyd-dual-view-review passes with existing private converted packs
(artifacts/battle-dual-view-native-final.log), normal3DSX452464980 bytes.
Point/matrix checks assert downward seam, centered crop, equal physical pixel
scale and preserved homogeneous/depth coordinates. Final build includes cached
actor ranges, avoiding shared top overlay/curtain vertex overwrite risk.

Two isolated ordinary checkpoint attempts stayed in the field and did not
exercise the battle view. The legacy checkpoint was already room1; a mistaken
save-offset adjustment changed only HP in a separate generated test fixture,
not room or implementation. No normal story-flow claim follows from those runs.
A separate ignored graphics harness temporarily enters actual original tutorial
initialization with beginFresh(0,7,10,4), without modifying committed main code.
Its source entry was restored afterward. Harness native build passes
(artifacts/battle-dual-view-harness-build.log), and tools/emulator_smoke.py uses
an isolated Xvfb/profile under the snapshot's artifacts/azahar-dual-view-graphics.
The run exits0 without native exception; inspected tutorial-two-screen-79.png
shows original stage/actors/HUD above and original empty seating/floor continuing
below with the centered seam/crop. This proves graphics in that harness, not
normal tutorial admission, original raster equality or audience behavior.
User emulator profile/saves were untouched. All screenshots/binaries/profiles
and original assets remain ignored; no new art or dialogue was generated.

Next: instantiate original audience actors/animations/throw telegraphs in the
visible seating, then target interruption and optional taps with physical X
alternative. Bind live class-menu actor/camera/SDK/RNG/palette/footer services
and once-per-simulation-tick ring publication; normal Game tutorial admission,
command/turn scheduling and successful Jump voice remain incomplete. Current
native stage camera/HUD reflow still needs fidelity refinement, and the former
lower submenu adapter for independent trio contexts still requires replacement
when their full original menus are bound. The active goal remains incomplete.

## 2026-10-14 — Original class footers and actual no-item audience query

Commited156822 implements complete local party/item cue, cue Sub, pinch order,
Mario warning and Party warning bodies. Actual partner/HP checks, source timer
and sound gate, retained projected midpoint, palette/language/color pulse,
translation/scale/rectangle requests and linked texture/projection/sound services
are preserved. The whole GetItemOn producer and three base/member/item getters
scan actual200 packed records, retain source eligibility/cache/live output writes
and produce actual no-match; no crowd or item is fabricated. See
[the footer/query contract](behavior/battle/CLASS_MENU_FOOTERS_AND_AUDIENCE_ITEM_QUERY.md).

Class, weapon and target-menu audience checks now admit that whole producer's
actual no-match path. Physical X no longer freezes the ready class menu at the
previously unbound getter; the native fixture can still select Jump using A.
A real item retains produced data/cache effects and the interruption-consumer
frontier; audience target/end/light handling is not falsely completed. The finite
native Init2D backend now restores texture*raster color rather than carrying the
class material register into warning quads. No story/pouch eligibility change,
new original asset extraction or dialogue rewrite was made.

Validation uses artifacts/class-footer-6vk2_fzv with snapshot base
7fa5b6462d7e4d7d804e84a80cdb9096e82f9b3e plus the scoped footer/query increment. Final make test
passes270/270 C++ and159/159 Python checks
(artifacts/class-footer-test-final.log). Required native make all -o assets-ready
BUILD=build-class-footer-review TARGET=ttyd-class-footer-review passes with the
already converted private packs (artifacts/class-footer-native-final.log),
producing452458424-byte3DSX. Verifier passes10 containing/local body hashes,
4 source data spans and r2 pin (artifacts/class-footer-source.log). New checks
cover partner/no-partner cues, counter crossing below0/clamping next call, sound
edge, healthy early exits, critical palette/color, Sub matrix translation,
GetItemOn eligibility/cache/outputs and actual physical-X no-match continuation.
An initial test-only mixed-auto declaration was corrected before final validation.
The final verifier was run separately and copied into the snapshot after builds.
No emulator/visible footer or original GPU parity check was performed.

Next: native camera/projection/SDK/RNG/palette/texture/sound bindings and actual
once-per-simulation-tick ring publication still need connecting to the available
GPU backend; manual development readiness remains. Warning/cue palette resources
and btlDispTexPlane2 are linked frontiers, not successful draw stubs. Normal Game
command/turn scheduling and successful Jump voice remain open. The lower screen
still needs original scene/audience continuation; bind real audience actors/events
and target interruption before optional bad-throw taps with a physical equivalent.
Do not reinterpret no-match admission as full audience or battle completion.
The active tutorial/real-battle goal remains incomplete.

## 2026-10-14 — Original disabled-icon material and ring position providers

Commita8c781a7 binds original iconGX flag32 appearance for class icon flags48:
three source R/G/B stages with555555ff coefficient average the texture channels;
record RGB is ignored, texture/record alpha is preserved. The native GPU uses
replicated channel operands and original accumulation order. Packet admission
now accepts flags16/48. Original assets are reused without gray variants.
Renderer refuses atlas work before graphics initialization and requires its
admitted catalog for the batch. See
[disabled material](behavior/display/CLASS_ICON_DISABLED_MATERIAL.md).

Commit786a77b2 implements whole original GetRingCenter80119cf0/0x84 and
GetRingOffset80119c7c/0x74 local bodies with explicit projection/cosine/sine
providers: actual current-global active-unit position address, source22.5/105
shifts, Z0, left clamp-180, and original80/35 ellipse around retained3pi/2-angle.
An unavailable sine preserves the preceding X write; no coordinates or trig
result are invented. Native shader stages are reset per quad and explicitly
cleared/restored before the Citro2D boundary. See
[ring position](behavior/battle/ACT_RING_CENTER_AND_OFFSET_PROVIDERS.md).

Validation: isolated artifacts/class-icon-material-4zzi9e98, base
0ac453e0c7ac53e39d7fabc3e81ecf1c44d7a5e6 plus scoped material/helper increments.
Material make test passes265/265 C++ and159/159 Python
(artifacts/class-icon-material-test.log); native make all -o assets-ready with
existing converted packs passes (artifacts/class-icon-material-native.log).
Final combined make test, including helper and explicit post-batch shader reset,
passes266/266 C++ and159/159 Python
(artifacts/act-ring-position-test-final.log). Final native make all -o assets-ready
BUILD=build-class-material-review TARGET=ttyd-class-material-review passes
(artifacts/act-ring-position-native-final.log), producing452454300-byte3DSX.
Material verifier pins containing iconGX body and coefficient span
(artifacts/class-icon-material-source.log); position verifier passes3 body hashes,
4 scalar spans and r2 pin (artifacts/act-ring-position-source.log). Tests check
source program channel/alpha ownership, RGB-tint independence, actual unit lookup,
projection mutating the global owner, clamp/shift and partial trig output.
No new original asset extraction, emulator capture or raster parity claim.
The rational-color test reference does not prove GX/PICA fixed-point rounding.

Next: connect actual native actor/camera projection and SDK/libm/RNG services,
packed source ownership and once-per-simulation-tick ring publication to the
available GPU backend. Bind party/pinch footer requests and ordinary source UI
providers rather than returning fake successful calls; remove manual readiness
only when the native owner is complete. Normal Game tutorial command/turn
scheduling, successful Jump voice and broader battle events remain unbound.
Preserve source pouch/story eligibility rather than enabling moves globally.
Upper original menus/HUD and lower audience continuation from user references
remain required; actual audience events and optional physical-equivalent
stop-throw controls are unfinished. The goal remains active and incomplete.

## 2026-10-14 — Native class-menu atlas and transformed quad backend

Commit284390c2 adds strict, atomic loading of original surface/class-icon
catalogs, synchronous finite GX packet assembly, source-ordered transformed
quads and native GPU texture/submission/release APIs. The dedicated class atlas
does not overwrite the global message-icon registry. Quad requests preserve
bubble mirroring, source material/shadow alpha and full-size flag16 icon anchors.
Plain US labels use the existing WID1 measurement and StringMatrix glyph
consumer, centered through the supplied matrix, with explicit two-per-glyph RNG
requests. Unsupported controls/flags and incomplete primitives reject; no glyph
or icon substitution is supplied. See
[the backend contract](behavior/display/NATIVE_CLASS_MENU_PACKET_BACKEND.md).

Native Renderer owns both original atlases and a bounded1024-quad linear arena,
reset per rendered frame and released at shutdown. Submission uses the existing
shader and original atlas UVs, source quad order, native upper-screen half-size
reflow and explicit GPU/Citro2D restoration. UI clocks are not advanced by draw.
The backend is available for a source presentation owner but has not been called
from normal tutorial/Game activation yet; no visible menu is claimed.

Validation: final make test in artifacts/tutorial-class-backend-b1d5g17f,
basecf3e8c442cf12be3207367e72fd961cc535e33df plus this scoped increment,
passes264/264 C++ and155/155 Python checks
(artifacts/tutorial-class-backend-test-refreshed.log). Tests load the actual
private catalogs/font and cover transformed mirrored quads, primitive admission,
source colors, icon anchors, explicit disabled-path rejection, glyph centering,
UV/color and required RNG calls. Initial compiler syntax error in the glyph
failure check was fixed before validation. A subsequent full run exposed stale
shared atlas data missing original524/134/136; the snapshot's existing committed
converter regenerated those assets read-only in
artifacts/class-backend-icon-refresh-87iiehnz and the verified private atlas was
published. No converter/source expectation was changed to bypass that failure.
ASSETS already records these original icons and unchanged conversion/licensing.

Native make all -o assets-ready BUILD=build-class-backend-review
TARGET=ttyd-class-backend-review passes with the prepared private packs
(artifacts/tutorial-class-backend-native-refreshed.log); final3DSX is452454664
bytes. An mmap inspection finds the exact refreshed message-icons.bin payload
at452314696 inside the native artifact. No Azahar/visual GPU check was performed.
Extracted assets, catalogs and compiled packs remain ignored.

Next: original flag48 disabled icons enter a separate three-stage TEV path at
iconGX80019a08; that appearance is still unbound, so the backend rejects it.
Bind it before admitting the complete live class menu. Connect actual packed
memory, actor projection/GetRingCenter/GetRingOffset, camera/matrix/RNG services
and once-per-simulation-tick ring publication, then Renderer::drawClassMenu and
normal tutorial command/turn scheduling. Manual class-readiness acknowledgment
remains until that owner is complete. Keep physical D-pad/Circle Pad/A/B and
upper original menus; lower audience continuation and optional physical-equivalent
stop-throw interaction remain required. Successful Jump voice, broader battle
scheduling and audience events are unfinished. The active goal remains incomplete.

## 2026-10-14 — Original class circle/bubble consumers and textures

Commit6a94d338 implements whole DrawMainIconCircle801191a8/0x4a0 and
DrawMainIconHukidasi80119648/0x474 local bodies. Typed requests preserve original
GX setup/texture/primitive/icon/font identities, matrix calls, shadows, circle
depth gray byte and scaling, disabled flags, icon0->403 fallback, bubble
mirroring and fresh enabled-state checks. Label width uses unsigned low16 bits,
compressing only X above90 source units. The circle icon's original rotation
then identity reset is retained. Unwritten source angle-offset cases stop at an
explicit frontier after prior effects. See
[the complete consumer contract](behavior/battle/CLASS_CIRCLE_AND_SPEECH_BUBBLE_DRAW.md).

Imported original battle/common/battle_common.tpl indices0(40x40 quadrant)
and1(128x80 speech bubble) after checking source constructor pathname and draw
indices. Private256x128 RGBA8 atlas uses2px transparent padding, unchanged pixels
and no resizing. Dedicated class-icon atlas remains separate. Normal assets-ready
now invokes this producer. Paths/hash/conversion/licensing are recorded in
[ASSETS](ASSETS.md); ISO, decoded pixels and compiled packs remain ignored.

Validation: make test in isolated artifacts/tutorial-class-raster-maq_jw1b,
base517fa8795ab9d58c75d0bff065088aedefbb6ba9 plus only this code increment,
passes261/261 C++ and146/146 Python checks
(artifacts/tutorial-class-raster-test.log). New checks cover exact point/quad
counts, shadow and gray colors, disabled/fallback icons, texture indices,
mirrored geometry, retained label with fresh text enable state, width truncation
and compression, atlas pixel equality, UV descriptors and padding. Read-only
source verifier passes3 body hashes,4 data spans and r2 pin
(artifacts/tutorial-class-raster-source.log). Native make all -o assets-ready,
BUILD=build-class-raster-review TARGET=ttyd-class-raster-review passes with the
prepared private packs (artifacts/tutorial-class-raster-native.log), producing
452451756-byte3DSX. Exact new bin and t3x payloads are found inside the artifact
(artifacts/tutorial-class-raster-embedding.json). Final verifier span coverage was
expanded separately after snapshot builds began, rerun successfully and copied
into the snapshot; compiled code/test/converter files match the tested copies.
No Azahar visual check was performed; bundled assets do not prove visible UI.

Next: bind these request consumers to actual native GX/SDK, palette/icon and US
font providers, load the new atlas in the renderer, and bind center/offset with
actual actor projection. Connect ring lifecycle once per simulation tick before
removing manual development class-readiness acknowledgment. Then normal tutorial
command/turn scheduling and successful Jump voice remain. Preserve upper-screen
original menus and bottom-screen lower audience continuation from both user
screenshots and mockup; optional stop-throw taps still require actual audience
telegraphs/events and a physical equivalent. Full weapon UI, audience logic and
broader ordinary battles are incomplete. The active goal is not complete.

## 2026-10-14 — Original class-ring draw lifecycle and icon routing

Commit26a1f8b8 implements complete local DrawMainMenu801160e4/0xb94 and
DrawMainIcon80119abc/0x1c0 bodies with host-testable typed providers. Entry,
stable selection, both rotation directions, wrap bookkeeping, resume translation
and growth update original work arrays and readiness bytes. Source arithmetic
uses the retained angle table, wrap32 integer products/signed division and
float/fused operations. Icon routing resolves the actual acting unit/cursor,
sorts positions through the original descending swap loop, checks visibility
and routes actual flag2 to speech-bubble rather than circle providers. Unwritten
source scratch values are explicit optionals; no invented transforms or count
clamping is supplied. See
[the draw contract](behavior/battle/ACT_SELECT_RING_DRAW_AND_ICON_ROUTING.md).

Validation: final make test in artifacts/tutorial-class-draw-7stb2j28 passes
259/259 C++ and145/145 Python checks
(artifacts/tutorial-class-draw-test-final.log). Snapshot base is
996ae10930d2f72124981347fa9aab0b4363860e plus this scoped increment; it preserves
committed concurrent fixes and excludes unrelated working edits. Final native
make all -o assets-ready BUILD=build-class-draw-review
TARGET=ttyd-class-draw-review passes using existing private converted packs
(artifacts/tutorial-class-draw-native-final.log), producing452443020-byte3DSX.
The read-only source verifier passes4 body hashes,3 data spans and r2 base pin
(artifacts/tutorial-class-draw-source.log). Earlier initial build also passed;
final run includes added sorting/routing cases and final source read ordering.
No emulator/visual run or new original asset extraction was performed.

Limitations/next: these are complete local controllers with linked providers,
not complete native rendering. Camera/projection, GetRingCenter/GetRingOffset,
SDK matrix/interpolation, DrawMainIconCircle/DrawMainIconHukidasi raster, party
button and pinch-mark bindings retain their own frontiers. The explicit native
classMenuPresented acknowledgment remains until a full presentation owner
invokes this lifecycle once per simulation tick. Bind original circle/bubble
resources and actual upper-screen rendering next, then normal tutorial command
and turn scheduling. Lower-screen audience scene continuation and optional
physical-equivalent stop-throw interaction remain required by both user
screenshots and the rough two-screen mockup. Successful Jump voice, full weapon
UI, audience events and ordinary battle scheduling are still incomplete.
The goal remains active; neither tutorial nor full remake completion is claimed.

## 2026-10-14 — Book runes/picture source cinematic crop

Completed the active inside-cover/page-text goal. User clarified the printed
runes and picture needed comparison against original Dolphin. Prior turns
corrected the green cover/paper width; this audit exposed the missing settled
source letterbox crop. Original camLetterBox8000e388 caps top49/bottom44; its
source quads expose Y49..436 (height387). Direct read-only DOL inspection is in
artifacts/book-vertical-letterbox.log. Original isolated Dolphin first-picture
border is at Y67 in reference-t110.png, hence expected native
(67-49)*240/387=11.1628. Previous native border was34; corrected Azahar border
is11. Actual-image row analysis is artifacts/book-vertical-comparison.json.
This proves the reported vertical framing correction without inventing an
independent rune/picture shift.

Commit996ae109 fills that original cinematic interval in the400x240 view with
uniform480/387 zoom and the asymmetric vertical offset. Capture textures keep
their existing193x240 composition/VRAM footprint; normal cover and paper consume
the same outer transform afterward. A source-illustration-edge fit, weighted
by opening/scale/fade, keeps the picture in view. About179px of green cover
remain visible, versus98 before the initial framing fix. Source meshes, UVs,
art, original dialogue, sequence and simulation/frame presentation gates stay
intact. Native pan/fit/left-width adaptations are explicitly documented; full
original image-camera/GX/Gekko equivalence is not implied. The final fade still
returns focus to the illustrated page.

Validation: final make test passed260/260 C++ and145/145 Python
(artifacts/book-vertical-final-tests.log); the preceding full run also passed
260/260 and145/145 (book-vertical-tests.log). Native make all with prepared
assets passed in the coherent private snapshot (book-vertical-native.log).
Both book crop/geometry checks cover the source visible bounds, asymmetric
offset, captured-front layout, hinge continuity and illustration-edge fit.
Original book/capture/camera8 verifier65 anchors passed again read-only
(book-vertical-original.log). The shared unrelated harbor regression from the
prior entry was repaired by its owner before this run; no book-test skips.

Isolated Azahar artifacts/azahar-book-vertical completed four turns and final
zoom/market/Peach handoff, exit0/no native exception. Reviewed initial green
cover and picture(t46), turned runes(t64), all subsequent pages, terminal
Rogueport(t116), final fade/zoom and Peach(t136). Main-screen picture edge11
matches the source visible crop; page/paper retain that position through turns.
Final immutable artifact artifacts/book-vertical-current/ttyd-2ds.3dsx has its
source-scope/binary SHA manifest. No user emulator profiles/saves or ISO writes;
no new extraction/conversion/source payload entered history. Source/rendered
comparison and licensing are recorded in MECHANICS/ASSETS and book contract.
Scoped whitespace/doc links checked. Physical hardware performance, full raster
parity and existing cold scene IO remain follow-up work. Full remake is open.

## 2026-10-14 — Original Trouble Center window rendering

Committed bc65e4ac. Portable full display requests and native Waku/font/scissor
backend now render original four frames/headings/all-count rows with completed
gray/active red styling, three fresh row-ID reads and independent scroll samples.
64 original US resources retained; client/contents copy verbatim21/31-byte caches,
widths prepared for immutablefont/spacing2. Matrix titles correctly ignore shared
.85; rows use proportional per-glyph scaled-pen FMA, existing shaders/arenas and
GPU clipping. No new texture/placeholder, source94 animation or acceptance write.

Reviewed tree35421e0b7b4d768ecc3ae9f40fb2423fadb704e1 passes256/256 C++ and
145/145 Python via make test -o assets-ready and normal make all -o assets-ready.
Sequential assets-ready and existing original class-icon dependency converter
passed. Logs artifacts/trouble-window-{final-assets,class-assets,final-test,
final-build}.log. Normal reviewed artifact SHA256
5c3224471812fddd35608bc63ac8bea3ee44c577503722e78eafb9662caa0a95.
Native fog enum/type and missing independent class-icon test pack failures fixed;
initial failed logs retained. Verified owned changes committed atop concurrent
work; unrelated edits preserved.

Private DEBUG_SHORT_BOOT preview renders original story8 headings/six rows using
explicit test completion/active colors; checkpoint byte-identical. Inspected
artifacts/azahar-trouble-window-preview-checked/original-trouble-window-preview.png;
smoke log trouble-window-preview-checked-smoke.log passes. Initial purewhite check
was corrected against original panelRGBA216/219/216/255; transient missing/tmp
script failure retained, final input script workspace-local. Preview SHA256
8bef578dd92e64696398060bba5f3888aa4c57cdb4940ee4ee468f40f42da388.
Private preview source restored/audited all2600 tracked files and normal-main
recompile timestamp restored. Unintended shared-root preview build artifact
unused; isolated artifact supplies evidence. User profile/saves untouched.
[Exact implementation/limitations](behavior/display/NATIVE_TROUBLE_CENTER_WINDOW_AND_US_CACHE.md).

Next: original menu icons and controller→display binding, complete saved switch
providers, case9/door/cutaway admission for the actual east board, first lecture
and confirmation/acceptance event. All30 help/confirmation table IDs are now
recovered. Then validate acceptedquest2→parcel→Goomfrey→McGoomba full chain.
Font-global/SDK/GX/matched-original pixels, GP38/pad/NDSP parity and hardware
performance remain open. Backend preview is not native board gameplay; full-game
goal remains active/incomplete and future original areas remain to be added.

## 2026-10-14 — User two-screen battle mockup

Recorded the user's subsequent rough hand-drawn layout in
[HUD/menu presentation](behavior/display/NATIVE_BATTLE_HUD_AND_SUBMENU.md):
upper screen holds HUD/stage/battle actors; narrower centered lower screen
continues into audience rows at consistent scene scale. Rough colors/shapes
are composition guidance; original ISO art and the earlier game screenshots
remain the style reference. Audience touch interaction is optional and must
retain a physical equivalent. Exact framing/gap compensation needs visual
validation when the scene continuation is implemented.

Documentation-only update; git diff --check passes. No code, original assets
or test expectations changed. Implementation and validation limitations from
the class UI handoff below remain open.

## 2026-10-14 — Original class UI ownership and physical command binding

Commit0ae3c80f implements whole original ActSelect Init80116cdc/0x1a4 and
End80116c78/0x40 with host-testable allocation/free providers, preserved unused
words, entry/resume arrays, signed source delay arithmetic and ready-byte gate.
Native harbor ActMenu setup/main/end now bind actual class/cursor/status,
position, physical pad and camera services. Class work uses tracked SDKheap0
allocations; legacy weapon projection lifetime is explicitly native. Weapon UI
End frees its current class owner before zeroing it. Physical B returns from
weapons to classes; after explicit presentation readiness, physical A selects
Jump and opens weapons again. The fixture enables Jump in its retained pouch;
source bare-pouch eligibility is unchanged. An explicit openClassMenu entry is
available, but normal Game activation is not added in this increment.

The user's two screenshot references are recorded in
[HUD/menu presentation](behavior/display/NATIVE_BATTLE_HUD_AND_SUBMENU.md):
compact original top HUD, circular icons beside Mario and a selected-command
speech bubble. The lower screen must continue the scene into the lower audience;
the existing lower-screen submenu adapter is interim. Audience stop-throw taps
remain optional and require a physical alternative plus actual original
telegraphs/events. No new original asset extraction or payload commit occurred.

Validation uses immutable snapshot artifacts/tutorial-act-ui-ziodkpyd, base
945cbe10773c328a63d4682fbe6bf86921ffbd71 plus only this code increment.
Required make test ran:256/257 C++ tests passed, including battle_act_ui and
harbor_tutorial_original; harbor_encounter_original fails at
harbor_encounter_tests.cpp:62, groundHeight/landedFloor and jumpHeight assertion,
in the separate actor-motion work. Log:artifacts/tutorial-act-ui-test.log.
Since make stops on CTest failure, Python was run separately:145/145 passed
(artifacts/tutorial-act-ui-python.log). Native make all -o assets-ready with
existing converted private packs, BUILD=build-act-ui-review and
TARGET=ttyd-act-ui-review passed (artifacts/tutorial-act-ui-native.log);
3DSX is452441496 bytes. Existing original act-menu verifier passes24 body pins,
472 instruction anchors and3 data spans
(artifacts/tutorial-act-ui-source-verification.json). No Azahar visual check
was performed for this increment; host/native success does not prove UI parity.

Limits/next: classMenuPresented is an explicit native readiness acknowledgment,
not recovered original visual cadence. Sound tokens are recorded, playback
still needs original binding. Connect Game tutorial turn ownership and original
class icon/bubble renderer, then replace the lower-screen submenu with audience
scene continuation. Successful Jump voice, full weapon UI lifecycle, Crump
scheduling, audience actors/throw response and broader ordinary battle providers
remain incomplete. Continue original US dialogue and simulation-tick action
commands; do not report the tutorial or full remake complete.

## 2026-10-14 — Turned book page width

Follow-up user report resolved in commitbc2cfe37. The native crop placement
compressed the source terminal39.9395-unit page width to120.4426px while its
49.875-unit height used249.375px. The left-folded side now uses the same5px/unit
as Y, yielding199.6975px and a narrow inside-cover border. The captured front
retains193/64px/unit and unchanged frame0 registration; placement is continuous
at authored hinge x=-32. All four original A_1 samples/UVs/materials, source rune
texture, captures, narration and180-tick presentation gates remain intact. This
piecewise native aspect adapter is documented explicitly; it is not recovered
original image-camera/GX raster equivalence. The inside-cover pan and final-fade
focus corrections from4dfec42d/2b62bdf3 remain active.

Validation: both updated book tests pass2/2 (artifacts/book-width-tests.log).
Required shared make test passes258/259 C++ checks; unrelated
harbor_encounter_original fails tests/harbor_encounter_tests.cpp:26
sawRun/sawJump/sawLockedJumpPose after concurrent actor changes
(book-width-shared-test.log). Standalone Python passes145/145
(book-width-python.log). Isolated native make -o assets-ready all passes
(book-width-native.log), using the already converted original packs and snapshot
base4dfec42d plus the scoped final-fade/width changes. A first compiler attempt
lost a temporary assembly file; rerun with snapshot-local TMPDIR passed.

Isolated Azahar normal run artifacts/azahar-book-width exits0 without native
exception, completing four turns, final zoom and market/Peach handoff. Reviewed
initial green cover, first/remaining turned pages, terminal Rogueport page(t116),
fade/zoom(t122) and Peach(t136). The left paper now occupies the inside cover
rather than leaving a broad green strip; front registration remains intact.
Final immutable playtest artifact: artifacts/book-width-current/ttyd-2ds.3dsx;
manifest records exact source scope and binary SHA. Native snapshot/captures and
source inspection remain ignored; no user emulator profile/saves or ISO writes.

Original US read-only Dolphin comparison also completed in an isolated profile
(artifacts/dolphin-book-width, book-width-dolphin.log). First illustration and
first turned rune back are observed at t110/t125; the original viewport clips
the left page and does not establish its complete width. Health-screen input and
process-relative capture labels are not a measurement of original event timing.
Observation is recorded in MECHANICS.md; source bounds remain static resource
evidence. Original cover/capture/camera anchors previously pass65 read-only and
are unchanged by this layout adapter. Asset paths/conversions/license are in
ASSETS.md. Scoped whitespace/document links checked.

Next: physical New2DS XL visual/performance checks, original outer image-camera/
projection comparison and existing cold scene IO stalls. Separate harbor battle
actor/timing regression remains with its owner. Full remake stays incomplete.

## 2026-10-14 — Opened book inside-cover framing

Commits4dfec42d and2b62bdf3. User clarified that the green inside cover immediately after opening looked
small. Source cover geometry/UV dimensions were intact; the closed-cover camera
center left the opened spine at x98, clipping the left green cover to98 visible
pixels while the right side occupied216. Commit4dfec42d adds an explicit native
horizontal pan during opening, proportional to source angle/scale and hinge.
At scale1.5/rotation180 the pan is102 pixels and spine x200; each inside cover
has200 visible pixels. Closed cover/vertical FOV, original geometry, source
artwork, narration order and simulation timing remain intact. This is a native
framing adaptation, not a recovered original camera mutation.

Turning-paper placement receives the same pan. Final zoom scales the pan and
weights it by1-fade to return focus to the illustrated right page.
GPU capture moves from110/0/193/240 to212/0/193/240; it still contains the entire
193x240 crop, including five pixels beyond the visible screen for deformation.
Untilting/crop projection and previous retained pages stay registered. Existing
original assets/texture conversions/licensing remain as recorded in ASSETS.md;
no source art, extracted payload or ISO writes were introduced.

Validation: shared make test passed256/258 C++ checks, including both book tests.
Unrelated trio_attack_original failed its cameraHistoryComplete/pendingFunction
assertion at tests/trio_attack_tests.cpp:464; concurrent battle_action_class_table
failed missing original romfs/battle-class-icons.bin at line74. Standalone Python
passed145/145. Shared make all hit the concurrent uncommitted trouble-quest
renderer compile error. Isolated committed snapshot artifacts/book-inside-review
then passed make -o assets-ready all, reusing prepared private assets. Read-only
US book/capture/camera8 verifier passed65 anchors. Logs are artifacts/book-inside-
test.log, python.log, build.log, review-build.log and original.log respectively
(the shared log basename prefixes are book-inside-). Binary source commit/SHA
is retained in the snapshot manifest. No user profile/save mutations.

Final book geometry/capture tests passed2/2 after the fade adjustment; native
incremental make all passed (book-inside-final-tests.log/final-build.log).
Isolated Azahar normal runs artifacts/azahar-book-inside and
azahar-book-inside-final both completed all four turns and market/Peach handoff
with exit0/no native exception. Reviewed green cover filling200px to the spine
at native x200 (t46), turned paper and final Rogueport page, final zoom and Peach.
Native snapshots/captures are private ignored artifacts. Physical hardware and
original full raster parity remain unmeasured; existing cold IO stalls remain.
The subsequent user report about turned-page width is under investigation;
source A_1 frame180 bounds are x[-71.9395,-32] versus initial[-32,32], and the
current capture-crop scale reduces its screen width further. Compare original
runtime/projection before altering that deformation. Full remake stays open.

## 2026-10-14 — Original action-class descriptors and class-menu icon pack

Committed `9a8a9d32`. Complete shared _btlcmd_MakeActClassTable now preserves
unit/pouch/setup-definition/cursor ownership and entry order before wrapped
M-zero, raw ABI return, seven ordered admissions, live kind/level/flag checks,
all eligibility checks even after denial, reason preservation, original message
keys and source icon selection. Message providers can change kinds, levels and
flags; subsequent source reads remain fresh. Operations/Lecture are independent
tests, allowing both/neither under mutation. Disabled rows still count; padding,
unused rows, leading fields and cursor+4 remain intact. Current count publication
and signed index correction retain negative and zero-count behavior.

Native buildClassMenu exposes actual Mario descriptors from the original field
pouch/definition and unchanged US label/cache providers. Native fixture builds
these records before its existing physical menu/target/cancellation/attack flow;
HP4/7, one payment, failure/dealt1 and result0 still come from the actual failed
Jump. This API does not open class UI or auto-advance turns. Class-table/UI
registration, constructor/animation/resource ownership and Game activation remain
unfinished. No table-shaped placeholders or rewritten labels are introduced.

Imported original IDs334..343 from icon.bin/icon.tpl images339..348, one static
48x48 frame each. Existing message/HUD atlas could not admit ten extra images;
the completed importer uses a dedicated256x256 RGBA8 sheet (262144 bytes) with
2px transparent borders and no resampling. Original general atlas remains
unchanged. Added its normal assets-ready producer. Private outputs are
local-assets/battle-class-icons/ and romfs/battle-class-icons.bin/t3x/json,
using existing PMICON1 format. Original pixels/UVs/padding and portable loader
are tested; native dedicated texture binding/drawing remains pending. Licensing
and conversion choices are in ASSETS; original payloads/ISO are not committed.

Validation: isolated artifacts/tutorial-class-table-tpbgou0l, based on
f9afecc2 plus scoped changes, passed `make test`:255/255 C++ and145/145
Python, no skips. Source fixtures cover kind/level/counter signedness, absence
versus disable, mutable message providers, retained cursor, both/neither final
rows, reason/padding/count/index boundaries and provider frontiers. Native fixture
checks real class labels/types/reasons before battle continuation. Final native
`make all -o assets-ready BUILD=build-class-table-review
TARGET=ttyd-class-table-review` passed with prepared private assets. Artifact:
that snapshot's ttyd-class-table-review.3dsx (451945808 bytes); mmap verified the
actual original class .bin embedded. Logs: artifacts/tutorial-class-table-test.log,
tutorial-class-table-native.log and tutorial-class-table-assets.log. Read-only
class verifier passed4 bodies/7 spans/366 anchors (complete producer and3 linked
services). Final scoped files matched review; diff check passed. Initial atlas
capacity failure was resolved with the dedicated sheet, preserving all general
assets. No Azahar/Dolphin, new original-runtime measurements or rendered/playable
class-menu claim. Concurrent camera/book/quest/render changes were preserved.

Next: original ActSelect UI Init/End and texture/resource lifetime, class-menu
provider bindings into Harbor's existing full Input controller, source first-act/
turn/post-action/phase scheduler and physical class/weapon/target presentation in
Game. Then native rendered menu/cancellation smoke and original-runtime comparison,
successful voice/pitch/volume/follow-up hits, hazards/guard/status domains, and
second-screen scene/audience extension with optional hostile-item taps and a
physical equivalent. Tutorial/real battle/full remake goal remains active and
incomplete; descriptors/resources alone do not prove a playable original loop.

## 2026-10-14 — Original class-menu controls and physical cancellation handoff

Committed `f7b4fba1`. Complete portable ActMenu Main/Setup/End, cursor initializer
and position-change gate now preserve source request order/retained owners,
exact nine cursor resets with counts/exclusions intact, fresh flags after UI
providers, signed Mario/Party move/status gates, live class-count validation,
reason precedence, timer word wrap, all14 published type routes/default and
local0/1 result policy. Sound/UI providers can change rows/cursors/reasons;
source rereads are retained. Navigation keeps forward priority, post-sound wrap
reads and unchecked source count arithmetic. Audience and late position requests
remain ordered; the second position query does not reuse the first answer.
Native class-table/UI constructor binding and Game activation remain incomplete.
The known Display Main blr is a linked provider boundary, not a rendered menu.

Host build exposed the already committed display-footer implementation missing
from HEAD CMake. Added its real source declaration to remove unresolved symbols;
no no-op replacements. Concurrent footer tests/quest-menu/render edits preserved.
Final isolated artifacts/tutorial-act-menu-xnbc3av2, based onc59eda21 plus
scoped helpers/link fix, passed `make test`:252/252 C++ and144/144 Python (no skips).
Native `make all -o assets-ready BUILD=build-act-menu-review
TARGET=ttyd-act-menu-review` passed with prepared private assets. Artifact:
that snapshot's ttyd-act-menu-review.3dsx (451924464 bytes). Logs:
artifacts/tutorial-act-menu-test.log and tutorial-act-menu-native.log.
Read-only act-menu verifier:24 bodies/3 spans/472 instruction anchors; six complete
source bodies/18 linked identities. Scoped implementation matched review and
diff check passed. No new asset conversion, original runtime observation,
Azahar/Dolphin or playable/visual class-menu claim in this step.

The interrupted previous turn completed commit `bf7a17b7`: full source command-
pose/status/body/animation helpers and native physical target B cancellation.
Normal status selects actual original M_C_3 through type67 lookup, not guessed
M_C_1. B wins over simultaneous A, closes target UI, sets state6/help0 and source
part flag80; the following invocation rebuilds state7's weapon list. Repeated
force0 publication preserves native pose timestamp. After UI presentation,
reselection and finalization still launch one original failed Jump: HP4/7,
failure/dealt1, one payment and result0. Weapon-to-class cancellation still needs
the class providers. Native status PoseAnime work remains separate from complete
status-clip/display consumption; no unproved animation completion is synthesized.

Imported unchanged original M_C_3 from a/b_mario/a/a_mario- into the existing
private tutorial pack. Existing mesh/animation conversion and half-size LANCZOS
texture choice retained; provenance/license recorded in ASSETS. Host sampled
actual command-pose vertices under12000. That step's final isolated
artifacts/tutorial-command-pose-j1wt02is (basef19f8ed2) passed251/251 C++
and144/144 Python, and native `make all -o assets-ready
BUILD=build-command-pose-review TARGET=ttyd-command-pose-review` passed.
Artifact451902760 bytes; mmap verified the actual updated Mario .pmanim embedded.
Logs: artifacts/tutorial-command-pose-final-test.log/final-native.log (native log
basename tutorial-command-pose-final-native.log). Verifier pinned8 complete
bodies, original priority27,2,9,1,4,5,3,-1 and actual M_C_3 lookup. Initial fixture
name assumption was corrected from source evidence; an unrelated stale coin-
fanfare manifest was refreshed with its existing original asset producer before
final checks. No ISO/source-payload commit, user profile/save mutation or visual
runtime proof. These validated changes were committed before interruption;
this entry supplies their previously missing session handoff.

Next: implement original class-table admission/eligibility and UI Init/End,
register Act-menu providers in Harbor Input, activate source command/first-act/
turn/post-action/phase scheduler in Game and render/smoke physical menus. Continue
original successful voice/pitch/volume/follow-up hits, damage/guard/hazards and
second-screen scene/audience expansion with optional hostile-item taps plus a
physical-button equivalent. The tutorial/real battle/full remake goal stays
active and incomplete; these are mechanics/provider advances, not completion.

## 2026-10-14 — Original Trouble Center selection producer

Committed a1dfd26a. New retained portable menu implements original30-slot story
unlock order, including20/21 before19, no completed-task filtering, exact A/B/
repeat/page/scroll/icon/SFX requests and next-call one-based selection publication.
Required original display/provider interface keeps native resources external;
no fabricated menu or acceptance writes. Source94 remains opaque; complete
original display does not use it. Copyguard and unknown-provider admission retained.

Source recovery corrected earlier acceptance prose: LW11 is one-based quest ID;
opcode115 is debug, notincrement; acceptance writes1420=ID after nonblocking
RUN_EVT of start_event[ID-1]. Original board/counter map cases, lecture, full
selection producer and Yes/No caller are documented. Complete window display/
64 US title/client/content resource provenance recovered; original frame/clipping/
colors/width rules now available for next native backend. No new asset pack.

Final isolated tree3239633f9328ade5d0fd2e6ef77d26439424567a passes make test
-o assets-ready (252/252 C++,144/144 Python) and normal make all -o assets-ready;
sequential assets-ready passed, converters unchanged. Logs artifacts/trouble-menu-
{assets,final-test,final-build}.log. Normal reviewed artifact SHA256
31499e76d8902f39d666fe01f7fbbe6942568ced17b290a5e6d412cbe33b1f60.
Initial host missing committed footer dependency registration/native mixed-width
min failures fixed and oldlogs retained. Read-only source review passes; audit
matches and concurrent edits preserved. No new native menu/hardware capture.
[Producer validation](behavior/field/PORTABLE_TROUBLE_CENTER_MENU_PRODUCER.md),
[original board](behavior/field/TROUBLE_CENTER_BOARD_SELECTION_AND_ACCEPTANCE.md),
[original drawing/US cache](behavior/display/TROUBLE_CENTER_ORIGINAL_WINDOW_DISPLAY_AND_US_CACHE.md).

Next: source window backend/icons/US cached strings and complete global switch
providers; source case9/door/cutaway ownership for actual east billboard (notNPC
or professor mask2); first lecture and confirmation/acceptance event. Consolidate
all30 help/confirmation table bindings, then validate acceptedquest2→parcel→
Goomfrey→McGoomba reward in isolated Azahar. Original GP38/pad/NDSP parity,
matched Dolphin/hardware performance and future original areas remain open.
Full-game goal remains active/incomplete; native board UI is not yet playable.

## 2026-10-14 — Native original McGoomba reward event

Committed d7b2c245. Game now admits source Goomba3 quest2/knowntrue5406 reward,
without5405/5376 admission guards. Original irai_02_02 attribution/order,
blocking coin20, quest1420clear→5376set→sys_request_finish speaker0 remain.
One outer NPC KeyOff holds through completion, then KeyOn. Reward EVT follows
camera publication and precedes current Mario; coin waits still visit Mario so
Get/Stay motions advance. Full coin service shape/post/resource readiness is
required before candidacy; copied Game cannot invoke live reward services.

Final isolated tree5c0faad74ecd588ac03b1f86848f3e2f3a8faef1 passes sequential
make assets-ready, make test -o assets-ready (250/250 C++,144/144 Python) and
normal make all -o assets-ready. Logs artifacts/reward-admission-{assets,test,
build}.log. Normal reviewed artifact SHA256
976e3b28e35d03178d877ee5be8cf409a7b2a0e2e1134576f0d9b51569bac5e1.
Read-only review passes; source audit matches and concurrent work preserved.

Isolated DEBUG_SHORT_BOOT Azahar reward capture/save passes using explicit
room0/story8/deliveredquest2, soundenabled/original100coin pouch and private DSP.
Saved v9: quest0/5376true/5406true/120coins/HP10/FP5, both checksums; further input
retains120. Inspected source dialogue, coin/window/pink effect and finish captures
under artifacts/azahar-goomba3-reward/. Log reward-admission-smoke.log. Debug
artifact SHA256046e6c6e0ed726f1a2475cf037eaef6b011749cadc9abfaa782d70769e7f14cb.
No reward instrumentation/forced transitions, new payload or user profile/save
mutation. Early startup capture black; later rendered stages supply the evidence.
Private source restored/audited; normal-main rebuild timestamp restored.
[Exact contract/evidence](behavior/field/NATIVE_GOOMBA3_REWARD_ADMISSION.md).

Next: original Trouble Center acceptance and complete acquisition→Goomfrey→
McGoomba reward validation from that accepted state. Original GP38 admitted-delta
pause clock and retained-input polling parity remain unbound; native audio tail/
matched Dolphin sound/pixels and hardware performance remain unverified. Continue
original area events/NPCs/objects using existing catalogs/providers. Full-game
goal stays active/incomplete; finite reward execution is now native playable.

## 2026-10-14 — Native retained coin presentation services

Committed50f5ceb1. Renderer now binds original coin window/403/type7/camera5,
M_I_U/Get/Stay, row219 shared NDSP and original pouchAddCoins to a Game-owned
stable FieldCoinGet. Per-frame service rebinding cannot overwrite an active
borrowed parameter. Window manager observes actual deletion then retires storage
independently of EVT return2. Overlay uses the original coin window; existing
field pass consumes original icon/effect. Readiness requires free icon, early
camera5 and exclusive shared resources, without parcel key-slot/row217 checks.
Texture/effect shader/vertex pool/atlas are reused; assets unchanged.

Camera placement uses original quantized angle table over current cam4 eye/at,
not renderer atan2 yaw. Native raw svcGetSystemTick uses source divisor units;
original GP38 admitted-delta pause/freeze publication remains unbound and this
clock is explicitly best effort. Prior audio detached-tail check remains open.

Final isolated tree037a54a0b71d081f75639085b1b96bd7a924097f passes make test
-o assets-ready (250/250 C++,144/144 Python) and normal make all -o assets-ready;
sequential assets-ready already passed, converters unchanged. Logs artifacts/
coin-provider-{assets,final-test,final-build}.log. Reviewed normal artifact
artifacts/ttyd-coin-provider-reviewed.3dsx SHA256
e5d86d111bc4e85d716c2596bcfd4619bd04a485ffb04e1fa4551ae1363a8d47.
Read-only final review passes finite resource/service scope. Source audit matches;
concurrent Game/battle edits preserved. No new emulator/hardware validation or
render/performance fidelity claim. [Exact provider scope](behavior/field/NATIVE_COIN_GET_PRESENTATION_SERVICES.md).

Next: bind original Goomba3 reward talk admission/EVT and outer NPC key hold to
these services and persistent5376, verify actual native reward presentation/save.
Implement original Trouble Center acceptance and test acquisition→Goomfrey→
McGoomba full chain. Add original GP38 admitted-delta owner and matched pause/
NDSP clock traces. Continue future-area events/NPCs/objects and hardware profiling.
Full-game goal remains active/incomplete.

## 2026-10-14 — Original reward5376 persistence

Committed b9310091. Appended optional completion5376 and native checkpoint v9
known/value bits24/25. Knownfalse and knowntrue write exactly1536 bytes using
existing dual checksums/pouch layout. Unknown state keeps previous version
selection. Versions1..8 import unknown, v8 retains strict reserved24..31 mask,
invalid value-only/reserved/payload records are rejected. Fresh swInit setsfalse.
Atomic backup flow and independent queued snapshots remain unchanged.

Source reward tests set5376 only after blocking coin return2, after quest1420
clear, and verify5376 does not become an invented admission guard. Read-only
parallel format/source review found no blockers. Final isolated tree
d810dfe0b19a7d7bab864417b809df6069b8d93d passes sequential make assets-ready,
make test -o assets-ready (250/250 C++,144/144 Python) and normal make all
-o assets-ready. Logs artifacts/reward-save-{assets,test,build}.log. Reviewed
normal artifacts/ttyd-reward-save-reviewed.3dsx SHA256
53ce6e34aa6fe0d65214bef9c9b054c7313a5c48181f86afafa6f7a429467842.
Source audit matches; concurrent changes preserved. No new assets or native
emulator/hardware evidence in this increment; previous audio-tail checks remain
unresolved. [Exact format/source contract](behavior/format/NATIVE_REWARD5376_CHECKPOINT_V9.md).

Next: wire Game reward coin/window/effect/icon/clock/audio/Mario services and
independent window deletion retirement to the new persistent5376 owner. Preserve
original reward admission/order and actual early camera5 publication. Implement
original quest acceptance, then verify acquisition→Goomfrey→McGoomba reward in
isolated Azahar. Continue original area events/NPCs/objects and hardware/render
profiling. Full-game goal remains active and incomplete.

## 2026-10-14 — Coin callback borrowed-window lifetime

Committed fcbe48db. FieldCoinGet now supports independent retirement after the
window owner confirms actual deletion in state101. Optional observation leaves
unbound providers retained; copied callbacks, early states and active closure
cannot release storage. Original state101/action query remains unchanged.
The actual CoinGetWindow integration test closes/deletes with action1 retained,
releases parameters without another grant/callback, and safely reopens the window.
Native Game/window-provider wiring remains pending; reward is not yet playable.

Reviewed isolated tree5fb57a5b3e6775acf245ffc331298914d218429f passes sequential
make assets-ready, make test -o assets-ready (250/250 C++,144/144 Python), and
normal make all -o assets-ready. Logs: artifacts/coin-lifetime-{assets,test,build}.log.
Reviewed artifact artifacts/ttyd-coin-lifetime-reviewed.3dsx SHA256
54fda8542d92c85e616fc5a3f71ade0048a9ae1950b2e3b3f9509ffe797c94fa.
Source audit matches the review tree; concurrent changes preserved. This step
uses existing assets, adds no payload and makes no Azahar/hardware timing or
rendering performance claim. Previous native audio tail checks remain unresolved.
[Lifetime contract](behavior/field/PORTABLE_GOOMBA3_REWARD_AND_COIN_CALLBACK.md).

Read-only parallel source/save review confirms5376 independent GSF GP+418 bit0;
reward clears1420 then sets5376 after acknowledged coin completion. It is not a
reward admission guard and quest acceptance does not clear it. Next: append
optional5376 state and native v9 checkpoint known/value bits24/25 while keeping
v8 reserved-bit validation and legacy unknown knowledge. Then bind real coin
window lifetime/audio/clock/motion services and Game reward admission, implement
original quest acceptance and verify acquisition→Goomfrey→McGoomba full chain.
Continue original events/NPCs/objects in future areas and hardware profiling;
full-game goal remains active and incomplete.

## 2026-10-14 — Complete source command Input and native target confirmation

Committed `a7557ea2`. Shared BattleCommandInput now implements all30 states and
the unsigned default with original request identities/arguments, captured B/U/M,
unit lookup before wrapped-M-zero, one private cursor output, exact setup flags,
reason-entry ID publication/current-ID waits, fallthroughs and local progress.
The pending-byte re-entry path preserves P; normal re-entry clears it. No loop
limit/one-state-per-frame yield or VM completion2 is substituted. State11 uses
the existing complete target core; state29 selects the source attack, freshly
clears live menu/battle flags, requests OffLight(0,0) and returns1 without a root,
turn consumption or battle/story result synthesis.

Native inputCommand(sample) composes the owner over existing menu providers.
Explicit confirmed-class openWeaponMenu admits state7. Weapon A progresses to
actual help key/mode1/state11 in the same call; a later target A stores29/help2;
the following invocation finalizes the original script. The root-registration
bridge consumes that selection once without selecting it again. Physical A/B/
repeat words come from the actual general pad. Target cancellation is rejected
before effects at its missing CommandAnimPose provider. In-body missing providers
freeze the native command context, and legacy root launch refuses that context,
so a later call cannot advance from a partial state write or replay lost SP08/P.
A new explicit menu build establishes a new context. The real weapon-menu B path
closes/stores1, then freezes at the unbound class Setup provider; it is not falsely
reported as a completed cancellation/menu return.

Complete native _commandRestoreRec is bound; present-item admission uses actual
source getters/retained unit lookup and only complete early exits (item>=339,
invalid target, or actual None item0). Gift/effect paths, operation payment and
populated audience entity producers remain unbound. Source light blr leaves and
whole help/cursor/sampling/attack-selector providers are reused. UI constructors/
animations remain native presentation adapters. This is an explicit native
provider bridge, not yet automatic source class/turn/menu activation in Game.

Validation: isolated artifacts/tutorial-command-8lfuey83, based on5f82f7e4 plus
scoped changes, passed final `make test`:250/250 C++ and142/142 Python (no skips).
Portable tests exercise every state, shared cursor reuse, changed type/actor
values, paired setup/fallthrough policy, reason IDs, retained owners, priority,
fresh final flags and P-preserving pending re-entry. Native fixture drives the
weapon/target/finalization sequence through one root and then actual failed Jump
completion: Crump HP4/Mario7, dealt1/failure1, one payment and result0. It checks
pre-admission target B retains UI and in-body class cancellation freezes across
calls/root-launch attempts. Final native `make all -o assets-ready
BUILD=build-command-review TARGET=ttyd-command-review` passed using prepared
private assets; artifact: that snapshot's ttyd-command-review.3dsx (451598064
bytes). Logs: artifacts/tutorial-command-final-test.log and
tutorial-command-final-native.log (same artifacts directory). Read-only Input
verifier passed39 bodies/3 spans/526 instruction anchors. Final scoped source/
fixtures matched isolated review; diff check passed. Initial fixture compile
scope error was fixed before all final checks. No new art/music conversion,
ISO writes, Azahar/Dolphin or measured/runtime visual-playability claim.
Independent audio/coin/Frankly/pose/field work was preserved by scoped commits.

Next: complete CommandAnimPose/body/status/animation providers so target B can
return to the weapon menu; original ActMenuSetup/Main/class table/UI construction
and BattleCommandInit/first-act/turn/post-action/phase owners; wire that scheduler
into Game and render/smoke the original physical menus; successful voice/pitch/
volume/follow-up hits and stage hazards; second-screen scene/audience expansion,
optional hostile-item taps and physical equivalent. Tutorial/real battle and
full remake requirements remain incomplete.

## 2026-10-14 — Native original coin fanfare219

Committed c3b72f6d. Coin row219 now uses original ff_get4 stereo32000/65352-frame
PCM and COMMON nonloop metadata through the shared retained group1 provider.
Exact readiness/profile gain, source envelopes and actual NDSP drain are reused;
preparation requires a free slot and preserves group0 field music/cursor. Asset
provenance/licensing recorded in ASSETS; no payload/binary/ISO committed.

Reviewed isolated tree17d43265e3d7d23205b213676c824d2af2128f7f passes sequential
make assets-ready,249/249 C++ and143/143 Python via make test -o assets-ready,
and normal make all -o assets-ready. Normal reviewed artifact SHA256
886742a5c7002bef7fe72e53701183ef3873c77f26f5cf2beba70f16e23714cb.
Logs: artifacts/coin-audio-{assets,test,build}.log. Read-only review passes
fresh-admission scope.

Both isolated instrumented Azahar probes FAILED detached-tail assertions.
Natural EOS/vacancy and retained group0/cursor were observed; independent
checkpoint comparison is byte-identical. A second probe used60 owner calls in
a burst and does not prove physical cadence. No source/hardware fidelity,
physical1000ms fade or tail-drain claim. Private sources restored/audited;
user emulator profile/saves untouched and concurrent changes preserved.
[Exact contract and evidence](behavior/audio/NATIVE_COIN_FANFARE_ROW219_PROVIDER.md).
AUDIO-01 requests matched clock/NDSP traces; best-effort provider remains.

Next: resolve native tail timing check, retire borrowed coin params on actual
window deletion, persist5376 and bind Game reward admission. Implement original
quest acceptance and validate complete acquisition→Goomfrey→McGoomba reward.
Continue original areas/events/objects and actual hardware profiling. Active
full-game goal remains incomplete.

## 2026-10-14 — Native original rotating coin and camera publication

Committed `200e326d`. Actual coin403 now retains source frame0/414/delay6,
cycles all four original frames every six icon-owner calls even while hidden,
and preserves phase on reveal. Queued resources resolve/advance in the same
visit; named setters/delete retain first-active duplicate behavior. Renderer
reads published frame state and reuses the existing icon shader/vertex pool.
Original40x40 pixels414..417 fit unchanged in the same512x512/1MiB atlas with
2px padding. No generated art/resampling or larger native allocation.

Native icon visit now follows Game item owners and precedes sound/windows/
effects. Game publishes camera5 immediately after camera4 before acquisition
EVT/Mario; drawing consumes this snapshot, correcting previous render-time
camera5 rebuilding. Existing up/aspect/math/shadow/global-context limits remain.

Final isolated treeff860bbe78016b77ae9cf4f9bd72b18dc7831f70 passed make test:
247/247 C++ and142/142 Python; make all passed. Logs ignored
artifacts/coin-icon-test.log and coin-icon-build.log. Normal artifact
artifacts/ttyd-coin-icon-reviewed.3dsx SHA256
d11a48afceb38a4fa482dc60fc826d2fa9d72d321ba3548ac3645a7b0e066d9f.
Read-only source review and animation/duplicate/camera snapshot checks pass.

Clean DEBUG_SHORT_BOOT parcel capture/save passes: one95,5405true/5406false,
quest2,HP10/FP5 retained, using explicit room0/story8 test state/private DSP.
Log artifacts/coin-icon-parcel-smoke.log. Private simulation-driven raster
preview shows original coin rotating with source title/pink effect, records18
phase captures and proves unchanged checkpoint bytes. Evidence:
artifacts/azahar-coin-icon-raster/ and coin-icon-raster-smoke.log. Preview adds
no reward/audio/Mario-motion/quest writes and validates no complete quest.
Private source restored; audit matches. No user emulator profile/saves touched,
matched Dolphin/hardware parity or stable60 claim; concurrent work preserved.

Next: bind distinct row219 ff_get4 PCM/NDSP, native parameter retirement on
actual window deletion,5376 persistence and Game reward admission. Complete
original task acceptance and verify acquisition→Goomfrey→McGoomba reward in
isolated Azahar. Continue original areas/events/objects and actual hardware
profiling. Goal remains active; full game remains incomplete.
[Exact source, assets, validation and limitations](behavior/display/NATIVE_COIN403_ANIMATION_AND_CAMERA_PUBLICATION.md).

## 2026-10-14 — Original TimingA display and A-button HUD icons

Committed `17b6bced`. Complete shared original battleAcDisp_TimingA8012a7cc/f8
and draw helper8012a6b0/110 now preserve source state gates, state99 interpolation
followed by unconditional30 overwrite, signed entrance/exit counter conditions,
fresh post-provider reads, separate caller/global owners and32-bit wrap. Draw
captures global B before camera8 lookup, freshly selects108/109 from B1f50 bit0,
copies original XYZ templates and requests source offsets(-200,50), scale1 and
flags16, then increments its retained owner's B1f20. This counter does not alter
TimingA result/count or Main's finish countdown.

Harbor admits its known borrowed camera8 callbacks after the event pass once per
physical simulation sample and publishes retained icon requests. Missing art or
unknown identity/owner reports actionDisplayPending before callback effects;
render retries do not mutate source display work. Native overlay consumes these
requests through the existing original-pixel atlas at centered half-size/cropped
400x240 coordinates. Native C2D rendering adapts original camera/GX; no complete
iconGX material/matrix or original-runtime visual alignment claim is made.
Physical A remains the input. Actual native Jump fixtures emit both original A
icons; failed Jump still completes, deals1 HP and produces original feedback,
and successful Jump still reaches the existing voice80104508 frontier.

Imported original icon.bin108/109 descriptors2442/2454 and icon.tpl108/109:
one non-looping48x48 frame each. Original pixels fit the existing512x512 atlas
with2px transparent padding and no resampling. Updated both ignored local and
isolated review packs, retaining newly committed coin403. Conversion/provenance
and copyright restrictions are recorded in ASSETS. No ISO or original payload
was written into source history; no generated art/tone or dialogue rewrite.

Validation: final isolated artifacts/tutorial-display-0d60usfi, refreshed to
297ba204 plus scoped changes, passed `make test`:249/249 C++ and142/142 Python
(no skips). Tests cover source callback state gates/fallthrough, mutable provider
owners/counters, missing providers, wrap, actual native icon requests, repeated/
absent input, unchanged failed-Jump feedback and original atlas pixels/UVs. Final
`make all -o assets-ready BUILD=build-display-review TARGET=ttyd-display-review`
passed with prepared private assets; artifact is that snapshot's
ttyd-display-review.3dsx (451596532 bytes).
Logs: artifacts/tutorial-display-final-test.log, tutorial-display-final-native.log
and tutorial-display-final-assets.log. Read-only display verifier passed3 whole
body hashes,5 constants,2 templates and both icon descriptors; startup r2 base
also pinned. Original listings remain in ignored tutorial-timing-display/draw/
icon artifacts. Final source/asset converter matched scoped review; diff check
passed. No Azahar/Dolphin or rendered screenshot/runtime cadence measurement.
Independent coin, shadow, pose and field edits were preserved.

Next: original class/target menu activation and turn/post-action/phase scheduler
in Game so these battle requests are reachable through normal play; rendered
cue/menu smoke fixtures and visual comparison; successful voice/pitch/volume,
follow-up hits and active stage hazards; second-screen scene/audience rendering
with optional hostile-item taps and a physical-button equivalent. Tutorial and
real battle/full-game goals remain incomplete.

## 2026-10-14 — Native tutorial action-command manager and deferred cleanup

Committed `36aee76a`. Harbor now runs the complete shared original action-command
manager once per physical simulation sample before its event-manager pass.
General input samples first; the manager owns the private-pad update, including
null-Main calls. Repeated timestamps do not shift history or tick Main again.
Missing physical samples leave pad/command work intact rather than synthesizing
no input. Native TimingA8012aae0 uses the existing source state machine: Stop's
1000 sets60 and decrements59 immediately,59 further calls reach1002, then the
next call clears main/display/result/delete in original write order. The outer
manager retains its independent zero-return main-only clear and fresh display
lookup. Unknown main identities remain an explicit provider frontier.

Original camera8/class1/priority15 display requests now enter a runtime-owned
DisplayQueue, exposed as borrowed read-only requests for the current simulation
tick. Tests assert the actual callback8012a7cc, retained BattleWork argument and
sort key399985. The queue is not yet consumed by an action-command draw backend.
The failed original Jump still completes its attack/recovery root, applies1 HP
and produces the actual failure count/original mac_0_b_t1_002 feedback through
explicit phase dispatch; it now also cleans up all four deferred callbacks.
Repeated-timestamp and absent-input fixtures protect countdown and private pad.
Success still stops at voice80104508; battle/story result remains0.

Validation: isolated snapshot artifacts/tutorial-manager-9ae6m6zm, based on
ff9d4b64 plus the scoped changes. Final `make test` passed246/246 C++ and142/142
Python tests. Final native `make all -o assets-ready BUILD=build-manager-review
TARGET=ttyd-manager-review` passed using existing prepared private assets;
artifact: that snapshot's ttyd-manager-review.3dsx (451352076 bytes). Final logs:
artifacts/tutorial-manager-final-test.log and tutorial-manager-final-native.log.
The initial full snapshot run also passed; final rerun covers the cleanup-order
adjustment. Read-only ISO checks passed46 motion/action/facing spans and the
input-cadence verifier's11 body hashes/154 anchors/28 calls/3 data bindings.
Original complete TimingA Main listing inspected at
artifacts/tutorial-manager-timing-main.txt. Diff check passed. No new asset
conversion, ISO writes, Azahar/Dolphin, measured frame timing or rendered cue
claim. Independent field/trio/display/shadow edits were preserved by a scoped
seven-file commit.

Next: original action-command HUD display consumption and source class/target
menu activation in Game; successful voice/pitch/volume and follow-up hits;
post-action/turn/phase scheduler and system-window restoration; active stage
hazards; second-screen scene/audience rendering and optional hostile-item taps
with a physical-button equivalent. This is native manager admission, not the
complete original BattleMain or a finished tutorial/remake.

## 2026-10-14 — Original coin window/effect7 and native camera5 fix

Committed `f82d32cc`. Original coin window80415e84 loads unchanged US title,
redNUM tokens and single-pane geometry. It gates before amount/measurement,
allows text during transition4 and caches expanded text/glyphs/measurement.
Shared native frame/font path serves coin and parcel. Effect kind7 retains
original .75/pink E85BD0, sharing original42/44 and PICA shader with kind6.
Source review found previous native EffectPayload5 was incorrectly a no-op;
actual Game now writes effect camera5 and renderer supplies that selected view.
Original coin403 documentation now pins four duration6 frames, animation even
while hidden, source camera5 and bottom-anchored24x24 geometry.

Final scoped tree4dc27d8aaf31a8e55c75e46efce4bdc60194f87f passed make test:
244/244 C++ and142/142 Python; make all passed. Normal artifact
artifacts/ttyd-coin-window-reviewed.3dsx SHA256
1136ad7461f9bc70c5b901a51feea0686a7fca798b73c72fd0062007720bcaba.
Logs: ignored artifacts/coin-window-scoped-test.log and scoped-build.log.
An earlier private snapshot included incomplete concurrent camera calls and
failed; final scope excludes those calls without changing foreign work.

Clean DEBUG_SHORT_BOOT parcel capture/save checks passed twice after camera5
fix: one95,5405true/5406false,quest2,HP10/FP5 retained. Held original windows/
effect capture: artifacts/azahar-coin-window-camera5-held/parcel-open-window.png;
log artifacts/coin-window-camera5-held-smoke.log. Private fixed-pose raster
preview shows original coin title/red20 and pink effect7 in
artifacts/azahar-coin-window-raster/boot.png. It supplies no reward/icon/audio/
switch writes and proves no native reward playability. Private diagnostic code
was restored; final source audit matches. Only isolated profiles/test saves
were used. No new asset payload/conversion or matched Dolphin/hardware check.
Emulator frames are not stable60/hardware performance evidence.

Important cleanup boundary: original AutoDelete clears active1 but retains4,
so deleted window action can remain1. Native borrowed coin-parameter retirement
must observe actual deletion separately, not delay reward writes waiting for
an unmodified state101 action0. Native icon403 playback, row219 PCM/NDSP,
flag5376 save owner and Game reward admission remain next; complete original
quest acceptance and verify full acquisition→hand-in→reward chain afterward.
Continue areas/events/object coverage and real-hardware profiling. Goal active.
[Exact implementation/validation](behavior/display/NATIVE_COIN_WINDOW_AND_EFFECT7_PROVIDERS.md).

## 2026-10-14 — Failed original Jump script completion and real feedback

Committed `e2740e06`. Complete shared WeaponAftereffect preserves weighted byte
load/subtraction/reread order, force/inhibition, conditional B restoration,
ordered stage requests, three mandatory chance draws, fresh force short-circuits
and object gate/optional draw. Native admission previews with a copy of retained
RNG and only complete non-mutating source leaf exits; unsupported hazard event
producers stay pending before live draws. It never rerolls for another outcome,
skips a selected hazard or rewinds live RNG. Actual dispatch runs once after
admission. Active destruction/nozzle/fall event backends remain incomplete.

Native HP/FP Drain amount wrappers now translate the RAW actor, retain work/
output and use actual badge byte/dealt HP signed min, including null/negative
semantics. Fresh zero badge counts naturally skip recovery; they are not a
universal hardcoded0. The original failed Jump now executes aftereffect,
bounce/return and its recovery child through actual root DELETE. Mario returns
to home/M_S_1 with restored wait handle; attack/damage roots are no longer live,
Crump HP stays4, Mario dealt1/failure1 and battle/story result0 remain. Deferred
TimingA state1000/main callback are still retained for their separate manager.
No turn consumption, source victory or tutorial completion is fabricated.

A controlled original phase dispatch using the produced count then prints
original US mac_0_b_t1_002 after mac_0_b_t1_000 and sets the actual failure
once-latch, without manually setting success/failure counters. This verifies
the failed-attack/feedback script path under explicit scheduler admission;
Game does not yet automatically schedule the menu/turn/phase loop. The successful
Jump still waits at original voice80104508; backend/UI/visual timing remain open.

Validation: isolated snapshot /home/carson/Documents/blender_tests/pm-port/artifacts/tutorial-aftereffect-gdofmq63, based on56e612ac plus scoped code/tests,
passed `make test`:245/245 C++ and142/142 Python tests. Native
`make all -o assets-ready BUILD=build-aftereffect-review
TARGET=ttyd-aftereffect-review` passed with prepared private assets. Artifact:
that review folder's ttyd-aftereffect-review.3dsx. Logs: ignored
artifacts/tutorial-aftereffect-full-test.log and tutorial-aftereffect-native.log.
Read-only aftereffect verifier passed13 bodies/7 data spans/56 anchors; badge
tail verifier18 bodies/32 commands/13 spans/71 anchors. Final isolated code/tests
matched scoped files; diff check passed. No new payload/conversion, Azahar/
Dolphin, measured original timing or complete playable/visual/audio claim.
Concurrent field/camera/display work preserved in scoped commits.

Next: bind original voice/pitch/volume and successful follow-up hits, active
stage hazards, native private-pad/TimingA main/display and post-action scheduling.
Connect original class/target Input/re-entry and restored HUD window to Game,
consume original poses, render original HUD/menu with physical controls and
second-screen scene/audience, plus optional hostile-item taps with a physical
alternative. Verify actual play in isolated Azahar. Goal remains active;
full tutorial/battle systems and full game remain incomplete.

## 2026-10-14 — Original Goomba3 reward caller and finite coin service

Committed `9172923a` and `b41495e1`. Portable reward retains original
irai_02_02 attribution, blocking coin20, quest1420=0, flag5376 and original
sys_request_finish speaker0. Source reward guard reads5406, not5405. Finite
coin callback preserves per-call Mario/camera reads and polynomial placement,
strict GP-low wrapped/signed >200/>500 waits, A-only state10→100, current
amount grant, resumeA0-before-Off201 and source return2 before window retirement.
Pouch coin arithmetic stores low16 before signed clamp. Stable parameter
storage stays borrowed until explicit state101 cleanup; no native gameplay
reward is advertised before its presentation/switch providers exist.

Source documentation now pins the complete callback/window/title/icon403/
effect7 and distinct row219 ff_get4_32k stream: stereo32000Hz,65352frames,
COMMON restartFFFFFFFF/nonloop despite DSP loop flags. No key-fanfare/color/
icon substitute or rewritten dialogue is introduced. New native icon lifecycle
and reward-binding requests are in ITEM-01. Existing Goomfrey/Goomba4 quest2
hand-in is already implemented; the earlier parcel handoff's “delivery/reward
unbound” wording should be read as remaining end-to-end validation/reward
providers, not absence of that hand-in owner.

Validation: final isolated tree b5e0c2fa5e74c25aa35aa4c3482a7a0af9180b13 passed
make test:242/242 C++ and142/142 Python; make all passed. Native artifact
artifacts/ttyd-field-coin-reviewed.3dsx SHA256
eb98e3fed60da05043601c011b7108c8aadbe9f3c54d7ff49af2371b0e91b0a2.
Logs: ignored artifacts/field-coin-final-test.log and field-coin-final-build.log.
Reward-only prior snapshot passed241 C++/142 Python/native build. Source review
caught C++ argument read order; explicit locals and regression checks now pass.
Tests compose actual reward/coin owners with original message bytes, source
clock/grant boundaries and delayed parameter cleanup. Native compilation only;
no Azahar/Dolphin/hardware rendering/audio performance observation this increment.
Concurrent changes preserved through scoped snapshots/commits; no asset payload.

Next: bind native coin window, descriptor403's original four-frame icon owner,
effect7 and row219 PCM/NDSP stream with retained cleanup; add original5376 switch
persistence and actual Game admission. Complete upstream quest acceptance and
verify acquisition→Goomfrey hand-in→McGoomba reward in isolated Azahar. Continue
original areas/events/object coverage and hardware profiling. Goal stays active.
[Contracts, evidence and limits](behavior/field/PORTABLE_GOOMBA3_REWARD_AND_COIN_CALLBACK.md).

## 2026-10-14 — Complete AP report build and invitation cleanup

Committed `a94b41e6`. Shared ApRecoveryBuild preserves12-byte allocation/reuse/
copy, retained Mario/party lookups, all13 audience-kind queries, original source
weights/sqrt/single rounding and ordered factors/slot bonus. It captures the old
accumulator at the branch-specific source site and wraps the truncated addition.
Even empty reports consume the100-bound RNG draw; fresh chance byte controls the
separate point-increment provider. Complete caller clears its retained12-byte
record only after build readiness. Kind count uses actual presence/state/kind
gates, distinct from Waiting. No empty-audience shortcut, invented report or
immediate pouch SP grant is introduced.

Native Harbor uses its SDK heap0 as explicit allocation policy, actual source
fields/table/math/getters and a zero-chance admission preflight while bingo
PointInc remains unbound. The failed original Jump now allocates/retains its
report buffer, clears the source input record and advances to WeaponAftereffect
801054e8, retaining Crump HP4, Mario dealt1 and failure1. Success still waits at
voice80104508; deferred StopAC/main/display scheduling and story outcome remain
separate. Native report lifetime ends with its battle runtime; no resource
ownership beyond this adapter is inferred from the source allocator call.

Validation: isolated snapshot /home/carson/Documents/blender_tests/pm-port/artifacts/tutorial-ap-vho_ojsu, based on270abd2b plus scoped files,
passed `make test`:241/241 C++ and142/142 Python tests. Native
`make all -o assets-ready BUILD=build-ap-review TARGET=ttyd-ap-review` passed
with prepared private assets. Artifact: that review folder's ttyd-ap-review.3dsx.
Logs: ignored artifacts/tutorial-ap-full-test.log and tutorial-ap-native.log.
Expanded read-only ISO verifier passed46 whole motion/action/audience/report/
getter spans. Final isolated code/test/verifier matched scoped files; diff check
passed. Tests cover allocation/reuse/copy/clear, missing allocation, weighted
factors/slot bonuses, RNG/point provider and presence gates, plus original script
buffer/pending state. Initial live build hit an unfinished concurrent camera
edit; isolated committed sources validated the battle changes. No Azahar/Dolphin,
new payload/conversion, measured timing, native visuals/audio or full playable
attack/tutorial/battle claim. Concurrent edits preserved through scoped commits.

Next: bind original WeaponAftereffect and attack finish, voice/pitch/volume and
bingo point producer; compose native private-pad/TimingA main/display queue and
post-action control. Connect original class/target Input/re-entry to registration,
consume original poses and render original HUD/menu with physical controls.
Complete second-screen scene/audience and optional hostile-item taps with a
physical equivalent, then verify actual play in isolated Azahar. Goal stays
active; tutorial/battle systems and full game remain incomplete.

## 2026-10-14 — Native original Goomba3 parcel acquisition

Committed `3abf5525` and `d8efdb46`. Actual Game owners now bind original US
irai_02_00, item95 receive/registry deletion, original name/help windows, M_I_U,
Box icon, effect6 and original fanfare/retained field BGM. Camera8 bit200 stays
separate from camera4; source level2 mask3 stops the type6 talk root while item
Main continues. NPC talk/item KeyOff depths1→2→1→0 preserve separate holds.
Windows, effect and audio advance in simulation order, independently of draw.
Readiness refuses missing assets/DSP/free audio slot/full key storage before
print. Original ISO music is now the runtime default. M_I_U textures stay
prewarmed instead of being reimported per frame. Portable service injection
remains available for later area/item providers. No asset payload is committed.

Final isolated review tree d32b5f5c67c83fe34e41f2c1e5e67379bb3e8199 passed
make test:232/232 C++ and141/141 Python; make all passed. Logs are ignored
artifacts/item-native-key-test.log and item-native-key-build.log. Final native
artifact artifacts/ttyd-item-native-key-reviewed.3dsx has SHA256
4d2d9c4d8459836a9fc24d9d0e8a9fb437ea27f5c3ca466335bc7a9cf4b7f638.
DEBUG_SHORT_BOOT isolated Azahar run passed: original dialogue/item presentation,
return and save prove one95,5405true/5406false,quest2 and HP10/FP5 unchanged.
Evidence: artifacts/item-native-key-final-smoke.log and
artifacts/azahar-item-native-key-final/. Fixture supplies room0/story8/known
quest2, bypassing Trouble Center acceptance; private existing DSP firmware was
copied only into the isolated profile. No user profile/saves were touched.

Limits: source letterbox consumers/global event manager, complete audio macros,
full-storage/discard UI, quest acceptance and Goomfrey delivery/reward remain
unbound. Inline additiveX/Z/combined norm have no retained native producers.
Effect mask appearance is consistent with original42/44, but original pixel/
audio parity and New2DSXL hardware performance remain unverified. Emulator
performance varies; no stable60 claim. Native source tracing was private and
restored. Other sessions' source/index changes were preserved in scoped commits.

Next: consume ITEM-01's full-storage/shared-window and additional-area pickup
requests, complete original quest acceptance/delivery, compare unoccluded
original/native effects and profile render/asset allocations on real hardware.
[Binding and exact validation](behavior/field/NATIVE_GOOMBA3_PARCEL_ACQUISITION.md).
Goal stays active; broader original events/NPCs/objects and full-game work remain
incomplete.

## 2026-10-14 — Original tutorial feedback declaration and audience gates

Committed `76a0ed97`. Shared action declaration preserves exactly-1 bad grading,
200-byte saturation without repairing255, classification, source good-record
weapon/bytes and fused grade multiplier. Audience bad/good helpers use all200
actual waiting queries, captured base/fresh flags and original populated-case
request order. Waiting retains GetExist/GetSysCtrl/halfword/state gates and
fresh getter ownership. Native Harbor preflights zero waiting actors before
count publication; required populated-case providers remain unbound.

Capture now retains live declaration bytes across scene publication. The
original failed Jump itself produces failure1 after its impact, keeps Crump HP4
and Mario dealt1, and advances to InviteApInfoReport8010436c. Repeated ticks do
not replay the count/hit. Actual AcrobatNotry80139898 has a base getter-only
callee; AP-recovery80139af4 is literal li2/blr. Those complete local bodies now
admit their source routes without fabricated SP recovery. Success remains at
voice80104508. Story/battle result stays0; no turn/tutorial completion is claimed.

Correction: earlier progress text called pending8010749c AcrobatNotry. Its
actual identity is AudienceDeclareACResult; AcrobatNotry is80139898. Execution
and assertions used the actual addresses. Native damage documentation is now
corrected. InviteApInfoReport calls actual ApRecoveryBuild801a1d0c and clears12
bytes afterward; its allocation/record/amount/backend services remain the next
boundary, rather than an invented report or immediate reward.

Validation: isolated snapshot /home/carson/Documents/blender_tests/pm-port/artifacts/tutorial-feedback-a7_ps2h9, based onebf83bc6 plus scoped code/tests,
passed `make test`:240/240 C++ and142/142 Python tests. Native
`make all -o assets-ready BUILD=build-feedback-review TARGET=ttyd-feedback-review`
passed with prepared private assets. Artifact: that review folder's
ttyd-feedback-review.3dsx. Logs: ignored artifacts/tutorial-feedback-full-test.log
and tutorial-feedback-native.log. Expanded read-only ISO verifier passed40 whole
motion/action/audience spans, including full declaration/waiting/bad/good and
literal stub bodies. Final isolated code/test/verifier matched scoped files;
diff check passed. No new payload/conversion, Azahar/Dolphin, original-runtime
measurement, native visuals/audio or playable battle/tutorial claim. Concurrent
field/camera/display work preserved through scoped commits.

Next: bind AP report construction, original voice/pitch/volume, aftereffect and
attack finish; compose native private-pad/TimingA main/display and post-action
control. Bind populated audience reactions and original class/target Input/
re-entry, consume original poses and render original HUD/menu with physical
controls. Complete second-screen scene/audience and optional hostile-item taps
with a physical equivalent, then verify actual play in isolated Azahar. Goal
remains active; full tutorial/battle systems and full game remain incomplete.

## 2026-10-14 — Original Mario impact and defender sound attribution

Committed `ecb4226c`. Harbor damage now retains attacker/defender IDs through
power, HP/FP calculation, direct application, flags/dealt HP and actual hit-event
registration. Existing Crump2→Mario1/default power remains; Mario1→Crump2 admits
original Jump/Hammer callback identities with live action bit, current pouch
level, actor bonus byte and source weapon power-table reads. No invented level,
success power or lower-index clamp is supplied. Native read-only base-power
preview supports the existing lethal preflight policy; status/counter/guard/
FP/inventory/other-unit domains remain explicitly bounded.

Damage sound metadata now identifies the actual defender and matching original
kind sound-table entry. Its synchronous owner is restored after application;
reactions no longer universally get Mario's identity and first sound key.
The source-script no-action-press Jump fixture applies1 damage: Crump HP5→4,
Mario U284=1 and real Crump U2b4 reaction. It stops at AcrobatNotry8010749c;
repeated ticks retain HP4/dealt1 without replaying the impact. Native fixture
assertions also retain original owner2 sound keys. Existing Crump impact and
retained lethal/guard checks pass. Success still waits at voice80104508 with
Crump HP5. Story/battle result remains0; no turn/tutorial completion is invented.

Validation: isolated snapshot /home/carson/Documents/blender_tests/pm-port/artifacts/tutorial-hit-7pc2paby, based on5a3784ae plus the three scoped
code/test files, passed `make test`:236/236 C++ and142/142 Python tests. Native
`make all -o assets-ready BUILD=build-player-hit-review
TARGET=ttyd-player-hit-review` passed with prepared private assets. Artifact:
that review folder's ttyd-player-hit-review.3dsx. Logs: ignored
artifacts/tutorial-player-hit-full-test.log and tutorial-player-hit-native.log.
Read-only orchestration verifier matched complete BattleCheckDamage/Pikkyoro/
presentation spans and13 anchors; return verifier retained17 ordered entries,
5 sentinels and21 native hierarchy bindings. Final snapshot matched scoped code/
tests; diff check passed. No new asset conversion/payload, Azahar/Dolphin,
original-runtime measurement or complete playable attack/battle/visual claim.
Concurrent field/display/camera work preserved through scoped commits.

Voice source inspection explains its remaining boundary: wrapper picks from
original5/10-key tables using RNG; unit sound derives position and size-status
pitch, consumes a41-bound draw, starts3D sound, sets pitch and gets/sets live
volume for its sentinel. Do not bypass those operations with a no-op or invented
volume. Listings remain ignored artifacts/tutorial-voice-source.txt,
tutorial-unit-sound-source.txt and tutorial-position-sound-source.txt.

Next: bind complete original voice/backend pitch/volume, AcrobatNotry/audience
and source hit/aftereffect/finish services. Compose private-pad/TimingA main/
display queue and post-action control, preserve deferred StopAC cleanup, and
connect original class/target Input/re-entry to registration. Render original
HUD/menu with physical controls and second-screen scene/audience, including
optional hostile-item taps with a physical alternative. Verify actual play in
isolated Azahar. Goal remains active; tutorial/battle systems and full game
remain incomplete.

## 2026-10-14 — TimingA result evaluation and deferred StopAC

Committed `a4def146`. Shared ResultAC captures work, calls its result provider or
reads null-callback result&2, then rereads the live pending bit independently;
scalar returns never substitute for that stored bit. Help0 resolves its own
current help/audience-window owners. StopAC invokes the retained delete callback
before help0 and preserves callbacks/result/count for deferred cleanup. Stored
result/count readers preserve raw destinations and signed words without any
evaluation/reset. Unknown providers remain explicit completion frontiers.

Harbor admits actual TimingA result8012a8c4 using original options, actor flags/
automatic byte, battle flags, general trigger history, difficulty, result/count/
classification and RNG. Ordinary history evaluation requires a current physical
producer; missing input is not interpreted as no press. No-pad source routes
retain their own admission. Actual delete8012a7c0 stores1000 without prematurely
removing callbacks. Complete native manager/private-pad/display ownership and
finish countdown scheduling remain unbound; this is not full TimingA cadence.

Tests prove ignored scalar return versus fresh pending bit, null fallback and
separate reread, help owner recapture, missing providers, deferred stop keeping
callbacks/count, and raw readers. Native source-script fixtures age menu A out
of general history, supply physical samples per simulation visit and establish
no-press result0/count0/class2 versus one-window-press result2/count1/class3.
Both clear help mode through source effects. With missing physical input Jump
still waits at ResultAC. With samples failure reaches CheckDamage8010a78c;
success reaches original voice80104508. HP and story result stay unchanged at
these service frontiers. These are controlled fixtures, not original measured
impact timing, a playable Game flow or completed battle/tutorial evidence.

Validation: isolated snapshot /home/carson/Documents/blender_tests/pm-port/artifacts/tutorial-result-rvjxv33f, based on20b93109 plus scoped files,
passed `make test`:235/235 C++ and142/142 Python tests. Native
`make all -o assets-ready BUILD=build-result-review TARGET=ttyd-result-review`
passed using prepared private assets, with final result-reader rebuild passing.
Artifact: that folder's ttyd-result-review.3dsx. Logs: ignored
artifacts/tutorial-result-full-test.log, tutorial-result-native.log and
 tutorial-result-native-final.log. Read-only ISO verifiers passed30 whole motion/
action/protocol spans; ordinary Jump19 bodies/268 commands/57 anchors; existing
Jump contracts10 fingerprints/55 instructions/9 constants. Final isolated source
matched scoped code/test/verifier files; diff check passed. An operand-service
type mismatch was fixed before green builds. No Azahar/Dolphin, new payload or
conversion, original-runtime timing or native visual/audio fidelity claim.
Concurrent field/display changes were preserved in scoped commits.

Next: bind original voice and Mario damage/power/source reactions, complete
native private-pad/sequence/TimingA main/display queue, then StopAC finish and
post-action control. Connect original class/target Input and re-entry to player
registration; consume original poses, render original HUD/menu with physical
controls and finish second-screen scene/audience plus optional hostile-item taps
with a physical alternative. Validate play/visuals in isolated Azahar. Goal stays
active; full tutorial/battle systems and full-game work remain incomplete.

## 2026-10-14 — StartAC/help, shared manager and Mario motion

Committed `751b81d5`. Complete source StartAC captures battle before resolving
on, stores state100 and conditionally requests help2 from the retained actor
token. Help key/display setters preserve current-owner recapture and actual
inline audience-window active byte; they do not alter crowd target/count.
Shared ActionCommandManager implements pad→main→fresh display order, null-main
display skip, exactly-zero clearing ONLY main, and camera8/class1/userB/priority15
registration. Missing services remain non-resumable provider frontiers after
prior effects. The obsolete speculative manager reference was replaced with
these inspected source rules.

Harbor now binds the existing complete MarioJumpPosition service using actual
eligibility, platform double arithmetic/FMA/fmod, and original sound key/position
requests with caller-unit metadata. Timing-option raw masks and adjusted
success-window queries are bound. The original selected Jump demonstrably moves
Mario and advances to ResultAC80107708; HP/story result and one payment count
stay unchanged. Native TimingA manager/display cadence is NOT yet automatically
invoked, and ResultAC is still an explicit frontier. No action success, hit,
rendered animation/audio playback, turn or tutorial completion is synthesized.

Validation: final isolated snapshot /home/carson/Documents/blender_tests/pm-port/artifacts/tutorial-ac-1ptdwkli, refreshed to8418dfb2 plus scoped
code/test files, passed `make test`:234/234 C++ and142/142 Python tests. Native
`make all -o assets-ready BUILD=build-ac-review TARGET=ttyd-ac-review` passed
with prepared private assets; final rebuilt artifact is that review folder's
ttyd-ac-review.3dsx. Logs: ignored artifacts/tutorial-ac-full-test.log and
tutorial-ac-native.log. Read-only ISO verifiers passed23 whole motion/action/
help/window spans; cadence11 bodies/154 anchors/28 calls; ordinary Jump19 bodies/
268 commands/57 anchors; Mario motion5 bodies/20 constants/76 anchors. Final
review source matched scoped code/test/verifier files; diff check passed.
Initial full validation used a stale pose-directory reader after concurrent
pack-format advancement; refreshing from committed HEAD fixed all three asset
loading failures. No new payload/conversion, Azahar/Dolphin, original-runtime
measurement or complete playable-menu/battle claim. Concurrent changes preserved.

Next: compose complete native private-pad/sequence/action-manager and display
queue owners; run TimingA callbacks once per simulation tick, bind ResultAC and
StopAC help/window effects, then original hit services and post-action scheduler.
Do not double-sample pads, use render frames, or clear remaining callbacks when
main returns0. Connect original class/target Input and re-entry to player
registration, consume original poses, render original HUD/menu with physical
controls and finish second-screen scene/audience plus optional hostile-item taps
with a physical alternative. Verify in isolated Azahar. Goal remains active;
full tutorial/battle systems and full-game work remain incomplete.

## 2026-10-14 — Complete original facing and Harbor Jump continuation

Committed `59317591`. Shared CalculateFaceDirection8010e684/318 implements
all original mode gates/ordering: actor/part seeds, captured-battle side default,
movement direction, target comparison, fresh literal-X comparison and final
word inversion. It preserves argument/raw-output capture, current-global unit
lookups, retained-unit part lookups, unit-before-part byte reads, explicit
absolute versus local part positions and single arithmetic. Combined modes are
not reduced to exclusive choices. Direction-write8010e5a8/6c narrows only189.
Harbor binds both with real event/unit/part/platform services.

Tests cover mode precedence, combined17, signed seeds, exact read order,
offsets/absolute XYZ, equality/NaN, integer-to-single rounding, inversion and
low8 write preserving188. The selected original Jump now executes its facing
calculation/write and stops live at StartAC80107788. Repeated ticks preserve
HP/result and the one payment count; no animation, hit, action result or
source tutorial/story completion is invented. Automatic Game/menu activation
and the target Input/re-entry loop remain separate integration work.

Validation: isolated snapshot /home/carson/Documents/blender_tests/pm-port/artifacts/tutorial-facing-se_b69nv, based on d771b764 plus scoped files,
passed final `make test`:232/232 C++ and141/141 Python tests. Native
`make all -o assets-ready BUILD=build-facing-review TARGET=ttyd-facing-review`
passed with prepared shared private assets, then rebuilt the final read-order
change successfully. Artifact: that review folder's ttyd-facing-review.3dsx.
Exact logs: ignored artifacts/tutorial-facing-full-test.log and
artifacts/tutorial-facing-native.log. Read-only ISO verifier passed19 complete
camera/action/facing/precheck spans, including both new whole wrappers. Source
snapshot matched final changed code/test/verifier files; scoped diff check
passed. A mixed-auto declaration was corrected before green builds. No new
asset conversion/payload, Azahar/Dolphin, measured timing or visual-fidelity
claim. Concurrent field/display work was preserved in scoped commits.

Next: implement complete StartAC with actual help/audience-window setter and
original TimingA manager cadence; then pose, motion/hits and action completion.
Connect original class/target Input and common re-entry to player registration,
render original HUD/menu with physical controls, and finish second-screen
scene/audience with optional hostile-item taps plus a physical alternative.
Verify actual play/visuals in isolated Azahar. Goal stays active; full tutorial/
battle systems and full-game work remain incomplete.

## 2026-10-14 — Harbor Jump camera, takeoff and original AC preparation

Committed `ba0850b3`. Complete shared explicit-parts homing resolves all five
arguments before the signed priority gate; preserves actor/part IDs and actual
home XYZ; target-1 preserves the target fields; other targets use the original
part/hit-position helper. Native Harbor binds this request plus the complete
existing takeoff, move-frame, jump-parameter, fall-acceleration and frame-to-ms
providers with runtime-owned backend/service lifetimes. Camera Main/rendering
is separate; this is not a full camera presentation claim.

Original GetWeaponActionLv, AcSetDifficulty and SetupAC now run in the selected
Jump graph. Setup preserves source reset/registration order, raw options,
actor-token display gate and fields outside its reset scope. It does not invoke
TimingA initialization/Main or clear history/parameters. Tests verify original
TimingA callback addresses, difficulty3, Mario1/part1 and Crump2/part1 camera
state, one zero-cost payment and unchanged HP/result. The original script now
stops live at CalculateFaceDirection8010e684 across repeated ticks, after the
actual takeoff calculation; no invented hit/turn/tutorial result is produced.

Validation: isolated snapshot /home/carson/Documents/blender_tests/pm-port/artifacts/tutorial-camera-000pqybf based on d413a9e9 plus changed code/test
files passed `make test`:231/231 C++ and141/141 Python tests. Native
`make all -o assets-ready BUILD=build-camera-review TARGET=ttyd-camera-review`
passed using previously prepared shared private assets. Artifact: that review
folder's ttyd-camera-review.3dsx. Exact logs: ignored
artifacts/tutorial-camera-final-test.log and tutorial-camera-final-native.log.
Read-only ISO verifier now pins17 complete camera/action/facing/precheck spans;
Jump preparation verifier passed13 bodies/5 constants/68 anchors. Source files
in the isolated snapshot match all changed code/test/verifier files. Scoped
diff check passed. A fresh unique snapshot avoided an older review directory's
symlinks; fixture offset errors were corrected before the final green run.
No Azahar/Dolphin, new payload/conversion, measured timing or playable-menu claim.

Next: implement complete original facing/pose request services, TimingA manager
cadence, Jump motion/hits and post-action scheduler. Connect source class/target
Input and common re-entry to original player registration. Preserve physical
A/B/directions and simulation-tick timing; render the original HUD/menu, then
second-screen scene/audience and optional hostile-item taps with a physical
alternative. Validate in isolated Azahar. Full tutorial/battle and full-game
work remain incomplete; goal stays active.

## 2026-10-14 — Native Mario retained sound posts

Committed `eb9155a9`. Native main now binds actual Audio idle/fall exit services
before each Playing simulation visit, including after restore. Game retains
source initial27c/280/284=-1; ordinary item-get/stay posts cancel27c then280 or
fall284 through source slot Off before clearing handles. NDSP field voices
reserve20..23; existing cue/house/map/music channels are excluded. Pending,
inactive and empty handles do no DSP work; unavailable ownership stays explicit.

Validation: isolated review tree6e163055 passed `make test`231/231 C++ and141/141
Python and native `make all BUILD=build-goomba-acquisition-review
TARGET=ttyd-goomba-acquisition-review`. Native adapter stub tests verify actual
channel clears/order, retained handles, no generation check, pending/repeated
cases and reserved-channel refusals. Review identified and corrected the broad
channel-adoption collision. [Exact artifact/logs/scope](behavior/audio/NATIVE_MARIO_MOTION_SFX_POSTS.md).
No new assets/substitute samples, emulator capture, audible-equivalence or
physical performance claim. Idle/fall On/Main/macros/emitter/lifetime are open.

Next: register native acquisition windows/effect/icon and existing fanfare/BGM
consumers, key/system/camera and inline horizontal motion clear after Game
restore. Preserve source sound/window/effect order and borrowed close storage.
Capture actual Goomba3 acquisition with isolated Azahar and measure rendering;
finish full-storage UI and remaining event/area/audio paths. Default acquisition
still has no complete presentation provider; the broad goal remains active.

## 2026-10-14 — Original selected Mario attack in Harbor runtime

Committed `f36818c0`. Harbor's explicit confirmed-command scheduler bridge
consumes a successful original weapon decision, publishes the source event,
and registers it through complete shared state6 rules. Original Jump80355cf0
now has a real priority10 ROOT/owner1, actual U2a8, wait cancellation, status
window SystemOff and channel6 advance. Source suppression admits no invented
root. Table rebuild invalidates the native decision latch; selection sets it
and publication consumes it. The bridge is not an automatic UI/controller.

Selected target/weapon, guard wait and complete fortune query callbacks are
bound. The live64-entry no-protector declaration route preserves target/part
outputs. Shared ordinary item0/FP0/AP0 payment retains real FP access, point/
count and AddAP(0) narrowing/clamping. Other declaration/cost domains remain
explicit frontiers. Harbor event status-window requests now use source packed
arbitration; native startup EventOn/SystemOn and immediate visibility projection
are adapter policies. Source slide animation remains open. Capture preserves
live battle flags against later scene publication.

Validation: isolated source snapshot based on f1754748 plus these six code/test
files passed `make test`:230/230 C++ and141/141 Python tests. Isolated native
`make all -o assets-ready BUILD=build-player-review TARGET=ttyd-player-review`
passed with previously prepared shared private assets. Artifact:
artifacts/tutorial-player-review/ttyd-player-review.3dsx. Exact logs:
artifacts/tutorial-player-review-test.log and tutorial-player-review-native.log.
Read-only ISO verifiers passed status-window6 bodies; attack-selection11 bodies/
8 data spans/301 anchors; weapon selection/cost16 bodies/2 constants; and fortune
child15 bodies/9 data spans/240 anchors. Scoped diff check passed. Initial live
checks encountered concurrent missing audio-test wiring and a native callback
return-type mismatch; the latter was fixed and tested in the isolated snapshot.
Missing isolated private-asset links were corrected before the final green run.
No Azahar/Dolphin or new asset conversion; no playable-menu/full-battle claim.

The integration test completes the original intro, uses physical A in the
source weapon submenu, then explicitly supplies confirmed scheduler admission.
The actual Jump graph retains Crump2/body1, increments zero-cost action count
exactly once, runs data-key7/fortune child and stops live at camera homing-
unitparts8012bc70. Repeated ticks preserve HP/result/count; no invented attack
outcome. The Game still does not automatically invoke this bridge.

Next: implement the complete camera homing-unitparts request (read-only listing
artifacts/tutorial-player-camera-next.txt), then original Mario pose, AC manager/
motion/hit providers. Bind class/target menu Input and common pending/progress
re-entry to selected registration, and render original HUD/menu with physical
controls. Finish second-screen scene/audience and optional hostile-item taps
with a physical equivalent. Verify in isolated Azahar. Goal remains active;
tutorial and full-game work remain incomplete.

## 2026-10-14 — Deferred Mario item-get motion/control

Committed `7929601c`. Game can now request original motion15/stay through
finite motion-table callbacks and initialize M_I_U on the following admitted
Mario owner, before later item ControlOff. Entry resets44/48/50 and horizontal
speed180 without clearing unrelated vertical/air state; original display-sector
heading drives the front-bank static pose. Control uses retained nested byte
and main2; stay is deferred. Nonnull idle/fall post providers are required at
candidate and pending admission; native cancellation is not registered yet.

Validation: review treeb6f6a2ea passed `make test`229/229 C++ and141/141 Python
and native `make all BUILD=build-goomba-acquisition-review
TARGET=ttyd-goomba-acquisition-review`. Tests cover deferred Game acquisition
pose/control order, missing exits, preserved motion fields, pose locks and
special/unsupported guards. Shared CMake configure initially hit an unfinished
concurrent battle test reference; review isolated the owned source changes.
[Exact validation/artifact](behavior/field/NATIVE_ITEM95_MARIO_MOTION_PROVIDER.md).
No native gameplay capture or physical performance claim; no new game payload.

Next: implement/register actual native idle/fall voice cancellation and inline
horizontal motion clear (174/17c/180/18c, not additiveY178); key/system/camera,
original windows/effect/icon and existing audio providers after Game restore.
Preserve sound/window/effect visits, borrowed close storage and once-only5405.
Capture native acquisition in isolated Azahar, measure rendering, then finish
full-storage UI and additional event/area paths. Default native acquisition
still lacks presentation providers. The broad remake goal remains active.

## 2026-10-14 — Original target confirmation and command finalization

Committed `92e366e0`. Shared Input state11 preserves the pre-refresh unit/part
pair, signed permutation/count handling, first-match remap, independent forward
and reverse Repeat gates, B-before-A cancellation, post-pose type reread,
retained type for literal multi-item cost pointers, and the audience query after
confirmation. Shared state29 uses the retained actor for attack selection before
fresh flag clears and original result1. Its integration test calls the actual
shared selector and publishes original Jump event80355cf0/category0/usage count
without fabricating an attack root, damage or tutorial completion.

Validation: `make test` passed231/231 C++ and141/141 Python tests; focused final
target-selection rerun passed1/1. `make all BUILD=build-tutorial-menu-layout
TARGET=ttyd-2ds-menu-layout` passed full asset preparation and native compilation.
Both used the project-local ignored TMPDIR. Read-only ISO command-input verifier
passed39 bodies/3 data spans/526 instruction anchors. Logs: ignored
artifacts/target-full-test.log, target-focused-test.log, target-native-build.log,
and target-source-verifier.log. Scoped diff check passed. No Azahar or Dolphin
run; no visual/playable-menu or full-battle fidelity claim. No new asset payload
or conversion; original/private packs remain outside history.

Boundary: handlers require complete invoked services. An unbound provider is an
explicit frontier after preceding effects, not a retry-safe continuation. The
native tutorial does not yet call these handlers, and storing state29 alone
does not set outer progress1 or automatically start an event. Game still needs
original class/target menu presentation, command-pose/UI/help ownership,
common pending/progress re-entry and selected-player attack registration.

Next: bind those complete native providers, connect actual selected Jump roots
and action-command services, then render the original extracted HUD/menu with
physical controls. Finish second-screen scene/audience presentation and optional
hostile-item taps with a physical-button equivalent; verify in isolated Azahar.
Tutorial/full-game work remains incomplete and the goal stays active.

## 2026-10-14 — Game Goomba3 acquisition ownership

Committed `4bd12c2f`. Game now retains the original quest2 acquisition and
registry, admits it through the actual NPC candidate/talk path when complete
presentation callbacks are registered, and schedules event before Mario/NPC
then item. Original printing ends before grant; dialogue/key hold lasts until
cleanup and the subsequent5405 poll. Copies keep presentation state but cannot
advance pointers into the live owner. Restore clears registration.

Validation: isolated tree d12eec21 passed `make test`225/225 C++ and141/141
Python tests and native `make all BUILD=build-goomba-acquisition-review
TARGET=ttyd-goomba-acquisition-review`. Game tests cover real candidate/talk
admission, missing callbacks, original message, one grant, delayed5405 and
snapshot regressions. Independent review confirmed the ownership guard.
[Exact artifact, logs and limitations](behavior/field/GOOMBA3_GAME_ACQUISITION_BINDING.md).

Next: register native main/renderer/audio providers after each Game restore;
bind motion15 pending entry and [source control-off gate](behavior/field/ITEM95_MARIO_MOTION15_ENTRY_DISPLAY_AND_STAY.md),
control/system/camera state, camera5 icon, original M_I_U/effect/windows/audio.
Preserve sound/window/effect order and closing-window borrowed storage. Capture
in an isolated Azahar profile, measure rendering and finish full-storage UI.
Default native acquisition still lacks providers; this is not a playable
acquisition/render validation or full-game completion. Goal stays active.

## 2026-10-14 — Original weapon submenu controls and physical tutorial bridge

Committed `012b38b2`: complete shared WeaponSelect Main/End and source key gate,
plus complete audience-interruption writer with explicit provider readiness.
Confirmation/pending/reason order, interior Repeat vs boundary Trigger,
post-request cursor rereads, forward priority, special Item cancel states,
UI-before-flags End and result0 audience ownership follow source contracts.
Native general pad is provisioned/sampled separately from private defense;
tutorial submenu API confirms with physical A and cancels with B only after
its explicit menu-presented signal. Actual Crump targets come from the source
decision; no HP/result/attack root is fabricated. Cursor/target/reason and raw
sound IDs/keys are exposed for original presentation consumers.

Native open/End/target setup are immediate UI projections, not complete original
UI constructors/animation. Game still lacks automatic original class/target
controller activation; the submenu is not yet visible/playable in the complete
tutorial flow. Host acknowledgements are controlled fixtures, not GPU evidence.
Audience trigger0 uses its source early return; an admitted X interruption
retains the unbound item producer instead of synthesizing item0/-1 or success.
Exact light/UI Main blr leaves remain no-ops; original sounds are queued without
generated tones. Cost/updater bindings were factored for reuse without rebuilding
the table in Main.

Validation: shared make test passed229/229 C++ and141/141 Python tests. Final
focused Main/tutorial rerun passed2/2 after a fixture global-pointer correction
and cursor-reread check. Native make all BUILD=build-tutorial-menu-layout
TARGET=ttyd-2ds-menu-layout passed; final rebuild after cleanup passed with
-o assets-ready (full preparation had completed). Existing source verifiers
passed menu20 bodies/344 anchors and interruption20 bodies/243 anchors. No
Azahar/Dolphin run; no native menu rendering or full battle fidelity claim.
No new game payload/conversion; scoped commits preserve concurrent work.

Next: original class/target controllers and UI activation, selected target/type/
event ownership, Mario action timing/impact and complete turns; original camera,
window/effect/audio and crowd item/throw providers. Preserve original menu/HUD
art and physical routes; audience taps on the lower screen remain optional.
The full tutorial/battle goal remains active.

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

## 2026-10-14 — Real player battle pose ownership

Committed `3a8194ec`: real mode1 handles and CPU workspaces for all three Mario
parts and Goombella, complete source descriptor selection/restart/rate history,
separate model/texture references and immutable bank sharing with the renderer.
Native forced display now consumes source names/mode gates and ordinary
counter/zero-effect services before best-effort CPU output. The complete source
Main8003ed2c boundary remains pending. Hidden defaults retain real buffer
ownership; clock branch names follow GP40/44 versus GP38/3c.
[Implementation scope](behavior/display/NATIVE_BATTLE_POSE_RESOURCE_OWNER.md).

Combined make test passed229/229 C++ and141/141 Python checks; native make all
passed. The final isolated source snapshot, including hidden-default refinement,
passed226/226 C++ and141/141 Python tests and native compilation. Azahar on the
first committed owner build reached the original10 Star Point reward/victory
and existing field-restoration hold with no detected exception. The final
hidden-default/clock-name refinement was host/native verified, not rerun in
Azahar. Exact artifacts, earlier failures and limits are in the
[archive](PROGRESS_ARCHIVE.md#2026-10-14--real-player-battle-pose-ownership).

Next: full animation Main/facing/GX publication, automatic/status-pattern part
consumers and enemy/Frankly factories, remaining queued display and real ordered
teardown/field/result/save ownership. Original global field/battle allocation
history, full cadence and physical hardware parity remain unproven; the full
playable original-fidelity prologue goal stays active.


## 2026-10-14 — Source battle animation time publication

Native battle poses now publish original elapsed/rate, ratio, selected time and
continuous sample fields before using the converted CPU sampler. Source
zero-span, early-time, nonloop end and fused-loop branches are retained;
selected time is sampled without a second wrap. Conversion edge domains stay
explicitly unavailable. The documentation inbox also records the recovered
[ordinary Mario post-Main draw queue](behavior/battle/ORDINARY_MARIO_POST_POSE_MAIN_ALPHA_AND_DRAW_QUEUE.md).

Isolated make test passed232/232 C++ and141/141 Python checks; isolated and
shared native make all passed. Shared host run passed234/235, with a concurrent
battle_facing test failure outside this change. Exact logs/source/binary hashes
and limits are in the [archive](PROGRESS_ARCHIVE.md#2026-10-14--source-battle-animation-time-publication).
No Azahar or hardware revalidation for this step.

Next: retain independent source discrete timeline/cache state, complete Main
publication and bind queued draw depth to original camera4. Enemy/Frankly
factories and ordered battle teardown/field return remain open; the playable
original-fidelity prologue goal remains active.


## 2026-10-14 — Original battle discrete state and model translucency

Private PMPOSE02 sidecars now retain original modelwordc4 and each descriptor's
independent discrete timeline. Native poses publish34/38 and actual value80
from that timeline, with original nonloop clamping and explicit predecessor-1
unavailability. Typed TestXLU reads model bit8; it does not infer translucency
from animation or part colors. [Native format](behavior/format/NATIVE_BATTLE_POSE_DIRECTORY.md).

Shared make test passed236/236 C++ and142/142 Python checks; isolated source
passed233/233 C++ and142/142 Python checks. Shared and isolated native make all
passed. Exact manifests/logs and limits are in the
[archive](PROGRESS_ARCHIVE.md#2026-10-14--original-battle-discrete-state-and-model-translucency).
No Azahar/hardware revalidation or complete Main claim for this step.

Next: real compressed cache3c/40/output ownership, full Main and camera4 depth
queue, then ordered battle teardown/result/field return. The full playable
original-fidelity prologue goal remains active.


## 2026-10-14 — Retained pose cache and ordinary Main completion

Native battle poses now retain cumulative five-channel state separately from
fractional output, implement original rebuild/forward/same-key rules and cubic
ordering, preserve loop-seam output and publish actual3c/40 cache history.
Ordinary Main completes for real metadata, unlinked/private-output poses with
facing bit8clear. The trio continues through source alpha and TestXLU, then
retains camera4/class/part/depth at actual dispCalcZ8001086c.
[Scope and limits](behavior/display/NATIVE_ANIMATION_CACHE_AND_ORDINARY_MAIN.md).

Shared make test passed237/237 C++ and142/142 Python checks; isolated snapshot
passed235/235 C++ and142/142 Python checks. Both native builds passed. Azahar
on the immutable isolated build reached original10 Star Points/victory and
the existing field-restoration hold without a detected exception. Exact logs,
source/binary manifests and limits are in the
[archive](PROGRESS_ARCHIVE.md#2026-10-14--retained-pose-cache-and-ordinary-main-completion).

Next: original battle-camera framing and live camera4 matrices, homogeneous
depth/queue callbacks, remaining pose factories/parts and real ordered battle
teardown/result/field return. General linked/paper/facing, exceptional numeric
domains, original libm bit parity and hardware performance remain unverified.
The full original-fidelity playable prologue goal stays active.


## 2026-10-14 — Original battle camera request and frame ownership

Native camera callbacks now apply original packed writes, including homing,
zoom, offset, MoveTo and Y freezes. Victory requests execute in source order.
A retained camera owner implements Init/default14..16, mode3 timeline and
victory17/18 using actual unit home positions, source interpolation order and
axis/delta history. Camera updates occur once per native time before events;
unsupported prior mode/shake history remains recorded. Explicit-matrix SDK
view/project/depth helpers now preserve homogeneous divide and source clamps.
[Scope](behavior/camera/NATIVE_BATTLE_CAMERA_FRAME_OWNER.md).

Final isolated make test passed238/238 C++ and142/142 Python checks and native
make all passed. Earlier shared builds passed240/240 C++ and142/142 Python
checks plus native compilation before the final event-wrapper additions.
Exact logs/manifests and limits are in the
[archive](PROGRESS_ARCHIVE.md#2026-10-14--original-battle-camera-request-and-frame-ownership).
No Azahar/hardware run for this step.

Next: original camera4 entry/lens/matrix lifecycle, remaining controller modes
and shakes, real display queue/part callbacks, then ordered battle teardown
and field return. The trio still holds at dispCalcZ8001086c; fixed renderer
framing has not been relabeled as original camera4. Full prologue parity
remains unfinished and the goal stays active.


## 2026-10-14 — Native display queue and original action camera framing

Added a host-testable native display queue with original record layout,
class/depth keys, source sort branches and deferred dispatch continuation.
Implemented camera modes0/7/10/11 and remaining MoveTo speed branches;
corrected the earlier speed1 divisor from5 to the ISO-confirmed15. Unit kind
dimensions and part radius/height, scale, rotation and pivot defaults now
come from original initializer stores.
[Scope](behavior/display/NATIVE_DISPLAY_QUEUE_OWNER.md).

Final isolated make test passed242/242 C++ and142/142 Python tests; native
make all passed. Exact logs, snapshot and binary manifests are recorded in
the [archive](PROGRESS_ARCHIVE.md#2026-10-14--native-display-queue-and-original-action-camera-framing).
No Azahar or hardware run for this step.

Next: retain actual field-to-battle camera4 lens/offset/matrix history, bind
the queue to original draw callbacks and native GPU services, and implement
ordered teardown/field return. The trio still holds at dispCalcZ8001086c;
the queue is not yet connected to rendered battle parts. Remaining controller
modes/shakes and full prologue parity stay open; the goal remains active.


## 2026-10-14 — Trio pose depth and borrowed display registration

Bound the finishing display prefix to the native queue using explicit camera
snapshots. Game supplies a declared field-lens numerical adapter in engine
units; original camera4 roll/offset/extra history remains open. A retained
depth continuation resumes missing-provider waits without repeating pose Main
or duplicating Entry. The ordinary immediate continuation now computes the
original stage floor and shadow position/radius, holding at shadowEntry8002be98.
[Scope and limits](behavior/display/NATIVE_TRIO_POSE_QUEUE_BINDING.md).

Shared make test passed247/247 C++ and142/142 Python checks. Final isolated
make test passed244/244 C++ and142/142 Python checks, and native make all
passed. Exact snapshot/logs/manifests are in the
[archive](PROGRESS_ARCHIVE.md#2026-10-14--trio-pose-depth-and-borrowed-display-registration).
No Azahar or hardware run for this step.

Next: shared original shadow pool and preparation/draw/reset cadence, remaining
part/unit display tails, and actual deferred part matrices/material/GPU draw.
Keep full inherited camera4 publication and ordered battle teardown/field return
open. Existing handheld battle view is unchanged; queue registration alone is
not rendered parity. Full original-fidelity prologue goal remains active.


## 2026-10-14 — Shared shadow requests and live player display traversal

Added the original-layout64-record native shadow request owner, retained settings
and Main registration/reset order. Game shares it with the trio and preserves
it across restores. Finishing display now follows live part links with retained
unit/part continuation, automatic status/name/rate processing and source unit-
tail gates. Mario's extra parts run descriptor0 Main, then original attributes
suppress their drawing/shadows; Mario and Goombella submit two pose and two
shadow records. [Scope](behavior/display/NATIVE_SHADOW_WORK_AND_TRIO_LIVE_DISPLAY.md).

Final isolated make test passed247/247 C++ and142/142 Python checks; native
make all passed. Exact snapshot, logs, failed intermediate checks and immutable
manifests are in the [archive](PROGRESS_ARCHIVE.md#2026-10-14--shared-shadow-requests-and-live-player-display-traversal).
No Azahar/hardware run for this step.

Next: replace the surviving Frankly synthetic part with the
[original constructor](behavior/battle/SETUP4_SURVIVING_FRANKLY_BATTLE_POSE_CONSTRUCTOR.md)
and produce its genuine battle metadata sidecar. Current finishing pass holds
at that pose-constructor provider801011fc. Shadow collision/image/cylinder/GPU
services, full outer-loop and deferred display cadence, footer providers, complete
camera4 history and ordered field return remain open. Full prologue goal stays
active. No rendered parity is claimed by request registration alone.


## 2026-10-14 — Original Frankly battle bank and first display footers

Provisioned surviving Frankly from the actual kind/part/pose table and a
distinct mode1 owner. The Professor converter now imports all17 original
animations, including D_2, and emits genuine PMPOSE02 metadata. The finishing
loop registers his part/shadow, then implements original StockExp counter/EXP
registration and both stage display records. Original display-init bit2 is
retained. [Scope](behavior/display/NATIVE_FRANKLY_BANK_AND_FIRST_DISPLAY_FOOTERS.md).

Shared make test passed250/250 C++ and143/143 Python checks. Isolated make
test passed249/249 C++ and143/143 Python checks; native make all passed.
Isolated Azahar on the immutable build reached original10-Star-Point victory
and the existing field-restoration hold without a detected native exception.
Exact logs/manifests/capture limits are in the
[archive](PROGRESS_ARCHIVE.md#2026-10-14--original-frankly-battle-bank-and-first-display-footers).
Asset paths/conversion/licensing are in [assets](ASSETS.md#2026-10-14--complete-frankly-animation-bank-and-battle-metadata).

Next: CommandDisplay's retained palette mutation80119e24, audience animation
interpreter/window gating and slot footer registration; then full borrowed
display dispatch/current-camera/GPU and ordered teardown/field return. Original
actor/update chronology, shadow preparation, full camera4 history and physical
performance remain unverified. The current finishing queue retains14 records
and three unprepared shadows, then holds at the palette provider. Full playable
original-fidelity prologue goal remains active.


## 2026-10-14 — Display footer mutations and Main result publication

Implemented the retained boot-lifetime command palette, all command registration
gates, full200-record audience scalar animation interpreter, independent
audience/window queues and unconditional slot registration. Missing script
reads retain prior stores. The finishing display now clears its original flags
and completes Main's late result/publication without replaying earlier services.
Its actual20 records remain queued; bank4 publishes end bit80000000 and waits
at next outer camMain8000f704.
[Scope](behavior/display/NATIVE_DISPLAY_FOOTER_COMPLETION.md).

Final isolated make test passed251/251 C++ and144/144 Python checks; native
make all passed. Shared C++ checks passed252/252; its Python pass exposed stale
private item-get metadata, refreshed with the existing read-only ISO converter
before final isolated validation. Exact logs/snapshot/manifests are in the
[archive](PROGRESS_ARCHIVE.md#2026-10-14--display-footer-mutations-and-main-result-publication).
No Azahar/hardware run for this step.

Next: global camera publication and real sorted/ascending-camera deferred draw
with live palette, part/material/GPU and shadow providers. Then bind the separate
sequence-manager exit, ordered BattleEnd/resource teardown and field waiter.
End-bit publication alone does not restore the field. Existing rendering/cadence
adapters and original actor/history/performance limits remain open. Full playable
original-fidelity prologue goal remains active.

## 2026-10-14 — Matrix publication and visible trio camera

Added source-ordered roll/translation and one-use extra projection publication,
including exact perspective float-store ordering with declared native numeric
adapters. The handheld trio view now follows its retained controller eye/target/up;
its world actor-facing reads use that same view. Restored the footer host source
registration missing from latest committed CMake. Recovered all13 original camera
boot/update/draw records without claiming their native implementation complete.
[Scope](behavior/camera/NATIVE_CAMERA_MATRIX_PUBLICATION_AND_TRIO_VIEW.md).

Final isolated make test passed252/252 C++ and144/144 Python checks. Native make
all DEBUG_SHORT_BOOT=1 passed. Isolated Azahar completed actual trio attacks,
original10-Star-Point victory and the existing field-restoration hold without a
detected exception; reviewed captures show wide command/close action/victory
framing. Exact results and immutable build/source manifests are recorded in the [archive](PROGRESS_ARCHIVE.md#2026-10-14--matrix-publication-and-visible-trio-camera).

Next: retain the full13-camera bank and resumable callback/zoom publication;
bind sorted ascending-camera draw/current index, actual queued part/material/shadow
and dummy GPU providers, then ordered sequence-manager exit/BattleEnd/field return.
The finishing camMain8000f704 hold remains. Gekko math, inherited field roll/extra
history, complete draw cadence and hardware performance remain unverified. Full
playable original-fidelity prologue goal stays active.

## 2026-10-14 — Retained camera bank and original tremor consumption

Added the13-camera retained bank, resumable Main and source-ordered portable
Draw owner. Game preserves its shared bank through restore/battle entry. Both
original tremor axes now interpolate, shift the live camera, restore speeds
and consume timers without republishing baseline or replaying earlier work.
Finishing native Main completes all13 camera updates and advances to actual
camDraw8000f454, preserving its20 borrowed records. Explicit missing-lens
callbacks still hold and resume without recollection.
[Scope](behavior/camera/NATIVE_CAMERA_BANK_AND_DEFERRED_DRAW_OWNER.md).

Final isolated make test passed255/255 C++ and144/144 Python checks; native
make all DEBUG_SHORT_BOOT=1 passed. Isolated Azahar completed the actual trio
attack/victory/fade sequence without a detected native exception and reached
the existing field-restoration hold. Exact logs/snapshot/manifests, initial
failed tremor integration check and reviewed captures are in the
[archive](PROGRESS_ARCHIVE.md#2026-10-14--retained-camera-bank-and-original-tremor-consumption).

Next: bind recovered BeroEXEC/letterbox global/scalar owners into the shared
camera8, then actual ascending-camera viewport/projection/borrowed GPU callbacks,
shadow preparation and genuine dummy draw. Native Draw is not certified by
portable tests; camDraw remains the finishing wait. Ordered sequence-manager
exit/BattleEnd/field return, full field-camera history, source Gekko numerics and
physical performance remain open. Full playable original-fidelity prologue goal
stays active.

## 2026-10-14 — Crump opening facing, original animations and airborne Mario

Commits `945cbe10` and `948c0672` replace fixed Crump/X-Naut Z_1 rasters with
original idle/talk/walk/run geometry and texture clips. Source tribe profiles
provide Goombella/Crump/X-Naut run thresholds20/50/50. Independent simulation
pose clocks and camera-relative NPC presentation now drive all four actors.
Original movement retains event yaw: Goombella retreats facing Crump, while
beforebattle's explicit270/90 turns remain in order. The old adapter incorrectly
turned her toward travel. Mario's scripted movement selects walk/run, keeps
ordinary midair physics and jump/fall poses under key-off, preserves explicit
source poses, and retains M_I_N during the authored displayflag2 surprised hop.
Original dialogue/control tokens, speakers, branches and story order are unchanged.
[Behavior scope](behavior/story/NATIVE_HARBOR_ACTOR_MOTION_AND_FACING.md) and
[asset provenance](ASSETS.md#2026-10-14--animated-crump-opening-field-actors).

Final frozen source `artifacts/crump-validation` passes **256/256 C++ and
145/145 Python** through `make test` (`crump-host-accepted.log`). Tests include
retained Goombella/enemy yaw, source run/walk threshold boundaries, pose clock
continuity, actual original moving geometry, midjump admission/real dry-floor
landing, authored hop pose lock and unchanged three-message battle admission.
Native `make all DEBUG_SHORT_BOOT=1` passes (`crump-native-accepted.log`), as does
normal `make all BUILD=build-normal TARGET=ttyd-2ds-normal`
(`crump-native-normal.log`). Native builds reuse verified original packs using
`-o assets-ready`; source converters ran read-only in this session. Only main.cpp
uses DEBUG_SHORT_BOOT; normal main was explicitly rebuilt without that define.
Verified normal binary is also copied to ignored `ttyd-crump-event.3dsx`.
Exact source/binary hashes are in `artifacts/crump-validation-manifest.json`.
Behavior-contract verification and scoped whitespace/Python syntax checks pass.

Isolated final Azahar capture `artifacts/azahar-crump-opening-accepted`, using
story3-entry and `tests/azahar_crump_opening_motion.py`, reaches source beforebattle
and tutorial entry without detected native exception. Reviewed captures show
Goombella facing Crump through retreat, enemy animated geometry and beforebattle
Mario/Goombella facing. Initial cold captures are black/0FPS; field samples later
reach60FPS, with35FPS at one scene sample and15FPS at tutorial entry. This does
not establish hardware performance or measured original execution parity.

Earlier shared `make all` hit a stale pre-migration shader dependency; fresh
native build passed. Shared host tests hit concurrent tutorial provider edits.
The first isolated /tmp build exhausted the shared tmpfs; final scratch/builds
use project artifacts. Two airborne test attempts assumed floor support at an
invalid harbor water fixture; the final test uses validated dry harbor ground
and verifies actual landing support. The older Azahar story script demanded a
story4 save despite active-event save suppression; its failing assertion was
replaced by an explicit capture script and host progression checks. Those initial
logs remain under `artifacts/crump-*` and are not counted as passing checks.
No ISO, user emulator profile or user saves were modified; original asset packs
remain excluded from source history.

Next: source NPC move-position easing/collision, complete force-move tick and
Mario direction-set timing, original crowd/effect/SFX services, and full tutorial/
postbattle progression. Existing story3 timing and movement bridges remain
adapters; no complete original-fidelity opening or full remake claim is made.

## 2026-10-14 — Original letterbox scalar continuation

Implemented original letterbox decisions, fresh sequence/Key/Ctrl/party reads,
sound-provider order, signed bar/alpha animation and always-registered borrowed
callback. Holds retain flag/scalar work and captured pointers. Native finishing
now enters the actual Draw owner, binds slot0, then waits at missing BeroEXEC
8007ac40; no scene-derived global values are fabricated. Explicit integration
providers resume through registration/sort to real GX viewport802ba56c while
preserving prefix work. [Scope](behavior/camera/NATIVE_LETTERBOX_SCALAR_CONTINUATION.md).

Final isolated make test passed257/257 C++ and145/145 Python checks; native
make all DEBUG_SHORT_BOOT=1 passed. Validation includes the harbor owner's two
fixture-coordinate corrections, independently proved necessary on unchanged
base; those changes are outside this camera commit. Exact snapshot/logs, initial
failed checks and provenance are in the
[archive](PROGRESS_ARCHIVE.md#2026-10-14--original-letterbox-scalar-continuation).
No Azahar/hardware run for this step.

Next: bind original BeroEXEC reset/mask writers, actual sequence-current lifecycle
and other retained letterbox globals, then real native viewport/projection,
queued part/material/shadow/letterbox and dummy GPU work. Native fieldSequence2
is not original current4 for trio. Sound SetFlag accumulated1/2 gates are now
correctly documented. Ordered outer Main/sequence exit/BattleEnd/field return,
original numeric/history parity and hardware performance remain open. Full
playable original-fidelity prologue goal stays active.

## 2026-10-14 — Retained Trouble Center menu/icons

Implemented finite stable-owner TroubleQuestPresentation and native renderer
binding: source local animation/hidden aging/deletion/reopen, exact five named
icons and original paging art, callback source cue IDs and optional state reads.
Corrected original Up Y-mirror geometry as well as UV reversal. Pixel-identical
atlas storage retains original descriptor frames/timing and512x5121MiB budget.
[Scope](behavior/display/NATIVE_TROUBLE_CENTER_RETAINED_PRESENTATION.md).

Validation: final isolated make test261/261 C++ and145/145 Python; restored
normal make all passes. Azahar original paged-menu preview passes both arrow
anchor checks and byte-identical copied checkpoint. Asset check verifies77
original frame entries in73 exact atlas cells. Tree/source audit and exact binary
hashes/logs are recorded in the linked contract. Initial nonoverlap test rejected
intentional aliases; regression now verifies every frame and rejects partial
cell overlap. First10-second preview was still loading;35-second boot passed.
These failures remain archived. Preview providers/visits are explicit fixtures;
no live board/acceptance/audio or hardware parity is claimed.

Next: native original gor_02 case9/door/lecture/global bank/save and original
help/confirmation/acceptance EVT, input/local-clock admission and matching sample
playback. Hardware/raster/event parity remains open. This step provides a callable
backend, not native board gameplay or a completed remake. Goal remains active.


## 2026-10-14 — Port code documentation: runtime, event VM and battle core

Commits `517fa879`, `b4635a79` and `ecde3ebb` add the
[port code guide](code/README.md), runtime ownership/timing, current native
v1..9 checkpoint byte/knowledge-bit tables, event VM implementation walkthrough
and core battle action/status/resource/damage guide. Source/header maps and the
project README link to this collection. Inline comments in **48 C++ files**
explain original masks, opcode/state/result numbers, packed offsets, signed
narrowing/wrap, callback counts, provider ownership and native policy values.
This pass uses the requested nearby-comment option for recovered binary values;
it does not rewrite numeric ABI constants or assign invented names to unknown
fields. In particular, it documents retained PAD samples after transfer errors,
first-poll wait decrement, WAIT_MSEC low-word signed/unsigned division, independent
handler/manager result protocols, flat/nested scan asymmetries, unresolved pointer
aliases, distinct map/SDK allocator headers, optional save-state knowledge,
private defense/general action input and requested-versus-consumed damage.

Final frozen baseline `99943ec006d7fbaf4dd7a0af6e03e90d1c390861` plus exactly
this documentation pass passes **260/260 C++ and 145/145 Python** through
`CMAKE_BUILD_PARALLEL_LEVEL=4 make test`, and normal native
`make all -o assets-ready -j4` passes. Exact logs are
`artifacts/code-documentation-validation/host-final.log` and `native-final.log`;
`manifest.json` records baseline, source hashes, unchanged executable-token
checks for all48 touched C++ files, and ELF/3DSX hashes. Live files match the
frozen source hashes. Code-guide local links and scoped `git diff --check` pass.
The isolated native build reuses existing private packs via `romfs`/`local-assets`
symlinks and skips regeneration; it does not independently freeze/verify every
private asset. No new emulator or hardware validation was needed/performed for
these comment-only executable changes. ISO and user emulator profiles/saves
were not modified. Existing compiler warnings remain outside this pass.

Initial shared `make test` failed because concurrent CMake edits referenced a
not-yet-present `tests/battle_class_raster_tests.cpp`; initial shared `make all`
failed on a stale pre-migration shader dependency. Those attempts are recorded
in `artifacts/code-documentation-test.log` and `code-documentation-native.log`,
and are not counted as passing checks. A fresh isolated source/build avoids
those unrelated mutations; other working-tree changes were excluded from the
documentation commits and validation snapshot. The first isolated runtime/event
pass also passed260/260 C++ and145/145 Python plus native build (`host.log`,
`native.log`), before adding battle-core comments and final validation.

Next documentation tasks, in order: audit the large `source/runtime/game.cpp`
and remaining runtime headers (especially units, collision pack flags and tick
ownership), renderer/animation/camera helpers, field/NPC/partner mechanics,
remaining battle packed/event/menu/display helpers, then message/inventory/story
modules and shaders. Review existing comments rather than blanket-inserting
generic summaries; retain exact dialogue/assets and original/provisional scope.
Continue incremental commits and required validation. The user's **port-code
documentation goal remains active**: this is a substantial first pass, not
complete coverage of the port, and not a completed remake claim.

## 2026-10-14 — Retained BeroEXEC and Key39 binding

Added original BeroEXEC startup/resolved-mask ownership with explicit bit
provenance. Generic-road admission, independent inline/parent requests and
arrival publish documented masks through scoped adapters; restore/cancel/unbound
interior history stays unavailable. Trio letterbox uses the exact known bit1
projection; explicit clear resumes it to the separate current-sequence wait.
Corrected idle Key input from Ctrl38 to signed Key39.
[Scope](behavior/overworld/NATIVE_BERO_EXEC_OWNER_AND_ROAD_ADAPTER.md).

Final isolated make test passed261/261 C++ and145/145 Python checks; native
make all DEBUG_SHORT_BOOT=1 passed. Isolated Azahar completed two reciprocal
plaza/east road round trips with all four saved-room assertions and no detected
native exception. Exact snapshot/logs/manifests and capture limits are in the
[archive](PROGRESS_ARCHIVE.md#2026-10-14--retained-beroexec-and-key39-binding).

Next: actual get_info room table/HitObj reset and remaining custom/door/pipe
writers, full original sequence current/next/exit/init lifecycle, then deferred
GPU/part/material/shadow/letterbox/dummy and ordered field return. Known bit1
does not certify the whole global word or original event timing. Native current
sequence remains unbound; full playable original-fidelity prologue goal stays
active. Original numeric/history and physical performance verification remain.


## 2026-10-14 — Game code documentation and portable runtime helpers

Commit `40649ee3` documents the large Game composition and seven companion
source/header files (eight C++ files total). The new
[Game implementation guide](code/game-composition.md) explains native/engine
units and identity namespaces, wall/ground/head/return/road pack layouts,
spatial-index bounds, filtering/quantization/probe selection, camera/input/NPC
publication order, scope-exit simulation work, restore lifetimes, original versus
harness story/battle gates, and native follower policies. Inline comments give
meaning to nearby masks/state IDs/size limits/timing/geometry constants and retain
unidentified source IDs without invented semantic names. Portable follower math,
head response, fade presentation gating, frontend requests and opening-departure
handoff now have corresponding numeric/ownership explanations. Executable tokens
are unchanged in all eight C++ files relative to that commit's parent. This
extends the first pass to56 distinct C++ files; coverage of the whole port remains
unfinished. No behavior, story text or action timing was changed by this pass.

Accepted frozen baseline `38f9f3de7825fab73c4049a5db9f6e5dd661ea4a` plus exactly
the comment additions passes **263/263 C++ and155/155 Python** via
`CMAKE_BUILD_PARALLEL_LEVEL=4 make test`; normal native
`make all -o assets-ready -j4` passes. Logs are
`artifacts/game-code-documentation-validation/host-complete.log` and
`native-accepted.log`. `manifest.json` records frozen source hashes, baseline,
comments-only commit token checks, private RomFS file hashes, refreshed icon
hashes and ELF/3DSX hashes. Code-guide relative links and scoped whitespace checks
pass. Native validation uses existing converted packs with asset regeneration
skipped except the isolated icon refresh described below; other local-asset
folders remain shared read dependencies. No emulator/hardware run was needed
for these executable-token-preserving edits. No user profile/save or ISO write
occurred. Existing compiler warnings are not resolved by documentation.

Initial frozen host runs (`host.log`, `host-final.log`) exposed a stale shared
icon pack missing committed Trouble Center resources524/134/136. The existing
frozen converter regenerated original icon pixels/delays into a private
validation directory from the read-only ISO. Shared packs were not overwritten.
A private RomFS copy then passed all263 C++ checks; Python tests in
`host-accepted.log` exposed the mismatched old shared atlas cache. Copying the
matching regenerated icon extraction/atlas cache into the validation snapshot
resolved both three-test HUD/selector groups, followed by the complete accepted
suite above. The failed runs remain archived and are not counted as passing.
[Asset paths/conversion/licensing](ASSETS.md#2026-10-14--private-icon-refresh-for-code-documentation-validation)
are recorded in commit `7a6868fc`. Native intermediate `native.log` and
`native-final.log` passed before that isolated asset refresh.

Game/header road-owner changes already in the worktree were preserved. A separate
comments-only index patch was generated against committed source to avoid
including those concurrent implementation edits; frozen validation likewise
excluded them. Subsequent unrelated source/asset work remains outside this pass.

Next: audit `include/runtime/platform.hpp` with renderer resources and
`source/display/` plus paired headers, including packed batch bits, GPU cache/
resource lifetime, animation clocks, queue/depth and camera continuation numerics.
Then audit field/NPC/partner subsystem modules, remaining battle packed/event/
menu/display helpers, message/inventory/story code and shaders. Recheck later
runtime additions during the completion audit. Keep original/provisional distinctions,
meaningful local comments and incremental commits; do not substitute a file-count
or comment-density metric for actual semantic review. The user's full **port-code
documentation goal remains active**.

## 2026-10-14 — Tracked editable originals and custom asset pipeline

The user explicitly replaced the previous extracted-asset history rule: editable
original and custom sources are now included in Git under `assets/`. `AGENTS.md`
and the active asset/build guides reflect that decision; the ISO, temporary
extractions, compiled RomFS packs and binaries remain excluded.
[Workflow](../assets/README.md) and [provenance](ASSETS.md#2026-10-14--tracked-editable-assets-and-native-source-pipeline).

Included 420 sources: 323 PNG textures/atlases, 6 room/stage model JSONs, 21
actor pose banks, 42 animation JSON banks and 28 original PCM16 audio clips.
The source manifest compiles textures, padded atlases, static native JSON/
triangulated OBJ models, named actor poses, PMANIM3/4 sparse animation streams
and music/SFX with sample-frame loop intervals. Original converter layouts/
geometry/scaling/rates/channels are retained. Upstream conversion manifests and
initial source hashes are tracked, with original rights retained per asset.
Custom assets can be added with their own author/license and stable output IDs.

Normal `make all` restores the previous base pack, runs remaining original
converters and compiles the active manifest. Content/dependency/tool hashes
cache compilation; all sources compile in staging before installation. Removed
entries restore base originals or delete additions, including stale audio loops.
Conflicting external output changes or damaged backups stop restoration without
overwriting them. An output-root content stamp triggers native RomFS packaging
for nested asset edits. Explicit import refuses nonempty destinations and never
runs during normal builds.

Final isolated `make test` passed **263/263 CTest checks and 159/159 Python
tests**, including 13 asset-pipeline tests. Final normal **`make all` passed**
without bypassing asset conversion. Both the first import and final regenerated
base comparison found **454/454 generated outputs byte-identical**: textures,
model/pose/animation bytes, WAVs and loop sidecars. Recipe validation, Python
syntax and scoped whitespace checks passed. No Azahar or hardware run was needed
for this source/build workflow; no original-rendering/hardware parity is claimed.

Exact validation lives in ignored `artifacts/asset-pipeline-validation/`: final
`make-test-final.log`, `make-all-accepted.log`, `roundtrip.json` and
`manifest.json` with frozen source and native binary SHA256s. Snapshot base was
`d0f953205c846ae5f687ce4e5b88696323e9fab3` plus this task's final sources,
recipes and Make rules. An initial shared `make test` stopped at a concurrently
added missing Trouble Center test source; validation used an isolated committed
base instead of modifying unrelated work. The first isolated native attempt
lacked the ignored reference symbol CSV; copied reference inputs enabled the
full successful build. Initial failure logs are retained and are not passing
checks. ISO access stayed read-only; emulator profiles/saves were untouched.

Implementation was committed incrementally as `abed7c83`, `cf3e8c44`,
`b1316b1f` and `29e3d150`. This completes the requested asset-history/workflow
change, not the full remake. Remaining base converters still require the local
ISO, and the import is the selected slice rather than the entire disc. New
resource names need native actor/trigger bindings. Changing geometry requires
matching collision, road, UV/mesh animation and object-support companions;
Blender rig export, general collision authoring, full MusyX macros and adaptive
music remain open. Story messages/IDs/tokens stay original and ISO-backed.

Next: import remaining UI/effect atlas sources and metadata through the same
reviewed recipes; add geometry/collision and Blender animation interchange;
expand the source music/sample catalog as content needs it; migrate remaining
base-converter inputs toward an ISO-independent source checkout. Continue the
original playable-prologue work separately; no full-game completion claim.

## 2026-10-14 — Original sequence owner and native field Exit

Implemented retained current/requested/previous IDs,32-byte work, all eight
source callback rows and resumable original sync/Exit/publication/Init/Main
ordering. Field battle-start requests(4,0,0) without changing current. The
native adapter imports an explicit field snapshot, waits the prior GX queue
through blocking Citro3D FrameBegin/empty FrameEnd, runs the complete field
Exit leaf and publishes current4 before holding at actual battle Init80073f20.
Finishing letterbox reads that shared current and reaches the separate sound
clear800dc0f8. [Source](behavior/platform/ORIGINAL_SEQUENCE_TABLE_AND_FIELD_BATTLE_LIFECYCLE.md)
and [native limits](behavior/platform/NATIVE_ORIGINAL_SEQUENCE_OWNER.md).

Isolated make test passed264/264 C++ and159/159 Python; native make all
DEBUG_SHORT_BOOT=1 passed. Isolated Azahar reached the trio battle without a
detected native exception. Review corrected an initial VBlank-only fence to
an actual GX queue wait. [Exact validation](PROGRESS_ARCHIVE.md#2026-10-14--original-sequence-owner-and-native-field-exit).

Next: bind ordered battle Init sound/event/control/data providers, source
Main/Exit and field return; finish letterbox sound/Key/Ctrl and real deferred
GPU/part/material/shadow/dummy callbacks. Existing gameplay still runs through
its native adapter while the source Init holds. Native field import/checkpoint
reset does not certify original startup or full field lifecycle. Original
numeric/history, event timing, raster parity and physical performance remain
unverified. The playable original-fidelity prologue goal stays active.


## 2026-10-14 — Portable display/camera/animation code documentation

Commit `bea3ba69` adds targeted inline explanations to40 portable display and
animation C++ files plus [display service](code/display-services.md) and
[animation data](code/animation-data.md) guides. The pass documents original
queue class bases/GX tuples, sort branches/tie behavior, borrowed callback/data
lifetimes and resumable state stages; thirteen-camera boot/update/draw and
letterbox scalar ownership; shadow request/reset and soft-effect deletion;
authored camera-road layout/units/offsets/selection/transition masks; converted
PMCHAR/PMANIM/PMPOSE and UVAN/MSAN formats; literal joint scale gate, cubic/delta
scales, cumulative-versus-fractional caches and loop-seam output; source paper
presentation and PointGet layout; atlas clipping, map visibility, pass-local draw
cache and cache-upload range limits. Unknown original tokens/fields stay raw;
native limits/math adapters are distinguished from source constants and measured
behavior. Existing camera-road service comment now correctly says six rooms.

Final frozen baseline `0ac453e0c7ac53e39d7fabc3e81ecf1c44d7a5e6` plus this
comments-only pass passes **264/264 C++ and159/159 Python** through
`CMAKE_BUILD_PARALLEL_LEVEL=4 make test`, and normal native
`make all -o assets-ready -j4` passes. Logs are
`artifacts/display-code-documentation-validation/host.log` and `native.log`.
`manifest.json` records baseline, source hashes, executable-token preservation
for all40 touched C++ files, live/frozen equality and ELF/3DSX hashes. Relative
code-guide links and scoped whitespace checks pass. Validation reuses the prior
accepted private RomFS and regenerated matching icon cache from
`artifacts/game-code-documentation-validation/source/`; other local-asset folders
remain shared read dependencies, and isolated native asset regeneration was
skipped. No new extraction/conversion, ISO write, emulator/profile/save change or
hardware validation occurred. Original numeric/rendering parity remains outside
what comment-only changes or portable tests establish.

A comments-only index patch excluded unrelated worktree changes from commit and
frozen validation. The main renderer/platform header and new class-menu backend
were concurrently under implementation and are not included in this pass. No
blanket summaries or numerical rewrites were substituted for semantic review;
source branches and executable tokens remain unchanged. These40 files extend
the first two batches to96 distinct documented C++ files; file count is progress
tracking, not proof of overall documentation completeness.

Next: audit `source/display/render.cpp` with `include/runtime/platform.hpp`,
covering arena sizes, packed layer/group/cull bits, scene resource transactions,
GPU state/cache synchronization, native framing and original/harness distinctions.
Then cover every `render_*.cpp` adapter (frontend/opening, message/windows/items,
battle resources/HUD/classes), including their matched shaders. New class-menu
modules need separate review once their authoritative implementation settles.
Continue field/NPC/partner modules, remaining battle helpers, message/inventory/
story and relevant port tool/build code; re-audit concurrent runtime additions at
completion. Preserve original assets/dialogue, meaningful local numeric notes,
exact validation and incremental commits. The full **port-code documentation goal
remains active**; portable helper coverage does not finish renderer or whole-port
documentation, and no complete-remake claim is made.

## 2026-10-14 — Shared original sound flags and native letterbox clear

Implemented retained unsigned16-bit psnd flags, complete ClearFlag leaf and
source-ordered resumable SetFlag inherited BGM/ENV fades. Field battle0x10,
item-get0x80 and trio letterbox0x20 share this owner; battle/field/checkpoint
reset no longer erases it. Finishing reads retained current4 and clears the
actual sound word, animates/registers letterbox once and reaches missing GX
viewport802ba56c with21 records. [Source](behavior/audio/GAME_SOUND_FLAG_OWNER_AND_ORDERED_SET_CLEAR.md)
and [native scope](behavior/audio/NATIVE_SHARED_SOUND_FLAG_OWNER.md).

Isolated make test passed266/266 C++ and159/159 Python; native make all
DEBUG_SHORT_BOOT=1 passed. Isolated Azahar reached the trio battle/action-command
presentation without a detected native exception. Source order/width/lifetime
review passed. [Exact validation](PROGRESS_ARCHIVE.md#2026-10-14--shared-original-sound-flags-and-native-letterbox-clear).

Next: actual GPU viewport/scissor/projection inside native frame recording,
original queue callback/part/material/shadow/letterbox/dummy providers, ordered
battle Init/Main/Exit and field return. Native nonzero inherited fade groups,
full sound/history/Key predicates, original event timing, audible/raster parity
and physical performance remain unverified. The original-fidelity prologue goal
stays active; this is a scalar/native continuation milestone.


## 2026-10-14 — Main renderer, platform resources and shader code documentation

Commit `3b45502b` documents `source/display/render.cpp`,
`include/runtime/platform.hpp` and the world/system-window vertex shaders, plus
[new renderer guide](code/renderer.md). Inline numeric/ownership notes cover
native arena budgets, vertex attributes/permutations, color byte conventions,
sidecar dimensions/UV scales, packed map bits, resource residency/cleanup,
model sampling keys, scene atlas admission, alpha-preserving field masks and
three-pass shadow masking, map UV/mesh/fence updates, cutaway holds, source text
records/reflow/atlas cells, presentation RNG/tween cadence, UI hitbox/minimap
choices, frame cursor reset and C2D/PICA ownership handoffs. GPU allocations and
source identities remain distinct; original versus native/harness numerics are
explicit. No shaders, source dialogue or executable behavior were rewritten.

Architecture now matches the actual GPU room policy: synchronize/retire the
previous Room before destination loading to bound linear memory, then attempt
old-room reload on rejection. That reload can also fail. This is separate from
in-memory collision rollback and must not be described as keeping both GPU rooms
staged. The stale v2-only checkpoint overview also now points to the versioned
committed-format guide rather than presenting32 bytes as the current format.
Concurrent save/sequence/class/field implementations remain outside this pass.

Frozen baseline `a8c781a71dd12de712e03927c31fdf518a8b7c5e` plus exactly this
comment pass passes **266/266 C++ and159/159 Python** via
`CMAKE_BUILD_PARALLEL_LEVEL=4 make test`, and normal native
`make all -o assets-ready -j4` passes, including fresh compilation of both
annotated `.v.pica` shaders. Logs:
`artifacts/renderer-code-documentation-validation/host.log` and `native.log`.
`manifest.json` records baseline, source hashes, unchanged executable-token checks
for both C++ files and both shaders, live/frozen equality and ELF/3DSX hashes.
Code-guide relative links and scoped whitespace checks pass. Native validation
reuses the previous accepted private RomFS/matching icon cache and skips pack
regeneration; other local-asset folders remain shared read dependencies. No new
asset extraction/conversion, ISO write, emulator/profile/save change or hardware
validation occurred. Existing compiler warnings remain outside documentation.
The later architecture-prose correction does not change compiled/tested source.

A comments-only index patch excluded unrelated worktree changes. The pass adds
two distinct C++ files to prior96 (98 total) plus two shaders; counts track
review progress only and do not prove whole-port completion. Native rendered
parity and original global dispatch/camera history remain separate claims.

Next: every scene-specific `source/display/render_*.cpp` adapter—frontend/opening,
message/window/item services, battle startup/curtain/HUD/classes—plus their
remaining shaders/resource helpers. Re-review recent class-menu and native camera
GPU providers once their authoritative implementation settles. Continue field/
NPC/partner, remaining battle, message/inventory/story modules and relevant port
build/tool code. Re-audit concurrent additions and changed checkpoint formats at
completion; do not claim complete documentation from comment density, file count
or passing unrelated tests. The full **port-code documentation goal remains active**.

## 2026-10-14 — Correct committed sound reset removal

The next isolated build found commit35db41b had omitted the validated removal
of the old fieldSoundFlags reset. The scoped commit assembler matched a
replacement prefix inside the old line. Removed that obsolete reset in the
committed source and strengthened the owned hunk with complete line boundaries
and obsolete-hunk rejection. The validated sound owner code already had this
removal. Current frozen GPU/source continuation snapshot, including this fix,
passes269/269 C++ and159/159 Python through make test and native make all
DEBUG_SHORT_BOOT=1; logs artifacts/gpu-test-final.log and gpu-build-final.log.
Those checks include pending GPU feature work; no whole-remake claim.

## 2026-10-14 — Persistent Trouble Center state and original physical admission

Added the source lecture flag, all30 completion flags and full quest working
flags/six bytes to native checkpoint v10, preserving known/unknown state and
canonical existing NPC fields. Fresh new-file swInit now initializes this domain;
load/room/menu entry never clear it. Original reset providers clear only working
progress. [Layout and scope](behavior/format/NATIVE_TROUBLE_CENTER_STATE_CHECKPOINT_V10.md).

Recovered the original story23 front lock/notice, same-map door/cutaway and
board/counter case9 contact/lifetime. Board entry uses priority0/typeef;
priority20 NPC stop is a different registration. Source hit centers/normals
are recovered without substituting a proximity trigger or professor mask.
[Original physical owner](behavior/field/TROUBLE_CENTER_DOOR_CASE9_GEOMETRY_AND_LIFETIME.md).

Validation: final isolated make test269/269 C++ and159/159 Python; make
assets-ready and make all pass. New checks cover all48 flags/six independent
bytes, reset scope, unknown migration, corruption and exact disk/backup recovery.
All3110 reviewed files match the snapshot; exact tree/binary/logs are in the
linked contract. Initial concurrent CMake staging duplicated registrations;
scoping fixed that. The obsolete sound reset compile error was corrected in
its owner's separate commit c632bfec and included in validation. No Azahar or
hardware run for this persistence step; physical gameplay admission stays open.

Next: native Trouble Center original door and interior geometry/contact-ray
case9, then original lecture/help/confirmation/acceptance EVT, physical input/
iconMain ordering and original menu PCM. Full original GP/card save, original
execution/raster and New2DSXL hardware performance remain unverified. These
state/providers do not constitute native board gameplay; goal remains active.

## 2026-10-14 — Frame-scoped finishing camera GPU and real dummy

Added actual native viewport/scissor/depth range/full projection adaptation and
original no-write dummy triangle inside admitted renderer frames. The source
Draw/dispatch cursors and borrowed queue survive renderer handoff. Cached
backend restoration reapplies state across frames without repeating the dummy
vertices; simulation emits no GPU work. Finishing reaches camera1's real
ColorUpdate802b9380 wait with21 records. [Frame frontier](behavior/camera/NATIVE_FINISHING_DRAW_FRAME_BINDING_FRONTIER.md)
and [adapter scope](behavior/camera/NATIVE_FINISHING_CAMERA_GPU_ADAPTER.md).

Frozen make test passes269/269 C++ and159/159 Python; native make all
DEBUG_SHORT_BOOT=1 passes. Isolated instrumented Azahar verifies one real dummy
submission,76 state restorations and current camera1/queue21 with no detected
native exception. The test-only harness supplies an explicit resolved Bero
bit1 clear; production room history is not supplied by this fixture. Exact
[validation and limits](PROGRESS_ARCHIVE.md#2026-10-14--frame-scoped-finishing-camera-gpu-and-real-dummy).

Corrected the prior committed sound-reset omission in c632bfec; strengthened
complete-line and obsolete-hunk commit checks. Native capture also motivated
explicit independent renderer blend/write/test restoration after the queued
pass; the final opaque ending capture still lacks the development notice.
No original story dialogue was changed.

Next: original common/class GPU dispatch state, actual shadow/part/material/
stage/audience/menu/EXP/letterbox callbacks, near/far and remaining cameras;
production Bero/get_info, full source Init/Main/Exit and ordered field return.
Original numeric/history, event timing, pixel/audio parity and physical
performance remain unverified. Full original-fidelity prologue goal stays active.

## 2026-10-14 — Native common/class state and captured shadow wait

Implemented independent RGB/alpha/depth write masks, source blend/alpha/depth
class requests and identity ZScaleOffset preserving viewport range. Only the
acknowledged real dummy seeds state; frame restoration preserves later class
settings without geometry or source-request replay. Explicit late-depth
adaptation covers recovered class pairings and rejects unsupported raster
combinations. [Contract and shadow frontier](behavior/display/NATIVE_CLASS_STATE_AND_SHADOW_FRONTIER.md).

Frozen make test passes272/272 C++ and159/159 Python; native make all
DEBUG_SHORT_BOOT=1 passes. Isolated instrumented Azahar records exactly seven
source state requests0..6 once and reaches captured shadowDisp8002d3d8 with
camera1/queue21. Class9 has depth-write0 and SRCALPHA/INVSRCALPHA blending.
The ending development notice is visible again; no detected native exception.
The harness supplies an explicit test-only Bero bit1 clear. Exact
[validation](PROGRESS_ARCHIVE.md#2026-10-14--native-commonclass-state-and-captured-shadow-wait).

Next: actual shadow Polygon setup/geometry/image/ray/cylinder/texture providers,
remaining camera/part/material/stage/audience/menu/EXP/letterbox callbacks,
production Bero/get_info and full source Init/Main/Exit/field return. Empty
eligible shadow domains cannot bypass setup or acknowledge callback success.
Original timing, numeric/history, raster/audio parity and hardware performance
remain unverified; full original-fidelity prologue goal stays active.

## 2026-10-14 — Independent shadow collision continuation and original texture

Added portable collision preparation with a retained camera/pool/ray cursor,
independent source filter identity, fresh post-hit dimensions/flags and literal
binary32 matrix operations. Misses preserve records; bypass records use the
recovered translation leaf. Imported exact original US N64 texture44, without
resizing, with verified pixel/source hashes and original sampler metadata.
[Scope](behavior/display/NATIVE_SHADOW_COLLISION_CURSOR_AND_TEXTURE_IMPORT.md).

Frozen make test passes273/273 C++ and160/160 Python; native make all
DEBUG_SHORT_BOOT=1 passes. No Azahar or hardware run: native shadowDisp binding
is unchanged and this helper has no production ray provider yet. Exact
[validation](PROGRESS_ARCHIVE.md#2026-10-14--independent-shadow-collision-continuation-and-original-texture).

Next: implement actual ordered camera1 Polygon/cylinder/Texture setup and
retained palette/texture binding, then connect independent filter8002d380 rays
and camera2 image/projection services. Remaining part/material/stage/audience/
menu/EXP/letterbox callbacks, production Bero and full Init/Main/Exit/field return
remain open. No callback-success shortcut or story dialogue change; original
runtime/raster/audio parity and hardware performance remain unverified. Full
original-fidelity prologue goal stays active.

## 2026-10-14 — Native Trouble Center front door and original kanban notice

Added original third door destinations/collision groups and separate mask4
interior/exterior visibility. Source story<23 prints original gor_02_035 with
speaker0/toge mode0 and balanced key lock. Implemented missing kanban type4
parser and original TPL29/31/30/32 sign composition: source CLAMP middle rows,
no stretched pixels or ordinary bubble. Original map/art resources are reused;
completed mask4 checkpoints use v10 with known story>=23. [Scope](behavior/field/NATIVE_TROUBLE_CENTER_FRONT_DOOR_AND_LOCK_NOTICE.md).

Reviewed source tree8b6bafe53a16393676a219b06ae867f30213806c passed
make assets-ready (artifacts/trouble-door-assets-validated.log), make test
(270/270 C++ and161/161 Python, artifacts/trouble-door-test-final.log), and
normal make all -o assets-ready BUILD=build-goomba-reward-review
TARGET=ttyd-goomba-reward-review (artifacts/trouble-door-build-final.log).
All3115 tracked files in the private checkout match that reviewed tree;
artifacts/trouble-door-audit-final.log records the audit and normal binary SHA256
5ebdd2c66fbafedecc7ef4640d2807f796b69bb7ed1410f6d0a288a9e0419a92.

Isolated Azahar story22 contact displayed the original wooden sign/red heading/
unchanged gor_02_035 (artifacts/azahar-trouble-door-locked/original-front-door-result.png;
pixel check artifacts/trouble-door-sign-pixels.log). Story23 actual entry shows
original interior floor/walls/board and Mario (artifacts/azahar-trouble-door-loader/
original-front-door-result.png). Its temporary development overlay confirms
mask4, completed door and target(2.5,.04), with60 emulator AppFPS/frame12.48ms.
Both scripts verify copied checkpoints unchanged; profiles/saves are isolated.
The story23 harness used DEBUG_SHORT_BOOT=1 and a temporary main diagnostic,
removed before the final normal build/audit. These are emulator observations,
not physical3DS timings or full original raster/event parity.

Failed early captures exposed overwritten visibility tags from the editable
pipeline and the native loader's stale12-group limit. The explicit inherited
visibility handoff and gor_02-only13-group admission fix resolve that case.
Final pack has156 batches (13:2670 vertices,7:5754), with no draw-time matching
or extra geometry. S_i04_msk is not forcibly hidden; source flag1000 controls
its group-darkness gate, not visibility. Earlier failing captures/builds remain
in artifacts and do not count as passing validation.

Next: real board/counter case9 pool/EVT admission, original lecture/acceptance
and retained menu clocks/input/audio; add original door-scoped c_monban with
its separate1222 talk branch, then exact door/wall/material/collision callback
and original audio ownership. Existing movement/camera/follower and case tick
adapters do not establish original execution/raster or hardware parity. Full
Trouble Center/remake goal remains active.

## 2026-10-14 — Native ordered shadow setup and camera2 ray frontier

Bound source Polygon/cylinder/Texture setup, retained material/TEV/attribute
state and actual N64 texture44 import/sampler/binding inside renderer frames.
Dummy seeds retained A0=129; restoration preserves source state without request
or geometry replay. Camera1's rejected-record domain completes all73 GX
requests before camera2/class5 enters the independent collision preparation.
[Scope](behavior/display/NATIVE_ORDERED_SHADOW_SETUP_AND_CAMERA2_RAY_FRONTIER.md).

Frozen make test passes276/276 C++ and162/162 Python; final native make all
DEBUG_SHORT_BOOT=1 passes. Isolated instrumented Azahar records73 requests
once, loaded16x16/no-mip texture44 with repeat/linear sampler, and ray frontier
800158a4 at camera2/queue21. Three type2/camera4 records retain flags1; no
fabricated preparation. Ending notice remains visible; no detected native
exception. Explicit test-only Bero resolution; production history still open.
Exact [validation](PROGRESS_ARCHIVE.md#2026-10-14--native-ordered-shadow-setup-and-camera2-ray-frontier).

Initial host validation found a stale shared two-door pack against newly
committed three-door source. Regenerated it from the frozen committed converter
into an isolated RomFS overlay; shared packs/source/foreign edits were preserved.

Next: source-correct independent filtered ray/map-context binding, retained
mutable camera2 projection/image profile and ordered suffix, actual geometry/
bounds/image services and remaining source callbacks. Full N64 async/language/
dummy lifecycle, production Bero/get_info, Init/Main/Exit and ordered field
return remain open. Original execution/raster/audio parity and hardware
performance remain unverified; full original-fidelity prologue goal stays active.

## 2026-10-14 — Original Trouble Center diary and scoped event-data prerequisites

Implemented original diary/type7 help-frame art, exact native clip/direct
placement, unchanged parser speed and source CLAMP sampling gutters. The
preloaded Citro2D/Citro3D texture adapter shares flat-window rendering with
kanban, without draw-time conversion/IO/allocation. Exported actual board/
counter installer/main/lecture and callback table dependencies as5464-byte
PMEVT001 data-only closure;205 bounded objects/5segments retain source words
and ADDR32 bindings while excluding executable sections. [Scope](behavior/field/NATIVE_TROUBLE_CENTER_DIARY_AND_SCOPED_EVENT_DATA.md).

Reviewed tree2dffa7c1920d15777ed922a30b8e6f5a84216eaa passes isolated
make assets-ready SOUNDTRACK=gamecube (artifacts/trouble-board-prereq-assets-private.log),
make test (275/275 C++ and164/164 Python, artifacts/trouble-board-prereq-test.log),
and normal make all -o assets-ready SOUNDTRACK=gamecube
BUILD=build-trouble-board-prereq TARGET=ttyd-trouble-board-prereq
(artifacts/trouble-board-prereq-build-final.log and post-harness
trouble-board-prereq-build-restored.log). The restored3152-file checkout has
zero tracked mismatches (artifacts/trouble-board-prereq-audit-restored.log).
Normal binary SHA2562568825b3273277d433f0daca4242c85e176bfd8d4f721fdd5c6c5096dda94fe.

An isolated DEBUG_SHORT_BOOT=1 direct-presentation harness opens unchanged
irai_help_02 over a declared room2/mask4/story23 fixture; it does not dispatch
the board. Actual capture artifacts/azahar-trouble-diary/original-diary-help02.png
shows original paper/help text/advance star,60 emulator AppFPS/frame9.66ms.
The script verifies copied checkpoint bytes unchanged and no detected native
exception. User emulator profile/saves were not used. Harness main.cpp was
removed and the normal build recompiled/audited afterward. This proves this
native diary presentation, not board acceptance or hardware timing.

The first isolated asset preparation failed from misplaced symbol references;
correct private reference directory fixes it. Initial native helper compile
failed because Texture is a private nested Renderer type; deduction through
shared template helpers fixes that without exposing the type. Failed logs
trouble-board-prereq-assets.log/build.log remain and are not counted as passing.
Source review confirms normal type7 opacity, exact CLAMP gutters and data-only
closure; saved-bank/case/EVT/render/audio/hardware parity remains open.

Next: actual native Board/Counter event owner and source case9 lifetime, original
retained print/selection/append/continue and menu callbacks, explicit saved banks,
start/reset/nonblocking-child/write1420 order, quest1/4 NPC/door child providers,
source c_monban and original audio. Type7 art and packaged data do not establish
complete board gameplay or full original raster/event/hardware parity. Broad
Trouble Center/remake goal remains active.

## 2026-10-14 — Actual original Trouble Center event interpreter adapter

Added TroubleBoardEvent: original installer, board and counter streams execute
through EventNativeRuntime with0/ef/20 root constructor and immutable pinned
21-callback program. Explicit known flags/bytes preserve unknown migration;
Check and Start follow source outputs/resets. Original event words retain
lecture/help/selection/acceptance/cancellation branches and independent children.
Root lifetime is exposed separately from remaining child work; absent provider
preserves firstCall. Live working-bank writers support real cancellation/pouch
composition. [Scope](behavior/field/ORIGINAL_TROUBLE_CENTER_EVENT_RUNTIME_ADAPTER.md).

Reviewed treeff6f41a850556954892c06f63366cde7aa5c042a passes make assets-ready SOUNDTRACK=gamecube
(artifacts/trouble-event-assets.log), make test (277/277 C++ and165/165 Python,
artifacts/trouble-event-test-synchronized.log), and normal make all -o assets-ready
SOUNDTRACK=gamecube BUILD=build-trouble-event TARGET=ttyd-trouble-event
(artifacts/trouble-event-build-synchronized.log). All3167 tracked checkout files
match the tree (artifacts/trouble-event-audit.log). Native artifact SHA256
b9c27aa3b7a3927572d6ce0e6bee8bcc294776e260df73d5ed69f24fb2b58f81.

Original resource-backed tests exercise installer order and all16 savedLWs;
lecture No/Yes without immediate menu fallthrough; quest2/13/30 help/confirm/
append/reset/write1420; menu decline/Continue/reentry without reallocation;
quest1 parent death/1420/KeyOn while its child remains pending, then completion;
counter empty/completed-all/30-coin cancellation/29-coin refusal; and late
provider attachment preserving first delivery. All requested message IDs are
looked up in unchanged original US resources. Declared callbacks acknowledge
fixture work; they do not prove actual source world, window or menu execution.
No new Azahar/Dolphin or hardware run: production Game/case/render binding is
unchanged by this standalone adapter.

Review found absent-provider polling consumed firstCall; manager admission now
gates it and the regression verifies later binding. Initial host run crashed
with a stale object/header ABI during concurrent layout edits; synchronized
rebuild removes that mismatch. Adding resource checks then found a fixture
member-name collision; the resource store is now distinct from the message
trace. Failed logs trouble-event-test.log/test-final.log remain and are not
counted as passing; only the synchronized source snapshot/checks above qualify.

Next: bind actual Game world/message/menu/pouch and source audio providers,
install exact case9 board/counter contacts through64-slot ownership/deferred
EVT scheduling, share original field saved banks and lifetime, then source
c_monban and nonempty NPC/door children. Standalone callback fixtures do not
prove interactive board gameplay or physical3DS performance. Broad original
remake goal remains active.

## 2026-10-14 — Native independent battle-map shadow ray

Imported original bti_01/stg_00_1 collision hierarchy in source map/slot/strip
order: five initial slots,166 triangles,engine units and original sphere/cull
metadata. Native retained registry binds independent finishing shadow queries,
with strict edges/endpoints/ties, actual flags publication and tagged-owner
holds. Exact source FMA/reduction forms; sqrt remains a declared numerical
adaptation. [Scope](behavior/battle/NATIVE_BATTLE_HIT_REGISTRY_AND_SHADOW_PREPARATION.md).

Frozen make test passes277/277 C++ and166/166 Python; make all
DEBUG_SHORT_BOOT=1 SOUNDTRACK=gamecube passes. Isolated Azahar verifies all
three type2/camera4 records become flags3 after actual battle-map hits. First
ray selects map0/node25508 A_bti_1_00/triangle1,distance11,point(-100,0,-5),
normal(-0,1,0). Matrices have original normal liftY1 and diagonal(-20,20,-20).
Camera2/queue21 now holds at the profile/image suffix, not the ray. Ending
notice visible; no detected native exception. Explicit test-only Bero history.
[Exact validation](PROGRESS_ARCHIVE.md#2026-10-14--native-independent-battle-map-shadow-ray).

Initial new sphere test retained a12-unit length while expecting a150-unit
midpoint; corrected the test fixture, with no implementation change. Native
original stage snapshot is explicit; full original live map/context/binding/
MOBJ mutation history and PPC numeric/raster parity are not established.

Next: retained mutable camera2 profile, exact viewport/view/projection/image
suffix, source GPU geometry/bounds/cylinder and remaining callbacks. Bind
unsupported registry writers with invalidation/restoration; compare original
rsqrt/math/history against source execution. Full N64 lifecycle, production
Bero/get_info, Init/Main/Exit and ordered field return remain open. Full
original-fidelity prologue goal stays active; original story dialogue unchanged.

## 2026-10-14 — Retained shadow profile target tracking

Bound both mutable original shadowConfig startup rows and size table; primary
word8 is now profileIndex. Camera2 class5 follows camera4.at (target), with
original eye/up/target store order and rounded1000+targetY. Held camera query
pins row/decision; later GPU waits preserve completed updates and collision
matrices. Native TrioAttack supplies its actual retained indexed camera.
[Contract](behavior/display/NATIVE_SHADOW_PROFILE_TARGET_TRACKING.md).

Frozen make test passes279/279 C++ and167/167 Python; make all
DEBUG_SHORT_BOOT=1 SOUNDTRACK=gamecube passes. Original-asset trio integration
asserts actual retained profile/camera equality. No new emulator/hardware
check. Initial builds found an inferred mutable-pointer callback return type;
fixed it to the declared const-camera pointer before passing final validation.
[Exact validation](PROGRESS_ARCHIVE.md#2026-10-14--retained-shadow-profile-target-tracking).

Next: real GXInvalidateTexAll802b7b54, then viewport/scissor readback and
400-square setup, matrix/view save/replace, unconditional Projection setup
and End restoration. Source viewport lies outside current exact integer
native adapter; do not fake completion for type2/zero-counter domains.
Other profile writers/pointer replacement, geometry/images, teardown and
ordered field return remain open. Full prologue goal remains active.

## 2026-10-14 — Original Trouble Center retained presentation

Added a stable noncopyable TroubleBoardPresentation connecting the original EVT
adapter to retained print/append/continue, independent original selection and
existing original task-menu/window/icon owners. Event-local toge selects the
correct mode7 fixed-world lecture anchor; source help02 uses original diary7.
Explicit simulation ordinals order EVT/icons/windows independently of rendering.
Decline reuses the same menu work/cursor; delayed message loading does not lose
first-call mutation. Original cue requests have explicit providers and unavailable
world services yield.

Validation: frozen reviewed tree2e449fd893822000c2c1319a65296d0e020bb90a,
3184 tracked files with0 mismatches. Sequential make assets-ready
SOUNDTRACK=gamecube passed; make test passed280 C++ and167 Python tests;
make all -o assets-ready SOUNDTRACK=gamecube BUILD=build-trouble-presentation
TARGET=ttyd-trouble-presentation passed. Logs: artifacts/trouble-presentation-
{assets,test,build,audit}.log. Native3DSX SHA256
17b16c13b4bc8a1862d222f7034ba207895bddeb97898031f06a3d147387c6e6.
Code commita64a081f. Read-only source review found no scoped lifecycle/poll bug.
No Azahar board interaction was run: native dispatch is still unbound.

Limitation: portable opaque menu work handle; original0x416c Cmap binary allocation
ABI remains unbound. Host world callbacks are fixtures. This owner is not yet
installed in actual native board/counter contact or renderer dispatch. Next bind
case9 contact/deferred lifetime, Game presentation owner/shared banks, actual
Key/directions, pouch, NPC/door children and original cue execution. No native
interactive board or physical3DS performance claim; broad remake goal stays active.
See [contract](behavior/field/TROUBLE_CENTER_RETAINED_PRESENTATION_OWNER.md).

## 2026-10-14 — Native Trouble Center board and counter

Connected exact original board/counter contacts to the retained original EVT,
message/selection/menu, renderer and native audio paths. Case9 accepts A+B and
consumes unavailable recognized contacts without replaying a jump. Source turns
now use freshly verified270-degree bases and the handler1 endpoint over13
simulation calls. Fresh BSS party current0 is separate from Mario party lookup.
Actual pouch coins/items support Q2/Q13 cancellation. Finite source banks import/
export each active visit; finished managers cannot overwrite later field state.
Reentry appends roots into the same manager and retains pending world children,
with a tested slot-capacity guard. Original UI PCM uses resident NDSP buffers.

Validation: reviewed tree6af150ebfa2e546c3e8c05d1e503051bed478fe9,
3199 tracked files,0 mismatches. Sequential make assets-ready SOUNDTRACK=gamecube
passed; make test passed282 C++/167 Python tests after the capacity guard.
Normal make all and DEBUG_SHORT_BOOT=1 build passed; the normal build was restored
and rechecked after scoped-render cleanup. Logs: artifacts/trouble-native-assets.log,
trouble-native-test-final.log, trouble-native-short-build-final.log,
trouble-native-build-corrected.log and trouble-native-audit.log. All five runtime
UI cues match the independent original PCM library (trouble-native-pcm-audit.json).

Isolated Azahar: artifacts/azahar-trouble-native-mcgoomba verifies the original
list, all three help02 diary pages, irai44 confirmation and decline/reopen at the
same McGoomba cursor. artifacts/azahar-trouble-native-lecture verifies first
lecture and separate choice with original mode7/world tail. Stable captures show
60 App FPS and about9.5..11.9ms/frame. Both isolated save copies were unchanged.
These are emulator readings, not physical3DS/source raster parity. Early scripts
sent input before readiness, held Up into the live list or assumed one diary
page; those attempts are not counted as target-flow completion. Corrected
readiness/page-aware scripts passed. Code92806b4f plus scoped cleanup eb3dd161.

Native normal SHA256:86c83526513ce9defdc9d90b3acd94e83ea5cf876d99af2118d184d53a49603e.
Short-boot capture SHA256:259de7f1e0bc3cc832039b38c0a6eeb0ab6d5392f88c65364d9a58f8bbc694ae.

Limits: exact-node native case adapter, portable menu work surrogate, private
field manager/finite GP domain and primary PCM fallback. Actual Q1/Q4 NPC/door
children and other world callbacks remain pending, while later roots can still
run; no fake success. Full party/global-index writers, shared64-case/GP ownership,
MusyX and original runtime/raster comparisons remain open. Next bind actual
source actor/door/item-world providers and shared field lifecycle, then extend
additional areas with original event/resource packs. Broad remake goal stays active.
See [contract](behavior/field/NATIVE_TROUBLE_CENTER_CONTACT_AND_PRESENTATION.md).

## 2026-10-14 — Native camera2 shadow passes and shared restoration

Bound real PICA texture-cache invalidation, retained logical viewport/scissor
readback, square setup, original Frustum/Ortho terms, independent current-view
copies, full unconditional Projection GX prefix and live GP14 material choice.
Source400-square viewport uses declared nearest physical endpoints on the
native target. Scissor remains active across viewport setters and holds.
Original auxiliary flags2/profile0/color40 owners now execute all class0/2/7
counter0 setup/restoration, shared saved buffers and color-write disable/enable.
[Contract](behavior/display/NATIVE_SHADOW_CAMERA2_PROJECTION_AND_RESTORATION.md).

Source-matched frozen make test passes282/282 C++ and167/167 Python; normal
make all DEBUG_SHORT_BOOT=1 SOUNDTRACK=gamecube passes. Isolated Azahar actual
ending flow completes camera2 and reaches camera4 DispAudience801a38f8 with21
records retained. Camera2 view/viewport/scissor restored; RGB writes enabled;
auxiliary flags2/counters0 and all3 prepared type2/camera4 records retained.
No detected native exception; ending notice still shows field return pending.
[Exact validation](PROGRESS_ARCHIVE.md#2026-10-14--native-camera2-shadow-passes-and-shared-restoration).

Validation corrected an arithmetic expectation (17.8 versus35.6) and a guessed
next callback (actual audience precedes cylinder). Shared audience atlas had
advanced to CLAMP guards; isolated old-source recipe now has its matching
private pack. Wrong working directory briefly rebuilt the shared pack; restored
its current converter output and verified both original-resource tests.

Next: bind actual camera4 audience prefix/200-record admissions and genuine
palette/matrix/quad services, then cylinder geometry and other callbacks.
Source audience/cylinder contracts delivered; do not assume unknown population
is empty. Nonzero shadow image paths, unknown profile/global writers, numeric/
raster hardware parity, production Bero/sequence lifetime and full ordered
battle-to-field return remain open. Original dialogue unchanged; prologue goal
remains active and incomplete.


## 2026-10-14 — Original Borodo actor and all-task cancellation tables

Added original gor_02 ボロドー２/c_borodo_n using the source tribe, RGBA art,
S_1/T_1/W_1/R_1 animations and original init branches. PMNPC009 retains exact
Shift-JIS event names separately from model identity and validates finite init
profiles; old versions remain readable. The original Q4 start/end children now
move the actual retained NPC previous/current/movement roots. Collision and
rendering use that same owner; switches do not replay init over later positions.
Missing actors/services remain pending. No replacement story dialogue added.

Cancellation now consumes all30 original irai_item_table pointers from the
immutable PMEVT program, replacing the Q2/Q13-only adapter. Nine source key
lists and21 empty lists preserve original order/sentinel semantics. All reset
the original flags/bytes, repeatedly count/remove listed keys through the live
pouch, and preserve other resources. Original caller owns the30-coin fee and
independent end child. Commits49a12f66 and3b855583.

Validation: frozen tree674c5d646308481654143ed0a672c165654b4213;3210 tracked
files,0 mismatches (artifacts/trouble-world-audit.json). Sequential
make assets-ready SOUNDTRACK=gamecube passed; make test passed284 C++/168 Python;
make all and DEBUG_SHORT_BOOT=1 native builds passed, then normal boot restored.
Logs: artifacts/trouble-world-assets-final.log, trouble-world-test-verified.log,
trouble-world-build-final.log and trouble-world-short-build-final.log. Checks
cover all30 real counter paths, actual Q4 acceptance/end reposition, unknown
source init state, below-map placement and persistent live positions.

Native verification caught a2MiB atlas import failure; the same original RGBA
atlas now uses512x256/512KiB with normalized UVs unchanged. Valid isolated
Azahar alley captures show the original blue Bandit and the source completed
task placement; field readings hold60 App FPS, approximately10..13ms/frame.
Captures: artifacts/azahar-borodo-clear-normal and azahar-borodo-clear-completed.
These are emulator readings, not physical New2DSXL or source raster parity.
Earlier captures are not accepted appearance evidence: -380/-180 was inside
house01 camera zone; -500/-180 overlapped Mario; -500/-100 was blocked by map
collision. Actual validated player checkpoint is(-500,0,-270), outside the
original NPC at(-475,0,-180). No speculative wall-hiding change was added.
Both final isolated save copies remain byte-identical to their fixtures.

Normal3DSX SHA256:ac2ce1cf1a9e62edcf4af6ac0544384c57b429a0302a8694b9659b4802e82a03.
Capture3DSX SHA256:e4d9dd017d8fee55cb5eccf597cc7d373d10006195d94c3737b0575e3b75e34a.

Initial host checks exposed two fixtures using old catalog offsets; corrected
PMNPC009 strides. The new start-child test needed its injected case contact
restored after its setup simulation tick; corrected and rerun. These failures
remain archived in trouble-world-test.log/trouble-world-test-final.log.

Limits/next: bind Garf/c_monban and actual house05 mutable72-byte door-copy
owner/GW11/NPC-list consumers; separate actor registration from draw visibility
before named movement of door-hidden actors. Borodo's talk44518 remains unbound.
Full shared GP/case/field event lifetime, generic evtGetFloat/name ABI and
original runtime/raster comparison remain open. Continue original area packs
and object/event owners using these data-driven catalog/service boundaries.
The broad remake goal remains active and incomplete.
[Native scope](behavior/field/NATIVE_BORODO_POSITION_AND_ALL_TASK_CANCELLATION.md);
[source alley diagnosis](behavior/field/ROGUEPORT_EAST_BORODO_ALLEY_CAMERA_AND_OCCLUSION.md).


## 2026-10-14 — Original Garf actor, init and ordinary talk

Added source gor_02 盗賊団２/c_monban with original art/animations/tribe
dimensions and flags. PMNPC009 profile2 validates the exact raw Shift-JIS name
and resource, waits for known1420 and initializes445/0/120 for quest1 or
10/0/-460 otherwise. Init is retained, not replayed after state changes.
The actual Q1 start/end children now reposition that same collision/render
body, then still wait on the missing house05 door owner.

Ordinary1420!=1 talk uses all eight original US message IDs/story branches,
including the unsuffixed gor_02_011 and absence of a112 threshold. Unknown
quest/active quest1 do not fall through to ordinary dialogue. Original key49
selection/advice/unlock scene remains unbound. Code commit91d057c4.

Validation: frozen tree3f37e000de37c812b314e8b8a46abe36147725f4,3221 tracked
files,0 mismatches (artifacts/garf-source-audit.json). Sequential
make assets-ready SOUNDTRACK=gamecube passed; make test passed285 C++/168 Python.
Normal make all and DEBUG_SHORT_BOOT=1 build passed; normal boot restored and
its3DSX hash reproduced. Logs: artifacts/garf-assets.log, garf-test.log,
garf-build.log, garf-short-build.log and garf-build-restored.log. Checks cover
source ordinary IDs/boundaries, unknown init state, exact model/name, no replay
and real Q1 acceptance/cancellation child positions. Source review found no
blocker within this bounded scope.

Isolated Azahar artifacts/azahar-garf-clear shows original Garf at quest1
445/0/120 from player520/0/200,60 App FPS/about10.2ms frame. New original
atlas uses512x256 RGBA8/512KiB; rendering uses the existing room-shared
Citro3D path. The copied checkpoint remains unchanged. Initial400/120 fixture
was map-wall blocked;350/50 had no ground;445/200 loaded but overlapped Mario
in projection. Only the final clear view counts as appearance evidence.
These are emulator readings, not hardware or original runtime/raster parity;
ordinary talk raster and natural house05 access remain unverified.

Normal3DSX SHA256:14c7cc80ecef78bfab444604ee8a19b2d73323ee29f4758305ca48700640c869.
Capture3DSX SHA256:445c9f6cf921b19ec45764645fa60939ffe75e97c6170734f6381c365d450f3e.

Next bind actual house05 mutable72-byte copy, GW11 and its map lifetime/NPC-list
consumers. Separate registered/name-addressable actors from render suppression;
retain mutable800/main flags independently of per-tick static catalog refresh.
Then bind key49 selection/unlock children and Borodo talk. Full shared case/GP/
field lifetime, original runtime comparison and additional area/object/event
packs remain open. Broad remake goal remains active and incomplete.
[Native scope](behavior/field/NATIVE_GARF_INIT_AND_ORDINARY_TALK.md).


## 2026-10-14 — Native per-record cylinder shadow geometry

Bound original prepared record matrices, TEVREG0 alpha and all three cylinder
passes to native Citro3D commands. The existing private shadows.pmshadow pack
supplies original indexed fans/strips, positions and both cylinder palettes.
Retained cursors preserve acknowledged commands and captured list arguments;
source alpha OR, destination-alpha blend and front/back culling use verified
native mappings. Cube resources and camera1 bounds remain unavailable.

Validation: isolated source based on b866809147dc03239869733043e746b147dd9f9c
plus exact owned edits; make test passed284 C++/167 Python and make all passed
(DEBUG_SHORT_BOOT=1,SOUNDTRACK=gamecube). Logs: artifacts/cylinder-test-final.log
and cylinder-build-final.log. Test-only provider harness also built, but isolated
Azahar cylinder-emulator.log stopped during Normal Jump before reaching its
probe. No GPU provider result or visible shadow parity is claimed. The displayed
pending-service notice is generic; existing Jump damage domains for targets1/2
remain unsupported and the actual pending address was not captured in this run.

Limits/next: bind the preceding retained camera4 audience callback801a38f8 and
producer/current-record state. Command cursors do not preserve framebuffer pixels
across provider holds after emitted geometry. Cube/bounds, nonzero image suffix,
full battle draw completion/field return and original raster comparison remain
open. Broad prologue goal remains active and incomplete.
[Provider scope](behavior/display/NATIVE_CYLINDER_SHADOW_PROVIDER.md).


## 2026-10-14 — Retained NPC flags and corrected house05 consumers

Named lookup now uses retained initialized active1 instances independently of
main800/draw visibility. NPC main-flag OR/AND writes persist through static
catalog publication and wandering-driver updates. Hidden actors retain named
position service, collision and candidacy; native drawing skips800 geometry.
Inactive slots cease to be name-addressable; map/catalog reset discards runtime
overrides. Scripted Professor setters and full coverage/registration lifetime
remain outside this bounded domain. Codebbbc106d.

Original-source research corrected the house05 plan: manual installer snapshots
closeLW15=0/openLW15=4 and omits the generic initial NPC-hide tail. Its record
word0=0 skips all generic mask1 main800 consumers. The actual unguarded copied
NPC-list consumer changes animPose material1800 via actor+104 instead. No
main800 action was speculatively bound to house05; GW11 and actual72-byte copy,
case/list lifetime and material consumers remain the next world-owner work.

Validation: frozen tree77be2517f8c79a29e5b217f273628d04d183e9db,3224 tracked
files,0 mismatches (artifacts/npc-flags-audit-final.json). Sequential
make assets-ready SOUNDTRACK=gamecube passed; make test passed285 C++/168 Python;
normal make all passed. Isolated diagnostic short-boot build also passed;
normal source/boot restored and normal3DSX hash reproduced. Logs:
artifacts/npc-flags-assets.log,npc-flags-test-final.log,npc-flags-build.log,
npc-flags-probe-build.log,npc-flags-build-restored.log. Initial test compile
required copying a const body before calling the state-mutating collision
admission helper; fixed, with earlier failure archived in npc-flags-test.log.

Isolated artifacts/azahar-npc-flags has inspected Garf visible/hidden/visible
images at60..61 App FPS/about9.6..10.1ms per frame. The diagnostic-only B hook
calls the actual retained flags API; it is not an original event binding and is
absent from committed main/normal build. Diagnostic source and input script are
archived as npc-flags-main-probe.cpp/npc-flags-smoke-input.py. Copied checkpoint
is byte-identical to its fixture. No new art/music/text conversion or original
assets were committed. Emulator readings are not hardware/source raster parity.

Normal3DSX SHA256:31ee6bae37761dcafa4c255ec4102cc95356e77ab0ee36c2aeb7b33bfe5d8807.
Diagnostic3DSX SHA256:e3e00fa7e70b850080e3ae6e580603e5c431738784c22f3b79f5feb93f725475.

Next create actual house05 mutable record/GW11/map ownership, preserving the
source inline installer and case-local snapshots. Bind its real material-list
consumer separately from main flags; parameter writes should complete only
once their real copied-record mutation occurs. Continue Garf key49/unlock,
Borodo talk, shared case/GP/field lifetime and further original area packs.
Broad remake remains active and incomplete.
[Native scope](behavior/field/NATIVE_RETAINED_NPC_FLAGS_AND_DRAW_SUPPRESSION.md);
[source installer](behavior/field/HOUSE05_ORIGINAL_INSTALLER_GW11_AND_NPC_LIST_CONSUMERS.md).


## 2026-10-14 — Actual retained audience callback, empty-admission domain

Bound original801a38f8 to the invoking finishing display record, with retained
BaseGetPtr/camera4 capture,16 real GX setup requests and200 fresh current-entry
admissions. Completed requests and skipped records do not replay across holds.
Any admitted actor remains held at its full unbound body. The native shader
owner now retains original RGB TEXC+RASC, alpha TEXA*RASA and register material
selection; the prefix does not invent a material or texture load.

Validation: isolated snapshot based on c71e04291faa8a6815a8de4e3e7f08c2994c697d
plus exact owned hunks. make test passed287 C++/168 Python; make all passed
DEBUG_SHORT_BOOT=1,SOUNDTRACK=gamecube. Logs: artifacts/audience-prefix-test-verified.log
and audience-prefix-build-verified.log. Initial test setup lacked its writable
artifacts directory/new original NPC catalog, then private Python asset paths;
fixed the isolated setup and reran. Initial integration expected both audience
passes before actors; actual source order reached80101774 after the opaque pass.
Corrected the assertion. Prior failed logs remain archived.

Host real-queue fixture checks the full initializer's200 zero records without
clearing them, then passes opaque audience and reaches actor80101774. Native
isolated deterministic physical/menu-input harness reaches the ending, passes
audience and holds at DispItem801a37f0, camera4/class1,queue21. Exact SD trace:
artifacts/azahar-audience-prefix-inputs-long/data/azahar-emu/sdmc/3ds/ttyd-2ds/draw-frontier.txt.
The fixture explicitly clears Bero bit1; it does not acknowledge missing combat
or render callbacks. Earlier60-second captures stopped in dialogue/Headbonk.
No cylinder lists, ordered render completion, original raster parity, hardware
performance or field return are claimed. Production sources/binary contain no
harness inputs, trace writes or Bero clear. Source/binary hashes are recorded in
artifacts/audience-prefix-validation-manifest.json.

Next: bind original inactive DispItem/other icon domains and actual borrowed
part callback80101774, then reach translucent audience/cylinder geometry.
Nonempty audience palette/pose/quad cursor, physical channel constants with
unknown owners, persistent framebuffer across holds, cube/bounds and nonzero
shadow images remain open. Broad prologue goal stays active and incomplete.
[Native scope](behavior/display/NATIVE_RETAINED_AUDIENCE_EMPTY_ADMISSION.md).


## 2026-10-14 — Original house05 copied record and actual GW11 writes

Implemented the finite original gor_02 copy/list-selection prefix after retained
NPC initialization. Original18-word/72-byte data is copied without embedded
pointer rebasing into map-owned native storage; GW11 receives its actual guest
address. Original Q1 start/end children now mutate record+34 through the real
DoorParameter service, selecting the original two/three-name lists after their
actual Garf position calls. Source records stay immutable. Parameter changes
alone do not hide actors or change main800/material1800.

The descriptor/owner supports more room records later. Room/catalog/data reload
invalidates copied addresses before held-NPC early returns; event death alone
does not free map storage. Unknown initialization inputs or missing copied data
hold their actual unfinished work. The bounded native guest arena is an explicit
adaptation, not a recovered original allocator/shared global EVT address space.

Validation: make assets-ready SOUNDTRACK=gamecube passed; frozen source tree
`d3e0a7e2bc4f9bd7ec433eab55c4f1828838fc30`,3235 tracked files,zero checkout
mismatches. make test passed287 C++/169 Python; make all passed with normal
startup and SOUNDTRACK=gamecube. Host tests independently compare original raw
record bytes/relocations and both SJIS lists; check actual mutable copies,
initialization once, stale addresses, malformed replacement and held-simulation
room/catalog changes. Native-board host integration runs original Q1 start/end
children against the actual Game-owned record. Logs: artifacts/door-data-assets.log,
door-data-test.log and door-data-build.log. Feature commit2a2cc97d; its owned
files/owned CMake block match the tested tree. Concurrent audience work was
preserved; this does not claim the whole merged HEAD was tested together.

Native isolated Azahar run exited0 with unchanged fixture save and no application
exception. The first45-second normal-startup capture stayed in the opening demo;
four physical A inputs in a second run reached file selection. Six physical A
inputs in artifacts/azahar-door-data-restored loaded the valid Garf fixture.
Inspected physical-a-5/6.png: original Rogueport east art, Mario/Goombella and
Garf visible,60 App FPS,7.89..8.47ms reported emulator frame time. This checks
restored field rendering only, not native execution of the Q1 parameter write
or complete original door behavior. User profile/saves were not touched; no
production diagnostic hook or startup change was introduced.

Normal3DSX SHA256:5535e4a845b72bc195c421f92e6e5b647c0705179358de7908fe21501dd735d8.
Ignored archive: artifacts/door-data-normal.3dsx. No original data or generated
pack/binary was committed.

Next bind source house05 inline case registrations with exact close/open LW15
snapshots, geometry/collision owners and separate animPose material1800 lists.
Continue Garf key49/unlock, Borodo original talk, shared case/GP/field lifetime
and additional original area packs. Full original house05 interaction, native
Q1 mutation capture, raster parity and hardware performance remain unverified.
Broad remake goal remains active and incomplete.
[Native scope](behavior/field/NATIVE_AREA_DOOR_DATA_AND_GW11.md);
[source installer](behavior/field/HOUSE05_ORIGINAL_INSTALLER_GW11_AND_NPC_LIST_CONSUMERS.md).


## 2026-10-14 — Retained audience item/AP local draw bodies

Bound actual DispItem801a37f0 and DispApSrc801a36c8 to their invoking records.
Their18-command GXInit, AP4-state suffix and distinct100-record banks retain
original order/owner captures. Active items/AP execute complete six/five SDK
matrix-request graphs and retain genuine icon/alpha arguments until the original
icon service becomes available. Inactive entries perform source setup/scan;
no actor-bank inference, bank clearing or missing-geometry acknowledgement.
Native shader retains texture-times-vertex RGB/alpha and native matrix adapters
preserve row-scaled translation, actual Item radians and Concat zero-term FMA.

Validation: frozen8adbd48618dbe9b273587e0c2564c14c486e19be plus exact owned
hunks. make test passed289 C++/169 Python; make all passed DEBUG_SHORT_BOOT=1,
SOUNDTRACK=gamecube. Logs: artifacts/audience-aux-test-verified.log and
 audience-aux-build-production.log. The final focused matrix/cursor fixture also
passed after formatting-only cleanup (audience-aux-local-final.log).
Initial suite lacked the newer base's private door-data pack; regenerated that
existing recipe in the isolated snapshot and reran. Source review corrected a
fixture's Item radians constant and the adapter's omitted Z-column zero FMA;
AP state fixtures now apply real NativeDisplayState instead of only tracing IDs.

Native isolated180-second physical/menu-input harness completed actual Item
setup/scan: auxiliary-complete.txt records801a37f0, camera4, vertex material1,
textureRGB1. Next hold is borrowed-part80101774, camera4/class1,queue21.
Evidence: artifacts/azahar-audience-aux/data/azahar-emu/sdmc/3ds/ttyd-2ds/.
AP is later in the actual queue and was not reached. No cylinder lists or complete
battle rendering/field return are claimed. Test-only Bero bit1 clear remains
explicit; normal sources/build contain no trace, controller or Bero changes.
Hashes are in artifacts/audience-aux-validation-manifest.json.

Delivered original part matrix/material/history contract and corrected older
trio prose: DrawMtx uses unshifted stack520; −5 belongs only to separate stack4c0
history. Mandatory history is ten68-byte records, separate from geometry.
Next bind80101774 actual matrices, poseF0/E8 material stores, three DrawMtx
passes and CPU history owner. Then reach translucent audience/cylinder/AP,
complete icon metadata/current-camera/GX providers and ordered field return.
Original raster/SDK rounding, hardware performance, nonempty icon ownership and
persistent target pixels across render holds remain open. Broad prologue goal
stays active and incomplete.
[Native item/AP scope](behavior/display/RETAINED_AUDIENCE_ITEM_AP_CALLBACKS.md);
[part source contract](behavior/display/ORDINARY_BATTLE_PART_CALLBACK_MATRIX_MATERIAL_AND_HISTORY.md).


## 2026-10-14 — Original house05 interior map/hit initialization

The retained house05 owner now executes the next recovered inline prefix:
read record+20/+24, hide the actual S_ie_05_naka subtree and disable actual
A_ie_05C hit descendants. These sequential requests complete once, preserve
later event changes and reset with area/resource ownership. This corrects an
initialization gap: house05 interior had no native visibility group. It removes
7416 of100332 east-room vertices from submission and disables23 source collision
identities. Existing ground/wall/head/support queries share the new filter.

A generic named map overlay retains original hierarchy, source identities and
16-bit vertex owners for five outdoor maps. Per-area metadata can add future
maps. Native drawing intersects existing Citro3D batches with cached visible
ranges; no per-frame vertex rewrite/texture upload or callback allocation was
added. Existing story/interior adapters remain independent. Only map bit1
visibility and the recovered hit subtree enable/disable effect are consumed;
full source flag words, transform and lighting ABIs are not claimed.

Source review found that house05's saved LW4..14 depend on earlier attached
Bero/door child execution and cannot be replaced with a fabricated zero bank.
New source contract records that frontier. Case9 records and original generic
door open/close execution remain unfinished; no synthetic mask8/door interaction
was introduced. See [native scope](behavior/field/NATIVE_NAMED_MAP_FLAGS_AND_HOUSE05_INIT.md)
and [incoming locals](behavior/field/HOUSE05_INCOMING_LOCALS_AND_INSTALLER_FRONTIER.md).

Validation: frozen final tree ded135326ab842f1719b05de03556ac5aef713c0,
3246 tracked files,zero checkout mismatches. make assets-ready SOUNDTRACK=gamecube
passed after the compact owner format update; make test passed289 C++/170 Python;
normal make all passed and reproduced the native binary. Initial C++ run had
one new fixture failure: a coplanar outdoor floor wins the original triangle
scan ahead of the interior floor. Replaced that query with the actual elevated
house05 box surface12e490; no production change was needed. Final integration
checks disable/enable/disable through real Game ground queries, nonreplayed
init, named subtree isolation and held room reentry. Original geometry
vertices/batches/ground/walls/head are byte-identical to the existing five-map
resources. New map flag packs total1472942 bytes plus352-byte room descriptor.

Logs: artifacts/field-map-assets-final.log,field-map-test-final.log and
field-map-build-final.log. Earlier asset/test/build logs and diagnostic query
probes remain ignored for audit. Feature commit ed125a6f's owned files/CMake
block match the final tested snapshot. Concurrent work was preserved; validation
is scoped to that snapshot, not the whole concurrently merged HEAD.

Isolated Azahar artifacts/azahar-field-map-flags exited0 with no application
exception; normal title/file flow loaded the existing Garf fixture using six
physical A inputs. Inspected physical-a-5/6.png show original east-room art,
Garf, Mario and Goombella at60 App FPS,8.03..8.67ms reported emulator frame time.
The isolated fixture save stayed byte-identical. No user profile/save changes,
production diagnostic hooks or startup skips. This is a field-render smoke,
not a matched original door trace or hardware performance result.
Normal3DSX SHA256:10a11783444b317fc80aee0e32d4f2c47c91b91a59b4ee5f0eb66296b72fbbfd;
ignored archive artifacts/field-map-normal.3dsx.

Next recover/bind actual incoming Bero/local-bank ownership and house05 inline
case snapshots, then original open/close events and actual camera/Mario/party,
map transforms/collision, material1800 and audio consumers. Continue Garf key49
unlock, Borodo original talk and further original area packs. Full original
house05 gameplay, raster parity and hardware performance remain unverified;
broad remake goal remains active and incomplete.


### Normal boot validation for retained item/AP change

Normal make all SOUNDTRACK=gamecube also passed with separate
build-audience-aux-normal directory (no DEBUG_SHORT_BOOT define):
artifacts/audience-aux-build-normal.log. Fresh isolated12-second Azahar capture was black with0 App FPS; the longer
45-second run reached the original storybook and unchanged US text at60 App FPS.
Evidence: artifacts/azahar-audience-aux-normal-long/boot.png and
 audience-aux-normal-smoke-long.log; earlier12-second capture is retained.
Temporary startup trace also compiled and was removed from the frozen sources.
This verifies normal startup, not full fresh-file prologue or hardware parity. Normal3DSX SHA256:
32e14011a820bce622af72a8a462c940f799e8803d79b8fbc30e64e2b2964117.
Runnable artifact: artifacts/audience-aux-source/ttyd-audience-aux-normal.3dsx.
Source implementation commit0d0eda1c; broad goal stays active and incomplete.


## 2026-10-14 — Exact original Bero tables and incoming query publication

Added the actual namespace-qualified Bero tables for all five outdoor maps:
3/1/2/5/4 entries in source order, including pipe/other entries omitted by the
cardinal ROADv1 adapter. Retains original60-byte rows, relocated pointers,
strings, null fallback, direction-prefix data and original default entry EVT.
The pack is1828 bytes plus144 bytes of room/HitObj bindings; referenced script
bodies remain unbound. Existing native road selection/placement is unchanged.

FieldBeroInfo now owns a real incoming-name alias and source-correct into_info
values. Successful Game::finishWarp publishes the original index/count/type/
direction/position-pointer/entry-script/mask outputs for the actual destination
entry. Query writes only LW1..8 and preserves other caller words/flags. Empty
incoming save-load selection and ordinary fallback differ as in source; LW8 is
ffff0000, not -1. Checkpoint restore alone does not establish an incoming entry,
and resource/room invalidation clears current publication. No false get_info
HitObj registration or BeroEXEC reset was introduced.

Source review also recovered the later ordinary initial_pos versus save-load
case-entry writers; the latter reaches LW13/14. Exact full incoming bank and
house05 case snapshots still require their real position/case/party/global
providers. See [native scope](behavior/field/NATIVE_BERO_TABLES_AND_INCOMING_QUERY.md)
and [complete source query](behavior/event/BERO_SELECTED_ENTRY_AND_INCOMING_LOCAL_BANK.md).

Validation: make assets-ready SOUNDTRACK=gamecube,make test and normal make all
passed on frozen tree072131578ff71f0c56e87ed5dc25bcb3e9129c6e:3260 tracked
files,zero checkout mismatches;291 C++/171 Python tests. Tests compare all
original table rows/relocations and fallback/default/prefix bytes; exercise
ordinary road/pipe, missing/empty incoming state, save-load arm, pointer reads,
untouched caller storage, area/resource invalidation, checkpoint unknown state
and actual Game road-arrival publication. Logs: artifacts/bero-info-assets.log,
bero-info-test.log,bero-info-build.log. Commit7de92eb0 owned files/CMake block
match the tested snapshot; concurrent work is preserved. This does not claim
the entire concurrently merged HEAD was validated together.

Isolated native artifacts/azahar-bero-info-roads exited0 after two plaza↔east
round trips; all four fixture-only saved-room assertions passed. Early arrival
captures included rolling load FPS14/25 and an unsettled camera, so these are
not steady-performance evidence. Follow-up artifacts/azahar-bero-info-steady
exited0 after another round trip with ten-second settles: inspected east-steady
and plaza-steady show original art,Mario/Goombella in view,60 App FPS and
9.68/12.92ms reported emulator frame times. Native capture does not directly
trace the eight query values; their correctness is covered by source comparison
and the actual Game host integration. Existing load latency, matched original
raster/entry timing and hardware performance remain unverified. Both runs used
isolated profiles/copies of the existing story11 east-road fixture; no user
profile/saves, production diagnostic hook or startup bypass was changed.
Normal3DSX SHA256:146b00dd4fecd856a220029f15cbc647d70f4b3c9b7295db4144a09f3e02969a;
ignored archive artifacts/bero-info-normal.3dsx.

Next bind actual get_info HitObj validation/registration/shared globals,
get_info_num/current-number and original placement/party services; retain later
initial_pos and save-load case allocation outputs before constructing exact
house0516-local snapshots. Continue generic door events/material1800/key49,
Borodo talk and additional original area packs. Full door gameplay and broader
remake remain active and incomplete.

### 2026-10-14 — Ordinary battle part matrix/material/history continuation

Implemented retained btlUnitPartsDisp80101774 local body in
`battle_part_display`: original SDK request graph, six rotation orders, current
camera correction and negative-Z rebuild, live float/gravity phase providers,
source material/color gates, fresh pre/post hooks, three DrawMtx passes and
embedded ten-record history publication. Held inputs stay pinned; later source
reads stay fresh. Draw matrices have no minus5 history-only bias. Negative pose
handles return before SDK work; special20000000 remains an explicit hold.
Source evidence corrected native phase halves to unsigned and GetSin arithmetic
to rounded float multiply/divide before double sine and float return. Native
SDK/trig numerical identity remains unproved.

Actual camera4 borrowed part invocation now binds VM memory, source angle table,
status predicates, pose colorF0/flagsE8 and ordinary unlinked model-pass admission.
It progresses to inner8003bc68 without acknowledging geometry or publishing
history. Runtime integration checks real queue retention/material stores and
unchanged history across that hold. Host tests exercise successful full local
bodies with explicit providers, held SDK/material/pass/history calls, source
poison arithmetic, six rotation orders, mirror, hooks, negative handles,
signed-zero rotation and high-bit unsigned phases.

Validation source is isolated at `artifacts/part-display-source/`, frozen from
63dd401f plus only this change; foreign shared edits are excluded. Initial full
suite failed missing Trouble/field-map/Bero pack dependencies; refreshed only
that snapshot's private original-derived packs (see ASSETS). Final commands:
`CMAKE_BUILD_PARALLEL_LEVEL=4 make -C artifacts/part-display-source test` and
`TMPDIR="$PWD/artifacts/native-stage-map-tmp" make -C artifacts/part-display-source all -o assets-ready BUILD=build-part-display-normal TARGET=ttyd-part-display-normal -j4`.
Both commands exited0:292/292 C++ and171/171 Python tests; normal native build
completed. Release logs `artifacts/part-display-test-release.log` and
`artifacts/part-display-build-release.log`; validation manifest records code and
private pack hashes. Normal 3DSX SHA256
`6f21cabc60ab9d1242b1a4f87ea428351a529c6b93b7d73c1d85f51255798369`.

Isolated180s Azahar ending harness used deterministic physical/menu inputs with
an explicit fixture Bero bit1 clear, no combat writes or fake renderer results.
`artifacts/azahar-part-display/.../draw-frontier.txt` reports pending8003bc68,
camera4,21 entries, callback80101774,class1; preceding Item801a37f0 completed.
Capture predates the final unsigned-phase/GetSin correction, which is covered by
release host tests and native rebuild. The screenshot remains the black ending
fade with development field-restoration notice, not geometry/parity evidence.
Harness changes are restored and excluded from source history. Final release
normal45s fresh-save smoke exited0 with original storybook/US message visible
and60AppFPS in `artifacts/azahar-part-display-normal/boot.png`; this verifies
boot only. Emulator profiles/saves are isolated under project artifacts.

Next: bind existing inner-draw contracts starting with mapTestXLU8001ca2c/current
map owners, exact material/facing/cache context, retained current-camera group
traversal and actual indexed source geometry. Keep history and queue completion
behind those real submissions. Linked/pool/special poses, SDK/trig equivalence,
nonzero hooks, AP active icons and field return remain open. Prologue acceptance
and original-game parity are unfinished; goal and EVT-01 remain active/open.

### 2026-10-14 — Original Bero registration and transaction-owned collision filters

Commits f0d7504b and5993c58d fix geometry rollback ownership and bind the next
original area services. Collision staging now clears/swaps all door/event/fence
filters and their caches with geometry; rollback immediately restores old masks,
including reused source-node IDs. Regression tests use the original house05
surface and candidate Game initialization, not synthetic collision geometry.

FieldMapFlags retains source-proven zero-initialized HitObj halfword flags:
name lookup excludes80 but ignores1; ray eligibility excludes0x81. Actual native
road arrival validates all original Bero names/IDs, publishes table registration,
resets only used BeroSW slots and the real BeroEXEC state once. Indexed functional
queries preserve other caller storage and reproduce LW3..14/default exit EVT;
length defaults/authored values feed the cardinal final offset while start radius
stays original30. Current-number BSS0 and literal signed writes survive get_info;
external checkpoint restore leaves history unknown. Shared per-room catalogs and
owners apply across all five existing outdoor areas and future imported tables.
No new per-frame geometry upload or artwork was introduced.

Missing-name diagnostic-ring semantics, unchecked invalid indexing and complete
raw EVT alias/float-store ordering remain explicit native boundaries. Case pool/
BeroID/switch callers, original Mario/party position callbacks and encoded high
locals remain unbound. Corrected earlier docs: used switches only are reset,
switch_on/off do not allocate/delete cases, and signed conversion bias is
2^52+2^31. See [native contract](behavior/field/NATIVE_BERO_REGISTRATION_AND_COLLISION_ROLLBACK.md).

Validation used frozen tree a0d367a9cf19c1a48cbb73295b3db2299b6c3ae7:
3263 tracked files, zero checkout mismatches. make assets-ready SOUNDTRACK=gamecube,
make test (291 C++/171 Python) and normal make all passed. Initial host compilation
caught a test referencing private fields; corrected to public arrival APIs before
the passing run. Logs: artifacts/bero-register-assets.log,
bero-register-test-final.log,bero-register-build.log,bero-register-build-final.log;
audit: artifacts/bero-register-source-audit.json. All nine committed code paths
match this tested tree; concurrent merged HEAD is not claimed fully validated.
Normal binary artifacts/bero-register-normal.3dsx SHA256
8f367abd1bf12bf8549cf135eac0496cf25592e71df05f75305475b1478712e5.

Isolated native round trips passed both saved-room assertions per run; profiles
and fixture copies only, no user saves/profile or production bypass. Inspected
captures retain original art and Mario/Goombella in view. Initial current run
artifacts/azahar-bero-register-steady reported31/23 App FPS and32.44/43.03ms;
sequential previous-binary comparison artifacts/azahar-bero-register-baseline
also slowed to45/41 FPS and22.13/24.12ms. Host load was elevated. Current-binary
repeat artifacts/azahar-bero-register-repeat exited0 and reached60 App FPS at
100% speed in both settled captures,10.13/10.84ms. The comparison demonstrates
variable host conditions; it is not a controlled hardware regression benchmark.
Original pixel/entry timing and actual New2DSXL performance remain unverified.

Next bind real shared case-pool allocation/BeroID/priority/caller state and
original position/party services before reconstructing complete house05 local
banks. Continue generic door open/close/material1800/key49, original NPC events
and additional area packs. Rendering/hardware fidelity and the full remake
remain active and incomplete.

### 2026-10-14 — Actual case records and retained Trouble installer

Commit4e91b0de adds reusable FieldCasePool:64 contiguous160-byte records, original
first-free allocation, type8000→flag4, saved names/argument/callback/script/
priority and all16 caller local words. Deletion/reinit clear active1 only;
explicit mode0 event deletion checks/retires the real event first. Full pool,
invalid indices and names>=64 hold as native admission policies instead of
reproducing original unchecked overrun/strcpy. Typed records do not certify
arbitrary original RAM/global-wp alias ordering.

Registration retains captured args, value reads0/1/2, raw evtGetNumber arg3,
value arg4, saved locals before index publication and fresh raw arg5 afterward.
Ordinary priority0 and Bero priority20 remain distinct. Actual second dispatch
pass checks real IDs, preserves live-event suppression, creates source roots,
publishes case index/ID/locals and calls NPC stop only for priority20. Missing
stop/free-event providers retain the pending mark. New bodies execute only on a
later manager visit. Case-created root IDs are tracked independently of active
records, so record deletion/reuse cannot hide a surviving event.

TroubleBoardEvent now owns original evt_run_case_evt internally. The original
retained Installer actually registers board/counter cases and their exact VM
snapshots. Tests trigger those installed scripts, suppress duplicate starts and
reenter after completion while retaining saved state/independent descendants.
This pool currently belongs to that runtime; Game's existing direct contact/root
adapter is not yet migrated. A standalone Installer constructor proves its own
zero locals only, not the original full gor_02/house05 incoming16-word bank.
Full shared field allocation order, mark pass, BeroID/party/door events stay open.
See [native scope](behavior/field/NATIVE_CASE_POOL_AND_TROUBLE_INSTALLER.md).

Validation: private frozen tree b919e5618f65529a46869df2d7f4be121b50b391,
3272 tracked files, zero mismatches; make assets-ready SOUNDTRACK=gamecube,
make test (293 C++/171 Python) and normal make all passed. Initial run also
passed; final rerun includes independent root-ID tracking. Logs:
artifacts/field-case-assets.log,field-case-test.log,field-case-build.log,
field-case-test-final.log,field-case-build-final.log; source audit:
artifacts/field-case-source-audit.json. Validation excludes concurrent live edits;
committed owned source matches the tested snapshot, with only the owned CMake
registration included. New source/header names are automatically found by the
native Makefile; no compiled/extracted resources were committed.

Normal archive artifacts/field-case-normal.3dsx SHA256
45e31e314ac58840b0674590a0debdec279b008bdb2dd5d68ae5412338cf0e55.
Isolated20s fresh-save smoke artifacts/azahar-field-case-normal exited0;
inspected boot.png shows original storybook/US text,100% speed,60 App FPS and
3.85ms emulator frame time. This validates boot only, not native in-world case
dispatch, full rendering fidelity or actual New2DSXL hardware performance. User
emulator profiles/saves remain untouched.

Next: bind a shared field case/EVT/area lifetime and actual source mark-pass
providers, then actual BeroID/switch callers. Recover/bind ordinary initial_pos
start-coordinate float locals and Mario/party/camera effects before constructing
real house05 case snapshots; do not substitute final arrival coordinates or
invent missing locals. Continue generic open/close/material1800/key49, original
NPC events and additional area packs. Goal and full remake remain incomplete.

Case/placement documentation follow-through2026-10-14: source agent delivered
[complete Bero initial-position contract](behavior/overworld/BERO_INITIAL_POSITION_LOCAL_PUBLICATION_AND_ACTOR_WRITES.md).
It distinguishes start P encoded intoLW10..12 from final F passed to recovery,
pins ordered Mario/party/camera effects and save-loadLW13/14 pointers. Corrected
older placement/bottomless notes: three duplicated XYZ inputs,mode==1 adds180,
explicit final-coordinate reloads and recoveryY+37 (GPRr0 is separate fromFPRf0).
No new code/binary or measured original trace accompanies these source findings.
Next bind those real providers and outer incoming-bank provenance; full field
case/door gameplay remains unfinished.

### 2026-10-14 — Inner pose state, facing and actual cache submission

Implemented ordinary unlinked _animPoseDrawMtx8003bc68 prefix with captured
resource/descriptor/current/next/fraction, source mode/pass admission,
mapTestXLU material predicates and fresh independent map reads, effective GX
pass versus original traversal pass, hook/built-in state publication, ordinary
angle rotation/uniform scale, held late Main, modeling-base Copy and context,
two fresh cache flushes, vertex-cache invalidation and real traversal request.
Held Main admission cannot disappear when it publishes P3c before completion;
unbound shared-ring/flag8 facing paths remain explicit holds.

Recovered and bound the real upstream lifecycle: battle_init calls
animPoseBattleInit, superseding cold work mode0 with1 before stage setup. Source
seq_battleMain calls bmapLoad(bti_01,stg_00_1) before BattleInit; native stage setup
replays its cleared selected bank1 material projection, with no authored trio
blend mutation or success fallback for unbound EVT commands. Native evaluated
position/normal arrays retain original Df0/Df8 counts and feed actual
GSPGPU_FlushDataCache calls; each call resolves fresh owners. Official Citro3D
GPUREG_VTX_FUNC=1 command clears the post-vertex cache within the active frame.
No geometry is fabricated or inferred empty; native queue now reaches real
dispProc8003c4bc. See
[inner prefix contract](behavior/display/NATIVE_INNER_POSE_DRAW_STATE_AND_CACHE.md).

Validation uses frozen `artifacts/pose-inner-source/` from0d8836ed plus only
owned edits, excluding concurrent shared code. Initial suite exposed stale Bero
pack dependencies and the incorrect cold-mode assumption; both were corrected.
The snapshot's copied RomFS was refreshed with its baseline Bero converter.
The live Bero pack was also regenerated using its live converter (ASSETS records
this); no original/generated asset is committed. Final `CMAKE_BUILD_PARALLEL_LEVEL=4 make -C artifacts/pose-inner-source test`
exited0:293/293 C++ and171/171 Python tests, including held Main after cache
publication, unsupported shared-ring hold, pinned GX/matrices, fresh map branch2,
hook work-pass preservation, original traversal pass and two flush holds.
`TMPDIR="$PWD/artifacts/native-stage-map-tmp" make -C artifacts/pose-inner-source all -o assets-ready BUILD=build-pose-inner-normal TARGET=ttyd-pose-inner-normal -j4`
exited0 after review corrections. Logs: `pose-inner-test-verified.log` and
`pose-inner-build-normal-verified.log` under artifacts. Normal3DSX SHA256
`b51e544f708c14e6d9a27b626249c287666d96a70a281514c5150792873453fd`.
`artifacts/pose-inner-validation-manifest.json` records exact source/private-pack
hashes; harness changes are restored and excluded from source history.

Isolated180s native ending harness records pending8003c4bc,camera4,queue21,
callback80101774,class1 at tick3103. Preceding Item completes. This first capture
predates the held-Main/ring/shift review corrections; those are exercised by
updated host tests and final rebuild. Final120s capture at
`artifacts/azahar-pose-inner-verified` again records pending8003c4bc,camera4,21
entries,source callback80101774 at tick3103. Final normal45s fresh-save capture
`artifacts/azahar-pose-inner-normal/boot.png` shows original storybook and US
message at60AppFPS; it proves boot only. The ending remains behind its black
fade/development restoration notice, not a completed field return or geometry
parity demonstration. Both final smoke commands exited0. Harness uses deterministic
physical/menu inputs and an explicit fixture Bero bit1 clear, no combat writes,
no fake geometry/cache success and no user profile/save modifications.

Next import original raw group/shape/draw/fan metadata alongside the existing
pose banks (PMANIM retains outputs but drops source shape IDs/stream order),
then bind actual dispProc group pushes, source current-camera shape entry,
material/texture/light requests and indexed fan streams using existing contracts.
Only after genuine traversal may source history publish and the queue complete.
Linked/pool/special facing, map blend/global lifetimes, SDK/trig hardware parity,
active icons and field return remain unfinished. Full prologue goal stays active;
EVT-01 remains OPEN.

### 2026-10-14 — Original normal placement and quantized native road starts

Commit54719a84 replaces the catalog-backed native cardinal-only calculation with
resolved original bero_get_position_normal semantics: three source integer XYZ
words, sentinel named center, unwrapped source heading/mode1, fixed30 start,
source30+length final displacement, fresh trig pairs and fused operations. Actual
staged destination geometry supplies the indexed NULL-filter raw floor ray;
miss retains centerY, unknown closer owners hold. Source recovery calculation
retains finalY+37. Typed start/end/recovery views do not acknowledge original
Mario recovery RAM/motion25/party/camera services as completed.

Real beginRoadArrival now uses the source event-float quantized starting P,
retaining exact LW10..12 words separately from unquantized final F. Finite
conversion admission holds invalid values instead of saturation; checkpoint
restore clears unsaved publication provenance. The existing native movement/
follower/camera bridge remains scoped. Standard math/FMA and float-converted
static geometry are not certified bit-identical to original MSL/SDK/FPSCR.
Work runs at transitions with the existing spatial index; no per-frame geometry
upload/rewrite is introduced. Future kind0 hierarchy imports are rejected until
a real leaf-center traversal provider exists; all8 current original ROAD nodes
are verified leaves. No art/audio/pack payload changed.

Documentation source audit now proves ordinary root creation via relSetEvtAddr
and seq_mapChangeMain→evtEntry(script,0,0). Separate NPC init roots do not copy
locals back. Ordinary house05 LW9/13/14 are source-proven zero; other words retain
actual Bero/outer-init producer dependencies. This resolves earlier provenance
uncertainty without fabricating runtime positions, case IDs or completed world
callbacks. See [source closure](behavior/field/HOUSE05_OUTER_INIT_BANK_CLOSURE_AND_ROAD_LEAF_CENTERS.md)
and [native scope](behavior/field/NATIVE_BERO_POSITION_AND_START_PUBLICATION.md).

Validation: frozen tree a8faa8c459a38351e80646ccd66a7a7e0d57f470,
3284 tracked files, zero mismatches (artifacts/bero-position-source-audit.json).
make assets-ready SOUNDTRACK=gamecube,make test (295 C++/172 Python) and normal
make all passed. Initial builds failed because an uncommitted concurrent camera
include was accidentally captured without its header; rebuilt the private Game
file from base plus only owned finishWarp changes, preserving live foreign work.
Passing logs: artifacts/bero-position-assets-final.log,
bero-position-test-final.log,bero-position-build-final.log. Tests include actual
Game publication/checkpoint invalidation/authored-length data flow, source
mode/sign/zero-distance/miss/hold/float encoding and original8-leaf admission.
Only owned CMake registration is in the frozen source; broader concurrent HEAD
is not claimed fully validated. Normal archive artifacts/bero-position-normal.3dsx
SHA25675da49b7e7ec4ce71019330c32e6307bfa57cbc8a4505bde9fd11641ad6b99ac.

Isolated native artifacts/azahar-bero-position-steady exited0 after a plaza→east
→plaza round trip; both fixture-copy saved-room assertions passed. Inspected
settled captures show original art,Mario/Goombella in view,100% speed,60 App FPS
and10.03/10.90ms emulator frame times. This does not prove every intermediate
float value, original pixel/timing equivalence or actual New2DSXL performance.
User emulator profile/saves were not changed and no production hook was added.

Next bind real ordered Mario/party position callbacks and camera road/shift
resets, then publish complete house05 saved locals through actual shared field
EVT/case ownership. Continue generic door open/close/material1800/key49, original
NPC services and additional area packs. Full game and goal remain incomplete.

### 2026-10-14 — Live actor position callbacks and entry camera reset

Commit3bd2bc4f binds portable original evt_mario_set_pos/evt_party_set_pos and
evt_cam_road_reset with captured args/actor owners and original fresh raw reads,
float resolution/immediate stores. Raw party selector0 reads current index;
all nonzero words selectslot1. Actual NULL returns2 before coordinate reads.
Camera callback writes raw follow/rootY+.01, reloads look, copies fresh Mario
retained/collision roots, then resets road before shift. Caller locals/flags,
heading, floor/pose and unrelated camera state are not silently reset.

Actual beginRoadArrival now applies those callbacks using getEventFloat over
the encoded starting locals. Native owners are actual progress/currentroot,
source-sized party records, fieldCamera.point/CollisionTarget and Mario retained/
collision targets. Road work and stale candidate cache reset; external road
assets persist. Shift reset preserves previousMilliseconds. View/target blends,
frame/lens, flags, global clocks and ground metadata remain unchanged. Per-arrival
completion prevents replay; unknown/unsupported current-party state holds this
prefix. Existing native movement remains a separate adapter. No per-frame
allocation or geometry upload is introduced; callback workspace is stack-local.

Native Mario engine*.01 storage means readback/camera values can gain rounding
from scaling (and currentY floor arithmetic), while party raw floats retain the
decoded words directly. This is explicit native representation adaptation, not
source-bit equality. Generic memory-provider tests verify exact raw store words
and call order. Full source Bero globals/recovery/motion25 writes and genuine
initial_pos EVT completion remain unbound; no full house05 scene is claimed.

Validation: frozen tree dbe1ce36ee515f6496c1fe970578cff783b8e64c,
3294 tracked files, zero mismatches (artifacts/position-events-source-audit.json).
make assets-ready SOUNDTRACK=gamecube,make test (297 C++/172 Python) and normal
make all passed. Logs: artifacts/position-events-assets.log,
position-events-test.log,position-events-build.log. Tests cover ordered raw reads,
resolver mutation/captured pointers, raw selector/null short circuit, real native
actor/camera/floor/blend/shift owners, unknown admission/nonreplay and actual
Game arrival application/checkpoint invalidation. Shared Game/header/CMake files
were frozen from committed base plus only owned additions; concurrent work is
preserved and entire merged live HEAD is not claimed validated together.

Normal archive artifacts/position-events-normal.3dsx SHA256
f6c2c0fa2fbf6e28a2048eae00a61e21dcb3b0e0f863c7cb815c9f9619c1c0e1.
Isolated artifacts/azahar-position-events-steady exited0 after plaza→east→plaza;
both fixture-copy saved-room assertions passed. Inspected settled captures show
original art/Mario/Goombella in view,100% speed,60 App FPS and9.14/9.83ms reported
emulator frame times. No user profiles/saves or production hooks changed.
Original raster/timing/SDK agreement and actual New2DSXL performance stay open.

[New original script closure](behavior/overworld/BERO_RETAINED_INITIAL_POSITION_EVENT_CLOSURE.md)
pins45 immutable commands at8030f060/278, six ROAD callbacks and no attached
script launches. Next import those original words, bind mutable six BeroS/E
words and actual recovery/motion25 stores, run the genuine retained entry event,
then feed its returned bank through shared field case/house05 initialization.
Continue generic door open/close/material1800/key49 and additional original NPCs,
objects and areas. Full goal/remake remain incomplete.

### 2026-10-14 — Original model records and retained SDK group traversal

Imported unchanged original MODEL companions for b_mario,c_pkr,c_kuri_h beside
existing pose directories; the standard asset writer and Professor cache
requirements now retain them. Native shared resources validate original table
extents, links/node spans/visibility, draw/run/index/UV ranges, cull selectors,
source-stack depth and ordered descriptor/output-count coherence. Normal arrays
remain independent from position arrays. No ISO or payload is committed.

Implemented retained joint/transform node SDK graphs, literal scale gates,
ordered six rotations/pivots/component dispatch, stack publication and final
world composition. Group traversal follows actual visibility, live node/parent
scale, separate output publication requests, mutable callback matrix aliases,
render/child/pop/sibling order and current-group sibling parentage even for an
invisible predecessor. Held requests never replay a completed push. Native
shape−1 returns are genuine source finite paths; nonempty shapes retain their
real renderProc8003c884 request. Fixed32-operation programs remove per-node
command/component allocations; ARMv6 REV word loads are confirmed in private
`artifacts/draw-traversal-native-word.dis`.

Documentation inbox work recovered exact local InvXpose/MultVec graphs, including
singular no-store behavior, reciprocal-estimate refinement, vector aliasing and
store/arithmetic order. New contracts and source dependency updates are indexed
under display; fres/FPSCR and renderer/material/GPU bindings remain open.

Validation uses frozen `artifacts/draw-traversal-source/` frombfc62a4b plus only
owned changes, excluding concurrent shared code. Private RomFS copied the prior
pose-inner pack, refreshed baseline Bero data and emitted unchanged model
companions only there. The strengthened history test initially assumed a fixed
part/current queue record; corrected it to snapshot all17 history words from
the actual borrowed callback. Source mode/pass rejections can legitimately
complete preceding parts. Final `CMAKE_BUILD_PARALLEL_LEVEL=4 make -C artifacts/draw-traversal-source test`
exited0:296/296 C++ and173/173 Python tests. Coverage includes original models,
cycles/ranges, SDK holds, maximum node program, mixed pivots/six rotations,
literal gates, invisible sibling-parent behavior, held mutable callback and full
held-part history. Log `artifacts/draw-traversal-test-release-checked.log`.
`TMPDIR="$PWD/artifacts/native-stage-map-tmp" make -C artifacts/draw-traversal-source all -o assets-ready BUILD=build-draw-traversal-normal TARGET=ttyd-draw-traversal-normal -j4`
exited0; log `artifacts/draw-traversal-build-normal.log`. Final normal3DSX SHA256
`031543d81e6f0b683e34d99c3a00ad5079f1c230ad6e19fb2c84845bf4cd73a2`.
Exact source/private-pack/binary hashes: `artifacts/draw-traversal-validation-manifest.json`.

Isolated150s ending harness exited0. Actual camera4 queue21/callback80101774
now holds at8003c884. `artifacts/azahar-draw-traversal/.../pose-traversal.txt`
records Goombella handle3,original pass2,groups22,group20,parent21,shape15,
frames2,stack3,positions128,normals308. Thus original model/node traversal and
source cache work reached an admitted nonempty shape; no geometry was falsely
acknowledged. Harness uses deterministic physical/menu input and explicit Bero
fixture bit1 clear, with no combat writes or fake draw success. Its temporary
traces/inputs are restored and excluded from source history. Final normal45s
fresh-save smoke exited0 with original storybook/US text at60AppFPS in
`artifacts/azahar-draw-traversal-normal/boot.png` (boot evidence only). Ending
capture remains black fade/development restoration notice, not field return or
raster parity. User emulator profiles/saves were untouched.

Next bind actual renderProc current-camera/view/normal matrices, defaults and
texture/light/material requests, then original indexed fans using the imported
streams. Keep admitted-part history and queue completion behind real submissions.
Global pose/resource rebinding, special matched groups/Vivian, native callback
and output-slot providers, active icons, hardware numeric equivalence and field
return remain unfinished. Full prologue goal remains active; EVT-01 stays OPEN.
