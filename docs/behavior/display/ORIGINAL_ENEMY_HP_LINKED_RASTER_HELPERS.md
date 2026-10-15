# Original enemy HP linked raster helpers

2026-10-14, US G8ME01 revision0. `battle_enemy_hp_raster.hpp/.cpp` supplies the
complete linked source helpers requested by the
[retained queued HP bar](QUEUED_ENEMY_HP_PRESENTATION_AND_RETAINED_BAR.md).
`resumeBattleEnemyHpRaster` accepts the actual stable parent request object,
retains its source call and service ownership, and advances only acknowledged
SDK/GX/FIFO children. Unsupported functions/shapes are unavailable. Source
camera, matrices, resource palette, texture object and GPU publication remain
real required providers. This increment does not install normal queue dispatch
or claim native HP pixels are already visible.

## Texture plane and shared children

TexPlane801032c8/11c captures its original five float registers at entry, then
runs whole Init801036e4/e0, actual camGetCurPtr8000f2e4, Scale8029e2b0 with
captured XY scales and fresh original one8042224c, Trans8029e230 with captured
XYZ, Concat8029dd64(translation,scale), then Concat(live camera+11c,local).
It loads matrix0 and only after acknowledgement reads the borrowed parent's
RGBA local and duplicates it into four private words. The captured texture
index then enters whole PlainGX801033e4/300. Original r5 is unused locally.
No Plane2 call substitutes for this distinct wrapper.

The existing `battle_texture_plane` implementation now exposes independent
retained `resumeBattleTexturePlaneInit` and
`resumePackedBattleTexturePlainGX(texture,borrowedFourColors,services,cursor)`.
The original Plane2 API and request order remain unchanged and reuse the same
implementation. Cursor entry tags prevent switching bodies while held.
PlainGX does not add Init or a matrix. It reads fresh global B, original
B+163f8 owner+a0 holder, actual palette and opaque TextureObject80037a80;
LoadTexture/Format/Width/Height operate on that admitted handle. Source format0
uses raster RGB and texture-alpha multiplication; nonzero uses MODULATE.
Low16 dimensions, fresh half/binary64 conversion constants, Begin128/0/4 and
all36 scalar FIFO writes retain source ordering.

Independent PlainGX borrows four actual color words by object identity. Their
byte loads retain the original instruction read-ahead: vertex0 RGB precedes
its X FIFO, alpha follows its fresh Z load; later vertices' bytes are loaded
while preceding vertex fields are written. A held FIFO store retains its
already loaded bytes/constants. There is no early four-word snapshot or a
late-at-store byte replacement. The wrapper's four private equal copies remain
stable naturally. Matrix providers retain the same request object/operands
across suspension and must complete the actual requested SDK function.

## Untextured raster bodies

Whole InitRasta80103b48/f4 includes whole InitSub80103c3c/94:
Cull0, DepthLocation1, AlphaCompare(7,0,0,7,0), BlendMode(1,4,5,5),
DepthMode(1,7,0), ClearDescriptors, Channels1,
ChannelControl(4,0,1,1,0,2,2), followed by original position/color descriptors
and attribute formats, TextureGenerators0, one TEV stage and exact raster
color/alpha operations, Order(0,255,255,4). These state setters differ from
Plane2 Init and must reach the real material owner.

Whole QuadRasta80103918/d4 captures rectangle/colors and issues Begin128/0/4,
then all four PointRasta801039ec/28 calls in order: left/top, right/top,
right/bottom, left/bottom. Each point writes X,Y, its own freshly loaded
80422240 Z, then each original color operand's low byte. All28 writes retain
separate acknowledgement; there are no ST lanes. The direct
LoadPosMtxImm802ba2fc child preserves the actual supplied camera matrix.

## Evidence and native integration frontier

Read-only original instruction witnesses are private under
`artifacts/hp-{plane,plain-gx,init-rasta,init-sub,quad-rasta,point-rasta}-source.elf`;
`artifacts/hp-raster-source-evidence.json` records raw function spans/hashes.

| Function | Size hex | SHA256 |
| --- | --- | --- |
| TexPlane801032c8 | 11c | 63de8a897268331430cb7bf561d05f19acbed4c8b7e4463c15dc2463e7a30024 |
| PlainGX801033e4 | 300 | 86fb71e482a29f6515a04c6f72fad9801bb085c9b7ceaa39d55d91fb67c4a983 |
| InitRasta80103b48 | f4 | 40a2b14c32e28d2701abb4cd91020b2d7ff5a9d9c1986a28293ac6bd349259b9 |
| InitSub80103c3c | 94 | ce0492b2e38a644ae90088dd34d8225d16c023f31eb04e7a7d76ed763fc70d37 |
| QuadRasta80103918 | d4 | 1f78c5fece30ab5d2cfe4836c05eb93b2ef2cd9e0dcad1795132543aedab4375 |
| PointRasta801039ec | 28 | d4cd96bc8423dc2e02f8b6bd1ece1a4c8d4c61693f29a98f5c44bb668090e97a |

Binding: own one stable `BattleEnemyHpRasterCursor` and actual
`BattleTexturePlaneServices` beside the active queued bar; its request callback
calls `resumeBattleEnemyHpRaster(q,services,cursor)`. Complete a request before
replacing its original parent-owned storage. Use actual source word spans,
current-camera service, SDK math and ordered GX packet owner. A missing provider
holds; no real texture/material/quad is acknowledged by this source helper.
Whole source number80019120 is a separate required linked owner. Queue sorting,
normal source frame cadence, record lifetime and cancellation remain caller
responsibilities. Physical controls and the lower audience scene are unchanged.

Original common bank17/18 are both40x16 CMPR14: texture headers at578/59c,
image blocks atb100/b240 of320 bytes each. Compressed block hashes respectively
1968aec5606a2dc326d272efa0602a98ebd08ab15a99b53014e8bedd26a73ddd and
21c140ae6dae0bda018f9468b364fb733789d5ba60deacb8879acc8e516bc202.
The exact217920-byte common TPL is already pinned by
[packaging provenance](BATTLE_CLASS_COMMON_TPL_PACKAGING.md). Footer GPU
catalog4..16 deliberately remains unchanged. A concrete next asset increment
should export a separate `battle-enemy-hp-surfaces.bin/.t3x` with versioned
PMBEHP01 catalog, two original40x16 records and a128x32 RGBA8 atlas using2px
transparent padding, no resampling. Admit actual bank17/18 descriptor format/
dimensions against that separate native generation owner; publish textured and
untextured HP quads in source order and preflight the whole immutable batch
before GPU effects. This is a proposed next increment, not an implemented or
admitted texture owner. No original asset bytes or converted packs are committed
here; original rights and [asset tracking](../../ASSETS.md) remain unchanged.

## Validation

Strict warning-enabled host tests exercise complete independent source-state
requests, matrix/geometry/color golden values, format0/nonzero paths, each
GX/math/FIFO boundary held twice with unchanged accepted operations and source
read traces, borrowed request/service mismatch, captured entry scalars and
held Scale constant, late RGBA after matrix load, independent PlainGX's exact
color-byte read-ahead, missing providers and unsupported requests. Original
Plane2 tests pass unchanged. ASan/UBSan passes with leak scanning disabled;
actual devkitARM ARMv6K hard-float syntax passes. These are explicit host
fixtures, not a source camera or native rendering proof. Root owns CMake,
matched-source full make test/all, actual asset packaging/GPU admission and
normal queued battle integration.
