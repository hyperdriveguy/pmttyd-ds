# Underground trio victory controller and field return

2026-10-13 documentation-only source delivery for the native stop at
btlseqEnd80215234, end bank7 state07000000/Bf24. Read-only US DOL body
size1470, listing `/tmp/trio-para-end.dis`; original32-entry jump table803b0c88.
This establishes source owners and the smallest next native steps. It does
not claim complete presentation, reward, audience, pouch or teardown binding.
Use the [trio setup](UNDERGROUND_GOOMBA_TRIO_SETUP4_AND_RETURN.md),
[original conclusion/barriers](BATTLE_WAITS_AND_CONCLUSION.md),
[result priority](UNDERGROUND_TRIO_TARGETS_TURNS_AND_VICTORY.md#exact-victory-result1-and-completion-publication)
and [field entry/return](../field/FIELD_BATTLE_ENTRY_AND_RETURN.md).

## Owner chain and callback domain

Let F=original field work returned by fbatGetPointer8004829c (GP168),
I=F20, B=current battle work, S=I+c=NPC+230 source setup work,
E=Bf28 allocated end work, P=pouchGetPtr. These are distinct owners.
BattleInit800f8f04 stores its incoming I pointer at B2738. Original
battle_init800738ac calls BattleInformationInit(I), then writes I+c=NPC+230
through BattleInfomationSetBattleSetupInfo80114510. NPC setup4 source work
S owns group S18, stage S0, original flags/audience/item/drop fields; it is
not the Field information I and not the raw44-byte area configuration.

F3c is I1c, a separate optional source end-event stream pointer checked only
after result1. It is not NPC setup S3c, which belongs to selected item-table
pointers. Original npcInit8004bb70 allocates580 bytes at GP168 and memset
clears the entire F at8004bc18, establishing initial F3c=0. The used
BattleInformationInit80114518 clears fields0,4,8,c,10,14 and bytes18..1b,
and **preserves I1c**. npcSetBattleInfo/setup initialize the NPC230 block;
they do not install F3c and do not copy raw configC18 into it.

The original161-command trio scene sets battleinfo4 and starts/waits the
battle; it has no authored end-event installation command. Read-only tik REL
relocation census finds no import of fbatGetPointer and no nonstack stw+3c
instruction in its text. Fresh initialized F3c=0 therefore follows the used
preserved path; a native caller must preserve the actual field work value.
This bounded constructor/used-path evidence is not permission to replace
any nonzero callback with null. General nonfresh/other-map F3c producers and
their pointer lifetimes remain an explicit inbox request.

F3c nonzero selects07000003. State03 calls evtEntry(F3c,10,0), stores entry
ID15c→F40, advances04. State04 waits evtCheckID(F40)==0, then requires the
full original owned-event barrier. F44 nonzero returns to turn bank3 state
03000001 and top bank0=2; zero starts victory05. Do not skip this alternate
resume or fabricate an authored setup4 callback.

## Earliest fresh victory path

Every btlseqEnd invocation captures Bf28/I and P, then calls
`_audience_Whistle_control`80212890 before dispatch. Null E is a real no-op.
With E nonnull and signed E0 in07000007..0700001b, decrement E2a0; a negative
value resets it to irand(120)+60 and requests BattleAudienceSoundWhistle.
The controller always reaches btlseqEnd_DispMain80215088 with (8,B) on its
ordinary common exit; this display service is a separate dependency.

State00 first btlsubResetMoveColorLvAll80125398, Mario/belong/enemy-side
queries, then original result-priority selection. Fresh enemy conclusion
and no override/own-loss yields result1 through setter80114508 at8021532c.
No story word, HP, stat maximum or field-return sequence is changed here.
Result1/F3c=0 then calls audience-end admission8021290c, sets bank7=07000005,
launches the Koura-deletion child, allocates2ac bytes with BattleAlloc800f8934,
stores Bf28=E and calls btlseqEnd_DispInit802151b4. Explicit initialized fields:
E0=0,E29c=0,E2a0=irand(120)+60,E2a4=0. Do not invent a zeroed allocation
or omit this original RNG request merely because the native audience is empty.

`_check_audience_battle_end_exec`8021290c uses original group S18, not the
live/deleted registry. First original unit kind level byte0e greater than
signed pouchlevel8a+1 requests GreatVictory8019effc. Sum all original group
kind levels; threshold=divw(divw(2*sum,level+1),3). Signed battle turn B0 below
threshold requests FastVictory8019efa4(float(threshold-turn)). Trio first
level7, sum7+8+7+1=23 and explicit source pouchlevel1 give threshold7.

GreatVictory computes f1=single(0.149999991*A13778),f2=+0;
FastVictory computes single products in order0.01*delta,5*that,A13778*that,
f2=+0. Both call BattleAudienceAddTargetNum801a1a74. Empty native audience
does not skip these source calls: even zero incoming amount executes original
target adjustment/clamps. The latter consumer and current A13778/1377c/13780/
13790 ownership must be bound or remain pending, rather than assuming return2.

Exact original coefficient addresses:80424988=+0;80424998=0.01;
8042499c=5;804249a0=0.15. Read their original single-precision words,
including0.009999999776 and0.149999991, rather than replacing their ordered
products with a differently rounded combined coefficient.

### Exact audience target adjustment ABI

BattleAudienceAddTargetNum801a1a74/180 is an ordinary C float ABI (f1 delta,
f2 retained-bias increment), not an event callback. It captures both inputs,
queries global audience801a86a0 and performs single-precision operations.
First A13780 += f2. If bias>0 and delta<0: bias+=delta; when new bias<0,
delta=newbias,bias=0, otherwise delta=0. If bias<0 and delta>0, symmetric:
bias+=delta; when new bias>0,delta=newbias,bias=0, otherwise delta=0.
Both comparisons are original ordered float tests; finite used inputs avoid
the unestablished unordered/NaN domain.

Call BattleAudienceAddTargetNumSub801a18cc/1a8 with remaining delta. Sub queries
global audience, exact float delta==0 returns before dividing by base.
Otherwise repeatedly consume positive chunk=min(delta,1), or negative
chunk=max(delta,-1), subtract chunk from delta and clamp residual toward0.
For each chunk compute n=fctiwz(single(single(target/base-1)/0.25)), using
A1377c target/A13778 base. Positive chunk and n>=1 adds
single(chunk/float(n+1)) to target. Negative chunk and n<=-1 **subtracts**
single(chunk/float(n-1)) from target. Otherwise add chunk directly.
Repeat until residual equals0. Nonzero amount with base0 has no source guard;
do not introduce a numerical result for that unsupported native domain.

After Sub returns, outer function clamps A1377c below0 then above the
single-precision signed32 conversion of A13790. This source order can yield
a negative value with a malformed negative cap; do not replace with a
different generic clamp. Clamp A13780 below-1000 then above+1000. No RNG,
queue/population allocation, NumToTarget, event output or readiness value
is produced. Source constants80424990=1,804249d4=-1,804249ac=0.25,
804249d8=-1000,804249dc=1000. Fresh amount0 still executes bias writes and
outer clamps; Sub itself returns before a division, even when base0.
Listings `/tmp/trio-end-addtarget.dis` and `trio-end-adjusttarget.dis`.

`_BattleMarioKouraDelete`80212844 launches original803b0c50 at priority10,
flags0, owner entry160=Mario registry ID; it does not install U2a8 locally.
Six-command child SHA256
`811c76b7b75a68cc6ce139587fed4eb438dbcbd32a549ae1d794cc2a2a8bd599`:
_check_guard_koura80212794(-3,LW10); if LW10!=-1 run data child(LW10,hex3e).
The leaf evaluates/translates actor, captures RAW output token; U104&20 clear
outputs-1/return2. Otherwise BtlUnit_GetGuardKoura801267cc null outputs-1;
nonnull outputs its registry word0. The used no-guard path has no fabricated
Koura actor or resource event. The real sibling event must still be scheduled.

State05 resolves Mario/party. Party statusflag1 clears byteU12b, flag1 and
selects body pose table8038ed58 when admitted. ConsumeReserveItem800f74b4;
clear status IDs0..24 for party(ifnonnull) then Mario in each iteration,
retaining25..27. If Mario U104&10, party data key0a starts an event at priority10,
owner party, tracked U2a8, and holds05; otherwise set06 and fall through.
State06 requires BattleWaitAllActiveEvtEnd before clap(-1,0), all-unit end
events, BattleInformationSetDropMaterial(I), fortune inspection and08 (or
fortune P5ba==3 creates8036356c/MarioU2a8→07 then waits). This is the next
small source-backed stopping point after the initial audience/Koura leaves.

ResetMoveColorLvAll80125398/54 takes B, scans64 BattleGetUnitPtr slots
ascending, and writes byteU311=ff for each nonnull record. It does not write
unit resources, pose IDs, phase arrays or move counts. Source listing
`/tmp/trio-end-resetcolor.dis`.

The state05 status loop calls BtlUnit_SetStatus80127bac(U,id,0,0), not
BtlUnit_ClearStatus. Existing [status setter](BATTLE_STATUS_STORAGE.md) fixes
each per-ID write and side effect. In particular ID16 **retains ordinary
Charge U129**, because strength0 adds0 to its old unsigned byte before
narrowing/saturation; this differs from ClearStatus's explicit-99 request.
ID19/20 invoke _CheckMoveCount with their source specialflag/move gates.
Burn8/Freeze9 retain ordered opposite cancellation, and shared size/attack/
defense pairs retain sign-side clearing semantics. IDs25..27 are skipped;
status27, recovery-change information, flags and party snapshot state are
not blanket-reset. Use exact portable per-ID setter services rather than
clearing all30 packed status bytes.

`_ExecAllUnitBattleEndEvent`802151c4 scans64 registry slots ascending, gets
data key3f for every nonnull unit, schedules nonnull stream at priority10,
flags0 and stores unit ID at entry160. No status/HP filter and no U2a8 write
in this helper. Original Mario/Goombella tables have no3f; Frankly data table
93530074 contains only terminator. Their missing keys legitimately yield no
event, not a generic callback success stub.

## Original state table and remaining rewards

All numbers in the state column are hexadecimal suffixes of070000xx. Entry
addresses are the actual original jump-table targets, not sequential guesses.

| State / entry | Source owner and next used behavior |
| --- | --- |
| 00 /8021529c | Result priority, field callback or fresh audience/05 allocation above |
| 03 /80215430;04 /80215468 | Actual F3c event, ID/barrier and F44 resume gate above |
| 05 /802155b4;06 /802156d0;07 /80215780 | Status/reserved item, reaction barrier/drop material, optional fortune event |
| 08 /802157a8 | Stop fog; E0c=0; stock EXP→S16 E8, fortune-double, level99→0, upper cap100; copy E8→Ea; EXP icons if positive; timerE4=120; camera/music/audience requests→09 |
| 09 /80215908 | Decrement E4, when<=0→0a |
| 0a /80215938 | Positive E8 starts actual Mario/party data40 victory events and EXP effect→0b; zero goes19 unless Bflags400000 then0e |
| 0b /80215a4c | Actual EXP counting into pouchS16+96, heldpad300 accelerates; each point per4 counts ordinarily; reaching100 resets points0 and marks E0c=1; level99 stops further gain; exhaustion chooses timer→0c |
| 0c /80215b48;0d /80215bb0 | Timer, icon cleanup, soft-delete effectE29c; level-up timer60 vs5; then clear E29c→0e |
| 0e /80215be8;0f /80215c50 | Bflags400000 admits actual special event at r13-1660 on MarioU2a8, then owned-event wait; positive EXP→10, else19 |
| 10 /80215c9c | E30 clearbit1; E0c nonzero initializes upgrade selector and audience/reel requests→11, else19 |
| 11 /80215d0c;12 /80215d8c;13 /80215f58;14 /80216054 | Original level-up menu event/selector, private battle pad triggers and confirmation event barriers; no level-up is fabricated for trio |
| 15 /80216084;16 /8021629c | Selected HP+5/FP+5/BP+3, pouch revision/full resource recovery and real import, upgrade voice/animation/effects, event deletion/level continuation |
| 17 /80216318;18 /8021637c | Actual rank lookup/change event and wait, then further reward flow |
| 19 /802163a4 | Source victory voice/pose gate, Mario body M_V_1, timer60→1a |
| 1a /80216488;1b /80216504 | Timer drain, Mario/eligible party body pose type42→1b, then1c |
| 1c /8021651c | Set1d, audience volume(0,30), field encountered NPC140 OR100 whennonnull |
| 1d /8021655c;1e /80216590 | Original fade(type10,time200,color80427144), BGMOff201, wait actual finish→1f |
| 1f /802165b4 | Actual unit→pouch export/field party teardown, endwork free, top bank0=4, rule/audience stop |
| 01 /80215500;02 /80215568 | Own-loss path: unit end events/camera/fade; source configuration flag40 selects end1f vs sequence5 |

This table is a source dependency map. Numeric/render/audio/menu/drop services
remain individually required; it is not a complete simulation of each leaf.
Full listing and callback census remain `/tmp/trio-para-end.dis`,
`trio-end-BattleInformationSetDropMaterial.dis` and other `/tmp/trio-end-*`
read-only listings. Fresh normal conditions must be read from packed state,
not asserted from this table to skip unimplemented branches.

Data40 Mario80417c50: AnimeChangePose(-2,1,M_V_2), WAIT_MSEC1000,RETURN/END;
SHA256`d899da9e7d4314708d33c5dd0228f3ce875889eb64477dc4eb01ad2c4aed32b5`.
Goombella80396fa0: AnimeChangePose(-2,1,PKR_Y_1),WAIT_FRAME64,
AnimeChangePoseType(-2,1,43),RETURN/END; SHA256
`325b2423c70ee02303e1fcd20da9dc785fd3c6153d20d9eb3fbbda270d021bea`.
Inventory `/tmp/trio-end-win-events.json`. Import actual original pose art,
honor tracked U2a8 and source same-index force policy. Source type43 has no exact row in Goombella's ordered table; category69
selects original PKR_S_1 at80393b58 with force0. This is a pose-category
fallback, distinct from a missing animation name's directory0 fallback.

DropMaterial801141bc reads S through I+c and ORIGINALgroup throughS18;
it writes field drop counts S10(heart)/S14(flower), itemS0c through source
HP/FP ratios, RNG tables, badge314 and selected held/back item domains.
Story<=19 forces group selection weights0/0/100 for the ordinary item route,
but does not skip preceding heart/flower table RNG or badge queries. Death
stock coins remain I14 (field information), distinct from S14 flower count,
not an immediate pouch
coin grant synthesized by this controller. Full drop/equipment/RNG closure
is an explicit next request; preserving fields is required for field drops.
The next used closure is now documented in
[drop material/entry pouch](UNDERGROUND_TRIO_DROP_MATERIAL_AND_ENTRY_POUCH.md).

## Teardown, exact field continuation and boundaries

State1f captures actual current party kind and translates it to field partyID;
actual setup S4 mask40 alone admits MarioHP<=0→1. Otherwise no HP heal occurs.
BattleSetMarioParamToFieldBattle800f8a68 unconditionally exports Mario, then
exports nonnull active party through original snapshot service. It does not
restore maximum resources. Free E,clearBf28,topbank0=4; clear field Mario
word4 bit1; actual party kind selects marioPartyEntry or marioPartyKill;
clear active field party word4 bit1; BtlActRec_JudgeRuleKeep8018ef8c;
BattleAudienceSoundStop(1). See [exact export](PARTY_FIELD_EXPORT_AND_SNAPSHOT_CALL_DOMAINS.md).

Uppercase BattleMain topbank4 returns0; lowercase battleMain publishes Bflags
80000000. seq_battleMain sees battleSeqEndCheck, then its own state counter
must exceed30 calls before original sequence2 (unless source global flags
select5). The field wait command still requires seq==2 AND Iresult!=0 before
control/music/postbattle restoration. Battle exit/main lifecycle and full
field resource re-entry remain distinct required providers.

The preserved gorotuki_sanninngumi root continues after wait at REL14104:
delete its three original brother event IDs; restore NPC XYZ
(-65,0,65),(-55,20,35),(-55,0,85); Mario M_S_1 and each NPC damage animation;
source camera then500ms; clearNPC1 flag200; print exact mac_0_093 attributed
to original NPC1; original departures and messages094/094_01 follow.
The scene itself does not query result1 at that point; its wait admits any
nonzero source result, while victory is established by the actual battle
controller. No substitute winner banner/stat reset or replayed lecture is
authored. Story12 is written by the later original root, never by end00.
Full original161-command continuation remains in
[first-arrival contract](../story/UNDERGROUND_FIRST_ARRIVAL_AND_GOOMBA_TRIO.md).

Next native binding boundaries: original audience target adjustment, real
Koura child and per-unit data query; endwork display allocator fields; ordered
status/reserved item services; source drop material/RNG/badge/pouch ownership;
EXP/victory/level/rank events and original art; fade/end publication/export;
then preserved field VM continuation. General F3c writers and nonfresh
callback/override/level-up cases remain separate inbox dependencies.

Validation: read-only original body/table/event inspection and `git diff --check`.
No native edits, original code execution, build rerun or commit.

## Bounded next native increment

The implementation owner is binding original00→05→06 up to the actual
BattleInformationSetDropMaterial801141bc call. Its empty-audience provider
may support Sub's exact amount==0 branch and the full outer bias/clamp
operations, while retaining nonzero Sub as an explicit missing provider.
Source end allocation uses the existing heap0 SDK adapter; DispInit writes
E30=0 separately. This scope does not include drop RNG/rewards/fade/export/
field return, and must not release story11/result0 locks before the actual
original result setter/controller services execute. Validation and completed
native behavior belong to the implementation owner's subsequent entry.

Native initial00/05/06 implementation now passes the integrated all-three-enemy
scenario, preserving packed victory versus field completion and stopping at
DropMaterial. See the dated PROGRESS entry for exact validation and remaining
providers. End display/clap presentation, nonzero target adjustment, nonfresh
callbacks/status branches and complete rewards/return remain unbound.
