# Partner motion2 initialization: two distinct numeric contracts

US G8ME01 disc0 revision0,ID1,recovered2026-10-12. Evidence B:complete local
main/jump2 initialization regions. Selection and target ownership are in
[destination preparation](FIELD_PARTNER_JUMP_DESTINATION.md);ordered updates
are in [flight/landing](FIELD_PARTNER_JUMP_FLIGHT_AND_LANDING.md). Constants
are read from the original binary,not measured trajectory/timing values.

R denotes single-result rounding,F a single-result fused operation,T original
fctiwz signed-word conversion. Each S below denotes the actual owner float+228
read at that stage. Its producers/meaning are not established here;do not name
it an elapsed-time factor or assume callback-separated reads stay identical.
Both handlers store word P+24 but NEVER decrement it during their local flight
updates. It is a preparation parameter,not an observed countdown/duration.

## Main phase0 common preparation

After [target selection](FIELD_PARTNER_JUMP_DESTINATION.md),request planar
distance root→P+94/+9c and store D at P+128. If signed P+28==0,request the
corresponding angle and store P+100;load a FRESH owner pointer from P+160,
set speed P+104=R(2.25*that owner's+228). Otherwise retain direction/speed.
The captured entry owner M is used again in later vertical calculations.

Using current D/speed,compute these literal operations:

```
r = R(D/speed)
r = R(original .9*r)
nRaw = T(R(F(10,r,.5)/10))
N = signed clamp(nRaw,12,40)
A = R(.5*float(N))
delta = R(targetY-rootY)
```

Store N at P+24. Do not substitute ceil(D/speed),a rendering-frame count or
a portable conversion with a different NaN/overflow rule. For the vertical
height input h:if ordered abs(delta)<300,use25. Otherwise preserve delta when
ordered delta>=0;otherwise use10. Thus ordinary delta0 still chooses25,and
exact±300 takes the second classification,not the25 override.

Compute denominator=F(A,A,-A),coefficient c=R(R(h+h)/denominator).
Compute initial velocity by R(S*R(c*A)),store to P+114,then cap only for
ordered value>13 or value<3. NaN is not sanitized by those ordered tests.
Set acceleration P+11c=R((-c)*a later M+228 read). No generic sqrt or measured
gravity replaces this expression.

OR0x10 into P+0,then clear P+0 mask2. Request partyChgPoseId(P,4),then
psndSFXOn_3D(sound0x927,&P+58). Set phase P+38=10. If P+0&8 is CLEAR,
request fresh footmark ID(P) and partyNextReadId(q). Then RETURN:there is no
same-call vertical integration or horizontal root publication on main init.
Destination preparation may already have mutated target/history earlier.

## Jump2 entry marker and target-distance preparation

Only P+8 mask2 triggers jump2 initialization. Consume it,store word P+24=0,
set acceleration P+11c=R(-1*S) and provisional speed P+104=R(2.25*S),using
captured owner reads. Request root→prepared-target angle,store P+100;request
planar D and cap it to50 only for ordered D>50. Store that capped D at P+128.
Prepared XYZ itself is NOT selected/rewritten by jump2 entry.

Compute nRaw=T(R(F(10,R(D/provisionalSpeed),.5)/10));clamp signed lower bound
to10 with no upper bound. Store N at P+24 and convert it to float. Then:

```
speed = R(S*R(D/float(N)))
delta = R(targetY-rootY)
u = R(float(N)*abs(currentAcceleration))
u = R(float(N)*u)
velocity = R(S*R(F(.5,u,delta)/float(N)))
```

Store speed at P+104 and velocity at P+114. Cap velocity to20 only when
ordered>20;replace with0 only when ordered<0. The currentAcceleration already
contains an owner+228 multiplication;do not cancel owner factors or replace
this with the main handler's coefficient calculation. Invalid/zero speed and
conversion/domain behavior remain target-CPU/source requirements.

Request pose ID4,then OR P+0 mask0x10 and clear mask2,then request sound0x927
at current root. Store phase+38=0 and continue into phase dispatch. Phase0
changes phase to1 and returns without integration. Thus a marker-initialization
call finishes in phase1,but no flight step runs until a later call. Existing
phase0 without a marker also performs only that phase advance. Other phases
are handled by the separate flight contract.

## Exact requests and validation limits

Takeoff request0x927 is not an invented tone or a fully bound effect playback
contract. The complete landing wrapper N_partyPlaySfxPartyLanding1 at0x800cbe84,
size0x2c requests psndSFXOn_3D(0x928,&P+58) and returns its result;it does not
clear physics fields or change motion. Original sound engine/ID→asset/pose
ownership remains external to these handlers.

tools/verify_field_partner_jump.py pins complete local handler/index/landing
bodies,independent numeric/call/store anchors and exact constants/bias/templates.
Finite deduced cases should distinguish main N12..40 vs jump2 N>=10,main
height override abs(delta)<300,velocity3..13 vs0..20,retained signed-counter
speed,callback rereads and no same-call flight step. These are not measured
ballistics or proof of source-runtime timing equivalence.
