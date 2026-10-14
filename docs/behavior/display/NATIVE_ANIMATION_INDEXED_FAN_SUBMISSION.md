# Native original indexed animation fan submission

US G8ME01 revision0. The implementation bridges the original
[array/FIFO graph](ANIMATION_RENDER_ARRAYS_AND_VERTEX_FIFO.md) and
[numeric/array domains](NATIVE_INDEXED_FAN_NUMERIC_AND_ARRAY_BRIDGE.md) to the
actual Citro3D immediate fan path. It does not establish original raster parity.

AnimationFanStream retains source INDEX16 descriptor/format/array state for
position9,normal10,color11 and optional texture13. These logical setters select
CPU decoding into the native XYZ/ST/RGBA shader ABI; they do not cast serialized
MODEL offsets into ARM pointers or change original request arguments to match
the older shadow array subset. Source array strides are12/12/4/8. Position and
normal bases independently index current global animated pose outputs; MODEL
colors/UV use their own checked big-endian tables and shape-relative byte bases.
Unused UV arrays starting at the global table end are not rejected eagerly.
Raw RGBA channel bytes remain in original order, never a host-endian color word.

A source Begin captures count and the exact texture package/cell/generator/matrix.
One fixed64-vertex staging array avoids per-fan allocation;64 is explicit native
admission, not the original LOW16 count limit. Current original fan maxima are14.
Each FIFO request decodes independent low16 indices and captures actual local
positions/normals/colors/UV. Prefix vertices stay retained. The final FIFO request
only succeeds after a complete validated fan is submitted; a held submit retries
that captured batch, not decoded/mutated inputs or previously submitted fans.
Zero-count Begin and counts1/2 generate no triangles. A mismatched owner/final
index request cannot reuse a pending batch. Resource retirement resets this owner
before both battle-atlas release paths, under existing FrameSync lifetime rules.

UV transforms use full ST11 dot products, including matrix columns2 AND3.
Generator60 preserves ST independently of a stale loaded matrix. The transformed
source ST is then cropped into the selected package-global atlas cell and gets
ONE final V-height correction. Source UV/matrix results outside[0,1] remain
pending: correct atlas clamp there requires subdivision, and per-vertex clamping
would distort interpolation or bleed adjacent atlas cells. Multislot source
arrays and unrelated texture packages remain unsupported native domains.

Native submission requires exact current resident atlas/cell/transform identity
and known disabled RGB/alpha lighting channels. It retains/validates normals but
does not pretend the XYZ/ST/RGBA shader implements lighting. Actual lit channels
need their normal-matrix/light shader provider. Source-local positions pass once
through the already composed source view*model uniform; flattened sample vertices,
shape.world and legacy actor scaling are not applied a second time.

Every admitted complete batch restores acknowledged GPU shader/texture state,
then emits C3D_ImmDrawBegin(GPU_TRIANGLE_FAN), real source XYZ/ST/RGBA attributes,
and C3D_ImmDrawEnd. No Begin survives a frame/provider wait. Citro3D's immediate
float24 conversion is a declared platform numeric boundary, not original XF/GX
bit equivalence. Nonzero culling/mirrored-model parity and framebuffer comparison
remain unverified. The current serialized finishing queue does not support
arbitrary interleaving of source GPU owners during a staged fan. Already emitted
actors are not replayed just because a later callback remains pending across
outer frames; sustained scene/teardown/field-return equivalence remains open.

Host tests decode all170 original fans/653 indexed vertices from actual trio
b_mario,c_pkr and c_kuri_h banks with independent array bases and original UV
selectors. That broad geometry census uses an explicit cell0 fixture, not a claim
about every real palette/animated key. The first original Goombella cell is
separately resolved through actual descriptor/TPL/package metadata. Tests also
cover actual normal/color indices235..237 exceeding position count128, held final
requests, retained prefix data, ST11 cancellation, identity60, atlas V/cell crop,
invalid owners/bounds/NaN/capacity and altered raw RGBA bytes. Synthetic changes
are fixtures, not original execution. Native GPU command evidence and its next
service frontier are recorded in PROGRESS; original visible images, full lighting/
samplers/pose coverage and hardware performance remain separate requirements.
