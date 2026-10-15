# Source GX objects, palettes and Window setup

These [GX constructors](../../source/display/source_gx_texture_object.cpp),
[palette producer](../../source/display/source_palette_texture_object.cpp) and
[Window loader](../../source/display/source_window_texture_setup.cpp) retain the
original CPU reads, stores and children. They do not import native textures or
submit a draw. See [cold UI](native-ui-startup.md), [Window packets](weapon-window-bridges.md)
and [shared font state](shared-font-state.md).

## SDK object fields and arithmetic

`GXInitTexObj` clears exactly 32 actual output bytes through the whole source
memset child. An engaged return zero acknowledges that child, including its real
stores. Every later read-modify-store stays separate. The insertion helper uses
PPC MSB-numbered rotate/mask fields; do not interpret those mask indices as C++
LSB bit positions.

In logical LSB terms, object word 0 packs wrap S/T, default linear magnification
and minification fields. Mipmap tests the low byte; exact full formats 8..10
choose filter field 5, others 6, while no mip uses 4. Byte 31 mask 1 marks mipmap;
mask 2 is set at Init completion. Max LOD scans the larger low-16 dimension,
starting at `0xffffffff` for zero; no new zero-size admission guard is added.

Object +20 stores full format. Word +8 packs low-ten-bit width-minus-one and
height-minus-one plus low-four-bit format. Word +12 encodes the image address
in 32-byte units via the original rotation. The format jump table uses low four
bits, indices 0..14 and default 15, selecting kind byte +30 and tile dimensions
8×8, 8×4 or 4×4. Halfword +28 contains the wrapping rounded tile-count product
masked to `0x7fff`. An unsupported captured jump target stays pending even if
its source table later changes.

LOD preserves original ordered comparisons: bias below -4 uses -4, at or above
4 uses the original 3.99 constant, then scales by 32. Min/max LOD independently
clamp to 0..10 and scale by 16. NaNs remain unordered until the genuine conversion
provider admits them. Actual constants stay fresh at their original reads;
no host clamp, exceptional conversion result or FPSCR parity is invented.
The full unsigned min-filter argument indexes the original byte table with
32-bit address wrap; it is not locally clamped to the usual enum range.

Capture order matters across conversion holds. Init saves word +4 before max
LOD conversion. LOD saves word 0 before bias conversion. For min/max, it writes
min into word +4 before max conversion, then reloads that word afterward.
Foreign mutations and aliases therefore have distinct observable effects.
See [SDK contract](../behavior/display/RETAINED_SOURCE_GX_TEXTURE_OBJECT.md).

## TPL header capture and private objects

Palette entry offset is wrapped `index << 3`. The producer captures palette+8
once, reads its image-header entry, then height/width/format/image/wrap fields
and unsigned min/max bytes at decimal offsets 33/34. Their inequality selects
mipmap. The source provides no count/index clamp here.

After actual Init completes, it reloads the header from the captured table/offset,
not palette+8. An Init write aliasing that table slot can change the LOD header.
The original unsigned conversion bias is one actual 64-bit load at `0x802c0140`.
Min and max independently use `0x4330000000000000 | byte`, then subtraction
rounded to binary32. Filters, raw bias bits and edge byte load at their source
points; biasClamp and anisotropy remain literal zero. The real nested SDK
continuations preserve completed stores and conversions.

`SourcePrivateTextureObject` is an optional 32-byte output domain. Binding needs
a genuinely reserved address and current strong owner, allocates no VM storage,
and starts every byte unknown. Only real size-1/2/4 big-endian stores make bytes
known. A full eight-word snapshot requires all bytes. Test `overlaps` before
falling back to other memory: partial overlaps and unknown owned bytes remain
unavailable, even if another provider supplies them. Retirement/current loss is
sticky; the object cannot rebind. See
[palette/private-object contract](../behavior/display/RETAINED_PALETTE_TEXTURE_OBJECT.md).

## Window archive, DVD callback and relocation

Globals `0x8041ea18/1c/24` are palette/completion/object array. Setup clears the
first two, calls original archive lookup, then branches on that actual result.
An archive hit relocates and allocates `count*32` with word wrap, constructs every
actual object and returns without writing completion one. Its loop reloads the
palette/count at each test and object global at each body.

An archive miss uses the real root, typed sparse private path, DVD Open/Length,
rounded `(length+31)&0xffffffe0` heap allocation, palette publication, handle
self-store at +`0x6c`, and actual ReadAsync with callback `0x8007fcf0`. A completed
scheduling child does not execute that callback. Old object-array bytes remain
while it waits. Source zero handles/allocations remain source values.

The actual callback reads second-argument +`0x2c` handle first, then palette and
relocation. It closes that captured handle before fresh palette/count/object
allocation and the late-global loop. Only all objects completing reaches the
completion-one store. Use separate Setup/callback cursors if real DVD scheduling
can call back before ReadAsync returns.

Relocation checks exact magic `0x0020af30`; mismatch reaches genuine panic with
original filename, line 36 and format. A returning child continues the source
graph. Table >`0x80000000` returns; equality still relocates. Nonnull image and
palette headers add the base with word wrap; nested data +8 relocates only when
its own unpack byte is zero (image +`0x23`, palette +2). Fresh table/header reads
precede the flag-one stores. Index increments as a word but compares low 16 bits
against a fresh word count, using offset `(index<<3)&0x0007fff8`. Extreme counts
can wrap/repeat; valid resource-memory admission belongs to the native owner.
See [Window contract](../behavior/display/RETAINED_WINDOW_TEXTURE_SETUP.md).

All controllers pin function/input/provider and strong control-block identity.
Successful source operations survive holds; reset drops native metadata without
undoing clear, relocation or DVD scheduling. CPU completion, source Window-ready
history, image/object provenance, camera admission and native texture residency
remain separate proofs.
