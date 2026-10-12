# Progress and next-session handoff

Updated 2026-10-12 (America/Boise). This is a working native development slice,
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

## Overworld continuation: exterior-wall cutaways

Implemented rendering cutaways for original exterior-wall S_gai_kabe scene
subtrees. Walls keep their original geometry/materials/textures; the converter
separates wall owners into batches, including inherited nested drawable pieces.
Runtime builds bounding boxes, probes camera-to-Mario foot/head and Goombella
body segments against original triangles, and skips only obstructing wall
groups. Eight simulation ticks of hold reduce toggling when a probe leaves an
edge. Ground, buildings, other props and collision are not removed by this
mechanism. Battle uses its normal room rendering. This is a handheld visibility
adaptation, not a measured reconstruction of original event-controlled cutaways.
Some caps or decorative pieces in separate original nodes may still require
original visibility rules; interior roofs/props are outside this classification.

Final packs retain original triangle counts: plaza 40,714, harbor 22,922,
east 33,444, west 34,885. Batch counts are now 104/78/98/115, with
26/6/14/13 tagged wall groups respectively. Data is recorded in manifest.json.
New portable visibility helpers use finite, two-sided segment/triangle tests
and slab bounds tests. Core scenarios cover winding, misses, behind-camera or
beyond-actor geometry, endpoint contact, parallel/degenerate triangles and
invalid bounds/NaNs. `make test` passed core scenarios and 17 Python tests;
`build-host/core_tests --rooms` passed six arrivals and 14,652 collision probes.
`make -j2 all` rebuilt the packs and native .3dsx.

Azahar fresh plaza boot was reviewed: Mario visible at the default position
previously obscured by the front wall, while nonblocking walls remain. Field
visibility sequence captured start, front movement, jump, left and rear movement;
reviewed front capture with Mario and partner visible and reported 60 fps.
Those movement captures are manual visual evidence, not pixel assertions.
The full existing encounter harness sequence reached victory at HP6 and returned
to the plaza; reviewed battle-select and battle-turn-5 captures at the corrected
shared character scale. West road script completed two reciprocal round trips,
asserting saved room IDs and exercising cutaway metadata after room streaming.
No hardware benchmark or original-story/mechanics acceptance claim.

Removed the leftover authored victory narrative line (plaza safe) in favor of
an explicit development-encounter notice. Partner display is a neutral status
label. Battle/victory/defeat now consistently display the battle-stage label.
Original English dialogue playback and story sequencing remain unfinished.

Reproduce:
```sh
python3 tools/emulator_smoke.py --fresh-save --seconds 4 --input-script tests/azahar_visibility.py
python3 tools/emulator_smoke.py --fresh-save --seconds 4 --input-script tests/azahar_walkthrough.py
python3 tools/road_fixtures.py
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/west-road.bin --seconds 4 --input-script tests/azahar_roads.py
```

Next: reproduce original map initialization visibility from REL event records
(including apparent helper/trigger geometry now visible near front openings),
original camera rails, and doors/pipes with their event conditions. Map helper
geometry should not be broadly hidden based only on appearance. Resolve gor_04
and interior links from original records; retain atomic room acceptance.
Original character animations, SFX, English dialogue/story, battle observations
and physical hardware acceptance are still required. The full goal remains active.

## Overworld continuation: original station road

Packaged original gor_04 station geometry/textures/collision and enabled the
original reciprocal west n_bero / station s_bero road. Five rooms now have eight
directed roads. Global distinct-table/reciprocity matching replaces ambiguous
per-room marker-subset selection: station contains an extra north marker, which
must not be confused with harbor's table. Station event/train travel is not
implemented. Preserved room IDs 0–3; new ID4 supports save v2 with registry-based
validation and a host save roundtrip test. Invalid ID5 remains rejected.

Found and corrected marker-midpoint arrival elevation: station s_bero spans
Y0–2, so its midpoint caused ground validation failure. Arrival uses original
marker base height; all eight destinations now pass ground/collision acceptance.
`make test`: core suite and 17 Python tests passed. `make -j2 all` passed.
`build-host/core_tests --rooms`: 18,315 indexed/full-scan collision probes and
eight original room arrivals passed. Azahar station road script completed two
round trips with saved room IDs asserted and HP7 preserved. Reviewed
road-station-arrival-0.png: station art and Mario entrance visible, emulator
reported 60 fps. Train components/static scene show rendering/visibility issues;
this is not original event-state or hardware acceptance. Initial midpoint build
failed both host arrival and native station entry; corrected build passed.

Reproduce:
```sh
python3 tools/road_fixtures.py
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/station-road.bin --seconds 4 --input-script tests/azahar_roads.py
python3 tools/emulator_smoke.py --seconds 4 --input-script tests/azahar_roads.py
```
First script also leaves a station checkpoint for restart; second detects room4
and exercises station->west->station. Restart result is recorded below when
verified. Next: original station train/map event visibility and original camera
behavior; doors/pipes/interior connections with event conditions; original
English story/message playback, full character animation and original SFX.
The full playable-game objective remains active.

Restart verification completed: updated station script left room 4/HP 7 checkpoint;
second invocation without fixture or save reset loaded station, completed two
station->west->station round trips and exited with room 4/HP 7 saved. Both scripts
returned exit0. Physical hardware, train travel, story conditions and original
station event/visibility fidelity remain unverified.

## Overworld continuation: original mesh sidedness

Converted original per-node GX face-culling settings into batch metadata.
Original drawable nodes use back culling or no culling in the packaged rooms;
front culling remains supported by the format. Materials now split by sidedness
as well as cutaway group; unchanged triangles/collision, modestly more batches.
Metadata packs original layer in low byte, group in bits8–23 and cull index in
bits24–25. Runtime masks fields separately, rejects unknown high bits and resets
culling for paper actors/2D rendering. Null draw-mode pointers on hierarchy-only
world_root nodes are valid (initial strict pointer reading rejected them).

First direct GX/PICA enum mapping removed plaza floor; inspected Azahar capture
and reversed enum mapping for current view transform. Corrected plaza boot
shows original ground/building fronts, and the moving/jumping cutaway capture
sequence completed. Station script completed two round trips plus station save
on corrected build, asserting room IDs. Reviewed station entrance image: static
train geometry still has malformed/black components; sidedness does not fix
original animation/event state, which remains next fidelity work.

Some Xvfb screenshots contain partial black areas in bottom UI. Temporary
command-buffer probe measured 0.1992 usage, ruling out exhaustion in that plaza
case; diagnostic code was removed before final build. Fresh boot capture showed
full bottom UI. A separate isolated station run forcing Qt window resize/repaint
showed restored menu tabs/status; resize clipped the lower screen in the fixed
1000x800 capture surface. Evidence points to virtual-display repaint/capture
behavior, but no broad menu-rendering acceptance claim is made from partial
captures. Do not silently ignore future reproducible native UI failures.

`make test` passed core and 18 Python tests (batch field isolation and invalid
metadata boundaries added). `make -j2 all` passed;
`build-host/core_tests --rooms` passed all eight arrivals and 18,315 probes.
Reproduce corrected field/station checks with the existing visibility and road
commands above. Battle-stage verification result follows below.
Next: original train/map event visibility and animations, original camera rails,
door/pipe and interior event links, original English message/story playback, SFX
and measured mechanics. Hardware acceptance still unavailable; goal active.

Final battle check: existing walkthrough returned exit0, reached victory at HP6
and returned/saved plaza. Reviewed battle-select and battle-turn-5 captures: stage
floor and curtains retain visible faces, bottom command/result menu fully drawn,
reported 60fps. Goombella remains partly behind the left curtain, and stage props
are still static; original scene presentation remains incomplete. No hardware
performance or observed-original-mechanics claim. Final code contains no temporary
command-buffer probe.

## Overworld continuation: original map mesh animation extraction

Added bounds/count/finite-validated original map mesh animation reader and
Hermite/step frame sampler. Conversion writes original mesh tracks into ignored
local JSON sidecars, records inventories in manifest, and accepts explicit
--animation/--frame visual previews. Original translation/rotation offsets and
scale dividers are respected; hierarchy bind*delta transform order follows
existing format reference. Collision remains static in previews; runtime packs
remain at bind pose. Original event activation, loop modes and playback are not
invented. Material animation table offsets are inventoried, not decoded.

Evidence changes the next station task: gor_04 mesh animation table contains
shadanki and n_1_door_open/close, with remaining entries having no mesh tracks.
There is no train-body mesh track in this table; the malformed/black locomotive
needs original material/scene/event inspection rather than an assumed idle clip.
The door clip targets node tobira. Sampling frames 0/23 changes exactly 702
vertices; UV/colors, batches, collision wall and ground files remain identical.
54 finite original samples across 18 tracks in all five maps and battle stage
passed `python3 tools/verify_map_animations.py`. Duplicate harbor fune_b names
are preserved and name-based preview rejects ambiguous selection.

`make test` passed core plus 20 Python tests, including real-layout synthetic
parser fixtures, truncation/count/nonfinite/step/divisor rejection, clamp behavior
and interpolation. `build-host/core_tests --rooms` passed 18,315 collision probes
and all 8 original arrivals. Native build result recorded below on completion.
No runtime animation/emulator/hardware acceptance claim for this offline step.

Reproduce original asset validation after `make all`:
```sh
python3 tools/verify_map_animations.py
python3 tools/map_convert.py local-assets/gor_04.d /tmp/ttyd-door-open --animation n_1_door_open --frame 23
```
Next concrete tasks: export original node hierarchy/vertex bindings for runtime
mesh playback; decode material animation tracks; recover REL door/event
selection and collision/visibility rules before enabling transitions. Inspect
train material modes, source vertex colors and node transforms to explain the
remaining locomotive defects. Continue English dialogue/story, original SFX
and measured combat; full playable objective remains active.

Final `make -j2 all` completed successfully with animation extraction included in
the asset fingerprint. No runtime mesh playback has been enabled by this change.

## Overworld continuation: material tracks and original init-call evidence

Implemented original material animation decoding and explicit offline UV frame
previews, sharing static/animated texture matrix math. Supported texgen0 material
binding with original names and _v/_x aliases; higher texgens produce a limitation
record. East mizu animation contains both texgen0 and1 tracks on lambert119, so
full water fidelity still requires second-layer rendering. Runtime animation
selection/rates/loop semantics remain unfinished. No default scene animations
were guessed or automatically activated.

`make test` passed core plus 21 Python tests: original-layout material fixture,
invalid texgen rejection, independent S/T scale interpolation and matrix/finite
boundaries. `make -j2 all` passed. `build-host/core_tests --rooms` passed all 8
arrivals and 18,315 collision probes. `python3 tools/verify_map_animations.py` passed
63 material samples across 21 tracks plus 54 mesh samples. East water midpoint
changes 135 UV vertices; west neon_01 midpoint changes 2,010; geometry/colors,
collision and batches remain identical. Existing 702-vertex station door sample
also passed. No new emulator or hardware animation acceptance claim.

Added independent read-only map event inventory, using publicly available
jdaster64/ttyd-utils US symbol CSV (external, ignored). Original REL event headers
provide argc/opcode; full command order/operands/relocation provenance retained,
no script interpreter or branch evaluation. Inventoried gor init scripts: harbor
starts nami/nami_02/hiki_nami; plaza hae; east mizu/noname/ie_01_light; west mizu.
These calls do not alone prove unconditional activation. Station init has
evt_mapobj_flag_onoff at 0x6073c with operands (1,0,train_2,2), and S_bero at 0x6051c
with (1,0,S_bero,2). Determine flag semantics and enclosing conditions before using
these as rendering defaults. This is concrete evidence for the remaining train
visibility issue, rather than assuming an absent train mesh clip.

Reproduce after obtaining external public US symbol CSV:
```sh
python3 tools/map_event_inventory.py --symbols artifacts/reference/us_symbols.csv --output local-assets/map-init-events.json
python3 tools/verify_map_animations.py
python3 tools/map_convert.py local-assets/gor_02.d /tmp/ttyd-water --animation mizu --frame 499.5
```
Next: interpret original map flag/animation-call semantics and branch conditions;
export runtime node/material bindings, enable proven ambient tracks, and repair
train scene visibility. Continue original camera/door/pipe/interior events and
English story/dialogue, SFX and measured combat. Full playable objective active.

## Overworld continuation: source-derived station visibility

Added read-only DOL function inspection with external US symbols and llvm-objdump
to resolve map visibility semantics from actual original bytes. Original
evt_mapobj_flag_onoff 0x80038468: selector0 chooses object flags, selector1 group
flags; onoff0 calls Off, nonzero On. mapGrpFlagOff 0x8001ecb8 calls mapFlgOff
0x8001eee8 recursively over group descendants, leaving owner siblings alone.
mapDisp 0x80024b08 skips nodes with bit0x4000, or flag 0x1 without flag 0x80.
Bit2 gates clipping calculations at 0x80024c54; when clear, branch to0x8002510c
still reaches rendering. Therefore train_2's unconditional bit2 clear is NOT
a hide instruction. Earlier possible train-visibility interpretation is ruled
out; do not suppress train_2 based on that call.

Verified opcode identities against primary evt_cmd.h: 12 IF_STR_EQUAL,32 ELSE,
33 END_IF. Original station init branch 0x604d0 compares entername with
hikousen_return; ELSE 0x604e4 sets group hidden flag 0x1 on gondora_2/hako_1.
Current station road arrives s_bero, so this branch applies. Implemented those
original visibility defaults for the packaged ordinary-road slice. Converter
now suppresses descendants: both group owners have zero own parts, so prior
leaf-only hiding could not implement this rule. Station now 41,084 triangles
(314 fewer), 54 batches; collision and texture inventory unchanged. Remaining
locomotive appearance has not been corrected by this rule or compared with
Dolphin station footage; original camera/lighting/transforms still need evidence.

`make test` passed core plus 21 Python tests; `make -j2 all` passed.
`python3 tools/verify_map_visibility.py` passed exact 314-triangle descendant
suppression and unchanged collision/textures. `build-host/core_tests --rooms`
passed all 8 arrivals and 18,315 collision probes. Original animation verifier
still passes 63 material/54 mesh samples and door/water/neon previews.
Azahar station road result recorded below after terminal completion.

Reproduce source inspection (requires ignored public US symbol CSV):
```sh
python3 tools/inspect_dol_function.py evt_mapobj_flag_onoff --symbols artifacts/reference/us_symbols.csv --output artifacts/reference/evt-map-flags.elf
python3 tools/inspect_dol_function.py mapGrpFlagOff --symbols artifacts/reference/us_symbols.csv --output artifacts/reference/group-flags.elf
python3 tools/inspect_dol_function.py mapDisp --symbols artifacts/reference/us_symbols.csv --output artifacts/reference/map-display.elf
python3 tools/verify_map_visibility.py
```
Primary opcode reference:
https://github.com/jdaster64/ttyd-tot/blob/main/ttyd-tools/rel/include/evt_cmd.h
No original/DOL/decomp executable implementation imported into runtime.
Next: preserve runtime scene nodes and bindings, prove ambient-animation
activation/loop semantics, original camera framing, and expand door/pipe/interior
event conditions. Build-time station subset must be replaced with dynamic
visibility when implementing hikousen_return. Original story/English messages,
SFX, measured combat and hardware acceptance remain incomplete; full goal active.

Final native check: station road script returned exit0, completed two reciprocal
round trips and final station entry; saved room 4/HP 7 verified. Reviewed
road-station-arrival-0.png: Mario entrance and full bottom status menu visible,
reported 60 fps. No claim that the remaining locomotive appearance or original
station story/event behavior is accepted.

## Overworld continuation: native original east material playback

Enabled native texture-zero playback for original east init's mizu/noname/
ie_01_light clips. Verified init call offsets 0x4d694/0x4d6a8/0x4d6bc are outside
conditional blocks. Inspected original US DOL evt_map_playanim, mapPlayAnimationLv,
mapMain and marioStInit: mode1 loops full duration; clock0; default speed1;
elapsed-ms frame calculation uses framerate field initialized 60 for NTSC
(VIGetTvFormat0), 50 otherwise. Upstream viewer duration-1 loop shortcut is NOT
used. Port follows fixed60Hz simulation ticks and resets epoch on room load.
This does not implement original pause/clock behavior during broader story events.

Original Hermite/step matrices baked per integer frame with original raw-UV
bindings. New portable UVAN loader rejects invalid format/version/counts,
nonfinite matrix/UVs, out-of-range/duplicate indices and trailing bytes; output
remains unchanged on failure. Native updates 369 vertices, flushes contiguous
cache ranges and streams/disposes animation metadata with rooms. East file
41,856 bytes; durations999/159/400. All rooms/stage have mandatory valid files,
empty where no proven material-only clip is enabled. Texture-zero east water
is animated; original texgen1 water layer remains missing. No mesh clip enabled
partially: ambient exporter rejects selected clips with mesh tracks.

`make test` passed core plus 21 Python tests. Core now tests UV coordinate
transforms, full-duration loop boundary, invalid-file atomic recovery.
`build-host/core_tests --rooms` passed original all 8 arrivals, 18,315 collision
probes, all room UV metadata imports and three east track loop probes.
Original animation/visibility verifier scripts passed. Native build passed
without warnings after correcting an initial indentation warning.

Azahar existing east road script completed two reciprocal round trips with
saved room IDs asserted. New stationary ambient capture script completed
east entry, movement away from marker, three captures at 1.5 s spacing and return
to plaza. Reviewed east-ambient-180: field/status render and reported 60fps,
sim 0.17/CPU 3.9/GPU 0.5 ms displayed. Transition capture temporarily reported 20fps;
steady capture reported 60. Partial black repaint artifacts persist in some
virtual-display screenshots; no pixel assertion or Dolphin side-by-side visual
acceptance claim. Native runtime update logic/asset imports are verified, but
original complete material appearance and hardware performance remain unproven.

Reproduce:
```sh
make test
make -j2 all
build-host/core_tests --rooms
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/east-road.bin --seconds 4 --input-script tests/azahar_ambient.py
```
Next: preserve runtime scene nodes for original mesh tracks (harbor water/boat,
plaza hae, west mizu), implement second-layer material rendering, original camera
framing and event visibility. Door/pipe/interior event links, original English
story/dialogue, SFX and measured combat remain necessary. Full goal active.

## Character size correction

Reduced the shared paper character scale from 0.5 to 0.35 following the
reported oversized characters. Heights, widths, paper tilt and shadows are now
30% smaller across field and battle; original map geometry remains at its
existing scale. This is a visual calibration, not measured original actor size.

Validation: `make test` passed core and 21 Python tests; `make -j2 all` built
`ttyd-2ds.3dsx`. `python3 tools/emulator_smoke.py --fresh-save --seconds 4`
exited successfully. Reviewed `artifacts/azahar/boot.png`: smaller field actors,
full dual-screen UI, reported 60 fps. Hardware performance is unverified.
Next: compare character proportions and camera framing against Dolphin.
Existing unfinished ambient-animation edits remain in the worktree separately.

## Native plaza and harbor ambient animation; harbor retreat fix

Completed the prior worktree's MSAN mesh animation path. Plaza original hae
animates12 vertices across2 owners (399 frames). Harbor original nami animates
4,581 vertices across3 owners (979 frames), alongside nami/nami_02/hiki_nami
texture-zero material playback (4 tracks,5,451 UV vertices). Original ancestry
is retained during conversion and world delta matrices are baked at integer
frames; native mesh/UV updates share the original full-duration loop epoch.
No cumulative transform drift; mandatory empty headers packaged for unused
rooms/stage. Matrices selected once per track before bound-vertex updates.

Native harbor verification initially FAILED return-to-plaza after walking away
from the marker. Host reproduction proved Mario could change from the entrance
floor to a lower surface inside a wall, then could not retreat. Walking now
checks collision at sampled destination elevation as well as previous elevation.
Added original entrance regression:60 ticks toward harbor, each position stays
walkable, then retreat requests the plaza warp within120 ticks. Map collision
data is unchanged. This does not establish complete staircase/prop semantics;
center harbor entrance is obstructed by original posts and railing.

Verified commands:
```sh
python3 tools/verify_mesh_animation.py
make test
build-host/core_tests --rooms
make -j2 all
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/harbor-road.bin --seconds 4 --input-script tests/azahar_mesh.py
```
Mesh verifier passed five original poses per room (plaza0/1/27/155/398;
harbor0/1/155/400/978), unchanged collision and batch files. Core plus21 Python
tests passed; original room checks passed all8 arrivals and18,315 indexed vs
full-scan collision probes, actual vertex-count animation bounds, loop checks
and new harbor retreat regression. Native build passed. Final harbor script
passed entry/HP7/three stationary captures/return-to-plaza assertions after fix.
Reviewed harbor-mesh-240: full status UI, reported60fps, sim0.20/CPU5.2/GPU0.5ms.
The current entrance camera obscures the dock/waves; these captures do not prove
visual wave fidelity. Original sampled mesh math and native loading are verified.

Limits: second texture layers and nami/fune_b_lambert83 missing visible material
binding remain recorded in manifests. No conditional boat animation, animated
collision, dynamic visibility or original event/pause behavior added. Physical
hardware and full original presentation remain unverified.
Next concrete task: original harbor traversable floor/wall attributes and camera
framing, then second-layer materials. Continue original English dialogue/story
playback, SFX, measured combat, doors/pipes/interiors; full objective remains active.

Additional final native road checks passed two reciprocal round trips each for
east, west and station, with saved room assertions. Station script also entered
the station for its final checkpoint. `python3 tools/verify_map_visibility.py`
passed original road-entry314-triangle omission/collision/texture invariants.
Reproduce:
```sh
python3 tools/verify_map_visibility.py
for route in east west station; do
  python3 tools/emulator_smoke.py --fresh-save --save-fixture "artifacts/fixtures/$route-road.bin" --seconds 4 --input-script tests/azahar_roads.py
done
```

Final `python3 tools/verify_map_animations.py` passed63 material/54 mesh
finite samples,702 moved station door vertices and prior east/west UV probes.
Final `python3 tools/emulator_smoke.py --fresh-save --seconds 4 --input-script
tests/azahar_walkthrough.py` completed the development battle sequence and
return. Reviewed return-plaza.png: smaller actors/full status UI; saved32-byte
checkpoint has room0/HP6/victory flag1. This validates stage resource loading and
return flow, not original battle mechanics or story acceptance.

## Harbor staircase traversal

Investigated original harbor geometry instead of deleting collision or moving
the warp. The entrance stairs have0.1-unit risers. Existing ground sampling
accepted0.25-unit ascent but wall clearance used0.1, so an actor's projected
footprint collided with nearby risers during descent. Unified both through
`StepHeight=0.25` and retained source geometry. This is a provisional native
allowance, NOT a Dolphin-measured original rule. Partner destination elevation
now also receives collision validation, as Mario's does.

Host regression follows the original right-hand staircase, crosses onto the
solid central dock floor at y0.15, checks Mario/partner positions stay walkable
and both reach that height, then retraces stairs and requests the plaza warp.
Additional synthetic collision cases ensure0.1 risers clear,0.3 obstacles block
and overhead walls do not block floor movement. All8 road arrival tests and
18,315 indexed/full-scan collision probes pass under the new clearance.

Initial traversal probe continued into water-side surfaces at x5.6/z2.4/y-0.1;
that did NOT prove solid dock traversal. Revised accepted route stays on the
solid dock, with native saved elevation0.2 (quantized original y0.15) asserted.
Water/fall/recovery behavior remains unfinished: raw original A_mizu collision
flags are0x200 at node drawmode+8, A_mizu_kabe0x5. These values were read from
the user's m/gor_00/d; their runtime semantics have NOT been recovered. Do not
infer universal hazard semantics from names or the unrelated0x40000000 viewer
flag. hitGetAttr/hitCheckAttr and Mario motion in original DOL are next evidence
sources. Current floor packs still lack original per-node attributes.

Validation (all exited0):
```sh
make test
build-host/core_tests --rooms
make -j2 all
python3 tools/road_fixtures.py
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/harbor-arrival-road.bin --seconds 4 --input-script tests/azahar_harbor_stairs.py
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/station-road.bin --seconds 4 --input-script tests/azahar_roads.py
```
Core plus21 Python tests passed. Native harbor script asserted solid dock
position/elevation/HP7, captured three stationary frames, and returned to plaza.
Station passed two reciprocal round trips and final station checkpoint. Reviewed
final harbor-dock-240: full menu, reported60fps; sim0.23/CPU5.1/GPU0.5ms.
Boat geometry obscures Mario at this central camera position; no original-camera
or complete harbor visual acceptance claim. Hardware remains unverified.

Next concrete task: retain original collision-node attributes with geometry and
recover water/fall recovery semantics, then original harbor camera/framing and
boat visibility/second-layer materials. Original English dialogue/story events,
SFX, broader traversal/doors/pipes, and measured combat remain incomplete.

## Original floor attributes retained through native movement

Converter now retains original collision node's drawmode+8 attribute word and
data-relative source-node ID for each exported floor triangle. Tagged high-bit
count selects44-byte records (9 floats +2 uint32); untagged36-byte legacy packs
still load with zero metadata. Coordinate/normal selection and wall geometry
remain unchanged. Manifest indexes original collision-node names and flags.

Native GroundContact is selected with the same highest supported floor as
height, then carried through Mario/partner movement, save restoration and warp
arrival. Contacts are reconstructed from geometry on load, not persisted in
saves. Import/staging owns attributes with geometry/index and preserves the old
room on failure. Query misses preserve caller outputs. No guessed water action
or collision-mask meaning was introduced; wall attributes and event mutations
remain missing from runtime.

Original DOL inspection establishes `_hitEnt`80018aa8–80018ac0 initializes
hit-entry+4 from node drawmode+8 (or0 when absent), and hitGetAttr80014310
returns it. marioLandOn80092de4–80092e98 tests0x200 then enters damage motion31
under additional state conditions. Original motion table31 points to
mot_damage8013ae04. Recovery timing/position/HP/effects still require full source
trace and Dolphin observation. Original DOL bytes were only read/disassembled;
none executed or linked into native runtime.

Validation:
```sh
make test
build-host/core_tests --rooms
make -j2 all
python3 tools/verify_ground_attributes.py
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/harbor-arrival-road.bin --seconds 4 --input-script tests/azahar_harbor_stairs.py
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/station-road.bin --seconds 4 --input-script tests/azahar_roads.py
```
Core/21 Python tests passed. Legacy zero metadata, tagged floor selection,
query-miss output preservation, invalid tagged import retention, and failed
room-transition attribute rollback/commit are tested. All8 original arrivals
and18,315 collision probes pass; exhaustive floor oracle now also compares
selected attributes/source IDs. Source verifier passed all3,576 triangle
attributes across five rooms (1010/463/878/1037/188). Harbor has16 A_mizu floors
with0x200. Native harbor solid dock/save/return and station two round trips plus
final checkpoint passed with tagged packs. Original water behavior is incomplete.

Reproduce byte inspections using tools/inspect_dol_function.py, external ignored
artifacts/reference/us_symbols.csv and ignored ELF outputs, with functions
`hitGetAttr`, `hitCheckAttr`, `_hitEnt`, and `marioLandOn`. Original motion table
was inspected read-only from DOL file bytes; no external implementation reused.
Next concrete task: trace mot_damage/mot_bottomless and observe harbor water
fall/recovery, then reproduce the original sequence using retained floor flags.
Continue original camera/boat visibility, layered materials and English story
events; these remain essential unfinished work.

Final native east check also passed two reciprocal round trips with saved-room
assertions, covering east/plaza streaming with tagged floor metadata:
```sh
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/east-road.bin --seconds 4 --input-script tests/azahar_roads.py
```

## Original water-damage trace and authored return-table extraction

Traced original mot_damage, hitGetDamageReturnPos, evt_hit_damage_return_set,
hitGrpDamageReturnSet, hitDamageReturnSet, hitObjGetPos/Sub and pouchAddHP.
Detailed addresses/control-flow evidence now in docs/MECHANICS.md. Source
shows ordinary character-byte0 loses1 HP after landing/recovery counter50;
zero HP follows a different state. Full timing and observed presentation remain
unverified. Return selection uses saved hit object+0x208 or source map fallbacks;
it does not choose an arbitrary nearby point. Initial return binding sets
0x40000000 on the original hit group/descendants.

Added bounded return-table parser to the asset pipeline, using US DOL import
address and REL relocation provenance without making the external symbol table
a build dependency. Two init calls,17 ordered source entries and5 Y=-1000
automatic-position markers are retained in local ignored JSON and pack manifest.
Literal harbor return points and sentinel cases are verified against original
source. Automatic positions are NOT guessed or replaced. East A_ground_01 is
a hierarchy-only owner absent from the current geometry-owner index: retain full
hit hierarchy before propagating return bindings. Native water recovery is still
unimplemented; this step supplies its original rules and authored inputs.

Validation:
```sh
python3 tools/verify_damage_returns.py
make test
make -j2 all
build-host/core_tests --rooms
python3 tools/verify_ground_attributes.py
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/harbor-arrival-road.bin --seconds 4 --input-script tests/azahar_harbor_stairs.py
```
All exited0. Core plus25 Python tests passed, including valid literal/sentinel
tables, invalid command/missing relocation/nonfinite/truncated/unrelocated
termination rejection, and unrelated import exclusion. Native pack rebuilt and
manifest contains2 tables/17 entries. All8 road arrivals,18,315 probes, and3,576
source floor attributes passed. Final Azahar solid-dock/save/return passed after
REL helper refactoring. No new gameplay acceptance or hardware performance claim.

Next concrete task: retain original hit hierarchy/leaf centers, apply the
verified init return-binding mutations, trace saved-hit+0x208 writers and
implement water damage/recovery using that evidence. Obtain actual Dolphin
fall/recovery observations before declaring timing/animations accepted. Original
camera/boat visibility, layered rendering, English story events, SFX and broader
traversal remain incomplete; full objective active.

## Original hit hierarchy and native authored return bindings

Retained all original collision hierarchy owners/parents/part counts/transformed
hit centers, including hierarchy-only A_ground_01. Automatic return resolution
averages transformed leaf centers; ordered group overrides bind descendants
without leaking to the owner's siblings. Pack activates only the two reviewed
unconditional init calls. No guessed safe point or whole-room center substituted.

Found and corrected a prior coordinate assumption BEFORE enabling recovery:
original _mapLoad uses uniform10x model-to-engine scale, read directly from US
DOL at800294dc–80029514 and constant8041f944=10.0. Literal event positions
therefore use native map scale0.1/10=0.01; previously mechanics notes incorrectly
suggested0.1. Model-derived centers already include native scale. Harbor authored
returns now correctly(.84,.15,2.2)/(4.75,.15,-1.8), automatic A_yuka approximately
(.85,.15,2.2). Original return point geometry stays in-room.

New mandatory per-room RPOS files load atomically with collision. Harbor5
bindings92 bytes; east16 inherited bindings268 bytes; other rooms12-byte empty
headers. Ground contacts include original init return-enabled bit0x40000000
and active-room groundReturn queries retrieve authored XYZ. Snapshot rollback
includes return maps, so failed load/staging preserves previous room bindings.
Raw source flags in ground packs remain unchanged. Runtime currently uses baked
init centers; dynamic hit transforms and repeat return-table events remain open.
Water damage/animation/HP/recovery and saved-hit selection are NOT implemented.

Initial build failed from a shadowing local struct import; fixed by retaining
one module import. Initial native solid-dock test reached correct floor but
failed its strict save byte==2 assertion: original y0.15 lies on a0.1-unit
quantization midpoint, and floating interpolation can save it as0.1 or0.2.
Changed that assertion to accept both quantized values; coordinate/HP/room
assertions retained. Final native script passed. No movement rule was changed
to satisfy that test.

Validation (final commands exited0):
```sh
make test
build-host/core_tests --rooms
make -j2 all
python3 tools/verify_ground_attributes.py
python3 tools/verify_damage_returns.py
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/harbor-arrival-road.bin --seconds 4 --input-script tests/azahar_harbor_stairs.py
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/east-road.bin --seconds 4 --input-script tests/azahar_roads.py
```
Core plus27 Python tests passed. Tests cover original leaf averaging vs owner
center/sibling exclusion, ordered overrides, malformed hierarchy, RPOS version/
finite/duplicate/EOF/atomic output preservation, return-map staging failure
rollback and commit. All8 road arrivals and18,315 indexed/full-scan probes
passed, now including effective init flags. All3,576 raw floor records and21
native return bindings verified; source golden checks distinguish event/model
units. Azahar solid dock/save/return and east two reciprocal round trips passed.
Hardware and full original presentation remain unverified.

Next concrete task: trace original writers of Mario saved-hit+0x208 and remember
return-enabled floor exactly, then implement source-derived water damage/recovery
sequence with authored bindings. Dolphin fall/recovery observation remains
required for timing/animation acceptance. Original camera/boat visibility,
layered rendering, English story events, original SFX and broader traversal
remain essential unfinished work; full objective active.

## Original return-floor selection and normal trigger-floor filtering

Recovered the actual saved-hit+0x208 writer in original marioMoveMain; it
remembers current floor only if0x40000000 is set, before horizontal movement.
Added native returnContact/recoveryReturn using that rule. Unbound/water contact
does not overwrite the remembered owner, and room restoration resets it.
Source-derived normal-Mario bit4 floor filter now excludes two west trigger
floors; original geometry/attributes remain packed unchanged. No guessed
respawn, water damage, camera freeze, animation or HP rule added yet.

A host path expectation initially failed: route crosses Ship_Change02 after
A_yuka_04, so the latest eligible source return is(5.5,.05,-1.9), not the earlier
dock point. Corrected that expected original owner; implementation unchanged.
Regression proves current water floor remains unbound, remembered eligible
return persists through idle/water, query returns authored XYZ, and a new room
clears it. Synthetic filter tests cover bit4 rejection and all identified
inclusion alternatives. Source pointer/ground-filter/camera-freeze evidence
is recorded in docs/MECHANICS.md.

Validation:
```sh
make test
build-host/core_tests --rooms
make -j2 all
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/west-road.bin --seconds 4 --input-script tests/azahar_roads.py
```
All exited0. Core plus27 Python tests passed; all8 arrivals and18,315 indexed/
full-scan probes plus new original return-selection/reset scenario passed. Native
build passed. Azahar west two reciprocal round trips passed after filtering.
Native harbor stair verification is also run below. Source geometry unchanged;
no hardware or complete original-mechanics acceptance claim.

Next concrete task: implement source-derived water recovery stages with eligible
return selection, field camera/NPC freeze, source one-HP/zero-HP behavior and
original presentation. Need original map fallback when no eligible owner exists,
jump/ground contact behavior and actual Dolphin timing/animation observation.
Water remains walkable in the development slice until that state machine is
implemented. Original story/English messages, SFX, camera/boat visibility,
layered rendering and broader traversal remain incomplete; full objective active.

Final native harbor solid-dock position/elevation/HP/save and return-to-plaza
assertions also passed (exit0):
```sh
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/harbor-arrival-road.bin --seconds 4 --input-script tests/azahar_harbor_stairs.py
```

## Native water damage/recovery and checkpoint reload

Implemented portable original-derived water motion stages and integrated them
with grounded0x200 contacts. Uses remembered authored return or original ordinary
map fallback, source sink/hold/rise/travel/fall parameters, camera/follower freeze
and blocked menus/input/warps. Horizontal movement uses source speed8 and
truncated tick count, leaving less than one step of residual distance instead
of snapping exactly to the point. Source landing counter fall-through and HP
penalty boundaries are tested. One HP lost; HP0 enters FieldDefeat and offers
checkpoint load, preserving saved HP instead of healing. Native save-load stages
room collision/returns/rendering atomically; water positions cannot save or
write on START exit. Existing battle defeat harness remains separate.

Original marioJumpData four fall parameters read from US DOL(.sdata source):
-.1,-.2,.024,-.004; terminal-18 and mot_damage cap-10. Source camera/NPC calls
were traced. Implementation math is native, without executing/linking original
code. Original source counters are not claimed observed timings. Original splash/
chomp/pose/SFX/game-over assets and dynamic landing ray remain unfinished.
Current controller lands using the static supporting floor at authored target;
wall widths/flags and full original air contacts are still fidelity limitations.

Validation:
```sh
make test
build-host/core_tests --rooms
make -j2 all
python3 tools/road_fixtures.py
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/harbor-arrival-road.bin --seconds 4 --input-script tests/azahar_water.py
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/harbor-last-hp-road.bin --seconds 4 --input-script tests/azahar_water.py
```
All exited0: core plus27 Python tests; all8 room arrivals/18,315 probes; water
source-stage progression and bounded completion, landing/HP counter boundaries,
zero HP, camera/follower freeze, ignored controls, no intermediate save, fallback
and field-defeat reload-request checks. Azahar HP7 run returned near the authored
Ship_Change02 position and savedHP6. HP1 run reached game over, preserved original
checkpoint bytes, reloaded HP1/entrance position and saved without healing.
Reviewed positive water-result (copy water-positive-result.png): actor returned
near original dock, full status UI, reported60fps. Reviewed
water-checkpoint-reloaded.png: HP1 and original harbor entrance, full UI. No
hardware performance or full original water-presentation acceptance claim.

User expanded priorities now explicitly include warp fade effects and proper
trigger/arrival distances, render proportions/distances, and correct physics.
These remain active requirements. Next concrete task: source-derived road fade
and entry/exit travel distances, then correct actor collision widths and remaining
water presentation/ray behavior. Preliminary original evt_bero inventory shows
road Bero records use auto direction20000/position100000/length-1; do NOT treat
those sentinels as literal lengths. Native .7 trigger/.9 inset/45-tick cooldown
remain provisional; fade is currently absent. evt_bero_info_run_road at8030e97c,
bero_case_exec_road and evt_fade_set_mapchange_type are next evidence sources.
Full English story/dialogue, original SFX, camera/boat visibility, layered
materials and broader traversal remain incomplete; full objective active.

Final road regression after water integration passed two reciprocal east/plaza
round trips with saved-room assertions (exit0):
```sh
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/east-road.bin --seconds 4 --input-script tests/azahar_roads.py
```
Normal gravity1.0 was additionally verified in original marioMain for Rogueport;
moon-only0.5 overrides do not apply. See mechanics notes for original addresses.

## Default road fades and landing gate

Added original-default300ms black linear fade-out/in to both screens. Queued
road crossings disable controls and wait for the existing jump to land first.
Only after an opaque frame is submitted does main stage/load the next room.
Successful commit fades in the destination; rejected load restores original
room resources and fades back in. Input/menus/camera/partner movement stay
frozen during fades. START does not save a partial departure. No original
assets were changed. Your separate AGENTS.md edit remains outside this feature
commit.

Verified defaults from original US DOL marioStInit/seq_mapChangeMain/evt_fade_in
and fadeMain (addresses in mechanics notes): type9 in/type10 out,300ms, black
RGBA, linear progress. Native18 ticks corresponds NTSC60Hz. Source road-event
landing gate also recovered. This does not claim original complete transition
choreography: scripted exit/entry travel and sensor/arrival distances remain
unfinished/provisional. Rendering proportions and general physics still require
original-game comparison.

Validation (all final commands exited0):
```sh
make test
build-host/core_tests --rooms
make -j2 all
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/east-road.bin --seconds 4 --input-script tests/azahar_roads.py
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/east-road.bin --seconds 4 --input-script tests/azahar_fades.py
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/station-road.bin --seconds 4 --input-script tests/azahar_roads.py
```
Core plus27 Python tests passed; all8 arrivals/18,315 collision probes passed.
Fade tests cover monotonic out/in, endpoints, no loading before opaque-render
acknowledgment, blocked controls/saves, failed-load fade-in and airborne landing
gate. East and station native scripts each passed two reciprocal trips; station
also entered its final checkpoint. Visual fade script passed partial/black/
restored paper-fill brightness and destination-room assertions on both screens.
Final samples for both screens included238.7→159→79.7→0; final fills restored
above220. Initial probes were panel-border pixels; corrected probes to paper
fill and reran before claiming that result. Reviewed fade-02 and fade-finished:
old room dims, new room/status appears; known Xvfb repaint artifacts remain
possible, so no whole-frame/Dolphin visual-fidelity claim. Hardware unverified.

Next concrete task: implement original scripted road travel and correct initial/
arrival placement using bero_get_position_normal, evt_bero_initial_pos and
evt_mario_mov_pos2. Source30-unit offset and FLOAT150 rate/length are now traced;
DO NOT substitute them directly for all distances without the entry sequence
and floor/ray checks. Native .7 trigger/.9 inset/45-tick cooldown remain
provisional. Then recover actual actor collision widths/ground physics and
compare render proportions/camera in Dolphin. Original English story/dialogue,
SFX, layered materials, water presentation and broader traversal remain
incomplete; full expanded objective active.

## Further character size reduction

Following the latest oversized-character report, reduced the shared render
scale from 0.35 to 0.24 (31.4% smaller in each dimension). Mario, Goombella,
NPCs, battle actors, tilt and shadows use the same calibration. Map geometry
and collision dimensions are unchanged. This remains visual calibration;
original actor proportions need a matched Dolphin camera comparison.

Validation: `make test` passed core and 27 Python tests; `make -j2 all`
produced `ttyd-2ds.3dsx`. `python3 tools/emulator_smoke.py --fresh-save
--seconds 4` exited 0. Reviewed `artifacts/azahar/boot.png`: smaller actors
against plaza buildings and dual-screen rendering, with emulator reporting
60 FPS. Hardware performance is unverified.

Next: compare actor/map proportions with Dolphin under matched framing before
further scale changes. Existing uncommitted road-motion work is separate and
still needs its own documentation and emulator acceptance; do not assume this
boot check validates that work.

## Scripted road departure and arrival

Implemented recovered ordinary-road distances/rate (see MECHANICS.md):
0.3-native inside arrival endpoints,0.6-native entry travel at0.025/tick,
1.5-native outside departure endpoint and9-tick pre-fade wait. Queued travel
waits for landing and blocks input/saving. Arrival movement overlaps fade-in;
saving resumes only at the validated endpoint. Failed staging restores the
saved departure position and fades the retained room back in. Successful
transitions no longer use the provisional45-tick cooldown; movement gates them.
Party following threshold is0.5 after the source road homing value.

Validation: `make test` passed core and27 Python tests;
`build-host/core_tests --rooms` passed all8 arrivals,24-tick arrival movement,
all8 failed-load rollback scenarios and18,315 indexed/full-scan probes.
`make -j2 all` built the native binary in the character-size session with these
road changes present. Isolated east-road Azahar script passed two round trips:
`python3 tools/emulator_smoke.py --fresh-save --save-fixture
artifacts/fixtures/east-road.bin --seconds 4 --input-script tests/azahar_roads.py`.
Further native room/fade results are recorded below as completed.

Known limitations: original party choreography/camera are incomplete; scripted
Y uses fixed support height, trigger radius/body dimensions remain provisional.
Next: verify station/harbor native trips, then recover wall attributes and
original collision/sensor dimensions. Original dialogue/story, SFX, layered
materials, doors/interiors and measured battle mechanics remain outstanding.
Full objective remains active; hardware performance remains unverified.

Additional completed checks: dual-screen fade script exited0, with both paper
fill probes ranging0→238.67 and intermediate alpha values. Station and harbor
road scripts each exited0 with two reciprocal trips; station also saved its
final entry. Station's first run failed the second entry while a concurrent
native rebuild changed the embedded RomFS binary; captured UI showed graceful
room-load failure/rollback. Repeated on the finished, unchanged binary passed.
Do not rebuild the mounted binary during emulator acceptance runs.

Final host checks additionally exercise jumping via input before all8 queued
roads: no departure movement, fade or save occurs until landing. Final
`make test`, `build-host/core_tests --rooms`, and `make -j2 all` exited0.
West road script also exited0 with two reciprocal plaza/west trips. Combined
with east, harbor and station scripts, all8 directed outdoor roads now have
native repeated-transition coverage on this implementation. Fixtures used:
artifacts/fixtures/{east,harbor,station,west}-road.bin; command pattern above.

## Preserve original wall collision attributes

Implemented ordinary-Mario wall filtering from original US `marioHitCheck` /
`chkfilterVec`. Converter retains source-node IDs and raw attributes in32-byte
wall records; runtime excludes bit4 sensor walls unless original inclusion masks
apply. Overlapping walls with distinct owners retain separate records. Added
metadata to collision snapshot/rollback and legacy-pack loading with zero flags.
Truncated metadata cannot replace valid collision data. No collision radius,
body height or step-height calibration changed in this step.

Verification completed:
- `make test`: core scenarios and28 Python tests passed, including inclusion
  overrides, rejected sensors, owner deduplication, malformed data and rollback.
- `python3 tools/verify_wall_attributes.py`: all5,264 wall records match original
  node bytes;81 non-solid records across five rooms; no bit31 cases present.
- `python3 tools/verify_ground_attributes.py`: all3,576 floor records match.
- `build-host/core_tests --rooms`:18,315 indexed/exhaustive probes, all8 road
  arrivals, airborne waits and failed-load rollback passed with regenerated packs.
- `make -j2 all`: rebuilt assets and native `.3dsx` successfully.
- Native harbor stairs/dock/return passed on updated binary:
  `python3 tools/emulator_smoke.py --fresh-save --save-fixture
  artifacts/fixtures/harbor-arrival-road.bin --seconds 4 --input-script
  tests/azahar_harbor_stairs.py`. Initial test stopped mid-black fade with the
  old fixed wait; now polls completed arrival checkpoint with a6-second timeout.
  The rerun exited0 and preserved HP7.

Next: recover actual normal-Mario collision width/height semantics from
marioHitCheckWidth and wall-ray helpers before replacing provisional .28 radius,
1.2 height and .25 step threshold. Recover original sensor activation and
camera/party choreography. Wall segmentation, transformed-Mario callbacks,
runtime event collision-flag changes, original story/dialogue, SFX and broader
playability remain incomplete. Full objective active; hardware unverified.
Native water-return regression also exited0 using the same fixture and
`--input-script tests/azahar_water.py`: checkpoint HP7→6, original harbor
return destination accepted, partial-recovery save suppressed. Final host,
original-room and unchanged native build checks exited0. Native tests verify
these scenarios; they do not establish complete original physics fidelity.

## Recover normal actor collision clearance and wall rays

Read-only source trace recovered normal width20,height37,half-width10 and
wall-ray heights11/26.64 engine units. Added reproducible source verifier
`tools/verify_actor_collision.py` (constant and instruction-site checks).
Changed normal collision clearance0.28→0.1 native; replaced1.2-high interval
wall blocking with separate0.11/0.2664 ray-height samples. Grid expansion follows
that clearance. Airborne Mario now queries walls using his raised position;
floor height queries remain separate. Nominal height37 was not substituted for
the source wall-ray table. Existing visual scale0.24 remains unchanged.

Validation: `python3 tools/verify_actor_collision.py` exited0; `make test`
passed core and28 Python tests; `build-host/core_tests --rooms` passed18,315
indexed/exhaustive probes and all8 arrivals, landing gates and rollback checks.
Host tests cover radius boundaries, walls between samples, upper-ray-only walls,
low/overhead obstacles, dock-edge jump and original water return owner selection.
Water route now stops at water entry instead of silently advancing recovery
while holding movement; it explicitly jumps over the newly detected dock edge.
`make -j2 all` built native `.3dsx` successfully.

Native water script exited0:
`python3 tools/emulator_smoke.py --fresh-save --save-fixture
artifacts/fixtures/harbor-arrival-road.bin --seconds 4 --input-script
 tests/azahar_water.py`.
It jumps across the low edge, preserves partial-recovery checkpoint, returns
to original Ship_Change02 destination and saves HP6 from HP7.

Next: replace wall height interval/segment proxies with actual triangle/ray
geometry and sliding; recover floor search, jump kinematics and ordinary walk
speed. Sensor activation/camera/party choreography, original story/dialogue,
SFX, interiors and broader playability remain incomplete. Full goal active;
no hardware performance or complete original-physics fidelity claim.
Native ordinary harbor descent/dock save/return also exited0 with HP7 using
`tests/azahar_harbor_stairs.py` and the same fixture/launch options. Initial run
caught FileNotFoundError inside the save replacement window; polling now retries
that transient read until the completed room0 checkpoint or the existing bounded
timeout. Final `make test`, original-room checks, source verifier and native
build exited0. An inspection-only `artifacts/reference/marioMove.disasm` was
prepared for next-session walking/floor work; no speed conclusion is established.

## Preserve wall triangles and resolve exact height slices

Implemented portable wall-plane intersection/clearance against retained original
triangle geometry, replacing full-height projected-segment blocking for new
packs. Removes false walls in empty triangular bounding regions. New68-byte
records preserve9 source-transformed coordinates, owner attributes and legacy
summary geometry. Distinct triangles sharing a projected summary no longer
collapse. Index bounds include all vertices; geometry survives room rollback.
Malformed/truncated geometry or inconsistent Y bounds cannot replace valid data.
Older wall formats remain loadable.

Validation: `make test` passed core and28 Python tests; analytical cases cover
sloped cutoffs, height-plane edges, a vertex-only slice, above-triangle clearance,
actual loader queries, rollback and malformed bounds/truncation. Converter fixture
checks original coordinate retention. `build-host/core_tests --rooms` passed all8
arrivals/airborne gates/rollback and18,315 indexed/full-scan probes. Full scan
uses the same triangle-slice primitive (it verifies indexing, not an independent
geometric algorithm); analytical tests separately verify the primitive.
`python3 tools/verify_wall_attributes.py` passed all7,830 triangle records against
original owner flags: gor_01=1920,gor_00=1068,gor_02=2232,gor_03=2277,
gor_04=333;92 non-solid records. This verifies attributes/source IDs, not an
independent original-coordinate extraction. `make -j2 all` rebuilt the RomFS
and native binary successfully. Native harbor water jump/return script exited0
with original return destination and HP7→6; launched with harbor-arrival-road.bin,
--seconds4,--fresh-save and tests/azahar_water.py as in prior entries.

Next: recover swept direction rays, sliding/contact response, coverage of angled
surfaces, floor search and original speed/jump parameters. Near-vertical wall
selection threshold remains0.2, minimap draws summary segments, dynamic hit meshes
and transformed-Mario cases remain incomplete. Original camera/party choreography,
story/dialogue, SFX and broader playability remain outstanding. Full objective
active; hardware performance remains unverified.
Final native harbor stairs/dock-save/return test also exited0 with HP7 on the
finished build: `python3 tools/emulator_smoke.py --fresh-save --save-fixture
artifacts/fixtures/harbor-arrival-road.bin --seconds 4 --input-script
 tests/azahar_harbor_stairs.py`. No native rebuild overlapped emulator execution.

## Source-derived ordinary walking and dash distances

Recovered ordinary rates from marioInit,mot_walk,mot_dash and marioMoveMain:
walk1.0 and dash2.25 engine units/tick (native0.01/0.0225). Replaced provisional
0.065 speed. Direction normalizes independently of magnitude; walk/dash selection
uses original55-unit threshold mapped to source70-unit stick cap. Full stick now
travels1.35 native units per second rather than3.9. Road scripted speed remains
0.025 and original distance rules remain unchanged. Follower budget follows
DashSpeed, while partner-specific source movement remains unfinished.

Extended source verifier checks speed floats, initialization stores, motion loads
and55² threshold. Host tests prove threshold boundary,60-tick distance and diagonal
normalization. Preserved stair/water fixture distances by extending directional
holds; .065 remains only as a legacy fixture-distance conversion, never runtime
movement. Incline test now walks far enough at corrected speed to reach its same
height assertions. `make test` passed core/28 Python tests;
`build-host/core_tests --rooms` passed all8 arrivals, airborne gates/rollback and
18,315 collision probes; `make -j2 all` rebuilt native binary successfully;
`python3 tools/verify_actor_collision.py` exited0 with walk1,dash2.25,cap70.
Native water route/jump/return script exited0 with HP7→6 at corrected speed.
Launch: harbor-arrival-road.bin,--fresh-save,--seconds4,tests/azahar_water.py.

Next: recover original walk/dash state transition delays, source jump/gravity and
airborne drift, plus swept wall/sliding response and floor search. Current Circle
Pad dead zone and partner catch-up are provisional. Original camera/party movies,
story/English dialogue, SFX and interiors remain incomplete; full goal active.
Hardware performance and complete original-movement fidelity remain unverified.
Native harbor stairs/dock-save/return exited0 with HP7 using the same fixture and
`tests/azahar_harbor_stairs.py`. Native east-road script passed two round trips:
`python3 tools/emulator_smoke.py --fresh-save --save-fixture
artifacts/fixtures/east-road.bin --seconds 4 --input-script tests/azahar_roads.py`.
This verifies source-trigger movement reaches existing exits; original sensor
activation is still provisional. Final host/original-room checks exited0.
Inspection-only marioJump and mot_jump disassemblies were prepared under
artifacts/reference for next-session recovery; no new jump constants are claimed.

## Source-derived field jump motion and hold behavior

Replaced36-tick1.1-height sine jump with FieldJump derivative integration from
ordinary US ascent/fall tuples, gravity1, counter>=4 A-release reduction,
source apex transition and terminal fall rule. Tap/held jumps now differ;
static-support tests land at21/27 updates after begin. Same integration advances
queued road landing waits; restore/battle/water clear it. Rendering and airborne
wall sampling consume the resulting jumpHeight. Battle harness motion unchanged.

Validation: `make test` passed core and28 Python tests; tap/held four-tick
heights0.17882/0.1946,21/27-update flat-support landing, all8 road landing gates,
original water return and18,315 indexed probes passed through
`build-host/core_tests --rooms`. `make -j2 all` built native `.3dsx`.
`verify_actor_collision.py` extended to verify original jumpData, release
constant and ordinary jump dispatch directly from read-only ISO.

Next: recover original floor and ceiling search using world Y (current jump is
relative to live support), edge falling, airborne horizontal drift, motion-state
transition delays and swept wall sliding. Source-derived constants do not establish
full original physics. Original camera/party choreography, story/English dialogue,
SFX, interiors and broader playable content remain unfinished. Full goal active;
hardware performance remains unverified.
Recovered startup ordering too: mot_jump countdown2/phase0, begin counter1,
next update waits at counter2, first integration counter3. Release reduction
begins at counter4. This corrects intermediate timing estimates without startup.
Final source verifier and host/original-room/build commands exited0.

Final native water route exited0 on this startup-aware build:
`python3 tools/emulator_smoke.py --fresh-save --save-fixture
artifacts/fixtures/harbor-arrival-road.bin --seconds 4 --input-script
 tests/azahar_water.py`. Preserved partial-recovery checkpoint, original return
point and HP7→6. Initial test's separate A pulse followed by a0.3-second pause
let the shorter jump land before movement; capture showed Mario still on dock,
HP7. Test now presses A+Down together during the jump. No game timing was
extended to fit the old test. Updated tests and source evidence committed together.

## Recover landing rays and add world-Y floor query primitive

Traced marioFall→marioChkLandon→marioSearchGround. Verified velocity-dependent
center ray: worldY-abs(v)-0.0001 through worldY+abs(v)+0.05 in native units.
Recovered five-probe footprint and ordinary radius0.075, plus source update
order: query before applying fall displacement, then snap accepted floor or add
velocity when no contact exists. Source acceptance also uses slope/attributes;
do not substitute raw intersection success for the entire landing state machine.

Implemented landingGround center primitive through a shared bounded ground query;
legacy groundAt behavior preserved. Tests verify descending support detection,
rejecting a distant roof, exact stationary upper bound, missed-query output
preservation and invalid velocities. Extended read-only source verifier with ray
constants, arithmetic instruction sites and five-probe loop bound. Source
inspection artifacts checkLand/checkLand2/marioFall/landOn now exist under
artifacts/reference for continuation.

Validation: `make test` passed core and28 Python tests;
`build-host/core_tests --rooms` passed all8 arrivals/landing gates/rollback and
18,315 indexed probes; `make -j2 all` built native `.3dsx`.
`python3 tools/verify_actor_collision.py` exited0:5/0.01 engine-unit margins,
7.5-engine-unit normal footprint radius and5 probes verified against ISO.
No new emulator acceptance claim: Game has not switched to this primitive.

Next concrete implementation: preserve Mario world Y independently of support;
implement original five-probe orientation and candidate acceptance, use the ray
before fall displacement, clear lost floor contacts, and test jumping between
unequal floors and walking off edges. Then ceiling search and airborne horizontal
response. Original story/English dialogue, SFX, camera/party movies, interiors
and broader content remain incomplete. Full goal active; no complete physics or
hardware-performance claim. Source recovery and query foundation are progress;
world-height gameplay behavior remains unfinished.

## Integrate world Y, five-point landing and edge falling

Mario now preserves takeoff world height through air movement and can land on a
higher or lower floor instead of shifting the whole jump with support. Game uses
source derivative motion without base-plane clamping, queries from pre-displacement
world Y, snaps accepted contact and clears lost contact IDs. Added oriented center
plus4 footprint rays (radius0.075), generic0.11 snap-distance/highest-candidate
selection. Grounded query loss checks footprint support then begins falling instead
of blocking movement at an unsupported edge. Original remembered return contact is
retained for water recovery. Airborne saves/START writes are suppressed.

Verification: `make test` passed core/28 Python tests. New scenarios prove jump
world height across a1-unit drop, landing below takeoff, jumping onto a0.2-higher
platform only during descent, rotated footprint edge coverage, lost contact IDs,
edge fall and blocked transient saves. Floor acceptance survives horizontal overlap
instead of falling through a valid floor (horizontal resolution remains incomplete).
`build-host/core_tests --rooms` passed all8 arrivals/landing gates/rollback and
18,315 indexed probes. `make -j2 all` built native `.3dsx`; source verifier passed.
The old straight harbor ledge return assertion depended on movement blocking at
the drop: it now checks the lower dock and inability to walk straight up the1.05
height change. The separate stair route still verifies correct return to plaza.

Native water route exited0 with partial-checkpoint protection, original return
point and HP7→6. Native stair/dock-save/return repeat exited0 with HP7. An earlier
stair run was interrupted; its handle was missing and no Azahar/python process
remained, so its terminal result was not inferred from captures. Repeat command:
`python3 tools/emulator_smoke.py --fresh-save --save-fixture
artifacts/fixtures/harbor-arrival-road.bin --seconds 4 --input-script
 tests/azahar_harbor_stairs.py`.
Water uses the same fixture/options with tests/azahar_water.py. Final east-road
script exited0 with two round trips, using east-road.bin and tests/azahar_roads.py.
Native code later centralized identical floor constants; final build/host checks
are recorded below, with no behavioral constant change.

Limitations requiring next work: original slope/attribute acceptance branches and
angle mapping remain simplified; grounded support still uses legacy0.25/0.5 band;
no ceilings, no-floor void recovery, or wall push-out/sliding. Unsupported space
without a lower floor can fall indefinitely. Partner stays on legacy stepping;
shadow/camera remain at last support during a drop. Source void recovery and
ceiling search are the next concrete physics tasks before stabilization. Original
story/English dialogue, SFX, camera/party movies, interiors and broader game content
remain incomplete. Full objective active; hardware performance unverified.
Final native lethal-water/reload regression exited0 on the centralized-constant
build: `python3 tools/emulator_smoke.py --fresh-save --save-fixture
artifacts/fixtures/harbor-last-hp-road.bin --seconds 4 --input-script
 tests/azahar_water.py`. Game over preserved checkpoint; reload retained HP1
and original entrance position. Final core/28 Python tests, all original-room
checks and native build exited0. Generated ttyd-2ds.lst is now ignored; user-owned
AGENTS.md edits remain separate from this feature commit.

## Bound void falls with source-derived return and damage

Recovered marioBottomless/mot_bottomless and respawn setter functions. Native
world Y<=-20 now starts recovery instead of falling indefinitely when a validated
return exists. Return chooses valid remembered original recovery binding, then
room/load entry; HP never resets to entry HP. Camera-aware offscreen probe uses
source3-unit start/0.1 increments; horizontal motion0.08/tick; normal fall tuple
with0.1 maximum drop,50 landing ticks,HP-1 and12 injury ticks reuse shared damage
logic. Controls/saves/road requests stay blocked during recovery. Lethal falls use
existing checkpoint-preserving defeat/reload. Companion is grounded/regrouped;
actual recovery height drives renderer cutaways. Framing constants are shared
with renderer but unchanged. Original reload/BG policies and dynamic return-target
selection are not complete, nor are exact camera/partner choreography.

Tests passed: `make test` (core plus28 Python tests),
`build-host/core_tests --rooms` (all8 arrivals,18,315 indexed probes, original water
and new original-plaza void return), `make -j2 all` and
`python3 tools/verify_actor_collision.py`. New host scenarios check inclusive
threshold crossing, nonlethal/lethal returns, ignored input/partial saves, restored
position/partner and defeat reload request. Source verifier reads original-2000,
37,300,10,8 constants and50/12 counter instructions directly from ISO.

Native nonlethal void script exited0: checkpoint protected while falling, HP7→6,
return within0.08 of entry. Reviewed void-return capture; companion hover caused
by copying source lift without NPC gravity was corrected to grounded presentation.
Final lethal script exited0 after render correction: checkpoint preserved on death,
reload at HP1 without healing and original entry position. Commands:
```sh
python3 tools/road_fixtures.py
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/void-road.bin --seconds 4 --input-script tests/azahar_void.py
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/void-last-hp-road.bin --seconds 4 --input-script tests/azahar_void.py
```
Fixture is valid original plaza floor at(.35,6.95), deliberately away from active
road approach, moving down beyond floor. A temporary host scan located it; durable
fixture generator and original-room test now reproduce it without the scan.
All extracted/generated originals and captures remain ignored.

Next: source ceiling search and grounded slope/contact rules; optional original
bottomless map reload and exact fallback/partner choreography; horizontal push-out
and sliding; dynamic support. Original story/English dialogue, SFX, camera paths,
interiors and wider playable content remain incomplete. Full objective active;
hardware performance remains unverified. Missing/invalid dynamic return anchors
are not solved by this static slice implementation.
Final source/host/original-room/build checks exited0 after the rendering and
companion correction (last header edit was descriptive only). Commit selected
feature paths only; agent-instruction changes remain in the user's separate
fe63afb commit. No native build overlapped emulator execution.

## 2026-10-12 — Reduce oversized characters again

User reports characters remain very large relative to the map. Reduced shared
PaperCharacterScale from0.24 to0.16 (one-third smaller in both dimensions).
Mario, Goombella, NPCs, Crump, battle actors, tilt and shadows share the change.
This is a visual calibration, not measured original proportions. Map, camera,
collision, movement, jump heights and asset conversion are unchanged.

Validation: `make test` passed core and28 Python tests; `make -j2 all` built
`ttyd-2ds.3dsx`; `python3 tools/emulator_smoke.py --fresh-save --seconds 4`
exited0. Reviewed `artifacts/azahar/boot.png` showing smaller actors against
original plaza geometry and both screens; emulator reports60 FPS. Hardware
performance remains unverified. No new tests for this render constant change.

Next: compare actor/map proportions under matched Dolphin framing, including
door heights and atlas transparent margins. Resume source ceiling search and
grounded contact work. Original story/dialogue, sound-effect events, animation,
camera and broader playable content remain incomplete; remake is not complete.

## 2026-10-12 — Recover ordinary ceiling contact policy

Previous goal turn made verified progress: reduced render scale and committed
2c4f5df. Continued physics work by reading original head search/overhead, filter
and triangle-ray routines. Added portable head attribute/contact policy and
analytical boundary tests (collision override differences, velocity cutoff,
contact rounding, negative-height truncation). Extended original ISO verifier
with100/10 precision,1 clearance,-10 velocity cutoff and original instruction
sites. Corrected an initially mistyped verifier opcode against original bytes;
final verifier passes. Detailed source locations and missing prerequisites are
in docs/MECHANICS.md. Runtime still lacks ceilings: helper is not connected yet.

Validation: `make test` passed core and28 Python tests; `make -j2 all` exited0
(existing native output up to date; new header used only by host tests).
`python3 tools/verify_actor_collision.py` exited0 against read-only original ISO.
No runtime/emulator changes or new emulator acceptance claim this step.

Next concrete task: extend collision pack with source drawmode byte1 and verify
original normal orientation (hitCheckVecFilter, hitEntrySub800186f4,
_hitEnt8001896c). Converter already reads drawmode for rendering but drops it
from collision metadata. Build upward-ray query over appropriate triangles,
five-probe head selection, then integrate post-jump bump/fall with takeoff clamp.
Verify low ceiling, sloped ceiling, one-sided platform underside and original
room jump in host/Azahar. Do not reuse floor attribute filter for head rays.
Grounded slopes, wall sliding, original story/dialogue/SFX/animations and camera
fidelity remain unfinished. Full goal active; no hardware acceptance.

## 2026-10-12 — Integrate source-derived static ceiling collisions

Previous goal turn made progress with source head-policy helpers (e5172f6).
This step adds complete per-room HEAD mesh/index with raw source cull metadata
and source collision strip order. Closer packed/unpacked hitCalcVtxPosition
trace corrected the initial assumption that render/collision winding match:
source swaps the even strip pair in the opposite direction to GX rendering.
Conversion tests and original instruction verifier now lock down this difference.
Runtime performs five rotated upward probes after jump integration, uses original
head-specific attribute filter, source face rejection and verified rounded
contact/clearance, then restarts normal falling at clamped root. New head mesh
loads with room resources, participates in rollback and clears on room release.
Original graphics/scale0.16, roads and existing floor/wall packs are unchanged.

Validation: `make test` passed core and29 Python tests. Host analytical scenarios
exercise low/sloped/one-sided/two-sided ceilings, rotated footprint, rejected
attributes, invalid pack retention, rollback and bump/landing without damage.
`build-host/core_tests --rooms` passed all8 road arrivals,18,315 wall index probes,
original water/void regressions and new original plaza ceiling jump.
`make -j2 all` regenerated locally converted packs and built ttyd-2ds.3dsx; final
repeat up to date. `python3 tools/verify_head_mesh.py` validated12,259 triangles'
source owner/attribute/cull metadata; `python3 tools/verify_actor_collision.py`
passed constants and parity/cross instruction sites. These verify metadata/
instruction evidence, not independent converted positions or Dolphin behavior.

Native boot smoke exited0. New repeated-ceiling script exited0 after5 held jumps,
landing/save at original position/elevation and HP7. Reviewed ceiling-jump
capture; source timing/proportions remain unmeasured. Native east road script
exited0 with two reciprocal trips after adding mandatory head resource loading.
Commands:
```sh
python3 tools/road_fixtures.py
python3 tools/emulator_smoke.py --fresh-save --seconds 4
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/ceiling-road.bin --seconds 4 --input-script tests/azahar_ceiling.py
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/east-road.bin --seconds 4 --input-script tests/azahar_roads.py
```
No native compilation overlapped emulator execution (final make was up to date).
All original extracts/packs/captures remain ignored. Head fixture plaza(-6.7,.1)
was located by temporary host scan; durable fixture/test now reproduces it.

Next: recover grounded slope/contact and head-clearance behavior when walking
under low geometry, original takeoff/fall flag variants, wall push-out/sliding
and dynamic collision/block events. Ceiling source SFX342 is not yet mapped.
Compare jumps/camera/proportions in Dolphin; character calibration still0.16.
Original dialogue/story, animation, SFX, interiors and complete content remain
incomplete. Full objective active; hardware performance/acceptance unverified.

## 2026-10-12 — Original behavior documentation and binary evidence

User requested original code behavior documentation suitable for a port or
strongly equivalent reimplementation. Added docs/ORIGINAL_BEHAVIOR.md: evidence
classes, exact US baseline fingerprints, public reference provenance, original
sequence/subsystem ordering, event context/scheduling/continuation and wait
semantics, field behavior index, story contract, subsystem coverage gaps and
an equivalence-validation procedure. This is a partial specification, not full
game recovery. Switch is an explicitly separate future reference; no Switch
code or behavior was analyzed. Accessible private evidence was the supplied
local ISO and existing project artifacts; no private external sources used.

New tools/behavior_evidence.py reads the ISO only as rb and emits metadata to
ignored artifacts/reference/behavior/evidence.json. Identified US G8ME01 disc 0
revision 0, 3,204 files, 29 RELs, 260 US message files. Report fingerprints ten
DOL functions, all 119 event dispatcher targets, the DOL span, gor.rel and US
message files. It also records direct branch-link sites; this is not a complete
call graph or execution trace. Public doldecomp/ttyd MIT source revision
62131fc31866f85a12e4817a8dc37f0c25eb5f8c targets Japan; selected findings were
independently checked in the US binary. Source/ELFs/disassembly stay ignored;
no original code or dialogue was added to source history or native runtime.

Validation completed: make test passed portable core and 29 Python tests;
make all exited 0 with native output up to date; verify_actor_collision.py
passed; git diff --check passed. New report was generated and independently
regenerated identically; checked expected DOL hash and opcode 2/9/10/91 targets.
Temporary altered-symbol fixture proved mismatched file/RAM mapping rejection;
CLI input-overwrite test proved nonzero exit and unchanged input. These latter
checks were session Python assertions, not new persistent unit tests. No new
Dolphin or Azahar execution was performed, and no original gameplay observations
or hardware claims were added. Commit 7d216de records research/tool changes.

Next: close US priority/tie, variable-namespace, pause, child/brother lifecycle
contracts; recover opening harbor/Crump event graph and exact US message bindings;
recover tutorial battle/action-command data and compare controlled Dolphin traces.
Continue field fidelity work described above independently. During this session,
unrelated edits appeared in include/game.hpp, source/game.cpp, tests/core_tests.cpp
and tools/verify_actor_collision.py; they were preserved and excluded from the
research commit. Validation reflects the shared working tree at execution time,
not an isolated checkout of that commit.

## 2026-10-12 — Correct ascending movement/collision order

Previous goal turn made progress with active static ceiling collisionsccf13ff.
Source marioMain tracing found ascent must integrate Mario's jump and head
check BEFORE horizontal movement. Native previously queried walls using prior
tick Y. Split vertical integration from landing completion; ascent wall rays
now use current integrated jump height and landing still selects after XZ.
Normal falling retains move-before-gravity. A new ledge fall begins integration
next tick. Source source-byte verifier locks down both branches.

Host regression: low wall blocks standing Mario, while first rising integration
clears it and advances XZ in the same tick. `make test` passes core and29 Python
tests; `build-host/core_tests --rooms` passes all8 arrivals,18,315 indexed wall
probes and original water/void/ceiling checks. `make -j2 all` built native3dsx;
`python3 tools/verify_actor_collision.py` passed original read-only instruction
checks. Initial regression compilation had a local Input name collision, fixed
with local scope; final checks passed.

Important source distinction recovered: marioChkGnd2 is not ordinary falling
landing. Its .1engine fixed-depth/penetration rule must not replace the velocity
ray from marioChkLandon, which marioFall calls before displacement. Experimental
fixed-depth change failed existing high-to-low landing test and was removed.
Final native falling query retains abs(v)+.01 below/abs(v)+5 above engine units.
Verifier now proves both callers separately; docs/MECHANICS.md records locations.

Final native water route exited0 with checkpoint protection, original return
and HP7→6 after corrected ascent order. Exact command:
`python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/harbor-arrival-road.bin --seconds 4 --input-script tests/azahar_water.py`.
No build overlapped emulator execution. Source extracts/captures remain ignored.

Next concrete task: move falling overhead check before fall gravity/search,
matching marioMain fall branch; recover marioChkLandon floor selection/acceptance
branches and source grounded marioChkGnd. Grounded overhead/legacy stepping
remain unchanged. Exact source flag transitions, slope/face filters, wall
push-out/sliding and dynamic support are unfinished. Original story/dialogue,
SFX, animation, camera, interior/story gates and complete playable content still
require work. Full goal active; hardware performance unverified.
Final native ceiling repeat exited0:5 held jumps, landed save at fixture
position/elevation and HP7. Command: `python3 tools/emulator_smoke.py --fresh-save
--save-fixture artifacts/fixtures/ceiling-road.bin --seconds 4 --input-script
tests/azahar_ceiling.py`. This verifies repeat/save stability, not measured
Dolphin action timing or source camera fidelity.

## 2026-10-12 — Detailed undecompiled-behavior research, first system set

Previous research turn made progress: original behavior baseline and reproducible
binary identity were committed. Current user goal is broader: thoroughly document
behavior not established by decompilation in multiple detailed files, then derive
complete-game system requirements assuming original data reuse. Goal remains
active; this step does not claim all systems documented or create a prematurely
complete requirements document. Native runtime was not changed in this step.

Added docs/behavior/README.md as the full-scope coverage/completion register and
four detailed files: EVENT_RUNTIME.md, BATTLE_INPUT_AND_DEFENCE.md,
STATE_AND_INVENTORY.md and RANDOMNESS.md. Original baseline links to the detail
and updates now-resolved priority/copy-back/RNG findings. Commit ef962d6 records
this system set, independent audit/verifier tools and three tooling tests.

Downloaded pinned MIT public doldecomp/ttyd source/config/REL research snapshot
62131fc31866f85a12e4817a8dc37f0c25eb5f8c into ignored artifacts/reference.
New AST-only coverage audit found 427 configured game-category objects: 421
NonMatching and 6 Matching declarations, 134 with source and 293 without.
These are configuration/source-presence counts, not verified matching functions
or a behavior-completion percentage. The public baseline is Japan; US original
bytes remain authoritative. No private external source or Switch code used.

New concrete US findings: defense checker is only a stub in public source;
original checks difficulty-dependent A/B history windows (default 8/3 entries),
15-entry A/B anti-mash count, eligibility/override branches and input-history
reset on every return. Public history-shift loop disagrees with US instructions.
Source defense caller gates and video-frame alignment remain unknown. Event
priority uses an unstable exchange sort (equal-priority order can reverse);
child creation appends to an active pass, child deletion copies locals back,
owner deletion recursively removes children/attached brothers. Saved-work index0
is a word while ordinary work entries are bytes; resource setters have different
lower-clamp rules; item insertion preserves ordered arrays and can mutate badge
seen flags before capacity failure. rand/irand arithmetic and zero-bound no-draw
behavior recovered. Detailed addresses, limits and derived examples are in docs.

Validation: make test passed portable core and 32 Python tests (three new audit
tests distinguish declaration from source presence, prove upstream config is not
executed, and reject escaping paths/unsupported config). make all exited0 with
native output up to date. verify_behavior_contracts.py passed 56 selected original
instruction anchors and both seven-entry defense tables. Expanded binary report
reproduced exactly with 51 function fingerprints; AST audit report also reproduced
exactly. All local links in baseline/new docs resolved; git diff --check passed.
These static checks do not execute original routines or establish gameplay
equivalence. No new Dolphin/Azahar/hardware validation performed. All downloaded
source and original inspection binaries/disassembly remain ignored.

Reproduce:
```sh
python3 tools/decomp_coverage.py --snapshot artifacts/reference/decomp-62131fc31866f85a12e4817a8dc37f0c25eb5f8c --output artifacts/reference/behavior/decomp-coverage.json
python3 tools/behavior_evidence.py --symbols artifacts/reference/us_symbols.csv --output artifacts/reference/behavior/evidence.json
python3 tools/verify_behavior_contracts.py
make test
make all
```

Next research: trace defense checker callers/override writers and complete
BattleCheckDamage (800fb01c), BattleSetStatusDamage (800fbed8), and battle turn/
action-command lifecycles; recover VM operand namespaces/all opcodes and original
harbor story graph. Then inventory persistent-state writers and original save
records; extend to UI/message interpreter, audio, camera/animation, all partners,
chapters, side content and minigames. Detailed full-scope register lists every
remaining system and requirements-document completion gate. Reused assets do not
remove the need to recover their runtime semantics. Do not mark the full goal
complete based on this first system set or the prototype's green tests.

Shared-workspace note: after validation, unrelated edits appeared in
source/game.cpp, tests/core_tests.cpp, tools/map_convert.py and
tools/verify_ground_attributes.py. They were preserved and excluded from the
research/handoff commits. Test/build results above describe execution-time state;
they do not certify those later edits or an isolated checkout.

## 2026-10-12 — Correct falling overhead phase and takeoff clamp

Previous goal turn made verified progress with ascent wall-query orderc4b2fc6.
This step moves falling head checks before fall derivative integration/search,
matching original move→overhead→marioFall order. A falling bump installs source
normal fall derivatives, then performs their first update in the same tick.
Head ray extension is now separate from actual velocity: falling uses source
10engine extension, preserves actual velocity contact cutoff. Falling bumps
bypass saved takeoff-Y minimum; ascent retains it. Apex-to-fall head query also
uses the non-ascent extension. This fixes missed pre-gravity contacts and upward
teleports when bumping below the takeoff floor.

Validation: `make test` passed core and29 Python tests. New host cases insert
overhead geometry at descending head height and assert clamp followed by-.003
normal fall movement; falling from height1 then bumping below it stays below
takeoff and lands on lower floor. Actual fast-fall cutoff still rejects contact
with explicit positive ray extension. Existing low/sloped/one-sided/two-sided
ceiling, ascent low-wall, lower/higher platform and invalid-pack tests pass.
`build-host/core_tests --rooms` passed all8 road arrivals,18,315 wall-index probes
and original water/void/ceiling checks. `make -j2 all` built ttyd-2ds.3dsx;
`python3 tools/verify_actor_collision.py` passed source move/head/fall calls,
ray-extension load, fall-flag takeoff bypass and fall-derivative instruction
sites. Source was read-only. No extracted originals/packs/captures enter Git.

Native ceiling repeat exited0:5 held jumps with landed position/elevation/HP7
save. Native water script exited0: partial checkpoint protection, original
return and HP7→6. No native build overlapped emulator execution. Commands:
```sh
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/ceiling-road.bin --seconds 4 --input-script tests/azahar_ceiling.py
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/harbor-arrival-road.bin --seconds 4 --input-script tests/azahar_water.py
```
These are repeat/recovery checks, not matched Dolphin timing/proportion proof.

Next: source marioChkGnd grounded support/head behavior and marioSearchGround
normal slope/attribute acceptance; replace generic.11 landing footprint snap
and legacy grounded.25/.5 band only with traced branches. Preserve distinction
between predictive marioChkLandon and marioChkGnd2 penetration checks. Floor
face culling, horizontal push-out/sliding, dynamic support and original contact
block/SFX events remain incomplete. Original dialogue/story, music/SFX event
fidelity, animation, camera, interiors and wider content still need work. Full
goal active; hardware acceptance remains unverified.

## 2026-10-12 — Source-derived floor-candidate reach

Previous goal turn made progress correcting falling head phase048ff7a. This
step replaces symmetric.11 landing footprint selection with original branches:
floor>=2engine below root must be reachable by abs(vertical velocity); remaining
near/above candidates use11engine snap. New include/floor_contact.hpp documents
the source policy, and unused FloorSnapDistance constant is removed. Original
source verifier now checks2/11 values and branch sites. No grounded stepping,
geometry export, camera, map warps or render scale changes.

`make test` passed core and32 Python tests (additional research tests appeared
in user worktree). New host boundaries cover2engine inclusive transition, zero/
small fall velocity, reachable18engine terminal step and11engine above contact.
New3.14→0 drop lands on lower floor with HP7 and no void recovery. Existing
original-room checks passed: all8 arrivals,18,315 indexed wall probes, water/
void/ceiling. `python3 tools/verify_actor_collision.py` passed source constants
and branches. `make -j2 all` built native3dsx; final header cleanup build recorded
below. Source ISO/extracted maps read-only; generated packs remain ignored.

Native water regression exited0 with checkpoint protection, original return
point and HP7→6: `python3 tools/emulator_smoke.py --fresh-save --save-fixture
artifacts/fixtures/harbor-arrival-road.bin --seconds 4 --input-script
tests/azahar_water.py`. Header-only unused constant removal followed this run;
no behavior changed afterward. No build overlapped emulator execution.

Next: recover normal slope/attribute branches before candidate acceptance,
source floor culling, grounded marioChkGnd support/head behavior and replace
legacy.25/.5 stepping. Original distance/angle outputs for rejected candidates
are not consumed yet. Wall sliding/push-out, dynamic support, original story/
dialogue/SFX, camera, animation, interiors and full playable content remain
incomplete. Full goal active; hardware acceptance unverified. Preserve unrelated
user work in tools/behavior_evidence.py and new research tools/tests; only stage
explicit feature paths.
Final header-cleanup `make -j2 all`, `make test` (core+32 Python) and
`build-host/core_tests --rooms` all exited0. Unrelated user research changes
remain outside this incremental feature commit.

## 2026-10-12 — Runtime ordinary slope acceptance

Previous turn made progress with source floor-reach branchescf633ee. Original
marioSearchGround normal slope threshold is50degrees; special states use75.
Added normal slope rejection after nearest solid plane selection. Removed
converter.65 normal-ratio cutoff, which incorrectly removed some valid near50
slopes; preserve all projected nondegenerate planes, including steep blockers.
Legacy ground format/face orientation remain unchanged. Native uses atan2
rather than source angleABf lookup arithmetic; boundary bit-exactness is not
established. Constants/load/comparison/reset instructions are read-only verified.

`make test` passed core and32 Python tests. Added49.7degree accepted import,
50.1degree rejection, vertical/degenerate normal and steep-over-flat masking.
Original-room full-scan oracle now independently applies squared-cosine cone
comparison after nearest-hit selection; initial stale oracle failed as expected
for newly rejected steep planes, updated against the intended rule.
`build-host/core_tests --rooms` passed18,315 indexed wall/floor probe cases,
all8 arrivals and original water/void/ceiling tests. `make -j2 all` regenerated
original-derived packs and built ttyd-2ds.3dsx. `python3 tools/verify_actor_collision.py`
and `python3 tools/verify_ground_attributes.py` passed50/75 source constants/
branch sites and all4,946 floor attributes/source nodes. Full-scan comparison
proves index/query consistency, not original collision fidelity.

Native harbor stairs script exited0: dock traversal/save at original elevation,
HP7 preserved, return to plaza. Exact command:
`python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/harbor-arrival-road.bin --seconds 4 --input-script tests/azahar_harbor_stairs.py`.
No build overlapped emulator execution. Source inputs read-only; all extracts,
generated geometry and captures stay ignored. Unrelated user research work
remains separate. No character-scale/camera/road-policy changes.

Next: add source cull byte and hit_topology winding to floor pack (new record
version with legacy compatibility), reject one-sided downward backfaces before
nearest hit selection and orient two-sided normals. Then recover grounded
marioChkGnd/head rules and replace legacy.25/.5 stepping. Source angleABf lookup
arithmetic, special states, wall push-out/sliding, dynamic supports, original
dialogue/story/SFX/animations/camera/interiors and full content remain incomplete.
Full goal active; hardware acceptance unverified.

## 2026-10-12 — Source floor winding and downward face culling

Previous goal turn made progress with runtime slope filter0b2f479. Floor export
now uses original hit_topology ordering (same as head geometry) and includes
raw source drawmode byte1. New header bits31+30 /48-byte records preserve
attributes/source node/cull. Loader supports legacy36/44-byte packs as two-sided;
invalid cull modes and truncated metadata preserve previous room. Cull travels
with GroundTriangle through snapshots/rollback. Downward source mode1 backfaces
(normalY<=0) are rejected before nearest hit; modes0/2/3 are two-sided. Original
1,867 downward backfaces across five rooms no longer act as candidate floors.
No render/camera/character-scale change.

Validation: `make test` passed core and32 Python tests. New analytical cases
verify culled upper underside allows lower floor, reversed upper winding is
solid, two-sided faces remain solid, invalid/truncated packs retain geometry,
rollback restores cull metadata. A new assertion initially used exact float
equality for interpolated.2 height; changed to numeric tolerance, final tests
passed. `build-host/core_tests --rooms` passed18,315 full-scan/index comparisons,
all8 arrivals and original water/void/ceiling checks. Oracle now reads face
metadata and rejects backfaces before nearest/slope selection.
`make -j2 all` regenerated source-facing floor packs and built ttyd-2ds.3dsx.
`python3 tools/verify_ground_attributes.py` verifies all4,946 records' source
attributes/cull/node bytes, reporting1,867 backfaces. Actor source instruction
verifier passes. These checks do not independently prove transformed source
positions or complete original behavior. ASSETS.md records pack/provenance and
legacy behavior; prior separate research commits remain intact.

Native harbor stairs exited0 (dock traversal/save, HP7, return to plaza). Native
east roads exited0 (two reciprocal trips, arrival saves). Exact commands:
```sh
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/harbor-arrival-road.bin --seconds 4 --input-script tests/azahar_harbor_stairs.py
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/east-road.bin --seconds 4 --input-script tests/azahar_roads.py
```
No build overlapped emulator execution. All original extracts/generated packs
remain ignored. Physical New2DS performance/acceptance remains unverified.

Next: recover grounded marioChkGnd reach arguments and support acceptance,
including upstream stair/step correction before changing legacy.25/.5 band.
Keep loader/arrival validation distinct from active grounded movement queries.
Grounded overhead still absent; original lookup angle arithmetic/special states,
signed contact normals/animation, wall sliding/push-out, dynamic supports/block
events and SFX remain incomplete. Original story/English dialogue, camera,
animations, interiors and complete playable content still require work. Full
objective remains active; no completion claim.

## 2026-10-12 — Original hit precheck and HP/FP calculation contracts

Research commit 05c59d3 adds docs/behavior/BATTLE_HIT_PRECHECK.md and
BATTLE_DAMAGE_CALCULATION.md. They document US branch order, numeric flags,
signed loads, conditional clamps, callback gates, result codes and explicit
unresolved dependencies. Precheck includes ordered per-copy evasion RNG draws,
accuracy reductions and early-exit side effects. HP/FP contracts distinguish
All or Nothing bypass behavior, absorption, modifier order, linear Double Pain
stacking and Last Stand integer division. Defense documentation now records the
confirmed btlevtcmd_ResultACDefence event entry without claiming attack impact
timing. Public names are annotations; US original bytes establish these local
contracts. No Switch-version equivalence or original execution was measured.

Evidence tooling now fingerprints 55 functions. Static verifier checks 78
instruction anchors, both seven-entry guard tables and binary32 accuracy
multiplier 0.5 at US RAM0x804221c4. tools/behavior_evidence.py rerun was identical
to its saved JSON across two reads. All docs/behavior relative links resolve;
git diff --check passed. make test passed the core test and32 Python tests;
make all exited0 with nothing to rebuild. These are checks of execution-time
shared-workspace state, not isolated checkout validation: concurrent runtime
edits in include/floor_contact.hpp, source/game.cpp, tests/core_tests.cpp and
tools/verify_actor_collision.py were present and were not staged by research.
No emulator launched, profile/save modified, or runtime behavior implemented by
this research step. ISO stayed read-only; extracted ELF/disassembly and reports
remain ignored. Provenance and licensing recorded in ASSETS.md.

Next research: recover BattleCheckCounter (0x800fa678), BattleCheckDamage
(0x800fb01c), BattleDamageDirect (0x800fdfd4), status application and emergency
revival into separate contracts; connect all callers and original weapon data.
Ignored annotated disassemblies for these and the status routines are available
under artifacts/reference/behavior but are not substitutes for reviewed docs.
Trace hit-counter reset lifetimes, battle mode/accuracy-effect producers and
weapon callbacks before claiming a complete combat contract. Add original
execution traces for differential validation. Continue the full-system register
in docs/behavior/README.md: story/event services, partners, audience/rewards,
save records, UI, animation/audio and complete content remain incomplete.
The complete-game requirements document remains gated on that recovery; full
research objective is active and has not been achieved by these local contracts.

## 2026-10-12 — Counter selection and revival eligibility research

Previous research turn was progress:05c59d3/20ac72e added hit/damage contracts.
This turn's4568b28 adds detailed BATTLE_COUNTERS.md:48-byte initialization,
all exclusive/accumulating selector branches, resistance/badge/status gates,
last-writer classification priority, exact12-entry return mapping and consumer
half-damage/status/direct-damage/event order. Binary code48 maps to return0
while retaining a populated counter record; the main caller uses work fields.
Separately rounded half contributions and the equality-only zero→one fallback
are explicit. Damage documentation adds complete local revival code-marking
helper:inventory vs unit item paths, inclusive kind ranges and unchanged failure
codes. Actual item consumption/HP restoration remains unrecovered.

Validation: read-only US ISO verifier passed95 instruction anchors, both guard
window tables, accuracy constant and all12 counter jump-table targets.
Evidence report fingerprints58 functions and repeated identically across two
reads. Relative links in docs/behavior resolve; git diff --check passed.
make test passed core plus32 Python tests; make all exited0, nothing to rebuild.
Checks used shared workspace at execution time, with concurrent runtime edits
in include/floor_contact.hpp, include/game.hpp, source/game.cpp,
tests/core_tests.cpp, tools/verify_actor_collision.py and untracked
include/field_step.hpp. Research did not stage/change those files. No emulator
or original executable was run, and no profile/save changed. Source ISO opened
read-only; original extracts/disassemblies remain ignored; ASSETS.md updated.
Static anchor checks validate cited bytes, not full dynamic equivalence.

Next: full BattleCheckDamage orchestration, BattleDamageDirect application,
BattleSetStatusDamage and supporting status getters/setters/lifecycle; original
attack callbacks and data bindings; death/revival event and item handling.
Counter presentation still needs exact sound/effect bindings. Capture original
execution for differential tests when contracts and a useful harness permit.
Continue all remaining systems in docs/behavior/README.md before deriving the
complete-game requirements document. Full objective remains active/incomplete;
these local combat contracts do not establish complete battle or full-game
behavior, story fidelity or New2DS performance acceptance.

## 2026-10-12 — Grounded height limits and three-tick upstairs motion

Previous goal turn progressed source floor facesf268eaf. Recovered original
front-step query/caller and mot_upstairs motion12. Active movement now blocks
rise>.11native; drops>=.02 fall once footprint support ends. Qualifying.05.. .11
rises on surface<=30degrees interpolate root over3 ticks instead of instant
snap. New FieldStep continues horizontal collision at interpolated Y and checks
overhead; transient saves/warps/jump/interaction guarded, lifecycle resets clear
state. Original query width*.265 forward distance, rise5..11/angle30 and three
counter/equal-displacement instructions are source verified.

Scope limit: current stair trigger samples candidate floor rather than original
forward point. Full grounded ray reach, quantization and XZ/motion flags are
not integrated. Room/load groundAt keeps legacy broad band; partner does too.
The main player height acceptance now bounds that candidate before applying it.
Original stair pose/shadow/camera and ceiling cancellation/floor reacquisition
need matched verification. This is further playable physics progress, not final
source/Dolphin fidelity.

Validation: `make test` passes core and32 Python tests; added exact3-step helper
and Game interpolation, partial-save/jump guards, blocking.2 ledge and.08 drop
fall/landing. `build-host/core_tests --rooms` passes all8 arrivals,18,315 indexed
wall/floor probes and original water/void/ceiling checks. `make -j2 all` builds
native3dsx; `python3 tools/verify_actor_collision.py` passes original step slope/
distance, motion-table target,3-count and velocity/division instructions.

Native harbor stair traversal/dock-save/return exited0 with HP7; native water
route exited0 with partial-checkpoint protection, original return and HP7→6.
Commands:
```sh
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/harbor-arrival-road.bin --seconds 4 --input-script tests/azahar_harbor_stairs.py
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/harbor-arrival-road.bin --seconds 4 --input-script tests/azahar_water.py
```
No build overlapped emulator execution. Original binary inspections/packs stay
ignored; separate untracked research notes are left untouched. Architecture and
mechanics notes describe state and adaptations. Hardware performance unverified.

Next concrete task: implement original forward step ray at width*.265,
root+half-height with downward reach half-height+11+.1engine, plus source filter
and caller step selection; replace candidate trigger. Then main grounded
marioChkGnd five-probe ray/height quantization and contact outputs, keeping
entry validation separate. Source move-state XZ rates during stairs and low
ceiling cancellation need review. Grounded overhead, wall sliding/push-out,
dynamic support, original story/English dialogue/SFX/camera/animations/interiors
and complete playable content remain incomplete. Full objective stays active.

## 2026-10-12 — Direct damage application and shared FP research

Previous goal turn progressed via4568b28/0ab7bdb counter contracts. Research
commit41f6e27 now adds BATTLE_DAMAGE_APPLICATION.md: full local ordering of
BattleDamageDirect, ±99 HP clamp, signed-byte amount accounting, conditional HP
mutation, original/unbounded FP argument handling, shared partner FP owner,
Pity Flower before FP subtraction, Sleep recovery request RNG, exact effect
parameters/sound priority, action-record calls and lethal-event flag selection.
Five supporting resource/owner helpers were inspected from original US bytes.
Death/removal, revival consumption and HP restoration are explicitly left with
unrecovered hit-event handlers rather than inferred from the marking flags.
Coverage links and ASSETS.md provenance were updated. No runtime implementation
was changed by this research.

Validation: static ISO checks passed119 instruction anchors, both guard tables,
accuracy constant,12 counter jump targets,9 damage-effect constants and7 sound
keys. Evidence report now fingerprints63 functions and matched a repeat read.
All behavior relative links resolved; git diff --check passed. make test passed
core plus32 Python tests; make all exited0 with nothing to rebuild. Checks were
against execution-time shared workspace, with unrelated runtime edits remaining
in include/floor_contact.hpp, include/game.hpp, source/game.cpp,
tests/core_tests.cpp, tools/verify_actor_collision.py and untracked
include/field_step.hpp. Those files were not staged. ISO stayed read-only;
extracted helper ELFs/disassembly and reports are ignored. No original binary or
emulator was run, and no user profile/save was touched. Static anchors verify
selected bytes, not complete dynamic equivalence or tested gameplay timing.

Next: recover BattleCheckDamage orchestration and weapon/event callers;
BattleSetStatusDamage, status getter/setter/recovery/turn lifecycle; actual hit
event dispatch and death/revival event bodies. Trace unit/part/data lifetimes,
attack callback catalog, action-record and audience helpers, complete effect and
sound-macro execution. Add original execution traces and differential tests.
The full-system register remains open for story/events, partners, field forms,
save/UI, assets/animation/audio and complete content. Derive the complete-game
requirements only after those contracts are documented. Full objective remains
active; no complete-game or complete-battle equivalence claim.

## 2026-10-12 — Battle reaction event dispatch research

Previous goal turn progressed with41f6e27/8de16ad direct damage contracts.
Commit b897a67 adds BATTLE_EVENT_DISPATCH.md: normal/direct hit dispatch,
low-code40 data-key46 gate, eight-byte terminated data lookup, ordered four-slot
cancellation, null-script distinctions, wait/phase event differences, same-pass
scheduler insertion and raw-vs-resolved service operands. Full stored damage
code, low-byte getter and any-bit mask test are distinguished. Public header
US offset mismatch is explicitly recorded. These contracts connect damage
application to scheduling without claiming death/revival script bodies are
recovered. No runtime code changed by this research.

Validation:141 static instruction anchors and previously documented tables/
constants/sound keys passed against read-only US ISO. Evidence report contains
74 function fingerprints and reproduced identically across two reads. All
behavior relative links resolve; git diff --check passed. make test passed
core plus36 Python tests at execution time; make all exited0, up to date.
Concurrent runtime and Switch-research changes were present and not staged:
include/floor_contact.hpp, include/game.hpp, source/game.cpp,
tests/core_tests.cpp, tools/verify_actor_collision.py, and new Switch tools/tests.
Other concurrent docs/ARCHITECTURE.md and docs/MECHANICS.md edits were also left
unstaged. Thus validation describes shared workspace, not an isolated commit.
No emulator/original code executed; profile/saves unchanged. Original extracts
and disassembly remain ignored. Provenance/fingerprint counts updated in ASSETS.

Next: recover complete BattleCheckDamage orchestration and status application/
recovery/turn lifecycle. Enumerate actual damage-script installations, relocated
script/data bindings and reachable death/revival bodies; inspect turn-controller
wait/ID ownership before claiming event lifetime equivalence. Constructor
capacity handling and remaining event services stay open. Continue full-system
coverage register (story, field/forms, partners, economy, save/UI, animation/
audio and all content) and derive complete-game requirements after recovery.
Full objective remains active; documentation and end-to-end equivalence are
incomplete, with static evidence not replacing original execution traces.

## 2026-10-12 — Activate original forward step ray

Previous goal turn progressed bounded grounded motion/FieldStep7857a24.
Replaced candidate-floor step activation with forward query after horizontal
movement: normalized direction*.053native, origin root+.185, bottom root-.111.
This corresponds to source baseWidth20*.265 and halfHeight18.5 downward reach
18.5+11+.1engine. Source attribute/face/nearest selection is reused; caller
requires5..11engine rise and<=30degree surface. Direct larger candidate rises
remain blocked until the forward ray starts the3-tick motion. Probe can now
anticipate a stair while actor center remains on lower floor. Existing loader
and room-arrival ground validation remain separate.

Validation: `make test` passed core and32 Python tests initially,36 in final
repeat after separate research inventory tests appeared. New host tests cover
forward5.3engine crossing threshold, reverse direction, zero-direction output
retention, rejecting a surface above18.5engine origin, rise/angle caller bounds
and early activation fromlowerx.07 before crossingupperx<=0. Existing3-tick
state/save/jump guards and tall ledge/drop tests pass.
`build-host/core_tests --rooms` passes all8 arrivals,18,315 index/full-scan
probes, original water/void/ceiling checks. `make -j2 all` builds native3dsx.
`python3 tools/verify_actor_collision.py` passes half-height/width multiplier,
ray-origin/length instruction sites and source constants/caller/motion proof.
No extracted originals/generated packs are committed.

Native harbor stairs/dock-save/return exited0 with HP7; native east roads
(two reciprocal trips/arrival saves) exited0. Exact commands:
```sh
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/harbor-arrival-road.bin --seconds 4 --input-script tests/azahar_harbor_stairs.py
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/east-road.bin --seconds 4 --input-script tests/azahar_roads.py
```
No build overlapped emulator execution. Separate Switch/package research tools
and tests remain outside this feature commit. Architecture/mechanics notes
record ordinary caller behavior and limitations. No hardware/Dolphin-fidelity
acceptance claim from these checks.

Next concrete task: replace main grounded center-first support with original
five rotated probes at7.5engine radius, source grounded ray reach and selected
floor quantization. Preserve separate predictive falling/forward ray/arrival
queries. Complete marioChkFrontStep drop-distance/angle outputs and source
movement-angle rounding, staircase XZ rate/state chaining and low-ceiling
cancellation under actual source framing. Wall push-out/sliding, dynamic
support, grounded overhead, signed normal animation outputs, original story/
English dialogue/SFX/animations/camera/interiors and full playable content remain
unfinished. Full goal active; hardware performance unverified.

## 2026-10-12 — Status storage and explicit recovery-step research

Previous turn progressed with b897a67/a6a19cd event dispatch contracts. This
turn adds BATTLE_STATUS_STORAGE.md: complete local getter/setter cases for
IDs0–27, shared signed fields, badge-only Electric presence, Burn/Freeze and
Defense cancellation, size/attack replacement, Charge narrowing-before-clamp,
Fast/Slow move-count gate, all recovery-step groups and HP/Instant Kill gates.
Direct-damage Sleep recovery now links to its verified local clearing outcome.
Recovery is specified as an explicit helper call, not assumed once per turn.
Infliction rates/immunity and complete turn scheduling remain unrecovered.

Validation:153 instruction anchors and all81 status dispatch targets passed
against read-only US ISO, alongside previous tables/constants/sound keys.
Evidence now fingerprints79 functions and reproduced identically across reads.
Behavior relative links resolve; git diff --check passed. make test passed
core plus36 Python tests; make all exited0, nothing to rebuild. This validates
execution-time shared workspace, not an isolated commit: unrelated runtime,
architecture/mechanics documentation and Switch inventory/tools/tests edits
were present and left unstaged. No native runtime behavior changed in this
research. Original helper ELF/disassembly/report files stay ignored; no
original executable/emulator run or profile/save change. ASSETS.md updated.

Next: BattleSetStatusDamage and weapon status parameter/rate helpers, complete
BattleCheckDamage orchestration, recovery-call schedule and periodic status
HP/FP/action effects. Recover actual unit death/revival event graphs and item
consumption. _CheckMoveCount's downstream BtlUnit_ResetMoveStatus remains open.
Continue full-system register beyond combat and derive complete-game system
requirements only once those original contracts are adequately documented.
Full objective remains active/incomplete; static checks are not original
execution tests or end-to-end fidelity proof.

## 2026-10-12 — Weapon status parameters and infliction admission

Previous turn progressed via5874375/cc82bc9 status storage/recovery contracts.
This turn adds BATTLE_STATUS_INFLICTION.md: complete weapon wrapper iteration,
base duration/strength/rate fields, equipped-badge parameter changes, original
weapon-pointer Charge cases, ordered removal/immunity gates, susceptibility
lookup, exact chance arithmetic/RNG draw and mutation/result-bit prefix.
Application presentation/announcement/return tail from0x800fc198 onward remains
explicitly outside detailed coverage. No complete status-system claim.

Validation:165 instruction anchors plus previous tables/constants/sound keys
passed; eight additional inspected infliction table fingerprints cover201
entries. Evidence report fingerprints84 functions and matched a repeat read.
Behavior links resolve; git diff --check passed. make test passed core plus36
Python tests. make all compiled game.cpp, linked and built ttyd-2ds.3dsx,
exit0 (session77701 completed). Validation was execution-time shared workspace:
concurrent floor/game/core-test/actor-verifier edits and new Switch research
tools/tests were present and not staged. No runtime changes authored by this
research. ISO opened read-only; original extracts/disassemblies/reports remain
ignored. No original code/emulator executed or user profile/save modified.
ASSETS.md records updated evidence scope. Fingerprints identify inspected data;
they do not prove semantic interpretation or dynamic equivalence.

Next: finish BattleSetStatusDamage effect/cleanup/announcement/sound and return
contracts, then complete BattleCheckDamage orchestration and all status turn
callers. Resolve weapon identities/data bindings, equipment-count helper,
periodic effects and actual death/revival event graphs. Continue other systems
in behavior/README before deriving complete-game requirements. Full objective
remains active and incomplete; original execution traces and end-to-end fidelity
checks are still required.

## 2026-10-12 — Status application presentation and completion

Commit0eb2aa1 completes local BattleSetStatusDamage control-flow documentation
with BATTLE_STATUS_PRESENTATION.md. It specifies position corrections, all stamp
cases and payload bytes, up/down effects, cancellation cleanup, signed result
announcement gates, unchanged Charge mode, sound bindings and final return.
Whole-call return1 after admission/chance is distinct from setter success;
opposite-status cancellation can return1 without an active requested status.
Infliction contract and behavior index now link this tail. Effect rendering,
announcement queue internals and lifecycle scheduling are still unrecovered.

Validation:177 instruction anchors, ten infliction/presentation table hashes
(254 entries), five status position constants and all prior checks passed
against the read-only US ISO. Evidence report identifies84 functions and the
unchanged US DOL digest. Behavior links resolve and git diff --check passed.
make test passed core plus36 Python tests. make all exited0, nothing to rebuild.
Checks used the shared workspace with concurrent runtime, architecture/mechanics,
and Switch research changes present; those edits were not staged. No runtime
change authored, original executable/emulator run, or profile/save modification.
Extracts/disassembly/report remain ignored; ASSETS.md records evidence scope.
Static checks do not establish dynamic or visual equivalence.

Next: complete BattleCheckDamage orchestration, status turn callers and periodic
HP/FP/action effects; recover announcement/effect internals, weapon bindings,
equipment-count semantics and actual death/revival event graphs. Continue the
full-system behavior register before deriving complete-game requirements. The
full objective remains active and incomplete; original execution traces and
end-to-end fidelity comparisons are still required.

## 2026-10-12 — Main five-probe grounded support

Previous turn progressed original forward step trigger03dc212. Added normal
groundedFootprint: five rotated7.5engine probes, depth11+.01engine, above5
for ordinary walking/standing velocity0. Source nearest face/slope/candidate
acceptance precedes selecting highest result across all probes. Game now
updates support once after full horizontal movement, instead of center-first
acceptance/fallback; forward stair ray observes old root before support update.
Missing support starts falling. Broad groundAt remains for entry/partner and
coarse unwalled tall-ledge guard; it no longer snaps main support via.25/.5 band.
Removed superseded groundedHeightCanSnap policy and retained source2/11 bounds
where consumed.

Important source trace: rounded floor height is actor+0x1c4, while actualY
+0x90 receives raw search output at8008fae8. No direct root quantization added.
A separate original floor cache is still unrepresented. Byte verifier checks
grounded11/current-v arguments, rounded cache store and raw actualY store.

Validation: `make test` passed core and36 Python tests; added highest-side
support despite lower center, rotated footprint changing selection, zero-vector
output retention,4.9engine accepted/5.1engine rejected upward probe reach and
Game edge-height/contact update. Existing stair anticipation/interpolation,
blocked tall ledge, fall landing and partial-save guards pass.
`build-host/core_tests --rooms` passes all8 arrivals,18,315 indexed/full-scan
probes and original water/void/ceiling checks. `make -j2 all` builds native3dsx;
`python3 tools/verify_actor_collision.py` passes source arguments/store evidence.

Native harbor traversal/dock-save/return exited0 with HP7; native water route
exited0 with checkpoint protection, original return and HP7→6. Commands:
```sh
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/harbor-arrival-road.bin --seconds 4 --input-script tests/azahar_harbor_stairs.py
python3 tools/emulator_smoke.py --fresh-save --save-fixture artifacts/fixtures/harbor-arrival-road.bin --seconds 4 --input-script tests/azahar_water.py
```
No build overlapped emulator execution. Extracted originals/compiled artifacts
stay ignored. Separate research files remain outside this feature commit.
Architecture/mechanics notes record source cache distinction and scope.

Next: grounded head/clearance after movement and exact source contact/cache/
quantization consumers. Review stair state chaining/XZ rate, nonzero residual
velocity and source direction rounding; source wall push-out should replace
the coarse tall-ledge guard. Partner still needs original NPC grounding.
Dynamic support, block/SFX events, original English story/dialogue, animation,
camera, interiors and complete playable content remain unfinished. Full goal
active; hardware/Dolphin fidelity acceptance unverified.

## 2026-10-12 — Damage orchestration research validation

The preceding goal turn made progress with5813bb3, then was interrupted before
this handoff. BATTLE_DAMAGE_ORCHESTRATION.md specifies all local BattleCheckDamage
branches, superguard variants, guard eligibility, status/damage ordering,
code rewrites, audience calls, reaction selection, result precedence and
Pikkyoro selection. Two retained RNG draws must not be optimized away: the
Freeze event comparison against zero and pattern selection after immunity.

Completed validation before interruption:203 instruction anchors and previous
data checks passed, plus orchestration presentation bindings/zero initializer.
Evidence report fingerprints87 functions and matched a second read byte for
byte. make test passed core plus36 Python tests; make all exited0, no rebuild.
Behavior links resolved; git diff --check passed. Concurrent runtime work was
committed separately asb182246; this research did not author or stage it.
Original extracts/disassembly remain ignored. No original execution, emulator,
profile/save modification or dynamic-equivalence claim in this research.

Next research: status recovery scheduling and periodic effects, event waits,
actual death/revival scripts, weapon bindings and full-system coverage. Complete
game requirements remain deferred until the systems are adequately documented.

## 2026-10-12 — Turn status recovery passes and wait boundary

Added BATTLE_TURN_RECOVERY.md:23 ordered recovery phases,64-slot iteration,
generic recovery event registration/sound, allocation failure handling, Slow
move reset, Freeze expiry damage/effect and event-wait transitions. Changed
Sleep exits before polling its wait state; other changed recovery phases recurse
into the wait. Empty phases skip waits recursively. Documented outer active-event
wait protocol; its initial unit/background helper still needs recovery.

Read-only DOL scan found24 direct branches to CheckRecoveryStatus:23 in turn
controller and one Sleep-interruption call in direct damage. Report explicitly
excludes indirect calls, REL relocations and script operands. Static validation
passed212 instruction anchors plus23 recovery calls,78-entry dispatch-table hash,
recovery sounds/scale and previous checks. Evidence92 function fingerprints
matched a repeat report byte for byte. Links and git diff --check passed.
make test passed core plus36 Python tests; make all exited0, no rebuild. A final
Sleep transition anchor was added afterward and the full static verifier rerun.
No native gameplay code authored, original execution/emulator run, or profile/
save changes. Concurrent Switch tools were committed separately asdfc276e.
Source extracts/disassemblies/reports remain ignored; ASSETS.md updated.

Next: periodic Poison/Burn/HP/FP phases before recovery, generic recovery event,
ResetMoveStatus, CheckShadowGuard, initial unit wait helper and conclusion
handling. Recover outer turn/phase cadence and actual death/revival event bodies.
Continue all other systems in behavior/README before complete-game requirements.
Full objective active/incomplete; static fingerprints do not prove original/native
runtime or rendering equivalence.

## 2026-10-12 — Periodic Poison/Burn requests and delayed Burn effects

Previous goal turn progressed witheff4d81 recovery scheduling. Added
BATTLE_PERIODIC_DAMAGE.md for turn states06..09: Poison slot scan and signed
strength damage, Burn presence/registration/polling/substate waits, complete
local Burn effect state machine and shadow-guard predicate. Burn does not
recheck HP/status at final damage; protection is checked after deleting the
effect. Fresh registration requires62 helper invocations absent external changes,
not a measured62-frame claim. HP/FP regeneration remains next recovery work.

Validation:222 instruction anchors and four added Burn binary32 constants plus
all prior checks passed against read-only US ISO. Evidence report95 function
fingerprints reproduced byte-identically. Behavior links resolve; git diff
--check passed. make test passed core plus36 Python tests; make all exited0,
nothing to rebuild. No runtime code changes, original execution/emulator run,
or profile/save modifications. Concurrent Switch docs/tools were left unstaged.
Original extracts/disassemblies/reports remain ignored; ASSETS.md updated.
Checks establish static identity/anchors, not executed-original equivalence.

Next: periodic HP/FP regeneration, sinfd numeric contract and effect internals,
recovery event body, move reset, unit-event wait/conclusion helpers, outer
turn/phase cadence and actual death/revival scripts. Continue full-system
register before deriving complete-game requirements. Objective remains active
and incomplete; original/native trace and rendering comparisons still needed.

## 2026-10-12 — Switch 1.0.0 baseline and static mechanics research

User changed this research goal's reference from GameCube to the supplied
Switch NSP, then instructed **static analysis only for now**. Honor that limit:
no further emulator launch/capture or firmware work. Native prototype development
and GameCube contracts remain separate; do not relabel their constants as Switch
findings. New entry point is docs/switch/README.md, linked from README and the
GameCube behavior index.

Confirmed package SHA25604fba23ab27de88276d2a04eaf69619cd09044acb41de62c1cb2f89a8de110e4,
application0100ecd018ebe000, CNMT version0, NACP1.0.0 and main NSO build
78f37bb55d015be3b368ec22af595455f1544dc1000000000000000000000000.
PFS0 has6 entries/5,456,787,792bytes. Decrypted using user-supplied local keys;
title.keys lacked this game's rights ID, resolved privately from the package's
common ticket. Original package/key files unchanged. hactool archive signature,
PFS0 and RomFS hash checks report GOOD; all3 CNMT content hashes/sizes match the
PFS0 inventory. First unmatched-key attempt reported corrupted sections and is
explicitly superseded, not misreported as success.

Extracted9,959 RomFS files, including1,246 script files,1,582 compressed data
ELFs and47 msg/US_English_Final resources. Added docs/switch/BINARY_BASELINE.md,
BATTLE_PARAMETERS.md, FIELD_PARAMETERS.md and VERSION_DIFFERENCES.md. Documents
separate binary values, reviewed native contracts, publisher descriptions and
unknowns. Nintendo1.0.1 fixes are not treated as behavior of the supplied1.0.0.
No story dialogue copied into source or invented.

Recovered661 ordered rows:360 battle parameters,18 player states,30 jumps,
197 Mario and56 partner action commands. Preserve26 null battle IDs and repeated
names. Row layout/pointer relocation verification does not imply runtime semantic
completion. Concrete native main queries779ff0/77a180 select seven defense/counter
values using clamp(3+unit168c-unit1688,0,6), scan64 slots for the first object
matching words68==4/6c==0, and return float0 if none. Other context cleanup side
effects and invalid-state assumptions documented. Equal modifiers select16 and3;
neither is claimed measured rendered frames. Partial update7225c4 loads these
rows and subtracts a runtime binary32 decrement from timers, disproving a basis
for blindly assuming decrement1. Modifier writers, timer clock and full input/
impact/reset gates remain unrecovered.

Tools switch_package_inventory.py/switch_table_evidence.py and4 PFS0 tests were
already present in commit dfc276e after the interrupted first part of this work.
Added switch_nso_evidence.py to validate the reviewed build,11 instruction anchors,
3 code-range hashes and generate the inspection ELF. Public tool snapshots pinned:
hactool1d64a83450e025622f3468c28fc4164dad2c5ef6 (ISC),
Unsimplifiere2bf56c22ae527560602a9b063bccb1bf03dac16 (Unlicense).
Assets/provenance recorded in ASSETS.md. All extracts/decompressed originals,
reports, source snapshots and disassembly remain ignored under local-assets and
artifacts; native runtime unchanged by this research.

Validation: make test passed the portable core and36 Python tests; make all
exited0/up to date. Table report regenerated identically with cmp. Native report
regenerated identically with cmp;11 anchors and3 range fingerprints pass.
git diff --check passed. These validate tooling and the current shared worktree,
not gameplay equivalence, complete mechanics, an isolated checkout or hardware.

Before static-only steering, isolated Ryujinx1.3.3 showed RYU-0002 firmware missing
and later RYU-0001 keys-not-found during installer attempts. No game execution was
obtained. User supplied Firmware.23.0.1.zip but successful installation was not
established. Normal user profile/saves unchanged. No emulator launched after the
static-only correction; a scoped cleanup check found no remaining research
Ryujinx process. Do not keep troubleshooting firmware under this instruction.

Next static work:
1. Trace main7225c4's timer decrement producer (+4a78 via pointer slotf0c0f0),
   input production and full impact-check candidate723bd8. Recover modifier1688/
   168c writers and script registration names; then document bounded defense
   algorithm instead of assigning frame/second durations prematurely.
2. Trace player/jump table consumers to recover units, clock, integrator, release,
   mode gates and collision order. Do not substitute GameCube motion equations.
3. Obtain/recover a compatible Switch script-bytecode decoder and native API
   registry; inventory story/service calls. Ascend is a discovery lead, not an
   installed/validated decoder. Preserve MSBT labels, tokens, order and speakers;
   Switch English path is msg/US_English_Final, not GameCube msg/US.
4. Continue damage/status/turns, audience/RNG, inventory/save/UI/audio/graphics,
   partners, all chapters/interludes and optional content. Current register lists
   gaps; the full documentation goal is active and not complete.

## 2026-10-12 — HP/FP regeneration and Happy badge phases

Previous goal turn progressed with36d17dc Poison/Burn contracts. Added
BATTLE_REGENERATION.md covering states0a..11: status23/24 recovery, HP display
bookkeeping attribute gate, shared FP destination, requested-amount effects,
regeneration wait event, Happy Heart/Flower per-copy draws, integer-divide-first
thresholds, local Flower probability inputs versus resolved FP owner, signed16
narrowing/capping and changed-phase wait differences. Recovered RecoverHp helper.
Happy Heart defers its wait; Flower/status regeneration recurse immediately.

Validation:236 instruction anchors, regeneration event fingerprint and prior
data checks passed against read-only US ISO. Evidence96 function fingerprints
matched a repeat byte for byte. Behavior links resolve; git diff --check passed.
make test passed core plus36 Python tests; make all exited0, no rebuild. Final
verifier rerun passed after representing the tiny event as a hash rather than
embedding its complete bytecode. No original executable/emulator run or profile/
save changes; no native runtime code authored. Concurrent Switch baseline was
committed separately asfc91c72. Original extracts/disassembly remain ignored;
ASSETS.md updated. Static evidence is not dynamic-equivalence proof.

Next: Happy/recovery event bodies and associated sound/pose callbacks, display
field consumers, remaining turn initialization/counters, unit-event waits and
conclusion/phase handling, move reset and actual death/revival scripts. Continue
full-system coverage before complete-game requirements. Full objective remains
active/incomplete; original/native execution and rendering comparisons needed.

## 2026-10-12 — Recovery/Happy event bodies and service operands

Previous turn progressed with9977046 regeneration phases. Added
BATTLE_RECOVERY_EVENTS.md: all local branches/calls in status recovery and
Happy Heart/Flower scripts, data-key8 override/null-value behavior, fallback
wait cancellation/pose/jump/start ordering, Happy pose support/status gates and
60-count wait. Recovered13 additional wrapper/helper contracts, including
special unit-ID translation, raw signed-byte status operand, pose-table support
scan and data/child dispatch. Fresh entry local0 differs from bound owner: LW10
and LW3 queries are preserved, not rewritten to self. Encoded fall acceleration
is retained exactly; public fixed-point interpretation is explicitly R evidence.
Motion, float coercion, actual pose/sound and key8 bindings remain open.

Validation:250 instruction anchors plus three script-span hashes and prior
data checks passed against read-only US ISO. Report109 function fingerprints
matched a repeat byte for byte. Behavior links resolve; git diff --check passed.
make test passed core plus36 Python tests; make all exited0, nothing to rebuild.
No runtime changes, original execution/emulator run, or profile/save changes.
Only research/provenance/tools edited; original scripts/disassembly remain
ignored. ASSETS.md records scope. Hashes prove byte identity, not interpretation
or dynamic equivalence of a replacement.

Next: JumpPosition and float resolution, pose/sound implementations, data-key8
bindings; move reset and remaining unit-event waits/conclusion handling; outer
turn/phase/action cadence and actual death/revival event scripts. Continue
full-system coverage before complete-game requirements. Full objective remains
active/incomplete; original/native execution and rendering comparisons required.

## 2026-10-12 — Unit-event barriers, reaction queue and conclusion predicate

Previous turn progressed with9317c39 recovery event scripts. Added
BATTLE_WAITS_AND_CONCLUSION.md: full unit-handle scan/cleanup, outer barrier,
64-pair reaction initialization/insertion/all-match compaction/dispatch,
missing-unit recursion, allocation failure readiness, four reservation slots
and consumption before conclusion, three-group configuration scans/result
writes and final forced-flag predicate. Duplicate unit insertion ignores a
new key; unit barrier visits all handles despite an earlier live event.
Queue/configuration/reservation producers and victory/defeat execution remain
open. Turn recovery's obsolete helper boundaries now link the new contract.

Validation:266 instruction anchors and prior table/constant/script checks passed
against read-only US ISO. Evidence117 function fingerprints reproduced exactly.
Behavior links resolve and git diff --check passed. make test passed core plus36
Python tests; make all exited0, no rebuild. Workspace started clean; only research
files edited. No runtime code authored, original executable/emulator run, or
profile/save modification. Extracts/disassembly/reports remain ignored; ASSETS.md
updated. Static anchors/fingerprints do not prove dynamic equivalence.

Next: queue/configuration/reservation writers and pouchRemoveItem semantics,
conclusion sequence/rewards, move reset and outer turn/phase/action cadence;
JumpPosition, float/pose services and actual death/revival scripts. Continue all
systems before deriving complete-game requirements. Full objective remains
active/incomplete; original/native traces and rendering comparisons required.

## 2026-10-12 — Inventory removal, reservation insertion and refunds

Previous turn progressed with729928b barriers/reaction queue. Added
INVENTORY_REMOVAL_AND_RESERVATIONS.md: complete local pouchRemoveItem and indexed
variant, array/capacity selection, first-match ordered compaction with badge
parallel array, unchecked index read order, reservation callback kind/item gates,
slotwise duplicate/empty checks, independent binary32 refund path and held-item
151 cleanup. Reservation removal now links exact inventory semantics. The
reservation writer was located through DOL immediate-operand scan; that scan
is a candidate locator, not a complete writer/caller census.

Validation:278 instruction anchors and two refund constants plus prior checks
passed against read-only US ISO. Evidence120 function fingerprints reproduced
byte-identically. Behavior links resolve and git diff --check passed. make test
passed core plus36 Python tests; make all exited0, no rebuild. Workspace started
clean; no runtime code authored, original executable/emulator run or profile/
save changes. Extracts/disassembly/reports remain ignored; ASSETS.md updated.
Static checks do not establish dynamic-equivalence or legal-call reachability
of indexed boundary cases.

Next: all reservation/consumption callers, item execution/cancellation and badge
parallel-array consumers; queue/configuration producers, conclusion/rewards,
move reset and outer turn/action cadence; motion/float/pose services and actual
death/revival event scripts. Continue full-system documentation before complete-
game requirements. Objective remains active/incomplete; original/native traces
and rendering comparisons still required.

## 2026-10-12 — Move-count reset and action/guard predicates

Previous goal turn progressed withf448f56 inventory/reservation contracts. Added
BATTLE_MOVE_COUNTS.md: Fast byte doubling/narrowing, Slow signed division and
low-count thresholds, reset snapshots/zero fallback, conditional setter reset,
max-count callback without reset, act/guard predicates and turn-first reset/
priority caller segment. DOL direct-call inventory identifies three reset sites.
Turn-first increments+24 before resetting all nonnull units; other+24/count
writers and outer cadence remain unrecovered. Null CanActStatus returns1 by
helper composition, without claiming a missing unit can act in the controller.

Validation:290 instruction anchors plus all prior checks passed read-only US ISO.
Evidence126 function fingerprints reproduced byte-identically; three direct
reset link sites checked. Behavior links resolve; git diff --check passed.
make test passed core plus36 Python tests; make all exited0, nothing to rebuild.
Workspace started clean; only research/tool metadata changed. No original code
execution/emulator run or profile/save modifications. Extracts/disassembly/reports
remain ignored; ASSETS.md updated. Static identity/anchors do not prove a native
implementation equivalent or establish complete Slow turn cadence.

Next: count consumption and+20/+23/+24 writers/consumers, full turn-first service
ordering and outer phase/action scheduler; queue/configuration/item lifecycle
callers, conclusion/rewards and actual death/revival scripts; motion/float/pose
services. Continue all systems before complete-game requirements. Objective
active/incomplete; original/native execution/rendering comparisons still needed.

## 2026-10-12 — Phase tables, actor ordering and count consumption regions

Previous turn progressed with14dddc1 move reset. Added BATTLE_PHASE_TABLES.md:
complete local phase-first table construction, stable absolute-home-X then
priority sort, separate slot/side-key phase-event table, phase wrapper and
phase-end predicate. Documented selected btlseqMove actor-selection and
btlseqAct count-consumption regions, including state normalization, one-use
count bypass, forced exhaustion and+24 reset/preserve flag. The full move/act
controllers remain explicitly outside detailed coverage despite fingerprinting.
This adds evidence toward Slow cadence without assuming all state producers.

Validation:304 instruction anchors plus15-entry move table hash, two phase-sort
constants and all prior checks passed read-only US ISO. Evidence134 function
fingerprints matched a repeat byte-identically. Behavior links resolve; git diff
--check passed. make test passed core plus36 Python tests; make all exited0,
no rebuild. Workspace started clean; only research/provenance/tools edited.
No original execution/emulator run or profile/save modification. Original
extracts/disassembly/reports remain ignored; ASSETS.md updated. Static identity
and local checks do not prove full scheduler or native dynamic equivalence.

Next: complete btlseqMove/btlseqAct state maps, command/attack and phase-event
services; state-byte/flag producers and outer turn progression, conclusion/
rewards and actual death/revival scripts; motion/float/pose services. Continue
all systems before complete-game requirements. Objective remains active and
incomplete; original/native trace/rendering comparisons still required.

## 2026-10-12 — Complete local btlseqMove state map

Previous turn progressed withce9b8a0 phase tables/consumption regions. Added
BATTLE_MOVE_CONTROLLER.md: all15 local states, immediate loop versus explicit
fallthrough versus outer-call return, optional Majinai event, selector1 fan-out,
selector0 serial phase events, bookkeeping masks/typed clears, actor handoff,
kind222 command flags, post-action conclusion/phase-end path, audience and dual
announcement drain. Recovered SetTotalHitDamage's signed16-to-word write. Phase
table contract and index now link complete local move coverage. Full action
controller/services/outer cadence remain open; no complete battle claim.

Validation:320 instruction anchors and prior table/constant/script checks passed
against read-only US ISO. Evidence135 function fingerprints reproduced exactly.
Behavior links resolve; git diff --check passed. make test passed core plus36
Python tests; make all exited0, no rebuild. Workspace started clean; only
research/provenance/tool metadata edited. Original disassembly/report files
remain ignored. No original execution/emulator run or profile/save changes.
ASSETS.md updated. Static checks do not prove callee semantics or dynamic/native
scheduler equivalence.

Next: complete btlseqAct, command/menu/attack and Majinai services/script,
audience/announcement internals and external channel/flag writers, outer turn
progression, conclusion/rewards and death/revival scripts; motion/float/pose
services. Continue all-system register before complete-game requirements.
Objective active/incomplete; original/native execution comparisons required.

## 2026-10-12 — Complete local btlseqAct state map

Previous turn progressed withb567a35 move controller. Added
BATTLE_ACTION_CONTROLLER.md covering all15 local states: kind/status/attribute
routing, camera/command gates, alternate data-key paths, priority-ordered event
selection, retained Electric draw and Confuse threshold, wait-event deletion,
barrier/announcement fallthrough, actor cleanup/count consumption, reserve-item
consumption, audience/Break Slot gating and explicit post-event+2cc polling.
Move/phase contracts now link complete local action coverage. Callee/script
bodies and outer scheduler remain open; local coverage is not full-game fidelity.

Validation:336 instruction anchors plus action15-entry table fingerprint and
camera2.0 constant, alongside prior checks, passed read-only US ISO. Evidence135
function fingerprints reproduced byte-identically. Behavior links resolve;
git diff --check passed. make test passed core plus36 Python tests; make all
exited0, no rebuild. Workspace started clean; only research/provenance/verifier
files edited. No original execution/emulator run or profile/save modifications.
Extracts/disassembly/reports stay ignored; ASSETS.md updated. Static checks do
not prove call interpretation or native dynamic-equivalence.

Next: command/menu and camera helpers, default/status/action script bodies and
flag writers, audience/announcement/Break Slot services, outer turn/controller
cadence, conclusion/rewards and death/revival execution; motion/float/pose
services. Continue all systems before complete-game requirements. Full objective
active/incomplete; original/native trace/rendering comparisons required.

## 2026-10-12 — Default Electric/zero-gravity scripts and entry services

Previous turn progressed witha82ddfc action state map. Added
BATTLE_DEFAULT_ACTION_EVENTS.md: complete local Electric interpolation/display/
wait script and zero-gravity pose/camera/message/wait sequence, original message
ID binding, shared interpolation locals/continuation/truncation, mode8 binary32
operation order/constants, display-offset sentinel and integer wrapper, command
initialization leaf and camera-speed vector/sqrt caller. Electric's final LW10
wait target remains distinct from owner.17 samples/yields are derived call counts,
not measured frames. Other curves and cosine/sqrt library semantics remain open.

Validation:348 instruction anchors, two default-script hashes, five float
bindings, mode8 dispatch target and original message ID plus previous checks
passed read-only US ISO. Evidence142 function fingerprints reproduced exactly.
Behavior links resolve; git diff --check passed. make test passed core plus36
Python tests; make all exited0, no rebuild. Workspace started clean; only research/
provenance/tool metadata edited. No original executable/emulator run or profile/
save changes. Script/code extracts and reports remain ignored; ASSETS.md updated.
Hashes/static checks do not prove dynamic or visual replacement equivalence.

Next: camera/message callbacks and original US message/control service, full
CommandInput, display/pose services, interpolation modes and library numeric
contracts; outer battle/turn cadence, audience/announcement and conclusion/
reward/death/revival events. Continue all systems before complete-game system
requirements. Objective active/incomplete; original/native traces still needed.

## 2026-10-12 — Camera request wrappers and setter semantics

Previous turn progressed with18dc31c default action scripts. Added
BATTLE_CAMERA_REQUESTS.md: mode/speed/zoom/homing callbacks, signed admission
threshold, mode reset and independently gated axis-reset calls, invalid speed
fallback, event zoom truncation versus direct float setter, first home versus
second hit-position cache, ignored first homing operand and−1 preserving second
target. Derived zero-gravity request outcomes retain these differences. Main
camera modes, producers and rendering remain explicitly open.

Validation:364 instruction anchors, mode-reset0.0 constant and prior checks
passed read-only US ISO. Evidence152 function fingerprints reproduced exactly.
Behavior links resolve; git diff --check passed. make test passed core plus36
Python tests; make all exited0, no rebuild. Workspace started clean; only research,
provenance and evidence tooling edited. No original execution/emulator run or
profile/save changes. Extracts/disassembly/reports remain ignored; ASSETS.md
updated. Static evidence does not establish actual camera shots or native fidelity.

Next: original message callback/control service, main camera update modes and
priority/vector producers, full CommandInput; audience/announcement services,
outer battle/turn progression, conclusion/rewards and death/revival events;
motion/float/pose services. Continue all systems before complete-game requirements.
Full objective active/incomplete; original/native execution comparisons needed.

## 2026-10-12 — Battle announcement timer, shared text and draw callback

Previous turn progressed with5a231ae camera requests. Added
BATTLE_ANNOUNCEMENT_MESSAGES.md: five resolved operands, first-call-only text/
position/timer initialization, mode1 original item-name branches, unchecked
parameter setter, per-invocation display registration, mode2 trigger gate,
post-decrement word/yield protocol and local draw geometry. Per-event timer and
shared global text/position ownership remain distinct. Added namespace-qualified
symbol selection in evidence tooling to identify battle_message.o's local _disp.
Original story text is not copied or rewritten; lookup/control semantics remain
open, along with unknown substitution helper and renderer/display-list lifetime.

Validation:380 instruction anchors, four announcement floats/window-color word
and prior checks passed read-only US ISO. Evidence157 fingerprints reproduced
exactly; qualified draw address/size checked explicitly. Behavior links resolve;
git diff --check passed. make test passed core plus36 Python tests; make all
exited0, no rebuild. Workspace started clean; only research/provenance/evidence
files changed. No original execution/emulator run or profile/save modification.
Original extracts/disassembly/reports stay ignored; ASSETS.md updated. Static
checks do not prove actual message timing, text rendering or native equivalence.

Next: msgSearch resources/fallback/encoding and exact US control-token service,
substitution helper, font/window/display lifetime, announcement parameters and
caller census; camera modes/producers, full command input, outer battle/turn
cadence and conclusion/reward/death/revival events. Continue all systems before
complete-game requirements. Objective active/incomplete; original/native traces
and rendering comparisons needed.

## 2026-10-12 — US message resources, loading and lookup

Previous turn progressed with3561cd8 announcement messages. Added
MESSAGE_RESOURCES_AND_LOOKUP.md: two-slot resource records, seven language paths,
slot1 maximum-language allocation, slot0 map allocation, retained-buffer failure
path, pair-index sorting, exact slot precedence and binary-search bounds,
localized fallback, 128-byte command split and language-dependent lead predicate.
Duplicate ordering, complete control dispatch, glyph decoding, caller lifetimes
and story conditions remain explicitly open. No original dialogue is rewritten.

Added read-only message_resource_evidence.py. Census of260 US files found13,186
key/text pairs, two duplicate key occurrences, zero unmatched keys/nonzero tails
and4,366 high bytes. Metadata retains offsets, raw key hex and text hashes;
literal tag lexemes are not interpreted controls. Reports and original extracts
remain ignored. No assumed UTF-8 normalization or dialogue export is committed.

Validation:392 instruction anchors and prior constants/tables/script checks
passed against the read-only US ISO. Evidence162 function fingerprints and full
message census each reproduced byte-for-byte in a second report. Five bounded
parser cases and all behavior document links passed. make test passed core plus36
Python tests; make all exited0, no rebuild; git diff --check passed. Workspace
started clean; only research/provenance/evidence tooling changed. No original
execution, emulator run or profile/save changes. Static checks do not establish
original text rendering, timing or native-game equivalence.

Next: msgAnalize command dispatch/variables/branches, font byte-to-glyph mapping,
resource initialization/release/callers and original qsort duplicate ordering;
story attribution/order/event conditions, substitution and display lifetime.
Continue camera/input/outer battle and other field/content/save services before
complete-game requirements. Full objective active/incomplete; original/native
execution comparisons still needed.

## 2026-10-12 — Message parser layout, control records and initialization

Previous turn progressed with8e00ce7 resource lookup. Added
MESSAGE_PARSER_RECORDS.md from the US msgAnalize binary: append-only 24-byte
record layout, terminal record outside the count, persisted coordinates and
local style/speed state, US versus Japanese glyph paths, line rounding,
post-command LF suppression, waits/style/color/audio controls, window types,
positioning and selection setup. Recorded the five-conversion/four-prepared-
destination sscanf ambiguity rather than inventing a safe grammar. Icon/pose/
annotation branches and consumers remain explicitly unfinished. Extended lookup
documentation with msgInit's original key-wait pose setup and global slot load.
Original dialogue and story conditions are not rewritten or inferred.

Validation:408 instruction anchors and prior checks passed against read-only
US ISO, plus two command/format block hashes, layout floats/colors and pose
bindings. An initially mistyped anchor address was corrected to the inspected
instruction at0x8008204c before the passing run. Evidence164 fingerprints
reproduced byte-for-byte. All behavior document links resolve; git diff --check
passed. make test passed core plus36 Python tests; make all exited0, no rebuild.
Workspace started clean; only research/provenance/evidence tooling changed.
No original execution/emulator run or profile/save modification. Original
extracts/disassembly and reports remain ignored. Static checks do not prove
consumer timing, rendered dialogue or complete-game equivalence.

Next: remaining icon/anim/paper/annotation parser paths, font glyph/width tables,
msgMain record consumption and window/key/audio/pose lifetimes; resolve select
format ABI against original resource data. Continue story attribution/order/
conditions and remaining camera/input/outer battle/field/content/save systems
before complete-game requirements. Full objective active/incomplete.

## 2026-10-12 — Embedded message objects and original control shapes

Previous turn progressed withcc0a578 parser records. Added
MESSAGE_EMBEDDED_OBJECTS.md: all18 US icon name/ID bindings, numeric/name/HM
routing, fused icon geometry and independent advance, animation pose setup
including global-versus-new callback ID, paper NPC records and annotation
anchor/gates/centering/halfword rules. Literal one-byte-versus0x2f72 comparison
makes the apparent closing annotation gate unreachable. Local msgAnalize
branches are now covered across two files; services/consumers remain incomplete.

Census schema2 now records selected control argument shapes. US resources have
394 icon tags, five anim tags and280 select tags, each select with four lexical
fields:278 all-integer, two containing a %d placeholder. No paper/r/R-prefix
lexemes were found in this bounded US scan. This narrows ordinary resource
selection shape without asserting substituted runtime values, library failure
behavior or absence of generated/non-US inputs. Dialogue remains reused data.

Validation:424 instruction anchors and prior checks passed read-only US ISO,
plus original icon table hash/all18 string-ID bindings and geometry constants.
Corrected a mistyped icon-advance anchor to inspected0x80082cbc before passing.
Evidence165 fingerprints and census schema2 each reproduced byte-for-byte;
aggregate resource counts/shapes were independently asserted. Eight synthetic
control-shape categories and all behavior links passed. make test passed core
plus36 Python tests; make all exited0, no rebuild; git diff --check passed.
Workspace started clean; only research/provenance/evidence tooling changed.
No original execution/emulator run or profile/save changes. Original code/data
extracts and evidence reports remain ignored. Static anchors do not prove visual
geometry, pose lifetime or original/native behavior equivalence.

Next: msgMain record playback and window/key/audio/pose lifetimes; font glyph
mapping/width data and icon dimensions; substitution callers for selection
placeholders. Continue story attribution/order/conditions and the remaining
battle/camera/input/field/content/save systems before complete-game requirements.
Full objective remains active/incomplete; executed-original comparisons needed.

## 2026-10-12 — Message playback and record admission

Previous turn progressed withad14ca1 embedded objects. Added MESSAGE_PLAYBACK.md:
complete local msgMain branch/14-opcode map, clock quotient/low-word schedule
advancement, two separately rounded scroll additions, input masks and latched
acceleration, wait-state/resume return codes, callbacks/history, terminal outside
parser count, unchecked schedule admission and paper-end index/pointer divergence.
Documented paper pose/group bindings and generic-record sound admission once per
invocation, including color/noise records and refreshed-versus-cached handle data.
No delays/rates are claimed as measured. Caller/state producers remain open.

Validation:440 instruction anchors and prior checks passed against read-only
US ISO, plus all14 playback targets, scroll coefficient and seven paper pose/
group bindings. Evidence167 function fingerprints reproduced byte-for-byte.
All behavior links resolve; git diff --check passed. make test passed core plus36
Python tests; make all exited0, no rebuild. Workspace started clean; only
research/provenance/evidence tooling changed. No original execution/emulator run
or profile/save modification. Code/data extracts and reports remain ignored.
Static checks do not establish clock/callback cadence, renderer fidelity or
original/native dialogue behavior equivalence.

Next: message creation/event callbacks and wait/input state producers, render/
selection/window lifetimes; clock snapshots and numeric helpers; font/icon/pose/
NPC/sound dependencies. Continue full story attribution/order/conditions and
remaining battle/camera/input/field/content/save systems before complete-game
requirements. Full objective active/incomplete; executed comparisons needed.

## 2026-10-12 — General/clear message states and history controls

Previous turn progressed withba1d207 selection. Added MESSAGE_WINDOW_STATES.md:
all12 general dispatch targets, talk countdown before dispatch, type-indexed
opening/closing sounds, integer elapsed fade/narrowing, retained stop-state
poll completion, wait-state resume/acceleration gating, history count-2 entry,
signed target scroll and ordered navigation. Recovered separate clear-state
behavior, playback during opening, no local wait input and forced-versus-terminal
closing timestamp differences. Renderer and event/callback producers remain open.

Validation:496 instruction anchors and prior checks passed read-only US ISO,
plus all12 general dispatch targets,24 sound table values and history binary32
coefficient. Evidence188 fingerprints reproduced byte-for-byte. All behavior
links resolve; git diff --check passed. make test passed core plus36 Python tests;
make all exited0, no rebuild. Workspace started clean; only research/provenance/
evidence tooling changed. No original execution/emulator run or profile/save
changes. Original extracts/reports remain ignored. Static checks do not prove
measured fades/history/input cadence, presentation or original/native equivalence.

Next: message/selection/key-wait rendering and manager scheduling; event print
identity/callback/substitution producers; pose/font/icon/audio services and clock/
input numeric dependencies. Continue full story attribution/order/conditions and
all remaining systems before complete-game requirements. Objective remains
active/incomplete; executed-original comparisons needed.

## 2026-10-12 — Message window creation and lifecycle controls

Previous turn progressed with2f17da5 playback. Added MESSAGE_WINDOW_LIFECYCLE.md:
seven-slot first-free pool and allocation/poll flags, fixed work zeroing and
creation timestamps/defaults, temporary strlen-sized strcpy allocation,
type/wpos/selection/small geometry, append over terminal, Continue/Repeat partial
reset, deletion's empty record-count loop, identity-based force-close/NPC versus
battle stay restoration and first-call event repeat/continue polling. Normal
constructor zeroing explains untouched parser schedule fields; allocator slack,
callback identity and main-state completion remain explicit dependencies.

Validation:456 instruction anchors and prior checks passed against read-only
US ISO, plus four default geometry floats and small-window multiplier.
Evidence179 function fingerprints reproduced byte-for-byte. All behavior links
resolve; git diff --check passed. make test passed core plus36 Python tests;
make all exited0, no rebuild. Workspace started clean; only research/provenance/
evidence tooling changed. No original execution/emulator run or profile/save
changes. Code/data extracts and reports remain ignored. Static checks do not
prove allocator failure behavior, window timing or original/native equivalence.

Next: msgWindow_Main/Clear/selection state machines, input wait-state transitions
and renderer; message event print/identity/callback producers and substitution;
allocator/embedded-pose ownership, font/icon/sound services. Continue story
attribution/order/conditions and all remaining battle/camera/input/field/content/
save systems before complete-game requirements. Full objective active/incomplete.

## 2026-10-12 — Selection window input and event result lifetime

Previous turn progressed with49af82c lifecycle. Added MESSAGE_SELECTION.md:
unconditional playback before selection state dispatch, strict opening clamp,
extra zero-opacity closing invocation, trigger wrap versus repeat endpoint guards,
independent opposite-direction blocks/net movement sound, cancel-before-confirm
including disabled-cancel precedence, signed cancel results, display registration,
first-call direct/key identity creation, saved +17c window index and signed local
work0 result before deletion. Recovered the three literal input snapshot readers
and both guarded-ID/unguarded-pointer window deletion helpers. Renderer, input
repeat producers/cadence and story caller/branch mapping remain explicitly open.

Validation:472 instruction anchors and prior checks passed read-only US ISO,
plus selection display-order base and original direct-input identity.
Evidence186 function fingerprints reproduced byte-for-byte. All behavior links
resolve; git diff --check passed. make test passed core plus36 Python tests;
make all exited0, no rebuild. Workspace started clean; only research/provenance/
evidence tooling changed. No original execution/emulator run or profile/save
changes. Original extracts and evidence reports remain ignored. Static checks
do not establish actual navigation cadence/highlighting or native equivalence.

Next: general/clear message window states and wait/input transitions; selection/
message renderer, event print callback/identity/substitution producers; clock and
input snapshot cadence, font/icon/pose/NPC/audio dependencies. Continue story
attribution/order/conditions and all remaining systems before complete-game
requirements. Full objective active/incomplete; executed comparisons needed.

## 2026-10-12 — Common print event identity and speaker placement

Previous turn progressed with9aa89de general/clear states. Added
MESSAGE_PRINT_EVENTS.md: four resolved operands, first-call-only window creation,
direct/key identity and event ownership, consumed layer/placement words,
event NPC owner versus named override, ten special-name/sentinel/relationship
routes, partner fallback/Mario/NPC anchors, battle speaker fields and projected
placement/ordered overrides. Preserved literal0x1effffff mask and completion on
poll flag rather than allocation/deletion. Caller story conditions and helper
services remain explicit gaps; original dialogue is not rewritten.

Validation:512 instruction anchors and prior checks passed read-only US ISO,
plus projected threshold and all ten special NPC strings. Evidence191 function
fingerprints reproduced byte-for-byte. All behavior links resolve; git diff
--check passed. make test passed core plus36 Python tests; make all exited0,
no rebuild. Workspace started clean; only research/provenance/evidence tooling
changed. No original execution/emulator run or profile/save modification.
Original extracts/reports stay ignored. Static checks do not prove actual speaker
attribution in every scene, projection rendering or original/native equivalence.

Next: print partner/insert/add variants, placement/layer producers and substitution
callbacks; renderer/key-wait indicator/manager cadence and speaker/font/icon/pose/
audio dependencies. Continue original story call-graph attribution/order/branches/
conditions and all other systems before complete-game requirements. Full objective
active/incomplete; executed-original comparisons still needed.

## 2026-10-12 — Partner print variants and placement setters

Previous turn progressed with2713628 common print. Added MESSAGE_PRINT_VARIANTS.md:
NPC name-only mutation, pending layer, accumulated orientation flags and literal
preserved placement mask/float operands; all11 field/seven battle suffix bindings,
signed field index versus e0-based battle kind, three distinct64-byte buffers,
variant first-call gates/shared identity lifetime, resumed null battle-unit early
completion and direct/key generic append. Original keys/text are retained; suffix
character/caller attribution and insertion substitutions remain open.

Validation:528 instruction anchors and prior checks passed read-only US ISO,
plus all18 suffix bindings and original field speaker literal. Evidence199
function fingerprints reproduced byte-for-byte. All behavior links resolve;
git diff --check passed. make test passed core plus36 Python tests; make all
exited0, no rebuild. Workspace started clean; only research/provenance/evidence
tooling changed. No original execution/emulator run or profile/save changes.
Original extracts/reports remain ignored. Static checks do not prove concurrent
buffer ownership, story branch attribution or original/native equivalence.

Next: insert/add-insert substitutions and callback bodies; partner/battle key
caller census and character mappings; renderer/key-wait indicator/manager cadence
and service dependencies. Continue full story attribution/order/branches/conditions
and all remaining systems before complete-game requirements. Full objective
active/incomplete; executed-original comparisons still needed.

## 2026-10-12 — Insert formatting callbacks and percent lexeme census

Previous turn progressed withaa1e5d4 variants. Added MESSAGE_INSERT_FORMATTING.md:
signed encoded insertion count, fixed/resumed operand resolution, one-through-four
word-register sprintf dispatch, raw/key format source, unchecked transient output,
no zero-count copy path, forced-direct new-window identity and first-call-only
append consumer. Original printf ABI/call-site types remain explicitly open.
No general token replacement behavior is inferred from these wrappers.

Census schema3 adds per-pair/aggregate printf-like metadata:93 US pairs/119 matches,
including one prose-like percent match. Lexical counts are not formatting-call
or story execution evidence; no dialogue/format text is rewritten or exported.

Validation:544 instruction anchors and prior checks passed read-only US ISO.
Evidence201 fingerprints and census schema3 each reproduced byte-for-byte.
Six synthetic lexical categories, complete US aggregate counts and all behavior
links passed; git diff --check passed. make test passed core plus36 Python tests;
make all exited0, no rebuild. Workspace started clean; only research/provenance/
evidence tooling changed. No original execution/emulator run or profile/save
changes. Original extracts/reports remain ignored. Static checks do not prove
printf/library equivalence, valid insertion callers or native dialogue behavior.

Next: remaining locale/replacement helpers/callback bodies and original sprintf
library route; insert format/type/count call-site census; renderer/manager cadence
and service dependencies. Continue original story attribution/order/branches/
conditions and all remaining systems before complete-game requirements. Full
objective active/incomplete; executed-original comparisons still needed.

## 2026-10-12 — Original format output and varargs consumers

Previous turn progressed with1fa2d28 insertion wrappers. Added FORMAT_OUTPUT_RUNTIME.md:
sprintf/vsprintf register-save setup and return-index termination (including-1),
string sink clipping/requested-versus-copied counts, varargs general/floating/
64-bit slot and overflow alignment, formatter literal streaming/consumer dispatch,
string precision/length-prefix/empty-null behavior, n stores, padding/sign order
and literal-tail fallback. Grammar/numeric/wide conversion helpers and actual
call-site validity remain explicitly open. No host-libc equivalence is assumed.

Validation:560 instruction anchors and prior checks passed read-only US ISO,
plus original narrow-null string's empty sentinel. Evidence206 fingerprints
reproduced byte-for-byte. All behavior links resolve; git diff --check passed.
make test passed core plus36 Python tests; make all exited0, no rebuild.
Workspace started clean; only research/provenance/evidence tooling changed.
No original execution/emulator run or profile/save modification. Code/data
extracts/reports remain ignored. Static checks do not prove library execution,
valid formatting callers or original/native dialogue equivalence.

Next: printf parse_format grammar and integer/decimal/hex-float helpers, wide
conversion/memory services; replacement/callback bodies and insertion call-site
census; renderer/manager cadence and original story attribution/order/branches/
conditions. Continue all remaining systems before complete-game requirements.
Full objective active/incomplete; executed-original comparisons needed.

## 2026-10-12 — Original printf directive grammar and normalization

Previous turn progressed withf1ccbdc output/varargs. Added FORMAT_DIRECTIVE_PARSER.md:
complete local repeated flag precedence, dynamic/digit width and509 cap,
precision-presence/negative-star/word-wrap rules, h/hh/l/ll/L enums, complete
conversion admission/normalization map and pointer formatting. Preserved unusual
lc/hhn consumer interactions and argument consumption before later rejection.
Corrected format-record byte3 from string-only naming to alternate form, including
its existing length-prefixed string interpretation. Numeric converters remain open.

Validation:576 instruction anchors and prior checks passed read-only US ISO,
plus both parser dispatch hashes and byte-classification hash/ASCII-digit mask.
Evidence207 fingerprints reproduced byte-for-byte; qualified printf address/size
verified explicitly against the report, avoiding scanf's same local symbol name.
All behavior links resolve; git diff --check passed. make test passed core plus36
Python tests; make all exited0, no rebuild. Workspace started clean; only research/
provenance/evidence tooling changed. No original execution/emulator run or profile/
save changes. Original extracts/reports remain ignored. Static checks do not prove
executed formatting/call-site validity or original/native equivalence.

Next: integer/decimal/hex-float conversion algorithms and wide/memory services,
format/type/count caller census, replacement/callback bodies and rendering/manager
cadence. Continue original story attribution/order/branches/conditions and all
remaining systems before complete-game requirements. Full objective active/
incomplete; executed-original comparisons still needed.

## 2026-10-12 — Original integer text conversion stages

Previous turn progressed withc0e0499 directive parser. Added FORMAT_INTEGER_CONVERSION.md:
32/64-bit ABI, NUL/reverse storage, zero-precision early return, signed minimum
modulo negation/unsigned division, base/sign-policy cases, digits/case, alternate
octal counting, mode2 width-to-precision mutation, literal capacity expression,
unconditional admitted hex prefixes and final sign order. Caller-copy mutations
and helper-failure literal-tail behavior remain distinct. Floating/wide/helper
arithmetic execution and original formatting caller census remain open.

Validation:592 instruction anchors and prior checks passed read-only US ISO,
plus both33-entry integer dispatch-table hashes. Evidence209 fingerprints
reproduced byte-for-byte. All behavior links resolve; git diff --check passed.
make test passed core plus36 Python tests; make all exited0, no rebuild.
Workspace started clean; only research/provenance/evidence tooling changed.
No original execution/emulator run or profile/save modification. Original
extracts/reports remain ignored. Static checks do not prove executed formatting,
valid caller ranges or original/native equivalence.

Next: decimal/hex-float conversion and rounding, unsigned64 and wide/memory
services; actual format/type/count caller census, replacement/callback bodies,
renderer/manager cadence and original story attribution/order/branches/conditions.
Continue all remaining systems before complete-game requirements. Full objective
active/incomplete; executed-original comparisons needed.

## 2026-10-12 — Decimal record rounding and preparation

Previous turn progressed withc66da66 integer conversion. Added FORMAT_DECIMAL_ROUNDING.md:
record ownership/count/ASCII digits, negative/no-op retention gates, discarded-tail
ties, zero-retention count-byte parity, carry propagation/logical trimming,
canonical zero/exponent wrap and sign retention. Recorded float2str entry cap,
32-digit generation request, trailing-zero preparation and original signed/case
infinity/NaN string routes. Full finite layout and binary decimal generation
remain explicit gaps; derived record examples are not executed original tests.

Validation:608 instruction anchors and prior checks passed read-only US ISO,
plus special-value string-block hash, binary64 sign-comparison zero and uppercase
classification. Evidence211 fingerprints reproduced byte-for-byte. All behavior
links resolve; git diff --check passed. make test passed core plus36 Python tests;
make all exited0, no rebuild. Workspace started clean; only research/provenance/
evidence tooling changed. No original execution/emulator run or profile/save
changes. Original extracts/reports remain ignored. Static checks do not prove
binary decimal generation, executed tie cases or original/native equivalence.

Next: finite float2str layout/mode selection/capacity, __num2dec generation and
hex-float output, runtime helper/wide/memory contracts; actual format/type/count
call-site census, replacement/callback bodies, renderer/manager cadence and story
attribution/order/branches/conditions. Continue all remaining systems before
complete-game requirements. Full objective active/incomplete; executed-original
comparisons needed.

## 2026-10-12 — Finite decimal formatting layout

Previous turn progressed withd4949a0 rounding/preparation. Added
FORMAT_FINITE_DECIMAL_LAYOUT.md: leading-exponent normalization, post-round g/G
mode boundaries and alternate precision rewriting, scientific exponent/mantissa
storage, fixed retention budget/rounding/digit-zero placement, sign policy,
caller-copy mutations and distinct capacity predicates. Kept fixed admission's
omission of requested trailing-zero bytes explicit, with derived record shape
rather than an asserted executed overrun or supported caller. Binary generation
and hex-float/service/caller contracts remain open.

Validation:624 instruction anchors and prior checks passed read-only US ISO.
Fresh evidence211 fingerprint report matched the previous repeat report exactly.
All behavior links resolve; git diff --check passed. make test passed core plus36
Python tests; make all exited0, no rebuild. Workspace started clean; only research/
provenance/verifier tooling changed. No original execution/emulator run or profile/
save changes. Original extracts/reports remain ignored. Static checks do not prove
generated decimal record invariants, executed output/capacity or native equivalence.

Next: __num2dec binary generation, hex-float output and runtime/wide/memory services;
actual format/type/count callers, replacement/callback bodies, rendering/manager
cadence and original story attribution/order/branches/conditions. Continue all
remaining systems before complete-game requirements. Full objective active/
incomplete; executed-original comparisons needed.

## 2026-10-12 — Binary decimal entry and numeric records

Added FORMAT_BINARY_DECIMAL_ENTRY.md for __num2dec and __num2dec_internal:
numeric versus ASCII record phases, special-value bypass, signed request cap,
positive-retention ties/carry/padding, exponent conversion, raw sign and signed
zero, finite mantissa trailing-bit count, helper ordering, integer decimal
reversal and final sign overwrite. Power-of-two decimal, multiplication and
runtime numeric helper internals remain explicit dependencies. Conventional
significand factorization is labeled inference pending those helper contracts.

Validation:640 instruction anchors and prior checks passed against the read-only
US ISO, including the distinct binary decimal input-zero constant. Two fresh
213-function fingerprint reports matched byte-for-byte. All local behavior
documentation links resolve; git diff --check passed. make test passed core plus
36 Python tests; make all exited0, no rebuild. Only research/provenance/evidence
tooling changed. No original execution/emulator run or profile/save modification.
Original extracts/reports remain ignored. Static checks do not establish complete
binary decimal generation, executed caller behavior or original/native equivalence.

Next: numeric decimal multiplication and power-of-two generation; frexp/ldexp/
modf and integer conversion/division contracts; hex-float and remaining formatting
services. Continue original call-site, renderer, story and other system recovery
before complete-game requirements. Full objective active/incomplete; executed
original comparisons still required.

## 2026-10-12 — Numeric decimal multiplication

After b7934cc, added FORMAT_DECIMAL_MULTIPLICATION.md for __timesdec: numeric
record domain/sign0, coefficient traversal and unsigned quotient/remainder carry,
scratch order, leading exponent, 36-digit copy, complete discarded-tail tie scan,
retained parity and count-preserving carry. Recorded lack of trimming/canonical
zero and unchecked scratch-count domain. Updated binary entry cross-links to
separate the now-recovered multiplication from open power-generation invariants.
Examples are static derivations, not executed original traces.

Validation:656 instruction anchors and prior checks passed against read-only US
ISO, including both halves of the quotient reciprocal constant. Fresh214-function
evidence reports matched byte-for-byte. All local behavior links resolve; git
diff --check passed. make test passed core plus36 Python tests; make all exited0,
no rebuild. Only research/provenance/evidence tooling changed. No original game
execution/emulator run or profile/save changes; original extracts/reports remain
ignored. Static verification does not prove valid power records, executed tie
cases/capacity or original/native equivalence.

Next: __two_exp table/recursion and numeric runtime services, then hex-float,
remaining formatter services/caller corpus; continue renderer, story and all
other missing original systems before complete-game requirements. Full objective
active/incomplete; original execution comparisons still needed.

## 2026-10-12 — Decimal powers of two

Previous goal turn made progress through b7934cc and480f34c. This turn began
clean and added FORMAT_DECIMAL_POWERS.md for __two_exp:73-entry sparse dispatch,
21 literal seed bindings, numeric record copy, original signed ASCII-byte versus
numeric5 comparison, both long-seed increments, identical seed-copy bodies,
signed halving, recursive square/odd correction,42-byte copy and per-step rounding.
Recovered power digit/count bounds establish normal combined multiplication
scratch<=56 digits, conditional on a valid integer record. Updated entry and
multiplication dependencies; numeric services/caller power range remain open.
The-53 seed's discarded ASCII '2' still increments; this static consequence is
explicitly distinguished from an executed-original trace or modern nearest seed.

Validation:672 instruction anchors and all prior checks passed read-only US ISO.
Independent verifier checks all73 targets,21 exponent/string bindings, mathematical
source digit identities and184-byte seed-only block hash. Inspected all21 seed
copy/round instruction bodies: identical after common-epilogue branch normalization.
Fresh215-function evidence reports matched byte-for-byte. All247 local behavior
links resolve; git diff --check passed. make test passed core plus36 Python tests;
make all exited0, no rebuild. Only research/provenance/evidence tooling changed.
No original execution/emulator run or profile/save change; original instruction/
seed inspection outputs remain ignored. Static checks do not prove executed
accuracy, original caller domain or full conversion/native equivalence.

Next: frexp/ldexp/modf and unsigned-double/integer division services to resolve
finite conversion power range; hex-float, remaining formatter services and actual
caller corpus. Continue renderer, story and other uncovered original systems
before complete-game requirements. Full objective active/incomplete; original
execution comparisons still needed.

## 2026-10-12 — Binary64 services and finite generator bounds

Previous goal turn made progress through8e2c774. Began clean and added
FORMAT_BINARY_NUMERIC_SERVICES.md: full local frexp/ldexp/modf/copysign and
__cvt_dbl_usll contracts. Recorded subnormal normalization, exponent writes,
wrapped scale gates and floating overflow/underflow operations, integral masks,
raw signed-zero fractional return for NaN/infinity, and converter signed-range
saturation despite symbol naming. Recovered helpers establish positive odd
significand<=2^53-1 and finite generator power[-1074,1023]. Updated prior entry/
power/multiplication dependencies. Floating environment and division remain open.

Validation:696 instruction anchors and all prior checks passed read-only US ISO,
including six binary64 normalization/scaling constants. Fresh220-function evidence
reports matched byte-for-byte. Independent integer bit-position enumeration of
109816 normal/subnormal exponent/trailing-position classes confirmed exponent
[-1073,1024], width[1,53], power[-1074,1023]; this is a derived-domain check, not
an original execution or exhaustive input trace. All local behavior links resolve;
git diff --check passed. make test passed core plus36 Python tests; make all
exited0, no rebuild. Only research/provenance/evidence tooling changed. No original
execution/emulator run or profile/save modification. Original disassemblies and
reports remain ignored. Static evidence does not establish FPSCR policy, executed
special cases/output accuracy or original/native equivalence.

Next: unsigned64 division/remainder, hex-float and remaining wide/memory/format
services, original call-site types/counts and floating environment. Continue
renderer, story and other missing original systems before complete-game
requirements. Full objective active/incomplete; original execution comparisons
still needed.

## 2026-10-12 — Unsigned64 quotient and remainder

Previous goal turn made progress through db98d01. Began clean and added
FORMAT_UNSIGNED64_DIVISION.md: word ABI, leading-zero/bit-length early return,
word-shift construction, shared restoring loop and XER carry pipeline, signed
trial rejection, final quotient insertion versus remainder return. Recorded no
zero-divisor guard and derived zero-divisor outputs, including65 iterations and
source clearing at a high-bit numerator. Linked recovered arithmetic into integer
and decimal conversion. Valid finite significand needs<=16 decimal digits,
refining multiplication scratch to<=52 digits; arbitrary unsigned64 remains<=20.

Validation:712 instruction anchors and all prior checks passed read-only US ISO.
Fresh222-function evidence reports matched byte-for-byte. Authored word-shift/
trial-result recurrence matched integer divmod for105350 nonzero-divisor pairs
(small complete pair domain, power/adjacent boundaries and5000 seeded pairs,
deduplicated); nine zero-divisor examples were derived. These are contract-model
checks, not original instruction execution or exhaustive64-bit equivalence.
All local behavior documentation links resolve; git diff --check passed.
make test passed core plus36 Python tests; make all exited0, no rebuild. Only
research/provenance/evidence tooling changed. No original execution/emulator run
or profile/save change; original disassemblies/reports remain ignored. Static
checks do not prove executed ABI flags, actual caller domain or native equivalence.

Next: hex-float and remaining wide/memory/formatter services; original format
caller census, floating environment and message renderer/manager cadence. Continue
story attribution/order/branches/conditions and other missing original systems
before complete-game requirements. Full objective active/incomplete; original
execution comparisons still needed.

## 2026-10-12 — Hexadecimal float construction

Previous goal turn made progress through ba057ec. Began clean and added
FORMAT_HEXADECIMAL_FLOAT.md for full local double2hex: signed precision cap,
decimal-generator special recognition, raw special-value signs/case, signed
integer exponent suffix, raw fraction nibble order/truncation, fixed leading1,
zero/subnormal construction, alternate point, value sign and output displacement.
Recorded precision>13's actual stack-address formula, initialized control bytes
versus padding/other locals and maximal stack+263 read address at precision509.
Examples/address consequences are derived, not executed originals or supported
caller/overrun claims. Updated prior formatter documents and index to link the
now-recovered local numeric stages and remove stale recovery gaps.

Validation:728 instruction anchors and all prior checks passed read-only US ISO;
special strings reuse the previously verified original block. Fresh223-function
evidence reports matched byte-for-byte. All293 local behavior documentation links
resolve; git diff --check passed. make test passed core plus36 Python tests;
make all exited0, no rebuild. Only research/provenance/evidence tooling changed.
No original execution/emulator run or profile/save changes; original disassemblies/
reports remain ignored. Static evidence does not establish original caller domain,
executed stack reads/output accuracy or original/native equivalence.

Next: return to message rendering/manager cadence and record consumers, while
retaining wide/memory services, real insertion format/type/count callers and
floating environment as open formatter dependencies. Continue original story
attribution/order/branches/conditions and other uncovered systems before
complete-game requirements. Full objective active/incomplete; original execution
comparisons still needed.
