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

## Exact compAngle wrap and tie behavior

`compAngle`0x800071f0,size0x3c takes angleA in f1, angleB in f2.
Compute D=R(B-A), then abs(D) in the source floating register. When
abs(D)>=180, adjust B exactly once: B<A adds360, otherwise subtracts360.
ReturnR(adjustedB-A). Otherwise return the original difference through its
final subtraction. Constants180/360 come from0x8041f464/0x8041f45c.

A difference+180 returns-180; difference-180 returns+180. This is not a
repeat-until-in-range wrapper: inputs separated by720 retain a360-sized result
after one adjustment. The side caller takes fabs of the returned value before
its strict40 comparison. Do not replace this with a generic nearest-angle
helper with different tie or repeated-wrap policy. NaN/unordered branches and
signed-zero/exception state remain part of the original numeric environment.

## Previous position snapshot and explicit placement

`evt_npc_set_position`0x80053158,size0xe4 resolves actor and XYZ, then writes
all three resolved coordinate words to NPCpreviousXYZ+0x98/+0x9c/+0xa0
at0x800531f4..53200. Only afterward copies previous to currentXYZ+0x8c/90/94
at0x8005320c..218. Explicit scripted reposition therefore seeds both states;
side rollback must not use an obsolete native previous position after this
service. `npcEntry` likewise seeds current and previous roots from its source
initial vector at0x8004b9b0..9dc.

During npcMain, previous position is not replaced before this response.
Current→previous snapshot is at0x8004b10c..120, after ordinary NPC horizontal
movement, optional npcMoveG, npcCheckHitMarioSide, later pose/color/display
registration and their bypass branches. It writes all XYZ, including currentY.
This preserves the preceding admitted NPC invocation's root for the source
rollback; explicit placement between invocations replaces that history.
Mainflags1/2-rejected records skip this snapshot, so it is not an unconditional
per-render or all-actor tick write. Longer transform/history-ring writes after
this point are distinct from previousXYZ.

## Wall retry ABI and semantic adapter mapping

Fresh full `marioChkWallAround`0x800888d0 proves r3=current mutable root,
r4=integer mode (retainedr21, only compared0 at0x80088d68; never dereferenced),
f1=ignored float speed,f2=heading,f3=query width,f4=height. The old
structure-only [wall sketch](MARIO_CHK_WALL_AROUND.md) called r4 an output
pointer; that label is superseded by this complete-body operand-use check and
[complete ordinary wall contract](../overworld/OVERWORLD_WALL_MOVEMENT.md).

NPC side call requests mode0,speed0,headingR(theta+160),queryWidth20,
currentMarioHeight. The semantic `correctFieldWallsAround` heading should be
source sincosf(theta+160)=(sin,-cos), independently of response delta. Give
its temporary actor width20,height=currentMarioHeight and source mode0; do
not pass the NPCwidth or separation radius. Query ray radius is half20=10,
normal cap max(1,.125*20)=2.5; mode0 motion10/11 uses max(1,.25*20)=5.
View flags and current motion remain source Mario inputs.

This mapping does not close the existing semantic backend's numeric/second-
pass implementation differences. Source saves the **unclamped** first correction
for the second-pass dot test; negative or unordered dot admits half second
correction, positive/equal suppresses it. Existing adapters must preserve these
owners or keep differences explicit. Original around-ray attribute policies
apply to this wall retry, unlike the ordinary talk null-callback ray.

## Static Chu and scripted Frankly flag requests

Read-only REL records: Chu3 row0x452d4 flags0x80600,secondaryflags0;
Frankly row0x44ff4 flags0x80600,secondaryflags0. NPCEntry publishes3,
setup ORs these flags, giving ordinary Chu0x80603. Chu init0x43dc8 returns/end
without further main-flag requests. Frankly init0x410c8 explicitly ORs
0x40000000, giving fresh ordinary entry0x40080603. This mask controls his
movement bypass, not the side-response terminal mask0x20.

Firsttalk event0x48fe0 ORs0x40000020 then0x48ff4 clears0x40000020;
wall-remove0x4919c ORs0x40000020 before movement/083/fence choreography,
and0x4949c clears it after084 before camera/input/story10 restoration.
`evt_npc_flag_onoff`0x800500b0 verifies action1 ORs the current main word,
action0 AND-NOT clears it. Preserve both bits independently and source order:
mask0x20 disables side response only during those windows; clearing also
removes0x40000000 until a new init/request restores it. Thus a fresh story10
room reload and same-room continuation can have different current flags even
though story values match. Hidden story9 uses source position(0,-1000,0),
not an invented general visibility-to-collision flag. Current source actor
state,position and event lifetime remain owners for collision eligibility.

## Mario travel-heading publication for response input

The side response reads Mario retained travel heading+1a4, not the local
heading chosen by marioMoveMain for a particular combined displacement.
Fresh local write census:

| Owner | Source +1a4 publication |
| --- | --- |
| Ordinary mot_walk | Current input+198 copied at0x80087c4c/450 after its ordinary request/arbitration gates; stronger input transition can return before this write |
| Ordinary mot_dash | Current input+198 copied at0x80087934/938; transition to walk returns before this write |
| mot_stay / mot_jump | Neither local body writes+1a4; called presentation/idle helpers have separate writers |
| marioAdjustMoveDir | Both ordinary snapped-display branches publish facing+1a0 and copy it to travel+1a4 at0x80096778/7c or0x800968c4/c8; also snapdisplay+1ac/target+1b0 |
| marioCheckWall | No+1a4 writes; nonzero wall-direction metadata updates separate facing+1a0 |
| marioMoveMain ordinary | Reads+1a4 for retained base-vector construction; its local combined heading and zero-norm toMovedir fallback do not publish+1a4 |
| marioMoveMain special flag0x40 | Reads forced heading+1a8 and stores+1a4 at0x80059998/9c; separate mainflag0x400 reuses retained+1a4 locally |

[Ground motion arbitration](../overworld/OVERWORLD_MOTION_ARBITRATION.md)
retains the exact request/transition gates. A walk tail can still publish input
heading after requesting pending jump; the eventual jump body has no local
travel rewrite. Zero-ground-input and airborne drift do not license overwriting
travel with display fallback or combined movement heading on every call.
A native retained ordinary travel field is an appropriate adapter only when
it preserves these source publication/lifetime rules. The original low-HP/long-idle service can invoke
[adjustMoveDir](../overworld/OVERWORLD_MARIO_LOW_HP_IDLE.md), which **does**
publish travel: ordinary mainflag0x00100000-clear snaps D=180 when current
display is inclusive90..270,elseD=0; H=wrap(Q1000(270-D+offset19c));
write facing1a0=H,travel1a4=H,display1ac=D,target1b0=D. Native callers of
that source service must publish both retained facing and travel at the service
invocation; this does not make every idle tick or combined displacement a
travel writer. Authored direction/event,transformation,placement and other
movement owners can write that source field
separately; this bounded census is not a full-game global-write closure.

## Scripted NPC movement does not reseed previous position

Fresh full evt_npc_move_position0x80052254,size0x690 has no direct writes
of currentrootXYZ+8c/90/94 or previousrootXYZ+98/9c/a0 and no call to
explicit evt_npc_set_position. Entry stores destinationXYZ+164/168/16c,
using resolvedX,currentY,resolvedZ; captures startXYZ+158/15c/160. It derives
heading+1ac, speed/driver state and movement components+1b4/+1bc during
continuation. NPC-main movement/collision then owns applying those components
and its ordinary end-of-update snapshot described above.

Therefore scripted movement steps are not repeated explicit placement or
teleport-history resets. A native actor placement generation should advance
for original entry/explicit-set-position/recreation requests, while a continuous
move driver preserves previous root until the NPC-main snapshot. Do not invent
a distance threshold to classify a source move command as teleportation. The
full movement driver event timing/speed/pose/pause and other command variants
remain separate, but this command's current/previous-root write absence and
explicit placement distinction are directly verified.

## Reproduction and boundaries

Fresh read-only ISO inspection of npcCheckHitMarioSide and npcMain plus direct
constant reads establish all local source branches/operations and coefficient
bits. Local inspection files remain /tmp. Documentation-only: no native edits,
make test/build or original-runtime/hardware claim. Underlying angleABf,
compAngle,PSVECDistance,frsqrte initial seed/FPSCR and marioChkWallAround still
require their original numeric/backend owners for instruction equivalence.
