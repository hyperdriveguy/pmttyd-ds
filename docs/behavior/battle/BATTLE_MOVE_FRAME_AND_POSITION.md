# GetMoveFrame versus MovePosition: count,velocity and event completion

US G8ME01 revision0;recovered2026-10-12 by documentation agent ID2.
Evidence B:complete btlevtcmd_GetMoveFrame8010c8c0/1b4 and
btlevtcmd_MovePosition8010ca74/334. These prepare/return the
[ordinary Jump](BATTLE_NORMAL_JUMP_EVENT_FLOW.md). Dist/angle/sin/cos numerical
execution and original motion/input traces remain explicit dependencies.

## Query argument/read policy

GetMoveFrame resolves actor operand0 via evtGetValue/BattleTransID,operand1
X via evtGetValue and signed32->single conversion,operand2 Y via evtGetValue
but discards its returned value,operand3 Z with signed integer conversion,
operand4 speed via **evtGetFloat**,then captures output operand5 raw.
Fetch U using current shared work,GetPos into separate temporaries,and
distABf(currentX,currentZ,X,Z). No first-call gate or unit field mutation occurs.
Return2 after the output policy below.

If resolved speed compares equal to single0,load U170. If that also equals0,
use original single1 at8042228c. This equality includes both signed zeros;
unordered/NaN speed bypasses fallback. The substituted speed is a query-local
value and is **not stored** into the unit.

Let D be the distance return and S the chosen speed. Compute
n=fctiwz(single(D/S));R=singleFusedSubtract(float(n),S,D).
If ordered R>0,increment n with32-bit wrap. If n==0 afterward,set n=1.
This is a ceiling-like estimate for positive ordinary inputs,not an unconditional
ceil/max1 formula for negative/exceptional values. Negative nonzero counts are
retained;there is no n<0 clamp.

If **raw output operand5** equals0xf1194d80,omit its store;otherwise write n
through evtSetValue. This differs from MarioJumpParam,whose sentinel test is on
the computed count rather than its raw destination. Current Y is not a query
distance input,even though GetPos read it.

## MovePosition arguments and first-call setup

MovePosition resolves/translates actor,then X/Y/Z through evtGetValue and
signed32->single conversion;resolve count operand4,sound-mode operand5,
sound-type operand6 with evtGetValue. Every resumed invocation repeats that
argument resolution and current-work unit lookup. No evtGetFloat,local null/
range validation or implicit speed fallback occurs in this motion body.

On firstCall!=0:

1. Set event byte0xd=1 and store resolved count at U16c.
2. GetPos(U),SetMoveCurrentPos from those coordinates,then SetMoveTargetPos
   from resolved X/Y/Z. Both setters have the established per-axis float
   -250000000 sentinel rule;do not force sentinel axes to current position.
3. angleABf(currentX,currentZ,inputX,inputZ)->U17c;
   distABf with those same raw input coordinates->U180.
4. Compare single(inputX-currentX) to0:write U188=+1 or-1 only for ordered
   positive/negative;retain its previous byte for equality/unordered.
5. If count==0,derive n=fctiwz(single(D/U170)),store it,and replace **exact0**
   with1. Recompute fused residual R=D-float(n)*oldSpeed and store
   speed=single(oldSpeed+single(R/float(n))). If supplied count was nonzero,
   instead store speed=single(D/float(n)) directly. No query fallback-to1 speed
   or ceiling-count increment is used.
6. LoadSeMode(resolvedSoundType,resolvedSoundMode,U+1c4,U+18c).
7. Fall through to integration on the same callback invocation.

The command byte is not consulted as a mode switch afterward. Stored target/
current/heading/count/sound fields retain their own lifetimes;this body does
not share MarioJumpPosition's sine/delta fields or exact-zero terminal policy.

## Every-call horizontal integration and terminal snap

Request btlMovePos(&U154,&U15c,U170,U17c),which advances horizontal baseline
X/Z through the [original sin/cos helper](BATTLE_JUMP_EVENT_CALLBACKS.md).
Baseline Y158 stays unchanged on this path. SetPos from baselineXYZ,decrement
U16c with word arithmetic,then compare the reloaded count as signed:

- If n>0,call _UnitMoveSoundControl(U) and return0 (blocked).
- If n<=0,SetPos(U,target160/164/168),call _UnitMoveSoundControlEnd(U),and
  return1 (advance+yield).

The final snap sets **all three visible target axes**,while stored baseline
remains the pre-snap horizontal integration/Y value. No floor/terrain collision,
per-step Y interpolation,HP/precheck or AC evaluator request is present in this
body. Do not introduce such features into the recovered approach service.
The negative-count case still performs one step before snapping/returning1;
it does not get rejected before movement.

Movement sound control is requested only after a step leaves positive count.
An automatic one-step move can load the record and terminate without any normal
per-step sound trigger. [Sound core/stop](BATTLE_MOVEMENT_SOUND_CONTROL.md) gives
the exact record/voice request rules;do not play a footstep unconditionally on
initialization. Resolved sound type/mode only feed the first-call loader.

## Concrete Jump callers and timing distinction

First-attack GetMoveFrame at80355fb4 supplies(-2,L0,L1,L2,0,L7),so the query
uses unit speed or local fallback1. The script converts the query count to
abs(n-22) for delayed brother AC setup. First-attack/ordinary approach sites
80356054/803561ac and return80356ba8 supply
MovePosition(-2,L0,L1,L2,0,-1,0):automatic movement count,type0/default sound.

For representative exact D=101 and unit speed5,the query derives21 via positive
residual;MovePosition derives20 and adjusts speed toward5.05. These are local
derived cases,not original executed scene/tick measurements. D0/speed5 yields
query1 and move count1 with adjusted speed0. If unit speed0,the query's local
fallback does not repair MovePosition's division by the stored zero speed;
exceptional conversion/valid caller constraints remain dependencies.

Therefore the query result cannot be treated as a guaranteed count of motion
callback invocations,or used to erase the script's abs(n-22) rule. First-call
integration,brother WAIT_FRAME/event scheduling and pad sampling must be composed
from the original contracts. A fixed native approach/impact schedule is not
established by these two bodies.

## Implementation requirements

| ID | Requirement | Derived acceptance cases |
| --- | --- | --- |
| MOVE-01 | Preserve query versus movement argument/precision rules | Query ignores resolved Y and reads float speed;movement uses signed integer XYZ and no speed fallback |
| MOVE-02 | Keep count policies and unit mutation separate | D101/S5 query21 versus motion20 with adjusted speed;query does not store unit speed;negative counts are not clamped |
| MOVE-03 | Preserve first-call step,blocked sound and full snap/yield | N1 integrates then snaps XYZ/returns1 without normal sound trigger;N2 has one blocked sound call;baselineY stays fixed |
| MOVE-04 | Bind actual Jump raw sound/automatic/count-delay operands | Type0/-1/automatic calls remain distinct from Mario jump type2;query output feeds abs(n-22),not a fabricated fixed22 delay |

tools/verify_jump_preparation.py pins both complete bodies,arithmetic/argument/
return sites,constants and all six ordinary takeoff/query/move caller records.
Original dist/angle/sin/cos/FPSCR,stored-speed/domain/target/sound source writers,
scheduler/pause and movement/input/audio traces remain open. ISO is read-only;
original inspection/data artifacts are not committed.
