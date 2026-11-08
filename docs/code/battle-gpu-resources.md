# Battle atlas admission and GPU packet ownership

The [class surface owner](../../source/display/battle_class_surface_gpu.cpp),
[submenu owner](../../source/display/battle_submenu_surface_gpu.cpp),
[footer owner](../../source/display/battle_class_footer_gpu.cpp) and
[enemy HP owner](../../source/display/battle_enemy_hp_gpu.cpp) admit distinct
native atlases. Original source indices, native texture slots, package rows and
PICA format values belong to different domains.

## Package identity and index domains

| Owner | Source indices | Native atlas | Catalog layout |
| --- | --- | --- | --- |
| Class surfaces | 0/1; class icons 334..343 | Slot 0: 256x128; slot 1: 256x256 | Surface cells 40x40 at (2,2), 128x80 at (46,2); icon cells 48x48, four columns, 52-pixel pitch |
| Submenu | Common bank 2/3 | Slot 0: 32x32 | Cells 16x24 at (2,2), 8x24 at (22,2) |
| Footer | Common bank 4..16 | 512x256 | Parsed by the separate footer catalog owner |
| HP | Common bank 17/18 | 128x32 | Cells 40x16 at (2,2)/(46,2) |

All four admit native RGBA8, PICA enum 0. Original CMPR images use source enum
14. Class/submenu additionally validate one whole, unrotated subtexture with
exact UV endpoints (0,1,1,0). HP/footer import descriptors expose only identity,
size and format; their API does not establish that stronger subtexture check.
Catalog V endpoints already encode the Tex3DS orientation. Mapping normalized
ST into a region must not flip V again.

HP's `PMBEHP01` is exactly 68 bytes: eight-byte magic, a little-endian count,
then two 28-byte rows containing index/width/height and four binary32 UVs.
Submenu's `PMBSUB01` reader checks both original indices and exact EOF. Its
existing parser reads native little-endian `BattleClassRegion` layout directly;
it is not a generic cross-endian serialization reader. Exact package layouts
are admission policies, not measured GX framebuffer equivalence.

## Retirement contracts differ

Class/submenu owners publish immutable catalog and texture identities together
only after every import passes. Their callback guards refuse recursive loads;
retirement during import cancels the candidate. Each nonnull imported identity
is an acquisition requiring release, even for a malformed descriptor. A rejected
aliased pair still represents two acquisitions. Before normal release callbacks,
these owners mark the generation nonresident and clear current ownership.
Copied leases remain invalid even if a future import uses the same address.
Their destructors retire the resource while callback storage must still exist.

HP/footer instead use a fresh shared byte allocation as the epoch token. The
byte's value zero has no semantic ID; identity comparison distinguishes imports.
Those portable owners require explicit `retire()` and have no releasing
destructor. They invoke release before clearing local state and do not guard
reentrant callbacks. Callers must retire packets first and externally serialize
load/retire/import/release. Do not transfer the stronger class/submenu callback
contract to these owners. Copied tokens and vertices preserve identity/data,
not the native GPU allocation. Reload cannot admit an old packet or provider.

Native import/release synchronizes C3D and separately owns Tex3DS metadata and
C3D pixels. Prepare outside FrameBegin, release while C3D is live, and keep real
callback owners alive. Texture lookup is ready-only; it never imports lazily.

## The HP provider recognizes complete source profiles

`readBattleEnemyHpTexture` follows the executing VM's actual B/FileEntry/a0/
pointer-cell chain, then verifies the 217920-byte common bank's relocated root,
97 descriptors and exact 17/18 headers. Header stride is 36 bytes; each 40x16
CMPR payload occupies 320 bytes. Clamp/linear/LOD and the source relocation
marker are checked before constructing an admitted object. Numeric handles 17/18
become usable only after this provider's successful TextureObject query.

The provider's masks and small profile codes are local recognition state. The
five InitSub bits form `fixed_ == 31`; POS/CLR0/TEX0 masks are 1/2/4. Generator
count 2 initially means unconfigured. Channel, RGB, alpha and order codes 1/2
select the admitted solid/textured or raster/modulate alternatives; they are
not passed through as GX enumerations. Inline comments map the actual GX
operands, including null texcoord/map 255, identity matrices 60/125, ALWAYS 7,
DIRECT 1 and the distinct RGB/alpha ZERO tokens 15/7.

Begin admits QUADS 128 (0x80), format 0 and four vertices. Solid vertices have
seven scalar lanes (XYZ floats, RGBA bytes); textured vertices add ST for nine.
Thus each quad acknowledges 28 or 36 lanes. The one-based counter denotes the
pending scalar, with zero idle. Every new lane is applied to private candidate
arrays. A refused final sink append commits neither that lane nor any partial
quad, so retry preserves the exact source boundary and publication order.

## Presentation consumes copied packets

HP/footer geometry applies a row-major affine 3x4 matrix and maps normalized
ST into the admitted atlas region. HP solid packets use null texture/index 0/
zero UV, but still require the current HP epoch. Batch preflight rejects invalid
identity, generation, material, nonfinite geometry or out-of-cell UVs before GPU
mutation. Empty HP batches need no resource; a nonempty invalid tail rejects
the batch as a whole. Footer preflight validates its dedicated packets and
allowed ordinary packet kinds/flags; ordinary resources need their own checks.

Native attributes follow XYZ/ST/RGBA, with packed color bytes normalized by
255. Three sampler slots remain valid, and all six TEV stages are reset. HP uses
ALWAYS depth with color-only writes and no alpha rejection; its alpha blend
uses source alpha on both channels. Footer uses the class UI profile: no depth
test, alpha reference 8, RGB SRC_ALPHA/ONE_MINUS_SRC_ALPHA and alpha
ONE/ONE_MINUS_SRC_ALPHA. These are implementation profiles, not a new claim
of original PE or hardware pixel parity. Caller owns target, projection,
FrameBegin, C2D flush/restoration and source-ordered dispatch.

See [HP resources and ordered batches](../behavior/display/ORIGINAL_ENEMY_HP_GPU_ASSETS_AND_ORDERED_BATCH.md),
[footer composition](../behavior/display/NATIVE_ORIGINAL_FOOTER_GPU_COMPOSITION.md),
[submenu surfaces](../behavior/display/RETAINED_SOURCE_WINDOW_AND_SUBMENU_SURFACES.md)
and [retained plane helpers](graphics-font-planes.md) for subsystem contracts.
