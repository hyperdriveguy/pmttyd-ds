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
