# Behavior-preserving rendering repair — 2026-10-14

The attempted working-tree optimization skipped required state establishment.
The replacement keeps simulation ticks, authored batch order, vertices, UVs,
colors, alpha/depth behavior, original dialogue and resource lifetimes intact.

## Implemented

- Removed the renderer-wide assumed-state cache. Its reset claimed cull NONE
  and alpha GREATER/8 were active without issuing those calls; its alpha/depth
  helpers did not retain complete comparison keys. Actor, capture and Citro2D
  code also changes GPU state outside those helpers.
- Restored world TEV initialization. The same shader program does not imply
  unchanged TEV stages after message/System-window/Citro2D rendering.
- `MapDrawCache` now lives inside one uninterrupted `drawMap` invocation. The
  first eligible batch establishes texture, wraps, cull and depth state. Later
  batches skip only identical setup. A texture pointer change always applies
  that descriptor's wraps; wrap changes on the same texture also rebind it.
  The installed Citro3D `C3D_TexSetWrap` changes `tex->param`; it does not bind
  the texture. Wrap setup therefore precedes bind. No batch sorting occurs.
- Removed per-vertex dirty bookkeeping from the append-only world buffer.
  Existing `vertices,count*sizeof(Vertex)` flushing already covers precisely
  the regenerated prefix, rather than the full 12,000-vertex capacity.
- House and ship object callbacks collect actual written vertex runs using
  fixed workspace. Sorted overlapping/adjacent runs merge before cache flush;
  fragmented input beyond 64 runs falls back to one encompassing range. No
  geometry or simulation inputs are cached or approximated. Untouched map
  vertices retain the initial upload from `loadMap`.
- Actor texture transforms calculate rotation sine/cosine once per evaluated
  surface, after animated shifts. Per-vertex arithmetic order is retained.
  Workspace capacity persists in `ActorAnimationSample` across evaluations.

## Evidence and limits

`artifacts/optimization-repair/upload-work.log` records real object-writer
indices from the locally converted original packs, with complete byte-for-byte
CPU/GPU-upload emulation comparisons over multiple moving/restored poses:

| Scene | Previous bytes per update | New bytes per update | Flush ranges |
| --- | ---: | ---: | ---: |
| House | 1,139,616 | 248,832 | 5 |
| Harbor arrival | 2,475,576 | 423,576 | 3 |
| Voyage | 808,704 | 423,576 | 2 |

These are cache-maintenance byte counts, not measured frame-time improvements.
The adapter test compares all vertex fields, including untouched UV/color data.
The texture-transform tests compare float bits and complete clipped output
against pre-change arithmetic across forward/repeated/backward surface shifts.
The map-state test replays 10,000 mixed draws against unconditional setup.

All-visible static plaza batch analysis (`static-work.json`) gives 150 batches:
texture binds 150→129, cull calls 150→26, depth calls 150→2. These count CPU
state API calls inside map passes only; visibility, other render passes, GPU
command coalescing and actual frame timing are outside this static analysis.
Draw-call count remains unchanged. The request inbox's 200+/100+ counts and
fixed performance targets were not established by a controlled profile.

No assets were extracted or reconverted for these changes. Existing private
packs and their provenance remain documented in [ASSETS.md](../ASSETS.md). No ISO or
private asset payload enters the commits.

## Additional optimization paths

1. Instrument whole-loop CPU intervals and percentile frame times. Existing
   `C3D_GetProcessingTime`/`C3D_GetDrawingTime` metrics omit synchronous work
   before `FrameBegin`, including save/audio/resource admission. Compare fixed
   routes on New 2DS XL before claiming reduced stutter or a target frame rate.
2. Route ordinary checkpoint saves (`source/runtime/main.cpp`) through the existing
   save worker. Preserve accepted snapshot/slot ownership, completion notices,
   failure handling and orderly shutdown; save-block writes already use it.
3. Preload house PCM before its animation clock starts, retaining the original
   sample/event mapping. Measure memory pressure and admission failure behavior.
4. Separate music file admission and initial buffer fill from the refill
   mutex. Publish completed owners transactionally; preserve fade/loop/event
   ordering and protect worker/file/buffer lifetimes.
5. Add conservative per-batch cutaway bounds, falling back to existing triangle
   checks for animated/fence geometry or refreshing its bounds. Preserve exact
   triangle predicates, target order and the eight-tick visibility hold.
6. Cache immutable actor sampling by complete exact pose/frame/material inputs,
   accounting for multiple actors sharing a model. No time quantization.

Both native render paths already use `C3D_FRAME_SYNCDRAW`. Adding a second
VBlank wait, removing GPU-lifetime synchronization, sorting translucent
batches, introducing LOD or threshold-based visibility caching is not justified
as a behavior-preserving optimization by the current evidence. Message glyphs
already submit through Citro2D's queued image path; the glyph loop contains no
per-glyph flush/program bind. Profile existing batching before replacing it.

## Final validation

- `TMPDIR="$PWD/artifacts/optimization-repair/tmp" make test`: 214/214
  CTest and 134/134 Python tests passed (`test-validated.log`). An earlier
  rerun encountered an unrelated battle test being edited concurrently with
  a missing `<vector>` include; after its owner supplied it the full rerun
  passed. Earlier 213-test runs also passed before the original-upload test
  was registered. Focused render checks pass 4/4 (`scoped-tests.log`).
- `make all` passed (`all-final.log`) and built the native 3DSX. The immutable
  snapshot under `artifacts/optimization-repair/snapshot/` has SHA256
  `d0a1d8e8475ccb3a36b14ad773d956de88c1b93f99edd7b252846e8bfc8bc9e7`.
- The snapshot completed the isolated fresh opening route with
  `tools/emulator_smoke.py` and `tests/azahar_opening_flow.py`, producing the
  expected 1536-byte checkpoint: room1, storyKnown1, story3. Captures are in
  `artifacts/azahar-optimization-repair/`; final harbor and intermediate house
  captures were reviewed. Early capture filenames reflect fixture timing and
  are not reliable scene identities. No native exception was detected.
- Xvfb initially could not bind its sandboxed display socket; the isolated
  rerun with display access passed. User emulator profile/saves were untouched.
- Scoped diff and report-link checks passed. Implementation commits:
  `2510625` (map cache), `32f3a9b` (animation/upload work). The initial map
  commit accidentally included a concurrent staged documentation edit; it
  was amended to exclude it. Later commits use a separate index to preserve
  other sessions' staged work.

This validates the adapted native route and host equivalence checks. It does
not establish pixel-perfect original GX output, physical hardware performance,
a controlled before/after FPS result or completion of the remake.
