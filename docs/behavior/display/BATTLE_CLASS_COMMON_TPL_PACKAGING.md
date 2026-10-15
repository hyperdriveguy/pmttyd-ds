# Original common battle TPL packaging

2026-10-14; US G8ME01 revision0. The existing
[`battle_class_surface_assets.py`](../../../tools/battle_class_surface_assets.py)
converter now also exports `romfs/battle-class-common.tpl`, an exact byte copy
of original `battle/common/battle_common.tpl` from the read-only ISO. This is
an ignored compiled pack resource, not an editable source import. Normal
`make assets-base` already runs this converter; no new build recipe is required.
Normal source overrides still compile afterwards from `assets/manifest.json`.
The converter does not write into `assets/` or overwrite checked-in source edits.

The original bank is217920 bytes, SHA256
`8eb685317056f42b3ef73138eaad85d16fa4943d830b44a149f09971a5959bb1`.
Its unmodified big-endian header has magic0020af30, image count97 and relative
table offset0c. Size and whole-file hash must match before packaging. A corrupt
or truncated source fails before replacing an existing output. The original
path, hash and new output record are included in
`battle-class-surfaces.json` under `common_tpl`.

This raw copy preserves every header, relative pointer, flag and original
compressed texel byte. It performs no decode, source-pointer relocation,
resampling, geometry reduction, lighting change or new art substitution. The
circle0/bubble1 atlas retains original40x40 and128x80 source pixels,
256x128 RGBA8 backing and identical catalog UVs/descriptors. Native two-pixel
gutters repeat original clamp-edge texels because the source sampler is
wrapS/T0,min/mag1. Only atlas gutter/native texture bytes and conversion
metadata change; source interiors, descriptor `.bin` and original common TPL
stay byte-identical. The icon atlas is separate and unchanged.

The resource permits a real native file owner to load the exact original TPL
and map its actual guest descriptor, pointer cell and palette data. It does not
itself allocate FileEntry storage, establish B+163f8, perform type4 Set, construct
GX objects or activate the original class menu. Those owners must accept the
actual packaged bytes and preserve their source lifecycle. Do not replace the
resource with arbitrary nonzero palette/descriptor addresses. See
[original Set relocation](../file/FILE_TYPE4_TEXTURE_PALETTE_ADDRESS_SETUP.md),
[file acquisition](../file/FILE_ACQUISITION_AND_REFERENCE_REUSE.md) and
[original bubble/circle consumers](../battle/CLASS_CIRCLE_AND_SPEECH_BUBBLE_DRAW.md).

Focused Python tests verify the exact source identity/header, byte-identical
packaging, metadata and same-size corruption/truncation rejection while preserving
an already packaged valid output. An isolated full converter run produced the
new raw TPL and unchanged surface outputs under
`artifacts/battle-class-common-packaging/`. This is static asset evidence, not
normal-game menu activation, a Dolphin observation or hardware rendering proof.

Original Nintendo/Intelligent Systems rights remain retained. Tracking or
packaging original data grants no new license or redistribution rights. The ISO,
temporary extraction cache, copied original TPL, converted atlas and compiled
packs remain outside source history. See [asset provenance](../../ASSETS.md)
and [editable source workflow](../../../assets/README.md).
