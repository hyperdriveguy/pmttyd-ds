# Last-stage texture requests and independent kind1 rebind

US G8ME01 disc0 revision0;ID1,2026-10-12. Evidence B:complete renderProc/
materialProc bodies. Both share this request graph AFTER their
[primary override scans](ANIMATION_RENDER_TEXTURE_OVERRIDE_SELECTION.md).
No original execution or completed TEV/GX interpretation.

## Additional texture admission and helper return nonuse

Admit extra block iff any kind was found,word[B+8]==0 AND CURRENT W+38nonnull.
Capture O=word[W+38];request smartTexObj(O,CURRENT W+2c),but DISCARD its
return. Later GXLoadTexObj uses captured O itself. This differs from primary
kind1/2 smartTexObj calls,which pass the returned object pointer.
No object/null/failure normalization is local after admission.

If CURRENT W+e0 !=3,request these opaque-style GX state operands in order:
GXSetZMode(1,3,1),GXSetBlendMode(0,4,5,0),GXSetZCompLoc(0),
GXSetAlphaCompare(6,128,1,0,0). Pass3 skips these four requests.
No state rollback/restoration is present.

## Base rebind and extra coordinate

Choose base fields using the retained kind1Found boolean:nonnull selects
CURRENT W+30/cc;zero selects CURRENT W+34/d0. Original `cntlzw`/mask arithmetic
encodes offsets0 or4;this is not a new kind scan. If selected base texture pointer
nonnull:

1. GXLoadTexObj(selectedBasePointer,0) DIRECTLY (no new smartTexObj call).
2. GXSetTexCoordGen2(0,1,4,30,0,125).
3. GXLoadTexMtxImm(selectedBaseMatrix,30,1),with NO separate matrix-null guard.

A null base texture skips these requests but does not skip the remaining extra
texture. Then ALWAYS:

1. GXLoadTexObj(captured O,1).
2. GXSetTexCoordGen2(1,1,4,33,0,125).
3. GXLoadTexMtxImm(CURRENT W+d4,33,1),without matrix-null guard.
4. mapSetMaterialTev(2,12,CURRENT pose+e8,CURRENT modeling matrix).

This hardcodes TWO texture stages,regardless of original draw slot count.
Default material helpers ran earlier if their guard admitted;extra requests can
replace their state. The original traversal pass is not substituted for W+e0.
Source domains should explain object/matrix validity;the leaf does not invent it.

## Independent kind1 rebind after extra block

Whether primary/extra blocks ran or not,reload CURRENT pose+0 mask0x10.
CLEAR skips. SET additionally requires CURRENT W+30nonnull AND W+ccnonnull.
Only then capture fresh resource/pose/N/base/state/count/descriptor inputs and
scan descending draw slot IDs via ascending coordinate slot j for kind1.
Use the same signed index bounds fallback0/printf as primary scans.
First kind1 match requests smartTexObj(captured W+30,captured W+24),
GXLoadTexObj(returned,j),GXLoadTexMtxImm(captured W+cc,30+3*j,1),
GXSetTexCoordGen2(j,1,j+4,30+3*j,0,125),then stops. No kind2 fallback.
These are fresh captures;do not retain primary scan's failed result as a shortcut.
No post-request cleanup/restoration of textures/coordinates/TEV occurs.

Renderer proceeds to [arrays/vertices](ANIMATION_RENDER_ARRAYS_AND_VERTEX_FIFO.md).
Material-only path proceeds directly to its next draw record;it performs no
vertex descriptor setup or FIFO emission. Source callback/resource/override
writers,smart object lifetime,SDK/GX/map TEV semantics and original images remain
open. Verifier pins the complete original ordering,not a host graphics oracle.
