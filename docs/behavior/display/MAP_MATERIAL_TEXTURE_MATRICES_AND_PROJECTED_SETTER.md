# Map texture matrices, literal zero gates and projected object publication

US G8ME01 disc0 revision0;documentation ID1,2026-10-12. Static read-only
binary/source analysis,no original instructions/GX execution or observed images.
`tools/verify_map_material_source.py` checks six complete local bodies,806
reviewed words,six binary32 constants,six original spans,two US lecture imports
and five direct DOL sites. Loader/source/GX/numeric/lifetime/runtime remain open.

Complete mapSetTextureMatrix80021468/0x360 takes material M r3. Capture initial
N=U8[M+0b]. Iterate i=N-1 down0,j=0 up;record F=M+28*i;matrix table pointer starts
802bf9f0+4*i then retreats4 per slot. No N/table/source extent check. The ten
original IDs are30,33,..57;coordinate j differs from descending matrix indexi.

Capture sx=F34,sy=F38,angle=-F3c,tx=F2c,ty=-F30. If sx==1 AND sy==1 AND
tx==0 AND ty==0 AND
angle==0,skip matrix construction/load and request generator
identity60. fcmpu means NaNs do not pass equality. Otherwise:

* Rotation admitted iff angle!=0. Read auxS=F40,auxT=F44;px=R32(R32(.5*auxS)*sx),
  py=R32(F32(-.5,auxT,1)*sy). Request Trans(px,py,0);RotRad Z with
  R32(3c8efa35*angle);Trans(-px,-py,0);Concat(Tplus,R,R) SECOND alias;
  Concat(R,Tminus,R) FIRST alias. Rotation is pivoted before scale/translation.
* Scale admitted iff sx!=0 OR sy!=0—NOT the usual !=1 gate. Scale(sx,sy,1).
* Translation admitted iff tx!=0 OR ty!=0;Trans(tx,ty,0).

All nonidentity components are combined in order R*S*T,with absent components
omitted and original grouping(R*S)*T. For only R/S/T,Copy that component to local
result;for R*S or R*T or S*T,Concat;for all three,Concat R*S then result*T FIRST
alias. If sx=sy=angle=tx=ty=0,outer identity test fails but ALL component gates
skip;NO local result initialization occurs before matrix load. Preserve this
literal unwritten-result path as an invalid/unproven source domain,not an
invented identity/zero matrix. No local guard repairs it.

Nonidentity branch:GXLoadTexMtxImm(local,word[current table pointer],1) FIRST. Then reread
CURRENT N=U8[M+0b];GXSetTexCoordGen2(j,1,N+3-j,matrixID,0,125),rereading ID.
Identity branch requests same generator with matrixID60,no matrix load. Each
iteration advances j and retreats record28/table4;loop termination uses initial
captured N. Thus source count after helper effects need not equal captured loop
extent. For stable N2,first j0 uses slot1/matrix33/source5,then j1 uses slot0/
matrix30/source4. Do not unify this with animation's ascending source j+4.
Prior [matrix/axis contracts](ANIMATION_MATRIX_STACK_AND_AFFINE_NUMERICS.md) cover
Concat/Copy/Scale/Trans;libm/GQR0/FPSCR/GX/source zero/alias/lifetime/images open.

## Projected texture source setter

Complete mapSetProjTexObj8001d2d8/0x48 takes object pointer r3,HOLDER pointer r4.
Copy EXACT eight words from source+0..1c to global embedded object8041e6b4..d0,
in four pairs:load both words of pair,store both,then next pair. This is an
interleaved copy,not a whole-object snapshot/memmove guarantee for overlapping
source/destination. Finally store incoming holder pointer into8041e6b0.
No matrix write,no allocation/refcount/image copy/cache flush/null/lifetime guard.
[Flag400 projection](MAP_TEV_FLAG400_TWO_STAGE_PROJECTION.md) later passes this
embedded object and stored holder to smartTexObj;that consumer establishes raw
second-argument meaning. Setter does not dereference the holder itself.

The complete seven-section DIRECT DOL census finds ZERO calls to this setter.
Its original API/source write behavior is specified because the shader consumes
these globals;actual REL/indirect/scene writer bindings and ownership are still
unproven,not claimed used from its symbol name. The map texture-matrix helper has
two concrete _mapDispMapObj sites80026460/800267fc,as recorded in the
[entry contract](MAP_MATERIAL_ENTRY_CHANNELS_AND_TEV_INPUTS.md). Original display
caller admission/loader/metadata and projected matrix producer remain separate.
