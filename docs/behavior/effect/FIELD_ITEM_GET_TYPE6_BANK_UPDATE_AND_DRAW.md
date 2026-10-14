# Field item-get type6 bank, update and draw

US G8ME01 disc0 revision0, read-only DOL/TPL recovery2026-10-13.
Finite [item95 get presentation](../field/ITEM95_GET_WINDOWS_MARIO_POSE_AND_ASSET_BINDINGS.md).
Original service requests and payload rules; no native raster/hardware claim.

## Base texture bank installer and actual assets

Original effTexSetup8005ee0c/114 clears effectmanager+8/+c, calls
arcOpen("effect.tpl",0,0),publishes returnedpointer+8. Nonnull calls
UnpackTexPalette and sets readiness+c=1. Null constructs original
"%s/e/%s/effect.tpl",root".",prefixjp whenGP16c==0,otherwiseus.
DVD async allocation/callback owns loading; callback8005efa8/48 unpacks
manager+8,closes retained filehandle,then readiness+c=1. This is separate
from the later effect_us language-cache bank and effect_n64 installer.

For original US1 the source file is **e/us/effect.tpl**,266784bytes,
SHA2563b62850fe75afb95eebb9fc240e20204d5ed320fba0fc956729f28405c2c789b,
118images. effGetTexObj8005ed58 routes IDs<118 unchanged through this bank;
unready+c==0 returns original dummy1x1 texture. Therefore type6draw IDs:

| ID/image | Original dimensions/format/data offset |
| --- | --- |
| 42 | 128x128,I4(0),data0x18660,paletteNULL |
| 44 | 16x16,I4(0),data0x1aa60,paletteNULL |

The dispatcher also loads image43 later, even when type6count1 makes the
additional particle draw loop empty. Do not rename these effect_n64 or US
language-bank images, or fabricate a sparkle texture from an unrelated pack.
ISO and privateconvertedassets stay outside history/distribution.

## Entry and original main-owner clock

Source effItemGetEntry80212488/2b8 type6 allocates exactlyone60-byte payload;
nameItemGet,maincallback80212188,activeflags|2. Payload fields:

| Offset | Fresh type6 value/meaning |
| --- | --- |
| 0 | kind6 |
| 4/8/c | retained authored effectXYZ (item95:MarioXYZ+(0,62,0)) |
| 10 | initial scale0 |
| 14 | .75*single(unsignedByte[8041b8b0+6]/10),byte12,retainedsingle~.9 |
| 18 | lifetime1000 |
| 1c | age0 |
| 20 | scale-mode0 |
| 34 | alpha255 |
| 38 | camera4 initially, then item95 caller overwrites5 |

EffItemGetMain80212188/300 is called through
[effect-driver update](EFFECT_DRIVER_UPDATE_AND_TEXTURE_CACHE.md).
It uses invocation counters, no milliseconds/GPFPS conversion. Do not tie it
to render invocations; simulation's admitted effect-driver calls own the clock.

If effectflags4(soft-delete) set,clear4,set payloadlifetime16. In **same call**,
if lifetime<1000,decrement by1. When resultinglife<16,set alphaword=life<<4.
If life<0,effDelete and return before age/display. Thus first soft-delete
update produceslife15,alpha240; life0 still submits alpha0; nextnegative
invocation deletes. Freshlife1000 remains1000 indefinitely until that request.

Increment age,then for unsignedage<12 set scale=unsignedByteCurve[age]/100.
Source bytes8041b878 are[0,18,38,59,78,96,110,120,121,114,105,100].
Age1 therefore.18,age8=1.21 overshoot,age11=1,age12+ retains1.
Cap age>1000 back1000. Scale-mode0 changesnothing; other modes1/2 have
separate1.1/.9 scaling and are outside original freshtype6 mode0.
No additional particle-update loop executes for count1.

Compute dispCalcZ(effectXYZ);call dispEntry(payload+38,2,
effItemGetDisp802118a0,effectpointer,z). Source caller's camera5 and display class2 are retained verbatim;
full display-manager camera/pass scheduling remains its own provider.
See [projected texture and GX state](FIELD_ITEM_GET_TYPE6_GX_STATE_AND_PROJECTED_TEXTURE.md).

## Two retained main quads and camera transforms

EffItemGetDisp802118a0/8e8 reads drawcamera via camGetPtr,rootpayloadXYZ,
scale=single(payload10*payload14),alpha payload34. Builds base
T(root)*Ry(-cameraYaw*0.01745329238474369)*S(scale),then an additional
T(2*sin(cameraYaw*2pi/360),0,-2*cos(cameraYaw*2pi/360)) left-concat.
The camera source is the callback camera parameter; do not replace it with
Mario/NPC paper yaw. Source viewmatrix camera+11c left-concats the model.
Projection/texture matrices are separately source-owned requests.

Two passes draw the same direct four-vertex quad:
XYZ/UV=(-32,32,0)/(0,0),(32,32,0)/(1,0),
(32,0,0)/(1,1),(-32,0,0)/(0,1). These are width64/height32 localunits,
not width4/height2: 80211d00 loads float64, negates and multiplies by .5
to form f30=-32; 80211d24 loads float32 into f31. Direct FIFO writes
80211e04..80211e88 retain those actual values. The earlier ±2/2 recovery
confused the separate pass offset constants with the vertex dimensions.
Pass0 further right-concats T(2,-2,0),uses alphatrunc(128*alpha/255),
retains sourcecolorword[r2+2458] RGB with replacedalpha. Pass1 uses unshifted
base,RGBffffff with alpha unchanged. The positive-SDA2 source resolves to zero-initialized .sbss2 at80429898;
see the [startup-color check](FIELD_ITEM_GET_TYPE6_STARTUP_COLOR_AND_QUAD_VERIFICATION.md).
Its startup RGB is000000; this is not a sampled Dolphin runtime value.
Quad dimensions/offsets are original localunits; scale and camera/model bridge
must be applied exactly once. No eight-star ring is drawn for count1.

## Exact retained two-stage TEV request domain

The original draw builds two texgens and two TEVstages,loads baseimage44 to
texturemap0 and42 totexturemap1. It sets TEVcolor1=e1e1e1ff and
TEVcolor2=typePalette[kind]; kind6 selects **eb7d06ff** at8041b888+24.
Color palette order swaps after the two main quads before unused particleloop.
Do not bake this orange tint into source grayscale textures or replace two
stages with ordinary texturealpha modulation while claiming original TEV.

At80211b54..c4c source request argument tuples:

| Service | Stage0 | Stage1 |
| --- | --- | --- |
| GXSetTevOrder | (0,0,0,255) | (1,1,1,4) |
| GXSetTevColorOp | (0,0,0,0,1,0) | (1,0,0,0,1,0) |
| GXSetTevAlphaOp | (0,0,0,0,1,0) | (1,0,0,0,1,0) |
| GXSetTevColorIn | (0,2,4,8,15) | (1,15,0,10,15) |
| GXSetTevAlphaIn | (0,7,7,7,7) | (1,7,4,5,7) |

These raw original enumvalues are sufficient to bind GXsemantics through its
SDK definitions; native PICAequations/numericprecision require their own
mapping. Stage output destinations, rastercoloralpha and texture coordinates
are not a singlepremultipliedbitmap shortcut. Original blend/Z/alpha/channel/
projection/texgen/triangleconsumer requests and helpers remain required draw
providers for a matchedraster claim. The native goal can first preserve typed
service requests while documentation/backend owners finish those domains.

## Evidence and limits

Fresh ignored inspection under artifacts/reference/goomba3-services:
effTexSetup,_callback_tpl,effGetTexObj,effItemGetEntry/Main/Disp. Tables,
constants and actual e/us/effect.tpl metadata freshlyread readonly ISO.
No sourceassets copied into history; no sourcecode/sharedindex/progress,
emulatorprofile or saves changed. Documentation-only; no runtime observation,
complete GX/PICAparity, hardwaretiming or resourcefailure-admission claim.
Next complete effectdraw state/texgen/
projection equation and native originalimage export/typedrequest consumption.
