# Original enemy HP assets and ordered GPU batches

2026-10-14, US G8ME01 revision0. This increment supplies a separate native
resource owner for original common-bank17/18 and an ordered GX/FIFO packet
provider for textured HP bars and the source's untextured raster highlight.
It consumes the [complete retained HP linked helpers](ORIGINAL_ENEMY_HP_LINKED_RASTER_HELPERS.md).
It does not install normal callback scheduling, render the lower screen as a
HUD panel, or change physical controls.

## Original asset provenance and conversion

`tools/battle_enemy_hp_assets.py` opens the user-provided ISO read-only through
existing extraction services and requires exact original
`battle/common/battle_common.tpl`,217920bytes, SHA256
8eb685317056f42b3ef73138eaad85d16fa4943d830b44a149f09971a5959bb1.
The original root is0020af30/count97/table0c. Actual HP bar instructions select
17 then18; see the [queued HP source contract](QUEUED_ENEMY_HP_PRESENTATION_AND_RETAINED_BAR.md).

| Bank index | Header/data hex | Dimensions/format | Compressed320-byte SHA256 | Decoded RGBA SHA256 |
| --- | --- | --- | --- | --- |
| 17 | 578/b100 | 40x16 CMPR14 | 1968aec5606a2dc326d272efa0602a98ebd08ab15a99b53014e8bedd26a73ddd | 250d1bb73ad77cf583cef3877e9c541e5525ba819163d3ece3dd098bb656d6a7 |
| 18 | 59c/b240 | 40x16 CMPR14 | 21c140ae6dae0bda018f9468b364fb733789d5ba60deacb8879acc8e516bc202 | d12e0bb295cd923f953c59fde90a3012f94d95f3a3f275f521bc9f8aa5b0806a |

The new ignored products are `battle-enemy-hp-surfaces.bin/.t3x/.json` under
the chosen RomFS output. PMBEHP01 is a separate68-byte little-endian catalog:
8-byte magic, count2, two28-byte index/width/height/four-float-UV records.
The128x32 RGBA8 atlas contains original unmodified decoded40x16 cells at
(2,2) and(46,2), with2px transparent padding and no resampling/recoloring.
Both original images' complete outer borders are transparent RGBA0, so that
padding preserves their clamp boundary color. UVs are source-region boundaries
with the existing tex3ds V orientation. GPU storage is16384bytes. Original
min/mag filters1 map to linear; wrap0 maps to clamp-to-edge.

The converter pins original whole-bank identity, exact header offsets,
dimensions, CMPR blocks and their hashes before decoding. It retains decoded
pixel hashes, source path/hash, conversion and rights in generated JSON.
Nintendo/Intelligent Systems retain original rights; no redistribution license
is granted. The ISO, extracted TPL/images, atlas and compiled products remain
ignored/private; no original binary is committed. The
[common packaging contract](BATTLE_CLASS_COMMON_TPL_PACKAGING.md) and
[asset register](../../ASSETS.md) remain authoritative for the source bank.
The existing footer4..16 PMBCLF01 ABI is unchanged.

## Resource and source provider ownership

`BattleEnemyHpGpuResource` validates the entire exact PMBEHP01 catalog before
calling a supplied real importer. Admission requires actual nonnull texture
identity,128x32 dimensions and native GPU_RGBA8 format0; malformed imports are
released through their owning callback. Each successful import receives a new
shared generation token. Explicit `retire()` releases the actual GPU owner;
stale copied packets/providers then fail generation validation, even when a
new import reuses the same texture address.

`readBattleEnemyHpTexture` reads the executing VM's actual live B global,
B+163f8 file descriptor, owner+a0 pointer-cell and actual palette identity. It
checks overflow and missing memory, original count/table, exact relocated17/18
header/data pointers, actual40x16/CMPR metadata, clamp/filter/LOD/bias fields
and source type4 Set marker. It does not extend footer-only readers or make a
fake palette. `BattleEnemyHpGpuProvider` calls its actual texture query and
constructs a native opaque object only after those dimensions/format/sampler
values are supplied. Its index-valued handles are local to that admitted owner;
a raw17 or18 cannot be loaded/queried before its TextureObject call succeeds.
The actual source current-camera provider remains mandatory.

The provider retains canonical original GX state, including whole InitSub's
color-update/alpha-compare/blend/depth setters, descriptors/attribute formats,
channels, texgens and exact TEV operations. Begin requires the complete proper
textured or untextured state, real matrix and ordered sink. TexPlane requests
use36 FIFO lanes; QuadRasta uses28 lanes without ST. A complete candidate quad
is built privately and the final FIFO lane is acknowledged only when the sink
copies that packet. Sink holds retain all preceding lanes and final operands;
no partial geometry or duplicate accepted quad is published.

`BattleEnemyHpGpuBatch::sink()` copies packets at their original publication
position. A positive-timer bar therefore retains background17, untextured
black-alpha highlight, fill18 order. The solid packet has explicit
UntexturedRaster material, null texture and index0; it never impersonates a
footer/HP image. Every packet, including solid, carries the same dedicated HP
resource generation. Matrix transforms, per-vertex source RGBA, atlas UVs and
material are copied into each packet. Only finite geometry and valid region
UVs are admitted; unavailable geometry holds rather than claiming a draw.

Build a mutable batch during the actual queued callback, then copy it into an
immutable publication after source completion. `validateBattleEnemyHpBatch`
preflights every packet's resource identity/generation/material/geometry before
presentation. A bad final packet rejects the whole batch. Publication owns its
copied vertices/tokens; the actual GPU resource must outlive consumers and be
retired explicitly before C3D shutdown.

## Native binding and remaining frontier

`NativeBattleEnemyHpGpu` imports the actual t3x with FrameSync outside
FrameBegin, then owns its C3D texture/import. `draw(batch,shader,uniform,
projection,parkedSampler)` performs whole-batch validation before any GPU
mutation, uses the existing XYZ/ST/RGBA shader layout and draws packets in
source order. Each packet sets its own RGB/alpha material: modulate texture
with source color, raster RGB with texture alpha, or untextured raster RGBA.
It applies source no-cull, enabled ALWAYS depth test without depth writes,
ALWAYS alpha compare and source-alpha/inverse-source-alpha RGB blending.
Target color writes are enabled; this adapter does not claim full Gekko PE
framebuffer-alpha inheritance or GX/FPSCR bit equivalence. Caller owns the
actual target, projection, FrameBegin, C2D flush and restoration.

The real queued HP composition can pass `provider.request` as the existing
`BattleTexturePlaneServices.request`; its actual VM actor memory and original
SDK math owners remain unchanged. The provider and its referenced resource,
query callbacks, sink and publication storage must remain alive through all
holds. Native source frame scheduling, actual camera/projection, source unit
HP/Tattle/status producers, number/icon resources and renderer publication
wiring remain separate real owners. This batch preserves bar primitives; the
separate number/icon backend must join the same renderer dispatch order (or
retain source-order packet-range markers) before claiming complete callback
interleaving across units. Rendering all numbers followed by all bars would
lose that original ordering. No production Renderer/main/header or
CMake registration is added by these isolated files. The current tests prove
the portable source/resource graph, not normal on-device battle pixels.

## Exact focused validation

Executed private extraction/conversion:

```
python3 tools/battle_enemy_hp_assets.py --output artifacts/battle-hp-assets/romfs --work artifacts/battle-hp-assets/work
python3 -m unittest discover -s tests -p test_battle_enemy_hp_assets.py
```

The two Python tests reject unverified sources and verify exact atlas pixel
preservation, transparent padding, dimensions, separate catalog/UV ABI and
RGBA8 compiler arguments using explicit fixture images. The actual ISO run
verified the original compressed/decoded hashes above.

Strict C++ tests load the actual generated catalog, use the actual read-only
original TPL relocated by the existing whole source type4 controller, and run
the retained TexPlane/Rasta helpers through this provider and actual shared SDK
math adapter. They verify original descriptor admission/malformed marker/
dimension/palette rejection; missing importer/camera/texture; bad GPU import
cleanup; unsupported indices; premature Begin; held final lane/sink retries;
malformed/interleaved lanes; source background/solid/fill ordering and fixed
transformed coordinates/colors; copied publication independence; stale/missing/
corrupt final-packet whole-batch rejection and same-address resource reload.

Warning-enabled host and ASan/UBSan tests pass with leak scanning disabled;
actual devkitARM ARMv6K hard-float syntax passes for the native owner. Root owns
matched-source full make test/all, shared build integration, real normal queued
composition and isolated native visual validation. No complete-game or physical
hardware claim is made.
