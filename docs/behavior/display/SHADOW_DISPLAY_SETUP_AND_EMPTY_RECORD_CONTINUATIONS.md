# Shadow display setup and empty-record continuations

US G8ME01 disc0 revision0, read-only static ISO recovery 2026-10-14.
This extends [shadow request/preparation](BATTLE_SHADOW_REQUEST_OWNER_AND_DEFERRED_PREPARATION.md),
[ground polygon](../overworld/OVERWORLD_MARIO_SHADOW_REQUEST_AND_GROUND_POLYGON.md),
and [air-shadow GX graph](../overworld/OVERWORLD_MARIO_AIR_SHADOW_GX_GRAPH.md).
Addresses and arguments below are original GX protocol, not native PICA enums.
Ignored complete inspection listings: artifacts/reference/display-class-check/.
No execution, raster, emulator or hardware result is claimed by this contract.

## Outer callback order for cameras1/4/5/7

shadowDisp8002d3d8 captures supplied camera r3 and userdata r4, then calls
dispGetCurWork80010864 at8002d3f8 unconditionally. For camera1,4,5 or7:

1. shadowCharShadowDisp_Polygon(camera,userdata)8002c818 at8002d420.
2. Fresh camGetCurPtr8000f2e4 at8002d424. Its return is unused locally;
   the query still occurs.
3. Complete shared cylinder setup below, in listed order.
4. cylinderShadowDraw(camera,userdata)8002bf84 at8002d618.
5. shadowCharShadowDisp_Texture(camera,userdata)8002c484 at8002d624.
6. Return. No render-state restoration is emitted.

An empty/admission-rejected record scan cannot skip steps1–5. Polygon and
Texture each set up GX before scanning. Cylinder itself has only pool/camera
queries before its scan; its per-record GX begins only after admission.

Each helper captures primary pool pointer from retained shadow owner.word108
before its camGetCurPtr call, then captures current-camera view pointer+11c.
Each loops92-byte records from index0, freshly reading owner.word104 at EVERY
loop test. Pool base remains captured while count/owner reads are fresh.
Each separately uses source float0 at8041f9d4 for its radius comparison.
Admission is ordered: radius10 compares unequal to0, then type BYTE2 equals
helper's type, then FLAGS HALF0 mask2 set, then signed HALF1a equals supplied
camera. Source NaN radius is not repaired into zero. No helper sets prepared2.

## Shared GX vocabulary and channel/TEV prefix

The following ordered prefix **C** is shared by Polygon, outer cylinder setup,
and Texture; they execute it independently, with a fresh color-owner read:

| Order | Request |
| --- | --- |
| C1 | GXSetNumChans(1)802b6f3c |
| C2 | GXSetChanCtrl(4,0,0,0,0,2,2)802b6f78 |
| C3 | Freshly read shadow owner.word100 RGBA; GXSetChanMatColor(4,&localRGBA)802b6e54 |

Prefix **T(A)** below follows texture/texgen admission where applicable:

| Order | Request |
| --- | --- |
| T1 | GXSetNumTevStages(1)802b8fcc |
| T2 | GXSetTevOrder(0,coord,map,4)802b8e30; coord/map specified per helper |
| T3 | GXSetTevColorOp(0,0,0,0,1,0)802b89e8 |
| T4 | GXSetTevAlphaOp(0,0,0,0,1,0)802b8a50 |
| T5 | GXSetTevColorIn(0,15,15,15,10)802b8960 |
| T6 | GXSetTevAlphaIn(0,7,A,5,7)802b89a4 |

Thus RGB is raster color. Polygon/cylinder A=1 selects register0 alpha;
Texture A=4 selects texture alpha; both multiply that input by raster alpha.
Register0 alpha is inherited, not initialized by these setup prefixes.

## Polygon8002c818 setup, admission and FIFO

After captured pool/current view, execute C, then:

| Order | Request |
| --- | --- |
| 1 | GXSetNumTexGens(0)802b5084 |
| 2 | T(1), coord255,map255 |
| 3 | GXSetFog(0,0,0,0,0,&localRGBA)802b8ff4; RGBA copied from80429538 |
| 4 | GXSetBlendMode(1,4,5,0)802b932c |
| 5 | GXSetZMode(1,3,0)802b93d8 |
| 6 | GXSetZCompLoc(1)802b940c |
| 7 | GXSetAlphaCompare(7,0,0,7,0)802b8d60 |
| 8 | GXSetCurrentMtx(0)802ba39c |
| 9 | GXSetCullMode(0)802b5c8c |
| 10 | GXSetArray(9,testKagePosArray80304b00,12)802b4db0 |
| 11 | GXClearVtxDesc()802b4520 |
| 12 | GXSetVtxDesc(9,2)802b3c08 |
| 13 | GXSetVtxAttrFmt(0,9,1,4,0)802b4558 |

All calls above precede first record test8002c9dc. Type gate is0.
For admitted camera1 record: sysWaitDrawSync80005ce4, GXClearBoundingBox802b69d0
BEFORE matrix/geometry. Compute PSMTXConcat(currentView,record.matrix2c,local),
GXLoadPosMtxImm(local,0)802ba2fc. GXBegin(0x98,0,6)802b5a6c, then six
position INDEX8 FIFO bytes exactly0,1,2,3,4,5. No UV/color FIFO fields.
The six original XYZ and strip winding are in the linked ground-polygon
contract. Then camera1 alone sysWaitDrawSync, GXReadBoundingBox(&left,&top,
&right,&bottom)802b6a08, and offscreenAddBoundingBox(record signed HALF18,
left,top,right,bottom)80072f38. Those postdraw queries are not optional if a
camera1 record is admitted. Other cameras skip both bounding sequences.

## Outer cylinder setup before cylinderShadowDraw

Execute C, then the following, even if no type2 record can draw:

| Order | Request |
| --- | --- |
| 1 | GXSetNumTexGens(0) |
| 2 | T(1), coord255,map255 |
| 3 | GXSetFog(0,0,0,0,0,&localRGBA), RGBA copied from80429538 |
| 4 | GXSetBlendMode(1,4,5,0) |
| 5 | GXSetZMode(1,3,0) |
| 6 | GXSetZCompLoc(1) |
| 7 | GXSetAlphaCompare(7,0,0,7,0) |
| 8 | GXSetCurrentMtx(0) |
| 9 | GXSetCullMode(0) |
| 10 | GXSetArray(9,positions80304b00,12) |
| 11 | GXClearVtxDesc() |
| 12 | GXSetVtxDesc(9,2) |
| 13 | GXSetVtxAttrFmt(0,9,1,4,0) |
| 14 | GXClearVtxDesc() AGAIN |
| 15 | GXSetVtxDesc(9,2) |
| 16 | GXSetVtxDesc(11,2) |
| 17 | GXSetVtxAttrFmt(0,9,1,4,0) |
| 18 | GXSetVtxAttrFmt(0,11,1,5,0) |
| 19 | GXSetArray(9,vertex_tbl80304ce0,12) |
| 20 | GXSetArray(11,ordinary palette80415140,4) |

The first position-array argument is source80304b00 from base80304a20+e0;
the later array replaces it before cylinder helper. First descriptor setup
is replaced before geometry too; no primitive occurs between these writes.
Type2 admission is at8002bfc8, after captured pool/current view only.
Prepared flag is tested before camera match. Unprepared type2 records do not
issue a collision ray here; only Collision owns preparation.

For admitted camera1 record, wait/clear bounding before PSMTXConcat and
GXLoadPosMtxImm; execute three complete original passes in record order;
then wait/read bounding and offscreenAddBoundingBox exactly as Polygon.
Variant BYTE3==1 emits the cube lists; all other variants call cylinder8002c2ec
for each pass. Full pass requests, imported indexed pairs, opacity math and
60-vertex pool are in the linked air-shadow contract. No source release/reset
or render-state restore occurs after the last record.

## Texture8002c484 unconditional asset/setup and conditional FIFO

After captured pool/current view, execute C, then:

| Order | Request |
| --- | --- |
| 1 | effGetTexObjN64(44,&localGXTexObj)801d78b0 |
| 2 | GXLoadTexObj(&localGXTexObj,0)802b7880 |
| 3 | GXSetNumTexGens(1) |
| 4 | GXSetTexCoordGen2(0,1,4,60,0,125)802b4e04 |
| 5 | T(4), coord0,map0 |
| 6 | GXSetFog(0,0,0,0,0,&localRGBA), RGBA copied from8042953c |
| 7 | GXSetBlendMode(1,4,5,0) |
| 8 | GXSetZMode(1,3,0) |
| 9 | GXSetZCompLoc(1) |
| 10 | GXSetAlphaCompare(7,0,0,7,0) |
| 11 | GXSetCurrentMtx(0) |
| 12 | GXSetCullMode(0) |
| 13 | GXClearVtxDesc() |
| 14 | GXSetVtxDesc(9,1) |
| 15 | GXSetVtxDesc(13,1) |
| 16 | GXSetVtxAttrFmt(0,9,1,4,0) |
| 17 | GXSetVtxAttrFmt(0,13,1,4,0) |

All precede first record test8002c69c. Type gate is1. effGetTexObjN64 does
not share effect.tpl's image44: loaded N64 bank is e/us/effect_n64.tpl.
The complete lookup801d78b0 captures pointer
wp80419a40, test state.word4. If0, GXInitTexObj(out,dummy80419a60,1,1,0,0,0,0)
802b72b0; otherwise TEXGetGXTexObjFromPalette(state.word0,out,44)80037a80.
Source initialized dummy byte80419a60 is00, wp's initialized token is8041ecd0.
The separate [effInit64 owner](../effect/EFFECT_POOL_INITIALIZATION_AND_CONTEXT_RELEASE.md)
establishes a reset; missing native N64 load history is not proof state.word4=0.
The lookup and binding occur even if every record fails the type1 gate.

For admitted camera1 record, wait/clear bounding, then PSMTXConcat and
GXLoadPosMtxImm as above. GXBegin(0x80,0,4) emits four direct XYZ/ST vertices:

| Vertex | XYZ | ST |
| --- | --- | --- |
| 1 | (-1.25,0,1.25) | (0,0) |
| 2 | (1.25,0,1.25) | (1,0) |
| 3 | (1.25,0,-1.25) | (1,1) |
| 4 | (-1.25,0,-1.25) | (0,1) |

FIFO order is XYZ then ST per vertex. Values come from float pools8041f9d4=0,
8041f9d8=.5,8041f9e4=1,8041f9f8=2.5,8041f9fc=1.25; negative corner is
source negate2.5 then multiply.5. Camera1 postdraw wait/read/bounds follows.

## Live native domain and next preparation owner

With only type2 records, prepared2 clear and record.camera4, source camera1
Polygon and Texture scans reject at type; cylinder rejects at prepared2
before camera match. They emit no primitives or bounding-box calls. Camera1
can finish through the genuine ordered setup/resource bindings above; this
does not certify type0/type1/cylinder geometry or later cameras. A whole
shadowDisp no-op is incorrect. Texture's N64 lookup/bind remains a real
provider even in this finite domain. A completed callback leaves Texture's
GX state and texture binding for subsequent source requests.

Next camera2/class5 dispatch reads the borrowed current display record's
class BYTE1 and calls shadowCharShadowDisp_Collision8002d16c FIRST at8002d660.
Collision captures pool/current camera, iterates fresh count and skips radius0
or prepared2. With type!=3 and flags1, it queries actual field collision from
XYZ+(0,1,0), direction(0,-1,0), length150, filter8002d380. Miss skips without
setting prepared2. Hit builds actual normal/point matrix before OR prepared2.
Type3 or flags1 clear uses its distinct translation/scale bypass. The battle
caller floor result cannot substitute this independent query; once Collision
finishes, camera2/class5 has additional image/projection services before return.

## Whole-body evidence

All ISO reads used rb; DOL header mappings and US symbol sizes cross-checked.

| Body/address/size hex | SHA256 |
| --- | --- |
| shadowDisp8002d3d8/7b4 | ade549cabd0dcc3d9d69acec14ed8f09c77ff4b4c2e6e063a1c7ac1ce36819ad |
| Polygon8002c818/2dc | ad7142c312d261f55e02371353709d5e754c2e04ec19cbbbf51e710d70b9ffaa |
| Texture8002c484/394 | 93e96eb0a02ed5da8e7d0d4d05b85446b8a6bede90fb668f083fc022f25a403f |
| cylinderShadowDraw8002bf84/368 | 9997a7bc2fa9a50f2dcca8aa45b77b8dd43841e067d4cd4315093d608cab49b5 |
| effGetTexObjN64801d78b0/60 | 3532b321a207e74d818bdabd00a2a0415261ead29507030b0eac166bd6f5d08f |
