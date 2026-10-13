# Original battle-ending fade10 texture consumer

2026-10-13 US read-only complete disp_texture80011a7c/5dc used-path closure.
Listing ignored `artifacts/recovered-temp-evidence/fade-texture.dis`;
source dispatch table80304520 and embedded constants inspected read-only.
[Scalar fade10](../battle/UNDERGROUND_TRIO_NO_LEVEL_UP_ENDING_TAIL.md#actual-fade10-scalar-completion)
owns ratio/timing/finish; this is its original GPU consumer, not an independent
fade clock. No native edits/commit or original raster comparison.

## Used context and gates

fadeMain registers this callback for slot0, passes manager, and disp_texture
reloads global manager=r13+1700. Manager+8 is slot flags, manager+c is slot
type. Original table80304520[type10]→80011ad8, so type10 **does not** suppress
draw on completed flag2. Neighboring odd types1/3/5/9/11/14 test that flag and
skip, but this is not permission to apply that gate to ending10.
The callback queries camera8 explicitly, saves current GX projection,
loads camera8 projection(+15c with type+19c), then uses camera8 view+11c.
It restores saved projection after drawing. Texture matrix is identity.

At branch80011e9c, initialize original texture with GXInitTexObj:
data=r13-7f00=80415020; width1,height1,format0(I4),wrapS0,wrapT0,mipmap0;
load to texture-map0. Original data starts byteFF then zero padding. This
single white intensity texel is embedded source data. **There is no external
fade TPL descriptor/palette/file lookup on this used type10 path.** Manager358
fade TPL is used by other shutter/spot branches; do not import their graphics
for ordinary black fade10.

## Original alpha and material

Set manager34 (slot2c) texture matrix identity. Compute
`alphaByte = low8(fctiwz(single(255.0 * manager2c)))` and store manager23,
the alpha byte of source color manager20/slot18. Used manager2c is slot ratio24.
Source float255=8041f7b4. This is truncation to signed integer then narrowing,
not smooth float alpha, round-nearest or the source scalar finish flag. RGB
remains passed000000 from original ending color000000ff. At ratio1 alpha255;
ratio0 alpha0; completion remains strict ratio>1 in the scalar producer.

GX state: color update enabled; depth test disabled/always/no depth write;
alpha compare always; blend sourcealpha/inversesourcealpha; zero color channels,
one texture-coordinate generator, one TEV stage. TEV color C0*TEXC and alpha
A0*TEXA use source color register0 loaded from manager20. Direct XYZfloat and
UVfloat vertices; cull mode2(back). Original white intensity texture multiplied
by black source color yields a uniformly black quad with source alphaByte.
Thus a native solid-black primitive can reproduce this particular source
texture result when its byte alpha/material/coverage match; it is not missing
original external artwork. Other fade types do need their original resources.

## Quad and camera projection

Load camera8 view matrix+11c as position-matrix0; texture matrix identity at
TEXMTX0. GXBegin quad80 hex,vertexformat0,count4. Source vertices:

| Order | XYZ | UV |
| --- | --- | --- |
| 0 | (-304,+304,0) | (0,0) |
| 1 | (+304,+304,0) | (1,0) |
| 2 | (+304,-304,0) | (1,1) |
| 3 | (-304,-304,0) | (0,1) |

Extent304.0=8041f800; zero8041f7a8,one8041f7ac. No extra per-slot position,
scale, ratio-dependent quad size or circle wipe is applied in this branch.
Full-screen-looking coverage follows original camera projection/clipping, not
an arbitrary inferred400x240/sourcehalf viewport. Native Citro3D may explicitly
adapt source camera8 to the top-screen viewport; describe that projection
adapter separately from recovered geometry/material. Source viewport/clip,
texture filtering, GX state restoration and executed raster comparison remain
backend dependencies. Do not apply the EXP line's glyph/layout scale to the
fade's coverage without its camera mapping.

## Ending1c NPC flag namespace confirmation

btlseqEnd8021651c first sets state1d and requests audience volume(0,30).
At8021653c fbatGetPointer8004829c;80216540 loads F8 actual encounterNPC.
Nonnull:8021654c reads NPCword140;80216550 OR100;80216554 stores NPCword140.
There is no NPCword0 write here. Keep this typed battle/encounter flag owner
distinct from ordinary NPC0 flags, actor visibility and model state. A native
fieldActor `npcBattleFlags` adapter must map that original140 owner and retain
its existing value; don't hide the actor or overwrite NPC0 merely because
numeric mask100 appears elsewhere.

Validation: complete original local body/table/constants, embeddedFF texel,
quad/FIFO/material request sequence and exact1c load/store inspected; scoped
`git diff --check`. Documentation-only/no native tests/builds/commit. Original
camera/GX numeric/raster parity and alternate fade resource paths remain open.
