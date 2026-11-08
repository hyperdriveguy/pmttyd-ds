# Class and submenu source texture packets

The [class provider](../../source/display/battle_class_surface_packets.cpp)
handles original circle/bubble indices 0/1; the
[submenu provider](../../source/display/battle_submenu_packets.cpp) handles 2/3.
Both translate finite source GX state and separately acknowledged FIFO scalars
into immutable packets. They do not execute a menu controller or establish
original readiness by observing native GPU residency.

## Source metadata is distinct from the native atlas

Both readers follow the live B global, B+0x163f8 FileEntry, +0xa0 pointer cell
and actual relocated palette. The original common bank is 217920 bytes, has
TPL magic 0x0020af30 and 97 eight-byte descriptors. Image headers are 36 bytes;
these paths require no CLUT. Bounds use wider arithmetic to reject a bank
crossing the 32-bit guest address space.

Class source images use I4 enum 0, while submenu uses CMPR enum 14. Native
converted atlases use PICA RGBA8 enum 0. Matching numeric values do not imply
matching storage. Both source formats occupy 32 bytes per rounded-up 8x8 tile.
The final header word packs edge/minLOD/maxLOD/unpacked bytes; unpacked 1 is
a source relocation marker, not GPU readiness. Exact packet admission further
requires the original dimensions, clamp/linear sampler and zero bias/LOD.

Actual TEX construction supplies an opaque output handle and all eight known
GX object words. Word 2 stores width-1 and height-1 in ten-bit fields; word 5
stores the original format. Handles are not source image indices or native
texture IDs. Reusing a handle supersedes old query entries. Load copies the
accepted object state before later constructor reuse.

## Class constructor snapshots and inherited state

Class queries additionally provide a typed witness attesting the exact output
address, 32 bytes and metadata at load. The provider then builds an independent
canonical immutable snapshot. Packet validation compares canonical fields and
owner identities before calling long source-bank proofs; it never rereads the
original temporary constructor cell. That temporary can legitimately be reused
or retired while the earlier packet remains valid.

The snapshot separately retains common-bank identity, callback/proof storage
and captured inherited GX history. Their control blocks need not match each
other. Neither these proofs nor the camera proof substitute for a resident
native atlas generation. Native/source retirement still rejects old packets.
Callback storage must remain alive without creating a context/publication cycle.

`ordinaryCurrent` initially missing can hold until real inherited state is
available. Once proven, its observed loss poisons the class invocation. Optional
`ordinaryCapture` snapshots the actual one-stage inherited state at LoadTexture;
legacy fixtures retain their explicit proof callback. Submenu also captures
that snapshot when supplied. InitSub alone does not establish all inherited
fog, swap, write-mask or indirect registers.

## GX state masks and material colors

Fifteen local setter bits form 0x7fff. Descriptor and format masks use bits
2/4/8 for POS/CLR0/TEX0, totaling 14. These masks recognize acknowledged source
state; they are not GX enum operands. Inline comments map the actual tokens
for direct attributes, depth/alpha/blend, channel control, texture generation
and TEV. ClearDescriptors resets both provider descriptor/format masks as its
finite admission policy; array-register behavior belongs to other owners.

Class RGB selects texture×raster or C0×raster; alpha remains texture-alpha×
raster-alpha in both. C0's RGB stays separate from per-vertex RGBA, and its
alpha is unused. MaterialRaster requires an actual C0 load into register 1.
TextureRaster can leave that unused value at white bookkeeping default
0xffffffff. Submenu additionally accepts GX_MODULATE, which establishes the
five associated TEV setter bits. Its accepted logical selectors NOOP 5 and OR 7
remain distinct source values even though BLEND mode ignores the logic selector.

## Held FIFO publication and long validation

Begin requires QUADS 128 (0x80), format 0, count 4 and the complete supported
state. Each vertex has XYZ floats, four high-to-low RGBA bytes and ST floats:
nine scalar acknowledgments, 36 per quad. The counter is one-based; zero is idle.
Private candidate arrays prevent partial publication. At final V, the provider
freezes one immutable packet and retains its exact final request across sink
holds. Float identity is bitwise, including signed zero. A sink must not accept
a packet and report false without owning its own deduplication state.

Currentness checks reject reentry and poison observed source/native/camera loss.
The immutable packet's long camera/allocation and resource proofs must survive
legitimate short callback completion. Source camera state retains its original
coordinate domain; native conversion happens later. Batch validation requires
one exact camera snapshot and source owner/control block/proof callbacks across
all entries. Empty submenu batches are accepted without inventing source draws.

Geometry applies the loaded affine matrix once and maps normalized ST into the
atlas cell, whose V orientation is already encoded. Class also checks every
native clip product and sum; finite offscreen, singular and transparent geometry
is retained. These are native admission policies, not measured PPC/FPSCR or
raster equivalence. Packet append is not whole source completion or GPU draw.
See [class surface packets](../behavior/display/ORIGINAL_CLASS_SURFACE_PACKETS.md),
[submenu packets](../behavior/display/ORDERED_ORIGINAL_SUBMENU_PACKETS.md),
[resource epochs](battle-gpu-resources.md) and [mixed publication](battle-publications.md).
