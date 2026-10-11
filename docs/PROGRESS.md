# Progress and next-session handoff

Updated 2026-10-11 (America/Boise). This is a working native development slice,
not a completed remake or fidelity-verified port.

## User decisions

Target New 2DS XL, 60 Hz gameplay/60 fps presentation target; emulator-only
validation for now. Start with an early Rogueport plaza slice and introductory
Crump encounter. Touch-centered menus retain physical button access. The user's
latest correction requires very similar original art and preservation of
original sounds/music. Earlier redrawn sprites and generated tones were removed.

## Implemented

- Reproducible devkitARM/libctru/citro3d/citro2d build, embedded RomFS `.3dsx`.
- Host-testable simulation, fixed ticks, bounded catch-up, input edge consumption,
  original-wall collision, following partner, camera, jumping and paper turns.
- Dialogue, touch and button menus, jump/hammer/guard battle harness, mushroom,
  victory, defeat/retry and return to exploration. Parameters remain provisional.
- Versioned save encoding, validation, temporary-file replacement and backup
  recovery, plus on-screen save failure reporting.
- Read-only inventory of 3,204 ISO files; exact-path extraction and provenance.
- GX tiled/paletted/CMPR texture conversion; original AnimGroup idle-pose baking;
  original Mario full-body idle/jump/hammer textures in a shared atlas.
- Original Rogueport plaza and common battle stage geometry/materials/textures.
  Plaza: 40,714 triangles, 79 batches, 60 textures; 711 near-ground wall segments.
  Stage: about 10,000 triangles, 14 material batches before visibility overrides;
  consult romfs/manifest.json for the final count.
- Original Rogueport, normal-battle and victory music decoded to PCM16 without
  resampling; NDSP streaming with original loop metadata.
- Original MusyX banks preserved and 1,247 samples indexed. Standalone sample
  decoding works; sound-event macro mapping/playback is unfinished.
- Isolated Azahar and Dolphin capture tooling, durable architecture/asset/mechanics
  notes and project instructions. No existing emulator settings/saves modified.

## Validation

`make all` builds ttyd-2ds.3dsx with devkitARM. `make test` passes the portable core
scenario suite and 16 Python codec/extraction/topology tests. Core scenarios cover
movement, collision imports, partner following, dialogue, menu input, action timing
boundaries/mashing, damage, item use, victory, defeat/retry and save recovery.

The isolated Azahar 2126.2 OpenGL/llvmpipe profile booted both screens and reported
60 fps during typical captures. It exercised touch tabs, movement, dialogue,
battle selection, five unsuccessful jump attacks, enemy damage, victory, return
to plaza and save creation. The saved result had Crump defeated and Mario at 6 HP.
Screenshots/logs are under artifacts/azahar. Capture sequence:

```sh
python3 tools/emulator_smoke.py --fresh-save --seconds 4 --input-script tests/azahar_walkthrough.py
```

Final verification repeated `make -j2 all` (up to date) and `make test` (core
suite and all 16 Python tests passed). Running
`python3 tools/emulator_smoke.py --seconds 4` without resetting the save visibly
restored Mario at 6/10 HP and the completed Crump encounter. The final battle
capture still contains an upper curtain clipping Crump and a central prop despite
the hidden-node overrides: investigate the actual node hierarchy/visibility before
claiming that these overrides resolve presentation. Plaza foreground occlusion
also remains visible in the reload capture.

The input sequence is not a pixel-asserted acceptance test. Review the captures.
Precise action/guard timing and defeat were verified in host tests; their full
emulator acceptance checklist remains below. Occasional capture/save transitions
showed lower emulator frame rates; no sustained performance benchmark or physical
hardware validation has been performed. Do not claim verified hardware 60 fps.

Dolphin successfully booted the original ISO in an isolated profile and captured
its initial health/safety screen. No original gameplay measurements were obtained.

```sh
python3 tools/emulator_smoke.py --reference-dolphin --seconds 12
```

SDL/X socket creation fails under the restricted sandbox; the isolated virtual
screen checks succeeded with sandbox escalation. No automatic review rejection
occurred. Tools write profiles/captures only beneath project artifacts.

## Immediate next work

1. Finish original sound effects: resolve event IDs through pmario.proj/pool macros
   to the indexed sdir/samp sources, including pitch/envelopes/loops; bind cursor,
   jump, hammer, damage, guard and success events. Do not substitute synthesized
   effects or arbitrarily select bank samples. Start with tools/audio_convert.py,
   local-assets/sample-directory.json and docs/ASSETS.md.
2. Capture actual US GameCube tutorial gameplay in Dolphin. Confirm available
   commands, initial stats, timing windows, damage, guard restrictions, defeat,
   music selection, original dialogue and victory sequence. Replace provisional
   BattleRules and record measured evidence in docs/MECHANICS.md.
3. Implement original character animations and direction variants. The converter
   currently applies passed-keyframe deltas but does not interpolate full original
   animations. Vertex, visibility, texture-matrix and node tracks need full support. Ensure model sizing is correct and faithful.
4. Finish battle presentation: original area backdrop, opening-curtain animation,
   audience, correct prop visibility and camera. The common stage currently uses
   explicit hidden-node overrides for the mask, closed upper curtain and inactive
   switch props; these are inferred defaults, not Dolphin-verified visibility rules.
5. Improve plaza camera/foreground occlusion, original ground/slope/trigger semantics,
   partner navigation around obstacles and collision broad-phase performance.
6. Complete Azahar checks for successful/failed actions and guards, hammer release,
   touch battle selection, healing, defeat/retry and repeated transitions.
   Extend captures with explicit checks rather than treating screenshots as assertions.
7. Profile physical New 2DS XL when available. Optimize/decimate geometry and tune
   texture budgets; add room asset unload/load before expanding story content.

## Build/tool details

- Default ISO: /home/carson/games/roms/G8ME01.iso; override with TTYD_ISO.
- Azahar: /usr/bin/azahar; Dolphin: /usr/bin/dolphin-emu.
- Runtime save: sdmc:/3ds/ttyd-2ds/progress.bin with .bak and temporary .tmp.
- Isolated test save: artifacts/azahar/data/azahar-emu/sdmc/3ds/ttyd-2ds/.
- --fresh-save resets only that generated test save; omit it to check reload.
- Source, generated/extracted assets and emulator profiles remain in the workspace.
  Original/compiled assets are ignored by .gitignore. The read-only .git placeholder
  is not a functional checkout; no commits exist. Do not delete it or claim commits.
- Build-time texture cap is 128px for maps; original source data is retained locally.
  Music keeps original sample rates/channels. Audio service failure is nonfatal.
- Room meshes remain resident and are reused through transitions; true room streaming
  is not implemented. No CIA packaging, full opening, additional chapters, or broad
  partner combat systems yet.

## Story/dialogue requirement update

User requires the original story and original US English dialogue, without
paraphrasing or invented substitute scenes. Recorded in AGENTS.md and README.md.
Removed authored NPC/Crump/Goombella dialogue from the harness; explicit
development notices now mark the missing implementation. Read-only extraction
of msg/US/gor_00.txt, gor_01.txt and global.txt with SHA256 provenance is in
local-assets/messages. Next: parse message IDs/control tokens, connect the
original intro event sequence and branches, and compare playback with Dolphin.
Do not treat merely inserting original lines into the fabricated plaza encounter
as faithful story reproduction. Original message playback is still unfinished.

Validation for this update: `make test` passed the core suite and 16 Python
tests; `make -j2 all` rebuilt the native .3dsx successfully. Emulator dialogue
playback was not rechecked in this update.

## Overworld continuation: original ground and partner traversal

Implemented original near-horizontal collision triangle export as map.ground,
packaged in RomFS. Portable groundAt performs barycentric height queries with
step-height constraints so ramps update actor elevation, missing floors block
movement and roofs cannot pull actors upward. Actor and shadow rendering use
player/partner elevation. Goombella follows a bounded path of Mario's travelled
positions instead of cutting directly toward Mario through obstacles. This is
still provisional: jump traversal, stairs, falling, dynamic geometry and partner
recovery need further work. Walls are still flattened and fixed plaza bounds
remain; collision broad-phase acceleration is not implemented.

Fixed restore when called with the game's own Progress reference. Added host
coverage for slope heights, inaccessible roofs, missing floor, step constraints
and self-reference restore. `make test` passed core scenarios and 16 Python
tests; `make -j2 all` rebuilt the embedded original asset pack and .3dsx.

Read-only extraction of rel/gor.rel and provenance is in local-assets. Indexed
39 original map/bero string occurrences in rogueport-event-strings.json. This
is evidence to resolve original exits, not a complete warp table. There are no
overworld room warps yet. Next concrete task: resolve gor_01 exit/event records
(including REL relocations), original node trigger positions and destination
arrival IDs; add native room asset loading and transactional room transitions,
then test reciprocal warps and save compatibility. Avoid inventing destinations
or treating original string occurrences alone as a verified connection graph.

The workspace now has a functioning git checkout (git log shows bf9fea8). Older
statements about a nonfunctional placeholder describe the earlier environment.

Azahar verification: `python3 tools/emulator_smoke.py --seconds 4` booted
the updated build and restored the existing 6 HP completed-encounter save.
Capture reviewed at artifacts/azahar/boot.png; foreground wall occlusion
remains. This boot check does not verify movement, slopes or room transitions.

## Overworld continuation: reciprocal Rogueport roads

Original roads now connect gor_01 plaza to gor_00 harbor, gor_02 east side
and gor_03 west side, in both directions. The asset pipeline parses original
REL self-relocations and BeroEntry records (0x3c bytes), matches contiguous road
tables to original map marker names, and rejects ambiguous/nonreciprocal links.
Geometry markers supply original AUTO-position centers. Native spawn inset .9,
trigger radius .7 and cooldown 45 ticks are provisional adaptation values.
Evidence and original record offsets are in local-assets/room-links.json and
romfs/manifest.json. Original reference layout was inspected at
https://github.com/jdaster64/ttyd-tot/blob/main/ttyd-tools/rel/include/ttyd/evt_bero.h;
no code from that header is compiled or copied into the runtime.

Three additional original rooms and texture packs are converted locally.
Renderer stages a destination room before freeing the previous one, synchronizing
GPU work before disposal. Main loop stages simulation arrival, validates target
collision/floor and swaps rendering before committing progress; failed loads
restore previous collision. Catch-up/input are reset across I/O. Partner follows
after entering, and transitions save the checkpoint. Only plaza shows the
provisional NPC/Crump encounter. Interior doors, pipes, story gates and gor_04
are still unsupported; original cutscenes/door animation/fades are unfinished.

Walls now retain vertical min/max extents (count high bit marks six-float wall
records); elevated harbor platforms are no longer blocked by lower walls.
Save version 2 keeps 32 bytes, using byte26 for room and signed byte27 for
elevation quantized to .1. Version 1 migrates to plaza/elevation0. Structural
decoding is independent of the currently loaded room; loading validates original
target collision/ground. Tests cover legacy decoding, elevation and invalid IDs.

Validation: make test passed core scenarios and 16 Python tests;
`build-host/core_tests --rooms` validated all six arrivals with original room
collision/ground, HP preservation, checkpoint encoding and cooldown. Azahar
asserted saved destination IDs on east/plaza round trip and two harbor/plaza
and two west/plaza round trips. Captures under artifacts/azahar were reviewed.
Room I/O produces transient frame drops; no hardware benchmark. A rendering
regression appears after streaming: some bottom-screen panels become black,
while save feedback remains visible. Additional C2D state reset before the bottom
screen is under verification; do not claim the rendering is fully accepted.

Reproduce isolated road tests:
```sh
python3 tools/road_fixtures.py
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/east-road.bin --seconds 4 --input-script tests/azahar_roads.py
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/harbor-road.bin --seconds 4 --input-script tests/azahar_roads.py
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/west-road.bin --seconds 4 --input-script tests/azahar_roads.py
```

Next: verify/fix Citro2D state after room streaming; expand reciprocal warp
testing to failed loads and restart in each destination. Improve camera Z and
foreground occlusion per original camera/event data. Add collision spatial
indexing and preserve distinct vertical segments sharing X/Z. Resolve original
interior/pipe triggers, conditions, door visibility and animation before expanding
connections. Original dialogue/story playback, sound events, full animation and
hardware validation remain required for the full playable remake.

Final streaming verification: resetting C2D state again before the bottom-screen
scene fixed the black-panel regression. Repeated the east-side test on the final
build (two round trips, saved-room assertions passed); reviewed
road-east-return-1.png with the full status/menu visible at reported 60 fps.
The earlier harbor/west runs predated this C2D-only fix. `make test`,
`build-host/core_tests --rooms` and `git diff --check` all passed again.
`make -j2 all` rebuilt successfully. The next agent should verify harbor/west
bottom-screen presentation and destination restart persistence, then camera and
foreground occlusion. No original story dialogue has been restored yet.

## Overworld continuation: framing, indexed collision and destination reload

Cameras now follow X and Z plus smoothed floor height. The fixed horizontal
clamp and fixed depth target were removed: restored/warped rooms initialize
the camera around the arrival, and exploration follows movement throughout
room depth. Field eye offset (0,4.8,6.5), target (0,1,-1.4), smoothing .07
are adaptation values, not measured original camera rails. Harbor arrival
now centers Mario instead of leaving him at the distant top edge. Foreground
geometry still needs occlusion handling; this does not solve the original
scripted camera system or all visibility problems.

Walls and floor triangles now build a two-unit spatial grid per successful
load. Runtime queries test nearby candidates instead of the entire room.
Coordinates/reference elevations are validated before cell lookup, oversized
indexes are rejected and failed indexing keeps the previously loaded data.
Empty imported wall sets are distinguished from the synthetic host fallback.
Converter deduplication now preserves distinct wall heights sharing X/Z,
fixing accidental replacement of lower/upper walls. Python regression coverage
checks stacked walls. Exported wall/surface counts: plaza 1240/1010, harbor
748/463, east 1555/878, west 1509/1037.

The touchscreen map now scales each room's original collision footprint into
its panel, filters walls to Mario's elevation, and displays Mario/Goombella
markers. It no longer assumes plaza bounds or shows Crump in other rooms.
Field labels use readable area names instead of internal pack filenames.

Verification:
- make test: portable core scenarios and 17 Python tests passed.
- build-host/core_tests --rooms: all six original-data arrivals passed;
  14,652 indexed collision/floor probes across four rooms matched exhaustive
  scans at multiple elevations, including cell boundaries.
- make -j2 all: original packs and native .3dsx rebuilt successfully.
- Harbor road sequence completed two round trips on the new camera/indexed
  build, with saved-room assertions. Reviewed harbor arrival capture: centered
  Mario, complete touch menu, reported 60 fps.
- West destination persistence sequence asserted room0 -> room3, HP7, exited;
  a second emulator launch without fixture/reset asserted room3 -> room0, HP7.
  This verifies destination restore through an actual usable return warp.
  Touch map capture reviewed in west room: original footprint and markers fit
  its panel. Bottom-screen menu remains visible after streaming.

Persistence reproduction (isolated test profile only):
```sh
python3 tools/road_fixtures.py
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/west-road.bin --seconds 4 --input-script tests/azahar_persistence.py
python3 tools/emulator_smoke.py --seconds 4 --input-script tests/azahar_persistence.py
```

Next concrete work: foreground occlusion / original scene visibility and
original camera/event data; destination restart checks for harbor elevation
and east room, failed-load recovery and broader room traversal. Resolve gor_04
and original interior/pipe triggers and story conditions before enabling them.
Native door/pipe animations, original dialogue/event sequencing, character
animation, SFX and hardware profiling remain incomplete. No full-remake or
hardware 60 fps acceptance claim is made.

## Character proportion correction

User reported characters very large relative to original maps. Applied one
shared .5 visual scale to character billboard height/width and matching shadow
footprints in field and battle. Map geometry, camera framing, source texture
resolution, actor positions and collision are unchanged. Relative character
proportions are preserved. This is an explicit visual calibration in response
to feedback, not a measured original actor-instance scale. Original AnimGroup
pose bounds alone do not prove game-instance sizing; compare the actual US
GameCube actors/doorways in Dolphin before claiming exact fidelity.

Validation: `make -j2 all`, `make test` (core + 17 Python tests), and
`git diff --check` passed. `python3 tools/emulator_smoke.py --seconds 4`
booted; reviewed artifacts/azahar/boot.png showing Mario smaller beside the
original plaza building/doorway. Battle sizing uses the same factor but was
not separately captured in this update.

## Overworld continuation: atomic collision rollback and saved-room recovery

Room collision now stages through an RAII CollisionTransition. Previous walls,
vertical intervals, floors, spatial indexes and map bounds stay in memory until
destination collision, arrival and rendering have all succeeded. Failed staging
or rejected arrival/rendering restores the old data without filesystem access.
Native warp failure no longer attempts an unchecked disk reload of the source
room. Startup save restoration also stages collision, rejects unavailable
positions coherently, initializes the default plaza camera/floor and shows an
explicit notice. Renderer synchronizes GPU work before disposing partial loads
on failure as well as before replacing a successfully loaded room.

Host tests cover truncated destination ground, previous room files removed
before rollback, failed commit, rejected staged arrival/graphics analogue, nested
rollback, bounds preservation, spatial-query restoration and successful commit.
`make test` passed core scenarios and 17 Python tests.
`build-host/core_tests --rooms` passed all six arrivals and 14,652 indexed-vs-full
scan probes. `make -j2 all` rebuilt the .3dsx; `git diff --check` passed.

Azahar persistence runs confirmed plaza -> harbor and restart harbor -> plaza
with HP7 preserved, exercising the elevated arrival. East-side persistence
confirmed plaza -> east with HP7; restart verification follows. Existing west
restart evidence remains in the preceding entry. No physical hardware tested.

Invalid-room fixture uses a structurally valid room1 save at (50,50), outside
its floor. Azahar displayed the fallback notice in a coherent plaza scene;
the script then moved Mario and saved, asserting room0, default HP10 and
expected plaza coordinates. Capture invalid-room-fallback.png was reviewed.
This verifies invalid-arrival recovery; it is not a native injected texture-load
failure test. Foreground wall occlusion is still visible in the plaza capture.

Reproduce saved-room / recovery checks using only isolated test profiles:
```sh
python3 tools/road_fixtures.py
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/harbor-road.bin --seconds 4 --input-script tests/azahar_persistence.py
python3 tools/emulator_smoke.py --seconds 4 --input-script tests/azahar_persistence.py
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/east-road.bin --seconds 4 --input-script tests/azahar_persistence.py
python3 tools/emulator_smoke.py --seconds 4 --input-script tests/azahar_persistence.py
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/invalid-room-road.bin --seconds 4 --input-script tests/azahar_invalid_room.py
```

Next concrete task: foreground wall/prop occlusion and original visibility
rules, because Mario can still be obscured at normal plaza positions. Then
original door/pipe event triggers and room conditions (gor_04 and interiors),
original English story/dialogue, character animation and original SFX. Preserve
the shared .5 character correction from user feedback; original actor sizing
still needs Dolphin measurement. Successful outdoor warps are not proof of
full story, world rendering or gameplay acceptance. Goal remains active.

Final persistence result: restarted east room -> plaza with HP7 preserved.
Both harbor and east restart tests completed on this transaction build. The
final native rebuild includes GPU synchronization before partial-load cleanup;
normal east persistence succeeded afterward. No GPU failure was deliberately
injected. All reported source changes and reproducible fixture scripts are
recorded in the incremental commit for this update.
