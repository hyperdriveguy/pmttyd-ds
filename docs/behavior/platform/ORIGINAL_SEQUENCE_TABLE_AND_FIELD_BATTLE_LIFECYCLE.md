# Original sequence table and field/battle lifecycle

US G8ME01 revision0, static read-only ISO inspection 2026-10-14. Extends
[global producer audit](../camera/CAMERA_LETTERBOX_GLOBAL_PRODUCERS.md) and
[field return handshake](../battle/UNDERGROUND_TRIO_FIELD_RETURN_HANDSHAKE.md).
These are original source orders, not Dolphin observations or certification of
native callbacks. Ignored complete listings: artifacts/reference/sequence-lifecycle/.

## Table and retained owner

Original table80304400 is96 bytes: eight12-byte rows, in init/Main/Exit order.
The numeric IDs are table indices, independent of native Scene or BattleWork
inner sequence banks.

| ID | Owner | Init | Main | Exit |
| --- | --- | --- | --- | --- |
| 0 | logo | 800874b0 | 80086d6c | 8008746c |
| 1 | title | 80009d30 | 80009cd8 | 80009cf8 |
| 2 | game/field | 8000753c | 800073a4 | 80007390 |
| 3 | map change | 8000874c | 80007c2c | 80008724 |
| 4 | battle | 80073f20 | 80073b24 | 80073f00 |
| 5 | game over | 80078a14 | 80078548 | 800789ec |
| 6 | load | 800f72b8 | 800f6f68 | 800f727c |
| 7 | E3 | 80252f8c | 80252818 | 80252f60 |

Current80415190, requested80415194, previous80415198 begin signed-1.
seqWork8041e780 has32 bytes; arguments next_p0/p1 are8041e778/77c.
seqSetSeq8002e218 stores requested, p0, p1 in that order. No callbacks or
current publication. Constructor8002e304 clears work then stores all three-1.

seqMain8002e228 first captures requested, then current, compares them. Equal
values go directly to a **fresh** current read and Main row selection. Unequal:

1. Captured current==-1 skips sync/Exit. Otherwise call sysWaitDrawSync80005ce4;
   then freshly read current and capture its Exit row; call Exit(seqWork).
2. Freshly read current; store previous; clear all32 work bytes.
3. Freshly capture requested (r9), p0(r6), p1(r5), in that order. Store captured
   requested to current. Capture its Init callback. Store work+0=requested,
   +4=0,+8=p0,+c=p1. Call the captured Init(seqWork).
4. Freshly read current and capture its Main callback; call Main(seqWork).

There is no second equality check after sync/Exit and no row range guard.
Callbacks may mutate sequence owners; the above fresh reads/captures matter.
A service continuation preserves completed mutations and captured arguments;
it cannot repeat sync/Exit/Init because a later callback waits. **Current is
published before Init.** Unavailable battle Init must retain that publication
and suspend before subsequent Main/outer services; it cannot certify completed
battle entry. A missing Exit prevents publication.

## Smallest field callbacks

seq_gameExit80007390/14 is a service-free leaf: load GP pointer; read GP.word0;
clear mask2; store same word. It does not release field actors/events/resources.
This exact callback can use a retained GP owner without inventing a full field
teardown. Together with actual draw-sync it permits the source2→4 current
publication. It does not by itself satisfy battle Init or Main.

seq_gameInit8000753c/38 calls OSGetTime8029d84c; stores returned low word r4
at8041e5a4, then high word r3 at8041e5a0 with a GP pointer load between stores;
clears GP.word18 mask10. This clock is the original time-base service; native
simulation ticks must be explicitly declared as an adapter, not measured time.

Field Main state0 performs incoming fade selection and presets described in
[field return/fade contract](../battle/FIELD_RETURN_SEQUENCE_COUNTER_AND_INCOMING_FADE_ORDER.md).
State1 includes original game-over checks, marioGetPtr8005e0c0, two independent
camera4 pointer queries and psndSetPosDirListener800dc380. Neither full callback
has a success implementation merely because the native field scene renders.

## Battle Init exact top-level service order

seq_battleInit80073f20/f8 ignores the work argument locally:

1. Capture GP.wordc into global8041e968 (r13+1a48).
2. fbatGetPointer8004829c, capture returned word8 as battle-data pointer r31.
3. evtGetValue80031260(null,-170000000): original GSW0 expression. If signed
   result>=109, read it again and skip music changes if second result<111.
   Otherwise read it again and skip if result==402. A constant story snapshot
   cannot replace these reads where callbacks may change retained story.
4. Unless skipped: psndBGMOff800daefc(1024), then capture data.word2f0 and call
   psndBGMOn800db7a0(1,captured value).
5. psndENVOff800d76b8(1024); padRumbleOff80005f28(0);
   evtStopAll80035c68(4); psndSetFlag800dc110(0x80), in that order.
6. Fresh fbatGetPointer, read word10. If exactly00800000 or01000000 and retained
   global8041e968==0: marioCtrlOff8005c2c4; partyCtrlOff800bed44;
   padRumbleOff(0). Otherwise return.

The field work data pointer/type, sound owners, event-root stop and nested
control ownership are actual providers. A whole Init no-op is not an authentic
binding. Existing native field Mario/party control owners can cover their used
leaf operations; they do not cover the preceding services automatically.

## Battle Main states and exit

State work.word4 selects switch8030c3e0; unsigned state>11 returns.

| State | Target | Main operation |
| --- | --- | --- |
| 0 | 80073b70 | fadeEntry80013368(22,1000,RGBA000000ff); fresh state++ and counter+10=0 |
| 1 | 80073ba0 | two ordered fileAsyncf requests for audience normal/Luigi TPL; each false returns before later work; then counter++, fadeIsFinish, conditional state++ |
| 2 | 80073c18 | pouch query; encounter-type/control branches; clear field battle word570; state++; falls through state3 |
| 3 | 80073ca8 | retained field battle/pouch table work and file alloc; state++ unless its local wait condition holds; falls through state4 |
| 4 | 80073d94 | battle_init800738ac; nonzero returns, zero state++ then falls through state5 |
| 5 | 80073dac | battleSeqEndCheck800f903c and listener update; no same-call state6 counter |
| 6 | 80073e3c | counter++ then signed>30 requests field2 or game-over5 |
| 7,8,9 | 80073ee0 | return |
| 10 | 80073e90 | fadeEntry(22,200,RGBA000000ff); fresh state++ |
| 11 | 80073eb8 | fadeIsFinish; if ready GP.wordc=0, request(5,1,0) |

State1 source file requests use getMarioStDvdRoot80007388 separately before each
fileAsyncf8018a0e8(4,0,"%s/%s",root,path). Paths802c2ae4 and802c2b08 are
battle/audience/audience_normal.tpl and battle/audience/audience_luigi.tpl.
False is ordinary async waiting, distinct from a missing provider. Do not
increment counter or issue the second request after first false. Detailed
state2/3 transitive allocation/data owners remain outside this top-level audit.

State5 calls marioGetPtr, camera4 pointer, uses listener location(0,0,400),
adds200 to Z if live B.wordef4 mask100, queries camera4 again and passes freshly
read yaw114 to psndSetPosDirListener. State5 end-check, alternate state11,
state6 condition and incoming field fade are in the linked return contracts.

seq_battleExit80073f00/20 calls battle_exit800736ec; the original full ordered
release/recovery chain is in the handshake contract. Native trio pose/export
completion is not that teardown. Draw-sync, BattleEnd, stage/pose/material/GPU
releases, preserved field event restart and party appearance remain independent
owners. Do not substitute a scene write or unconditional success callback.

## Source identity and native frontier

Fresh startup80005418 bytes3c40804260427440 pin r2=80427440. This supersedes the
older semantic-guess r2 value in SMALL_DATA_R2_BASE_AND_FLOAT_POOLS.md for these
source reads. Pools804203b0/b4 are000000ff; format804203bc is"%s/%s";
804203c4/c8/cc floats are0,400,200. Table96-byte SHA256:
f5d895373a4a3db2fd3e847978ad7130a2909ef7d4fdd0be17662dad83f8f3be.
Battle switch48-byte SHA256:
fd32a77222467af08f24b7f9b6c1f33f9aeac01d68102040c1e2bfb1bbb60a7b.

| Complete body | SHA256 |
| --- | --- |
| seq_gameExit80007390/14 | e728cfb971f5e99ecbea23546f0abb37ef026c644eddbdf8aedf766da400707b |
| seq_gameInit8000753c/38 | ee640b28e51bd0bc7385f539c52bdf363daefff7215bdc70b35a7f71b60a5380 |
| seq_gameMain800073a4/198 | 52cd83b5a3acfdb559a7e0950b35c278de49bd97cd35d9a367dcebadd8e2606a |
| seq_battleExit80073f00/20 | e28c54959a8d3a205a4d5e25ad2cc4bfafe3e16e2b2ae350eb473c3b39128ec0 |
| seq_battleInit80073f20/f8 | 3753f89ed2fcc71a4457f8995b399d75809f6cf27c8f6dff9da1753145580904 |
| seq_battleMain80073b24/3dc | 013f9d3a2a4da7f4507dd7e8268be1b37f4088a93c89eeeaeb6e41cf07116ea2 |
| seqMain8002e228/dc | 239f5917466e109cc1cada2f7c75c3b6def9333ba5f2c114f016bded6958a36e |
| seqSetSeq8002e218/10 | a1c1b994f152d51a1aea8e6e76bf87f03f02f07678d00a832daa4bd640b211aa |
| seqInit_MARIOSTORY8002e304/3c | b490d5253c4e807368ce5a8885751df18b81c48a098fc8587364731e56838de4 |

Native Game inspected before this documentation pass has fieldSequence2 with
setSequence writing that same word, and no original retained current/requested/
previous lifecycle. Smallest next binding is separate sequence owner, exact
field Exit leaf and actual draw-sync; then suspend at actual battle Init service
without replaying publication. That creates truthful state4 ownership; continued
outer battle/camera/display must still respect unfinished Init/Main. Full
original sequence callback coverage and field return are not verified here.
