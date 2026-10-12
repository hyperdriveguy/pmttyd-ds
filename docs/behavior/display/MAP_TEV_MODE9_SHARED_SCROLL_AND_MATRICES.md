# TEV mode9 shared scroll mutation and coordinate-matrix requests

US G8ME01 disc0 revision0;documentation ID1,2026-10-12. Static read-only
binary/source analysis,no original execution or observed image/clock trace.
`tools/verify_map_tev_time.py` checks the containing TEV identity as PARTIAL
semantics,266 reviewed words,six binary32 constants,four original spans,two US
lecture imports and six direct DOL sites. Modes8/9 are complete local base blocks;
six later middle branches and original backend/source/runtime remain open.

Complete block80022b00..80022e24 exclusive is reached by positive-N mode9
[base dispatch](MAP_TEV_BASE_MODE_ZERO_AND_SOURCE.md). Unlike
[mode8's raw-clock pulse](MAP_TEV_MODE8_PULSE_AND_RAW_CLOCK.md),it directly updates
persistent material state on EVERY call. No GP time read,elapsed-time scaling,
pause predicate,object ID,once-per-tick/frame guard or per-material phase copy.

## Four ordered shared phase updates

Phase array P=804150b0 (`scrl_uv$1622` source annotation),four binary32 floats;
original serialized initial words are all0. Increment array Q=8041e0c0
(`queen_uv` annotation),original four words:

| Component | Initial increment bits |
| --- | --- |
| 0 | 00000000 |
| 1 | bad1b718 |
| 2 | 00000000 |
| 3 | ba51b718 |

These are startup metadata,not proof of immutable values;other original writers/
initialization/reset/scene ownership remain unrecovered. First Q address is
formed by lis8042 plus SIGNED lfsu displacement−8000→8041e0c0. The update-form
load modifies the address register;do not mislabel it as an unrelated operation
or compute8042e0c0 by treating its low displacement as unsigned.

For i=0,1,2,3 in order,read P[i] and Q[i],compute/store P[i]=R32(P[i]+Q[i]).
Reread P[i]. If ordered P[i]>100,store R32(P[i]-100). Reread again. If ordered
P[i]<-100,store R32(P[i]+100). The constants are0x42c80000 and0xc2c80000.
Each component completes its store/tests before the next component reads.

There is exactly ONE subtraction and ONE addition opportunity per component,
with strict inequalities;it is not modulo/fmod or repeated normalization.
Values exactly100 or−100 remain. For example,post-add250 becomes150 and stays
out of range;post-add−250 becomes−150. Ordered comparisons on NaN do not select
those stores;FPSCR compare effects and NaN payload/rounding remain open numeric
work. In-range ordinary samples do not justify replacing this literal algorithm.

Because phases are shared,the number/order of material calls changes their
future values. A native once-per-simulation-tick update or object-local UV phase
would be a changed cadence/domain unless original caller scheduling proves
that equivalence. This contract supplies mutation order,not an invented game
frame rate or measured scroll speed.

## Coordinate and matrix request order

After all four updates:

1. PSMTXTrans8029e230(local,P[0],P[1],0).
2. GXLoadTexMtxImm(local,30,1)802ba3d0.
3. GXSetTexCoordGen2(0,1,4,30,0,125)802b4e04.
4. Reread P[2]/P[3];PSMTXTrans(local,P[2],P[3],0).
5. GXLoadTexMtxImm(local,33,1).
6. GXSetTexCoordGen2(1,1,5,33,0,125).

Second phase pair is read AFTER the first matrix/generator requests;do not
snapshot all four across their called effects. Source pair0/1 is captured for
its own Trans request. [PSMTXTrans](ANIMATION_MATRIX_STACK_AND_AFFINE_NUMERICS.md)
local numeric/store behavior was previously recovered. This branch loads
matrices BEFORE generators,unlike the ordinary texture-state helper's nonzero
matrix component path. It does not multiply the model matrix or regenerate
loaded texture objects. N>0 alone admits this mode;no local N>=2 guard despite
its unconditional coordinate1 request. B/C remain N rather than forced2.

## Three exact TEV rows and shared continuation

For each stage s,in row order,request Order,ColorOp,AlphaOp,ColorIn,AlphaIn,
SwapMode as in [fixed-stage grammar](MAP_TEV_FIXED_STAGE_PROGRAMS.md).
ColorOp/AlphaOp always(s,0,0,0,1,0);SwapMode(s,0,0).

| Stage | Order(coord,map,channel) | Color A,B,C,D | Alpha A,B,C,D |
| --- | --- | --- | --- |
| 0 | 0,0,255 | 15,15,15,8 | 7,7,7,4 |
| 1 | 1,1,255 | 0,8,9,15 | 0,7,4,4 |
| 2 | 255,255,4 | 15,0,10,15 | 7,0,5,7 |

Set K=3;branch to shared postlude80022f48. No B/C/I increments and no prior
matrix/GX state restoration. Later constant/shadow/flag/callback/count stages
can change publication. These rows match fixed mode7 locally,but mode9's prior
shared phase writes/matrix requests are additional behavior and cannot be erased
by deduplicating only the final TEV table. Source phase/increment writers and
actually selected US scenes,GX numeric/images/cache/lifetime remain open.
