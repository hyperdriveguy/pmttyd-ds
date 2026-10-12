# Animation texture coordinate matrices and literal component gates

US G8ME01 disc0 revision0;documentation ID1,2026-10-12. Static read-only
binary analysis. `tools/verify_animation_texture_light.py` checks fourteen
complete local bodies,1189 instruction words,eleven binary32 constants,eight
original spans,two US lecture imports and41 direct DOL reference sites. These
are evidence counts,not full-game completion or execution coverage. No original
instructions or hardware images were executed for this recovery.

This is the complete second pass of animSetMaterial_Texture8003dcfc/0x5ac.
It runs after ALL first-pass [object loads](ANIMATION_TEXTURE_OBJECT_SETUP.md),
including skipped nonzero kinds. Loop j and reversed S selection are identical;
no texture descriptor lookup or kind test is repeated here.

Capture state=T+24*S:tx=float+4,ty=float+8,sx=float+0c,sy=float+10,
angle=float+14. The first-pass index byte at+0 is not consulted. Component mask
starts0;bit1 rotation R,bit2 translation M,bit4 scale S. `R(x)` below means a
single-precision rounded operation,not a measured host/Gekko parity promise.

## Component construction

If angle!=0 by fcmpu,request Translate(+.5,+.5,0),Z-axis RotRad with
R(0x3c8efa35*negate(angle)),Concat(positive pivot,rotation,temp),
Translate(-.5,-.5,0),Concat(temp,negative pivot,rotation). Set bit1.
The actual negative angle operation precedes multiplication. Positive/negative
pivot constants are0x3f000000 and0xbf000000. NaN is unequal and enters this path.

Translation is admitted when tx!=0 OR ty!=0 OR **sy!=0**. That third operand is
the Y scale,not another translation or a default-one comparison. It requests
Translate(tx,R(R(1-ty)-sy),0) and sets bit2. Thus default(tx,ty,sy)=(0,0,1)
still requests an identity translation matrix;all three0 skip a mathematically
nonzero Y translation. Preserve this literal gate rather than simplifying it
from intended-looking affine algebra.

If sx!=1 OR sy!=1,request Scale(sx,sy,1) and set bit4. Comparisons use fcmpu,
so NaN enters component paths. No finite,reciprocal,epsilon or range guards.

## Ordered products and GX publication

Pinned eight-word table80306678 selects these products:

| Mask | Matrix / request |
| --- | --- |
| 0 | Identity coordinate-generator request;no local matrix load |
| 1 | R |
| 2 | M |
| 3 | R*M |
| 4 | S |
| 5 | R*S |
| 6 | M*S |
| 7 | (R*M)*S |

Mask0 requests GXSetTexCoordGen2(j,1,j+4,60,0,125)802b4e04 and advances.
Every other mask computes matrixID=30+3*j,requests
GXSetTexCoordGen2(j,1,j+4,matrixID,0,125) FIRST,then
GXLoadTexMtxImm(selected local matrix,matrixID,1)802ba3d0.
This ordering differs from the later override path that loads a supplied matrix
before requesting its generator. Neither sequence restores prior state.

The [matrix primitive contract](ANIMATION_MATRIX_STACK_AND_AFFINE_NUMERICS.md)
specifies PSMTXConcat/Scale/Trans grouping,paired arithmetic and supported alias
captures;[axis rotation](ANIMATION_SDK_AXIS_ROTATION.md) specifies RotRad/Trig
and sinf/cosf wrappers. Underlying libm,FPSCR/GQR0,GX generator/matrix packing,
texture filtering and original pixel comparisons remain open. Reusing original
textures does not justify treating these coordinates as fixed authored UVs.
