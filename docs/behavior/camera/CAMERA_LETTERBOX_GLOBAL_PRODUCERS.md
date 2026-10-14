# Camera letterbox global producers and native binding limits

US G8ME01 revision0, read-only ISO/DOL inspection2026-10-14. Supplements
[letterbox request order](CAMERA_LETTERBOX_AND_DUMMY_DRAW_SERVICES.md).
This is static source evidence and a current native binding audit, not original
runtime observation. Addresses are distinct from BattleWork sequence banks.

## Sequence current, requested and previous are separate owners

seqGetSeq8002e210/8 loads signed word **now_seq80415190**. seqGetNextSeq
8002e200 loads next_seq80415194; seqGetPrevSeq8002e208 loads prev_seq80415198.
All three ISO data words initially containffffffff. seqInit_MARIOSTORY
8002e304/3c clears32-byte seqWork8041e780, then explicitly writes-1 to all
three words. This constructor is distinct from MusyX seqInit80276770.

seqSetSeq8002e218/10 writes requested r3 to **next**, and r4/r5 to
next_p0/p1 at8041e778/77c. It does not publish current or invoke callbacks.

seqMain8002e228/dc compares next against current. For unequal values:

1. If current!=-1, request sysWaitDrawSync80005ce4, freshly load current,
   select row current*12 of table80304400, and invoke row+8 exit callback
   with seqWork. Neither callback is optional in this source path.
2. Freshly load current; store it to prev. Clear seqWork32 bytes.
3. Freshly load next and arguments; store next to current and seqWork.word0,
   seqWork.word4=0, arguments to words8/c, then invoke row+0 init callback.
4. Freshly load current; invoke its table row+4 Main callback with seqWork.

For equal values only step4 executes. No range clamp or substitution for -1
is present in the row lookup. A missing exit/init/Main owner requires a retained
service continuation, not early current publication or an inferred scene ID.
The outer update requests seqMain before battleMain/camMain; see
[outer order](../platform/GAME_OUTER_LOOP_AND_EVENT_UPDATE_ORDER.md).

After camDraw returns, marioStDisp's8006f8f0 store clears global8041e810;
it then returns to main. At80005608 main requests marioStMain immediately in
the same outer-loop iteration. StMain first executes its GP.word10 readiness
barrier, then makeKey80005ff8 at8006f928, seqMain8002e228 at8006f92c,
card/bg/shadow/anim Main, battleMain800f904c at8006f940 and camMain at944.
OSGetTick/profiling and DEMODoneRender follow the complete StMain call, rather
than preceding this sequence check. A completed all-disabled camDraw therefore
does not directly tear down battle or replay Draw: its next owner is the display
global-clear, then these outer update services. Fresh complete main800055e0/54
and marioStDisp8006f8dc/28 agree with the linked outer-order contract; ignored
inspection ELFs were reread from the ISO into the same artifact directory.

Current native Game.fieldSequence starts2. Game::fieldBattleServices maps its
setSequence directly to that same word, and Game::startBattle (Crump harness)
calls startFieldBattle, which requests4. Game::returnFromBattle explicitly
writes2 as a harness adapter. Those writes currently conflate requested/current;
they are not this original owner. The underground trioAttack.begin path does
not call startFieldBattle or write fieldSequence4. Its fieldSequence2 therefore
cannot establish original current sequence4 at the finishing camDraw. Neither
Scene::Dialogue/Battle nor a BattleWork inner sequence number repairs this gap.

## Mario Key and Ctrl use different retained bytes

marioChkCtrl8005c464/18 returns1 iff sign-extended Mario byte38 is0.
FieldMarioBattleState.controlDepth is a usable retained view of that byte;
Ctrl's predicate is controlDepth==0, not the signed value returned by native
fieldMarioControlOffCheck. ControlOff/On mutate this byte with modulo256
increment/decrement; preserve nesting rather than generic input enablement.

marioChkKey8005c47c/13c has the complete ordered service predicate in
[Key admission](../overworld/OVERWORLD_MARIO_LOW_HP_IDLE.md). Its byte test is
**Mario39**, represented by FieldMarioBattleState.keyDepth. Field KeyOff/On
item services mutate that distinct byte. Native idle now reads signed keyDepth
through FieldKeyServices.inputByte, correcting the former controlDepth binding;
see [native producer follow-through](../overworld/NATIVE_BERO_EXEC_OWNER_AND_ROAD_ADAPTER.md).
Other native Key ability/party/sequence adapters remain separate fidelity gaps.

Key first captures partyGetPtr(marioGetPartyId()). Native FieldPartyRegistry's
current(fieldMarioBattle), pointer(index), actor.word0 and actor.bytes31 can
supply retained registry inputs within its two-slot domain. The source's final
partner discriminator really reads signedbyte31, values5/6/7; the native party
constructor stores kind there. Do not replace it with an animation-state enum.
Key's registry pointer differs from letterbox's independently captured slot0.

Game's fieldShipChanging, fieldRollEnabled, fieldSlitEnabled,
fieldChristineStatus, fieldBomheiKeyCheck, fieldYoshiStatus and fieldHeldItem
are explicit native ability adapters. Their current0/1 defaults are not proof
of authentic original subsystem constructors or complete live service history.
ShipChanging must be queried twice when the first result!=1; original Key's
null-partner early return skips the later ability gates.

## GP word4 timing constructor

marioStInit8006fb3c/348 clears GP's13d8-byte work, then at8006fce4 requests
VIGetTvFormat802a5020. Result0 stores60 to GP.word4 at8006fcf8; every nonzero
result stores50 at8006fd08. This is a source VI-format constructor, not measured
renderer frequency or a battle sequence number. Existing native TickRate60 can
serve as an explicitly declared NTSC timing adapter. No independent retained
native GP.word4 owner currently exists; simulation or host refresh changes must
not silently alter this signed integer division input. Letterbox fade uses
2*(255/signedGP4), stores/reloads signedhalf before its negative clamp.

## Sound halfword and inherited side-effect gates

psndInit800dc4e0/170 explicitly zeros halfword **803df73e**, at800dc608.
It is psnd803df6e8+56; the older audio-driver table naming803df188 is not the
address emitted by these fresh original bodies. Global initialization and other
sound services remain separate owners.

psndClearFlag800dc0f8/18 reads that unsignedhalf, AND-clears requested r3,
and storeshalf, with no downstream requests. psndSetFlag800dc110/8c:

1. Readhalf, OR requested32-bit mask, store low16.
2. Test bit1 of the OR result; if set, request psndBGMOff_f_d800dabac
   (0,1000,0), then(1,1000,0).
3. **Reload** retainedhalf; if bit2 set, request psndENVOff_f_d800d7470
   (0,1000,0), then(1,1000,0).

The first two calls can affect the fresh read for the second gate. Continuation
must not OR/replay completed calls after a provider wait. Adding20 has no extra
call only in the bounded inheritedbit1/2-clear domain. No stream restart is
implied by ClearFlag. See [sound routing](../audio/FIELD_ITEM95_FANFARE_BGM_ROUTING_AND_QUERIES.md).

Native Game.fieldSoundFlags is currently uint32, initialized0 with10 and80
mutations; startBattle resets it0. Those known writers preserve bit1/2 clear,
but this is a bounded native adapter, not a whole-game sound-halfword lifetime.
Sharing a uint16 owner across letterbox/field/audio, retaining it across battle
and providing the conditional calls is required for the general source path.

## First missing native owner and validation

After letterbox's initial slot0 read and camera8 flag200 admission, the first
missing original automatic-decision owner remains BeroEXEC8041ea14. Once that
query is explicitly supplied, authentic sequence-current publication is a
separate missing owner. This audit supplies no fabricated query result.
Explicit providers can still execute recovered scalar work and stop at the
next genuine service wait. Native raster/GX and field return remain incomplete.

Fresh complete byte-span SHA256 values:

| Body | SHA256 |
| --- | --- |
| seqSetSeq8002e218/10 | a1c1b994f152d51a1aea8e6e76bf87f03f02f07678d00a832daa4bd640b211aa |
| seqMain8002e228/dc | 239f5917466e109cc1cada2f7c75c3b6def9333ba5f2c114f016bded6958a36e |
| seqInit_MARIOSTORY8002e304/3c | b490d5253c4e807368ce5a8885751df18b81c48a098fc8587364731e56838de4 |
| marioStInit8006fb3c/348 | 8018bb2efc969cac5acb75f1b3c5c027f40ba108ddde3050a5b8465264237f08 |
| psndClearFlag800dc0f8/18 | 004dade64a998ad5d80c0c4549a1f9c9b9e40884cddd1be84cbdc3b943cff10c |
| psndSetFlag800dc110/8c | 5c6a89e80c0716d9981162b651bb95e9ebf7b2ab9f1200ce361d63603e83abe4 |
| psndInit800dc4e0/170 | 3e0abb3a40bb3770ab7ad17952694b9d29c945ff9219102ebe5d53fdbe1e0483 |

Ignored listings/ELFs: artifacts/reference/letterbox-globals/. ISO openedrb
only. Documentation-only work; no new build, emulator, hardware or original
execution result asserted. No code/index/progress files changed by this task.
