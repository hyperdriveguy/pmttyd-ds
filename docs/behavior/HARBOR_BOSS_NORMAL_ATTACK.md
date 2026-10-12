# Opening Crump normal attack: target, hit dispatch and presentation requests

US G8ME01 revision 0, recovered 2026-10-12. Evidence B: the complete annotated
normal_attack_event@unit_boss_kanbu1.o span in shipped rel/gor.rel, file offsets
0x653c8..0x6597b, size 0x5b4, 95 commands. SHA-256:
`1ac3f7e075a9f37d216d9d2504590e0fd2bf071533d55f059b88cb36d0adceba`.
Addresses below are **REL file offsets**, not runtime virtual addresses.

This document specifies the script's requests and branch graph. It does not
claim complete movement/camera/pose/audio callback semantics, measured hit ticks
or original gameplay execution. Reusing assets does not establish their playback
cadence. The enclosing [tutorial events](HARBOR_BOSS_TUTORIAL_EVENTS.md) install
the attack hook; the hook first tests item-use behavior and otherwise supplies
weapon_kanbu1_attack at file 0x64718 as local 9 to this child stream.

## Operand and target ownership

Local n's raw operand is 0xfe363c80+n. The stream uses local 3 for target unit,
4 for target part, 5 for the precheck/damage result, 9 for the inherited weapon,
and 0..2 as temporary side/result/position outputs. Owner selector is signed -2.
Do not replace the weapon operand with an immediate fabricated power.

| Offset | Request/branch |
| --- | --- |
| 0x653c8 | check_battleflag(local0,2) |
| 0x653d8..0x653e4 | If local0 != 0, GetFirstAttackTarget(local3,local4) |
| 0x653f8 | Else GetEnemyBelong(-2,local0) |
| 0x65408 | SamplingEnemy(-2,local0,local9) |
| 0x6541c | ChoiceSamplingEnemy(local9,local3,local4) |
| 0x65434 | If target unit local3 == -1, enter missing-target branch |

The [target interfaces](BATTLE_TARGET_EVENT_INTERFACES.md) establish these
callbacks' argument resolution, output order and byte widths. Full sampling
policy and the initial battle mask-2 state remain open. First-attack helper
selects from mode-dependent pointer pairs; it cannot be assumed identical to
normal candidate sampling.

Missing target: request CheckToken(-2,0x10,local0) at 0x65440. Nonzero result
runs subsetevt_confuse_flustered as a child at 0x65460, then explicit event-delete
opcode 2 at 0x65468. If that token test is zero, the stream executes GOTO(99)
at 0x65470. **There is no label 99 anywhere in this 95-command span.** Its only
labels are 90, 91 and 98. Do not silently invent a successful no-target return.

Relevant original GOTO code in evtmgrCmd at 0x80031764..0x800317c8 resolves
the label argument, then scans 16 signed-byte label slots at event+0x1c and reads
a matched target from event+0x2c+4*i. For ordinary label 99, no match leaves its
working pointer at event+0x18's argument pointer, which is stored as next-command
pointer at event+0x14. This is not an implicit event deletion. Whether a caller's
label table or an invariant makes this path safe/unreachable is unresolved;
label registration and actual no-target execution need investigation. Pointer
GOTO's separate encoded-value route is outside this concrete label branch.

## Approach and jump request order

The target-present route requests the following in parent-event order:

| Offset | Request |
| --- | --- |
| 0x6547c | Camera 0 mode 8 |
| 0x6548c | Camera 0 homing owner -2 and target local3 |
| 0x654a0 / 0x654b0 | Camera movement-speed level 2; zoom 200 |
| 0x654c0 | Owner part 1 pose `R_2` |
| 0x654d4 | GetHitPos(target unit,part,local0,local1,local2) |
| 0x654f0 | FaceDirectionSub(-2,local0,60) |
| 0x65504 | SetMoveSpeed(-2, raw encoded float 0xf24a8a80) |
| 0x65514 | MovePosition(-2,local0,0,local2,0,-1,1) |
| 0x65538 | Owner sound `SFX_BOSS_KANBU_JUMP1` |
| 0x65554..0x65588 | Inline event: pose `A_1A`, WAIT_FRAME(10), pose `A_1B` |
| 0x6558c | Re-read target hit position into local0..2 |
| 0x655a8 | JumpSetting(-2,0,raw 0xf24a8280,raw 0xf24a7b4c) |
| 0x655c0 | JumpPosition(-2,local0,local1,local2,0,-1) |
| 0x655e0 | PreCheckDamage(-2,local3,local4,local9,0,local5) |

Opcode 110 begins the unnamed inline event and 112 ends its source block.
Its pose/wait operations are concurrent event work, not automatically an extra
10-frame parent delay before jumping. Parent order means a later request is
reached after the event dispatcher advances past the preceding callback;
those callbacks' polling/completion semantics still determine elapsed ticks.

Encoded float operands above are retained as raw event words. Their numeric
values require the original evtGetFloat mapping and callback argument policy;
they are not direct IEEE binary32 bit patterns to reinterpret blindly. Likewise
literal JumpPosition arguments 0 and -1 have not been renamed as a fixed duration
or landing signal. Recover the callbacks before calculating an impact schedule.

## Precheck branch map and defense boundary

PreCheckDamage import at file 0x655e4 binds DOL callback 0x8010abac. It writes
local 5, which SWITCH at 0x65600 snapshots. The original branch requests are:

| Precheck result | Requests before common recovery |
| --- | --- |
| 4 | CASE_OR at 0x65608; GOTO(90) |
| 3 | StartAvoid(local3,38) at 0x65624; GOTO(90) |
| 6 | StartAvoid(local3,39) at 0x65648; GOTO(90) |
| 2 | StartAvoid(local3,40) at 0x65668; GOTO(90) |
| 1 | GOTO(91); defense and damage requests below |
| Other | Default at 0x65694; GOTO(98) |

Label 90 at 0x656a4 immediately GOTO(98). Label 91 at 0x656b4 executes:

1. ResultACDefence(local3,local9), request 0x656bc, import target 0x80107694.
2. CheckDamage(-2,local3,local4,local9,0x100,local5), request 0x656cc,
   import target 0x8010a78c.
3. GOTO(98) at 0x656ec.

Thus this stream requests defense exactly on precheck result **1**, before
CheckDamage. Local 5 is reused as the damage result destination, but this stream
has no subsequent result-specific branch on it. Recovery is shared by successful,
avoided and default outcomes. The [hit precheck](BATTLE_HIT_PRECHECK.md) documents
the underlying local returns, including conditional RNG and null-target behavior;
do not confuse these codes with defense results or damage results 18/19.

Original CASE_OR and CASE_END routing regions were inspected at 0x80033b64 and
0x800337ec. CASE_OR supports its existing matched-state -1 and sets that state
on equality; CASE_END clears/skip-routes a matched group or searches for the next
case when unmatched. These markers are not evidence of C-style fallthrough
into every later body. The exact script has **no END_SWITCH opcode 49**; its
selected routes jump directly to labels 90/91/98 and ultimately delete the event.
Preserve that control flow rather than inserting a fabricated switch-pop into
an emulator specification. The broader nesting/state lifetime is still a VM
integration dependency; [integer branches](EVENT_INTEGER_BRANCHES.md) documents
previously recovered local rules.

The enemy weapon's byte +0x13 is 1, satisfying the recovered local B-defense
weapon-byte gate. Its power callback is weaponGetPowerDefault at 0x80189d30.
Neither fact proves global superguard availability, actual damage value, status/
badge overrides or tutorial input policy. Mask 0x100 passed to CheckDamage is
separate from the init's battle flag 0x1000. See [defense](BATTLE_INPUT_AND_DEFENCE.md)
and [feedback/stage flag](BATTLE_ACTION_FEEDBACK_AND_STAGE_GATE.md).

## Common recovery and sound/pose sequence

Label 98 starts at 0x656f4. The following requests occur on every route reaching
it, including evasion. Sound callback operands after owner/key are always raw
0xf1194d80, 0, 0xf1194d80 at the listed sites; retain that resolution policy
rather than interpreting both sentinel words as literal audio parameters.

| Offset | Parent or inline request |
| --- | --- |
| 0x656fc | JumpContinue(-2) |
| 0x65708 | Owner part 1 pose `A_1C` |
| 0x6571c | Sound `SFX_BOSS_KANBU_HIP_ATTACK1` |
| 0x65738 | Camera shake_h(0,10,0,60,0) |
| 0x65754 / 0x6576c | GetPos(owner,local0..2); FaceDirectionAdd(-2,local0,20) |
| 0x65780 / 0x65798 | JumpSetting(-2,0,0,raw 0xf24a7b4c); JumpPosition(-2,local0,local1,local2,20,-1) |
| 0x657b8 / 0x657cc | Pose `A_1C`; FaceDirectionAdd(-2,local0,10) |
| 0x657e0 / 0x657f8 | Same JumpSetting; JumpPosition with argument 10 instead of 20 |
| 0x65818 / 0x6582c | Pose `A_1C`; parent WAIT_FRAME(25) |
| 0x65834..0x65878 | Inline sound `SFX_BOSS_KANBU_HIP_ATTACK2`, WAIT_FRAME(7), same sound again |
| 0x6587c / 0x65890 | Pose `D_1`; parent WAIT_FRAME(15) |
| 0x65898 | GetPos(owner,local0..2) |
| 0x658b0 / 0x658c8 | JumpSetting(-2,15,0,raw 0xf24a7e80); JumpPosition(-2,local0,local1,local2,10,-1) |
| 0x658e8 / 0x658fc | Pose `S_1`; parent WAIT_FRAME(10) |
| 0x65904 / 0x65914 | Camera mode(0,0); pose `R_1` |
| 0x65928 / 0x65940 | GetHomePos(owner,local0..2); SetMoveSpeed(-2,raw 0xf24a9680) |
| 0x65950 | MovePosition(-2,local0,0,local2,0,-1,0) |
| 0x65974 / 0x65978 | Explicit event-delete opcode 2; terminal opcode 1 |

The second jump uses the retained temporary position values from the preceding
GetPos/FaceDirection operations; do not insert another target-position query.
The parent waits 25,15,10 and the inline waits 10,7 are script literals, not a
measured total attack duration. Inline audio, movement and dispatcher cadence
must remain separate until their lifetime and blocking behavior are established.
No fresh hit-precheck/defense/damage request follows label 98 in this stream.

## Implementation obligations and outstanding evidence

An implementable dispatcher must preserve inherited local 9, unit/part target
identity, precheck-before-defense ordering and the result-1-only guard request.
Keep post-hit animation/audio recovery after the dispatched damage request and
retain avoidance codes rather than treating every nonhit as the same event.
Use original pose/sound resources; unknown playback/services remain explicit.
Do not replace these branches with the native harness's provisional fixed enemy
impact tick 54 or one-damage attack.

The verifier pins the whole script, seven target/defense/damage imports, result
case/label words and complete label census. Remaining blockers: target cores and
mask-2/mode producers; missing label 99 feasibility; child/inline/switch-state
ownership; movement/float/pose/camera/sound callbacks; weapon power and damage
application; exact main/private pad sampling relative to defense; death/reward/
field-return producers. Original hit/evasion/no-target/mash/held/A+B traces must
record input/state/RNG and animation/audio boundaries. Until then this is a
complete request-stream contract with incomplete runtime composition.
