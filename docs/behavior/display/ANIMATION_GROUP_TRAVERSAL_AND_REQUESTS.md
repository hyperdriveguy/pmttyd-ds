# Animation group traversal:visibility,node pushes,cache requests and draw branches

US G8ME01 disc0 revision0;ID1,2026-10-12. Evidence B:complete
Eval evalProc0x80039a24,size0x1e8 and Disp dispProc0x8003c4bc,size0x3c8.
These are actually called by the recovered wrapper/inner. No original execution.
Matrix push/render/material/Vivian services remain distinct dependencies.

## Shared node/visibility graph

Inputs Eval(parent r3,group r4);Disp additionally(current r5,next r6,fraction f1).
Only group==−1 returns immediately;no other index/cycle/depth guard. Capture
W+20 resource D and W+18 pose P;G=word[D+1a8]+88*group. Load byte through
word[P+5c]+word[G+4c],sign-extend,then compare0. ANY nonzero byte is visible,
including negative bytes. Do not equate visibility to literal1 or to cache P+58.
Zero skips this node's push,all child work and pop,then still traverses sibling.

For visible node,N=word[P+64]+4*word[G+50] is the node-float pointer.
If G+54==0,call pushGXModelMtx_TransformNode__(N)0x8003e7ec.
Otherwise call pushGXModelMtx_JointNode__(N,f1=sx,f2=sy,f3=sz)0x8003e2a8:
if parent==−1 OR parent group's word+54==0,use(1,1,1);else read parent node
floats+0c/+10/+14 through CURRENT pose node buffer. No parent bound/scale guard.
Push bodies' reciprocal/transform/stack arithmetic remain open.

After push,reload CURRENT global pose from W+18. Mask0x40 at pose+0 SET:
copy raw XYZ translation from current modeling matrix+0c/+1c/+2c to
word[word[pose+15c]]+12*group. Mask0x80 SET:PSMTXCopy current modeling matrix
into word[word[pose+164]]+48*group. Separate pose/global reloads precede each.
Temporary vectors seed from802c1054(Eval)/802c1048(Disp) but all three words
are overwritten by translation before publication. No destination guard.

## Eval child gate and sibling order

If CURRENT pose+0 mask0x20 SET and word[word[pose+120]]==group,skip child.
Otherwise call Eval(parent=CURRENT group,child=word[G+44]). After admitted
visible-node work,pop modeling state literally:level−1 to0x8041e89c;pointer=
0x803d8e70+48*newLevel to0x8041e8a0. No stack-underflow check/restoration.
Finally ALWAYS call Eval(parent=CURRENT group,sibling=word[G+40]).
Sibling parent argument is the CURRENT group,not incoming parent. Do not replace
this with a conventional tree walker that passes the incoming parent to siblings.
Invisible nodes also pass their current index on the sibling route.

## Disp special matched-group branch

CURRENT global pose+0 mask0x20 SET and word[word[pose+120]]==group selects
special work instead of ordinary callback/render/child recursion. Capture kind
K=pose+11c,saved matrix address B=pose+124,current modeling matrix M.
Read base=word[0x80415db0+4*K] BEFORE validating/classifying K;
pair=base+8*CURRENT pose+14 (animation index,not passed key index).
No kind/index/null guard. Float parameters:

| K | extra f3 | extra f4 |
| --- | --- | --- |
| 1 | −2 | 0.5 |
| 2 | −3 | 0.5 |
| 3 | −1 | 0.5 |
| other | 0 | 0 |

Request materialProc(word[G+48],currentKey)0x8003d60c;then
vivihimo(M,B,f1=pair[0],f2=pair[4],f3/f4 above)0x80183f18.
Reload CURRENT global pose and clear ONLY mask0x20 in its word+0.
For captured B and M,blend ONLY nine BASIS floats at offsets
00,04,08,10,14,18,20,24,28 (hex),in order:
Bfield=fmadds(0.03999999910593033,single(Mfield−Bfield),Bfield).
Translation0c/1c/2c is not blended here. The effect request can already have
changed B;do not move the blend before it. Skip ordinary render/child,then pop
and sibling as below. This file documents requests,not vivihimo's full effect.

## Ordinary Disp branch and final traversal

If special branch not selected,read CURRENT global pose+154 callback. Nonnull
calls(callbackData=pose+158,groupIndex,currentModelingMatrix);no prepared pass/
fraction args. Then call renderProc(word[G+48],currentKey)0x8003c884.
No local shape−1 guard:the render service owns that domain. Recursively call
Disp(parent=CURRENT group,child=G+44,currentKey,nextKey,ORIGINALpass,fraction).
Then pop level/pointer exactly as Eval. ALWAYS call sibling with parent=CURRENT
group and unchanged retained sample/pass/fraction,including invisible nodes.
Callbacks may change globals;captured G remains the source of child/sibling IDs.

## Concrete resource and fidelity boundaries

Original a/a_mario has42 groups,serialized base0x88cc,root41. Root metadata:
sibling35,child40,shape−1,visibilityIndex41,nodeFloatIndex984,jointFlag0.
`verify_animation_draw.py` checks this original metadata/hash;root data proves
why a shape−1 node can still traverse children/siblings,not a visual runtime trace.
The static verifier exports no serialized group payload. The
[native companion](NATIVE_ORIGINAL_MODEL_GROUP_TRAVERSAL.md) now retains original
model data and binds ordinary node/traversal requests to the first real renderer. Full source graph/visibility/node/link/
stack/callback lifetime,SDK push matrices,render/material/GX/Vivian and original
geometry remain open. The current native idle bake is not full original traversal
or fractional animation;these contracts must guide those implementations.

ID1 node builder dependency update (2026-10-12):
[joint](ANIMATION_JOINT_NODE_MATRICES.md) and
[transform](ANIMATION_TRANSFORM_NODE_MATRICES.md) now close complete local node
push bodies;[SDK stack/affine](ANIMATION_MATRIX_STACK_AND_AFFINE_NUMERICS.md)
and [rotation](ANIMATION_SDK_AXIS_ROTATION.md) close their local primitive calls.
Source13/29 node kinds are checked. Full libm/GQR0/FPSCR/render/material/GX/
resource/source/lifetime/original visual composition remains open.

ID1 render/material dependency update (2026-10-12):
[render/material entry](ANIMATION_SHAPE_RENDER_ENTRY_AND_DEFAULTS.md) links
complete local defaults/overrides/last-stage and indexed fan emission contracts.
Both callees ignore their second incoming key-index argument locally. Material
requests differ from ordinary renderer alpha-input/emission work. Called shader/
texture/GX/normal-matrix/source/lifetime/original visuals remain open.
