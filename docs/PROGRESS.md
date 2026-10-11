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
