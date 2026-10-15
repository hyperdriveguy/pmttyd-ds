# Ordered enemy HP publication and native icon generations

2026-10-14, US G8ME01 revision0. This increment joins source number/iconGX
packets and the [original HP texture/solid packets](ORIGINAL_ENEMY_HP_GPU_ASSETS_AND_ORDERED_BATCH.md)
in one retained source-order publication. It composes with the retained
`BattleEnemyHpFrame`; it does not install normal source scheduling or invent
an allocation, camera, HP/status/Tattle result or missing asset.

## One source-order owner

`BattleEnemyHpOrderedPresentation` requires a nonzero actual allocation key and
real `current(allocation)` and source-frame `complete(pass)` providers, actual
icon residency queries, actual HP raster-view queries and the actual HP GPU
resource. A caller-supplied nonzero number or B pointer alone is not allocation
proof. Missing providers refuse binding. The native owner token created by
bind identifies this publication instance only; it does not replace the real
allocation proof. Caller must keep every callback context/resource alive.

`begin(pass)` opens a monotonic native publication sequence, never a source
clock or simulation tick. The retained source callback emits each number glyph
and each bar helper inline; their sinks call `appendIcon(pass,packet)` and
`appendHp(pass,quad)` at that exact source point. Both copy accepted data into
one variant vector. No per-unit regrouping or separate overlay is introduced.
For two units the retained order can be digitsA,17,solid,18,digitsB,17,18.
A stable `BattleEnemyHpOrderedHpSink{owner,pass}` supplies the HP sink ABI; an
old held sink cannot accidentally append into a new pass.

Icon packets copy the actual transform, viewport/scissor/depth, source material/
pipeline, source image and original geometry, together with the actual resident
texture identity and import-generation token. HP entries additionally copy the
actual raster projection/viewport/scissor/depth at admission. HP vertices
already contain model/view transforms, so their projection is the source PICA
projection-only matrix, not the last number icon's P*model/view. Borrowed input
packets are copied before provider callbacks can alter external storage.

All admission checks actual allocation liveness before and after external
proof callbacks. Provider reentry cannot mutate an active owner operation;
retirement requested in a provider is deferred until its request returns, then
discards both pending and published native metadata. Actual allocation loss
also discards both histories, with no partial publication. Resource/proof
unavailability holds admission; stale resource tokens cannot become drawable.
Caller must retire/rebind the source composition when its allocation ends.

`publish(pass)` requires actual source completion, then checks every icon/HP
resource and full copied geometry/raster metadata, then rechecks completion and
allocation. Only then does it replace the immutable publication atomically.
An unfinished next pass preserves the prior complete publication. Empty complete
passes intentionally publish empty geometry. `validate` accepts only this
owner's actual immutable publication object; a forged/copied struct with the
same token/pass cannot claim source completion. Replay also requires current
allocation and every actual resource generation.

## Actual native residency and explicit renderer journal

New `render_battle_enemy_hp_publication.cpp` implements the approved narrow
Renderer bridge. Shared declarations/capture/import edits are delivered as an
exact old/new replacement journal at
`artifacts/hp-ordered-renderer-journal.json`, with baseline file hashes. Those
shared files are not edited in the worker's live worktree. The journal targets
`include/runtime/platform.hpp`, `render_source_icons.cpp` and
`render_retained_icons.cpp`; root owns applying it to the matched scope.

Each actual SourceIconCell receives a new shared generation only after successful
Tex3DS import and existing exact backing/format/subtexture admission. Eviction
and release retire it. `enemyHpIconResidency` reads that actual cell plus its
original image dimensions; no image index alone or synthetic guest pointer
becomes a GPU allocation. Separate read-only preflight checks packet material,
source pipeline, geometry, source dimensions, exact padded backing, texture
identity and that same import generation. Same-address reimports receive a new
token. Existing legacy `drawSourceIcons` behavior stays intact; the new API is
explicitly selected for complete truthful HP publications.

`bindEnemyHpPresentation(owner)` installs a borrowed owner for residency pinning;
that owner must outlive the binding. `captureEnemyHpPresentation(pass)` enables
number capture only for its actual building pass. Pass0 disables capture while
keeping both building and published icon images pinned. During explicit HP
capture, `captureSourceIcon` routes the actual copied packet into the ordered
owner instead of the legacy icon-frame sink, avoiding a duplicate later draw.
Root must enable this scope only around the matching real queued HP invocation,
then disable capture while retaining the owner. Source icon release detaches
the bridge; allocation/resource shutdown must explicitly retire/rebind owners.

The real renderer residency cache keeps its existing202-cell capacity. The
new owner adds correct pins, not silent eviction of a published glyph. Extreme
combined legacy/HP residency pressure can hold admission; an increased native
capacity would be a separately justified platform allocation decision.

`enemyHpRasterView(game)` requires actual active Trio/current camera8 plus
admitted source projection/viewport/scissor. It obtains the retained original
projection from `NativeCameraDraw` and applies the existing pure PICA tilt with
an identity affine factor. It does not reuse the previous number matrix,
synthesize a projection or write any logical source camera state.

## Whole combined replay preflight

`drawEnemyHpOrdered(owner,hpGpu)` requires the explicitly bound owner, actual
initialized C3D/C2D renderer, valid shader transform location, actual font sampler
and its immutable snapshot. It first
validates the complete publication against actual allocation/resource proofs,
then independently checks every icon against this Renderer and every HP packet
against the supplied native HP owner. All checks precede the first GPU effect.
A bad final glyph or HP packet rejects the entire publication.

After the entire combined preflight succeeds, C2D is flushed before any
viewport/material state is overwritten. The replay loop executes no source controller, provider query or import. It
uses the exact ordered variant vector, per-icon source material/pipeline and
per-HP original material, each packet's copied raster state, actual resident
textures and the shared native shader. It restores the full top viewport/
scissor/depth domain afterward. Caller owns current target, C2D flush/restore,
FrameBegin and single-threaded lifetime exclusion: imported GPU resources cannot
be released concurrently with a replay. A second per-packet validation is a
defensive guard, not permission for a source provider to execute during draw.

This closes isolated combined publication ownership and replay hooks. Real
normal source-frame registration, actual source provider binding, allocation
lifecycle integration and visual proof remain root-owned. No normal activation
or on-device complete HP HUD claim follows from these host fixtures.

## Validation

Strict host tests verify mixed two-unit packet order, no publication before
actual completion, immutable owned publication identity, previous publication
retention during a held next pass, actual allocation loss during a proof,
retirement reentry, missing icon/view admission, stale icon generation and HP
resource rejection, old sink/pass rejection, publication pins and complete
empty passes. Explicit fixture import/allocation proofs are not native rendering
claims. ASan/UBSan passes with leak scanning disabled.

Actual devkitARM ARMv6K hard-float Werror syntax checks use a private header/
source overlay with the exact journal applied, including new native replay,
modified import/capture bodies and portable owner. Shared live files stay
unchanged. An initial ARM-only std::pair<int32_t,int32_t>/pair<int,int>
comparison mismatch was corrected by explicit fixed-width comparisons; accepted
ARM checks pass. Root owns journal integration, matched full make test/all,
native captures and physical-device validation. No new original art/audio or
asset conversion is introduced beyond the already frozen separate HP pack.
