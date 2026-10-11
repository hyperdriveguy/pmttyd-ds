# Original asset pipeline

The user's latest preference supersedes the initial hybrid-placeholder plan:
use assets very similar to the original game, preferably original geometry and
textures; scale textures/reduce vertices/adjust lighting for performance.
Preserve original sounds and music. Do not reintroduce the earlier vector redraws
or synthesized sound effects.

`tools/iso_assets.py` reads the FST from the legally acquired `G8ME01` ISO. It
validates bounds and filenames, inventories all files and extracts exact selected
paths. Extracted files receive SHA-256 provenance sidecars. The source ISO is
opened read-only. Original/converted content belongs under ignored local-assets,
romfs or artifacts directories, never source history.

`make` invokes `tools/build_assets.py`. Set TTYD_ISO to change the input path.
Selected original asset sources:

- `a/a_mario-`: complete Mario pose texture 0.
- `a/c_pkr` and `a/c_pkr-`: Goombella, animation PKR_Z_1 at frame 0.
- `a/c_kanbu` and `a/c_kanbu-`: Crump, animation Z_1 at frame 0.
- `a/c_kinopio` and `a/c_kinopio-`: Toad, animation KNP_Z_1 at frame 0.
- `m/gor_01/d` and `m/gor_01/t`: original plaza mesh, scene graph and textures.
- `b/gor_01.tpl`: room backdrop.
- `m/stg_01/d` and `m/stg_01/t`: original common battle stage.
- `sound/stream/stg_gor1_32k.stm`, `btl_zak1_32k.stm`, `btl_win1_32k.stm`:
  original Rogueport, normal-battle and victory music. Confirm encounter-specific
  track choice in Dolphin; do not assume normal battle music is the tutorial's.
- `sound/proj/pmario.{sdir,samp,pool,proj,etbl,slib}`: preserved original sound bank.

GX decoding covers I4/I8/IA4/IA8/RGB565/RGB5A3/RGBA8, indexed palettes and CMPR.
Only the base mip level is used. Map texture dimensions are capped at 128 pixels
and normalized to powers of two for native repeat sampling. Original character
poses are rasterized from source geometry to 128-pixel billboards. This is an
intermediate optimization; proper original animations and direction variants
must replace the idle-only display. No animation timing is inferred from the
atlas.

The map converter flattens original S subtree transforms and triangle topology
at scale 0.1, preserves texture coordinates, material colors, wrap modes and
alpha-layer grouping. The current plaza has 40,714 triangles and 79 batches; the stage has 10,801
triangles and 14 batches before static visibility overrides. The mask, closed
upper curtain and inactive switch props are suppressed for this playable view;
verify these inferred visibility rules against the original battle scene. Exact
current counts are in romfs/manifest.json. Near-ground original collision triangles are reduced
to 711 planar wall segments. The walkable subset still needs original floor,
slope, trigger and visibility semantics.
Geometry decimation, dynamic visibility and original map animations are pending.
Materials with multiple samplers are explicitly reported rather than assumed
correct. The selected room currently has none.

STM conversion preserves rate/channels and original loop points, clamped to
valid source sample bounds. Runtime streams PCM; it does not load the entire
music track into RAM. MusyX directory indexing exposes 1,247 original samples;
macro-to-game-event resolution is still needed before faithful effects play.
Unresolved effects remain silent, not replaced with generated tones.

Format research used noclip.website (MIT) and vgmstream (ISC); corresponding
license notices are in third_party. Nisto/musyx-extract was read for format facts;
its unlicensed source is not incorporated. No Paper Mario decompilation code is
currently incorporated. Record licenses before adding any external code.

Ground surfaces are exported from the original collision subtree into
map.ground (little-endian uint32 count, then nine float32 coordinates per
triangle). Slopes with absolute normalized Y normal >= .65 are retained. This
is a provisional traversal filter, not an observed original engine threshold.
Original rel/gor.rel is extracted read-only with provenance for exit/event
analysis; no original executable code is run by the remake.
