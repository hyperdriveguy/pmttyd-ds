# Field item-get type6 GX state and projected texture

US G8ME01 disc0 revision0, read-only DOL and original TPL recovery2026-10-13.
Finite kind6/count1 complements [entry/update/quad contract](FIELD_ITEM_GET_TYPE6_BANK_UPDATE_AND_DRAW.md).
Static source requests and backend mapping; no Dolphin raster or hardware claim.

## Explicit GX state

EffItemGetDisp802118a0 starts with these requests:

| Source callsite | Request | Meaning |
| --- | --- | --- |
| 8021194c | GXSetBlendMode(1,4,5,0) | blend, source alpha / inverse source alpha, ADD blend |
| 80211954 | GXSetZCompLoc(1) | depth comparison before texture/alpha test |
| 8021196c | GXSetAlphaCompare(7,0,0,7,0) | ALWAYS AND ALWAYS, no alpha rejection |
| 8021197c | GXSetZMode(1,3,0) | depth test enabled, LEQUAL, depth writes disabled |
| 80211c84 | GXSetNumChans(1) | one raster color/alpha channel |
| 80211ca4 | GXSetChanCtrl(4,0,0,0,0,0,2) | COLOR0A0, lighting disabled, material REG, no lights |
| 80211cac | GXSetCullMode(0) | no face culling |

At80211da8/80211ddc GXSetChanMatColor(4,passRGBA) supplies the shadow/main
raster color, not a per-vertex attribute. GXClearVtxDesc80211cb0 then enables
only position9 and TEX0 attribute13 as DIRECT. Format0 for each is float32:
positionXYZ and textureST, zero fractionbits. GXBegin(128,0,4) at80211e00
submits one QUADS primitive per pass, with the vertex order in the linked contract.
Native triangles must preserve this order and the two-pass submission order.

Source manager [dispatch](../display/DISPLAY_QUEUE_AND_DISPATCH.md) supplies
GXSetColorUpdate(1), GXSetAlphaUpdate(0) for camera5, ZScaleOffset(1,0), and
class2's matching blend/depth/alpha defaults before invoking this callback.
The callback repeats its explicit settings; there is no local restoration.
It does not locally reset fog, TEV swap tables/selectors, indirect stage state,
scissor or viewport. Those inherited owners remain bounded dependencies.
A native implementation may explicitly neutralize its own unrelated PICA state,
but must not present that isolation as proof of the original inherited state.

## Texture object metadata and actual installed filtering

Original e/us/effect.tpl (SHA256
3b62850fe75afb95eebb9fc240e20204d5ed320fba0fc956729f28405c2c789b)
image headers, read from the local extraction and verified source pack:

| Image | Headeroffset | Width x height | wrapS / wrapT | min / mag filter |
| --- | --- | --- | --- | --- |
| 42 | 0x9a4 | 128x128 | 2 / 2, MIRROR | 1 / 1, LINEAR |
| 43 | 0x9c8 | 16x128 | 2 / 2, MIRROR | 1 / 1, LINEAR |
| 44 | 0x9ec | 16x16 | 1 / 1, REPEAT | 1 / 1, LINEAR |

All are I4, no palette, minLOD=maxLOD=0, bias0, edgeLOD0, unpacked0.
TEXGetGXTexObjFromPalette80037a80/0xdc computes mipmap=(minLOD!=maxLOD),
therefore0. It passes original wrap to GXInitTexObj802b72b0, then explicitly
GXInitTexObjLOD802b7544 with min/mag/bias/edgeLOD from the header,
min/maxLOD converted from bytes, biasClamp0 and maxAniso0 (1x).
These filters are installed afresh when effGetTexObj resolves the ready bank.
RGBA conversion must preserve I4 intensity as RGB and alpha; do not tint assets.
Image43 is loaded after the main quads, but count1 draws no particle using it.

## Two independent coordinate streams

The source does **not** sample both textures using the quad's supplied UV.
GXSetNumTexGens(2) at80211b04 followed by:

| Callsite | GXSetTexCoordGen2 arguments | Coordinate source |
| --- | --- | --- |
| 80211b20 | (0,1,0,30,0,125) | generator0, MTX2x4, POS, TEXMTX0 |
| 80211b3c | (1,1,4,60,0,125) | generator1, MTX2x4, TEX0, IDENTITY |

Both normalize0 and postmatrixDTTIDENTITY125. Texturemap0/image44 uses
generator0, a projection of local vertex POSITION. Texturemap1/image42 uses
generator1, retaining directly suppliedST. Reusing UV0 for image44 loses the
original projected/repeated pattern and is not an equivalent two-texture shader.

## Exact projected matrix construction

Stack+7c is world base M, retained from the preceding source model construction:
T(camera-relative shift) * T(effectroot) * Ry(theta) * S(scale*size).
The shift is (2*sin(cameraYaw*2pi/360),0,-2*cos(cameraYaw*2pi/360)).
Theta=single(-cameraYaw*0.01745329238474369). Matrix M excludes source
camera+11c view and the shadow pass's later T(2,-2,0).

Before matrix setup, original seed vectors are copied from802fe6fc..802fe71c:
eye=(0,0,1000), up=(0,1,0), target=(0,0,0). These nine original binary32 words
are 00000000,00000000,447a0000,00000000,3f800000,00000000,00000000,00000000,00000000.
Stack+13c still holds the Ry(theta) previously built802119b4.
PSMTXMultVec8029e6a4 at80211ac8 rotates ONLY eye by that matrix,
with output aliasing eye atstack+50. Up at+44 and target at+38 are unchanged.
No age, GP frame counter or animated angle is read here.

C_MTXLightOrtho8029e61c at80211ab8 receives f1..f8:
(0,6,0,6,.5,.5,.5,.5), output stack+dc.
Here top0,bottom6,left0,right6, sScale=.5,tScale=.5,sBias=.5,tBias=.5.
The helper8029e61c..8029e69c uses explicit binary32 subtract/divide/multiply/add;
its mathematical affine matrix P is:

```
[ 1/6    0    0    0 ]
[  0   -1/6   0    1 ]
[  0     0    0    1 ]
```

1/6 is the helper's rounded calculation ((2*(1/6))*.5), not a claim that
native reassociation/constants are bit-identical. Source helper constants
80429358=1,8042935c=0,80429368=2 were freshly checked.
C_MTXLookAt8029e330 at80211adc builds V atstack+ac from rotatedeye/up/target.
For ordinary finite yaw, mathematical V has rows approximately
(cos(theta),0,-sin(theta),0), (0,1,0,0),
(sin(theta),0,cos(theta),-1000). SDK normalization/trigonometric/paired-single
rounding remain separate numeric dependencies; preserve original helper requests
when strict parity is required.

PSMTXConcat(P,V,P)80211aec then PSMTXConcat(P,M,Q)80211afc,
Q atstack+10c. GXLoadTexMtxImm(Q,30,1)80211b4c loads a2x4 matrix once.
For localvertex (x,y,z,1), image44ST=(dot(Qrow0,vertex),dot(Qrow1,vertex));
there is no STQ perspective divide for this generator.

Each pass independently computes camera+11c * M into stack+19c at80211d64
or80211dbc. Shadow then right-concats T(2,-2,0) at80211d88;
GXLoadPosMtxImm(stack+19c,0)80211de8 loads position matrix0.
The texture matrix is **not** rebuilt for the shifted shadow. The projected
pattern moves with the same authored ST while the shadow geometry moves.
Do not put drawcamera view into Q, apply shadow offset to Q, or translate
source effectroot twice through a native projection bridge.

## TEV meaning and native consumption

Two-stage tuples in the linked contract use ADD,biasZERO,scale1,clamp enabled,
outputPREV for color and alpha. GXSetTevColor numeric register1 initializes
GX_TEVREG0, referenced by color operand2 (C0), to e1e1e1ff;
numeric register2 initializes GX_TEVREG1, operand4 (C1), to eb7d06ff.
Stage0 RGB interpolates gray C0 to orange C1 using projected image44 RGB.
Its alpha is zero. Stage1 RGB multiplies previous RGB by channel0 raster RGB;
alpha multiplies image42 alpha by channel0 raster alpha. Stage0 channel255
is COLORNULL; stage1 channel4 is COLOR0A0. Neither original image42 RGB nor
image44 alpha contributes to this finite two-quad result.

Native C3D can use separate texturecoordinate attributes or generate projectedST
in its vertex shader, two texturebindings and PICA combiner stages for the
interpolation/modulation. Keep source tex44/image42 wrap/filter distinct,
straight alpha blending, LEQUAL with no depth writes, and materialRGBA per pass.
This maps service semantics; GX's intermediate fixed-point interpolation,
filtering and PICA precision are not proved identical.
Enum interpretations cross-checked against primary
[devkitPro libogc GX definitions](https://github.com/devkitPro/libogc/blob/master/gc/ogc/gx.h).

## Camera dispatch and projection owner

Payload+38=5 is the camera byte passed to dispEntry, not a rendering-layer enum;
class byte is2. dispDraw(5) invokes effItemGetDisp with r3=5,r4=effectpointer.
camGetPtr(5) supplies cameraYaw+114 and viewmatrix+11c inside the callback.
Depth-sort calculation still independently uses camera4 through dispCalcZ.

camDraw8000f454/0xf4 sorts then visits13 camera slots, skips camera flags&1,
sets viewport from camera+fc..110 and scissor+f4..fa. Before dispDraw(cameraID),
GXSetProjection(camera+15c,camera+19c) at8000f504 installs its owned projection.
A conditional perspective rebuild precedes it when flags0x40 and projectiontype0.
The item callback never installs a new scene projection. Its LightOrtho is
solely the texture-coordinate projection. Native scene-camera5 production and
source-to-screen projection remain external; a finite renderer can consume its
current world camera as an explicit adapter without claiming complete camera parity.

Inspection artifacts stay ignored under artifacts/reference/goomba3-services/:
original effectdisp, TEXGetGXTexObjFromPalette, C_MTXLightOrtho, dispEntry,
dispDraw and camDraw. No original bytes, ISO, images or compiled assets committed;
no emulator profile/saves modified. Documentation-only, source/native raster
comparison and hardware profiling remain pending.
