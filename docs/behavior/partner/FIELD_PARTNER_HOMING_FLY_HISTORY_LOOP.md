# Flying homing: live distance versus history,unbounded local retry and recovery

US G8ME01 disc0 revision0,ID1,recovered2026-10-12. Evidence B:complete local
party_motion_homing_fly0x8014f048,size0xd04. Read
[entry and retained variables](FIELD_PARTNER_HOMING_FLY_ENTRY_AND_SPEED.md)
first. This specifies every local loop branch0x8014f3ec..0x8014f7f8,then the
common post-loop requests before the
[collision/height tail](FIELD_PARTNER_HOMING_FLY_COLLISION_TAIL.md).
Called source/math/backend/domain behavior remains separate.

## Loop top and planar threshold

On each iteration reset local far-recovery flag F=0. Request angleABf(X0,Z0,
TX,TZ),store angle H to local frame+40. Request planar D=distABf(X0,Z0,TX,TZ).
X0/Z0 remain the captured entry root;TX/TZ may change when history advances.
Choose planar limit L:

- If CAPTURED M+0 mask0x8000 set,L=R(current P+f4+current M+1b8).
- Else L is the single FMA of0.5*current P+f4+current P+104.

Only ordered D>L enters the live-distance/speed graph below. Otherwise,
including unordered D,jump to the history-index handling section. A far live
target and a nearby history coordinate are separate concepts here.

## Planar D>L: live-distance decisions

Copy X0/Y0/Z0 to another vector and request
[partyGetTargetDist3D](FIELD_PARTNER_LIVE_TARGET_DISTANCES.md)(q,&vector),
retaining live distance E. If ordered E>=C:

- Set K=B if P+8 mask0x00020000 set,else C.
- Compute U=R(0.1*R(E-K));cap ordered U>4 to4,then if ordered U<0.5 replace
  it with0. This is not a clamp to0.5:values strictly below0.5 become0.
- Request immediate speed writer(P,R(current captured M+180+U)).
- Set F=1 and exit the loop to common post-loop requests.

Otherwise request marioGetSlitFloorPos(&scratch)0x800a6258. If nonzero:

1. Request smoothed writer(P,1.5),then partyNextReadId(q).
2. Request marioGetSlitFloorPos(&scratch) AGAIN. If now0,exit loop without
   replacing H. If nonzero,copy returned position;request planar distance
   from X0/Z0 to its XZ,then angleABf to the same coordinates and replace H.
3. If that planar distance is ordered<=5,request partyChgMotSlitFloor(P).
   Exit loop in either distance case. No local F=1 assignment occurs here.

If the FIRST slit-floor request was0,compare E with B. For ordered E>B:

- P+8 mask0x00020000 set:compute U from E-B with the same0.1,cap4,
  below0.5→0 pipeline,request immediate writer(P,R(M+180+U)),then enter
  history-index handling rather than exit immediately.
- Flag clear:U=R(E-B). If ordered U>=1,replace U with1,except ordered B<20
  replaces that value with retained S. For U<1 or unordered retain U.
  Request smoothed writer(P,U),then exit loop.

For E not ordered>B,including unordered E,inspect floats P+a0 and P+a8.
If both compare equal0,request smoothed writer(P,current recaptured owner+180)
and proceed to history-index handling. Otherwise request immediate writer(P,0),
request angleABf(current rootXZ,captured M.xz),store P+100,and request
partyChgPoseId(P,1);then proceed to history-index handling. These two fields are
literal predicates here;no guessed velocity/wind meaning replaces them.

## Index handling and explicit backedges

Request write index partyGetFootmarkWriteId(q),then read index
partyGetFootmarkReadId(q),compare signed words for equality. If equal:

1. Request smoothed writer(P,current recaptured owner+180).
2. Recapture owner AGAIN. If owner+180 is not equal0 AND P+a0/P+a8 both
   compare equal0,exit loop with current H.
3. Otherwise overwrite H with current P+100,request smoothed writer(P,0),
   store float0 at P+11c. If P+0 mask0x200 clear,request pose1.
4. Request partyChgMot(P,0),then exit loop.

If indices differ:

1. Request partyNextReadId(q),then partyGetReadId(q,&TX,&TY,&TZ).
   These loads replace all local target coordinates DIRECTLY;the entry +J
   Y offset is not reapplied. Source/read-index semantics are not guessed.
2. If P+0 mask0x04000000 set,return to LOOP TOP immediately. No local
   iteration limit exists.
3. Otherwise clear P+0 mask0x80,request partySearchGround(P,100,100).
   Nonnull attr0x200 is discarded. Store Hground/null to P+138.
4. Null ground returns to LOOP TOP. For remaining nonnull,request attrs AGAIN,
   set P+0 mask0x80 if attr0x100 set,else clear it. If these same attrs have
   0x800,request motion0. Then return to LOOP TOP in either case.

This can consume several records and repeat live-distance/speed/floor requests
within ONE invocation. There is no local guarantee of one index advance,finite
iteration count,monotonic geometry or termination for arbitrary source states.
Do not add a cap without either preserving reachable behavior or documenting
the intentionally changed contract. The entry root captures remain unchanged
through these local iterations even though called services may mutate P.

## Common post-loop work and F recovery

If current speed104 is not equal0,request unk_800cbb10(P,speed),otherwise
request motion0. If signed byte P+4e is>10,replace H with angleABf(current
rootXZ,captured M.xz). This counter read happens before later tail increments.

If F=1:

1. Recapture owner O=P+160;request placement3(P,&candidate),ignore its return.
   The [placement3 output contract](FIELD_PARTNER_PLACEMENT3.md) initializes
   the buffer;do not invent an uninitialized-output failure here.
2. Set H=angleABf(current rootXZ,candidateXZ). Request ground(P,10,0),copy
   current P+e4 into candidateY REGARDLESS of query result,then discard
   nonnull ground with attr0x200.
3. Remaining nonnull ground snaps rootY to candidateY. Otherwise blend rootY
   toward R(O.y+J),J=1 if current P+4 mask0x8000 set,else5,using original
   delta then0.05 single FMA. Candidate XZ are not teleported into root.

Continue to [collision/height publication](FIELD_PARTNER_HOMING_FLY_COLLISION_TAIL.md).
No rootXZ move has yet been published by this local homing body;called services
may of course have their own writes. Static branch recovery is not original
history cadence,trajectory or numeric parity evidence.
