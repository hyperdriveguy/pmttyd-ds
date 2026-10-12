# Flying history initialization: bank seed and transient selector writes

US G8ME01 disc0 revision0,ID1,recovered2026-10-12. Evidence B:complete local
partyMoveFlyInit0x800c8ec8,size0x42c. Inputs P and integer A in r4. Both
[used callbacks](FIELD_PARTNER_FLY_CALLBACK_BINDINGS.md) pass A=1 after clearing
their pending P+8 mask8. The helper itself does NOT test or consume that marker;
calling it directly still performs the graph below.

## Prefix and bank seed

First request marioGetPtr;its returned pointer is not retained as this helper's
seed source. OR2 into P+0,then choose initialization bank using P+0 mask8:
clear→bank0,set→bank1. This differs from
[writer slot selection](FIELD_PARTNER_FLY_HISTORY_WRITER.md) and
[consumer footmark ID](FIELD_PARTNER_FOOTMARK_CLEAR_AND_ID.md).

Bank0 queries fresh global Mario,loads raw XYZ+8c/+90/+94 once,and seeds all
80 records from those words. Bank1 captures fresh global Mario POINTER G,requests
marioGetPartyId then partyGetPtr Q,and selects:

| Condition | Seed |
| --- | --- |
| Q null | Captured G XYZ |
| Q nonnull,nokonokoGetStatus(Q)==3 | Captured G XYZ,without distance request |
| Otherwise PSVECDistance(G.xyz,Q.root) ordered>=150 | Captured G XYZ |
| Otherwise | Q rootXYZ,including unordered distance domain |

Seed coordinate words are loaded only AFTER the chosen status/distance path;
do not reuse an immutable vector from before those service requests.
Bank1 Mario-coordinate fallback STILL seeds bank1;it does not select writer's
bank0 fallback policy. The
[status classifier](FIELD_PARTNER_TARGET_STATUS_AND_CLEARANCE.md) and
[SDK distance](FIELD_PARTNER_SDK_DISTANCE_NUMERICS.md) contracts specify those
services. Source/owner/global pointers are not assumed identical.

For each selected bank,ten iterations write eight records each,advancing128
bytes per iteration. For each record,write raw X,Y,Z at+4/+8/+c,THEN flag byte0
at+0. Padding+1..3 remains untouched. After all records,clear selected WRITE
word first,then selected READ word. Bank addresses are in the writer document.
Other bank's buffer and indices are preserved. No root/destination placement
or consumer read advance occurs during this seed.

## Selector2 early return

After seeding,if CURRENT unsigned selector P+3b==2,store float50 to P+40
and RETURN. It retains P+3d,3e,3f,44,motion36 and other flags that the ordinary
path below changes. The initial P+0 OR2 and selected history reset already
occurred. Do not move this early return before the history work.

## Ordinary selector graph and overwritten intermediates

For other selectors,compute byte P+3d=0 if A==0,else2;this tests the full32-bit
argument,not its low byte. If current selector P+3b==6,clear P+0 mask0x8000.
Store selector0 and OR4 into P+8. Now sign-extend P+3f:

- Values1 or3:store0 to P+3f and P+3d,then selector1 and OR4 into P+8.
- Value2:store0 to P+3d,then selector60 and OR4 into P+8. P+3f remains2.
- Every other signed value:preserve P+3f and prior P+3d.

Those branches contain selector6 cleanup tests after the known selector0 store
and with no intervening callback;they are locally redundant false tests. Keep
this distinction from the later post-pose selector6 check. The transient60 is
not this function's final selector under ordinary callback-preservation state.

Store word16 to P+44,float50 to P+40,byte0 to P+3e. Request
partyChgPoseId(P,1),then reread CURRENT selector. If==6,clear P+0 mask0x8000.
This later test follows a service call,so it is not equivalent to the earlier
dead selector tests. Store selector1 and OR4 into P+8 unconditionally.

Finally read unsigned motion P+36. If==3,clear P+0 masks0x70,0x03000000,
0x04000000 in three separate live load/store operations. Store motion0 and
OR2 into P+8,then return. This helper does not initialize P+118/+114 as the
ordinary walking callback does;do not import those writes from a similar loop.
Float50,word16 and selectors are recovered constants,not measured tick durations.

Both paths are complete local contracts. Pose/source/slot/flag/index/alias/FP
lifecycle and original entry/recruitment/transition trajectories remain open.
The flying-history verifier pins the full initializer,loop stores,seed gates,
argument classification,transient selectors and two actual callback bindings.
