# Field Mario owner, control depth and battle preparation

2026-10-13 read-only US source scope: complete marioGetPtr8005e0c0/8,
marioCtrlOff8005c2c4/28,marioCtrlOn8005c290/34,
marioCtrlOffChk8005c2fc/10,marioFBattlePrepare8005c3cc/6c,
partyUsePost800bcf34/5c and global partyCtrlOff/On800bed44/0c (38bytes each).
Full marioInit/ReInit listings inspected for the explicit initialization
boundary and retained/reset fields; their transitive engines are not declared
complete. Ignored evidence `artifacts/recovered-temp-evidence/field-*`.
No native edits, shared PROGRESS edits or commit.

## Identity and honest initialization boundary

marioGetPtr loads WORD8041e900 (r13+19e0) and returns it. It performs no
allocation, validation, actor lookup, context selector or null repair. Every
used field Mario service must refer to this same persistent owner; neither
battle Mario unit memory nor ProfessorSceneActor alone is this original object.

marioInit8005b5c8 allocates __memAlloc(0,2f8), publishes returned pointer
at8041e900, then memset(pointer,0,2f8) at8005b5f4..600. This establishes
**word0=0,word4=0,unsigned byte38=0** at that constructor point. It then sets
five pose/resource words27c..28c=-1, initializes pouch/motion/camera/resources
and the remaining field state. The local body has no explicit byte38 store;
the later repeated `stw ...,38(r5)` is a DIFFERENT global position-history
array starting803e9848, not Mario byte38. Do not mistake that disassembly
offset for a control-depth initialization. Pose requests locally mask word4;
called motion/camera/resource services remain their own producers. The zeroing
point is not proof of every final full-object field after all callbacks.

marioReInit8005b1d8 reuses the existing owner, captures selected motion,
angles, pose/resource handles and party/current/saved-kind bytes, then memsets
the same2f8 bytes at8005b2a4..b0. It preserves ONLY word0's80000000 bit
explicitly before subsequent services; it does not capture word4 or byte38.
Restored fields and call side effects must follow that real re-entry path.
Neither battle1f nor field waiter calls marioReInit. Resetting depth or inventing
a full zeroed record at battle finish is not original behavior. See
[retained directions](../overworld/OVERWORLD_MARIO_DISPLAY_DIRECTION.md).

## Complete nested control operations

CtrlOff: load original owner; word0 OR2 and store FIRST; load unsigned byte38,
add1 as word, store low8; reload byte38, sign-extend and return. Wrapping is
source byte arithmetic, not saturating bool/count. It does not touch word4.

CtrlOn: load owner; unsignedbyte38-1,store low8 FIRST; reload/sign-extend for
zero comparison. Only signed result==0 clears word0mask2; every nonzero result
retains flags. Reload byte38 again, sign-extend and return. Byte0 decrement
becomesff/-1 and does not clear inhibit. No underflow guard, clamp or forced
zero exists. Unknown/unbalanced call history must remain explicit, not repaired
by unconditional control enable.

CtrlOffChk reads and returns signed byte38. It does not return bool word0&2
or normalize negative values to0/1. Other main flags, motion, keys and event
locks remain separate owners. Existing native `keysEnabled`/control booleans
must not overwrite this reference depth or word4 merely to reflect UI state.

## Exact prepare order and transitive gates

Prepare captures M=global8041e900 once. Copy M8c thenM90 as words into
globals8041e8f4/8041e8f8; copyM94 into8041e8fc (saved XYZ). Call
marioAdjustMoveDir800965fc. AFTER it returns, reread capturedM motionU16+2e:
if!=20, word0 OR1000/store; motion20 leaves that bit unchanged.
No word4/byte38 write occurs in this body.

Then marioGetPartyId80141050→partyGetPtr800bf1b4→partyUsePost800bcf34,
followed by partyCtrlOff800bed44. UsePost accepts null as real no-op;
nonnull Pmode34!=3 also no-op. Mode3 sign-extends Pkind31, indexes
803143e8+40*kind, invokes nonnull record+10 callback with P; null callback
skips. Broader used ability/callback teardown remains a separate provider.

partyCtrlOff ignores incoming arguments, visits BOTH registered pointers
8041ead8 and8041eadc in order; each nonnull Pword0 OR20000000. CtrlOn visits
both identically and clears that mask. Frankly auxiliary slot1 is included;
do not use only the current Goombella lookup as the service's write scope.
Preparation already calls global partyCtrlOff before the outer start callback's
additional current-party-nonnull request.

Ordinary adjustMoveDir branch is described in
[low-HP idle/facing helper](../overworld/OVERWORLD_MARIO_LOW_HP_IDLE.md): it snaps
from current display1ac, not battle facing or current input. Special main00100000
branch and numeric/trig owners remain explicit separate inputs. A compact
flags/depth implementation cannot silently claim that entire motion helper.

## Start, ending and field waiter ownership

evt_npc_battle_start8004ef54 first-call order is Prepare thenCtrlOff, party
lookup/nonnullCtrlOff, result0, audio flag and sequence4; resumed returns2.
seq_battleMain state2 adds CtrlOff when global r13+1a48=8041e968
(debug_battle_flag) equals0; battle_exit adds
CtrlOn under the same global gate. Admitted field waiter adds its own CtrlOn.
For that ordinary balanced history, depth0→1→2→1→0; these are source-derived
call effects, not permission to initialize directly to2 at battle entry.
Script key on/off remains independent; preserve the unchanged field VM.

Ending1f exact sequence after resource export:

* 80216614 load E=Bf28;18 BattleFree800f8908;24 clearBf28;30 topbank0=4.
* 80216634 marioGetPtr;38 load THIS Mword4;40 clear mask1;44 store Mword4.
* 80216648 tests retained PRE-EXPORT current battle-party KIND, not current
  field registry. Nonzero:4c pass translatedID;50 marioPartyEntry801412fc;
  zero:58 marioPartyKill80141198.
* 8021665c marioGetPartyId;60 partyGetPtr;nonnull6c loadPword4;70 clear1;
  74 storePword4. Then78 rule accounting;80 audienceSoundStop(1).

The field-Mario pointer query occurs AFTER E free/topbank writes and BEFORE
party reconstruction. Mask1 applies word4 only, preserving all other word4
bits; it does not clear word0/depth, call controls on, change position or unlock
pouch. Constructor gate/appearance remain
[field handshake](../battle/UNDERGROUND_TRIO_FIELD_RETURN_HANDSHAKE.md) and
[Goombella constructors](../partner/FIELD_PARTY_POSITION_CONSTRUCTORS.md).

## Native owner boundary and remaining requests

A source-backed compact native owner can retain actual mapped word0,word4,
unsignedbyte38 with the operations above, plus separately supplied preserved
XYZ/action/party indices when their original services are bound. It should live
with field state and be shared by Game/ProfessorRuntime/ending adapters.
This is a scoped projection of original fields, not a newly invented complete
2f8-byte Mario object. If encoded pointer identity is exposed, restrict access
to actually bound offsets and retain missing providers; do not read invented
zero fields through arbitrary original callbacks.

Needed next: bind balanced live start/sequence/exit/wait control calls and typed
word4 owner; actual source constructor/reinit policy at newgame/map re-entry;
saved XYZ, AdjustMoveDir and party ability/collision/appearance contexts; field
party registry/model/pouch join and paused-root scheduling. Native3DS storage,
save mapping and original execution comparisons remain explicit boundaries.

Validation: complete read-only used leaves, initializer direct-write/pointer
audit and ending1f instruction order; scoped `git diff --check`. No native
tests/builds/shared PROGRESS edits or commit.

## Original source SHA256 evidence

| Body/span | Address / bytes | SHA256 |
| --- | --- | --- |
| marioGetPtr | 8005e0c0 /8 | 3a59459f28ddd5d287c200434ad158a9dfe277f4f6bcaae3eb8698505aa9773d |
| marioCtrlOff | 8005c2c4 /28hex | 75e64f986f5a9f6355056deba25c2679df4975f4f7e38fdafc4307d99461eed4 |
| marioCtrlOn | 8005c290 /34hex | 027dbe3ead8a5095ff6196ff4ed5e643933e9255dd57875c519ee3bd70e850ff |
| marioCtrlOffChk | 8005c2fc /10hex | 885582e119541dec38cb4ec1613da1fbb6cc639958deda35601be1a7b3201300 |
| marioFBattlePrepare | 8005c3cc /6chex | cf6119386901ec44eac10a0408897195448a7c50925c63f46acdc68ecf9d04fa |
| partyUsePost | 800bcf34 /5chex | 83b854256122fbfcc9126d2722882cf7674f8f3078970a1b3886eb7d69331ea9 |
| partyCtrlOff | 800bed44 /38hex | 480fc234b664147581dc21623e9af2fc2c2cf0ba0a35c3386880bd5e36d253a6 |
| partyCtrlOn | 800bed0c /38hex | 0ce771d9991495096138debff7650f315c875f8d056b2acf6b22cdae0c835318 |
| marioInit complete source fingerprint; scoped init evidence only | 8005b5c8 /488hex | e3976200ee5fc18a245b234f0b88c4095a0de3108c620034b54df1ea545ee6ce |
| marioReInit complete source fingerprint; scoped reset evidence only | 8005b1d8 /3f0hex | 768a85127d313dcaf6f86c3cbc9b9cc6c42fa3e4f826f72aa494af13ce81f661 |
| Ending free→field-pointer/party→rule/audience span | 80216614 /70hex | 4208dd0b7a86cca54c796d1e451365e68f81ede1d25017e8e818e0194468f926 |

Hashes pinoriginalbodies/spans; theyarenotproofallcalledenginesorvalidnative
domains execute correctly. ISOreadonly; nooriginalbody bytes/assets committed.

## Native implementation frontier reported by parent

FieldMarioBattleState is an explicitcompact projection of sourcewords0/4 and
unsignedbyte38 initializedatverifiedconstructor-memsetpoint. Portablewrapping
signedCtrlOff/On/Chk andGamefieldservices/inputinhibit sharethissameowner.
Ending1fqueriesit,clearsword4mask1,preservesotherbits,andretainsoriginal
marioPartyEntry801412fc frontierbeforeconstructor success. Prepare/Post are
explicitunboundrequests; nofullMarioInit/record/field-return/controlbalanced
execution claimisaddedbythisdocumentation. Parentownsactualcode/tests/build
validation andincrementalcommit; thisdeliveryonlychanges thissubsystemcontract.
