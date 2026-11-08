# Graphics program, glyph and texture-plane ownership

[The process graphics program](../../source/display/native_graphics_program.cpp)
protects SDK shader metadata across renderer lifetimes.
[Font GPU resources](../../source/display/battle_font_gpu.cpp) admit resident
font identity and retained glyph packets.
[Texture planes](../../source/display/battle_texture_plane.cpp) retain the source
SDK/GX/FIFO sequence. These owners have different completion and lifetime rules.

## A stable program survives scene shader destruction

The existing [world shader](../../source/runtime/world.v.pica) has vertex inputs
v0 XYZ, v1 UV and v2 RGBA. Four uniform rows produce clip coordinates with
homogeneous W=1; CPU code supplies UV orientation and color. Its ABI is already
commented in the shader and is unchanged here.

The parking owner independently parses those compiled bytes, selects DVLE 0 and
initializes a real shaderProgram. Preparation happens after C3D_Init but before
C2D/scene programs can become cached. Every failure releases its own partial
CPU metadata, with program instances freed before their DVLB backing.

The installed SDK retains the previous program pointer across graphics Fini/
reinit and dereferences its shader metadata on a later bind. A null program is
not a supported substitute for this real binding. After GPU synchronization,
the caller binds the prepared stable program before freeing scene/C2D shaders.
The parking object survives until process exit; it neither draws nor manages
texture residency or GPU synchronization. Host builds report unavailable. See
[the shader lifetime contract](../behavior/display/NATIVE_SHADER_PROCESS_LIFETIME.md)
for the installed-SDK evidence and limits.

## Font generations own identity rather than texture allocation

Admission requires the original-US 162-glyph metadata, 22-pixel glyph height,
512x256 atlas and exact whole-image import. Native format 0 means RGBA8; one
unrotated image has UV endpoints 0/1/1/0. The resource copies immutable metadata
and creates an opaque generation, while Renderer still owns the texture pixels.
Retirement invalidates all copied leases before those pixels are replaced or
freed. Reusing a native pointer cannot revive an old generation.

Raster viewport/scissor coordinates use the tilted top-screen 240x400 space.
The admitted depth mapping has scale [-1,0] and offset [0,1]. Projection already
contains native tilt and the loaded affine matrix already contains source camera
view. Appending either a second time changes placement. The finite guard checks
the composed 4x4 and every product/sum for all four shader corners before RNG.
It does not divide by W or cull blank, offscreen, transparent or zero-area glyphs.

The sink needs explicit source matrix slot 0, TEV color slot 1, ordinary GX profile,
current resource leases and capacity. It reserves storage before the glyph
kernel's two irand(64) calls. Ordered admission/emission hooks must be paired.
After accepted RNG, void emission cannot fail, reenter or retire resources;
a violation aborts instead of silently dropping an admitted glyph. A call-local
strong resource pin defers cleanup requested before admission until unwinding.

Source operations check short callback currentness; snapshots check only the
long publication lifetime. Either checked-currentness failure poisons the sink
permanently. Finishing a camera does not itself poll short currentness during
snapshot replay, so a valid publication can outlive that callback. The sink is
one-shot and cannot rebind after retirement. Its immutable leaf publication
still does not complete a mixed menu sequence, source callback, camera queue or
GPU submission. See [resident fonts and glyph packets](../behavior/display/RESIDENT_BATTLE_FONT_AND_GLYPH_PACKETS.md).

## Plane helpers retain source requests and borrowed values

One cursor distinguishes Plane2, Init-only and PlainGX-only. Request part,
provider identities and borrowed base/RGBA/color-array objects stay fixed across
holds. Values are read at their source usepoints: Plane2 copies its base after
Init and reads RGBA after matrix upload. PlainGX performs neither matrix work
nor Init, and reads ahead individual bytes from its four borrowed color words.
reset() discards cursor state without undoing committed SDK/GX/FIFO effects.

Init uses direct position/color0/UV0 attributes (9/11/13), format 0, source
vertex material color and one texture generator. GX enum operands preserve
XYZ/F32, RGBA/RGBA8 and ST/F32 formats; they are not arbitrary native flags.
Exact texture format 0 selects raster RGB and raster-alpha×texture-alpha; every
other format takes the source MODULATE branch. It does not classify all intensity
formats under that first branch.

The actual battle common palette is resolved through B+163f8 → FileEntry+a0
holder → payload. TextureObject returns an opaque handle; load/format/dimension
queries use that retained handle rather than a source index or fabricated GPU ID.
Width and height each read fresh source half and unsigned-conversion bias,
using LOW16 dimensions. Zero dimensions are not rejected by this source helper.

GXBegin primitive 128 (0x80) is a four-vertex quad. Each vertex writes XYZ floats,
RGBA bytes and ST floats: nine scalar acknowledgments, 36 total. Live ZERO/ONE
and color-byte read-ahead stay in original order. A held FIFO write retains its
already loaded scalar; eager quad snapshots and late-at-store reloads differ.
Engaged return 0 can acknowledge a void child; empty optional alone means hold.
See [the linked raster contract](../behavior/display/ORIGINAL_ENEMY_HP_LINKED_RASTER_HELPERS.md)
for source identities and exact ordering. Source helper completion does not
establish real GPU resources, queue draining, original pixels or hardware timing.
