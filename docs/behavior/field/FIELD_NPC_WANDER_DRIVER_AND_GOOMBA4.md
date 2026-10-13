# Original wandering NPC driver and east Goomba4

US G8ME01 disc0 revision0 read-only DOL/REL/tagged-map evidence. Bounded
answer to [OW-INTERACT-01](../../requests/OW-INTERACT-01.md), 2026-10-13.
This is static instruction evidence, not Dolphin timing, hardware measurement
or complete NPC/event lifetime closure.

## Direct callers and work initialization

`gor_villager_urouro` gor.rel0x200e8/0x4c reads position(me,LW0,LW1,LW2),
then invokes urouro_init_func(me,LW0,LW2,100.0,30.0,0), followed by the
ongoing urouro_main_func(me), RETURN/END. Encoded float operands100/30 are
0xf24c0a80/0xf24af280. Tagged aitypeなし does not cancel this regular event.

DOL urouro_init_func0x80237878/0x15c resolves actor via evtGetValue then
EvtNpcNameToPtr, X/Z/radius/speed via evtGetFloat and flags via evtGetValue.
It allocates44 bytes with __mapAlloc0x8002ff58 using heap SDA r13+0x18e8,
publishes the work pointer in EventEntry+c4 and returns2. Allocation failure,
work destruction and regular-event restart ownership are outside this body.

| Work offset | Publication |
| --- | --- |
| +0 | Caller flags |
| +4/+8/+c | Phase/countdown/flip count, all0 |
| +10 | sysMsec2Frame(6000) move budget |
| +14 | sysMsec2Frame(9000) idle budget |
| +18 | sysMsec2Frame(1500) flip wait budget |
| +1c/+20/+24 | Caller X/current NPC Y/caller Z anchor |
| +28 | Caller radius |

It writes speed to NPC+1b0 and ORs mainflags0x00400000. This mask is **not**
the automatic-pose mask0x400 tested later by npcMain; its candidate-loss resume consumer is recovered below. Other consumers remain
open. It does not write current/previous roots. Main0x802372a8/0x5d0 resolves
actor/work and Mario work, services one phase, and returns0 on every local exit.
No phase directly writes actor XYZ or changes animation by name.

## Exact four-phase graph

Let half(n)=signed integer truncation toward zero of n/2. Let R mark an
individual binary32 arithmetic result; normalize means reviseAngle0x80007328.
Source angleABf/compAngle numeric owners are documented in
[dimensions/facing](../display/NPC_DIMENSIONS_TALK_ANCHOR_AND_DISPLAY_REGISTRATION.md)
and [side-response numerics](FIELD_NPC_MARIO_SIDE_RESPONSE_NUMERICS.md).

**Phase0** (0x80237324..3d8): count=half(moveBudget)+irand(half+1), then
countdown=20*trunc(count/20). The compiler division/remainder graph rounds down
to a multiple20 for positive budgets; it does not store count%20. Set flips0;
yaw=normalize(R(R(currentYaw+irand(60))-30)); publish yaw NPC+144 and
movement heading+1ac; phase1. Return without movement.

**Phase1** (0x802373dc..3770c): first compute horizontal source distABf
between NPC and anchor, retaining it across the following ordered checks:

1. First animPoseGetPeraEnd: if ready and distance>radius, OR workflags3 only
   when bit1 is clear; if bit2 set, turn yaw and movement heading toward anchor,
   then clear bit2. If distance<=radius and bit1 set, clear bits1|2.
2. Second readiness query: require ready, marioGetTouchNpcPtr()==this NPC,
   abs(compAngle(NPC movement heading,Mario+1a4 travel))<90,
   abs(compAngle(angleABf(NPCXZ,MarioXZ),Mario travel))<90 and distance<radius.
   Then yaw/heading=normalize(R(heading-180)), clear bits1|2.
3. Third readiness query: if ready and NPC+2fc map contact pointer nonnull,
   yaw/heading turn toward anchor and clear bits1|2.
4. Decrement countdown even when unready. If the result is exactly0, publish
   phase2 and return. Otherwise query readiness a fourth time; false returns.
5. Compute displacement and then service the sound tail below.

These are separate source calls, not a single cached readiness request. Early
exits leave prior movement components untouched. For displacement, speed is
NPC+1b0 and dt is NPC+1a4 **seconds**, unrelated to Mario+1a4 travel angle:
step=R(speed*dt); cap=R(R(20*speed)/1000); if step>=cap replace step with cap.
Thus ordinary finite step is min(step,cap), not max. Angle radians are
R(R(6.2831854820251465*heading)/360); libm sin/cos returns are rounded with
frsp. Publish dx=R(step*R(sin(radians))) to+1b4 and
dz=R(step*(-R(cos(radians)))) to+1bc. Source orientation is -Z0,+X90.

**Phase2** (0x80237710..377d0): countdown=half(idleBudget)+irand(half+1),
without multiple20 quantization. Apply irand(60)-30 to yaw and publish heading
as in phase0. Set flips0, then if workflags0x10000 set, flips=2+(irand(1000)%3).
Publish phase3; return without movement.

**Phase3** (0x802377d4..37850): decrement countdown; only exact0 acts.
Reset countdown=half(flipWaitBudget)+irand(half+1). If flips==0, publish phase0
and return without turning; otherwise decrement flips and publish
normalize(R(yaw+180)) to NPC+144. This idle flip **does not publish+1ac**.
There is no fallthrough into another phase on the same invocation.

Constants r2=0x80427440: +9f8=30, +9fc=90, +a00=180, +a04=20,
+a08=1000, +a0c=6.2831854820251465(bits0x40c90fdb), +a10=360,+a14=0.
Standard binary32 and libm/lookup behavior remain distinct numeric dependencies.

## Sound tail, retained fields and flags

After writing dx/dz, psndGetFlag0x800dc0e8 mask0x100 set together with
animPoseGetMaterialFlag0x8004069c mask0x1800 clear adds sound requestmask0x10000;
otherwise requestmask0. Convert uint16 NPC+1e8 to binary32 and divide by speed.
If elapsed NPC+1dc is strictly greater than that threshold, select sound name
NPC+1e0 when signed halfword toggle+1d8!=0, else+1e4. A nonnull selected name
is resolved by searchPSSFXList0x800dc650; invoke psndSFXOnEx_3D0x800d9808
with (resolvedID|requestmask,rootXYZ,255,255,0,8). Regardless of a null selected
name, reset elapsed0 and write sign-extended byte(1-toggle) as halfword+1d8.
Then if+1e4 nonnull OR toggle!=0, publish elapsed=single FMA(1000,dt,elapsed);
otherwise retain elapsed. No audio tail runs on phase1's early readiness/count
exit or other phases. Sound catalog/playback and elapsed/toggle initializers
are transitive dependencies, not invented output durations.

## Clock, RNG, readiness and NPC-main integration

sysMsec2Frame0x800059cc/0x28 reads live GP+4 frame rate, multiplies with mullw,
and divides the wrapped signed product by1000 with truncation toward zero.
At rate60 the three budgets are360/540/90; this is a conditional deduction,
not evidence that every source context runs at60. irand0x80005da0/0x44 returns0
without consuming rand when bound0, otherwise returns signed rand()%bound.
rand0x8026c324/0x20 updates shared SDA r13+0x14a0 seed modulo2^32:
seed=seed*0x41c64e6d+12345; result=(seed>>16)&0x7fff. Shared seed writers and
exact interleaving with other consumers remain open; a private native seed
cannot claim identical original random sequences.

animPoseGetPeraEnd0x80040750 returns1 if poseflags8 clear; otherwise returns
abs(R(target+7c-retained+74))<double10.0 at0x802c11a0. This is paper-facing
readiness, not animation-clip completion. Entry publishes facing input+70,
retained+74,target+7c all0, leaves sign+78 uninitialized, then npcEntry PeraOn
makes ordinary fresh poseflags9 with snap100 clear. Existing
[Main/Inner facing contract](../display/NPC_ORDINARY_YAW_MATRIX_AND_POSE_FALLBACK.md)
owns the subsequent target/retained angle publication; do not add a clip-end
proxy or initialize sign as if measured source behavior.

Root order is camMain,evtMain,MarioMain,mobjMain,npcMain,caseMain: the regular
user function consumes the previously published NPC dt/map-contact and Mario
touch actor. npcMain initially clears Mario touch+1dc, scans source actor order,
requires mainflags1 and2, and sets state+1d4 mask10 when mainflags100000 clear.
Under state10 it publishes elapsed seconds+1a4 at0x80049d6c from source clock
milliseconds/1000; an elapsed interval>500ms is capped to the source16ms clock
request. Paused/state-clock bookkeeping is not equivalent to globally using
simulation frame delta. See [complete side gates](FIELD_NPC_MARIO_SIDE_RESPONSE_NUMERICS.md).

Horizontal movement requires state10 and dt>0. Components+1b4/+1bc are
already displacements: optional special half scale; mainflag08000000 suppresses
Z; mainflag40000000 directly adds them to current XZ at0x80049ee4..49ef8,
bypassing npcHitCheckSide and not refreshing map contact+2fc. Otherwise the
original map-side owner runs. Then clear both components and invoke npcMoveG;
Mario side response follows unless mainflags8/20/4000. dt<=0 retains components
and skips horizontal movement/npcMoveG but still reaches side-response gates.
Previous XYZ snapshot and placement owners retain their
[source history contract](FIELD_NPC_MARIO_SIDE_RESPONSE_NUMERICS.md).

Automatic pose selection is mainflag00000400, already in setup80600. After
side response compute actual horizontal current-minus-previous norm/dt.
Speed<0.10000000149011612 at0x8041fcdc selects tribe+10 idle and phase+108=1;
otherwise speed<tribe+3c*f31 selects+18 walk and phase2; else+1c run and phase3.
f31 normally1, special state100000 with state10000000 clear uses0.5. Only a
changed phase requests animPoseSetAnim with restart1. Source reciprocal-square-
root refinement/numeric corner branches, gravity/support, regular event stop/
resume and exact source scheduler/backend cadence remain separate owners.

## Goomba4 concrete early slice and quest guard

East gor_02 setup row0x45278; init0x438f0/0x34 sets mask40000000 and calls
set_position(me,-370,0,130) at0x43904, overriding tagged(-375,0,150). Tagged
direction270 remains the facing request. Regular0x43924/0x54 reads current
XYZ and calls init with radius70(encoded0xf24b9280),speed30,flags0, then main.
Fresh setup80600+Entry3+init40000000 gives40080603; wander init gives40480603.
There is no recovered story0..55 visibility predicate in those leaves: that
range is native coverage, not an original admission condition. Complete
room/lifetime/foreign dynamic actor reference closure remains open.

Tribe row0x8033335c NPCクリボー uses a/c_kuribo_n and a/c_kuribo_n-;
+8/c/10/14/18/1c/20/24 poses S_1,Z_1,S_1,T_1,W_1,R_1,S_1,S_1.
Width/height30/30; run threshold+3c=20; step names+40/+44
SFX_FLD_NPC_KURI_MOVE1L/SFX_FLD_NPC_KURI_MOVE1R; uint16+48=12000.
Use source dimensions/anchor and ordinary drawscalar2, not clip bounds.
Original texture/geometry conversion requires separate asset provenance and
backend validation before a native rendering claim.

Talk0x43978/0x450 has **quest branches before story**:

- At0x43978 require raw quest0xf5de070c==2, flag0xf840709d==1 and item0x5f
  count>=1. Print irai_02_03 with speaker me, select item_tbl. If selection!=-1,
  print irai_02_04, remove item0x5f, set flag0xf840709e=1. Return even on cancel.
- At0x43a30 require quest==13, flag0xf840709e==1 and item0x6b count>0.
  iri_13_make_item_tbl precedes first/continuing branch. Flag709f==0 prints
  irai_13_001, poses Mario M_N_5B(wait500ms),M_N_C(wait500ms),M_S_1, then
  irai_13_002 and selection/removal loop. Each matching selection removes
  item0x6b, updates the table and breaks when its returned count is0; otherwise
  print irai_13_003 before repeating. After this first loop set flag709f=1,
  including a canceled/nonmatching selection. Continuing loop uses irai_13_005. If final item
  count0 print irai_13_004; free table then return. These messages use numeric
  speaker0, not me. Original item-table/selection subgraphs remain unbound.

Only when both entry branches do not return does the story switch0x43c58
select ordinary evt_msg_print(0,key,0,me):

| First-match condition | Original US message |
| --- | --- |
| <56 | gor_02_030_00 |
| <112 | gor_02_030_01 |
| <164 | gor_02_030_02_01 |
| <175 | gor_02_030_03 |
| <176 | gor_02_030_03_01 |
| <215 | gor_02_030_03_02 |
| <261 | gor_02_030_04 |
| <339 | gor_02_030_05 |
| <373 | gor_02_030_06 |
| <401 | gor_02_030_07 |
| default | gor_02_030_08 |

Early0..55 ordinary print at0x43c68 is exact only after quest guard. A native
implementation without item services may conservatively refuse both quest/flag
pairs, rather than fabricate their dialogue or assume low story excludes them.
Missing quest/flag state is unknown. Quest global namespaces, lifecycle states
and original payload/control tokens need binding before those branches activate.

## Reproduction and outstanding owners

Read-only rel_event_inventory.py exported gor_villager_urouro,kuribo4_init,
kuribo4_regl,kuribo4_talk with ADDR32/import bindings directly from ISO.
gor.rel642564 bytes SHA256d4965f21254ecf1a78e5d66ea12a0b000eed3f123032af6ab3cb0f703d3f185a;
generic wrapper SHA25623c48a8c42386fe13eac63d26820766f899b1fd9be292b59f6870a12da2ab3d3.
inspect_dol_function.py recovered init/main, readiness,sysMsec2Frame,irand,rand
and npcMain; DOL pointers/constants/tribe row independently read through ISO
DOL section address mapping. Temporary disassemblies and extracted artifacts
stay outside source history; no ISO or original dialogue/assets distributed.

Still open: additional 00400000 consumers, allocation/event-work destruction and restart,
source event pause/resume versus regular reentry, complete clock/gravity/map
support owners, shared RNG seed/cadence, original quest item tables/window/
control/state transitions, exact lookup/libm/sqrt/backend numerics and matched
Dolphin/New2DSXL behavior/performance. Docs-only validation makes none of these
runtime measurements. No native code, tests, emulator profile or saves changed.


## 2026-10-13 follow-up: authored ground no-op and retained regular lifetime

Fresh npcEntry0x8004b938..944 clears all832 bytes before setting mainflags3.
Consequently seconds+1a4, vertical displacement+1b8, horizontal components,
contact+2fc/+300 and sound elapsed/toggle begin0. It seeds clock anchor
+198/+19c from GP+38/+3c at0x8004ba38..48. Init/regular timing relative to
prior npcMain calls means the wander callback need not always see entry dt0;
there is no authored20ms default. Clock initialization and dt publication are
separate from sysMsec2Frame's budget conversion.

Complete npcMoveG0x80048d9c/0x330 captures vertical+1b8 then clears it before
any gate. It optionally halves the captured displacement under state100000
with state10000000 clear. Gravity contribution starts0 and is computed only
if mainflags00020000 set; constructor40000000 is not a gravity gate. Then
combine captured displacement and gravity. With mainflags10 clear, equality
to0 at0x80048ee4..ee8 exits BEFORE Y/contact/state/ray writes. Nonzero positive
vertical sets state1 and adds Y; negative vertical probes ground via
hitCheckFilter0x8001577c, with center and two width-offset fallback rays and
contact+300 publication. General gravity/time/scaling/ray support numerics
are beyond this authored no-op deduction.

Selected Goomba4 flags40480603 lack both00020000 and10. Its init changes only
flags and XYZ; regular writes only horizontal displacement, yaw, heading,
speed and work. Thus with this source constructor/init/regular setup and no
foreign vertical/flag writes, npcMoveG sees netvertical0 and performs no ground
probe or rootY mutation. Direct horizontal movement bypasses map-side checks
under40000000 as already pinned. Native floor snapping/map-wall correction for
this actor would introduce behavior not requested by these source owners.
This conclusion is bounded to the selected lifecycle; it is not a general
NPC gravity implementation or a claim that every actor with40000000 floats.

### Setup creates init first, regular after completion

`evt_npc_setup`0x8005323c/0x288 on first callback traverses original92-byte
setup rows, resolves existing actor and ORs main/secondary flags. A nonnull
row init+0c deletes existing NPC init event+118 if nonzero, creates
evtEntry(init,priority0,flags0), sets event byte10=0,actor context+170=NPC
and publishes eventID+15c to NPC+118. It saves regular script row+10 to
NPC+124, talk+14 to+128 and other event metadata. It returns0 after setting
field mode1. Later polls return0 while any selected actor initID+118 is live
per evtCheckID; once all end, each nonnull regular script deletes an existing
regularID+11c, creates evtEntry(regular,0,0x20), sets event byte10=1 and
actor context+170, savesID+15c to+11c, then returns2.

The [root constructor](../event/EVENT_ROOT_CONSTRUCTORS_AND_APPEND.md) gives
these events type0xef;0x20 permits source active-manager append. The
[USER_FUNC continuation](../event/EVENT_DISPATCH_AND_WAIT_CONTINUATIONS.md)
owns return2 same-invocation next command and return0 retained callback. Thus
urouro_init_func's2 can immediately reach main's first0 in the same admitted
dispatch invocation. Main0 does not fetch its following RETURN/END. Event
manager credit/speed can cause multiple polls, or none when stopped; phase
countdowns are callback invocations, not independently guaranteed rendering
frames. Do not reset the driver each frame or replace this owner with a fixed
wall-clock duration. Admission after map init and complete manager interleaving
remain [event runtime](../event/EVENT_RUNTIME.md) dependencies.

### Candidate admission pauses before A; candidate loss resumes

In fbatEncountCheck0x80047bec..47c90, the candidate admission boolean is
distinct from strict-nearest winner. When a ray/facing/radial admitted actor
lacks NPCstate+1d4 mask20000000: set that mask, save ry+144 to+148, call
evtStopID(regularID+11c), and turn ry toward Mario unless mainflags01000000.
Already marked actors retain that state/yaw. This takes place during the
ordinary candidate scan **before A event admission**, including admitted
actors that lose nearest selection.

When that actor no longer admits, clear state20000000. At0x80047c64..7c80
mainflags00400000 restores ry from+148 and calls evtStartID(regularID+11c);
without that mask it instead ORs main NPC event flags+140 with10000000, a
separate later owner. This is a concrete reason the wander init ORs00400000;
it is not automatic-pose400. Mainflag/state-word400000 uses in ordinary draw
are distinct: the recovered draw occurrences load NPCstate+1d4 for color
darkening and must not be mistaken for this mainflags consumer.

[Talk lifecycle](FIELD_ORDINARY_NPC_TALK_CANDIDATE.md) then stops regular again
in fbatTalkMode stage0 and starts original talk event on the following NPC
invocation after A1→2. Talk completion waits for eventID death; when
state20000000 clear, starts regular and restores saved ry. When that mask
remains set it leaves regular paused for candidate-loss ownership. Automatic
pose400 resets NPC+108 on talk completion; this must allow pose selection to
restore movement/idle after the original talk pose, rather than resetting
urouro's work phase/countdown.

Exact StopID/StartID bodies scan every matching active event ID and forward
hardcoded categorymask0xef. The called evtStop/evtStart traverse event
relationships and set/clear event flags byte8 mask2 only when type intersects
the mask; this ordinary root's typeef intersects. No pointer/work/countdown/
program-counter reset occurs in those stop/start bodies. See
[event control scans](../event/EVENT_CONTROL_OPCODES_AND_ID_SCANS.md). Explicit
restart/delete/setup is separate from pause/resume and may destroy/recreate
work; no native teleport-distance heuristic can substitute that lifecycle.

### Mario touch actor spans exactly the following source field interval

`marioGetTouchNpcPtr`0x80096948 reads Mario+1dc. `npcMain` clears that pointer
at0x80049adc before its ascending pool scan. Each admitted Mario/NPC side
collision can publish the colliding NPC there; later source actors can replace
it, with no nearest arbitration. The next field evtMain precedes MarioMain
and npcMain, so urouro sees the previous NPC scan's retained pointer. Current
NPCmain then clears it and republishes after current Mario movement. A native
driver invoked after clearing or after current collisions would consume a
different interval. See [source side publication](FIELD_NPC_SPHERE_AND_MARIO_SIDE_COLLISION.md).
Other field sequence/pause and foreign writes remain separate owners; a native
current-frame touch boolean must not silently replace this retained pointer.

All follow-up bodies extracted read-only with inspect_dol_function.py; mask/
load-owner checks pin the00400000 candidate branch separately from draw state
flags. No original assets, code, saves or source ISO mutated, and no measured
Dolphin/New2DSXL claim added. General quest state remains unknown unless its
source globals are explicitly published; these lifecycle findings do not
authorize assuming quest0.


## 2026-10-13 follow-up: paused regular does not pause NPC clock/pose

The per-actor npcMain loop publishes dt and services horizontal movement,
component clear, npcMoveG and Mario-side collision before automatic-pose
selection. It then registers presentation and copies current XYZ to previous
+98/+9c/+a0 at0x8004b10c..120. Only after all actors are processed does the
field-mode dispatcher call fbatEncountCheck at0x8004b31c. Candidate admission
stops the regular event after that invocation's current movement/pose/snapshot;
there is no retroactive cancellation of already applied movement.

StopID modifies event stopped flags, not NPC clock anchors/dt/state10. A regular
callback paused by candidate state20000000 therefore consumes no new phase/RNG/
countdown calls, while ordinary admitted npcMain continues publishing seconds
and servicing any retained movement components. After a dt>0 movement pass
clears those components, a following pass without a new callback or other
writer moves zero. Actual post-collision root delta divided by current dt then
selects idle S_1/phase1 under mainflag400 (unless other root/movement writers
intervene). The wander phase can still be moving phase1 or idle phase3; it is
independent from NPC automatic-animation phase+108.

Idle driver phases and readiness/countdown early exits do not themselves clear
components; an adapter must retain their fields and let npcMain own the clear.
Under NPCdt<=0 source skips movement/clear/automatic speed selection but still
reaches the existing side-response gates. Regular pause is not equivalent to
forcing dt0, resetting clock anchor, resampling RNG, restarting driver phase0,
copying current root into previous prematurely or immediately issuing idle
before the already serviced stop-frame movement.

The [candidate contract](FIELD_ORDINARY_NPC_TALK_CANDIDATE.md) now distinguishes
per-actor full admission from strict-nearest winner and skipped/unvisited actor
paths. Original talk-event presence is independent of native quest/playback
availability. Turning copies only ry+144/+148, preserving movement heading,
driver work and paper-facing retained state; presentation remains owned by the
normal Main/Inner path. This admits original auto-turn without inventing quest0
or playing an unsupported quest message. Exact stop/start source timing and
numeric/render cadence still require matched original/runtime observations.
