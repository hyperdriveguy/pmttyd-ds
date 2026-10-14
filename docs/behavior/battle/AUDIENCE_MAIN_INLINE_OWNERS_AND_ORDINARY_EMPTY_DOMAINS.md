# Audience Main inline owners and ordinary empty domains

2026-10-13 US G8ME01 disc0 revision0 read-only original DOL. Next ending
provider after [StageMain](STAGE_WORK_INITIALIZATION_AND_INACTIVE_COUNTER_PASS.md)
and [AcHelp Main](SPOTLIGHT_MUTABLE_COLOUR_BOOTSTRAP_AND_AC_HELP_ADMISSION.md).
This closes complete outer ordering/owners and bounded empty subservice paths,
not a SP0 early-return replacement or whole audience simulation claim.

## Correction: 801a1cdc opens the window, not NumToTargetSub

Actual symbol/body801a1cdc/30 is BattleAudience_WinSetActive. True
BattleAudienceNumToTargetSub is801a1bf4/6c. Earlier top4 provider prose misnamed
801a1cdc; this source correction takes precedence. The uncommitted AcHelp
contract was corrected separately. WinSetActive retains rawinteger argument,
requests WinGetPtr801a85fc, then stores LOW8 argument into returnedWindowBYTE0
at801a1cf4. No crowd amount/target adjustment occurs in that body.

Thus AcHelpSetDispType mode0 sends WinSetActive(1), nonzero sends(0).
AudienceMain's conditional WinSetActive(1) similarly opens its actual window;
neither call invokes target/count allocation. No fabricated crowd-target
provider is appropriate for that address.

## Complete Main order with captured versus current owners

Audience_Main801a836c/8c first captures CURRENT global B8041eb90 in r30 at
801a837c, then BaseGetPtr801a86a0 returns CURRENTglobalB+2858, retained A in
r31. B and A are separate captures; service-mutated global state does not
recapture these outer local owners.

Read A.word0 at801a8384. If mask10000 SET, skip conditionalwindow request.
If clear, read retainedBef4; only mask80 SET requests WinSetActive(1) at
801a83a4. The leaf queries its own CURRENT window owner. Then ALWAYS call:

| Site | Request | Complete body size |
| --- | --- | --- |
| 801a83a8 | AudienceCtrlProcess801a5ca8 | 3a8 |
| 801a83ac | AudienceItemCtrlProcess801a4f0c | 958 |
| 801a83b0 | AudienceApSrcCtrlProcess801a4998 | 574 |
| 801a83b4 | AudienceWinCtrlProcess801a4570 | 1e4 |
| 801a83b8 | AudienceSoundMain8019f97c | 1e8 |

Returned values are ignored. There is no state/pouchSP/emptycrowd early exit.
After all5, freshly reread retainedBef4 at801a83bc. Ifmask80 SET, reload
retainedA.word0, OR10000/store at801a83c8/d0. Clear instead reloads thatword,
clears ONLY10000/store at801a83d8/e0. Earlier field/callback mutations survive
because flags are reread; finalouterA is not replaced with then-currentGlobalA.

## Actual inline owner map

Each getter independently queries BaseGetPtr (CURRENTglobalB+2858); no
allocator/null/bounds check locally. Relative to returned A:

| Getter / address | Result |
| --- | --- |
| AudienceGetPtr801a8664 | A+1bc+slot*134, 200 actor records |
| ItemGetPtr801a8624 | A+f25c+slot*48, 100 item records |
| WinGetPtr801a85fc | A+1375c (B+15fb4 for unchanged B) |
| SoundGetPtr801a85bc | A+133fc+slot*24, 24 sound records |
| AP-source local traversal | A+10e7c+slot*60, 100 records |

The native inline B2858 storage domain now matches this source ownership.
Copied local standalone work used by an older audience adapter must not
silently replace original packed pointers as setters/callbacks run.

## CtrlProcess: empty actors still recount and gate throw requests

It captures B, queries A, zeros WORD A1378c/13788/13784 (in thatorder) at
801a5cd4/d8/dc. Scans right then left100 slots each through GetExist801a29c0.
Counts only low8 existence exactly1 and actorBYTE19 neither0c nor0b; updates
right13788 andleft1378c, then sum→13784 at801a5dc8. These cachedcounts differ
from sound PPAudienceNum_Sub's excludedstates0c,0d,11; preserve each reader.

GetExist reads actorflagsWORD0: bit1 mustbeSET and masks100/2 bothCLEAR.
No flags/actors are created by this countpass. It then gets actor0, scans200
records stride134. Actorflag10 selects local verticalposition/velocity update
regardless of existence; otherwise GetExist0 skips typeAI dispatch80192a0c.
Therefore a bounded allnonexistent AND flag10clear bank admits no actorAI/motion
services, but countzero writes and tail stillrun.

Tail at801a5f00 reads retainedB19078. Mask20000 sets A137f4=10000. Then:

1. Retained Bef4 mask80clear skips the remaining throw/RNG tail.
2. Otherwise read signed A137f0 andA137f4; f0>=f4 skips it.
3. Admitted f0 value0/1/2 selects RNGbound1800/6000/30000; unsupportedvalues
   retain localbound fromsource. B19078 mask20000 replacesbound60.
4. irand(bound); nonzero skips. Zero builds/shuffles200 indices using1000
   swaps(twoirand200 each), then tries ThrowItemEntry801a5aac(index,0,1)
   in resulting order until a return1; success increments A137f0 once.

This tail has no SP8c or countzero guard. Nonempty throwing/state/effect helpers
remain real providers if admitted, rather than pretend allinvalid indices
succeed or all200 attempts fail without their original leaves.

### Actual Init throw limit cannot be assumed0

Audience_Init calls SetThrowItemMax801a5a0c at801a8458 even with no crowd.
That helper queries A and irand(100), then writes A137f4:
<80→0,80..89→1,90..94→2,95..99→3; draw>=100 leavespriorvalue. Finally it
writes A137f0=0 andA137f8=0. Source sourceRNG/history determines currentmax;
SP8c0 does not imply f4=0. Native suppliedsourceinit must retain this request
and actual output, with deterministic nativeRNG being an explicitly declared
adapter if it is not the original RNG state. Full later max/producers remain
owned separately.

## Item and AP-source used inactive admission

ItemCtrlProcess captures B, queries A, calls ItemGetPtr(0), then scans100
records stride48. Each itemWORD0&1clear skips ALL item services/writes.
Activeitems resolve an owneractor and enter originalstate/motion/hit/clear
graph; those cannot be converted to absent merely because actors areempty.
Audience_Init's actual13914 memset supplies initialitemflags0. Real item
producer/deletion history must preserve that domain for an empty itempass.

ApSrcCtrlProcess queries A and tests A.word0 mask2. CLEAR returns immediately
at801a49c0→4ef0, after the base request and before all AP-source/sound work.
Set scans100 AP-source records and enters actual originalsound/status/curve/
pouch addAP providers. A sourceemptyactorbank or SP8c0 does not by itself
prove A.flags2clear; the actual flags publisher remains required.

## WinCtrlProcess: real scalar/timer/position writes even count0

It queries A then WinGetPtr, retaining Window V. Read signedactualcount
A13784→floatC, read V.float18 displayedcount, compute singleD=C-V18.
D exactly0 skips amountadjustment. Positive D adds max(.1,.1*D), negative D
subtracts max(.1,-.1*D), all with original single rounding. Afterwards fresh
A13784 is read again for crossing clamp to actualcount. This is a follower of
retained displayedfloat, not a direct reset to cachedintcount everytick.
Nonfinite/negative anomalouscount domains remain original numericproviders.

Then ALWAYS process V.BYTE0 active:

* Nonzero: readWORD14, wrappingadd1/store; freshlyreadnewvalue signed>15
  clampsWORD14=15. Negative counts are not lower-clamped on this branch.
* Zero: wrappingdecrement/store; fresh signednewvalue<0 clampsWORD14=0.
  Values>15 are not upper-clamped on this branch.

At801a4728 calls intplGetValue80006c8c(mode4,readWORDtimer14,duration15,
FLOATstart-500,FLOATend-280). Returnfloat→V08 at801a472c. Then V0c=-160,
V10=0 at801a4738/3c. These are fullFLOAT stores, not bytes/roundedintegers.
Original literal addresses80424a30/34/38 and80424988. The FLOAT mode4curve
is required when timer changes; don't reuse integer intpl_sub squared helper
or claim no windowupdate because count0. With amount0, V18=0, active0,timer0,
this stillwrites ordinaryXYZ(-500,-160,0). With actual WinSetActive1 from
StopAC/Main, timerincrements and originalpositioninterpolation isadmitted.

## SoundMain and next smallest faithful binding

[Sound bank/history](../audio/BATTLE_AUDIENCE_SOUND_BANK_AND_ENDING_CLAP.md)
owns its full24record/Main/NoiseMain chain and realhandlecheck/volume services.
Allrecords are checked regardless of audience-sound flagsbit1. A real failed
unassigned-key request handle-1 is the documented nativebackendfailure adapter,
not proof every oldzero handle isinvalid; actual public SFX-slot ownership and
liveness still supply its backend. No SP0 audioMain skip appears here.

The next faithful completion increment can bind this exactouter Main and
getter/WinSetActive leaves, CtrlProcess actualcount/inactiveactors and guarded
throwtail, Item flags-based inactivebank, AP flag2clear branch, Win amount/timer/
mode4position and SoundMain existingactualrecord/backend adapters. Any admitted
throw, activeactor/item/AP or unresolvedsound/curve provider must retain its
original address as nextboundary. Whole AudienceMain cannot be marked source-
complete simply by observing initialdesiredcrowd0; scalar/flags/UI/audio
history remain operative on that domain.

## Evidence

Complete original raw DOL SHA256:

| Body/address/size(hex) | SHA256 |
| --- | --- |
| AudienceMain801a836c/8c | 42db3ae62cdf99511f6b4bcf72933bcda669c62dd1b27a0f09c7183b0811993a |
| CtrlProcess801a5ca8/3a8 | 626fcc2ecfd480f224dd1d02f56d4fe26e48e447c71bcfdd30f3972437e5135e |
| ItemCtrl801a4f0c/958 | 5e30b324f7fff2f6ad60bdb9756b2c248407af39f7edc8cfb6aef9c323e6ce82 |
| ApSrcCtrl801a4998/574 | 18b4d7706db37eb21d93b760a52fbb1a9f606830f81ddf6a23d09452efbd78b2 |
| WinCtrl801a4570/1e4 | 9d34c9d5dc54db93371924e8b874cd5780adf3463e8cac38d050708b24e1093a |
| WinSetActive801a1cdc/30 | 33a3b7999c1fbf527f0fc10ddaf665d0c21e1bd2d2e43b481d2f59cc8c07896b |
| ThrowItemMax801a5a0c/a0 | dfebbb471c2cb5e209ba0a37dcf78381ceea2264b16a70c279e060106d0a48dc |

Complete evidence under ignored artifacts/recovered-temp-evidence/audience-main-*
and ending-top4-BattleAudience*. Checked completeouter captures/5call order/
finalflags, inlinegetters, conditionalleaf identity, usedemptygates and real
windowtimer/position outputs; nestedactive providers remain bounded requests.
Documentation only; no code/shared docs edits, build rerun or commit. Scoped
diff check passed; no SP0 unconditionalskip or originalsound/playback claim.
