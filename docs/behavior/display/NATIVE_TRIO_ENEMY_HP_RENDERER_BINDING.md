# Native Trio enemy HP renderer binding

2026-10-15, US G8ME01 revision0. New
`render_battle_enemy_hp_runtime.cpp` connects the actual queued Trio HP callback
to the retained source frame, original icon resources and original HP atlas.
The exact shared changes are delivered in
`artifacts/hp-runtime-renderer-journal.json`; the worker does not overwrite the
foreign `render_finishing_camera.cpp` or other shared live files.

## Native lifecycle and actual ownership

`prepareEnemyHp(game)` runs before FrameBegin. It requires actual Trio battle
ownership and the admitted runtime allocation, then loads the real
`NativeBattleEnemyHpGpu` catalog/t3x outside frame submission. It never creates
an original FileEntry/palette/camera to make rendering succeed. Texture queries
still require the live actual VM B/FileEntry/a0/holder/common-bank chain.
Original17/18 conversion and rights remain in the
[separate HP asset contract](ORIGINAL_ENEMY_HP_GPU_ASSETS_AND_ORDERED_BATCH.md).

One stable heap context owns native GPU resources, the raw GX table, HP provider,
ordered publication and source callback contexts per actual allocation. Runtime
retains that context through the explicit `TrioEnemyHpNativeServices.lifetime`
lease. Every callback checks native liveness before accessing raw Game/Renderer
pointers. Native release marks the context inactive first, detaches capture,
retires publication, discards the provider and releases the actual GPU bank
while C3D is alive. The native GPU resource release callback synchronizes C3D
before freeing texture storage, including preparation before the next FrameBegin. A retained old runtime context subsequently returns
unavailable safely. Shutdown releases HP before source icons/shaders/C3D.

The context retains the actual shared allocation token as well as its numeric
key. It compares both through the actual Trio APIs. Keeping the token alive
prevents allocator-address reuse from falsely admitting an old publication;
a nonzero address or copied key alone is not allocation proof. Publication
liveness does not depend on the short HP camera lease, so accepted copied
geometry survives the source callback's legitimate finish.

## Exact queued source graph

Only actual callback8011fde0 is replaced with `Renderer::resumeEnemyHp`.
The runtime verifies its real camera8 Draw invocation/current record,
allocation, successful camera Main epoch, full HP camera bridge admission,
source memory and retained source-service ownership. It supplies original
status27, Tattle/saved switches, registry/HP/gauge reads, interpolation and
truncation. No always-visible HP, fake Tattle flag, harness HP or guessed gauge
ratio is added by the renderer.

The [actual Trio runtime binding](TRIO_ENEMY_HP_RUNTIME_BINDING.md) accepts Number and raw-plane providers plus
begin/publish hooks. Begin creates/opens the ordered native pass only after
runtime admission. The source frame executes the complete HP parent, original
Number→SourceIconMatrix→iconGX and Bar→TexPlane/Rasta paths. Native icon words
use actual `SourceIconResources` with actual Trio resource-word fallback;
those camera-owned addresses have first refusal inside the Trio bridge and
never fall through to an old synthetic camera view. Halfword native resources
and the existing actual map flags adapter remain supplied; runtime also owns
actual VM halfwords. Matrices/current camera come from the actual admitted HP
bridge, not a fixture/legacy camera token.

Number GX calls use the existing renderer material, map/fog/light, original
texture-object/import and pipeline services. Its final quad captures the
actual resident texture import generation and copied source raster/material.
An icon miss schedules the existing real out-of-frame import and retains the
source call. HP TextureObject queries use `enemyHpResourceWord` and the safe
actual common-bank reader; native atlas readiness does not bypass that source
resource ownership. Missing map/light/fog, source camera/memory, original glyph
or common-bank providers remain real source waits.

Capture is enabled only around the actual HP invocation. A held retry enables
the same retained building pass again; every return disables capture while
keeping copied building/published icons pinned. Publish requires actual
`enemyHpFrameComplete(pass)` and whole combined allocation/resource preflight.
It never treats an individual glyph/bar or camera-slot snapshot as completion.
Runtime finishes its camera lease only after accepted publication.

## Original ordering and presentation

One [ordered owner](ORDERED_ENEMY_HP_PUBLICATION_AND_NATIVE_ICON_GENERATIONS.md)
receives glyphs and bar primitives inline: numberA,17,optional solid,18,
numberB,17,optional solid,18. There is no all-numbers pass or later bar overlay.
Each HP packet captures actual projection-only PICA P and its viewport/scissor/
depth because its vertices already contain source model/view. The previous
number's P*model/view is never reused as the bar projection.

Native replay happens after world/camera4 scene icons and before ordinary
native overlay/menu drawing and legacy camera8 icons. This deliberately follows
the recovered current source camera8 ordering: HP class0 priority902 sorts to
399098, StockEXP class1 priority901 to399099, AP source priority499 and reel490
later. See [display footer registration](BATTLE_DISPLAY_FOOTER_REGISTRATION_AND_MUTATION.md)
and [live display footer](BATTLE_DISPLAY_LIVE_PART_LOOP_UNIT_TAIL_AND_FOOTER.md).
The existing original-looking HUD/menu art and lower audience scene remain
unchanged. Any future camera8 callback with an earlier key needs its own
integration review; this increment does not invent a universal sort order for
unimplemented callbacks.

Combined replay preflights all actual icon/HP generations and the initialized
C3D/C2D renderer, shader transform location and real parked font sampler before
flushing C2D or mutating presentation GPU state. It uses copied per-packet
material/raster state in source order. Physical controls and simulation timing
are unchanged.

## Precise activation frontier and validation

These hooks bind the genuine source invocation when the existing runtime
scheduler reaches it. The existing normal CameraMain/Draw path currently begins
in the finishing sequence after victory. Previous native evidence admitted
camera4's part/stage queue and reached actual HP8011fde0; an intentionally
missing Shape provider in a host test is not proof of a normal native camera4
hold. The fresh combined normal route must report its actual current frontier. This increment alone does not make original
HP visible during the active enemy turn loop, reconstruct missing original
BattleInit/Main/turn scheduling, or prove the complete tutorial/real battle
system. Removed enemies can legitimately yield an empty HP publication.
No fixture camera, forced source callback retry or fabricated actor is used as
a normal fallback.

The read-only `enemyHpMetrics(game)` exposes actual native resource readiness,
allocation/token liveness, admitted source pass, actual source-frame completion,
published pass and immutable glyph/bar packet counts. It neither validates by
mutating publication nor advances a source cursor/camera. A private native probe
can distinguish completed normal callback with an empty post-defeat publication
from visible-HP proof. Successful GPU replay must still be observed separately
through the real draw return/capture; these counters alone are not pixel proof.

Strict ARMv6K hard-float syntax checks cover the new native module and the
exact journal-patched finishing callback. The patched full renderer also checks
with its existing unrelated parentheses warning left nonfatal; no foreign
expression is changed. The runtime worker’s battle_enemy_hp_number_bridge_tests supplies the actual
Trio constants and original icon resources through full SourceIconMatrix/GX;
its strict host test passes. Root owns full make test/all, actual source pending-frontier
observations and isolated native capture. Prior separate source/GPU/ordered
fixtures remain distinct from normal battle activation proof. No original
asset conversion is changed by this renderer binding.

A separate exact journal corrects a source-function name in the earlier HP
helper enum/provider/contracts:802b940c is GXSetZCompLoc, already correctly
represented by ShadowGX::DepthLocation and the primary US symbol table. The
original InitSub call at80103c54 passes1. The rename changes no numeric request,
argument, continuation, material or geometry. Native HP uses ALWAYS alpha and
no depth writes, so the current late-depth hardware adapter remains equivalent
for this particular admitted bar program.
