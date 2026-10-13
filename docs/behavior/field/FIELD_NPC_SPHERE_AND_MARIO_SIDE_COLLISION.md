# Field NPC sphere query and Mario side response

Read-only US G8ME01 disc0 revision0 static DOL evidence, partial answer to
[OW-INTERACT-01](../../requests/OW-INTERACT-01.md). The partner
[horizontal last-match scan](../partner/FIELD_NPC_HORIZONTAL_OVERLAP_SCAN.md)
is a different service and must not substitute for these queries.

## Complete local point/sphere versus NPC body query

`_npcHitCheckSphere`0x800446b4,size0x178 takes NPC in r3, XYZ in f1..3,
extra radius in f4, distance output in r4. It reads NPC rootXYZ+8c/90/94,
height+150 and width+14c. For finite ordinary inputs its vertical gap is:
Y-rootY below the body;0 inside inclusive[rootY,rootY+height];
Y-(rootY+height) above. Horizontal gaps are X-rootX,Z-rootZ. Squared
norm uses ordered binary32 operations: verticalGap², FMA(dx,dx,vertical²),
then FMA(dz,dz,previous). Positive norm uses Gekko frsqrte and three double
refinements before final binary32 result; zero/negative/NaN use the original
classification/canonical-NaN path. Always store resulting distance through r4.

Admission is strict `distance < FMA(0.5,NPCwidth,extraRadius)`: return the
original NPC pointer on match,0 otherwise. Equality misses. This local function
has no flags, pool scan, facing, talk-event or ground-state filter. These belong
to callers. Height is not scaled by render geometry, and width is halved here.
The service is a point sphere tested against the NPC vertical-segment body,
not a full Mario/NPC interaction or movement-resolution routine.

## NPC-main Mario side owner and initial predicates

`npcCheckHitMarioSide`0x800490cc,size0x578 takes one NPC, queries Mario.
Reject if NPCrootY>MarioRootY+MarioHeight(+1bc), or MarioRootY>NPCrootY+
NPCheight(+150); touching vertical endpoints is admitted. Horizontal distance
is SDK PSVECDistance after copying MarioXZ at NPCY. Radius is
`0.5*(MarioWidth(+1b8)+NPCwidth(+14c)+5)` (0x80049168..180).
Reject distance>radius; equality is admitted.

Then overwrite NPC currentX/Z with NPC previousX/Z(+98/+a0), preserving Y;
recompute distance from the same MarioXZ-at-NPCY temporary point, reject if
it now exceeds radius. This rejection retains the NPC XZ rollback. Successful
response publishes NPC to Mario+1dc, computes heading from MarioXZ to the
rolled-back NPCXZ using system angleABf, and compares its absolute compAngle
to Mario move heading+1a4 against40.

When heading difference<40, response delta is penetration(radius-distance)
projected away from Mario move heading, using the original inline polynomial
sine/cosine approximations. Otherwise it projects Mario out to the separation
circle around the rolled-back NPC, using Mario-to-NPC heading and those
polynomials. Exact polynomial/FMA evaluation is a numeric dependency; a libm
sin/cos or normalize-vector replacement is not instruction-equivalent.

Delta norm uses the same three-refinement source square-root shape. If
Mario+1c0>4, normalization threshold is3; otherwise1.5. Norm>threshold scales
delta by2/norm; it does not clamp to threshold. The routine adds delta to
Mario currentX/Z, leaves Y unchanged, then invokes
marioChkWallAround(&MarioRoot,0,0,heading+160,20,MarioHeight), in source
integer/float argument banks. Return1 follows this wall service; early exits0.
Do not omit the original wall correction after actor response.

## Exact NPC-main caller gates and source-local order

npcMain0x80049a7c clears Mario+1dc before its pool scan (0x80049adc).
Within an admitted NPC update, source map-side movement calls npcHitCheckSide,
then npcMoveG at0x80049f84. Next at0x80049f88..9fa8, NPC main flags masks8,
0x20 or0x4000 each bypass npcCheckHitMarioSide. Otherwise call it once before
later NPC displacement/pose processing. Hidden/display masks are not substituted
for these collision masks. Full active/update guards above this local block,
map contact attributes and ordinary frame ordering remain separate dependencies.

This caller is NPC-owned, even for stationary NPCs. A native Mario movement
query may reuse the recovered finite geometry predicates as a documented
adapter; it must not claim an invented scan/order/separation formula is this
original response. The ordinary field-root call order is MarioMain before mobjMain before
npcMain before caseMain; see [talk owner order](FIELD_ORDINARY_NPC_TALK_CANDIDATE.md).
Pause/retrace and full event/update scheduler remain open.

## Validation and open boundaries

Fresh inspect_dol_function.py extractions of _npcHitCheckSphere,
npcCheckHitMarioSide,npcHitCheckSide,npcMain. Direct read-only ISO constants
with r2=0x80427440:0.5 at0x8041fc80,5 at0x8041fd08,40 at0x8041fd0c,
4 at0x8041fd30,3 at0x8041fce0,1.5 at0x8041fcac,2 at0x8041fd34,
160 at0x8041fd38,20 at0x8041fd3c. Original inspection files remain /tmp;
no native code/assets changed or original-runtime/build claim is made.

Complete polynomial helper numerics, caller-wide pool/flag domains, Mario map
wall placement and retrace order, dynamic NPC collision and battle/interaction
arbitration remain open. [Ordinary talk candidate](FIELD_ORDINARY_NPC_TALK_CANDIDATE.md)
owns its different radius/facing/occlusion and A-admission predicates.

Scoped numeric/caller-gate follow-up: [exact side response and polynomial](FIELD_NPC_MARIO_SIDE_RESPONSE_NUMERICS.md)
pins the source FMA coefficients and remaining direct npcMain gates, including
active1+2 and current state+1d4 mask10. The local terminal flags8/20/4000
are not the entire caller admission chain.
