# Part pending-pose modes and automatic status selection

2026-10-14 US G8ME01 disc0 revision0, read-only original source. Used btlDispMain
continuation after the [forced pose consumer](FORCED_PART_POSE_DISPLAY_RATE_AND_ANIMATION_FRONTIER.md)
and [real per-part factory](../display/BATTLE_PART_POSE_ENTRY_RELEASE_AND_OFFSET_SERVICES.md).
Owned new contract only; no native/shared docs/index/request/code changes.

## Pending-name consumption after animPoseSetAnim

This whole region executes ONLY after actual P204 bit1 admits the pending-name
branch and the original SetAnim call returns. Bit1-clear parts jump directly
to80102b68 without clearing/recomputing BYTE P214/P215 or flag40.
At80102a44 source sets temporary wantedEffect=0 and BYTE P214=0. Then the exact
ordered logic is (flags reloaded after the shown services/stores):

```
P214 = 0
wantedEffect = false
if P204 & 4:
    P215 = 0
else:
    P215 = 1
    if CheckStatus(U,18) != 0 or U8(P4f3) != 255:
        P215 = 7
        wantedEffect = true

f = fresh P204
if !(f & 2) or (f & 8) or (f & 0x80):
    P215 = 1
    if CheckStatus(U,18) != 0 or U8(P4f3) != 255:
        P215 = 7
        wantedEffect = true
    if CheckStatus(U,7) != 0:
        P215 = 4
        wantedEffect = true

if wantedEffect and (P1ac & 0x8000):
    wantedEffect = false
    P215 = 0

if !(fresh P204 & 4):
    if wantedEffect and !(P204 & 0x40):
        P204 = fresh P204 | 0x40
    else if !wantedEffect and (P204 & 0x40):
        P204 = fresh P204 & ~0x40
```

CheckStatus calls have ordinary args(r3=U,r4=18 or7), source target80127814.
The color gate is the unsigned BYTE at P4f3, the alpha lane of source color
WORD P4f0; it is not unit U311/U312 alpha. Status18 is checked twice when both
sections admit it; don't deduplicate live queries. Status7 is checked only
inside the second section. Source only suppresses the admitted effect when
part attributes8000 set; it does not unconditionally suppress the pose itself.

Concrete normal forced case after source pending flags3 are consumed:
flags become2, partcolorffffffff, CheckStatus18=0; P215=1,P214=0,
wantedEffectfalse, no flag40 store if alreadyclear. _pose_def later therefore
admits its real SetEffect(handle,0,0) operation. Force8 or prior80 changes
the second-section admission and must remain real. Flag4 set can leave P2150
and skip finalflag40 update, rather than being normalized to mode1.

At80102b68 regardless of whether name consumption occurred, call in order
gravityOffsetControl80101060(P), floatOffsetControl80100f34(P),
statusPoseControl80100e6c(P), PartsWorldPos80128b10(P,&XYZ),
animPoseMain8003ed2c(P1c0). Their complete ordinary counters/dispatch/SetEffect
frontier is the linked display contract. This is not completion of whole Main.

## Complete _GetStatusPoseType80103084/74

Input ordinary r3=unit U. First read U104; mask20000 set returns raw27 WITHOUT
CheckStatus calls. Otherwise scan actual WORD table80416ab0 in ascending order:
27,2,9,1,4,5,3,-1. For each nonterminator, CheckStatus80127814(U,signed8(id));
first nonzero returns the original table WORD. Terminator-1 returns28.
No table length/status sorting/RNG/pose resource read. Source CheckStatus
includes its real storage/extra-lane behavior. Do not substitute a hardcoded
28 just because a cached no-status assertion existed at battle entry.

## Subsequent automatic branch when P204 bit2 is clear

Part posehandle P1c0==-1 skips before this graph; actual valid handle0 is
ordinary. Initially retain namePointer=P+1c4. Then ask _GetStatusPoseType(U).
Source unit kind U8 read after that request controls the following selection:

* signed kind1..215: searchPoseTbl80103cd0(P1bc,statusType), retain returned
  namePointer; jump directly to status19/speed processing. This enemy-range
  path skips the later statusflag01000000/CanAct override. Frankly180 belongs
  to this range even though he is a scripted helper actor.
* Other kinds (Mario224,Goombella225 included): if type28, CheckStatus(U,22).
  When false and U138&20000000, GetPoseNameFromType8012772c(P,57);
  null result falls back searchPoseTbl(P1bc,28). Otherwise proceed to the next
  gate: if type28, CheckStatus(U,22) AGAIN; false and U138&10000000 requests
  GetPoseNameFromType(P,56), null→searchPoseTbl(P1bc,28).
  If neither specialized selection admits, searchPoseTbl(P1bc,statusType).
* After this player-range selection: CheckStatusFlag801277f8(U,01000000).
  Nonzero then CanActStatus8012724c(U); nonzero overrides retained namePointer
  with GetPoseNameFromType(P,45), with no null-fallback branch at this site.

All automatic kinds then CheckStatus(U,19). Nonzero chooses btlDispAnimeSpeed
80100a4c(P,2.f). Otherwise CheckStatus(U,20); nonzero chooses.5, otherwise1.
Constants80422250=40000000,80422234=3f000000,8042224c=3f800000.
The forced bit2 path instead immediately requests speed1 without these two
status checks. Exact rate setter/pose-owner dependencies remain their contract.

The automatic selection does NOT strcpy the returned name to P1c4 and does
NOT compare names to set bit1. It only retains a pointer used by animPoseSetAnim
IF the later fresh P204 bit1 is already set. If bit1 clear, selected name is not
applied here. Do not turn every automatic lookup into an immediate animation
restart or fabricate a request flag absent from source producer history.

## Actual extra Mario parts with source defaults

Original Mario has3 source parts. IDs2/3 attributes13010000, pose table80417bc8
type28 KRN_H_1, copied initial P1c4 KRN_H_1. They have actual shared b_mario
resource allocations with initial descriptor0 M_Z_1; the directory has no
KRN_H_1 descriptor. Fresh mapped part construction gives P2040 until a real
source producer changes it; copying the idle-name buffer does not set bit1.

For current no-status/U10420000clear, the automatic helper checks the original
seven statuses and returns28. If real player specialized statusflags areclear,
searchPoseTbl returns authoredKRN_H_1. Real statusflag01000000 and19/20 gates
are still read. With flags204 still0, speed1 runs but pending-name consumption
does not: no SetAnim, no P214/P215 rewrite. Thus descriptor0 remains selected,
and statusPoseControl sees its actual initialized mode0 rather than an invented
mode1. Subsequent world-position/Main and attribute-dependent draw branches
remain original active providers. Do not remove the parts or create KRN art.

## Evidence and validation

Original complete _GetStatusPoseType80103084/74 SHA256
cb3b3eb0cb1e2d5c86dfb3b79cb5f26e39ec00630aebd2e6a3da0a96ddec6a62.
Priority table80416ab0/20 SHA256
f803958fc7151d87d2e191f3f270d6f121ac45dd6a2f545827e1b8218ff5e0eb;
pending-mode span80102a44/124 SHA256
521675fe0fbfbe978ee7d3e9572b18641f874e3660d48244d0c2826bf1140c9e.
No instruction execution/render equivalence is claimed. Complete containing
btlDispMain801024f0/b94 and its ignored ending-next-btlDispMain.dis remain the
authority. New ignored helper listing part-auto-statuspose.elf.

Scoped source store/mask/call-order inspection and documentation diff check
only. Native build/testing/provider implementation remain parent-owned.
