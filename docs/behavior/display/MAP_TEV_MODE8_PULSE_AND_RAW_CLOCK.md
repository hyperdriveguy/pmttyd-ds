# TEV mode8 pulse, raw clock operand and color publication

US G8ME01 disc0 revision0;documentation ID1,2026-10-12. Static read-only
binary/source analysis,no original execution or observed image/clock trace.
`tools/verify_map_tev_time.py` checks the containing TEV identity as PARTIAL
semantics,266 reviewed words,six binary32 constants,four original spans,two US
lecture imports and six direct DOL sites. Modes8/9 are complete local base blocks;
six later middle branches and original backend/source/runtime remain open.

Complete block800229fc..80022b00 exclusive belongs to mapSetMaterialTev
80021b80/0x2dbc. The [base dispatch](MAP_TEV_BASE_MODE_ZERO_AND_SOURCE.md) reaches
it when signed N>0 and unsigned mode8. No local N>=1 validation beyond that
positive comparison. Texture-map/coordinate counters B=C=N and indirect I=0
remain unchanged;this block ends with base TEV stage count K=1.

## Exact pulse computation

Read GP=word[80415fd0],then raw word t=word[GP+1c]. Neither the getter nor this
block names its unit/cadence;source writer/clock lifecycle still needs recovery.
Compute u=low32(t<<1);DO NOT interpret the top input bit as a signed time value
or preserve a33-bit doubled integer. Construct the double0x43300000:u,subtract
original802bfc20=0x4330000000000000 with fsubs. Thus q=R32(unsigned32(u));
large u may round when converted to single. R32 denotes original single-rounded
operations,not a host/Gekko equivalence promise.

Compute theta=R32(R32(0x40c90fdb*q)/360),with binary32 factor
6.2831854820251465 and binary32 divisor0x43b40000. Call double `sin`80272410;
FRSP its result to single s. Then fMADDsingle y=F32(127,s,127),using one fused
multiply-add and0x42fe0000. FCTIWZ converts y by truncation toward zero;take
low8 of its integer result for a color byte. Original libm/FPSCR/invalid conversion
behavior remains separate. No local divide/finite/epsilon guard or clipping.
Replacing this with host sin/float multiply-add is an adapter requiring numeric
validation. It is not documented as a measured period in milliseconds or ticks.

Before the sine call,copy RAW color word[80429528] to a local buffer. This is a
SBSS2 source address,not a serialized DOL color seed;this recovery does not invent
its initial/runtime RGBA. Replace only local byte+1 with the computed low8 value.
Retain original bytes+0,+2,+3. Copy local color word into a request buffer and
GXSetTevColor(1,&color)802b8ab8. It is the TEV color-register request,not KColor1.
The global raw source word is not written by this block. Do not replace the
retained alpha/red/blue with constants or infer all channels pulse together.

## Exact base requests after color publication

In order:

1. GXSetTevOrder(0,0,0,4).
2. GXSetTevColorOp(0,0,0,0,1,0).
3. GXSetTevAlphaOp(0,0,0,0,1,0).
4. GXSetTevColorIn(0,15,10,8,2).
5. GXSetTevAlphaIn(0,7,5,4,1).
6. GXSetTevSwapMode(0,0,0).
7. Set K=1 and branch to shared postlude80022f48.

[Constant/character-shadow](MAP_TEV_CONSTANT_AND_CHARACTER_SHADOW.md) and
[final callback/counts](MAP_TEV_FINAL_CALLBACK_AND_OPEN_BRANCHES.md) still apply;
K1 is not necessarily final K. The block does not load texture objects or UV
matrices,does not update GP+1c,and has no once-per-frame/tick guard. Same observed
raw t reproduces the same local pulse request subject to backend numeric state.
Actual original GP/color writers,scene mode8 bindings,call frequency/GX/libm and
image parity remain required before complete-game behavior or timing claims.
