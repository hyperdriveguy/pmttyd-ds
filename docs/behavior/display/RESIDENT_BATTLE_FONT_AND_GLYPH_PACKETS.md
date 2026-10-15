# Resident original battle font and native glyph packets

The native Renderer borrows its actual existing `font.t3x` import into
`BattleFontGpuResource`. Admission requires512x256 RGBA8, one unrotated whole
image with exact0/1/1/0 UV endpoints, and the finite162-glyph original-US
`font.pmfont` catalog. The owner copies that metadata into immutable storage.
This is resource admission, not original FontInit/Setup or a fabricated GX
object. Original source resource initialization remains a separate producer.

Each successful adoption creates an opaque generation. Copied leases must match
that exact generation, native texture identity and immutable metadata owner.
Retirement invalidates every lease before actual texture release/replacement;
reusing the same native pointer cannot revive one. The borrowed import remains
owned/freed by Renderer. Shutdown retires retained class frames, font leases and
then synchronizes/frees actual textures. Init retires the old generation before
any replacement attempt. Class-menu native publications now carry this actual
font lease rather than relying on the raw imported pointer alone.

`BattleFontGlyphSink` consumes the [shared actual glyph raster](RETAINED_MATRIX_FONT_RASTER.md).
Its source actor readers must access the genuine BFN/WID/GLY memory. Source
matrix loads0 and effective TEV color1 are retained independently; there is no
default identity, raw-color recomposition or default successful font profile.
An external actual GX state owner must prove the ordinary original font setup,
including static texture object8041e97c, residency, texture matrix, TEV and fog.
An unavailable/noise/rainbow profile holds before RNG/geometry admission.

The sink reserves its native capacity before execution. Whole one-quad admission
precedes the glyph owner's two real irand64 calls and void emission. Accepted
RNG/emit callbacks may not fail, reenter rendering or change admitted resources.
Recursive glyph/matrix/color/publication operations are rejected during glyph
execution. An admitted callback contract violation aborts rather than silently
dropping geometry after consuming RNG. Blank, zero-alpha or offscreen glyphs
still consume source RNG and retain a packet; visibility does not replace source
execution. Capacity is an explicit native budget, not a measured source limit.

Every immutable packet copies its exact source quad, currently loaded affine
matrix, effective color, font generation and actual camera projection/viewport/
scissor/depth. Its glyph coordinates already use the source font's signed16
domain. The source matrix already contains camera view. Native submission uses
the admitted tilted projection times that loaded matrix, with no second view,
tilt, unit conversion, fixed selector projection or layout replay. Atlas UVs use
the unchanged512x256 original font image with native V inversion. Composed
float transforms and every product/sum of all four shader clip vertices must
remain finite before RNG or GPU admission. This native guard does not divide
by W or reject singular, zero-area, transparent or offscreen source glyphs.

The finite ordinary shader outputs C0 RGB and A0*TEXA, as the original normal
DrawStart profile requests. It uses the actual source raster viewport/scissor/
depth, no alpha cutoff, ALWAYS depth compare without depth writes, normal source
alpha blending and no culling. Texture/quad/projection/resource checks occur for
the complete leaf batch before any C2D/GPU mutation. Replay performs no source
execution or RNG. This is a declared PICA numeric/raster adapter, not a hardware
or original framebuffer equivalence claim.

Short source currentness proves the real executing callback/camera pass. Long
publication currentness proves allocation/resource lifetime and remains separate
after camera finish. Losing the latter permanently poisons the sink; resource
pointer reuse or a later true predicate does not resurrect a publication. All
current/profile predicates are pure nonreentrant queries. Callback userdata and
source cell owners outlive their invocations. Retirement inside glyph execution
defers cleanup through its strong invocation pin. Explicit retirement breaks
context/resource cycles; callers should prefer separate actual resource owners.
The same sink is never rebound after retirement.

A published glyph leaf does not acknowledge the whole weapon display callback.
The outer [weapon native frame](NATIVE_WEAPON_MENU_FRAME.md) must preflight and
submit ONE immutable mixed sequence of font/icon/surface/window packets in source
order before camera finish or queue advancement. The actual native texture,
lease and packet drawing hooks exist; the source GX/resource/weapon compositor
bindings remain required. They are not replaced by the old selector's independent
LCG or a captured request vector. No source readiness or battle activation is
forced by this module. Physical controls and source dialogue bytes are unchanged.

Host checks use read-only original US BFN plus the existing font pack, original
setup's private width7→9 patch, actual glyph atlas admission, resource rollback/
retirement/reuse/forgery, matrix/color capture, capacity/RNG order, reentry, blank/
offscreen glyphs, profile waits, source/allocation loss and post-camera publication.
Original source/binary assets remain private under the existing asset ledger.
