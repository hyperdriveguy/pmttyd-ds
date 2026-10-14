# Audience camera4 class1/class2 callback and genuine empty bank

2026-10-14, original US G8ME01 disc0 revision0, read-only complete
BattleAudienceDispAudience `801a38f8/92c`. This documents the next actual
camera4 callback before cylinder shadows. Compose with
[footer registration](BATTLE_DISPLAY_FOOTER_REGISTRATION_AND_MUTATION.md),
[display dispatch/current-entry ownership](DISPLAY_QUEUE_AND_DISPATCH.md),
[actual audience Init](../battle/AUDIENCE_INIT_ORDER_RESOURCES_AND_LIVE_DOMAIN_BOOTSTRAP.md)
and [setup4 population proof](../battle/AUDIENCE_SETUP4_ZERO_COUNT_EXACT_OPERATION_ORDER.md).
Missing population, resource or pose providers do not establish an empty crowd.

## Captures and unconditional graphics prefix

Both callback arguments are unused. The two original registrations have
userdata0 and priority FLOAT +0 at `80424988`; that address is **not** shared
userdata or an audience-owner address. Actual current entry is the word at
`8041e61c`, returned by `dispGetCurWork80010864`.

Source first calls `BaseGetPtr801a86a0`, retaining A in r30; then
`camGetPtr8000f2fc(4)`, retaining C in r31. Neither is reselected by the actor
loop. C's view matrix at `+11c` remains borrowed live until each admitted
actor's final concatenation; it is not copied at callback entry. Next execute
these requests in exactly this order, even if every actor will be rejected:

| Site | Request, numeric original arguments |
| --- | --- |
| 801a3970 | GXSetCullMode(0) |
| 801a3974 | GXClearVtxDesc() |
| 801a3980 | GXSetVtxDesc(9,1) |
| 801a398c | GXSetVtxDesc(13,1) |
| 801a39a4 | GXSetVtxAttrFmt(0,9,1,4,0) |
| 801a39bc | GXSetVtxAttrFmt(0,13,1,4,0) |
| 801a39c4 | GXSetNumChans(1) |
| 801a39e4 | GXSetChanCtrl(4,0,1,0,0,2,2) |
| 801a39ec | GXSetNumTexGens(1) |
| 801a3a08 | GXSetTexCoordGen2(0,1,4,60,0,125) |
| 801a3a10 | GXSetNumTevStages(1) |
| 801a3a2c | GXSetTevColorOp(0,0,0,0,1,0) |
| 801a3a44 | GXSetTevColorIn(0,15,12,8,10) |
| 801a3a60 | GXSetTevAlphaOp(0,0,0,0,1,0) |
| 801a3a78 | GXSetTevAlphaIn(0,7,5,4,7) |
| 801a3a8c | GXSetTevOrder(0,0,0,4) |

Positions and Tex0 are direct XYZ/F32 and ST/F32, respectively, with no direct
vertex color or normal stream. Channel COLOR0A0 has lighting disabled,
ambient source vertex, material source register, no lights, diffuse clamp,
attenuation none. Stage0 produces RGB `TEXC + RASC`, alpha `TEXA * RASA`,
with add, zero bias, scale1, clamp and PREV destination. Do not replace the
color equation with the more usual texture-times-material equation. No
material color is set in this prefix, and no texture is fetched or bound.

No local blend, color/alpha-write, depth, alpha-compare, fog, projection,
viewport, scissor or restore requests exist. Those remain inherited from
actual class/common setup and preceding callbacks. This callback does not
call BattleAudienceGXInit; that separate function has different direct-color
and TEV inputs and cannot replace this prefix.

## Exactly200 fresh current-entry admissions

After setup, set index0 and call `BattleAudienceGetPtr801a8664(0)` once,
retaining its returned actor pointer R. This helper independently queries its
current BaseGetPtr and returns `base + 1bc + index*134`; its base need not equal
the previously retained A after an external mutation. Load constant20 and
retain negative20 for the eventual quad. Loop exactly200 records, ascending
index, advancing the retained pointer by134 bytes without another GetPtr.
Each iteration calls `dispGetCurWork` **before** actor admission:

1. Read current entry BYTE1. If equal2, reject when R.BYTE44==255. Otherwise
   reject when R.BYTE44!=255. Thus class2 admits all alpha values0..254,
   including0; every other class follows the opaque255 branch.
2. Only after passing that test, read R.WORD0 once. Reject unless flags1 and4
   are both present. There is no further local existence/state/count gate.
3. Only admitted records enter texture/resource, pose/math, material and FIFO
   work below.

The source queries current entry200 times even for original zero actors.
For class1, their alpha0 rejects before flags are read; for class2 alpha0
passes, then original flags0 rejects. A genuinely retained zero bank produced
by the full original setup4 initializer can therefore finish both callbacks
without texture/pose/matrix services, after the genuine graphics prefix and
all current-entry requests. This is a finite used domain, not an unconditional
callback no-op. Source mutations to actor alpha/flags remain fresh admission
inputs. Backend holds retain index, actor pointer, original A/C captures and
current operation; completed setup/calls must not replay.

## Admitted resource and pose path

Actor type BYTE1b<=11 takes palette from live `A.WORD0c->WORDa0->WORD0`,
reads actor frame BYTE1a, and calls `TEXGetGXTexObjFromPalette80037a80(palette,
localTexObj,frame)` followed by `GXLoadTexObj(localTexObj,0)`. Then call
GXGetTexObjWidth, mask returned value to16 bits, reread actor type, and read
row `803b4de4 + type*20`, FLOAT18. Store actor FLOAT54 as successive single
multiplications `R(R(R(row18*width)*0.015625)*actor.FLOAT60)`.
Then call GXGetTexObjHeight, mask16, freshly reread actor type/table FLOAT1c
and actor FLOAT64, and store actor FLOAT58 using the analogous three rounded
multiplications. Width mutation precedes the height getter.

For type>11, scan two guest slots in order. Compare captured type with fresh
`A.BYTE18+slot`; a match selects fresh `A.WORD10+4*slot->WORDa0->WORD0`,
then executes the same palette/load/width/height path and exits the search.
If neither matches, texture binding and actor sizes remain inherited; source
still continues to pose and quad emission. Do not fabricate a missing guest
resource or force a no-geometry outcome. Type used in size-table lookup is
reread after each dimension getter, independently of the initial guest match.

The first new nonempty native resource service is this original palette/frame
lookup (normal/guest audience TPL), not an arbitrary packed character texture.
No `animPoseDraw`, pose allocation, visibility test or character renderer call
occurs here. The callback builds the audience member's matrix directly.

Capture sizes `(R54,R58,R5c)` and pivot products
`(R(Ra8*R54), R(Rac*R58), R(Rb0*R5c))`; call PSMTXScale into local S.
Read rotations R9c/Ra0/Ra4 and multiply each by original single radians
constant `80424a20 = 0.017453288659453392`. Call double sin/cos in order
sinX,cosX,sinY,cosY,sinZ,cosZ; round each return to single. The exact source
rotation matrix rows, where x/y/z denote these rounded trigonometric values,
are:

```text
[ yc*zc - xs*ys*zs, -yc*zs - xs*ys*zc, -xc*ys, 0 ]
[ xc*zs,             xc*zc,            -xs,    0 ]
[ ys*zc + xs*yc*zs, -ys*zs + xs*yc*zc,  xc*yc, 0 ]
```

Products xs*ys and xs*yc and their subsequent products are rounded single;
row00/01/20/21 final combinations use source single FMA/FMS instructions,
not independent multiply-add rounding. Row01 is FMS(-yc,zs,(xs*ys)*zc),
row21 FMA(-ys,zs,(xs*yc)*zc). Source stores rotation matrix elements in order
00,02,10,11,12,20,22,01,21, after writing translation zeros.
Before PSMTXMultVec, write actor FLOAT90/94/98 from original vector
`802fa018 = (0,0,-1)`, in XYZ order. Multiply rotation by that actor vector
in place. This updated facing vector is subsequently read by GetFront.

Add captured pivot products to S translations (XYZ single stores), then
PSMTXConcat(rotation,S,S). Next adjust S translationX by three successive
single adds: `((-pivotX + actor48) + actorcc) + current S03`, storing X.
Z analogous uses actor50/d4, storing Z next. Y uses actor4c/d0; type freshly
read==2 adds actor130 before adding S13; other types skip that extra add.
Store Y last. Call PSMTXConcat(C+11c,S,localModelView), then
GXLoadPosMtxImm(localModelView,0). The matrix is source world pose through
captured camera4 view, not current dispatch camera supplied as callback arg.

## Front selection, live animation/color and quad

After matrix load, copy128 bytes from source UV array802fa024 into local
storage; only then call `BattleAudience_GetFront801a2b28(index)`.
That helper queries BaseGetPtr (return unused), GetPtr(index), camGetPtr(4),
and computes dot `(camera.at-camera.eye)` with freshly selected actor facing
R90/94/98. Reduction is rounded Yproduct, single FMA(X,Xf,Yproduct), single
FMA(Z,Zf,previous). Finite dot<0 returns1, otherwise0. Its actor/camera may
differ from caller's retained R/C after service mutation.

Next read retained R script-table WORD20 and script WORD24; if script equals
(table+24)'s pointed script, start UV base8, otherwise0. Capture RGBA bytes
R41/42/43/44 before angle normalization. Normalize fresh angle FLOATe8 by
source single division/truncation/FMA arithmetic into finite [0,360), storing
only on >=360 and <0 branches. Orientation bit is1 for `(0,180]`,0 for angle0
or `(180,360)`. If GetFront low8==1 and current script differs from fresh
script-table entries at offsets24,20,1c, invert that orientation bit. If bit1,
add4 to UV base. Nonfinite angle behavior is not a proved native domain.
Then call GXSetChanMatColor(4,capturedRGBA), GXBegin(128,0,4), emitting a
single QUADS primitive. No local restore follows.

Using source constants left=-20/right20/top40 and actor offsets `(R6c,R70,R74)`,
quad position order is:

```text
(-20+R6c,40+R70,R74), (20+R6c,40+R70,R74),
(20,0,0),             (-20,0,0)
```

Each position is followed by ST at local UV indices base,base+1,base+2,base+3.
Actor position fields are freshly read for each of the first two vertices;
last two do not add those fields. Exact source UV pairs by base:

| Base | Four consecutive ST pairs |
| --- | --- |
| 0 | (0,0), (1,0), (1,1), (0,1) |
| 4 | (1,0), (0,0), (0,1), (1,1) |
| 8 | (0,1.0750000476837158), (0,0.07500000298023224), (1,0.07500000298023224), (1,1.0750000476837158) |
| 12 | (0,0.07500000298023224), (0,1.0750000476837158), (1,1.0750000476837158), (0,1.0750000476837158) |

Keep the actual final pair at index15; do not silently repair presumed artwork
symmetry. No actor-next request occurs until this entire admitted quad is done.

## Evidence and native frontier

Private listings: artifacts/reference/display-class-check/DispAudience.disasm,
AudienceGetPtr.disasm, AudienceGetFront.disasm, AudienceGXInit.disasm.
Complete original body SHA256 `801a38f8/92c`:
`dbe38033161ab320e7cbc48832b98a3d4f5cc4b4b6df55b4b04b2ea7deec8d69`.
GetFront `801a2b28/98`:
`67e1a975e327bf037ba8fc9d2d9e934a55036a934d8e4809b18b0c9b89622fc7`.
Original facing12 bytes802fa018:
`4c25974396fb752c1ac68d196585032ac12032e870ec358f5663da9275026337`;
UV128 bytes802fa024:
`d94c07d90ac6ca8756a46e1f585c0b0a654dcd0aa0e3ef53226e4c4104aa46b9`.
Constants were read with startup r2=80427440; width factor80424a1c is1/64,
angle period80424a24 is360, halfperiod80424a28 is180.

The proven original zero bank can close camera4 class1 and2 after genuine
state and per-entry queries. A native shader/state adapter must actually
retain channel material-register source and the ADD-texture-color equation,
without inventing a texture load for empty actors. Existing shadow setup's
texture-times-A0 program alone is not the audience prefix. If that real native
state provider is absent, the first unavailable service is the first required
GX/state request; population emptiness does not bypass it. Nonempty resource,
mutable math/script domains and actual quads remain separate admissions.
Documentation validation: full original listing read, fixed data read rb from
ISO, `git diff --check`; no native raster or emulator equivalence asserted.
