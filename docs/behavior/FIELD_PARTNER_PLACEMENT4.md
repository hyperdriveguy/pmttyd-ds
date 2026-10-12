# Partner placement4: recovery search and output ownership

US G8ME01 disc0 revision0,ID1,recovered2026-10-12. Evidence B:complete local
partyGetAppearPos4 at0x800c59ec,size0x860. The used
[fall recovery](FIELD_PARTNER_FALL_AIR.md) calls it at0x8014cdb8 with
(P,&P+58),ignores its return,then adds37 to the current rootY. The output
aliases the live root in this actual caller. No original execution or recovery
trajectory has been measured. The general fallback partyGetAppearPos remains
an explicitly open called engine;its name does not establish its output policy.

## Captured inputs and initial output

Inputs are partner P and output vector V. Capture owner M=P+160 and initialize
captured buddy Q=null. No output/owner null guards exist. Select base distance D:
unsigned P+31==5 gives32;otherwise P+8 mask0x00200000 gives28;otherwise26.
Capture ray length L=R(D+P+f4),and opposite heading
revise360(R(180+P+54)). R denotes the original single-precision operation,
not a measured tuning rule. The
[facing contract](FIELD_PARTNER_FALL_FACING.md) specifies revise360's loop.

If P+0 mask8 is clear,write M+8c/+90/+94 to V in XYZ store order.
Otherwise request anotherPartyGetPtr(sign-extended P+2f);retain that returned Q.
Nonnull Q seeds V from Q+58/+60/+5c in XZY store order. Null Q seeds it from
M+8c/+94/+90,also XZY store order. No status3 or distance150 gate is requested.
These are not the live target selector's rules. Preserve load/store order if
source and output may alias;do not silently snapshot all source coordinates.

Capture maximum loop index11,or13 when current P+8 mask0x00200000 is set.
Attempt indices are0 THROUGH that captured limit,inclusive:12 or14 attempts.
Attempt0 uses current P+54 directly. Later attempts request toMovedir2 with
selected copied signed offset;the exact
[angle layout](FIELD_PARTNER_PLACEMENT4_ANGLE_LAYOUT.md) includes adjacent-stack
reads in the final attempt. Selection rereads flags/map/current facing while
loop limit,D,L,opposite heading and Q remain captured.

## Per-attempt ray and admission sequence

Request sincosf(angle),yielding u=sin and v=-cos. Construct direct query Qray
at frame+120,with origin(current V.x,R(37+current V.y),current V.z),direction
(u,0,v) and length L. Call hitCheckVecFilter(&Qray,null callback) at0x800c5ffc.
Any nonnull result rejects this attempt immediately;no attribute exemption or
partner-specific ray filter applies here.

On a horizontal miss,request movePos(&V.x,&V.z,D,angle). This changes the
CURRENT output rather than restarting from owner or buddy. Then cast another
direct/null-callback ray from(V.x,R(37+V.y),V.z),direction(0,-1,0),length87
at0x800c6094. A null result rejects the attempt WITHOUT restoring V.x/V.z.
No floor height is published on that path.

For nonnull floor H,the local admission sequence is:

1. Request hitGetAttr(H). Either0x200 or0x800 rejects.
2. Request hitGetAttr(H) AGAIN. If0x80000000 is set,request its map-object
   owner and compare owner+15 against the exact25 names/null table0x80314fe8
   from [ground support](FIELD_PARTNER_GROUND_POSTPASS.md). A matching name
   rejects. The local inlined scan has no map-owner null guard. Null checks
   nested after established nonnull H are unreachable local tests,not rays.
3. Request npcNearDistCheck(V.x,V.y,V.z,current P+f4),BEFORE snapping floorY.
   Any nonnull NPC rejects. The
   [complete NPC scan](FIELD_NPC_HORIZONTAL_OVERLAP_SCAN.md) returns the last
   strict horizontal overlap;this caller saves no best candidate.
4. Write V.y from the raw downward pointY at Qray+28/frame+148.
5. Request unk_800c27c0(P,V,f1=0,f2=0,f3=28,f4=5) at0x800c6184.
   The [surrounding-wall binding](FIELD_PARTNER_SURROUNDING_WALL.md) specifies
   mode1,width override6 for P+0 mask0x1000 and shared correction ordering.
   Do not replace the fixed width28,height5 arguments with P's dimensions.
6. If unsigned P+32==1 and V.y is ordered below current captured-owner M+90,
   replace V.y with R(5+M+90). This occurs AFTER wall correction.
7. Return integer1 without another candidate.

There is no normal-angle admission gate and no wrapper floor-point quantization
in this local body. These differ from placement3. Query point layout is+24 and
normal+30;only raw pointY fills V.y here. Trig/movePos/backend numeric behavior,
callbacks,resource and pointer domains remain dependencies.

## Rejection,exhaustion and actual alias behavior

Every rejection advances ALL five angle cursors by4 and the index by1,
including attempt0 where no offset is read. A horizontal hit preserves V for
that attempt;floor/classifier/NPC rejection preserves prior movePos writes.
Displacements therefore accumulate across horizontal misses. There is no
transaction or common retry reseed. With V=&P+58 in fall,these writes modify
the live root while the search is still running.

After index exceeds the captured limit,request partyGetAppearPos(P,V)
at0x800c61e0,target0x800c6c9c. Ignore that function's return. Apply the SAME
P+32==1 and ordered V.y<M+90 replacement with M+90+5,then return integer0
unconditionally. Zero means this local search exhausted,not unchanged output,
not invalid V and not necessarily failure of the general fallback. Its full
0x135c-byte engine/output contract remains to be recovered before closing the
exhaustion path's end-to-end behavior.

Fall ignores this integer result,uses the then-current rootY+37,and proceeds
with its documented motion reset and horizontal tail. No immutable initial root
is retained by placement4. Broader source aliases,flag changes during calls,
query-header provenance,FPSCR/trig domains and original traces remain open.

## Reproduce the static checks

Run `python3 tools/verify_field_partner_placement4.py`. The pinned manifest
checks the complete placement4 body,a containing fall body for selected actual
caller instructions,88 instruction anchors,eight constants,six data spans,two
US lecture imports and59 reconstructed angle word sources. The initializer
analysis tracks source addresses through the bounded straight-line copy block;
it does not execute original instructions or prove feasible runtime states.
Original ELF/listing/report stay ignored under artifacts/reference/behavior.
