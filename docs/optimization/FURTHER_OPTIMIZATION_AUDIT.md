# Further optimization audit — 2026-10-14

Read-only investigation following the rendering repair. No runtime changes
are made by this audit. Rankings identify concrete repeated work; they are not
hardware timing measurements. The [previous repair](BEHAVIOR_PRESERVING_REPAIR.md)
remains the baseline and records earlier save/loading/cutaway candidates.

## Highest-priority new paths

| Priority | Path | Evidence | Behavior-preserving boundary |
| --- | --- | --- | --- |
| 1 | Stop redundant idle audio cleanup | `source/story/opening_house_pcm.cpp:19,37` clears all eight channels on every `update(nullptr)` after samples have loaded. Main calls this outside House each frame. At 60 updates/second, that branch makes 480 clear calls/second. | Retain actual queued/owned voice state; clear on retirement or mute transition, preserving replay, re-entry, failed admission and shutdown. Do not skip source event/envelope advancement. |
| 1 | Avoid actor deep copies before filtering | `source/display/render.cpp:325–326` copies the full `FieldActorRecord` before rejecting mismatched models. The record includes a `talkRules` vector and strings, so these copies can allocate. The entire catalog is scanned per resident model. | Check model first; use lightweight mutable render locals. Prebind ordered actor indices per model if needed. Preserve scripted professor admission, story/visibility rules and NPC state preparation. |
| 1 | Add an exact single-bin actor-surface fast path | `include/display/actor_surface.hpp:100–102` performs four polygon clipping passes even when all triangle UVs lie wholly inside the only selected S/T bin. | Require exactly one bin per axis and all three vertices inside both bins. Retain normalization, atlas mapping, vertex/color order, output limits and the full clipping fallback. No UV rounding or tolerance change. |
| 2 | Reuse collision ray workspace | `source/runtime/game.cpp:381–387` creates/grows/destroys a vector for every multi-cell ray and sorts/uniques duplicated triangle indices. | First retain capacity in collision-owned query workspace. Preserve sorted candidate order and first-hit behavior for equal distances. Handle room transactions, nested queries and multiple Game instances explicitly. |
| 2 | Suppress unchanged audio mix publication | `source/runtime/audio.cpp:176–185` publishes `ndspChnSetMix` per live frontend stream each frame, even when its gain is unchanged. | Cache the exact published mix/gain per fixed stream owner, including track/item-get gain policy; volume alone is insufficient if policy changes. Invalidate on start/stop/reuse. Advance source fades on the existing ticks. |
| 2 | Skip exact repeated map-animation work | `source/display/render.cpp:482–507` transforms and flushes every mesh/UV track on every opaque draw, including repeated render submissions for the same simulation state. | Track exact selected frame plus vertex-owner revision. Another writer, capture or ownership reset must invalidate the cache. Preserve the current cutaway-before-animation ordering rather than changing it incidentally. |

## Original-asset clipping probe

An instrumented copy of the existing animation/surface headers was compiled
under `artifacts/optimization-audit/`. The product headers were not modified.
The probe evaluates the **first pose** in every one of the **40** local
`.pmanim` packs at input frames **0, 17, 45**. It counts actual surface calls
and compares a proposed direct single-bin output against the existing clipping
result, including complete vertex bytes and colors.

- Evaluations rejected: **0**.
- Surface triangle calls: **221,707**.
- Single-bin wholly contained triangles: **209,823**, about **94.6%**.
- Direct-output comparison mismatches: **0**.

Reproduce with the retained `surface-probe.cpp`, instrumented headers and build
command in the audit artifacts; results are in `surface-work.tsv`. This is a
sampled work/geometry comparison, not a frame-weighted gameplay profile or a
proof over every pose, UV boundary, nonfinite input or supported pack. A runtime
implementation still needs boundary-focused differential tests and native
route validation before adoption. Multi-bin/edge-clipped triangles must retain
the existing path. No FPS gain is inferred from the eligible percentage.

## Additional paths and constraints

- **Animation prefix state:** `include/display/actor_animation.hpp:126–145` copies base
  state and replays every prior sparse key on each evaluation. Retain an exact
  uninterpolated prefix, or sparse checkpoints assembled in the same addition
  order. Never incrementally accumulate into an already interpolated sample.
  Verify loop wrap, backward seek, pose switching, visibility/texture shifts,
  sparse-node easing and memory cost. This has higher complexity than copy
  filtering or the single-bin path.
- **Effective-time actor caching:** field sampling at `source/display/render.cpp:374`
  invalidates on every simulation tick; `characterModel` at line 580 evaluates
  unconditionally. The cinematic owner already normalizes time and compares
  pose/time at `source/display/render_frontend.cpp:184–190`. Extend exact caching only
  with per-instance ownership and equivalent validation/output state; a shared
  one-entry model cache can thrash between actors. Do not quantize time.
- **Other redundant channel clears:** muted field updates clear eight effects
  channels each frame (`source/runtime/audio.cpp:165,173`); map-turn updates clear
  channel 19 each Opening frame before map entry (`audio_opening_map.cpp:12`).
  The same ownership/transition rules as the house cleanup apply.
- **Cold audio and mutex contention:** first map-turn playback opens, allocates
  and reads PCM (`audio_opening_map.cpp:17–24`). First key-item resource
  validation does filesystem work under the music mutex
  (`audio_item_get.cpp:11–24`). `serviceMusic` also holds that mutex while
  refilling from files (`audio.cpp:161`), and frontend updates acquire it every
  frame. Measure lock wait/hold time and stage immutable metadata/samples
  earlier; publish admitted stream/buffer owners transactionally. Preserve
  failure/retry semantics and source event ordering.
- **Already known severe hitch candidate:** ordinary saves still execute
  `writeSave` synchronously in `source/runtime/main.cpp:229–231`; extend existing
  worker ownership with immutable snapshots, slot/path ownership, completion
  notices and serialized reload/exit behavior.
- **Collision arithmetic:** precompute immutable triangle edge/normal
  intermediates at load if bitwise query comparisons show equivalence. Keep
  runtime contact/door policy reads live and preserve tie ordering.
- **Cutaways:** use conservative batch bounds or ordered spatial hierarchy,
  with dynamic geometry revision tracking or scan fallback. New bounds must
  account for the existing triangle test's numerical acceptance tolerance;
  an unpadded AABB can reject a hit accepted by that test.
- **Small allocation cleanup:** `render_opening_cover.cpp:28–34` builds a
  vector of at most five quads each frame. A fixed array removes that bounded
  allocation while preserving composition order. This is lower priority than
  audio cleanup and full actor-record copies.

## Validation to require for implementation

Use whole-loop p50/p95/p99 frame intervals, audio lock wait/hold times and
allocation/call counts on fixed routes; Citro3D processing metrics alone omit
save/audio/loading work. Validate on New 2DS XL before stating hardware gains.
Count NDSP clear/mix calls using a stub while checking mute/re-entry/overlapping
tails and unchanged sample/rate/queue traces. For geometry/collision work use
independent pre-change output comparisons, including exact boundaries and
first-hit ties, followed by required `make test`, `make all` and isolated
native route checks. Avoid debug departure targets with SD trace logging when
profiling ordinary performance. Keep GPU lifetime synchronization, draw order,
simulation ticks, original assets and story behavior unchanged.
