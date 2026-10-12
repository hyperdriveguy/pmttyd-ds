# Flying homing: front contact,height correction,publication and terminal queries

US G8ME01 disc0 revision0,ID1,recovered2026-10-12. Evidence B:complete local
party_motion_homing_fly0x8014f048,size0xd04. This continues
[entry](FIELD_PARTNER_HOMING_FLY_ENTRY_AND_SPEED.md) and
[history loop](FIELD_PARTNER_HOMING_FLY_HISTORY_LOOP.md) at0x8014f8ec.
H denotes the current local heading;M is the initial captured owner;q retained
history bank. R and F denote original single-result and fused instructions.
This closes the local tail,not backend/resource/numeric/source domains or
original execution.

## Front query and explicit result suppression

Store H to P+100 FIRST. Copy current rootXYZ to local V. Request
partyGetMoveDirSpd(P,&H,&speed),which may change local H independently of the
already published P+100. Request partySearchFrontWall(P,&V,speed,H).
If current P+0 mask0x01000000 set,force its returned wall pointer to null.
This does not undo V writes or skip the query. Otherwise retain its result.
The [front-wall](FIELD_PARTNER_FRONT_WALL_QUERY.md) and
[direction/speed](FIELD_PARTNER_DIRECTION_AND_SPEED.md) contracts specify those
services. Later movePos and clearance calls sometimes use P+100 rather than H;
do not collapse them into one cached heading.

## No retained wall

If P+8 mask0x00200000 clear,request
[getFrontFloor](FIELD_PARTNER_FRONT_FLOOR.md)(P,&candidate,f1=current P+100).
If set,compute facingP+54=revise360(R(270+captured M+19c)),then request
[placement4](FIELD_PARTNER_PLACEMENT4.md)(P,&candidate) instead. Both integer
returns are ignored;their local output guarantees must be composed literally.

If candidateY is ordered>=current M.y,blend rootY toward
R(candidateY+J),J=1 when P+4 mask0x8000 set,else5,using delta then coefficient
0x3d4ccccd(approximately0.05) with a SINGLE FMA. CandidateY below/unordered does not take this write.
Store byte0 to P+4e regardless of that test. If current speed104 is not equal0,
overwrite V.x/V.z with current rootXZ and request
[movePos](../field/FIELD_MOVEMENT_TRIG_AND_PLANE_LEAVES.md) with that speed and current
P+100. This supersedes front-query XZ corrections on this local no-wall route.
Then proceed to common rootXZ publication;V.y is not copied.

## Retained wall and movement gate

Store wall to P+12c. If current speed104 compares equal0,skip all wall attr/
state/height work and proceed to common publication. Otherwise store wall to
P+134,request hitGetAttr and test mask0x4000.

For0x4000 set:

- If unsigned P+37!=1,recapture owner O=P+160 and test O+0c mask0x80000000.
  If set,proceed to publication with no local state change or extra move.
  If clear,OR1 into P+8,store byte1 to P+37,then proceed without extra move.
- If P+37==1,reread current speed104. If not equal0,overwrite V.x/V.z from
  current root and call movePos(speed,current P+100);otherwise no extra move.
  Proceed to common publication. This may bypass the wall-adjusted coordinates.

This is a literal contact-state protocol,not a guessed climb/jump animation.
No common reset of P+37 is present in this body.

## Ordinary retained wall: elevated downward probe

When attr0x4000 clear,copy current rootXZ into two local floats,capture rootY
as Y0,and request movePos on those local XZ with distance current P+f4 and
LOCAL H from frame+40. Read the dynamic clearance float at SDA r13+0x1cf0
(the [clearance setter/access contract](FIELD_PARTNER_TARGET_STATUS_AND_CLEARANCE.md)
documents its producer). Cast hitCheckFilter(null callback) with:

- Origin(probeX,R(Y0+clearance),probeZ).
- Direction(0,-1,0),maximum length1000.
- Separate point XYZ,normal XYZ and mutable length destinations.

This uses the [scalar ray wrapper](../field/FIELD_SCALAR_RAY_WRAPPER.md),not a
partner-specific callback. Miss sets selectedY=-2000. Hit takes raw pointY,
computes single FMA100*pointY+0.5,converts fctiwz,converts signed word through
bias double0x802f4730,then divides by100 with fdivs. Negative values still use
+0.5;this is not symmetric thousandth rounding. After that conversion,request
hitGetAttr(Hprobe) but ignore the returned attributes locally.

If selectedY is ordered>-2000,blend current rootY toward
R(selectedY+current P+f0) with delta then0.05 FMA,and proceed to common
publication. No floor-type admission test uses the ignored attribute result.

Otherwise increment P+4e's UNSIGNED loaded byte by1 and store low byte,
without a saturation guard. Request unk_800ccb34(q,current rootY). Its result
now has a [complete absolute-live-Y source contract](FIELD_PARTNER_FLY_SOURCE_LEAVES.md);
use its ordered source reads and returned scalar. If ordered result>=1 AND ordered abs(R(M.y-rootY))>=10:

1. Blend rootY toward R(10+M.y) with delta then0.1 single FMA.
2. Publish V.x then V.z to root.
3. Return directly at0x8014fd00,skipping map-specific head query,final ground,
   hazard/attribute mirroring and out-of-screen reset.

Otherwise continue to common publication. This special early exit is observable;
do not refactor every path through the terminal queries below.

## Common rootXZ publication and las_10 ceiling path

Publish V.x then V.z;V.y remains unused. Compare CURRENT GP+12c map string
with `las_10` at0x802f47c8. Only equality requests the local ceiling path:

1. Copy current rootXYZ to Vhead and current P+f0 to height scratch.
2. Request partySearchHead(P,&Vhead,&height,f1=current P+10c).
3. Null return skips the correction. Nonnull writes P+140 and computes
   D=R(R(Vhead.y-current P+f0)-current rootY).
4. If ordered D<-5 OR ordered D>=0,blend rootY using0.1*D plus current rootY
   as a single FMA. Ordered[-5,0) and unordered skip this correction.

The [ceiling contract](FIELD_PARTNER_CEILING_QUERY.md) documents that its Vhead.y
is the input Y rather than raw intersection Y,and that this caller ignores the
returned height scratch. Preserve this binding;do not substitute the backend's
hit point or the modified height as D's source.

## Final ground/hazard/screen tail

Clear P+0 mask0x80,then request partySearchGround(P,10,10). Null proceeds
straight to out-of-screen reset. Nonnull requests hitGetAttr:

- Attr0x800 requests partyChgRunMode(P,13),then RETURNS without the screen
  reset or0x100 mirroring.
- Otherwise attr0x100 sets P+0 mask0x80;absence clears it AGAIN.

This final ground pointer is not stored to P+138 here,and attr0x200 is not
locally discarded in this final tail. Floor service writes still occur according
to its own contract. Finally request
[party_force_reset_outofscreen](FIELD_PARTNER_SCREEN_RECOVERY.md)(P),then return.
The body supplies no reliable gameplay integer return contract to its dispatchers,
which ignore results. Static pins cover every local gate/request/store and these
early exits;called engine and original-runtime equivalence remain unproven.
