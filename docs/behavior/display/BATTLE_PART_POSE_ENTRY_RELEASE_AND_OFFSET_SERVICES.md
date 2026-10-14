# Battle part pose entry, release, and offset services

2026-10-14 US G8ME01 disc0 revision0. Read-only original/native source evidence
for the [typed pose factory](BATTLE_POSE_POOL_PROVISIONING_AND_TYPED_NATIVE_ADMISSION.md).
Owned new contract only; no native/shared documentation or index edits/commit.

## Complete per-part entry order and original model pointer

btlDispEntAnime801011fc/90 captures first part U14, then follows the live
nextWORD P0. For each:

1. Read P1bc pose table. Null→WORD P1c0=-1, advance live next.
2. Read source part S=P4, then retain model-name pointer S8. Null→P1c0=-1,
   advance, leaving existing pose-name bytes alone.
3. searchPoseTbl80103cd0(P1bc,28)→name. Source table search checks at most28
   rows8bytes, first exact requested OR type28; this request specifically asks28.
4. strcpy8026d5fc(P+1c4,returnedName) BEFORE resource Entry. No null/name-size
   guard or fallback is added locally.
5. animPoseEntry8004176c(retainedModelName,mode1)→rawhandle; store WORD P1c0.
   No failure check or negative-handle repair.
6. Read P0 freshly, continue untilnull.

Each qualifying part gets a distinct pose allocation, even when model pointers
are equal. Model/texture resource caches use their own shared references.
Entry chooses initial descriptor0 independently of the copied idle name;
copying P1c4 is not an animPoseSetAnim call.

## Actual Mario has three source parts, not two

Original kind80372afc BYTEb4=3; source part table80372a18, stride4c.

| Part ID | Source record | Model pointer/string | Pose table | Source type28 name | Attributes |
| --- | --- | --- | --- | --- | --- |
| 1 | 80372a18 | 802f3fa8 / b_mario | 803728b8 | M_S_1 | 00000009 |
| 2 | 80372a64 | 802f3fa8 / b_mario | 80417bc8 | KRN_H_1 | 13010000 |
| 3 | 80372ab0 | 802f3fa8 / b_mario | 80417bc8 | KRN_H_1 | 13010000 |

Goombella kind80393bec BYTEb4=1, record80393ba0/ID1, model pointer804243b0
`c_pkr`, poseTable80393a98/type28`PKR_S_1`, attributes00000009.

The original b_mario directory85 descriptors does **not** contain KRN_H_1.
Keep the authored part-name bytes, but original later string selection exhausts
and selects descriptor0 M_Z_1. Do not invent a KRN pose or remove these parts
to avoid the fallback. Parts2/3 still own their source attributes/draw admission,
resource references and pose handles even when ordinary visible-body gates
suppress their vertices.

## Unit deletion invokes effect detach before pose release

Original registered BtlUnit_Delete80128e08/1d8 first performs ordered event,
ordering-table,status-effect/icon,custom and extension cleanup. In its part
loop, set P4e8=-1 if needed. For each P1c0!=-1:
animPoseSetEffect80040a28(handle,0,0), then reload P1c0 and
animPoseRelease8003b2d0(handle). It does not locally change P1c0 or free each
part node. Follow nextP0; later BattleFree(U14block),BattleFree(U), registry
clear LAST. Borrowed original source/kind/pose-table data is not freed.
The full [unit lifetime contract](../unit/UNIT_PART_DEFENSE_AND_LIFETIME.md)
owns this broader deletion graph.

## Complete ordinary pose release and cache refcounts

Release8003b2d0/194 computes captured pose R=currentPool+ID*170hex.
If RWORD4==0 (ordinary Entry default), in order:

1. Rflags0&40 set→smartFree8002f878(R15c).
2. Fresh Rflags0&80 set→smartFree(R164).
3. Fresh current W resource table W0; captured R10 resource index→E with10hex
   stride. Wrapping decrement ErefWORD4, store, then signed<=0 admits teardown.
4. If resource teardown: Eflags0=0 FIRST; fresh W texture table W8, EWORDc
   texture index→T with0c stride. Wrapping decrement TrefWORD4; if signed<=0,
   fileFree8018a260(TWORD8), then Tflags0=0. Always fileFree(EWORD8) afterward.
5. Rflags0=0, even if resource still referenced. Other R fields are not cleared.

Pose GetPtr/Release has no ID/active/count validation. A typed native owner
must avoid invoking source release twice for one real lifetime; treating a
stale registered number as a new pose can decrement references incorrectly.
Do not zero whole resource/texture/pose records or free immutable pack payload
merely because one shared pose was released.

Resource refcount is per live pose. Texture refcount is per distinct loaded
model resource cache entry, not per pose: Mario's three b_mario poses normally
share modelref3 and one model-associated texturecache ref. Releasing the first
two decrements only modelrefs; final model release decrements/releases its
texture cache before model file handle. Native shared pack/atlas lifetimes may
project these source caches, but preserve the separate explicit counts.

RWORD4!=0 enters the borrowed/group branch: wrapping decrement RWORD8 first;
new signed>=0 returns without resource/poseflags release. Negative then does
resource/texture decrement/free ordering above, without the two ordinary
smartFree requests. This branch is real if effect/linked reuse is admitted;
it cannot be merged with ordinary mode1 Entry's R4=0 destructor.

## Offset service order and exact ordinary scalar leaves

After pose/name/effect-mode consumption, original btlDispMain always calls:
gravityOffsetControl80101060(P), floatOffsetControl80100f34(P),
statusPoseControl80100e6c(P), BtlUnit_GetPartsWorldPos80128b10(P,&XYZ),
then animPoseMain8003ed2c(P1c0). They are ordinary pointer-based services,
not event wrappers; none supplies an event readiness result.

gravityOffsetControl reads current B and ownerU=P4ec. Active predicate:
Bef4&40000000 set AND (U104&1004)==0. Active increments LOW16 P1a6.
Inactive with U16P1a6==0 returns unchanged; otherwise store value%120, reload:
>=60 incrementsLOW16;1..59 decrementsLOW16;0 returns. No float offset,
sin/cos, pose setter or global reset occurs in this leaf.

floatOffsetControl active predicate: both U1c bits00100000 AND00200000 set,
then CheckStatus80127814(U,27)==0. It calls CheckStatus only after both flag
tests. Active increments LOW16 P1a8. Inactive with value0 leaves it; otherwise
store U16value%90, reload:>=45 incrementsLOW16;1..44 decrementsLOW16;0 leaves.
Read owner and counters live; initial zero counters alone do not bypass an
admitted increment branch. Geometry consumers use these phase counters later.

statusPoseControl dispatches BYTE P215:
0→return;1/defaultunsupported→_pose_def80100e00;
2/3/7→_pose_one_pattern80100d28;4/5/6→_pose_two_pattern80100a80.
These are actual helpers, not imported animation descriptor names.

Complete ordinary _pose_def: if BYTE P214!=0 return. Otherwise storeP214=1
BEFORE animPoseSetEffect80040a28(P1c0,0,0). Read ownerU1c; mask01000000 set
requests animPoseSetMaterialLightFlagOn800406f0(handle,8); clear skips.
Thus opaque/no-status pending-name consumption settingP2151/P2140 admits
SetEffect as a real next provider before animation Main.

With actual unlinked pose R90=-1, SetEffect(handle,0,0) skips old-link release,
stores R90=-1, reloads currentwork/pool and clears ONLY materialLightWORDec
bit1; no resource/file/allocation/Main/clock request. Existing linked90 requires
its complete detach/refcount body first. Positive pattern modes need their
authored effect/paper services; no generic successful deformation is inferred.

## Evidence and validation

New ignored complete/scoped listings battle-pose-constructor-* under
artifacts/recovered-temp-evidence/; raw source kinds/tables inspected directly.

| Body/address/size(hex) | SHA256 |
| --- | --- |
| btlDispEntAnime801011fc/90 | e593e6568df2bd63080b3ec2bd48abd120880757ac6c95a48d076213448bfb0f |
| Release8003b2d0/194 | 32d91c22e37a382d32a667e138d66e29f3d42fb2287b963bdc390154457863b6 |
| gravity80101060/94 | 325f9082d7b9cd0502692af16e35159c0d4dadb60fd956cb35c4050f2d6a6919 |
| float80100f34/c8 | 29f3ea3f7b46c1575fbcc03eea542730bd72e67a64663d0798be4eb3aee2b867 |
| status80100e6c/64 | c3930664c425ccb96a3dab18cad12805c35e5ad88d7da4273e33bc677d9eb6aa |
| _pose_def80100e00/6c | 21690ddded8375908c3f26b57b870183a9e4d72f47a4aaafe0d9b83eb25c1b9f |

Read-only original/local model evidence and scoped documentation diff check.
No native test/build/render/resource-lifecycle equivalence claim or commit.
