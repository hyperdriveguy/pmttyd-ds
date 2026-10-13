# NPC-owned Mario side response: polynomial and FMA numerics

Read-only US G8ME01 disc0 revision0 static evidence. Extends
[side-collision contract](FIELD_NPC_SPHERE_AND_MARIO_SIDE_COLLISION.md), answering
[OW-INTERACT-01](../../requests/OW-INTERACT-01.md)'s numeric/caller-gate subset.
No Dolphin trajectory or native numerical-equivalence claim is made.

R(x) means binary32 rounding; F(a,b,c) single fused multiply-add; G(a,b,c)
single fused multiply-subtract(a*b-c); N(a,b,c) the source single negated
multiply-subtract(c-a*b). Preserve Gekko signed-zero/NaN/FPSCR domains separately.
The following complete finite ordinary graph applies after source vertical,
horizontal and previous-NPC-position predicates admit the response.

## Captures, angle selection and source polynomial

Retain radiusD=R(R(5+R(MarioWidth+NPCWidth))*0.5), rolled-back horizontal
PSVECDistance d, theta=angleABf(MarioXZ,NPCpreviousXZ), and Mario move heading
M+1a4. If abs(compAngle(theta,moveHeading))<40 use moveHeading for trig;
otherwise theta. Exactly40 takes the theta branch.

The selected degree H converts to radians
T=R(R(TAU*H)/360), TAU bits0x40c90fdb. Source does not wrap H inside
this polynomial block; ordinary normalized-angle ownership is a caller domain.
For ordinary0<=T<=TAU, choose reduced U and signs with this exact branch tree:

| Branch | U | sin sign | cos sign |
| --- | --- | --- | --- |
| T<=PI and T<=HALF_PI | T | + | + |
| T<=PI and T>HALF_PI | R(HALF_PI-R(T-HALF_PI)) | + | - |
| T>PI and T<THREE_HALF_PI | R(T-PI) | - | - |
| T>PI and T>=THREE_HALF_PI | R(HALF_PI-R(T-THREE_HALF_PI)) | - | + |

The second row's actual source reduction is **R(HALF_PI-R(T-HALF_PI))**,
not PI minus T in one operation. This preserves its two rounded subtractions.
The table's fourth row likewise retains two rounded subtractions.

Compute V=R(U*U), sine polynomial S=R(F(G(A,V,B),V,1)*U),
cosine polynomial C=F(G(CA,V,CB),V,1), then apply row signs using fneg.
Do not replace these with std::sin/cos or a different series. Exact coefficients:

| Value | Binary32 bits / source address |
| --- | --- |
| TAU | 0x40c90fdb / 0x8041fd10 |
| PI | 0x40490fdb / 0x8041fd1c |
| HALF_PI | 0x3fc90fdb / 0x8041fd20 |
| THREE_HALF_PI | 0x4096cbe4 / 0x8041fd24 |
| A | 0x3bf95d4f / 0x8041fd14 |
| B | 0x3e2a0903 / 0x8041fd18 |
| CA | 0x3d17c1be / 0x8041fd28 |
| CB | 0x3efe4f76 / 0x8041fd2c |

PI/HALF_PI comparison equality stays in the upper two branches;
THREE_HALF_PI equality takes the last. S/C have separate source computation
chains with freshly loaded literal inputs; sharing a computed host reduction
requires equivalence proof across the retained input/rounding domain.

## Response delta and cap

Near-heading branch(<40): penetrationP=R(D-d), then dx=R(P*fneg(S)),
dz=R(P*C). Other branch: dx=R(N(D,S,NPCpreviousX)-MarioX),
dz=R(F(D,C,NPCpreviousZ)-MarioZ). Thus a stationary NPC can push Mario
away without changing NPC position beyond its source XZ rollback.

LengthSquared=F(dx,dx,R(dz*dz)). Positive input obtains Length with the
original frsqrte seed and three double Newton refinements:
each round computes half*estimate and(3-q*estimate²), multiplies them;
final sqrt=R(q*estimate). The <=0 branch follows its source classification/
canonical NaN path. Source double half/3/zero at0x802c13c8/d0/d8 and canonical
NaN0x8041e3c8 are dependencies; an arbitrary host sqrt is an adapter.

Threshold=3 if Mario+1c0>4,otherwise1.5. If Length>Threshold,compute
factor=R(2/Length),dx=R(dx*factor),dz=R(dz*factor). This can enlarge a delta
between1.5 and2; it is not a conventional maximum-distance clamp. Otherwise
retain delta. AddX thenZ with separate binary32 adds. Preserve source current
loads, not previously captured Mario roots, at these final stores.

Finally source invokes marioChkWallAround with current Mario-root pointer,
integer mode0 and float inputs(0,theta+160,20,MarioHeight). Height/radius and
heading160 are exact requests; do not substitute180 as an apparently natural
opposite heading. It returns1 after the wall service; wall mutations remain
part of successful response ownership.

## Complete direct npcMain admission gates for this call

npcMain selects current/alternate832-byte NPC registry and clears Mario+1dc
before scanning. At0x80049b00..b10 it requires both NPC main flags1 and2;
otherwise skips record. The admitted pre-update sets pose snapflag0x100 iff
NPC mainflags0x8000 (else clears it). If NPCmainflags0x100000 is clear it
ORs NPCstate+1d4 mask0x10. Time/counter bookkeeping then runs; at0x80049dc4
it rechecks current mainflags2 and skips remaining record when clear.

At0x80049e90..e94 require current NPCstate+1d4 mask0x10, else jump directly
0x8004a160 past this movement/side-response region. NPC state/time/special
producers are separate lifecycle owners; a native boolean active cannot replace
that second state-word gate.

If NPC+1a4<=0, jump directly to side gating at0x80049f88: no NPC horizontal
movement or npcMoveG call happens on this path. If +1a4>0, obtain movement
components+1b4/+1bc, optionally halve under state0x100000 with state0x10000000
clear; mainflags0x08000000 zeros Z component. Mainflags0x40000000 directly
adds those components to NPCXZ, otherwise npcHitCheckSide handles map response.
Then clear source movement components and invoke npcMoveG before side gating.

At0x80049f88..9fa8, current mainflags8,0x20 or0x4000 each bypass the side
call. Only with all clear invoke npcCheckHitMarioSide. These checks are independent
of draw culling/alpha, talkEvent presence and partner query policy. They close
the direct branch/gate chain to this call, not every preceding timer producer
or full NPC-main state machine.

## Reproduction and boundaries

Fresh read-only ISO inspection of npcCheckHitMarioSide and npcMain plus direct
constant reads establish all local source branches/operations and coefficient
bits. Local inspection files remain /tmp. Documentation-only: no native edits,
make test/build or original-runtime/hardware claim. Underlying angleABf,
compAngle,PSVECDistance,frsqrte initial seed/FPSCR and marioChkWallAround still
require their original numeric/backend owners for instruction equivalence.
