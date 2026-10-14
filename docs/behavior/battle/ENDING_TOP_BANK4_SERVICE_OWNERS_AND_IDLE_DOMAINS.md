# Ending top-bank4 service owners and bounded idle domains

2026-10-13 US G8ME01 disc0 revision0 read-only DOL audit. Complements the
[complete Main order](BATTLE_MAIN_SERVICE_ORDER_AND_FINAL_STATE_CHECK.md),
[input packet cadence](../input/BATTLE_INPUT_CADENCE_AND_SNAPSHOTS.md) and
[ending tail](UNDERGROUND_TRIO_NO_LEVEL_UP_ENDING_TAIL.md). This identifies
used provider domains; it does not replace seven services with assumed no-ops.

BattleMain800f8e28 captures B before its ordered11 requests. Its topbank4
return0 is checked only at800f8e74..88 AFTER stage/icon/help/audience/slot/draw.
The callee services can recapture current global B independently. No topbank4
admission skip or pouch8c shortcut appears in the seven bodies below.

## Stage objects: actual selected-map owners

BattleStageObjectMain80149798/438 recaptures B and scans32 records at
B1715c+i*7c. Signed recordWORD0<=0 skips that record; positive enters. The
trio's original five selected objects are initialized active by source setup;
zeroing these records would erase original map ownership. Its current/cache
position04..0c, offset10..18, rotation1c..24 and additional translation28..30 comparisons
update corresponding cached34..60, and original map-object provider calls
apply changed transforms. Flags68 and objectname/ownerpointer64 determine
further map flag/animation consumers. Final unconditional80149bb8 requests
80149794, whose complete body is blr; it is not a readiness barrier.

Actual selected root/model transforms and source hierarchy are already in
[stage contract](UNDERGROUND_SETUP4_INTRO_PHASE_STAGE_AND_PLAYER_COMMANDS.md).
The smallest native pass must retain its actual5 objects/map owner and use
that transform adapter, with flag/animation services explicit when admitted.
All32 WORD0<=0 is a source inactive domain but is not original setup4 history.

## Battle icon records: bounded empty domain

BattleIconInit801aa228/34 queries global B and memset(B182cc,0,9c0).
That is16 records of9c bytes. Main801aa168/c0 iterates all16; each is skipped
unless U16 record0&1 AND recordWORD1c nonnull. For admitted records it requests:
iconSetPos80019644(handle+18,recordfloat08/0c/10),
iconSetScale8001944c(handle+18,single(float14*float18)), then icon flag2 on/off
800195a4/80019504 according U16 record0 mask2. Main owns no per-icon creation.

This original initialization supports a real empty bank and a predicate-based
Main pass. Any actual status/icon producer/deletion history must be retained;
EXP's separately owned19 icons are not automatically these battle iconrecords.
Do not clear a live record only because ending bank is4.

## Stage animation/work: flags and mandatory spotlight owner

BattleStageInit80139780/e4 memset(B163fc,0,b3c), then initializes12 entry
records at W+4+i*e8, setting each recordWORD0 andWORDe4 zero. It initializes
ambient/background records W+ae4 andW+b04 active1 with source colour/scalar
data, and calls StageObjectInit80149bd0 then batSpotInit80138068.

StageMain80139150/630 captures W=B163fc and ALWAYS calls batSpotMain8013800c
first. batSpotMain reads pointer804175a0 (r13-5980hex), loops its WORD0 count
without changing any record, then WORD8>0 queues display callback80137d20 via
dispEntry80010bc4(camera6,class0,user0,sourcepriority). Original batSpotInit
sets count5, allocates5*2c bytes via BattleAlloc and initializes spotlight
records; its object/light consumer remains a real provider, not a null owner.

Each of12 StageMain entries: recordWORD0&1 clear skips animations, but if
recordWORDe4 nonnull it BattleFree's it and clears the pointer at80139590/598.
Thus even inactive flags may own cleanup. Active entries own counter/colour,
position/rotation/scale interpolation and memory lifecycle. After entries,
positive W+af0/af4/b10/b14 counters decrement and request source intplGetValue
80006c8c, then write output bytes. Allinactive/nullscratch and allfour counters
<=0 skips those updates; spotlight pass still executes. Source init zeroes
counters, but later original stage/attack effects must retain their writes.
Pouch8c does not gate StageMain.

## Action-command help: exact empty/display-off gate

BattleAcHelpMain802168a4/64 recaptures B. B19008 WORD key0 OR B1900c signedWORD
mode0 OR Bef4 mask2 set skips all display requests. Otherwise it registers
callback80216728 via dispEntry(camera8,class1,user0,prioritysource804272b8).
BattleAcHelpInit80216908 is a4-byte blr; the original19098-byte BattleWork
memset, not this helper, supplies initialzero words. SetHelp80216954 and
SetDispType8021690c are later writers. A native retained mode0 after actual
source StopAC/menu/death requests admits a real display-off pass, without
pretending an inherited nonzero help key never existed.

## Audience Main: no prologue SP shortcut

Audience_Main801a836c/8c captures B, queries A, and if A.flags10000 clear and
Bef4&80 set calls AudienceNumToTargetSub801a1cdc(1). Then it ALWAYS calls:

| Order/site | Request |
| --- | --- |
| 801a83a8 | AudienceCtrlProcess801a5ca8 |
| 801a83ac | AudienceItemCtrlProcess801a4f0c |
| 801a83b0 | AudienceApSrcCtrlProcess801a4998 |
| 801a83b4 | AudienceWinCtrlProcess801a4570 |
| 801a83b8 | AudienceSoundMain8019f97c |

Finally rereads Bef4&80 and ORs/clears A.flags10000 at801a83c8..e0. Neither
pouch8c0 nor all200 nonexistent positions skips those calls.

Bounded empty subdomains actually observed locally:

* CtrlProcess first clears/recounts A13784/88/8c through all200 original
  existence/state gates. It then walks each record. Recordflag10 clear and
  GetExist0 skips actor motion/AI calls. Tail still reads B19078 mask20000
  (can write A137f4=10000), then Bef4&80 and A137f0/f4 gates admission of
  original random item throw. No empty-crowd shortcut appears before that tail.
* ItemCtrlProcess gets item0 and scans100 records, stride48hex, skipping only
  itemWORD0 bit1 clear. Initial audience memset supplies zeros; producer history
  must keep them inactive to close this pass.
* ApSrcCtrlProcess returns immediately when A.flags mask2 clear. Set invokes
  separate AP-source/sound/interpolation owners even with no people.
* WinCtrlProcess always compares actual countA13784 to retained windowfloat18;
  zero delta skips that adjustment but still processes window timer14, invokes
  intplGetValue(mode4,timer,15,sourcefloats), and writes window08/0c/10. It is
  not empty-crowd no-op.
* SoundMain's original24-slot liveness/volume loop is in
  [sound contract](../audio/BATTLE_AUDIENCE_SOUND_BANK_AND_ENDING_CLAP.md).
  Slot failed/unassigned backend requests and explicit native unavailable-key
  adapter remain their established history, not a Main-global silence flag.

Population/8c-zero boundaries remain
[initial audience contract](AUDIENCE_SETUP4_INITIAL_POPULATION_AND_POUCH_GATE.md).
Next missing used numerical/window/ramp/spotlight providers must be retained
explicitly rather than converting this request graph to a success stub.

## Break slot: state0 still writes all three reels

BreakSlotGetPtr8020524c/10 = current B+1616c. ReelGetPtr80205210/3c queries
that owner and adds18hex+index*5c. Original Init8020507c/194 zeroes134 bytes,
sets controllerWORD4=0, assigns each reel ordinalWORD0 and reelWORD8=0.
It allocates/copies reel data arrays via BattleAlloc:8,8,5 WORDs. Allocations
and data pointers survive the idle pass; do not substitute absent reel owners.

Main80203aa0/15dc first requests AudienceBaseGetPtr, BreakSlotGetPtr,
BattleGetMarioPtr800f87a8(B), BattleGetPartyPtr800f8728(B). Controller state0
counts3 reelWORD8==2; if count!=2 skips control-state mutation. Afterwards
it always iterates three reels. Each reelstate0 dispatches802043fc: read
reelflagsWORD4, clear bit1, store flags at80204408; set BYTE58=ff at8020440c,
then advance. This is a complete used original allthree-state0 scalar pass;
nonzero reel states own their original data/interpolation/effect/mapping graph.
Source state0 does not reset pointers, ordinal, countdown or controllerstate.

## btlDispMain: survivors and unconditional draw footer

btlDispMain801024f0/b94 captures B and iterates64 unit slots by BattleGetUnitPtr
800f88cc. Null skips that slot. Surviving Mario/Goombella/Frankly source actor
parts keep their original unit draw gates/pose/matrix/queue requests; deleting
allthree enemyregistry entries does not imply an empty unit draw pass.
Original class1/2 camera4 and transforms are in the stage/display contracts.

After the64-unit loop it ALWAYS requests, in order:
BattleStockExpDisp8010045c, StageDisp80139084, CommandDisplay80122870(B),
Audience_Disp801a6c18, BreakSlot_Disp80203a68. StageDisp always obtains B420's
unit and queues callbacks80138da8 and801389a0 in camera4,class2. BreakSlot_Disp
always queues802036d4 camera8,class0. Queueing is not proof eventual drawing
has admitted active state; adapters must preserve actual registration and
callback readiness semantics. No topbank4/pouch8c skip encloses this footer.

StockExpDisp's local scalar update precedes any drawing gate: when signed
Bf00>Bf04, increments Bf08; if newBf08>=4 increments Bf04. It does not reset
Bf08 in this body. Bef8&2 then gates source camera8,class1 callback80100258.
Finally btlDispMain individually clears Bef4 masks10000 then20000 if set,
using fresh rereads/stores at80103048..6c. Those real flags side effects remain
even when drawing is implemented through an explicit native render adapter.

## Evidence and scope

Complete original raw DOL SHA256 (sizes hexadecimal):

| Body/address/size | SHA256 |
| --- | --- |
| StageObjectMain80149798/438 | f54092d325edbfc3f19bc7571087be134bc24f9b468040665ee79bb3870c79f4 |
| IconMain801aa168/c0 | ccdd8867bed5ceb45a2d9de9aef1b3b081f2a3d6e1ca1a10196035342e65c6d9 |
| IconInit801aa228/34 | 459023ba635654a1ad9761d3290a6ff36cd5f2f7dfd722162e900b9091d95930 |
| StageMain80139150/630 | f61083bae0f1b5fcb95e3490e3fa3216a3f34ebe9838dd5bf1440a1ee11f62e5 |
| StageInit80139780/e4 | c692d3c176e90df5800a44f8416eb8f1ad5772a59318f60756544db5af0e6bf2 |
| AcHelpMain802168a4/64 | ac32c584aebdfe843e27b481f92a6cb19d43d3f0766a769394f8135809ebb320 |
| AudienceMain801a836c/8c | 42db3ae62cdf99511f6b4bcf72933bcda669c62dd1b27a0f09c7183b0811993a |
| BreakSlotMain80203aa0/15dc | 3d1402e7ea435de74ede23e837a7f2252544e3f3d58179ba8a1c3365c4dec01b |
| BreakSlotInit8020507c/194 | 0dc91abdce67abf87e0c2808852f078071579933c644d3e3723edd7cd505848f |
| btlDispMain801024f0/b94 | 6281c04f5a9784643665fd82f9f0bbc37f053f52fb2cdf672076049a4c17802f |

Complete listings under ignored artifacts/recovered-temp-evidence/ending-top4-*.
Local admission/initializer/used scalar order checked; nested active/transitive
callbacks remain bounded providers listed above. Documentation only, scoped
diff check passed, no native/shared docs edits, build rerun or commit. Topbank4
late return is closed at Main level; the whole seven-service native completion
pass is not claimed source-complete by this bounded provider audit.

## Ending1f exact top-bank write: selector0 means B+4

Urgent integration follow-up, same read-only original source. Complete
btlseqEnd80215234 retains incoming B in r26 at80215248. Its ending1f free/
publication prefix at80216614..30 is:

| Site | Exact original operation |
| --- | --- |
| 80216614 | r3=WORD(retainedB+f28) |
| 80216618 | BattleFree800f8908(r3) |
| 8021661c | r0=0 |
| 80216620 | r3=retainedB (mr r3,r26) |
| 80216624 | WORD(retainedB+f28)=0 |
| 80216628 | r4=0, sequence selector |
| 8021662c | r5=4, sequence value |
| 80216630 | BattleSetSeq800f8a08(B,0,4) |

BattleSetSeq table8034c6d0[0] selects800f8a28, whose operation is
`stw r5,4(r3)`. Consequently the correct native packed publication is
BattleSetSequence(B,0,4), producing WORD(B+4)=4. It does NOT store WORD(B+0)=4
and does not alter adjacent HALFWORDS B0/B2. The phrase top bank0=4 uses
selector0, not byteoffset0. A native literal write(B,4) to offset0 would
corrupt the flags/counter halfwords and leave the top sequence at its prior
value, so the later manager/completion pass would follow the wrong path.

Original raw bytes80216614/20 SHA256
029cc70262d5bf1eed101afe2180f5e45633961bfa1734c06131c6ec5fabaca9.
Complete setter listing under ignored artifacts/recovered-temp-evidence/
ending-top4-BattleSetSeq.dis; its source selector mapping is also the existing
[sequence-leaf authority](BATTLE_SEQUENCE_FIELDS_AND_INIT_SOURCE_BINDINGS.md).
No native patch or source logic inference made; scoped documentation diff passed.
