# Jump takeoff position: side sampling,closest X and ordered outputs

US G8ME01 revision0;recovered2026-10-12 by documentation agent ID2.
Evidence B:complete btlevtcmd_GetTakeoffPosition0x80109054/0x354,
SHA256`8526e7e1fe793874d7ab406629c24983cf0ac09ee233cdc280f3b9a539015edb`.
This specifies the callback's complete local requests/branches. Candidate core
selection/RNG policy and original executed positions remain dependencies;do not
derive a deterministic complete takeoff policy from the wrapper alone.

## Argument resolution and initial coordinates

Capture shared BattleWork before resolution. Resolve actor,target and target-part
operands0/1/2 through evtGetValue,in that order. Capture output operands3/4/5
**raw**,then translate actor through BattleTransID and fetch its unit from the
captured work. No local first-call gate,null/invalid target check or polling.

Read actor GetPos with outputs currentX and **the same ignored pointer for Y
and Z**. The ignored temporary retains Z after the ordered getter;only currentX
is subsequently used. Translate/fetch target,GetPartsPtr with resolved part ID,
then perform:

1. GetHomePos(actor,&X,&Y,&Z),initializing all three candidate coordinates.
2. GetHitPos(target,requestedPart,&X,&ignored,&Z),overwriting X/Z while leaving
   actor-home Y in place.

Thus the initial takeoff coordinate is **target hit X/Z with actor home Y**,
not target hit Y or current actor Y. The position layers and single-part lookup
exception are in [unit/part queries](BATTLE_UNIT_PART_POSITION_QUERIES.md).
No extra hand-authored distance offset is applied before sampling.

## Private candidate sample and concrete arguments

Request GetEnemyBelong(actor),narrow its return to signed byte B. Read unsigned
side byte b=U8(capturedWork+8*B+0xa),compute bias=S8(-b),and request:

`BattleSamplingEnemy(stackBuffer,null,translatedActorID,B,0x01001240,
                     0x01000000,bias)`

The buffer starts at stack+0x28;it is **not** the shared buffer work+0x428 used
by the enemy attack sampling wrapper. Retain the sampler's register return Q
for later zero/nonzero branching. Then always request
BattleChoiceSamplingEnemy(stackBuffer,4,&chosenUnitID,&chosenPartID),ignoring
its register return. Outputs are not locally initialized before that core.

Read S8(stackBuffer+0xa6c),the byte at stack+0xa94. Only a positive value takes
the candidate branch below. It is not a test of the chooser's register return
or chosenUnitID!=-1. Sampler/chooser admission,capacity,sort/random choices,
return Q and count/output initialization are unclosed core obligations. These
literal masks/null weapon are actual original arguments;do not replace them
with the current attack weapon's flags or inferred display labels.

## Candidate X selection and offsets

If the signed sample count is positive:

1. Translate chosenUnitID again through BattleTransID,store the translated ID
   back to its temporary,fetch U and GetPartsPtr(U,chosenPartID).
2. GetHitPos(chosen unit/part,&chosenX,&ignored,&ignored),using one shared
   ignored pointer for Y/Z. Only chosenX is subsequently used.
3. Compare abs(single(currentX-X)) with abs(single(currentX-chosenX)). If the
   first is ordered **greater**,replace X with chosenX;ties/unordered retain X.
4. Compare the requested-part pointer to the chosen-part pointer. Equal pointers
   use offset magnitude60;unequal use30. This is pointer identity,not just unit
   ID or source numeric part ID equality.
5. Reread side byte b and sign-extend it as F=S8(b). If Q!=0,add -magnitude*F
   to X;otherwise add +magnitude*F. Integer multiplication precedes signed-word
   to-single conversion and single addition.

The count-not-positive branch instead rereads/sign-extends b and adds **+60*F**
to X,without consulting Q. The positive-count branch's closer-X replacement
does not replace retained Y/Z with the chosen target's coordinates:Z stays the
original requested target hit Z,and Y stays actor home Y.

For a derived F=1,Q=0,equal-part case,offset is+60;nonzero Q makes it-60.
Unequal parts use+30/-30. F=-1 reverses those signs. These are local derived
branches,not evidence of actual harbor sampler return,count or chosen target.
The side byte is not clamped to±1;retain its original signed width.

## Ordered event outputs and implementation requirements

Finally fctiwz X and evtSetValue(rawOutX,value),then independently fctiwz Y
and write rawOutY,then fctiwz Z and write rawOutZ. Return2. There is no local
raw-sentinel skip for these three destinations. Aliased destinations retain the
last write;do not coalesce them into a single vector assignment. Actor/target/
part and side values are not locally mutated except through invoked services
and event outputs;candidate state is owned by the stack buffer.

| ID | Requirement | Derived acceptance cases |
| --- | --- | --- |
| TAKEOFF-01 | Preserve resolved inputs/raw outputs and mixed coordinate sources | Final Y comes from actor home;Z remains original target even when a closer candidate replaces X |
| TAKEOFF-02 | Preserve private buffer,null weapon/masks and count/output ownership | Sampler receives exact seven arguments;chooser mode4 always requested;positive signed buffer count selects branch |
| TAKEOFF-03 | Preserve closest-X tie rule,pointer identity,return and signed side | Equal distance retains initial X;Q!=0 reverses candidate offset only;nonpositive count always uses+60*F |
| TAKEOFF-04 | Preserve integer output conversion/order | Fractional/negative coordinates truncate toward zero;raw aliases and no local sentinel skip are retained |

The two [ordinary Jump sites](BATTLE_NORMAL_JUMP_EVENT_FLOW.md)80355f64 and
803560a0 supply(-2,L3,L4,L0,L1,L2). Both subsequent approach movement and
MoveFrame query consume these integer outputs. Candidate core effects and actual
unit/part/side/domain can change them;do not insert a native fixed takeoff X
or assume the original target itself is always sampled.

tools/verify_jump_preparation.py checks the full body,concrete two caller records,
sample/choice/branch/output anchors and signed integer conversion bias. Remaining:
BattleSamplingEnemy/Choice core semantics,unit-part/world/scale/source writers,
translated ID feasibility,original RNG/input/position traces and full scene
composition. ISO is read-only;original inspection artifacts remain under /tmp
or ignored paths. No original scene timing or complete game is claimed.
