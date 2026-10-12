# Screen-reset placement3: candidate admission and output on failure

US G8ME01 disc0 revision0,ID1,recovered2026-10-12. Evidence B:complete
partyGetAppearPos3 at0x800c624c,size0x4cc. Actual
[screen recovery](FIELD_PARTNER_SCREEN_RECOVERY.md) calls it at0x8014b27c
when owner-to-partner planar distance is ordered<20 and projectedY>800,then
ignores its integer return and copies the output XYZ to the partner root.
This document closes the local output guarantee and nine-candidate graph;
full scene/resource/numeric domains and original trajectories remain open.

## Initialization and nine angle requests

Inputs are(P,output V). Capture owner M=P+160 without a null guard. Copy nine
signed integer offsets from0x802cc3dc into local storage,in exact order:
45,135,-135,315,-315,180,-180,90,-90. Capture base direction M+1b0 and
maximum horizontal ray length L=R(30+P+f4). Initialize bestNPCDistance=-1.
Write current M.x/M.y/M.z to V in XYZ order BEFORE any candidate query.
Output therefore has defined owner coordinates on valid-pointer entry,even
when all later candidates fail without another output write. There is no
failure-preserves-input contract or output-null guard.

For each offset,convert its signed integer to float using the original bias
double0x802cc5b0,and request
[toMovedir2(baseDirection,offset)](FIELD_PARTNER_PLACEMENT3_MATH.md). Save its
returned angle H and request sincosf(H) to obtain u=sin,v=-cos. These calls
use global Mario state inside toMovedir2;do not replace them with simple
baseDirection+offset or assume P's owner always equals that global pointer.

## Horizontal gate,output mutation and downward ray

Cast directly with hitCheckVecFilter(&Q,null callback) from
(current M.x,R(37+current M.y),current M.z),direction(u,0,v),length L.
Any nonnull hit rejects this candidate immediately. The partner Yoshi/Nokotaro
filters are not requested and no attribute exemption applies to this first ray.
Null callback does not disable the backend's own geometry/cull rules.

After a horizontal MISS,overwrite V with fresh current M.xyz,then request
movePos on V.x/V.z with distance30 and angle H. V.y remains the fresh ownerY.
These output writes occur BEFORE floor/NPC admission and remain observable
even if the candidate is later rejected. Each such candidate restarts from
owner coordinates;it does not accumulate the last output displacement.

Cast a second direct/null-callback ray from(V.x,R(37+V.y),V.z),direction
(0,-1,0),length87. Null second result rejects the candidate. Nonnull G
enters the admission graph below. Ray result layout is point+24,normal+30;
these are raw direct-backend floats here,not the quantized outputs of
partyHitCheck. Neither horizontal point nor horizontal distance fills V.

## Ordered floor/classification admission

1. Inline the exact named-support classifier:if G's attr0x80000000 is set,
   get its map-object owner and compare owner+15 against the25 names/null
   table0x80314fe8 from the
   [ground support contract](FIELD_PARTNER_GROUND_POSTPASS.md). A match rejects
   the candidate. No map-owner null guard exists. Absence of that high bit
   produces no name match. This is a local inlined scan,not a call to the
   earlier classifier helper.
2. Copy G's normal into local storage and request
   [partyNrmToAngle](FIELD_PARTNER_PLACEMENT3_MATH.md). Reject only for ordered
   inclination>60;exact60 and unordered do not take this branch under
   nontrapping continuation. This differs from floor query's >=50 rejection.
3. Obtain hitGetAttr AGAIN. Reject when either0x200 or0x800 is set.
4. Otherwise repeat the high-bit owner/name scan AGAIN. A fresh name match
   rejects;no match admits. Do not merge the duplicate classifications without
   establishing callback/resource/numeric state equivalence. The null-G checks
   nested after the earlier nonnull admission are unreachable local branches,
   not additional ray requests.

These steps do not snap V.y to the raw floor point yet. They also do not
write P's slope cache or retained contact fields. Full backend result/owner
lifetime and dynamic mutation domains remain required.

## NPC-free success versus saved overlap fallback

Request [npcNearDistCheck](FIELD_NPC_HORIZONTAL_OVERLAP_SCAN.md) with current
V.x/V.y/V.z and radius=current P+f4. The helper returns the LAST qualifying
active NPC in its selected pool,using strict horizontal distance<radius. It
does not return a nearest NPC or reject by vertical separation.

If that result N is nonnull,request PSVECDistance(N+8c,V). Only if ordered
distance>bestNPCDistance,request the SAME SDK distance AGAIN,store that second
result as bestNPCDistance and save current V.xyz separately. Advance to the
next angle in all nonnull-N cases. Ties preserve the earlier saved candidate.
Thus the fallback maximizes the recorded three-dimensional separation from
each candidate's last horizontally qualifying NPC;it is not a nearest-clearance
solver across the entire NPC set. The second distance request is preserved
explicitly. Saved V.y is still ownerY,not the floor point.

If N is null,set V.y to raw downward pointY from query+28,then request
unk_800c27c0(P,V,f1=2,f2=0,f3=current P+f4,f4=current P+f0). The complete
[mode1 surrounding binding](FIELD_PARTNER_SURROUNDING_WALL.md) supplies width
override and correction rules;speed2 does not size its rays. Ignore its return,
return1 immediately,and do not test subsequent placement angles. This route
can correct V.X/Z after the floor point-Y snap.

After all nine angles,if ordered bestNPCDistance>=0,write saved V.xyz to output
and return1. This fallback has NO floor-Y snap or surrounding correction. If the
comparison fails,return0 WITHOUT reverting output. V then retains initialization
or the most recent horizontal-miss/moved candidate,even when that candidate's
floor/classification failed. Screen recovery copies this output on either return;
no uninitialized output is implied on valid-pointer entry merely because it
ignores the integer result. This resolves the earlier caller-level dependency.

## Verification and remaining scope

tools/verify_field_partner_placement3.py pins complete placement/heading/inclination/
NPC/SDK-distance bodies,selected backend regions,actual caller and branch/store
anchors,exact angle/name table identities,constants and vector/numeric spans.
The prior surrounding and filter verifiers check linked helper contracts.
Metadata checking does not execute placement or measure numerical equivalence.

Deduced cases include initial owner writes on all-first-hit failure,last moved
candidate retained on return0,raw floor pointY on NPC-free success,exact60
inclination admitted,duplicate name/distance request ordering,last NPC rather
than nearest,largest fallback distance with first tie retained,and fallback
ownerY/no surrounding correction. These are implementation fixtures,not
observed original behavior.

Next:motion2/homing/fall/jump/fly consumers and all placement callers,scene/hit/
NPC/camera/source/numeric domains and original screen-reset trajectories.
Complete field/game documentation and requirements remain incomplete.
