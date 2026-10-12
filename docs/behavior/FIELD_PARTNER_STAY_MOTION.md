# Partner stay motion: admission, distance decisions and collision requests

US G8ME01 disc0 revision0, ID1, recovered 2026-10-12. Evidence B: complete
party_motion_stay at0x8015164c,size0x6b4. This documents every local branch
and ordered request;called footprint/geometry/collision/pose engines remain
dependencies. M is owner P+0x160;[footmark ID q](FIELD_PARTNER_FOOTMARK_CLEAR_AND_ID.md)
comes from (P+0 word>>2)&1. No original trajectory/timing is claimed.

This handler is selected by [walkMain](FIELD_PARTNER_WALK_DISPATCH.md)
selector0/substate0 or selector1/substate0. The driver mode P+0x34 is
checked later,so a handler named stay is not an unconditional no-work path.

## Entry marker and distance threshold

If word P+8 mask2 is set,consume it and request partyChgPoseId(P,1).
Reread word+8:if mask0x00800000 is clear,request partyClearFootmark when
P+0 mask8 is clear,otherwise partyClearFootmark2. Then clear word+8
mask0x00800000,store -1 at P+0x168 and call unk_800cbf84(P,float0).
These footprint helpers are not substituted with this body's index writes.

If owner unsigned state M+0x2e!=14 and P+0 mask0x80000 is set,clear that
mask. Start threshold T from float P+0x40. If M+0x194 equals float0 and
M+0x180 is ordered<1,override T=**100**. If unsigned M+0x3c==2,
override T=**70**. Both tests retain original floating comparison behavior.

Compare current map string at GP+0x12c against **aji_02,aji_05,aji_12**,
in order,using strcmp and fresh GP pointer loads. A match skips later string
comparisons. If any matches and marioKeyOffChk returns0,override T using:

| Object classification | T |
| --- | --- |
| Unsigned P+0x32==0 |18 |
| Otherwise signed kind5 or6,P+8 mask0x00200000 set |8 |
| Otherwise signed kind5 or6,mask clear |30 |

OR0x20000 into P+8 after that override. Independently,if the reread P+8
mask0x20000 is set,apply the same classification again from a separate FP
temporary,overwriting T. Thus this flag can retain the special threshold
outside those map checks. The body does not clear it here.

For nonzero P+0x32 with kind other than5/6,these override paths load no
fallback into their FP temporary before copying it to T. They consequently
depend on incoming nonvolatile FP state for that out-of-classification path.
Do not invent a fallback distance;the intended object domain or original
register trace must establish applicability. Normal kind1/8 init sets P+0x32=0.

## Common pre-decision requests and exceptional return

Call **party_slit(P)**0x8014be2c,**party_roll(P)**0x8014ba6c and
**unk_800cbc30(P)**0x800cbc30,in that order. Reread owner state. If it is14
and P+0 mask0x80000 is clear,request **partyMoveBehindMario(P,0)** at
0x800cb4a4 and return immediately. This exceptional path precedes the later
mode0 guard;it skips this body's collision and terminal pose paths.

Otherwise,if reread unsigned **P+0x34==0**,skip the distance/motion-change
decision and go to collision requests. Mode0 still runs the entry,common
pre-decision and collision/terminal paths admitted above.

## Primary or missing-primary distance route

If signed slot P+0x2f!=0,query marioGetPartyId,then partyGetPtr;nonnull
result selects the secondary route below. Slot0 or null lookup selects this
primary route.

Suppress the distance decision when signed M+0x50!=0 and M+0x180==float0,
or owner state is31,32,16 or17. Otherwise:

* Mobility byte P+0x32==0:copy current P position into a stack vector;
  request partyGetTargetDist3D(q,&vector),retaining its result d;then
  partyGetTargetDist(q,P.x,P.z),whose result is unused on this route.
* Nonzero mobility:compute angleABf(P.x,P.z,M.x,M.z),request movePos on
  stack X/Z with **R(0.5*P+0xf4)** and that angle,then retain distABf of
  the adjusted X/Z versus owner X/Z as d. The [live target wrappers](FIELD_PARTNER_LIVE_TARGET_DISTANCES.md) are
  now specified;planar geometry/angle helper internals and rounding remain
  separate contracts.

If marioChkSlitThrouh is nonzero,continue without testing d;otherwise require
ordered d>=T. Then require marioChkDeepSleep==0. If admitted,request
**partyChgMot(P,1)**0x800cb584. Continue to collision requests regardless of
admission;no local replacement of the current dispatcher occurs.

## Secondary distance route

Call anotherPartyGetPtr(signed P+0x2f) as Q;null goes to collision requests.
For nonnull Q:

1. If nokonokoGetStatus(Q)==3,or distance(owner position,Q position) is
   ordered>=150,query partyGetTargetDist(q,P.x,P.z). Result ordered>=T
   requests partyChgMot(P,1),then partyClearFootmark2. Go to the slit check.
2. Otherwise,if Q kind byte3 and mode byte3,suppress this route for owner
   states31,16,17 (not32). Other owner states compare distABf(P.x,P.z,M.x,M.z)
   against T;ordered>= requests partyChgMot(P,1). Go to the slit check.
3. Otherwise,when Q+0 mask0x100 is set and its previously read kind is not4,
   skip this distance request. If not suppressed,compare distABf(P.x,P.z,Q.x,Q.z)
   against T;ordered>= requests partyChgMot(P,1).
4. At the common slit check,marioChkSlitThrouh nonzero requests
   partyChgMot(P,1),even if an earlier secondary branch already did so.

Do not coalesce the repeated motion-change request or replace the distinct
owner-state exclusions with a common guessed list.

## Ordered collision and position stores

Capture current P+0x58/+0x5c/+0x60 into a stack position. Request
[partyGetMoveDirSpd(P,&direction,&speed)](FIELD_PARTNER_DIRECTION_AND_SPEED.md),then
**partySearchFrontWall(P,&position,speed,direction)**0x800c2c00. Retain its nonnull
return H at P+0x12c;also store H at+0x134 when float P+0x104 is not equal
to zero. With null H,these hit fields are not cleared by this local body.

Store returned position X and Z at P+0x58/+0x60;do not store Y here.
Request partyGetMoveDirSpd again,then
**unk_800c27c0(P,&position,speed,direction,P+0xf4,P+0xf0)**. Store the resulting
position X/Z again. These are ordered service calls using floating outputs;
the direction/correction local bodies are now linked;called trig/geometry,
wall and surrounding-correction bodies remain open.

If P+0x32 is exactly1,request unk_80150fac(P) and return. Any other value
requests unk_801512e0(P),then party_force_reset_outofscreen(P)0x8014b1a8,
before the optional kind7 pose path. Do not treat every nonzero mobility byte
as equivalent to1 on this tail.

## Kind7 pose cycle

Continue only when unsigned kind byte is7,P+4 mask0x8000 is clear,
marioKeyOffChk==0,marioChkDeepSleep result!=1,and chuchu_searchObject result
!=0. Notice the deep-sleep predicate differs from the primary route's exact0.
Failure returns without this cycle's byte/word changes.

Read unsigned byte P+0x39:

| Value | Ordered local action |
| --- | --- |
| 1 |Decrement32-bit word+0x24 and store;while signed result>0,return. Otherwise request partyChgPoseId(P,1),write word30 and byte2 |
| 2 |Decrement/store word+0x24;while signed result>0,return. Otherwise clear byte+0x39 to0 |
| Others |Request partyChgPose(P,**PCH_A2_1**),write byte1,then word30 |

Subtraction and signed comparison use the original32-bit result;there is no
saturating timer policy. The pose key is an original resource identifier,not
dialogue. Numeric30 is a local counter literal,not measured elapsed time.

## Verification and remaining work

tools/verify_field_partner_executor.py pins the complete stay/walkMain bodies,
66 selected instructions,nine constants,five data spans and two lecture imports.
Map IDs/pose key are metadata with hashed original string spans;original
executable/resource payloads and listings remain outside history.

Next:motion-state and footmark query/clear bodies,geometry/direction/wall/
surrounding/floor/fly helpers and pose/ability services;flag/slot/index/owner
domain and original stop/run/following trajectories;complete field/battle
teardown. This local closure does not complete all partner locomotion or the game.
