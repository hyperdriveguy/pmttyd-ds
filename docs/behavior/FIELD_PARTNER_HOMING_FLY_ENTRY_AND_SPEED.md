# Flying homing: entry,gates,thresholds and initial speed requests

US G8ME01 disc0 revision0,ID1,recovered2026-10-12. Evidence B:complete local
party_motion_homing_fly0x8014f048,size0xd04. Used calls occur in
[flyMain](FIELD_PARTNER_FLY_DISPATCH.md),walkMain and explicit walk-mode wrapper.
The complete local graph is split across this document,
[history loop](FIELD_PARTNER_HOMING_FLY_HISTORY_LOOP.md) and
[collision/height tail](FIELD_PARTNER_HOMING_FLY_COLLISION_TAIL.md).
This is not an executed following trace or a fixed interpolation recipe.

Notation:P partner;capture owner M=P+160 once in the prefix. Capture
q=partyGetFootmarkId(P),the [bank leaf](FIELD_PARTNER_FOOTMARK_CLEAR_AND_ID.md)
returns(P+0>>2)&1. This q remains retained despite later live state reads.
R denotes original single-result operations;F is single fused multiply-add.
Calls retain their original order even if float results are unused/repeated.

## Pending setup,far threshold C and near threshold B

If P+8 mask2 set,clear that marker,clear P+0 mask0x00080000,and store byte0
to P+4e. No history clear/root reset occurs on this setup path. Set C=28 if
signed halfword P+28 is NONZERO,else45. This includes negative halfwords.

Compare current GP+12c map string with `aji_02`,then `aji_05`,then `aji_12`,
short-circuiting on equality and rereading GP for each comparison. If any matches,
request marioKeyOffChk. ONLY result0 takes this threshold override:

| Domain | Selected special threshold |
| --- | --- |
| Unsigned mobility P+32==0 | 18 |
| Mobility nonzero,signed kind P+31==5 or6,P+8 mask0x00200000 set | 8 |
| Same kinds/other flag state | 30 |
| Mobility nonzero,other kind | No local assignment to FPR25 before its use |

Copy FPR25 to C and OR0x00020000 into P+8. In that last domain,C therefore
uses inherited floating-register state if no external ABI violation/change occurs;
the body does not provide a default. Do not silently set it to30 or prune the
branch without proving shipped source/kind domains. Original map strings are
pinned at0x802f47b0..0x802f47c8;they are resource IDs,not authored dialogue.

Request [partyGetTargetDistY](FIELD_PARTNER_LIVE_DIRECTION_AND_ACTIVITY.md)(q).
This is an ACTIVITY scalar,not a vertical coordinate difference. If its result
is not equal0(including unordered),set B=50;otherwise45. Next,if current P+8
mask0x00020000 is set,apply the same table above to FPR26 and copy it to B.
The other-kind/nonzero-mobility domain has no local FPR26 assignment either.
This flag can already be present independently of the special map branch;
it is not cleared when maps cease to match. C/B are separate retained values.

## Early admission,owner suppression and speed amount S

Request party_slit(P)0x8014be2c;result0 returns immediately. Otherwise request
party_roll(P)0x8014ba6c;result0 also returns. These early exits occur AFTER the
threshold/query/marker work above. Complete local [slit](FIELD_PARTNER_SLIT_GATE.md) and
[roll](FIELD_PARTNER_ROLL_GATE.md) contracts now bind these return gates.

If CAPTURED M+2e unsigned motion==14 and current P+0 mask0x00080000 clear,
request partyMoveBehindMario(P,0) and return. Otherwise recapture owner O=P+160
for the ordinary movement-admission predicate:if signed O+50!=0 and O+180
compares equal0,suppress;else suppress motions31,32,16,17. Suppression requests
partyChgMot(P,0) and returns. O and captured M have different read roles.

Request S=partyGetTargetMovespd2(P)0x800cc95c,then
[PSVECDistance](FIELD_PARTNER_SDK_DISTANCE_NUMERICS.md)(captured M.xyz,rootXYZ).
Ordered distance<45 replaces S with1;else ordered distance>=50 replaces S with3;
otherwise preserve the service result,including the unordered domain. Next
recapture owner and if its word+0 mask0x8000 set,replace S=1 and C=100.
Request [smoothed speed writer](FIELD_PARTNER_FLY_SPEED_WRITERS.md)(P,S).
This call can change target108 without changing speed104 under its own gates.

Capture root floats X0,Y0,Z0 AFTER that request. Obtain history pointer
R0=partyGetFootmarkPtr(q)0x800cd1f4,then copy record+4 to local TX,record+8
plus J to TY,record+0c to TZ. J=1 if P+4 mask0x8000 set,else5. The source
record pointer is distinct from a live Mario/partner target.

Copy current rootXYZ to a scratch vector and request
[partyGetTargetDist3D](FIELD_PARTNER_LIVE_TARGET_DISTANCES.md)(q,&scratch).
If ordered result>=C,request the smoothed writer(P,S) AGAIN. If ordered B<20,
request the same writer AGAIN independently. These tests can cause two additional
updates in this invocation;do not coalesce them. The distance wrapper chooses
LIVE Mario/party positions,not R0/TXYZ.

Continue into the [complete local history loop](FIELD_PARTNER_HOMING_FLY_HISTORY_LOOP.md)
with retained q,M,C,B,S,X0/Y0/Z0 and local TX/TY/TZ. Its backedges preserve
these initial captures except for explicitly rewritten locals;no measured
one-record-per-tick or render-frame cadence is established.

The flying verifier pins this complete body,all local gates/requests/stores,
exact constants and map identities. The
[target-speed/history-pointer leaves](FIELD_PARTNER_FLY_SOURCE_LEAVES.md) and
[flying record writer](FIELD_PARTNER_FLY_HISTORY_WRITER.md) now have complete
local contracts. Slit/roll collision/animation backend,source/index/FP-register/
kind/flag/alias domains and
original execution remain dependencies even with all static checks passing.
