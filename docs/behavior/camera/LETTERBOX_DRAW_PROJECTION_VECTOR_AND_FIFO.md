# Letterbox projection-vector and signed16 FIFO recovery

Original US G8ME01 disc0 revision0 ISO opened rb. Complete original listings stay private in artifacts/letterbox-draw-evidence. constants-and-bodies.json pins spans/hashes and actual constants. This report supplements docs/behavior/camera/CAMERA_LETTERBOX_AND_DUMMY_DRAW_SERVICES.md; no original execution/hardware image/NaN payload equivalence established.

## Exact gate/owner order

Callback8000e564/284 ignores supplied camera and userdata. At8000e57c load current GP pointer from80415fd0 (r13-28496, verified r13=8041cf20); then8000e580 capture camera8 pointer from803d95f8+20. At584 read WORD[GP+0], at588 test mask8000, set returns immediately. Therefore actual camera8 pointer fetch precedes GPword gate but signed alpha read DOES NOT happen if GPbit gate set. At590 read signedhalf[C8+1e0], compare exact0; zero returns. Negative alpha proceeds. Captured camera8 and GP pointer are retained local owners, not callback-registration camera or arbitrary global rereads.

For admitted callbacks exact save order GXGetViewport802ba5b4(out6), GXGetProjectionv802ba2b4(out7), GXGetScissor802ba6bc(out4), then load C8+19c kind and callGXSetProjection802ba184(C8+15c,kind). Projection kind read after all getters. Remaining setters match existing contract. No viewport/scissor setter occurs before geometry; current viewport/scissor are used, only projection changes for camera8. Restore saved viewport, projectionv, scissor in that order after geometry.

## Material capture

AFTER GXSetNumChans(1) returns, read WORD80429510 at8000e618. This startup-BSS word provides RGBA bytes, but original overwrites its lowest byte. At624 FRESH read signedalpha C8+1e0; store low8 into local fourthcolorbyte. Copy resulting word and callGXSetChanMatColor802b6e54(4,&localRGBA) at638. Thus gatealpha and materialalpha can differ across held preceding setters, and RGB read precedes materialalpha read. Material captures combined local RGBA before its called boundary. Source default BSSword0 yields blackRGB; no ISO payload exists at that BSS address. Original __init_data clears it as documented, but eventual indirect mutation is not disproved.

## Geometry and narrowing

GXBegin802b5a6c(0x80,0,8) ONCE, followed by direct S16XYZ for8vertices. No color/ST/normal FIFO values occur. Source attribute9 is direct, XYZ1, type3=S16, fractional0. Original emits QUADS: native adaptation may split into2quads/fans, but must preserve original separate topology and source-current-state effects.

Vertices and live bar read sites:

0=(-304,240,0)
1=(304,240,0)
2=(304,S16(low16(240 - SIGN16(C8.top))),0), read8000e734
3=(-304,S16(low16(240 - SIGN16(C8.top))),0), FRESH read8000e748
4=(-304,S16(low16(SIGN16(C8.bottom) - 240)),0), read8000e75c
5=(304,S16(low16(SIGN16(C8.bottom) - 240)),0), FRESH read8000e770
6=(304,-240,0)
7=(-304,-240,0)

Arithmetic uses signedwordsubfic/addi (32bit intermediate), then FIFO sth narrows LOW16 regardless overflow, interpretedS16 by declaredvertexformat. Do not clamp toscreen/barpositive or convert un-narrowed floatY. For top=-32768,240-top=33008 narrows-32528; for bottom=-32768,bottom-240=-33008 narrows32528. Ordinary49/44 have no wrapping. Current top/bottom are separately reread per mentionedvertex; single captured bar for bothvertices loses source read order. Alpha is not reread pervertex: material established before primitive.

No restore of model/view, TEV, channel, descriptors, depth/blend/cull/material or fog follows geometry. Only viewport/projection/scissor restore.

## Original GX retained projection owner

__GXData pointer804293d8 initially80413e40. Original GXData+4d8 holds projection kindWORD; +4dc..4f0 sixcoeffsf32. This is distinct from Mat4 and matrix-combined native shader transform. Pointer global is reread at source-defined points; no external child calls inside these3SDKleaves.

GXSetProjection802ba184/a4(matrix,kind):

- Store RAW kindWORD atGX+4d8, without validating0/1.
- Coefficients A0,A2,A4,A5 read/store first from matrix+0,+14,+28,+2c (row-major4x4: m00,m11,m22,m23), in that order.
- IF kind==1, A1=m03, A3=m13; ELSE A1=m02,A3=m12. No generic matrix normalization, inversion, division, transposition or float arithmetic.
- Write XF command0x10/header00061020, then sixcoeffsf32 then RAWkindWORD to FIFO; set GXhalf2=1. Source unknownkind is not normalized here.

GXGetProjectionv802ba2b4/48(out7):

- ReadGXkindWORD; output[0]=float0 when WORD==0, float1 otherwise. Literals804294d0=00000000,804294d4=3f800000.
- Reread __GXData pointer; copy6coeffs through3paired-floatloads/stores into out[1..6]. No recomputation and no projectiondivide.
- Thus a prior unusualrawkind2 Get maps scalar1; it does not preserve unusualrawkindword. Ordinary0/1 domain is exact canonical identity.

GXSetProjectionv802ba228/8c(vec7):

- Readliteral0 thenvec[0], unorderedcompare; if EQ(+0/-0 both) writekind0, allother values (includingNaN) writekind1.
- Copy vec[1..6] intoGXcoeffs via3pairedload/stores. No coefficient arithmetic or matrixrounding.
- Emit same XFheader/sixfloat/kindword andGXhalf2=1.

Gekko psq-load/store GQR0 means f32 projection lanes; for admitted finite coeffs these SDKleaves only copy numbers, they do not run a rounding adjustment. Exceptional NaN payload/paired-load behavior is not asserted by source-only report.

## Native fullMat4 adaptation

NativeCameraDraw currently retains projection_ as full CameraProjectionMatrix and view_ separately; shader packet is tilted projection*view. GetProjectionv must save SOURCE projection, never this combined/tilted transform. Source projection kind is currently available onCameraState only; NativeCameraDraw::projection signature carries no kind and projection_ does notretainkind. Add actual source kind+sixcoeffowner (or validated canonical equivalent) to restore exact getter/setter semantics. Merely returning fullmatrix and pretending originalvector7 is an untyped16float snapshot does not document/sourceclose the boundary.

Canonical matrix reconstruction for supported kinds (source GX semantics):

Perspective0:
[A0,0,A1,0; 0,A2,A3,0; 0,0,A4,A5; 0,0,-1,0]

Orthographic1:
[A0,0,0,A1; 0,A2,0,A3; 0,0,A4,A5; 0,0,0,1]

Canonical reconstruction introduces only literal0,+1,-1 plus original6float coefficients. For arbitrary fullMat4 terms outside those6slots, originalGXSetProjection ignores them but current native multiplication may use them; therefore either canonicalize sourceprojection when it is set or validate/admit only matrices exactly satisfying sourceGXsupportedshape. A known-kind field is necessary: zerooffsetperspective/ortho may share misleading coefficients. RestoreProjectionv must retain currentview: original callback restores projection, NOT camera8positionmatrix changed by callback. Saving and restoring projection*oldview would wrongly undo a source-unrestored positionmatrix.

This extends the [original callback contract](CAMERA_LETTERBOX_AND_DUMMY_DRAW_SERVICES.md). The [native continuation](NATIVE_LETTERBOX_DRAW_CONTINUATION.md) declares native kind0/1 and finite selected coefficient admission.

## Source body identity

| Function | Address | Bytes | SHA256 |
| --- | --- | --- | --- |
| camLetterBoxDraw | 0x8000e564 | 644 | 9cfe7c12742dc5aee4d8a4417a1f52584a56fea387c76c6fc34361087d04f585 |
| GXSetProjection | 0x802ba184 | 164 | 704b7ca753e91eecd56b12b1a334112fa4ceeb10a1c59a6977ac69528b8c83b2 |
| GXSetProjectionv | 0x802ba228 | 140 | c5db6be2ea45bff058131c94885654fc5c480be8c9e5b8b8198e9683429803d6 |
| GXGetProjectionv | 0x802ba2b4 | 72 | 7b9bd47cad82e6b939a82e819ea6726783d4929ec0af5e1d7aea3b3fe14f50c4 |
