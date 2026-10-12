# Flying source leaves: history pointer,speed amount and absolute live Y error

US G8ME01 disc0 revision0,ID1,recovered2026-10-12. Evidence B:complete local
partyGetFootmarkPtr0x800cd1f4(size0x28),partyGetTargetMovespd2
0x800cc95c(size0xf8),unk_800ccb34 0x800ccb34(size0x160).
[Flying homing](FIELD_PARTNER_HOMING_FLY_ENTRY_AND_SPEED.md) requests all three.
They have different selector/ownership semantics;neither a name nor a shared
150 gate makes them interchangeable with
[live distance wrappers](FIELD_PARTNER_LIVE_TARGET_DISTANCES.md).
No original execution or source-domain invariant is established here.

## partyGetFootmarkPtr(q): current read record address

Compute bank-byte displacement1280*q with mulli and index-word displacement
q<<2 with32-bit operations. Load current read index R from
0x8041eae8+4*q,then return
0x803ec2c8+1280*q+(R<<4),with original32-bit address arithmetic. No record
coordinates/flag are read by this leaf. It does not return a write record,
advance indices,check capacity or validate pointer/null/bank/index ranges.
Homing's q producer supplies(P+0>>2)&1;other callers' domains remain separate.
The same shared buffers are documented in the
[complete flying writer](FIELD_PARTNER_FLY_HISTORY_WRITER.md).

## partyGetTargetMovespd2(P): one captured-owner/buddy selection

Capture M=P+160. If P+0 mask8 CLEAR,return owner activity
M+180 when ordered>0,else M+188. If mask8 SET but P+8 mask0x00020000 SET,
return the SAME captured-owner activity rule without party pointer queries.
Unordered M+180 falls back to188;no maximum/zero clamp is added.

Otherwise request marioGetPartyId and pass its returned index directly into
partyGetPtr as Q. Null falls back to the captured-owner activity rule. Nonnull
Q requests nokonokoGetStatus(Q):status3 marks unusable without distance.
Other status requests PSVECDistance(M.xyz,Q.root);ordered>=150 marks unusable,
unordered does not. Usable Q returns raw current Q+104. Unusable returns the
captured-owner activity rule,with live field loads at that later point.

There is ONE pointer selection and no expanded/repeated Mario/buddy pass in
this function. It accepts P,not a boolean bank q. It does not normalize P's
slot,request anotherPartyGetPtr,read history records or update P's speed.
The caller later chooses how to apply this amount through
[the two speed writers](FIELD_PARTNER_FLY_SPEED_WRITERS.md). Status/distance
leaf contracts are linked from
[target status](FIELD_PARTNER_TARGET_STATUS_AND_CLEARANCE.md) and
[SDK distance](FIELD_PARTNER_SDK_DISTANCE_NUMERICS.md).

## unk_800ccb34(q,Y): absolute vertical live-target error

Capture the input float Y. Request global Mario G0,normalize original32-bit q
via neg/OR/shift to boolean b=(q!=0),and request anotherPartyGetPtr(b) as Q0.
Even q==0 performs that other-pointer request. Then q==0 returns
fabs(R(current captured G0+90-Y)),without another global-pointer request.
This is an absolute Y difference,unlike TargetDistY's activity scalar.

For q!=0,Q0 null uses that same captured G0 Y fallback. Nonnull Q0 requests
status;status3 marks unusable without distance. Otherwise ordered
PSVECDistance(G0.xyz,Q0.root)>=150 marks unusable. Initial unusable Q0 also
uses captured G0 Y. Initial usable Q0 triggers a SECOND complete selection:

1. Request global Mario G1,retain as later Y fallback.
2. Request global Mario G2,retain for the second separation check.
3. Normalize ORIGINAL q again,request anotherPartyGetPtr(b) as Q1.
4. Nonnull Q1 requests status. Status3 or ordered distance(G2.xyz,Q1.root)>=150
   marks unusable. Null skips these queries. Unusable replaces local Q1=null.
5. Select current Q1+5c when nonnull,otherwise current G1+90.
6. Return fabs(R(selectedY-input Y)),with single fsubs followed by fabs.

Do not cache Q0 or merge G1/G2 roles. The initial fallback uses G0;the expanded
fallback uses G1,while G2 owns separation. Pointer and coordinate reads occur
at these distinct service boundaries. No epsilon,sign direction,history lookup,
output pointer,range clamp or local state write is present. Original numeric
exceptions/NaN/domain behavior remain subject to Gekko state.

In the [homing collision tail](FIELD_PARTNER_HOMING_FLY_COLLISION_TAIL.md),
this scalar is compared ordered>=1 after a failed elevated floor probe;that
caller separately compares abs(M.y-rootY)>=10 and can return before its normal
terminal queries. The scalar's complete local source rule now supplies that
previously open dependency;it does not close source writers or trajectories.

`tools/verify_field_partner_fly_history.py` pins these three complete bodies,
selectors,loads,status/distance/request and repeated-read branches. Static
verification does not establish all source pointer/kind/slot/alias/numeric domains
or original endpoint/activity behavior during transitions.
