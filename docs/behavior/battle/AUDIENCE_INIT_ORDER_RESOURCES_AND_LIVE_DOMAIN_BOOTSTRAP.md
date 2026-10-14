# Audience Init order, resources and live-domain bootstrap

2026-10-13 US G8ME01 disc0 revision0 read-only complete BattleAudience_Init
801a83f8/1c4 and used leaves. Native currently has original-size inline A=B2858
storage; a prior BattleWork memset alone is not completion of this initializer.
Source [initial population](AUDIENCE_SETUP4_INITIAL_POPULATION_AND_POUCH_GATE.md),
[Main owners](AUDIENCE_MAIN_INLINE_OWNERS_AND_ORDINARY_EMPTY_DOMAINS.md) and
[sound bank](../audio/BATTLE_AUDIENCE_SOUND_BANK_AND_ENDING_CLAP.md) remain
composed authorities, with their bounded population/numerical/backend domains.

## Complete ordered initializer request/store graph

Each noargument getter below recaptures its own current global B; original Init
retains its first A and Window V pointers, so callback-mutated global state
must not silently replace their later stores.

| Original site | Exact request/store order |
| --- | --- |
| 801a8408/0c | BaseGetPtr801a86a0→retained A |
| 801a8410/14 | WinGetPtr801a85fc→retained V |
| 801a8418 | pouchGetPtr800d5eb0; returned pointer unused locally |
| 801a842c | memset(retained A,0,13914hex) |
| 801a8430 | BattleAudienceSettingAudience801a60c4 |
| 801a843c/44 | tplRead801a86ac("battle/audience/audience_normal.tpl"); returned WORD→retainedA0c |
| 801a8454 | GuestTPLRead801a6050(index0,count12decimal,name"audience_luigi.tpl") |
| 801a8458 | SetThrowItemMax801a5a0c (always originalirand100) |
| 801a8464 | WORD retainedA13908=0 |
| 801a846c/70/74 | Clear WORD A1378c,13788,13784, in that order |
| 801a8478..84d8 | Right half100-slot existence/state scan; increment cachedA13788 |
| 801a84dc..8544 | Left half100-slot existence/state scan; increment cachedA1378c |
| 801a8550..8568 | Read right+left; wrappingWORDsum→A13784 |
| 801a856c..8580 | Reread retainedA13784, signedint→singleFLOAT→retainedV18 |
| 801a8584 | SoundNoiseAlways801a0b88 |
| 801a8594 | SoundSetVol8019fe28(slot2,target60,duration180) |
| 801a85a0 | WORD retainedA13910=0 |
| 801a85a4 | BattleAudienceNumToTarget801a1c60 |

The initial actors are not inferred from a prior B memset: SettingAudience runs
BEFORE counting, sound and target adjustment. There is no SP8c0 return around
this graph. Physical file/resource and sound requests still execute in that
domain. No item/AP/sound bank pointers are individually allocated here because
their source records are embedded in13914-byte A.

## Actual bank initial state and subsequent used producers

A memset supplies zero for all200 actor records A1bc+i*134: WORDflags0,
BYTE19 state0, typeBYTE1b0 and all other fields0 BEFORE SettingAudience.
Source population may then call Entry801a3400→entryinitializer801a31a8, which
owns actual type/state/position/animation flags. Positive creation cannot be
substituted by assigning flags1 alone. With source rank0/prologue snapshot
P8c0, desired initialcount is clamped0. The subsequent
[exact setup4 operation audit](AUDIENCE_SETUP4_ZERO_COUNT_EXACT_OPERATION_ORDER.md)
corrects the older zero-weight concern: admission counts successful nonzero
categories, so actual setup4 always admits type10, produces positive summed
weight, divides zero numerators normally, and calls no Entry. Its whole source
population graph retains all200 zero actor records while preserving actual
RNG/service/store order. The capacity gate alone is still not this full graph.

All100 item records atAf25c+i*48, all100 AP-source records atA10e7c+i*60,
all24 sound records atA133fc+i*24 and WindowV=A1375c begin fullyzero. The local
initializer does not directly install active item/AP flags. Child admitted
population/event/audio/target producers must retain their writes instead of
being discarded to preserve an invented empty lifetime.

Window initial BYTE0 active=0, WORD14 timer=0, FLOAT08/0c/10 positions0,
FLOAT18 displayedcount0 frommemset; its V18 is explicitly replaced AFTER
actual population/count scan. Main subsequently writes/interpolates real
position/timer even when that displayed amount remains0. WinSetActive801a1cdc
is its separate active-byte writer; source Help/phase/event history may open it.

Each initial half scan counts GetExist(index)low8==1 AND actorBYTE19 neither
0c nor0b. Source order right index20*row+10+column then left20*row+column.
This count differs from PPAudience sound-count helper's excludedstates0c,0d,11
and from NumToTargetSub's excludedstate0c. Preserve each original reader.

SetThrowItemMax always consumes irand100 and writes maximumA137f4 distribution
0(<80),1(80..89),2(90..94),3(95..99), then resets A137f0=0 andA137f8=0.
The max is not a rank, fixedcapacity or automatically0 for SP0. Other Init
clears listed above supply real13908/13910 counters. Source initialRNG,state
mutation/provider order matter even when no audience actor is created.

## Real original TPL acquisition and per-bank metadata

Original strings:802fa710="battle/audience/audience_normal.tpl";
802fa734="audience_luigi.tpl"; Guest format802fa6fc="battle/audience/%s".
GuestTPLRead(index,count,name) captures its arguments, queries A, and for signed
index<2 (no lowerbound clamp) snprintf's the format into local buffer, calls
tplRead, stores returned WORD atA10+4*index and LOW8count atA18+index.
Used index0 therefore installs real Luigibank handleA10, countBYTE A18=0c.
No guestbank1 load is requested locally; A14/A19 retainmemset0 unless another
provider writes them. This is not a12-pointer filename table.

tplRead801a86ac/44 retains inputpath, requests getMarioStDvdRoot80007388,
then fileAllocf8018a61c(type4,format80424a88="%s/%s",root,path), returning the
real file-handle result. There is no localnull/failure repair or fakehandle.
Original cache/filepayload/TPL unpack/render lifecycle remains its own owner.
Native may import converted originalTPL data behind an explicitly typed
file/resource handle; mere atlasname plus fabricated positive ID does not
establish that original availability domain.

Read-only ISO inventory confirms:

| Resource path | Original bytes |
| --- | --- |
| battle/audience/audience_normal.tpl | 428896 |
| battle/audience/audience_luigi.tpl | 39776 |

These resources are requested even when SP8c0/count0. Original texture files
stay outside source history; any actual conversion/import must record its own
choices/licensing in ASSETS. Documentation discovery alone did not extract or
convert them. Parent's current native actor assets do not automatically supply
these separate crowd image-bank/file handles.

## Noise, volume and target adjustment after actual counts

SoundNoiseAlways queries current A and slot2, then live right/left
PPAudience counts. Existing flagsbit1 set skips restart. With livecount0 and
flagsbit1clear it sets categoryBYTE08=0, then still requests Sound1 with a local
unassigned key. Source key/zero-volume backend domain is the same documented
uninitialized-name problem as Clap; no source SP0 silence gate is added here.
Native explicit unavailable-key failure may return-1 as a declared backend
adapter. SoundNoiseAlways stores that returned handle into slot2WORD04; -1
skips active-bit/lifetime/volume setup. Preserve its actual requested failure
history rather than retain a synthetic constructorhandle0 silently.

SoundSetVol(2,60,180) still executes AFTER that attempt. It stores slot2BYTE1f
from priorBYTE1e, BYTE20=60, WORD14=0,WORD18=180,WORD10=0, even inactive/-1.
It does not itself invoke backend audio or clear activeflag. Source background
Aflags2000 is ORed only following a successful NoiseAlways handle!=−1.
Other zero-initialized sound records still retain handle0; later SourceCheck
needs actual positional SFX-slot bootstrap/history, not an assumedinvalid0.

Finally NumToTarget801a1c60 queries current A; flags40000set skips. Otherwise
computes clamped signed target fromfloat1377c+13780 andcapacity13790, calls
NumToTargetSub801a1bf4, then AddAudienceNum801a1708(target-current).
AddAudienceNum captures B,A,pouch, then P8c==0 returns0 BEFORE amount branches,
so canonical actualP8c0 admits a real finaltarget leaf return WITHOUT skipping
all earlier Init. Source currentcount scan and request history still run.
See [existing zero-target authority](UNDERGROUND_GOOMBA_TRIO_SETUP4_AND_RETURN.md)
for packed flags/status gates; this is not WinSetActive801a1cdc.

## Meaningful native implementation path / first unresolved provider

Start with an originalAudienceInit lifecycle call at actual BattleInit point,
using persistent canonical pouch and inline A, not ending-time fake reset.
Bind exact localzeroing/order and actualpacked getters. First transitive
boundary is SettingAudience801a60c4, which must supply actualsetup-derived
capacity/type/RNG/count admission using the exact setup4 operation contract.
Then import originalnormal/LuigiTPL resource availability, preserve source
SetThrowItemMax RNG output, scan real actors, publish windowamount and run
noise failed/unassigned request adapter +realvolume fields; finaltargetP8c0
leaf can use its already documented originalgate. A later ending Main must
operate this retainedhistory, not replay Init to erase active fields.

Positive population, active item/AP/throw services, texture backend, original
RNG state and uninitialized key contents remain explicit providers. A bounded
native original setup4 zero-count graph can prove initial actor existence;
its unavailable-key/backend behavior must still not be presented as measured
original audio silence. Generic allzero type setups retain a separate numeric
provider question. No invented10/50crowd or source
story/party/pouch grant is authorized by this initialization gap.

## Evidence

| Complete raw original body | SHA256 |
| --- | --- |
| Audience_Init801a83f8/1c4 | a926eae5ff02962f79d66e063dc5a345edfceb4e58130f38a13e65db79d25d99 |
| GuestTPLRead801a6050/74 | b34d46c4ca66a2c36c02eec6b7c57c5ca0c03f18b0528c728616353894b710c2 |
| tplRead801a86ac/44 | c92a924a1d3830b6bceba1544f835bbbc2363cdec2a9df5441aa3394d847b803 |
| NumToTarget801a1c60/7c | 30e68a9f7d1c3c49a6acbe8d03aa2cb19ad25523b7aaad25accb4509ff6b7bd3 |

Complete evidence under ignored artifacts/recovered-temp-evidence/audience-init-*
and audience-BattleAudience_Init.dis. Checked completeorder, inlineclears,
actualresource strings/read-only inventory, scan/exclusion differences and
source slot2/target provider chronology. Documentation only; scoped diffcheck
passed; no native/shared docs edits, assetconversion, build rerun or commit.
