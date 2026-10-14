# Native animation texture object and coordinate continuation

US G8ME01 revision0. Original
[object/LOD request graph](ANIMATION_TEXTURE_OBJECT_SETUP.md) and
[coordinate graph](ANIMATION_TEXTURE_COORDINATE_MATRICES.md) are the baseline.
Implementation: animation_texture.hpp/.cpp, render_animation_texture.cpp and
actual battle finishing-camera dispatch. No original frame equivalence is claimed.

The directory writer optionally emits unchanged original TPL companions alongside
unchanged MODEL companions. Mario/Goombella and Professor conversions supply the
correct source TPL; Professor cache requirements include the new product. Shared
BattlePoseResource loads and validates TPL table/header/base-image/palette extents
and ordinary descriptor image indices. Missing companions remain an unavailable
texture provider; malformed present companions reject the bank. Native admission
limits and recognized formats are loader bounds, not original TEXGet guards.

AnimationTextureResource keeps image/palette header projections and provenance
file offsets. The temporary raw-byte import buffer is released; data/paletteData
are NOT executable/native pixel pointers or owned pixel/TLUT buffers. Original
payload remains in private RomFS. Actual CMPR images use their existing decoded,
scaled, resident RGBA atlas package. CI/TLUT payload ownership for other providers
is not closed by these metadata requests alone.

The retained helper processes reversed draw slots. It captures original base plus
live unsigned state byte, descriptor index and wrap override, then performs two
separate TEXGet requests. The FIRST lookup chooses CI versus ordinary; the SECOND
supplies the retained image/palette headers. All object requests finish before
the second reversed coordinate pass. CI follows TLUTInit,TexObjCI,LOD,LoadTlut,
LoadTexObj; other kinds skip image setup but still generate coordinates. Invalid
indices hold at the original diagnostic/fallback boundary rather than silently
mapping to an unrelated atlas cell. Current resource/pose ownership is retained;
arbitrary mutable source-handle/global-resource rebinding remains unsupported.

Second pass reads all5 live T24 transform lanes from PMANIM4 evaluated surfaces.
PMANIM3's empty surfaces use original base lanes: that converter admits only
identity bases and zero original UV-shift keys for the converted clips. It builds
SDK requests with the actual negative-angle-before3c8efa35 coefficient, literal
sy!=0 translation gate, source R*M*S grouping and generator-before-matrix load.
This helper does not use the older ActorTextureTransform convenience arithmetic.
Native SDK matrix/trig remains an explicit finite numeric adapter, not Gekko
paired-single/libm/FPSCR bit parity. Held GX/math requests do not replay completed
lookups or matrix operations. Matrix inputs are captured before their calls.

The native GPU domain is coordinate0,kind0,CMPR,clamp,linear,no mipmaps/edge/bias,
no CI. Init/LOD prepare typed object state; Load requires a valid palette cell,
exact converted resource identity and actual live GPU texture. Texture-name
matching alone is insufficient: two packages can share an original TPL name but
pack different atlas cells. Actual resident Mario, Goombella and Frankly battle
textures are admitted by exact shared resource identity. Frankly reuses the
original professor package independently of field-room retirement: its atlas
is1024x256,UVscale4; current Mario/Goombella packages are512x512/512x256 with
scales1/2. The native converted layout domain is width512 or1024 and scale1/2/4/8,
with height exactly width/scale. This is a conversion admission policy, not a
GX texture-size rule. Load binds the actual resident texture through Citro3D
and publishes the character-atlas texture0 owner in shared display state.
Later restoration and indexed-fan UV scaling resolve that same resource owner.
Scene retirement synchronizes GPU work, invalidates this binding and then frees
all three scene textures. It never
casts source offsets to PICA pointers or uses a missing texture as success.

The coordinate generator and matrix feed the
[indexed-fan UV consumer](NATIVE_ANIMATION_INDEXED_FAN_SUBMISSION.md). Source T24 matrix must apply FIRST, then atlas cell crop and V-height
correction ONCE. Setup alone does not submit or acknowledge any vertices. Generic
multislot/CI request tests do not establish corresponding GPU support. Frankly's
battle residency owns its own imported professor atlas; field room5 can free its
separate texture safely. It is never substituted with Mario, map-icon or another
texture. Interleaved/reentrant local-object production
would additionally need explicit object generation checks; the current callback
is one retained same-resource draw in the blocked camera queue.

Host tests cover real image44, held load capture/nonreplay, live state1 palette,
CI order, reversed two-slot/nonzero-kind skipping, live float capture and source
matrix composition. Synthetic altered resources/providers are test inputs, not
original execution observations. Actual native trace/remaining work lives in
PROGRESS and EVT-01. The later
[ordinary TEV binding](NATIVE_MAP_TEV_PROGRAM_AND_ORDINARY_COMBINER.md) now
completes the admitted light/texture/last-stage color/fog-off/TEV setup prefix.
The indexed-fan backend now submits admitted original geometry. Later normal
numeric/material/callback providers still gate complete part history and camera
queue completion; other GPU fog/projected domains remain separate boundaries.
