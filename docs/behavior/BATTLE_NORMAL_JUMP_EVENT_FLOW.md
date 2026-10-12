# Ordinary Mario Jump: complete request graph and per-hit ownership

US G8ME01 disc0/revision0;recovered2026-10-12 by documentation agent ID2.
Evidence B:all268 commands in marioAttackEvent_NormalJump at DOL
0x80355cf0..0x80356bfb,size0xf0c,SHA256
`2858f3f63cfb95dc84eb6ed3b363c530e24200063044f37afd275d3847f9720c`.
The [ordinary weapon](BATTLE_WEAPON_POWER_CALLBACKS.md) points to this stream.
This documents every request block and branch;callback internals listed as
dependencies and original execution are **not** closed by the stream hash.

All addresses are US DOL virtual addresses. L[n] is raw operand0xfe363c80+n;
owner selector is signed-2. L3/L4 retain target unit/part,L12 the weapon,L10
the script's loop count,L5 the precheck/damage output,and L6/L7 are reused
results/counts/motion temporaries. L0..2 are reused position/window temporaries.
Do not merge script L10 with shared AC count1cb4 or infer each use's meaning
from its variable number alone. Branch VM service rules are documented in
[Jump VM/concurrency](EVENT_JUMP_BRANCHES_AND_CONCURRENCY.md).

## Entry, target selection and ordinary versus first-attack preparation

Initial request0x80355cf0 is JumpSetting(-2,20,raw0xf24a7a80,
raw0xf24a7d4c). The documented literal float route yields speed0 and
acceleration0.69921875;these settings are not an elapsed20-frame attack claim.
Then check battle mask2 into L0 at0x80355d08:

| Mask2 branch | Exact requests before convergence at0x80355e80 |
| --- | --- |
| Nonzero | GetFirstAttackTarget(L3,L4);if L3==-1,GOTO99. Set L12=ordinary weapon0x803538a8;ACHelpSet(that weapon);camera mode(0,10),homing(target L3/L4,owner -2/part1),zoom250,speed level2 |
| Zero | GetSelectEnemy(L3,L4);if L3==-1,GOTO99. CommandGetWeaponAddress(-2,L12);AttackDeclare(-2,L3,L4);WaitGuardMove();PayWeaponCost(-2,L12);RunDataEventChild(-2,7);child0x80363388 (Ma jinai power-check annotation);camera mode(0,11),homing(owner -2/part1,target L3/L4),speed level1,zoom250 |

The branches occupy0x80355d18..0x80355e7c. Missing target goes straight to
label99,skipping cost,AC setup and return movement. Unlike the boss attack's
missing label99,this stream really contains label99 at0x80356bcc.
[First-attack target helper](BATTLE_TARGET_EVENT_INTERFACES.md) has its own
nonnull assumptions;the script's unitID check does not repair helper failures.

At0x80355e80 recheck battle mask2. If zero,GetWeaponActionLv(L12,L0),
AcSetDifficulty(-2,L0),SetupAC(-2,1,raw1,0),then converge. No local AcSetParamAll
or AcSetFlag request appears anywhere in these268 commands. Do not manufacture
parameter/flag reset as part of ordinary Jump. Setup's raw option1 is distinct
from flags1cc4/options1f50. Weapon action-level and difficulty callbacks have
their own consumers;initial tutorial/first-attack mask producers remain open.

## Approach and delayed first-attack AC setup

Recheck mask2 at0x80355ed8. Nonzero takes0x80355ef4..0x80356054:

1. GetHomePos(-2,L0..2),FaceDirectionSub(-2,L0,250),SetPos(-2,L0..2),
   WAIT_MSEC(1000).
2. SetMoveSpeed(-2,raw0xf24a8e80=5),AnimeChangePoseType(-2,1,42),
   GetTakeoffPosition(-2,L3,L4,L0..2),CalculateFaceDirection(-2,-1,L0,L2,1,L6),
   ChangeFaceDirection(-2,L6),GetMoveFrame(-2,L0..2,0,L7).
3. If signed L7>22,SUB(L7,22). Else SUB(L7,22),MUL(L7,-1).
   Thus the valid nonoverflow case forms **abs(moveFrame-22)**,not max(frame-22,0).
4. Create attached brother block0x8035600c..0x80356050. Its copied locals wait
   L7,then GetWeaponActionLv(L12,L5),AcSetDifficulty(-2,L5),SetupAC(-2,1,raw1,0).
5. Parent requests MovePosition(-2,L0,L1,L2,0,-1,0) at0x80356054.

The zero-mask alternative0x8035607c..0x803561ac requests speed
raw0xf24a8680=3,pose type42,takeoff position,face calculation/change,then creates
brother0x803560f0..0x803561a8 and requests the same parent MovePosition.
That brother's first switch maps weapon pointers0x803546e8->L13=4,
0x803547a8->3,0x80354868->1. Its second OR-case group for those same pointers
requests _jump_star_effect(-2,L13),GetPos(-2,L0..2),and owner sound
`SFX_BTL_MARIO_HAMMER_SHINE1`. Other weapon pointers produce no requests in
this brother. Do not generalize this effect to every ordinary Jump.

All listed owner SE requests carry the remaining raw operands
0xf1194d80,0,0xf1194d80. Preserve resolver/sentinel policy instead of inventing
volume or pitch values. Brothers copy locals and share referenced unit/work
services;their local changes do not overwrite the parent's L0..L13.

After convergence,CalculateFaceDirection(-2,-1,L3,L4,16,L15),
ChangeFaceDirection(-2,L15),StartAC(1) at0x80356204,SET(L10,0),label10.
There is **one StartAC site**,outside the hit loop. First-attack setup is
concurrent delayed work;which main/event invocation reaches it first requires
movement and scheduler composition,not a fixed native tick assumption.

## Per-pass precheck and motion request branches

At label10=0x8035621c:

| Script predicate | PreCheckDamage request |
| --- | --- |
| Signed L10>=1 | (-2,L3,L4,L12,0x100100,L5),site0x80356230 |
| Otherwise | Same actor/target/weapon,flags0x100,site0x80356254 |

SWITCH L5 at0x80356278 groups2/3/4/6 with CASE_OR/CASE_END and accepts case1;
all these listed values skip the default WAIT_MSEC(1000). Every other precheck
value executes that wait before END_SWITCH. All routes then GetHitPos(L3,L4,
L0..2),ADD(L2,5) and request pose`M_J_1B`. Preserve the actual VM ADD request;
position encoding and callback conversion determine its numerical effect.

Signed L10>=1 requests L6=36,MarioJumpParam(-2,L0..2,L6),then creates brother
0x8035632c..0x80356430 and parent MarioJumpPosition(-2,L0,L1,L2,L6,3,-1)
at0x80356434. Otherwise it requests owner SE`SFX_VOICE_MARIO_BATTLE_JUMP1`,
L6=36,the same MarioJumpParam,and MarioJumpPosition(...,L6,0,-1) at0x803564a0.
The two MarioJumpPosition modes **3 and0 are distinct**. The now recovered
[Mario motion](BATTLE_MARIO_JUMP_MOTION.md) and [numerics](BATTLE_MARIO_JUMP_NUMERICS.md)
specify this larger callback0x801093a8,which differs from the generic
[JumpPosition contract](BATTLE_JUMP_EVENT_CALLBACKS.md). Do not substitute it
or infer impact36.

The subsequent-pass brother waits3,computes L0=L6/2-3,L1=L0-7,L2=L0+5,
then L0=L6-4,and requests ACRStart(-2,L1,L2,L0,0),ACRGetResult(L6,L7).
If [MarioJumpParam](BATTLE_MARIO_JUMP_PARAMETER.md) leaves L6=36,the copied
count derives request operands8,20,32. Distance correction can increase it;
preserve the updated count in parent motion and copied stylish formulas.
CASE>=2 of returned
L6 requests pose`M_A_5`,AudienceDeclareAcrobatResult(L12,1,-60,-25,0),
snd_voice(-2,1);default requests evt_audience_acrobat_notry. The brother's
result L6 does not change parent motion L6. Stylish callback timing/result
and audience service internals remain dependencies.

After the parent motion callback,if L5!=1:

- Result3 requests StartAvoid(L3,38);6 requests39;2 requests40.
- Result4 and other values request none of those three avoid services.
- All non1 values request StopAC,JumpContinue(-2),owner SE
  `SFX_VOICE_MARIO_LAND_DAMAGE1`,pose`M_D_2`,WAIT_FRAME(40),then GOTO95.

Thus avoidance skips ResultAC,damage,feedback declaration and weapon-aftereffect
in this stream's later hit paths. It still reaches label95 return movement and
label99 badge/wait tail. Other hit-event or avoidance services may have effects
of their own;preserve request order and do not collapse2/3/6 into one policy.

## Timing evaluation and three distinct damage requests

For precheck1,request pose`M_J_1D` at0x8035658c,timing_flag_onoff(1,raw1),
timing_get_success_frame(L0),DIV(L0,2),WAIT_FRAME(L0),timing_flag_onoff(0,raw1),
ResultAC(),GetResultAC(L6). These are ordered parent operations after the motion
callback. Window W comes from current difficulty/count/options;the wait is
signed integer truncation W/2,not an added fixed four-frame rule. ResultAC may
block and reevaluate;see [result/stop services](BATTLE_JUMP_RESULT_AND_STOP_SERVICES.md).

IF_NOT_FLAG(L6,raw2) at0x803565f4 selects **timing failure**:

1. CheckDamage(-2,L3,L4,L12,0x100,L5),site0x80356600.
2. If L10==0,AudienceDeclareACResult(L12,-1);on later-pass failure no such
   declaration occurs here.
3. Camera mode0,StopAC,evt_audience_ap_recovery,InviteApInfoReport,
   WeaponAftereffect(L12),pose`M_J_1B`,fall acceleration0xf24a7d4c.
4. GetPos(-2,L0..2),GetHomePos(-2,sentinel,L1,sentinel),FaceDirectionSub(-2,L0,27),
   JumpPosition(...,20,-1);fall acceleration0xf24a7ce6,
   FaceDirectionSub(...,20),JumpPosition(...,12,-1),FaceDirectionSub(...,8),
   JumpPosition(...,8,-1);GOTO95.

Result containing mask2 selects **timing success**:
snd_voice(-2,2),GetResultCountAC(L6),then compare signed L6<=1.

| Count branch | Exact hit/continuation order |
| --- | --- |
| <=1 | Create independent inline camera block0x803567a8..0x803567f4;CheckDamage flags0x20000 at0x803567f8;GetHitPos(L3,L4,L0..2);GetResultPrizeLv(L3,0,L6);ACSuccessEffect(L6,L0..2);AudienceDeclareACResult(L12,L6);WAIT_FRAME(2);ADD(L10,1);GOTO10 |
| >1 | Camera mode0;CheckDamage flags0x20100 at0x803568a0;WAIT_FRAME(2);converge on label90 |

The camera inline requests speed level0/zoom200,wait10,then speed level1/zoom250.
It is not an extra10-frame parent wait or an attached stylish brother.
The count reader returns stored word1cb4;it does not itself increment it.
Successful [TimingA result](BATTLE_TIMING_A_COMMAND.md) increments that word.
Setup resets it to0,whereas the loop has **no new SetupAC/StartAC call**.
Neither damage destination L5 nor prize/effect result is tested before the
<=1 branch loops. No GetHp/GetMaxHp or explicit target-alive gate appears in
this stream;the next pass reuses L3/L4/L12 and executes its precheck again.
Do not invent unconditional guaranteed second damage:precheck can divert it.

For normal setup count0,one successful evaluation yields count1 and the repeat
request flags0x20000;another success yields count2 and final flags0x20100.
This is a **derived composition case**,not a measured original two-hit trace.
Other initial counts,repeated result polling and forced paths need their owners.
Failure is a real0x100 damage request,not simply no damage. Preserve per-hit
calculator state and flags rather than doubling one power/HP update.

## Final success recovery, return and common tail

Label90 at0x803568cc requests camera mode0,StopAC,WeaponAftereffect(L12),
pose`M_J_1B`,fall acceleration0xf24a7d4c,GetPos(-2,L0..2),GetHomePos with only
L1 destination active,FaceDirectionSub(-2,L0,60),SET(L6,32).
It creates attached brother0x8035696c..0x80356a70,using the same stylish
arithmetic/case/output requests as above;copied32 derives ACRStart operands
6,18,28 after WAIT3. Then parent JumpPosition(-2,L0,L1,L2,L6,-1),pose`M_J_1B`,
evt_audience_ap_recovery,InviteApInfoReport,fall acceleration0xf24a7ce6,
FaceDirectionSub(...,20)/JumpPosition(...,12,-1),then direction8/jump8.
The acceleration literals decode to0.69921875 and0.599609375 respectively;
retain the quantized values,not rounded0.7/0.6.

Label95=0x80356b30 is shared by avoidance,failure bounce and final success.
Request camera mode0,speed level1,pose`M_W_1`,move speed0xf24a9280=6,
pose type40,GetHomePos(-2,L0..2),MovePosition(-2,L0,L1,L2,0,-1,0).

Label99=0x80356bcc requests ResetFaceDirection(-2),child
btldefaultevt_SuitoruBadgeEffect0x8034f2e4,StartWaitEvent(-2),explicit event
delete opcode2,then terminal opcode1. There is no earlier return/reward/victory
setter in this stream. Child completion and local copy-back must follow the
[event runtime](EVENT_RUNTIME.md);an invented immediate after-attack return
would omit these original requests. Independent inline camera work has different
ownership from the four attached brothers.

## Implementation requirements and evidence gaps

| ID | Requirement | Derived acceptance cases |
| --- | --- | --- |
| JUMP-EVT-01 | Preserve target/cost/setup branches and current weapon | First-attack selects ordinary pointer with delayed setup;ordinary branch uses selected weapon;missing-target jumps to real99 without costs/setup/return movement |
| JUMP-EVT-02 | Preserve shared AC count and script-loop counter separately | Normal count0,success/success requests20000 then20100;success/failure requests20000 then100;one Start outside loop,no setup reset between passes |
| JUMP-EVT-03 | Keep precheck,motion,window wait,evaluator,damage order | Next-pass precheck uses100100;avoid codes3/6/2 request38/39/40 and bypass evaluator/damage;default precheck waits1000msec |
| JUMP-EVT-04 | Preserve brother versus independent inline ownership and copied locals | Four attached brothers do not overwrite parent temporaries;camera wait10 does not block parent;owner deletion cancels attached work only |
| JUMP-EVT-05 | Preserve feedback,aftereffect/AP and common cleanup branches | Failure declares-1 only at L10==0;repeat success declares computed grade;final success does not duplicate it;label99 always child badge then wait-event request |

tools/verify_jump_event_flow.py pins the original stream,268-command/label/
concurrency census,concrete precheck/damage/setup/result/loop records,and now
19 service/VM/distance fingerprints plus57 instruction anchors after parameter
recovery. The constructor fingerprint specifies
only the used ownership region;it does not close full allocation/label setup.
Reproduce a fresh annotated inventory with tools/dol_event_inventory.py using
marioAttackEvent_NormalJump and the US symbol index. Artifacts stay ignored or
under /tmp;ISO reads are read-only.

This answers JUMP-01's **complete request-graph** portion. The
[MarioJumpParam contract](BATTLE_MARIO_JUMP_PARAMETER.md) now closes parameter
preparation and [MarioJumpPosition](BATTLE_MARIO_JUMP_MOTION.md) now closes
the local flight. [Takeoff](BATTLE_JUMP_TAKEOFF_POSITION.md) and
[MoveFrame/MovePosition](BATTLE_MOVE_FRAME_AND_POSITION.md) now close those
local preparation bodies;remaining:their candidate/math/audio/domain services,
exact floating/event
arithmetic and progress at motion completion;GetSelectEnemy/cost/data-child/
power-check/stylish/prize-byte/aftereffect/badge-tail service bodies;AC result/option
writers and original tutorial weapon/menu gates. Original input/state/RNG/hit/
animation/audio traces must validate branch feasibility and actual cadence.
The reused original data and recovered graph cannot justify the harness's
impact48/return42 schedule or make the complete game/encounter finished.


## Stylish service follow-up (2026-10-12)

[Timing/results](BATTLE_STYLISH_TIMING_AND_RESULTS.md) and
[icon requests](BATTLE_STYLISH_TIMING_ICON.md) now close the local ACRStart/
ACRGetResult/BattleAcrobat obligations above. Boundaries use a separate shared
invocation counter; first start polls t1, lower is excluded, upper included,
and both ordinary callers supply history count0. Original event scheduling,
source writers,audience feedback and rendered/runtime traces remain open.
