# Battle jump callbacks, movement storage and event completion

US G8ME01 disc 0 revision 0, recovered 2026-10-12 by documentation agent ID 1.
Evidence B: original DOL instructions, initialized constants and the shipped
Crump REL caller. No original execution, measured impact frame or Switch behavior
is asserted. This closes local callbacks; it does not close the physical-input,
event-manager, animation and battle-main composition needed for faithful timing.

| Inspected routine | US address | Size | Semantic scope |
| --- | --- | --- | --- |
| btlevtcmd_JumpSetting | 0x8010d6cc | 0xa4 | Complete local body |
| btlevtcmd_JumpPosition | 0x8010d208 | 0x4c4 | Complete local body; listed callees remain dependencies |
| btlevtcmd_JumpContinue | 0x8010d0f0 | 0x118 | Complete local body |
| evtGetFloat | 0x80030840 | 0x4bc | Used literal conversion regions only |
| BtlUnit_SetMoveTargetPos | 0x801281a8 | 0x34 | Complete local body |
| BtlUnit_SetMoveCurrentPos | 0x80128210 | 0x34 | Complete local body |
| BtlUnit_GetPos / SetPos | 0x80128c7c / 0x80128c6c | 0x1c / 0x10 | Complete local bodies |
| btlMovePos | 0x80125b70 | 0xac | Complete local body; sin/cos remain dependencies |
| BattleGetFloorHeight | 0x800f7b84 | 0x50 | Complete local body; stage state producer remains open |

Whole-body fingerprints identify all ten routines, including the partly specified
float resolver. They do not imply its other variable namespaces are documented.
The dedicated verifier is separate from the general evidence tooling to avoid
colliding with concurrently reserved weapon-power research:

```sh
python3 tools/verify_battle_jump_contracts.py --output artifacts/reference/behavior/battle-jump-evidence.json
```

It opens the ISO read-only, checks ten body hashes, 55 instruction anchors, nine
constants, nine harbor import bindings and concrete settings/count operands.
Ignored inspection listings use [Gekko decoding](../GEKKO_DISASSEMBLY.md).

## Persistent fields and basic accessors

Let U be the unit obtained from shared BattleWork after actor resolution and
BattleTransID. These callbacks do not locally handle a translated -1 or null U.
The fields are independent, even when initialization copies the same position:

| Field | Type and local use |
| --- | --- |
| U+0x3c, +0x40, +0x44 | Binary32 visible position x,y,z; GetPos/SetPos read/write in that order |
| U+0x154, +0x158, +0x15c | Binary32 move-current x,y,z |
| U+0x160, +0x164, +0x168 | Binary32 move-target x,y,z |
| U+0x16c | Signed word remaining integration count N |
| U+0x170 | Binary32 horizontal speed S |
| U+0x174 | Binary32 vertical acceleration G |
| U+0x178 | Binary32 vertical increment V |
| U+0x17c | Binary32 horizontal heading in degrees A |
| U+0x180 | Binary32 horizontal distance, then residual D |
| U+0x188 | Direction byte; first-call x displacement writes +1 or -1, equality retains old value |
| U+0x18c, +0x190 | First words of destination sound-mode record,used as playback request inputs;voice handle lifetime remains open |
| U+0x1c4 | Address passed to sound-mode loader as caller-owned storage |

SetMoveTargetPos and SetMoveCurrentPos compare each incoming coordinate against
the exact binary32 sentinel **-250000000** at 0x804226c4. On equality retain that
axis's previous value; otherwise store. Checks/stores occur x, then y, then z.
Their fcmpu-based equality also means NaN does not equal the sentinel and is stored.
They do not substitute current position for an untouched target automatically.
SetPos unconditionally stores all three visible axes; GetPos loads each visible
axis and writes its output before loading the next. Aliased output pointers can
therefore affect subsequent loads. No bounds/null validation is added locally.

## JumpSetting and the concrete float words

JumpSetting(event) resolves actor operand 0 using evtGetValue and BattleTransID,
then operand 1 using evtGetValue as a signed word N. Resolve operands 2 and 3
using evtGetFloat, in order, as S and G. Fetch U, store N at +0x16c, S at +0x170,
G at +0x174, and return callback result **2**. It has no first-call gate, range
check, initialization of V/position, or automatic count derivation. Zero speed
and zero count remain zero until a later callback acts on them.

For the encoded literals used by this caller, evtGetFloat calls evtGetWork then
enters the signed-word range -250000000 < word <= -220000000. The used path at
0x800308dc..0x8003090c adds 230000000 with word arithmetic, converts that signed
result to single precision and multiplies by binary32 **1/1024**, constant
0x8041fa1c = 0x3a800000. These operands are event encodings, not IEEE words:

| Raw word | Integer after bias | Verified converted value |
| --- | --- | --- |
| 0xf24a8280 | 2048 | 2.0 |
| 0xf24a7b4c | 204 | 0.19921875 |
| 0xf24a7e80 | 1024 | 1.0 |
| 0xf24a8a80 | 4096 | 4.0 |
| 0xf24a9680 | 7168 | 7.0 |

Raw literal 0 follows the ordinary integer conversion route and supplies 0.0.
The last two encoded words occur in SetMoveSpeed requests; this document proves
the resolver values, not that callback's full mutation/completion policy. Do not
round 0.19921875 to an invented exact 0.2. Other evtGetFloat namespaces, pointer/
variable validity and boundary cases remain separate work.

## JumpPosition argument policy on every call

The callback receives (event,firstCall). Before testing firstCall, resolve all
six operands in order:

1. Actor through evtGetValue, then BattleTransID.
2. Target x, y and z each through **evtGetValue**, converted from signed integer
   to single precision using the original integer-bias conversion sequence.
3. Requested count through evtGetValue.
4. Sound-mode operand through evtGetValue.

Fetch U only after these resolutions. Target coordinates are not directly
resolved using evtGetFloat here. A resumed call still resolves every operand and
fetches the unit even though its initialization branch is skipped. Caller variable
changes can therefore change selected U or resolution side effects; this is not
a pure cached-target object API. Preserve this order if variable services matter.

## First-call initialization and count selection

On firstCall != 0, set event byte +0xd to 1. If the requested count is **nonzero**,
store it at U+0x16c; requested 0 retains the count previously supplied by
JumpSetting. Thus 0 does not always request automatic duration: it uses the
existing count, deriving a new one only if that stored count is also 0.

SetMoveTargetPos(U,resolved x,y,z). Get visible position into three stack outputs,
then SetMoveCurrentPos(U,those values). Call angleABf at 0x8000703c with
(currentX,currentZ,resolvedX,resolvedZ), store returned heading at +0x17c.
Call distABf at 0x8000722c with the same arguments, store returned distance D at
+0x180. Their numeric implementations remain dependencies here. These calls use
the resolved target arguments, not a fresh read of sentinel-preserved target axes.

Compute single-precision resolvedX-currentX and compare with zero. Positive
writes direction byte 1, negative writes 0xff (-1); zero and unordered outcomes
leave the previous byte unchanged. Compute single-precision vertical displacement
dY = resolvedY-currentY in the stack y slot.

Let R denote an original single-precision rounding boundary, n the original
signed-count-to-single conversion, and FMA the corresponding single-result fused
operation. The supported ordinary finite arithmetic path is:

- If stored N == 0: compute R(D/S), convert with fctiwz (toward zero), store the
  resulting signed word as N. Then D := FMA(-n,S,D), using the reloaded count.
- Otherwise: S := R(D/n), store it; then D := FMA(-n,S,D).
- If resulting stored N == 0, return **2 immediately**. The initialization above
  remains applied, but no integration, sound setup or target snap occurs.
- Otherwise V := FMA(R(G*n),0.5,R(dY/n)); store +0x178.
  Then S := R(S + R(D/n)); store +0x170.

Constants zero/half are 0x80422280/0x804222a0; integer conversion bias is
0x802ee608 = 0x4330000080000000. The residual uses fnmsubs and must not be
rewritten as differently rounded multiply/subtract. n conversion itself rounds
to single precision before divisions/products.

There is no local minimum count 1, positive-speed check, negative-count clamp or
finiteness guard. fctiwz exceptional inputs and FPSCR behavior are not specified
here as ordinary gameplay values. Negative nonzero counts follow the nonzero
initialization branch and the signed completion comparison below; a port should
not silently normalize them without proving the actual caller domain.

For nonzero N call BtlUnit_LoadSeMode at 0x80126588 with integer kind 2, resolved
sound-mode argument, pointers U+0x1c4 and U+0x18c. If the resulting handle at
+0x18c is nonzero, request psndSFXOn_3D at 0x800d9848 using that handle and a copy
of move-current position. There is no local play request for a zero handle. Full
mode selection, handle creation/cancellation and original samples remain open.
Initialization then proceeds into integration **in this same callback call**.

## Integration, visible writes and result semantics

Every non-early-return call, including the first one, performs these ordered
mutations:

1. moveCurrentY := R(moveCurrentY + V).
2. V := R(V - G).
3. If the **new** V is strictly negative and moveCurrentY is below moveTargetY,
   clamp moveCurrentY to moveTargetY. Equality does not meet either strict test.
4. btlMovePos(&moveCurrentX,&moveCurrentZ,S,A).
5. SetPos(U,moveCurrentX,moveCurrentY,moveCurrentZ).
6. Decrement stored N by 1 using word arithmetic.
7. If signed N > 0, return callback **0**; otherwise finish as below.

Finishing unconditionally SetPos(U,moveTargetX,moveTargetY,moveTargetZ), overriding
the visible position just written. It does **not** also snap move-current storage
or clear V/S/N. If U+0x190 is nonzero, request psndSFXOn_3D on that handle at the
move-target position. Return callback **1**. The +0x190 handle is consulted without
a local reset/store in this body; do not infer its production from +0x18c setup.

Callback result 0 blocks the current command. Result 1 fetches the next command
on a **subsequent interpreter invocation** and yields the present invocation.
Result 2 advances immediately. These are [dispatcher rules](../ORIGINAL_BEHAVIOR.md),
not counts of rendering frames. Event scheduling/time scale can permit multiple
interpreter invocations in a manager pass; pause, eligibility and caller order
still determine physical-input/tick boundaries. For a stable unit and an ordinary
positive initialized N, there are N integration calls, including initialization,
then a yielded boundary before the next command. The zero-count early result 2
has a different scheduling contract even though neither path remains blocked.

## Horizontal integration and floor-height service

btlMovePos reads original binary32 constants tau = 6.2831854820251465 at
0x80422690 and 360.0 at 0x80422694. Compute rad = R(R(tau*A)/360). Call sin(rad)
at 0x80272410 and round its returned double to single; call cos(rad) at
0x80271ea8 and round likewise. Read x after both calls; write
x := R(x + R(S*sin(rad))). Then read z and write
z := R(z + R((-S)*cos(rad))). Store order and separate products/adds are part of
the local contract. Aliased x/z destinations expose the first write to the second
load. Exact trig output and floating environment remain dependencies.

BattleGetFloorHeight(W,x,y,z) calls BattleStageGetPtr at 0x80139864 and compares
z against the returned stage's binary32 +0xb2c. If z <= that value, return the
original zero constant at 0x80422170. Otherwise return stage binary32 +0xb34.
An unordered comparison takes the second route. This body uses z, not the
incoming x/y values, and does not validate the stage pointer. Stage initialization
and the two values' world-coordinate meaning must be recovered separately.

## JumpContinue and the strict landing boundary

Every call resolves/ translates actor and fetches U. On firstCall != 0, set event
byte +0xd to 1, GetPos(U) into stack outputs and SetMoveCurrentPos(U,those values).
It retains V, G, S and A from earlier movement; it does not initialize them.
Then, on **every** call:

1. moveCurrentY := R(moveCurrentY + V); V := R(V - G).
2. btlMovePos using S/A, followed by visible SetPos from move-current.
3. Query BattleGetFloorHeight(sharedW,currentX,currentY,currentZ).
4. If moveCurrentY < returned floor, store floor into moveCurrentY, SetPos again,
   and return **1**. Otherwise return **0**.

Unlike JumpPosition, this callback neither decrements N nor clamps against stored
target Y. Floor equality remains blocked. It has no new sound requests or count/
velocity resets. Horizontal motion still occurs on the call that goes below the
floor; a port must not stop x/z as soon as old y reaches ground. The first-call
copy matters because JumpPosition may have snapped visible position while leaving
move-current at its integrated coordinates. After a yield, the following pose/
sound script commands require another interpreter invocation.

## Concrete opening Crump composition

The [normal attack request stream](HARBOR_BOSS_NORMAL_ATTACK.md) gives these
verified settings/counts; the final JumpPosition operand is sound mode -1:

| Request offset | JumpSetting(N,S,G) | Following JumpPosition requested N |
| --- | --- | --- |
| 0x655a8 | (0,2.0,0.19921875) | 0 at 0x655c0; derive N from horizontal distance/speed |
| 0x65780 | (0,0.0,0.19921875) | 20 at 0x65798; nonzero count overrides before speed derivation |
| 0x657e0 | (0,0.0,0.19921875) | 10 at 0x657f8 |
| 0x658b0 | (15,0.0,1.0) | 10 at 0x658c8; requested10 overrides stored15 |

Thus the last bounce is not a 15-call duration simply because its JumpSetting
contains 15. The two zero-speed recovery settings do not divide by zero on the
ordinary path: their nonzero JumpPosition counts select S = distance/n.

The initial landing yields result 1 before PreCheckDamage. After result-1
precheck, defense and damage requests lie later in the script, before JumpContinue.
The continuation retains landing V/G/S/A and yields before the later `A_1C` pose,
impact sound and camera shake. Do not move these post-continuation requests to an
invented impact tick or assume the hit happens at the camera-shake request.
The degenerate initial automatic count 0 can advance immediately without any
integration/snap; original target positions determine whether it is reachable.

This supplies implementable local movement and event boundary contracts. Still
open: approach MovePosition/SetMoveSpeed, target/hit-position outputs, angle/dist/
trig/FPSCR services, sound-mode handles, inline/animation ownership, stage values,
main/general/private pad update order, event execution credits and original state/
input traces. Preserve simulation integration calls separately from render frames.
No fixed tutorial impact54 or attack duration is justified by these static bodies.

## Scoped implementation requirements and acceptance cases

These requirements derive from the local B contracts above, not from the native
harness or original execution. They can be implemented independently while the
remaining services are recovered. They are not the complete-game requirements.

| ID | Required behavior | Derived acceptance check |
| --- | --- | --- |
| JMP-01 | Retain separate visible/current/target storage, count, speed, acceleration and velocity | After a final JumpPosition call, visible position is snapped to target; move-current retains integration/clamp results unless equal by arithmetic |
| JMP-02 | Resolve all arguments before the first-call branch; initialize only on firstCall != 0 | Resumed polls resolve actor/arguments and integrate without recomputing target/count/trajectory or reloading sound modes |
| JMP-03 | Nonzero requested count overrides stored count; requested0 preserves it and derives only from stored0 | Setting N15 then requesting10 enters integration with10 and leaves9 after its first ordinary step; requested0 with stored15 uses15 |
| JMP-04 | Preserve the automatic zero-count result2 and nonzero-count landing result1 distinction | With finite zero horizontal distance and speed2,stored/requested N0 returns2 after initialization with no integration/snap/audio setup; initialized N1 integrates,snaps and returns1 |
| JMP-05 | Continue from visible position on first call, retaining velocity/speed/heading; land only below floor | At floorY0,currentY0,V0,G1,finite heading/speed0,first call remains blocked at equality while V becomes-1;next integration goes below,clamps to0 and yields1 |
| JMP-06 | Keep rounding/fused boundaries, signed count comparison and sentinel policy | Verify used literal0xf24a7b4c decodes to exact0.19921875;each sentinel axis preserves its destination;equal-x direction retains the old byte |
| JMP-07 | Preserve event callback status semantics separately from simulation/render ticks | Interpreter result0 repeats the command;result1 yields before the next command;result2 can advance immediately;scheduled invocation count is recorded independently of rendering |

Acceptance cases use controlled finite service inputs to test these local rules.
They do not establish actual Crump target coordinates, stage layout, input sample
ownership, sound lifetime or the number of physical ticks before an impact.
An original comparison must additionally log first-call flags, all movement
fields, visible/target positions, callback status, event invocation identity,
precheck/defense calls and sampled pad history for the same input sequence.
