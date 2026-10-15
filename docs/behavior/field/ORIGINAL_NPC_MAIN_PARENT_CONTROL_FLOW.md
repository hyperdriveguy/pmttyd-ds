# Original NPC Main parent control flow and movement publication

US G8ME01 revision0 static read-only DOL evidence. This maps the entire local
npcMain80049a7c/19a4 parent, including paths beyond the admitted native ordinary
slice. Called owners remain separate providers; this is not a claim that
those providers, original instruction numerics or the native rendering path
are complete. Read [wander event ownership](ORIGINAL_WANDER_EVENT_PACKED_BINDING_FRONTIER.md),
[side-response numerics](FIELD_NPC_MARIO_SIDE_RESPONSE_NUMERICS.md),
[candidate/interrupt](FIELD_ORDINARY_NPC_TALK_CANDIDATE.md) and
[ordinary display](../display/NPC_ORDINARY_YAW_MATRIX_AND_POSE_FALLBACK.md).

All offsets are hexadecimal. A denotes the current packed NPC, S=A+1d4
state, M a separately requested Mario pointer, F the current GP+168 field
work. Source callbacks may change later freshly loaded values. Missing native
providers must suspend at their actual request with completed stores retained;
they do not authorize skipping the rest of this parent or moving its snapshot.

## Entry, source registry and common actor gates

Capture initial GP; select registry8041e8b4, adding14 when that GP+14 is
nonzero. Request marioGetPtr and store0 to returned Mario+1dc (collision
pointer). This is distinct from radial-hint+1d8 consumed by wandering.
Read npcMainCount8041e8ac, registry count+4 and array+c, then increment/store
MainCount. Count and starting array are retained for the ascending340-stride
loop; do not substitute a filtered renderer/catalog vector.

For each record at80049b00:

1. Capture A+0 flags; inactive1 or query2 clear skips the entire record,
   including pose, clocks, snapshot and final state clear.
2. Request animPoseGetAnimPosePtr(A+104). Mainflag8000 sets returned pose
   flags100, otherwise clears100. This is a real pose mutation before clocks.
3. Fresh main100000 clear ORs S with10. Then state10000 set clears S10.
4. Read unsigned halfword A+320; decrement/store if nonzero.
5. S10 selects clock accounting below. Otherwise refresh only clock anchor
   low19c/high198 from fresh GP38/3c, preserving stale dt1a4 and elapsed fields.
6. Recheck fresh main2. If clear, skip all remaining work to final S10 clear
   at8004b1b4; this differs from initial rejection, which skips that clear.
7. Copy prior special state100000 into state200000: set200000 iff100000,
   otherwise clear200000. Clear state100000. Compare fresh GP+12c with source
   string kpa_03 at802c1394. For this match and not(A.Y>365), set100000;
   unordered/NaN Y enters because the original branch rejects only greater.
   if prior200000 clear and this new100000 set, request psndSFXOn_3D with
   source name SFX_KUPPA_3RD_WATER_FALL1 at802c139c and pointer A+8c.
8. If state100000 and not10000000, set A+1d0=100 and retain f31=.5;
   otherwise retain f31=1. State100000 also ORs A+31e halfword with1.

## Clock stores: source ticks, integer quotients and float seconds

Under S10,80049b94..49da8 performs:

- Reload GP; compute wrapped64 GP timestamp38/3c minus A anchor198/19c.
  Store subtraction low to194, then high190. Read bus word800000f8.
- Let B=bus>>2; Dms=high32(10624dd3*B)>>6. Signed __div2i(delta,Dms)
  is compared against500. Only a result strictly greater500 replaces delta
  with low32(Dms<<4),high0, stored low194 then high190 (source16ms cap).
- Mainflag20000: add current delta to accumulated178/17c, low then high;
  otherwise store0 to17c then178. Always add delta to188/18c, low then high.
- Reload GP and current timestamp; store anchor low19c then high198.
- Publish four separate conversions in this order: elapsed188→1a0,
  accumulated178→180, delta190→1a4, absolute anchor198→1a8.
  Each reloads bus and its own source high/low words. Let
  Dus=high32(431cde83*(bus>>2))>>15. Form wrapped64(source<<3), divide signed
  by Dus with __div2i, convert signed64 through rounded binary64 then
  binary32 with __cvt_sll_flt, then
  SP divide by **1000000.0**, literal49742400 at8041fd50, and store.

At bus243000000, Dms60750 and Dus486. A source17ms delta1032750 ticks
becomes quotient17000 microseconds then float .017 seconds. Converting rounded
integer milliseconds/1000 is an algebraic shortcut with different precision;
it does not reproduce this instruction sequence. Negative/overflow/zero-divisor
domains belong to explicit integer/numeric admission, not guessed saturation.
Regular-event StopID does not disable these NPC clocks.

## Movement, gravity, side response and automatic animation

At80049e90, S10 clear skips this entire region to8004a160. With S10 set,
dt=A+1a4 not strictly positive skips horizontal clear/gravity to side gating.
Otherwise capture A+1b4 X and A+1bc Z displacements; state100000 without
10000000 halves both. Main08000000 zeros captured Z.

Main40000000 directly loads currentX8c, SP adds capturedX, stores8c; then
loads currentZ94, SP adds capturedZ, stores94. It does not multiply by dt,
touch currentY, copy previousXYZ or refresh map contact2fc. Otherwise call
npcHitCheckSide(A,&temporaryX,&temporaryZ,&A.heading1ac), store returned pointer
to2fc and reload it. Nonnull contact calls the same side helper a second time
with temporary-minus-fresh-current displacement and actual current X/Z output
pointers; null contact copies temporary X/Z into current fields. The second
call's return is discarded; it does not replace2fc.

After either path clear1b4 then1bc to+0; call npcMoveG(A). Source gravity owner
captures/clears1b8 before its own gates. The authored no-gravity/no-displacement
case is described by the wander contract; it is not a general substitute.
Then fresh main8/20/4000 each bypass npcCheckHitMarioSide; otherwise call it.
Even dt<=0 reaches this side gate. See the full side service contract.

Request PSVECSubtract(current8c,previous98,temporaryDelta). If main400 and
dt>0, compute source horizontal norm(deltaX,deltaZ)/freshdt. Below binary32
.10000000149 choose descriptor+10 idle/phase1; otherwise below
descriptor+3c*f31 choose+18 walk/phase2; else+1c run/phase3. Only phase changes
request animPoseSetAnim(handle,name,1), then store A+108 phase. Preserve the
source norm sequence and exceptional-number paths below. No direct root or
previous writes happen as part of pose selection.

## Event detection and npcCheckInterrupt parent gates

At8004a160, regularID A+11c==0 jumps to display8004accc, skipping
npcCheckInterrupt itself. Nonzero ID requests evtGetPtrID. Null returned entry
or its stopped flag2 immediately goes to npcCheckInterrupt8004acc4. Otherwise
capture returned event byte10 into T and process the following priority:

| Local range | Predicate and result |
| --- | --- |
| a184..a2d8 | If A+13c script nonnull and T neither5 nor8: unless main20000000 or20, request cloudGetBreathPower at three current-root samples: Y, Y−.5*height150, Y−height150, each with width14c. First positive power posts A+140 bit8 and jumps to interrupt. |
| a2dc..a4e0 | If A+1f8 territory kind nonzero and T1/2: kind1 checks horizontal distance from center1fc/204 against214; kind2 checks center±halfextents (T2 uses214/21c, T1 uses208/210). Outside posts trigger4 and jumps to interrupt. Other kinds produce no departure trigger. |
| a4e4..a858 | T2 avoidance path: capture Mario−NPC XYZ, previous−current vector and default yaw144. If S2 and horizontal movement norm>.05 use heading1ac instead. Apply player admission and range/height/angle graph below. Failure posts trigger2 and jumps to interrupt. |
| a85c..acc0 | Only T1/4 reaches proximity path. Capture Mario−NPC XYZ and horizontal distance. Main08000000 subtracts original level-indexed kpa radius from distance, clamping below0 to0. Reduce NPC yaw with reviseAngle; choose literal90 when<180,270 otherwise. Apply player/range/height graph below; admitted path posts trigger1. Other T values go directly to interrupt. |

The repeated player admission graph (a634/a9fc) requires Mario flags
82000000 clear, flagsa clear, GP164==0, seqGetSeq()==2, Mario halfword2e!=25,
and vivianGetStatus()==0 or a second separate vivianGetStatus()==1.
Do not coalesce the repeated status requests or use current native Scene
without proving the source sequence/motion/state mapping.

For T2, failed player admission is avoidance failure. Otherwise, only when
horizontal distance>.25*A228: fail if Mario−NPC Y>A20c, its negation>A218,
distance>A228, or abs(compAngle(retained facing,
angleABf(0,0,deltaX,deltaZ)))>.5*A22c. Distance<=quarter range bypasses all
these additional tests. Failure posts trigger2; success continues later graph.

For T1/4, failed player admission rejects trigger1. Otherwise adjusted
distance>.25*A220 requires distance<=A220 and absolute direction difference
<=.5*A224; quarter range bypasses direction only. All admitted distances then
require vertical delta<=A20c and negative delta<=A218. T4 additionally requires
inner territory: kind1 distance-to-center<=double(.75)*A214; other kinds use
inclusive rectangular center±(.75*214/.75*21c). A failed check suppresses1.
Source strict/inclusive comparisons and unordered paths must follow the
original branches, not a generalized geometric predicate.

Finally npcCheckInterrupt(A) runs once for every nonzero-ID path, whether
the old entry is alive, stopped or missing. Its prioritized event replacement,
pointer/ID publication and work reset are documented separately. No return
value from it controls display. Initial regularID0 does not enter this owner.

## Display, material, shadow and current→previous history

At8004accc, main800 hides all following display/shadow registrations but still
reaches snapshot8004b10c. Otherwise camGetPtr4→PSMTXMultVec(view11c,currentRoot),
fresh camGetPtr4→PSMTX44MultVec(projection15c,temp,temp). Ordinary cull requires
projected−1<Z<0 and inclusive X/Y−2..2. Main40 bypasses cull failure.

If any color byte A+114..117 differs fromff: set actual pose material flag40,
capture A+114 color word and request animPoseSetMaterialEvtColor. Otherwise
clear material40. Compute sortdepth=clamp(FMA(5000,projectedZ,5000),0,10000),
then multiply by−1. Main80 bypasses ordinary-main/shadow/blur queues but
continues optional separate110-pose display. Also halfword320 bit2 with
state10000 clear takes that bypass (after the earlier decrement).

Ordinary queue: alpha117!=ff requests dispEntry(A+1f4,2,npcDisp_xlu8004c7e0,A,depth);
opaque alpha requests npcDisp8004c50c three times classes0,1,2. Then:

- Main04000000 selects Vivian path: root temporary, downward hitCheckFilter
  query, hitY replaces temporaryY when hit; animPoseVivianMain(handle,temp);
  shadowEntry at currentRoot with width-derived scale and A+154. Main80000
  requests shadowSetCollision(token,0).
- Otherwise main40000 suppresses ordinary shadow only. Normal shadow copies
  current root, and descriptor nonnull offsets by scaled descriptor30/34/38,
  with X sign from sinfd(literal90/270 selected by normalized yaw<180).
  shadowEntry uses factor1.33000004*(width/3*scaleE0*154); main80000 again
  requests shadowSetCollision(token,0).
- Main100 requests dispEntry(A+1f4,2,npcDisp_blur8004bc3c,A,depth), even when ordinary
  shadow was suppressed by main40000.

At8004b080, A+110==-1 bypasses alternate pose. Otherwise alpha!=ff requests
dispEntry(1,2,npcDisp_offscreen_xlu8004c234,A,depth); opaque alpha requests
dispEntry(1,class0/1/2,npcDisp_offscreen8004bf28,A,depth). No implicit submission occurs
when the original parent bypasses this point through culling/main800.

Snapshot8004b10c captures currentX and currentY words together, stores prevX98
then prevY9c, then reads fresh currentZ and stores prevZa0. Main100 additionally
shifts five XYZ history vectors only on even fresh MainCount: d4←c8,c8←bc,
bc←b0,b0←a4,a4←current. Each XYZ copy reads X/Y, writes X/Y, then reads/writes Z.
Finally clear S10 at8004b1b4; increment index and actor pointer. Thus snapshot
is neither an early pre-move operation nor an unconditional all-slot update.

## Complete post-pool field tail

Reload current GP→F. Retain effect record E=F+550. If E+0 nonzero, E+8 state2
increments E+c count: <20 calls intplGetValue(5,count,20,-600,0)→E+10;
20..79 writes E+10=0;80..99 calls intplGetValue(1,count−80,20,0,+600)→E+10;
>=100 clears E+4 and skips queue. Reached animation steps set E+14=0 and
dispEntry(9,2,_fbatFirstAttackAnnouceDisp80047dfc,E,0). E+8 state1 increments count; >60 clears E+4,
otherwise writes E+10/E+14=0 and submits the same queue. Other states skip.
E+0 is not cleared by these completion branches; do not change that store.

Read signed halfword F+0 once. State1 calls fbatEncountCheck then freshGP→F2
and marioGetPtr. Read retained Mario byte3c freshly before kpaJumpChk and
again before kpaFireAttackCheck; each request occurs only if its own read==2,
otherwise that local result0. A jump callback can change the second gate. Jump true: if F2+57c>0 and
F2+578>=2, request kpaAddScorePos(F2+57c,&MarioRootPlusY60); then clear578/57c
regardless of reward eligibility. Fire false clears F2+574. States2/3 call
fbatTalkMode; state5 calls fbatBattleMode;4/6/others do neither. Finally clear
byte F+54d using the original retained F, even if a callback changed GP+168.

## Numerical primitives and complete direct frontier

Repeated norm blocks (49fd8,4a304,4a534,4a6d4,4a870,4ab44) retain their
distinct SP sum versus FMA construction. Positive squared norm uses frsqrte,
three double Newton steps with constants.5/3, product squaredNorm*refinedReciprocal,
then frsp. Negative q returns source NaN8041e3c8; nonpositive/unordered q is
classified by exponent/fraction, replacing NaN with that source NaN and
otherwise retaining q (including zero). This source primitive needs an explicit
numerical provider to claim instruction parity. Ordinary std::sqrt is a native
adaptation, not evidence of identical rounding or exceptional behavior.

The complete unique direct-call inventory, call sites, constants and original
listing are in ignored artifacts/npc-main-parent-evidence/inventory.json and
npcMain.txt. Direct identities in source order of first occurrence:

`marioGetPtr, animPoseGetAnimPosePtr, __div2i, __cvt_sll_flt, strcmp,
psndSFXOn_3D, npcHitCheckSide, npcMoveG, npcCheckHitMarioSide, PSVECSubtract,
animPoseSetAnim, evtGetPtrID, cloudGetBreathPower, seqGetSeq, vivianGetStatus,
angleABf, compAngle, kpaGetLevel, reviseAngle, npcCheckInterrupt, camGetPtr,
PSMTXMultVec, PSMTX44MultVec, animPoseSetMaterialFlagOn,
animPoseSetMaterialEvtColor, animPoseSetMaterialFlagOff, dispEntry, sinfd,
shadowEntry, shadowSetCollision, hitCheckFilter, animPoseVivianMain,
intplGetValue, fbatEncountCheck, kpaJumpChk, kpaFireAttackCheck, kpaAddScorePos,
fbatTalkMode, fbatBattleMode`.

Source callback targets queued by dispEntry are retained identities, not direct
calls inside this body. GPU/pose facing/color/shadow callbacks and queue lifecycle
must actually consume those requests. Existing native catalog state cannot
replace the real registered owner, original pose namespace or retained Mario
and F ownership. A complete parent adapter may suspend on an unavailable
transitive service; it must preserve the reached source cursor and earlier
clock/move/audio/event/display effects instead of restarting the whole pass.

## Reproduction and scope

Read-only tools/inspect_dol_function.py npcMain with
artifacts/reference/us_symbols.csv and the supplied ISO yields original body
SHA256decc7aa26c3e7990624bbb9204ed0ce286ecabc49405d3140390a301e59a6b7f.
The evidence inventory records raw constant words, not inferred measurements.
The static source audit itself changes no code, build, asset import, emulator,
user profile or save. Native follow-up validation is documented separately.
This local complete block map does not complete called physics/query/battle/
display services or prove original/New2DSXL runtime/performance fidelity.

### 2026-10-14 Tail interpolation sign correction

Read-only instruction/constant audit pins first -600->0 and second0->+600;
[history/traversal evidence](NATIVE_NPC_MAIN_HISTORY_AND_TRAVERSAL.md) records
addresses/hashes. Earlier prose reversed both signs. Field tail remains open.
