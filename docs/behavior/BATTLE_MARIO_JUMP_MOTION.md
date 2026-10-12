# MarioJumpPosition: initialization, integration and event completion

US G8ME01 revision0;recovered2026-10-12 by documentation agent ID2.
Evidence B:complete btlevtcmd_MarioJumpPosition at0x801093a8,size0x8c0,
SHA256`bfe6ed6ef66602af64ebad20435d2f708346d851172f3978391411913a433c40`.
This is the Mario-specific body,distinct from generic JumpPosition0x8010d208.
The [numerical contract](BATTLE_MARIO_JUMP_NUMERICS.md) specifies rounding and
equations;[sound-mode contract](BATTLE_UNIT_SOUND_MODE.md) closes the local loader.
No original executed impact tick,animation or audio trace is asserted.

## Arguments and independent storage

Every invocation,including resumed calls,resolves all seven operands in order:
actor via evtGetValue/BattleTransID;X,Y,Z via evtGetValue followed by signed32
to binary32 conversion;count N,mode M,sound-mode S via evtGetValue. Fetch U
using the current shared BattleWork and translated ID. No evtGetFloat or local
unit/null validation occurs. M/count/actor can be reread differently on a resumed
call if their source variables change;do not cache the whole argument set only
at initialization. The caller-supplied first-call integer is tested for nonzero.

| Storage | Local role |
| --- | --- |
| Event+0x0d,signed byte | Callback phase1,2 or3 |
| U+0x3c/+0x40/+0x44 | Visible X/Y/Z position |
| U+0x154/+0x158/+0x15c | Linear baseline P(X/Y/Z),separate from visible sine offset |
| U+0x160/+0x164/+0x168 | Retained target T(X/Y/Z) |
| U+0x16c | Signed remaining integration count n |
| U+0x170 | Horizontal speed,adjusted in initialization;not used to advance baseline per step |
| U+0x174 | Phase increment base G;terminal path writes1.8 |
| U+0x178 | Sine phase Phi;terminal path computes negative self-difference |
| U+0x17c / +0x180 | Heading-helper result / original horizontal distance D |
| U+0x184 | Height amplitude H |
| U+0x188 | Direction byte written from input X-currentX sign |
| U+0x18c..+0x1a7 | Destination sound-mode record;word18c is playback request input |
| U+0x1a8/+0x1ac/+0x1b0 | Per-step baseline delta X/Y/Z |
| U+0x1b4/+0x1b8/+0x1bc | Angle theta / angle step omega / last displayed Y |
| U+0x1c4 | Start of source sound-mode data |

Some fields serve different roles in other callbacks;these are the uses of
this body. A semantic port must retain independent visible/baseline/target values
and must not substitute packed offsets from a body-part record.

## First-call dispatch and setup

With firstCall!=0,write event phase1. If M==1,replace it with phase2 and jump
directly to the phase switch:skip target/baseline/distance/count/curve/sound
initialization. Otherwise execute the complete normal setup:

1. SetMoveTargetPos(U,X,Y,Z). Its per-axis -250000000 sentinel retains old
   targets. Then GetPos into three temporaries and SetMoveCurrentPos from them.
2. Request angleABf(currentX,currentZ,inputX,inputZ),store heading17c;
   distABf with the same arguments,store distance180. These use the **resolved
   input coordinates**,not targets after sentinel retention.
3. Compare binary32(inputX-currentX) with0:positive writes direction+1,negative
   writes-1,equality/unordered retains old byte188. Store supplied N at16c.
4. If n==0,derive n by fctiwz(binary32(distance/speed)). Otherwise recompute
   speed=single(distance/float(n)). Compute fused residual distance-n*speed.
5. If resulting n==0,return event2 immediately. Target/baseline/heading/distance/
   direction/count stores above remain;no curve,sound or integration follows.
6. Compute deltas=(target-baseline)/float(n),reset Phi to positive0,set
   G=binary32(pi)/float(n),and adjust speed by residual/float(n). Initialize
   angle90,mode-dependent step,height and first Phi increment as specified in
   the [numeric document](BATTLE_MARIO_JUMP_NUMERICS.md).
7. LoadSeMode(type2,resolved S,U+1c4,U+18c). If destination word18c is nonzero,
   request psndSFXOn_3D(word18c,&copiedBaselinePosition). The temporary vector
   uses current baseline X/Y/Z;return is ignored. No local voice-handle store,
   stop-on-landing or per-integration position update for that voice occurs.
8. Fall through to the phase switch **on the same invocation**.

No negative-count clamp or automatic fallback for nonzero counts exists.
The zero-count branch can complete2 without moving visible position. For normal
positive counts,the initialization invocation also performs the first step.

## Phase1: baseline plus sine flight

For each phase1 invocation:

1. Add stored delta to baseline X,then Y,then Z with separate single operations.
2. GetPos(U,&x,&y,&z);store its current visible Y to lastY1bc. Replace temporary
   X with baselineX;compute visibleY=singleFMA(H,single(sin(Phi)),baselineY);
   temporary Z=baselineZ. SetPos(U,x,visibleY,z).
3. If ordered targetY>computed visibleY **and pre-decrement n<3**,replace only
   temporary Y with targetY and SetPos again. Equal Y does not enter this clamp;
   no floor query or X/Z correction occurs. Store the resulting temporary Y
   into lastY1bc (overwriting the earlier visible-Y snapshot).
4. Advance Phi using current mode M and stored G/theta,then theta=reviseAngle(
   single(theta+omega)). Decrement n with signed32 word wrap.
5. If n!=0,return0 (blocked). The test is exact zero,not n<=0. For negative
   counts,no local immediate completion or positivity repair occurs.
6. If n==0,SetPos again using the latest temporary X/Z and targetY. Set
   Phi=negative(single(lastY-lastY)),set G=original binary32 1.7999999523162842,
   and return1 (advance plus yield).

The final self-subtraction is retained as an operation;finite lastY produces
negative zero,but NaN/infinite lastY must not be replaced by an unconditional0.
Phase byte remains1. The caller/interpreter must advance after result1;polling
this same completed callback again would decrement zero to-1 and is a different
execution. Final X/Z are interpolated baseline values,not snapped to target.

## Phase2/3: mode1 eligibility and retained baseline

Phase2 requests BtlUnit_GetACPossibility(U). That helper's local predicate is
in [status/move services](BATTLE_MOVE_COUNTS.md);this is **eligibility**,not an
input-history or action-command result check. If it returns0,return event2 with
no local motion setup. If nonzero:

- Phi=negative(single(lastY-lastY)),G=1.7999999523162842,n=1,theta=90;
  omega=float(signed integer360/n),therefore360 for the stored1.
- H=single(double(abs(single(lastY-lastY)))/16.5).
- GetPos(U,&temporaries),set event phase3,return0. The returned position
  temporaries are **not copied** into stored baseline154/158/15c.

Finite lastY produces signed-zero phase and zero height here;retain the original
operations rather than inventing a positive jump/height difference. Baseline and
other setup fields are retained from earlier owners because mode1 skipped setup.

Phase3 performs a separate pair of sin calls with the same current angle:
baselineX=single(double(baselineX)+double(single(H*sin(angleRadians)))/33);
baselineY=singleFusedSubtract(H,single(sin(angleRadians)),baselineY).
Then advance/normalize theta,SetPos from the stored baseline XYZ,decrement n,
and return1 exactly when n==0,otherwise0. There is no target snap,landing G/Phi
reset or other phase1 terminal cleanup in this phase3 route. Unexpected signed
phase values (outside1/2/3) return0 without integrating.

## Actual ordinary Jump composition and AC ownership

The [ordinary graph](BATTLE_NORMAL_JUMP_EVENT_FLOW.md) uses M0 for the initial
pass and M3 for subsequent passes,both S=-1,with count produced by
[MarioJumpParam](BATTLE_MARIO_JUMP_PARAMETER.md). Both use phase1,but their angle
steps/phase coefficient differ. M1 eligibility motion is a different local route,
not a recovered ordinary Jump or Crump branch.

The motion body does **not** directly store shared AC result1cb8,count1cb4,
state1c9c or options1f50;it calls neither ResultAC nor a battle pad history
checker. Shared work is used to fetch U. Its event phase byte is not AC main
state. Numeric/audio callees retain their own effects as dependencies.

For valid positive n,the body integrates once in each invoked phase1 call,
including first call,and returns1 at exact countdown zero. The script then
requests pose/window/WAIT(W/2)/ResultAC. That evaluator comes after flight
completion and a separate script wait,not an invented evaluation inside this
motion callback. Multiple interpreter credits/main invocations can share input
samples;[pad cadence](BATTLE_INPUT_CADENCE_AND_SNAPSHOTS.md) and scheduling
must be composed before deriving actual original time.

## Implementation requirements and outstanding evidence

| ID | Requirement | Derived acceptance cases |
| --- | --- | --- |
| MJP-MOTION-01 | Preserve seven integer-resolved arguments and first-call state | Resumed calls still resolve all arguments;M1 skips normal setup;zero derived count returns2 after earlier stores |
| MJP-MOTION-02 | Keep target/baseline/visible/deltas and curve fields independent | Input sentinel retains target but raw X/Z still feed distance/heading;speed is not per-step baseline delta |
| MJP-MOTION-03 | Integrate on first call and preserve exact countdown/landing order | Positive N steps includes first;last n0 returns1;targetY clamp requires pre-decrement n<3 and strict greater;final X/Z unsnapped |
| MJP-MOTION-04 | Keep M0/M3 rounded numerical routes distinct | N42 uses floating360/N for M0 versus integer360/N for M3;different coefficients reach phase updates |
| MJP-MOTION-05 | Keep phase2 eligibility and phase3 local state separate | Ineligible mode1 returns2;eligible setup returns0 then phase3 n1 returns1;GetPos does not replace retained baseline |
| MJP-MOTION-06 | Preserve sound/service/AC boundaries | One init playback request only when record18c nonzero;no direct AC result/count store;timing evaluation remains later script request |

tools/verify_mario_jump_motion.py pins the full body,76 instruction anchors,
original20 constants,two concrete mode/count/sound caller records and related
helper identities. See numerical/audio documents for which helper bodies are
specified versus fingerprint-only dependencies. Unit lookup/translation/domain,
angle/distance/sin/fmod/FPSCR precision,source sound writers/macro playback,
movement initialization and original flight/input/state traces remain open.
This closes the local motion body,not a complete tutorial/game. Read-only ISO
inspection outputs remain under /tmp or ignored artifacts;no native gameplay
implementation,asset conversion or story text rewrite is implied.
