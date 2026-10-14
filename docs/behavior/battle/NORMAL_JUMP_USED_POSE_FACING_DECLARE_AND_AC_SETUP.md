# Ordinary Jump: used pose, facing, declaration and AC setup bindings

US G8ME01 disc0 revision0, read-only static DOL inspection,2026-10-13.
Scoped follow-up to [JUMP-01](../../requests/JUMP-01.md),
[EVT-01](../../requests/EVT-01.md) and[AC-01](../../requests/AC-01.md).
Read the [complete268-command Jump graph](BATTLE_NORMAL_JUMP_EVENT_FLOW.md)
for command order. This pins used source domains and resolves stale local
summaries; it does not establish original executed timing or native equivalence.

## Exact ordinary Jump AC preparation and reset scope

The ordinary mask2-clear branch requests GetWeaponActionLv→AcSetDifficulty→
SetupAC(-2,1,raw1,0). First-strike branches place equivalent preparation in an
attached movement brother; do not call both preparations on ordinary entry.
No AcSetParamAll or AcSetFlag request exists in NormalJump.

| Callback | US address/size | Concrete source behavior |
| --- | --- | --- |
| btlevtcmd_GetWeaponActionLv | 801058e0/48 | Resolve weapon operand0; read unsigned byte weapon+6e; write raw output1; return2 |
| btlevtcmd_AcSetDifficulty | 80107b38/80 | Resolve actor then base, translate actor, fetch unit, call difficulty setter, return2 |
| BattleActionCommandSetDifficulty | 800fa12c/30 | Low8 base→B1cbc; adjusted clamp(base-U8(unit305)+U8(unit306),0,6)→B1cbd |
| btlevtcmd_SetupAC | 801077ec/98 | Actor resolve/translate/fetch; resolve ID; RAW option operand2; resolve operand3; call setup; return2 |
| BattleActionCommandSetup | 800fa45c/94 | Ordered fields below, then registration lookup; no callback invocation |

Jumpweapon0x803538a8 byte+0x6e is3. Base difficulty is therefore3;
unit modifier bytes305/306 can alter adjusted difficulty. Base is written before
registration, and setup does not erase it. Source SetDifficulty arithmetic uses
the full incoming base with byte modifiers before signed0..6 clamp, not a clamp
of an already narrowed base. Existing [Hammer branch](BATTLE_HAMMER_WEAPON_BRANCHES.md)
also pins these shared callbacks; its source size80 for AcSetDifficulty is
correct, while older short-summary size38 is stale.

Setup's literal ordered writes are:

1. Unit pointer→B1c90; option/raw1→B1c94.
2. State0→B1c9c; count0→B1cb4; pending result1→B1cb8.
3. Clear callbacks in order main1ca0, display1ca8, result1ca4, delete1cac.
4. Resolved final argument0→B1c98; zero→B1cc0.
5. Scan20byte registration records at0x80364348 until ID matches or zeroID.

ID1 registration is [1,8012aae0,8012a8c4,8012a7cc,8012a7c0]: main TimingA,
result TimingA, display, delete. Copy main; copy display only when actor token
unit27c bit0x10 is clear; copy result and delete. Setup has no null-actor guard.
UnknownID still retains earlier resets but leaves callbacks zero.

**Setup does not clear** B1cc4 flags, B1cc8..1ce4 parameters,
B1cbc/bd difficulty bytes, B1ce8 classification, B1f20..1f4b scratch,
B1f50 options or either general/private pad history. No implicit AcSetFlag0,
parameter defaults, history reset or TimingA-main call belongs in SetupAC.

[TimingA main](BATTLE_TIMING_A_COMMAND.md) owns a separate state0 initializer:
clear44bytes atB1f20, float-300→1f34, float0→1f38, word20→1f40,
state99→1c9c, clear ONLY bit0 of1f50 and classification0→1ce8. Preserve bit1
of1f50 and all fields outside this scope. Main-state100 is an awaiting state;
StartAC writes100 and does not independently initialize this scratch.

[BattleMain order](BATTLE_MAIN_SERVICE_ORDER_AND_FINAL_STATE_CHECK.md) is
BattlePadManager→BattleSequenceManager→BattleActionCommandManager before stage/
audience/display. The callback manager supplies main-state0 execution when its
original scheduler reaches it; setup does not run that callback itself. Native
composition must keep initializer and activation ownership separate and must not
invent a render-frame delay/reset. Result uses general pad history, while the
manager also owns distinct private-pad history updates.

## Mario pose table and typed pose requests

Mario bodypart1 original pose table0x803728b8 is34 eight-byte type/key records.
Concrete used requests:

| Requested decimal type | Original key pointer/name | Group fallback |
| --- | --- | --- |
| 42 (0x2a) | 80422f38 / M_R_1 | type69 / M_S_1 |
| 40 (0x28) | 80422f30 / M_W_1 | type69 / M_S_1 |

Type42 is run, type40 walk; neither selects standing M_S_1. The source
GetPoseNameFromType8012772c/80 scans8byte records, returns requested match or
bandstop28/39/69; type>=70 returnsnull. It does not normalize case or infer a
pose by current movement speed. Part's table pointer is runtimeP1bc.

AnimeChangePoseType80106b20/98 resolves actor, part and type, calls
GetPoseNameFromType(P,type) and ignores that return, then SetAnimType(P,type).
SetAnimType801276ac/80 performs a second lookup and calls
btlDispChangeAnime80101168(P,name,force0 for types40/42). ChangeAnime copies
name toP1c4 and updatesP204: OR3, clears0x80/0x200/0x10 and clearsforcebit8
for this route. Actual pose binding/frame publication belongs to btlDispMain.
It does not write unit facing or move-count state. Original b_mario animation
resources own these keys; duplicating a stale idle pose for types40/42 is invalid.

## Used facing modes and exact storage

CalculateFaceDirection8010e684/318 resolves actor, actorpart, target/literalX,
part/literalZ, then retains mode operand4 RAW and outputoperand5 RAW. Used:

- Mode1: target operand2 is signed literalX, converted tofloat. Start from
  S8(U189), or actorpartPbd when supplied. Compare literalX minus actualactorX
  (including provided localpartX). Positive→+1,negative→-1,equal retains sign.
  LiteralZ operand3 is resolved but does not affect this mode's result.
- Mode16: translate target operand2 as a unit ID and use resolved targetpart.
  Start from actor/part facing as above. Compare targetcurrentX plus localpartX
  versus actorcurrentX plus localpartX. Positive→+1,negative→-1,equal retains.
  Partflag0x10000000 uses the absolutepart position getter instead of adding
  localX. Used actorpart-1 supplies no actorpart; triotargetpart1 has localX0.

No source atan2 or target hitbox normal determines these two used sign outputs.
Other modebits have separate branches; the scoped adapter must not claim them
implemented by this description. Both modes write only the event output and
return2. They do not change actor position, facing or part facing themselves.

ChangeFaceDirection8010e5a8/6c resolves/translates actor, resolves direction,
fetches U and writes low8(direction) to**U189**, thenreturn2. It does not update
U188 (last movement direction), partPbd or yaw. ResetFaceDirection8010e4bc/70
copies sourcegroup byteB+a+8*S8(Uc) toU189, thenreturn2. FreshMario restores+1;
opponents restore-1. These direction and facing fields are separate owners.

FaceDirectionSub8010b7c4/98 resolves actor and amountoperand2, readsS8(U189),
then resolves CURRENT value of operand1 and writes
value - amount*sign back THROUGH THE SAME raw operand1. It is coordinate/work
arithmetic, not actor rotation or movement. Ordinary Jump uses this to move
bounce/return targetX behind current facing. Preserve32bit integer arithmetic
and event variable routing. Part position/side fields do not substitute forU189.

## AttackDeclare and fresh trio no-protector route

btlevtcmd_AttackDeclare8010ad00/ac resolves/translates actor, then resolves/
translates target and resolves part; its actor lookup result is not used by the
core. It passes target/part by reference toBattleAttackDeclare800faecc/150,
then writes possibly rewritten target/part to their original raw eventvariables,
return2. It can redirect a protected target; no host nearest-target choice.

Core scans64units for an alternate unit with unit140==requestedtargetID,
unit27c lacking masks0x1/0x4, CanGuardStatus true, and a part whose attribute lacks8.
On a match it schedules protectedtarget hitevent0x1029, sets its token4,
sets battleflag1, schedules protector0x1038, sets protector token2 andrewrites
unit/part to protector'sactualIDs. These callbacks/barriers remain sourceowners.

Fresh source initialization/phase reset sets unit140=-1 and ordinarytrio has no
protector assigning requestedtargetID there, so the scan finds no match:
no flag/event/target mutation occurs in this scoped case. The wrapper still
performs resolution/translation/output writes. WaitGuardMove8010a1c8/1c returns
0 whileBef4 mask0x1 is set, otherwise2. Do not erase that service globally merely
because freshtrio follows its immediate route.

## Mario datakey7 and bounce/landing owners

Mario data table0x803729c8 tag7→0x80416bd0,
subset_attack_ready_jump_event. Exact8bytes are opcode2 thenopcode1, nooperands,
SHA256`1e9fcd4ca7e6723c4a822c370faad68aa89031573b2376d87e4cc3c6626ede61`.
RunDataEventChild still owns child creation/binding/deletion even though this
script requests no services. It is not a missing arbitrary attack-ready scene.

[Complete Jump callbacks](BATTLE_JUMP_EVENT_CALLBACKS.md) ownJumpSetting,
JumpPosition andJumpContinue. Ordinary initialJumpSetting suppliesN20,S0,
G0.69921875; MarioJumpPosition is a different trajectory service used toreach
attack impact, so do not replace it with genericJumpPosition.

Generic recovery source calls:

| Path | TargetX arithmetic behindU189 | Counts | Acceleration |
| --- | --- | --- | --- |
| Timing failure | subtract27,then20,then8 | 20,12,8 | first0.69921875;later0.599609375 |
| Final success | subtract60,then20,then8 | 32,12,8 | first0.69921875;later0.599609375 |

Each uses currentMarioXYZ, actorhomeY andsourceZ; final soundmode-1 is not
speed. JumpPosition honorsstoredcount whenrequested0, deriveshorizontal speed,
initialverticalV=G*N/2+dY/N, integrates onfirstcall, decrementsN, snapsVISIBLE
XYZ atfinish andreturns1(advance+yield). Its moveCurrent baseline/V remain
separate. Quantizedaccelerations are original eventfloat encodings.

Avoidance JumpContinue retains existingS/G/V/heading. FirstcallcopiesvisibleXYZ
intomoveCurrent; eachcall integratesY/V andhorizontalmotion, testsfloor STRICTLY
currentY<floor, snapsY andreturns1 whenbelow; equality stillblocks0. It doesnot
initializevelocity, usefixedcount, decrementN orstartnewsound. BattleStageInit
setsstage+b2c=90,+b34=-50; BattleGetFloorHeight returns0 forZ<=90,else-50.
UsedtriojumpZ(-10,0,10) remains on sourcefloor0. Firstcall andstrictlanding must
follow original event ticks, not renderer frames or a fabricated bounce curve.

## Remaining bounded dependencies

Original executed AC-manager/event/pad order andfloat/trig rounding, pose-render
clock/facingmatrix publication, protector/hit-event/transitive services, complete
MarioJumpPosition andothermotion/result/aftereffect/camera/audio providers,
originalsourcepouch/status/flag writers andused268-command visual/inputtrace
remain required. No nativecode edits,build/testsororiginalexecution inthis task.

## Complete shared facing wrappers and Harbor admission (2026-10-14)

`battle_facing.cpp` now implements the complete CalculateFaceDirection8010e684/
318 and ChangeFaceDirection8010e5a8/6c wrappers. Input argument pointer and
initial battle are retained before actor resolution/translation. Resolve actor
part, target/literalX and target part; retain RAW mode and output. Only masks1
or8 skip target translation. Actor/target unit lookups independently use current
global battle; part lookups retain those fetched units. Target unit/part lookup
is gated by mask16. Both unit189 and partbd are read in source order for the
seed; mask4 replaces it with the signed side byte from the initial captured
battle, and mask8 replaces that with unit188 then optional partbc.

Mask16 compares actual actor/target X after unit-position reads and optional
part adjustment. Attribute10000000 uses absolute part XYZ; otherwise only X
adds the part offset with a single add, without a facing factor. Next mask1
independently compares the retained signed literal X (single conversion) with
fresh actor/part X. Positive delta selects1, negative selects-1, equal/unordered
preserves the current result. Mask100 finally negates the word with wrapping
arithmetic. The event output is written once at the end. Mode0, combined modes,
part seeds, negative byte values and inversion retain their source meanings;
there is no normalization to a guessed heading or nearest target.

Direction-write resolves/translates actor, resolves direction, fetches current
unit and narrows to unit189 only. Facing calculation does not itself mutate
unit/part facing or rotation. Native Harbor binds both wrappers using its real
unit/part/event memory and platform single arithmetic providers.

Tests cover mode precedence, both comparisons in combined17, signed seeds,
unit-before-part read order, offsets versus absolute positions, equality/NaN,
integer-to-single rounding and low8 direction write preserving188. The selected
original Jump now passes both facing requests and stops live at StartAC80107788;
HP/result and the one payment count stay unchanged. No automatic tutorial/menu
activation, animation request/acknowledgment, action-manager execution, hit or
visual-fidelity result is fabricated. Source body hashes are pinned by the
read-only harbor-motion verifier; listings remain in ignored artifacts.

Next provider: complete StartAC wrapper with its real help/audience-window
setter and original manager cadence, followed by source pose and motion/hit
services. Preserve simulation tick ownership and physical-button input while
integrating original menu/HUD and second-screen scene/audience presentation.
