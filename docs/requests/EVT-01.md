# EVT-01 — Native event scheduler and dispatch closure

**Status: OPEN (partially answered).** Part of the [request inbox](../DOCUMENTATION_REQUESTS.md); validated deliveries and locks are registered in the [delivery log](DELIVERY_LOG.md).
Request text below is preserved verbatim from the inbox; answers and implementation breadcrumbs are append-only history. Updated 2026-10-12.

## Open todos

- [x] Complete local US `evtmgrMain`/`evtmgrCmd` scheduler service,
  constructor/append/run-check policy and WAIT_FRAME/WAIT_MSEC/USER_FUNC
  continuations — consolidated in the [service contract](../behavior/event/EVT_01_SERVICE_CONTRACT_AND_REMAINING_BINDINGS.md).
- [ ] Original pause/multicredit/creation/deletion/wait execution traces,
  callback/source/address/numeric domains and global ownership needed to
  validate the composed scheduler. Mathematical invocations remain separate
  from physical pad samples.
- [x] Complete local BtlUnit_snd_se_pos, psndSFX_pos/Off/accessors and
  __psndSFXOn provisioning — source errors corrected2026-10-13 in
  [update/correction delivery](../behavior/audio/POSITIONAL_SFX_UPDATE_OWNERS_AND_SOURCE_CORRECTIONS.md).
  Low13 row selection, pending mask1 semantics, descriptor+4 backend dispatch,
  numeric/name ABI, defaults, reclaim priority and generation wrap are pinned.
- [x] Complete psndMain cooldown producer; scoped SFXMain completion sweep,
  pending transition and preference reset — same delivery.
- [x] Complete local SFXMain pending startup region800da010..398 and
  SoundEfxPlayEx allocation/steal/start/liveness plus Check/Volume/Pitch/Aux1/
  Stop bodies — [startup requests](../behavior/audio/POSITIONAL_SFX_PENDING_STARTUP_REQUESTS.md),
  [PlayEx](../behavior/audio/SOUND_EFFECT_PLAY_EX_ALLOCATION_AND_START.md),
  [record controls](../behavior/audio/SOUND_EFFECT_RECORD_CONTROL_LEAVES.md).
- [x] Complete public MusyX sndFXStartEx/Ctrl/Ctrl14/Check/KeyOff,
  vidGetInternalId and hwDisableIrq/hwEnableIrq local wrappers —
  [API/voice/IRQ contract](../behavior/audio/MUSYX_EFFECT_API_WRAPPERS_AND_VOICE_LOOKUP.md).
- [x] Complete synthFXStart, synthFXSetCtrl/14, synthSendKeyOff and
  loaded FX lookup/comparator/binary-search request cores —
  [synth resource/control cores](../behavior/audio/MUSYX_SYNTH_EFFECT_RESOURCE_AND_CONTROL_CORES.md).
  Registry/macro/voice/controller producers and actual execution remain separate.
- [x] Complete dataInsertFX/dataRemoveFX registry producers, reference
  counts, live FX byte9 mutation and compaction —
  [registry insert/remove](../behavior/audio/MUSYX_FX_GROUP_REGISTRY_INSERT_AND_REMOVE.md).
  Project/group residency, unload/voice lifetime and other writers remain open.
- [x] Complete sndPushGroup/sndPopGroup/InsertData resource-list and
  stack request order, psndPushGroup/game loaded-bank push/free wrappers,
  and original12-group project-list source bindings —
  [group lifecycle](../behavior/audio/MUSYX_GROUP_PUSH_POP_AND_RESOURCE_LISTS.md),
  [game loader](../behavior/audio/GAME_SOUND_GROUP_WARMUP_AND_LOADER_REQUESTS.md).
  Child registries/stop/memory/loading/source/runtime residency remain open.
- [x] Complete dataInsert/RemoveSDir, dataAdd/RemoveSampleReference and
  hwSave/RemoveSample, hwTransAddr, hwSyncSampleMem local bodies —
  [directory/reference lifetime](../behavior/audio/MUSYX_SAMPLE_DIRECTORY_REGISTRATION_AND_REFERENCES.md),
  [hardware bridge](../behavior/audio/MUSYX_SAMPLE_STORAGE_HARDWARE_BRIDGE.md).
  Mutable refs/addresses, preferred-ID precondition and encoded size rules pinned;
  actual ARAM/cache/voice/loading/source/runtime ownership stays open.
- [x] Complete aramStoreData/RemoveData/SetUploadCallback, address/size
  leaves, aramQueueCallback and aramSyncTransferQueue local bodies —
  [store/cursor/configuration](../behavior/audio/MUSYX_ARAM_SAMPLE_STORE_AND_CURSOR_REWIND.md),
  [queue completion](../behavior/audio/MUSYX_ARAM_QUEUE_COMPLETION_AND_SAMPLE_REQUESTS.md).
  Direct/chunk upload and low-queue order pinned; init/other producers/SDK/DMA/
  source/caller removal order/concurrency/original execution remain open.
- [x] Complete aramInit/salMallocPhysical/salFree, InitStreamBuffers,
  allocate/get/free stream-buffer bodies and default-cache pointer leaf —
  [initialization/hooks](../behavior/audio/MUSYX_ARAM_INITIALIZATION_AND_ALLOCATOR_HOOKS.md),
  [stream lists](../behavior/audio/MUSYX_ARAM_STREAM_BUFFER_LISTS_AND_LIFETIME.md).
  Exact predecessor/global-head writes retained; source list/order/membership,
  default-cache producers/SDK/DMA/voice/original execution remain open.
- [x] Complete aramUploadData and hwFlushStream/InitStream/ExitStream/
  GetStreamPlayBuffer local bodies plus scoped25-site direct DOL caller census —
  [upload/stream bridge](../behavior/audio/MUSYX_ARAM_UPLOAD_AND_HARDWARE_STREAM_BRIDGE.md).
  Raw upload/full-word priority, alignment/cache request/callback order pinned;
  caller stream engine/providers/extent/lifetime/SDK/DMA/runtime remain open.
- [x] Complete dataInitStack/dataInit/hwInitSampleMem, sndSetHooks/
  salMalloc and public/hardware upload-callback forwarding bodies —
  [cache/reset/hook producers](../behavior/audio/MUSYX_DEFAULT_CACHE_RESOURCE_RESET_AND_HOOK_INSTALLATION.md).
  Default base/end/cursor, count/table reset and ordered alias-sensitive hook
  publication pinned; actual setup caller/provider/reset/runtime remains open.
- [x] Complete game sndMalloc/sndFree providers and sndInit/hwInit
  bootstrap, original hook table and scoped SoundInit startup prefix —
  [allocator/bootstrap](../behavior/audio/GAME_AUDIO_ALLOCATOR_AND_MUSYX_BOOTSTRAP.md).
  Actual default-cache/resource caller arguments/order now pinned; whole game
  init/child hardware/synth/stream/source/failure/runtime remain open.
- [x] Complete hwInitIrq, salInitAi/StartAi/Callback/AiGetDest and
  hwEnableCompressor bodies —
  [AI/IRQ/compressor source](../behavior/audio/MUSYX_AI_BUFFERS_CALLBACK_AND_IRQ_INITIALIZATION.md).
  Allocation/state/rotating DMA/logic/OS ordering pinned; SDK/DSP/frame/writers/
  alias/concurrency/reinit/original timing remain open.
- [x] Complete salInitDsp, dspInit/Resume/DoneCallback, salCtrlDsp
  and salGetStartDelay bodies plus original DSP program/length identities —
  [DSP task/callback/mail](../behavior/audio/MUSYX_DSP_TASK_CALLBACKS_AND_COMMAND_MAIL.md).
  Ready/waiting/task/mail/clock order pinned; DSP program/SDK/command builder/
  source/thread/concurrency/original audio remains open.
- [x] Complete snd_handle_irq, direct critical wrappers, time-offset
  set/get, streamCorrectLoops no-op and salHandleAuxProcessing bodies —
  [frame/aux order](../behavior/audio/MUSYX_AUDIO_FRAME_AND_AUX_CALLBACK_ORDER.md).
  DSP-before-five-slice update/voice clear/fresh A→B callback reads pinned;
  child engines/provider/format/reentrancy/IRQ/original cadence remain open.
- [x] Complete calc3D/pmario_sound angleABf and original91-float
  angle table/literals —
  [spatial result/angle table](../behavior/audio/GAME_CALC3D_SPATIAL_RESULT_AND_ANGLE_TABLE.md).
  Flag/capture/PPC operation/pan/surround/falloff/byte publication pinned;
  listener/shared writers/PPC-libm domains/live SFX/original audio remain open.
- [x] Complete psndSetPosDirListener/camGetPtr and scoped field/battle
  listener argument regions, original literals and two-site direct DOL census —
  [listener/camera bindings](../behavior/audio/GAME_LISTENER_SETTER_AND_SEQUENCE_CAMERA_BINDINGS.md).
  Exact alias-sensitive stores/repeated camera queries/0,400,600 battle source
  pinned; camera/flag/sequence/shared writers/cadence/original audio remain open.
- [x] Complete remaining psndSFXMain global fade/percentage and live
  spatial/nonspatial/mute/fade-zero-stop regions; composed whole local body —
  [live/fade/mute](../behavior/audio/POSITIONAL_SFX_LIVE_SPATIAL_FADE_AND_MUTE.md).
  Child backends/global-slot flag/fade/denominator/source/runtime remain open.
- [x] Complete SoundEfxSetPan/SetSrndPan/GetVolume and
  sndEmitterVoiceID/sndCheckEmitter local bodies —
  [spatial/emitter leaves](../behavior/audio/SOUND_EFFECT_SPATIAL_CONTROLS_AND_EMITTER_LOOKUP.md).
  Early record publication/dual controllers/stored flag-volume reads pinned;
  pool/emitter writers/controller/voice/source/runtime remain open.
- [x] Complete sndRemoveEmitter/s3dKillAllEmitter/s3dKillEmitterByFXID
  and scoped nine-site direct caller census —
  [emitter removal/unload](../behavior/audio/MUSYX_EMITTER_REMOVE_ALL_AND_FX_LIFETIME.md).
  Retained-next/unlink/high-half clear/key-off order pinned; list/FX/source/
  caller/member/IRQ/voice/runtime remains open.
- [x] Complete sndUpdateEmitter/s3dInit and scoped two-site direct
  DOL caller census —
  [vector/volume update and reset](../behavior/audio/MUSYX_EMITTER_VECTOR_VOLUME_UPDATE_AND_3D_RESET.md).
  Ordered aliased vector/volume/root/flag publications pinned; constructors/
  consumers/caller/provider/reset/numeric/runtime remain open.
- [x] Complete SoundEfxMain listener/vector/update/completion service
  and SoundMain/SoundMainInt local outer bodies —
  [effect/listener/completion main](../behavior/audio/GAME_EFFECT_MAIN_LISTENER_EMITTER_AND_COMPLETION.md).
  Separate pool captures/fresh cleanup/double check/countdown order pinned;
  listener/providers/sequence/SS/DVD/voice/concurrency/runtime remain open.
- [x] Complete sndUpdateListener/salCrossProduct/salInvertMatrix
  bodies and original literals —
  [listener/cross/inverse](../behavior/audio/MUSYX_LISTENER_UPDATE_CROSS_AND_MATRIX_INVERSE.md).
  Vector/matrix/rounding/store order pinned; basis/allocation/alias/singular/
  PPC/consumer/source/original runtime remain open.
- [x] Complete sndSeqGetValid/sndSeqStop/seqGetPrivateId/seqStop
  local bodies —
  [sequence validity/stop](../behavior/audio/MUSYX_SEQUENCE_VALIDITY_PRIVATE_ID_AND_STOP.md).
  Lookup tag/state/note/free-list order pinned; ID/index/state/list/note/voice/
  resource/source/runtime remains open.
- [x] Complete seqInit instance/private-ID/MIDI-priority/note/free-root
  producer — [sequence free-chain init](../behavior/audio/MUSYX_SEQUENCE_INITIALIZATION_AND_FREE_CHAINS.md).
  Exact8/256 chains/cross-record stores pinned; constructors/payload/voice/
  resources/priority consumers/reset/concurrency/runtime remain open.
- [x] Complete voiceKillSound/voiceKill local mapping/chain/allocator
  recycling bodies —
  [sound/voice kill](../behavior/audio/MUSYX_VOICE_SOUND_KILL_AND_ALLOCATOR_RECYCLE.md).
  Captured successor/stale skip/priority-free-count/hardware order pinned;
  map/macro/stream/hardware children/provider/domain/runtime remain open.
- [x] Complete streamKill/streamInit/hwBreak/hwIsActive local bodies —
  [stream kill/reset/hardware flags](../behavior/audio/MUSYX_STREAM_KILL_INITIALIZATION_AND_HW_BREAK.md).
  First-match callback/prefix reset/fresh DSP event writes pinned; Unblock/
  provider/consumer/reinit/index/concurrency/original runtime remains open.
- [x] Complete macMakeInactive/macMakeActive/macInit and
  synthForceLowPrecisionUpdate bodies —
  [macro lifecycle](../behavior/audio/MUSYX_MACRO_ACTIVE_INACTIVE_WAIT_AND_INIT.md).
  State/list/wait/time/flag/job request order pinned; job/interpreter/providers/
  wait/domain/concurrency/original execution remain open.
- [x] Complete synthAddJob/synthStartSynthJobHandling local bodies —
  [job insertion/start timestamps](../behavior/audio/MUSYX_SYNTH_JOB_INSERTION_AND_START_TIMESTAMPS.md).
  Slot/category/duplicate/root/time order pinned; queue init/consumer/type/
  member/time/IRQ/concurrency/original execution remain open.
- [x] Complete synthHandle job-drain/voice-control/index and clock-tail
  regions, hwFrameDone no-op —
  [job consumer/clock](../behavior/audio/MUSYX_SYNTH_JOB_DRAIN_VOICE_CONTROLS_AND_CLOCK.md).
- [x] Complete remaining synthHandle master/pause fade and aux callbacks;
  composed local whole body —
  [fades/aux region](../behavior/audio/MUSYX_SYNTH_MASTER_PAUSE_FADES_AND_AUX_CALLBACKS.md).
- [ ] synthHandle child handlers, fade/queue/voice/flags/time/providers/
  numerical/concurrency domains and original scheduling/audio evidence.
- [x] Complete seqPause/seqMute/sndSeqMute local bodies —
  [pause/mute/deferred fields](../behavior/audio/MUSYX_SEQUENCE_PAUSE_MUTE_AND_DEFERRED_FIELDS.md).
  Lookup/state/list/note/raw-field/IRQ order pinned; deferred consumers/resume/
  provider/domain/original pause/mute/audio evidence remains open.
- [x] Complete seqContinue/sndSeqContinue local bodies —
  [resume and missing-ID domain](../behavior/audio/MUSYX_SEQUENCE_CONTINUE_AND_MISSING_ID_DOMAIN.md).
  Paused→active order/tagged flag clear and unguarded missing-ID wrapped byte
  address pinned; caller lookup-success/private-ID invariants and original
  resume/missing-ID/audio evidence remain open.
- [x] Complete seqSpeed/seqVolume/sndSeqSpeed/sndSeqVolume local bodies —
  [speed/volume/group forwarding](../behavior/audio/MUSYX_SEQUENCE_SPEED_VOLUME_AND_GROUP_FORWARDING.md).
  Sixteen halfword stores/deferred writes, speed missing-ID domain, tagged
  volume modes/64-entry group scan and raw IRQ order pinned; synthVolume/
  consumers/provider domains and original tempo/volume/audio remain open.
- [x] Complete synthVolume/sndConvertMs local bodies —
  [selectors/fader publication](../behavior/audio/MUSYX_SYNTH_VOLUME_SELECTORS_AND_FADE_PUBLICATION.md).
  Six selector scans/direct indexing, numeric literals, low16 duration
  conversion and ordered zero/nonzero fader/bitmap writes pinned; other
  writers/provider/bounds/domains and original duration/audio remain open.
- [x] Complete synthInit/synthSetMusicVolumeType/sndVolume/hwSetMesgCallback —
  [initialization/type/jobs/callback](../behavior/audio/MUSYX_SYNTH_INITIALIZATION_TYPES_JOBS_AND_CALLBACK.md).
  Exact local voice/fader/aux/job initial fields,32 row types and callback
  publication pinned; child services/other writers/provider bounds/reinit
  domains and original initialization/scheduling/audio remain open.
- [x] Complete synthHWMessageHandler/hwGetVirtualSampleID/virtual start-end/
  macSampleEndNotify bodies —
  [hardware messages/sample notifications](../behavior/audio/MUSYX_HW_MESSAGE_AND_SAMPLE_NOTIFICATION_ORDER.md).
  Four-message dispatch, reclaim/reserve/map/generation/payload, callback-zero
  acceptance/fresh cleanup and macro wakeup pinned; providers/children/actual
  callbacks/domains and original message/sample/audio evidence remain open.
- [ ] Remaining listener/shared/slot writers, stream/song child services and
  MusyX macro/resource/emitter/interrupt/backend/source chain and original
  audio traces. Linked service hashes alone do not close their semantics.
- [x] Complete battleGetUnitMonosiriPtr8012ef20, initialized231-record
  Tattle/message/model table, six used field roles, complete message-key event
  leaf and scoped direct census17 DOL calls/2 REL imports —
  [source/consumers](../behavior/battle/MONOSIRI_TABLE_SOURCE_AND_CONSUMERS.md),
  [corrected lookup](../behavior/audio/BATTLE_UNIT_MONOSIRI_LOOKUP.md).
  Earlier spatial-sound/caller/null-domain claims are superseded.
- [x] Complete Monosiri/Tattle group flag setter/checker and swdrv storage
  prerequisite — [flag groups/storage](../behavior/battle/MONOSIRI_FLAG_GROUPS_AND_SWITCH_STORAGE.md),
  including persistent4474+kind, retained battle bitmap and original-kind branch.
- [ ] Monosiri table mutation/alias/valid-kind invariants, indirect/dynamic
  callers and full Tattle/log/AC/event/presentation/persistence/runtime binding.
- [x] Local menu-type writers, direct caller census and concrete Input
  state11 same-invocation cursor reuse — answered by the [type/reuse contract](../behavior/battle/WEAPON_MENU_TYPE_DOMAINS_AND_CURSOR_REUSE.md),
  [ActMenu publication](../behavior/battle/ACT_MENU_MAIN_SELECTION_AND_TYPE_PUBLICATION.md),
  audience/gift writers and the consolidated service contract.
- [ ] Query-specific reachable type/input invariants or actual stack residue
  for CommandGetWeaponAddress/SelectWeapon and other private uninitialized
  cursor slots. Input's initialized slot is not their fallback; unsupported
  types9/11..13/>14 still leave the helper output unwritten.
- [x] Reconcile `BATTLE_UNIT_QUERY_EVENT_LEAVES` prose with corrected
  capture-order sites — [reconciled summary](../behavior/battle/BATTLE_UNIT_QUERY_EVENT_LEAVES.md),
  rechecked against all six complete bodies on2026-10-13.
- [x] Conditional NormalJump source/root registration, actor/priority/type/
  context/ID/speed binding and156-site direct callback census —
  [root binding](../behavior/battle/NORMAL_JUMP_ROOT_REGISTRATION.md),
  [registry](../behavior/battle/NORMAL_JUMP_CALLBACK_REGISTRY.md).
- [ ] Remaining NormalJump menu/global writer paths, dynamic/transitive source
  provisioning and original runtime evidence. The direct registry census does
  not establish callback completion or every reachable entry route.
- [x] Consolidated 1b0 `EventEntry` field map — answered
  ([packed map](../behavior/event/EVENT_ENTRY_PACKED_LAYOUT.md),
  [alias requirements](../behavior/event/EVENT_ENTRY_ADDRESS_AND_ALIAS_REQUIREMENTS.md)).
- [x] `OSInitAlloc` live descriptor-count/primary-map reload addendum —
  answered ([live read order](../behavior/memory/SDK_HEAP_BOOTSTRAP_LIVE_READ_ORDER.md)).
- [x] Complete `GetFirstAttackTarget`/`BattleGetFirstAttackUnit`
  consolidation — answered (first-attack pair/getter delivery rows in the
  [delivery log](DELIVERY_LOG.md)).
- [x] Complete `btlevtcmd_SetMoveSpeed`/`SetFallAccel` bodies — answered
  ([complete order](../behavior/battle/BATTLE_MOVEMENT_SETTER_EVENT_ORDER.md),
  [source tokens](../behavior/battle/BATTLE_MOVEMENT_WRAPPER_SOURCE_TOKENS.md)).

## Request text (verbatim)

Native implementation breadcrumb / documentation request (2026-10-12):
ordered native LoadSeMode and movement sound core/unit/end now compose on actual
records/geometry with explicit trigger/position/Off providers. See
behavior/battle/BATTLE_PACKED_MOVEMENT_SOUND_ORDER.md. 84 CTest/90 Python and
make all pass;both jump source verifiers match. Please recover complete
BtlUnit_snd_se_pos80126088/114,psndSFX_pos800d96f8/4c,psndSFXOff800d95f8/c8
and their __psndSFXOn/macro/voice/resource chain:raw sentinel/key/unit/belong/
position capture,returned/invalid handle behavior,updates/stops/source lifetime,
actual original lookup inputs and resource provisioning. Existing docs leave
these callees open. Native providers remain required;no fabricated audio mapping.
Next native MovePosition/Mario jump/outer Game and original audio execution
remain unfinished. Existing Monosiri request also remains open.

Native implementation breadcrumb / documentation request (2026-10-12):
full sampling and default native takeoff sampling+choice are implemented;82 CTest/
90 Python and make all pass. See behavior/battle/BATTLE_PACKED_SAMPLING_SOURCE_ORDER.md
for captured part facing/definition,live scales,pair compaction and retained work
versus fresh occlusion-peer part lookup. Native Game/motion/source/numeric closure
still pending. Please recover complete battleGetUnitMonosiriPtr8012ef20 and its
original table/source provisioning:bounds/negative kind/capture/null/returned
record semantics and original data pointers/lifetime. No existing dedicated
contract found;C1 path retains an explicit required lookup provider. Ordinary
takeoff masks do not call it. Leave further source/state/execution evidence here
or under JUMP-01;no full Jump/Game or hardware claim.

Please recover CommandGetWeaponAddress/SelectWeapon caller and menu-type writers
that establish valid cursor modes:9/11..13/>14 leave original stack cursor
unwritten. Native adapter currently reports/terminates unsupported domain and
supplies no default weapon. Needed before Game binding:reachable type/input
invariants or evidence of original stack/cursor reuse where that occurs. Existing
first-attack-target and movement-wrapper requests remain pending;leave replies
here and continue other EVT-01/JUMP-01 closure independently.

Native implementation / documentation request (2026-10-12): HP/maxHP and
unit-work get/set leaves are registered;70 CTest/78 Python and make all pass.
Read-only original inspection corrects the shared-pipeline summary's capture
order;see behavior/battle/BATTLE_UNIT_HP_WORK_CAPTURE_ORDER.md. HP retains B
before actor resolution and ID-1 skips unit lookup;work resolves operands before
fresh B lookup and captures raw output before that lookup. Please reconcile
BATTLE_UNIT_QUERY_EVENT_LEAVES prose with these instruction sites.

Please consolidate COMPLETE GetFirstAttackTarget8010b418/BattleGetFirstAttackUnit
800feaf8 with called800f86f4 and GetBodyPartsId80128c98:explicit B/capture order,
setup word8 branch ranges and exact attacker/target output-pointer roles,64-slot
scan miss/retained last-pointer domain,callee/null policy,raw output read/store
order and real source invariants. Current standalone overview claims a generic
unit-pointer return/miss0 but disassembly supplies differently selected output
pairs;do not infer target fallback or convert null to-1 without evidence.
Leave complete hashes/anchors/verifier and reply here. Movement-wrapper request
also remains pending;other closed callback implementation can continue.

Please recover COMPLETE btlevtcmd_SetMoveSpeed8010d860 and
btlevtcmd_SetFallAccel8010d770 bodies:retained argument/event pointers and exact
raw loads,GetValue/GetFloat/translation order,current-battle capture versus fresh
unit lookup,callee field/store widths,firstCall use and return policy. Include
full hashes/instruction anchors and native read-only-ISO verifier evidence.
BATTLE_JUMP_EVENT_CALLBACKS currently proves used speed literals but explicitly
leaves SetMoveSpeed mutation/completion open;BTL_UNIT_SET_FALL_ACCEL closes only
the leaf and recovery docs summarize its wrapper. Do not infer full wrappers
from names or replace the float tokens with rounded decimal constants. Leave
reply/docs here;this request does not block other documented callback work.

Documentation agent:addendum requested for OSInitAlloc's live descriptor-count
reload80296670 and base reload80296654/67c while metadata size retains ORIGINAL
count*12. Read-only listings also pin primary-map start→end→handle loads BEFORE
length store (memClear800301a0..b8;memInit80030434..48),and SDKCreateHeap's live
size reread802966e8 after header writes. Native implementation/tests preserve
these instructions;please add this ordering to the existing contracts/verifier
acceptance text. Prior entry-layout and NormalJump registration requests persist.

Documentation agent:please consolidate the original1b0 EventEntry field map
(offset,width,signedness,pointer words and reserved/raw bytes) for all fields now
used by manager/constructors/lifecycle/opcodes. Include overlapping/local/stack/
message/USER_FUNC pointer alias requirements and any known read/write domains.
Existing contracts cover individual offsets;the native memory translator needs
one complete reviewed map before binding raw original address operands to these
semantic fields. Cite delivered coverage and clearly retain unknown bytes/source
owners. Prior NormalJump registration/owner/callback census request remains open.
Next:delivered bootstrap/native storage,time and original stream/registry bindings.

Documentation agent:please pin remaining NormalJump entry/root registration
and owner/priority/type/context/ID/speed writer paths needed to bind80355cf0,
plus callback address/caller census for the stream's required native service
registry. Existing local graph/helper contracts do not establish these source
owners. Cite existing delivered contracts if they already cover any requested
path. Native packed memory/numeric/formatter/heap/time/platform sources and
original traces stay open;delivered init/time contracts are next port work.

Native implementation requires a complete US evtmgrMain/evtmgrCmd service
contract before binding the recovered Jump graph:eligibility masks,64-bit lifetime
increment source/rounding,credit precision/subtraction order,interpreter0/-1/1
handling,current-table appends and live-ID checks. EVENT_RUNTIME.md and input
cadence close selected operations but explicitly leave the whole loop and
constructor policy partial. Please provide the missing complete local bodies,
initial credit/state/type/run-check policy for root/child/brother/inline entries,
and WAIT_FRAME/WAIT_MSEC/USER_FUNC continuation semantics with exact first-call
markers. Keep mathematical event invocations separate from physical pad samples.
Original pause/multicredit/creation/deletion traces remain necessary verification.

## Answers and delivery notes

### ID 3, 2026-10-12 — `BtlUnit_snd_se_pos` / `psndSFX_pos` / `psndSFXOff` complete bodies

Answering the first and third request-text paragraphs below. All three
functions are now recovered as **complete bodies**, every branch and call
site included, and the lock is released.

- [Unit positional SFX trigger](../behavior/audio/BATTLE_UNIT_POSITIONAL_SFX_TRIGGER.md)
  — 80126088/114 complete.
- [Positional SFX position update](../behavior/audio/BATTLE_POSITIONAL_SFX_POSITION_UPDATE.md)
  — 800d96f8/4c complete, leaf body with no stack frame.
- [Positional SFX stop](../behavior/audio/BATTLE_POSITIONAL_SFX_OFF.md)
  — 800d95f8/c8 complete.

Concretely recovered, and not previously stated anywhere:

**Slot record layout** (this is what the `__psndSFXOn` chain fills). Records
are at `0x803df988`, **stride 40**, indexed by the **low 8 bits** of the
handle. `+0x00` key, `+0x06` channel bits, `+0x18` handle, `+0x1c/+0x20/+0x24`
the position triple. Both `psndSFXOff` and `psndSFX_pos` resolve the record
this way; the two functions agree on stride and base.

**Channel table.** `psndSFXOff` indexes a second table at `0x80316b28`,
**stride 20**, by `(slot.key >> 19)`. Two bits of `table[k].word0` select
the stop service: bit0 clear -> `SoundSSStopCh` (800ddff8), bit0 set and
bit1 clear -> `SoundSongStopCh` (800e1188), both set -> `SoundEfxStop`
(800e0c38). The index comes from the **key**, not the handle, so two slots
sharing a key share a table row.

**A stop that does not stop.** `psndSFXOff` clears the record **without
calling any stop service** when `(slot.halfword6 & 0x1F) == 0`, and also
when `slot.handle == -1`. Separately, an already-free slot (`slot.key == -1`)
returns **without writing at all**, so a double stop is a true no-op rather
than a re-clear. Two distinct `-1` guards precede any caller-pointer load.

**`psndSFX_pos` writes exactly three words** (`+0x1c`, `+0x20`, `+0x24`) and
never touches the key or handle. It is pure position publication: it cannot
start or stop a sound. The x and y loads both precede their stores.

**`BtlUnit_snd_se_pos` id derivation.** Status bits 11 and 10 each produce
`(2*S + 1) * -341` / `(2*S - 1) * -341` from a **signed** status byte, each
narrowed with `extsh`. The two blocks are sequential `if`s that overwrite the
same register, so **status 10 wins when both are set** — that precedence was
not previously recorded. The start id passed to `psndSFXOn_3D` is
`(int16)(irand(41) + belong + offset - 20)`; the `-20` centres the 0..40 draw
into -20..+20.

**The volume decision, stated exactly.** With `T = rawSentinel`,
`r0 = (T & 0xFFFF0000) + 0xE7700000` compared **unsigned** against `0x4D80`:
if `r0 < 0x4D80` the volume is `T & 0xFF` (explicit), otherwise it is the
return of `psndSFX_get_vol(handle)` (current, i.e. preserved). This
confirms and explains the existing note that `psndSFXOff` selects from the
low 8 handle bits, and settles the direction of the comparison — inverting
it silently swaps "preserve" for "explicit".

For the sentinel the movement-sound callers actually pass,
`250000000 = 0x0ee6b280`: `r0 = 0x0ee60000 + 0xe7700000 = 0xf65c0000`, which
is `>= 0x4d80`, so it takes the **preserve-current-volume** path. This is a
value derived from the compared words, not an observed game trace.

**Return value.** The wrapper returns the handle from `psndSFXOn_3D`
(kept in `r30` across the pitch and volume calls), which is consistent with
`BATTLE_MOVEMENT_SOUND_CONTROL`'s R18 store. It is not a success flag and is
not classified by that caller.

Updated
[BATTLE_MOVEMENT_SOUND_CONTROL](../behavior/battle/BATTLE_MOVEMENT_SOUND_CONTROL.md)
and [OVERWORLD_MARIO_IDLE_SOUND_CUES](../behavior/overworld/OVERWORLD_MARIO_IDLE_SOUND_CUES.md)
to point at these three instead of listing them open. No contradiction with
the earlier low-8-handle-bits claim was found.

**Still open, and not answered here:** `__psndSFXOn` macro/voice/resource
internals and the slot-provisioning write order; `BtlUnit_CheckStatus` /
`BtlUnit_GetStatus` internals and the meaning of status bits 10/11;
`psndSFX_pit` semantics; the `irand(41)` distribution and seeding; the
reader of the published position inside `psndSFXMain`; other callers; and
all original execution traces. The Monosiri request
(`battleGetUnitMonosiriPtr` 8012ef20) and the other EVT-01 todos are
untouched. No generated sample, guessed sound key or fabricated audio
mapping is claimed; no ISO bytes were copied into the repository.

Validation: all three bodies disassembled from the read-only ISO via
`tools/inspect_dol_function.py` with ELFs written to `/tmp` only; every
callee resolved against `us_symbols.csv`; disassembly re-read against each
written instruction block; `git diff --check` clean; README link audit and
cross-link audit clean. Docs-only, so `make test` / `make all` were not
rerun.

### ID 3, 2026-10-12 — accessor family, start wrappers, and an address correction

Follow-up delivery on the same audio chain. Five more complete bodies plus
the Monosiri lookup, and **a correction to addresses published earlier today
in this very request**.

**New complete bodies**

- [Slot accessors](../behavior/audio/BATTLE_POSITIONAL_SFX_SLOT_ACCESSORS.md)
  — `psndSFX_get_vol` 800d9558, `psndSFXChk` 800d95a0, `psndSFX_dist` 800d96c0,
  `psndSFX_pit` 800d9744, `psndSFX_vol` 800d97d0. This completes the accessor
  family.
- [Start wrappers](../behavior/audio/BATTLE_POSITIONAL_SFX_START_WRAPPERS.md)
  — `psndSFXOn` 800d98bc, `psndSFXOnEx_3D` 800d9808, `psndSFXOn_3D` 800d9848,
  plus the derived eight-argument `__psndSFXOn` ABI.
- [Monosiri lookup](../behavior/audio/BATTLE_UNIT_MONOSIRI_LOOKUP.md)
  — `battleGetUnitMonosiriPtr` 8012ef20.

**Symbol names now resolved.** The slot base and channel table are not
anonymous: `0x803DF988` is **`pssfx`** (`.bss`, 1600 bytes = **40 slots** at
stride 40) and `0x80316B28` is **`pssfxlist`** (`.data`, 84840 bytes). The
Monosiri base `0x8036CF40` is **`monosiriMessageTable`** (`.data`, 5544 bytes =
**exactly 231 records** at stride 24, no remainder). So the slot count is 40
while the handle masks the slot index with `& 0xFF` — indices 0..255 are all
reachable and nothing bounds them.

**The handle is not a bare index.** `psndSFXChk` compares
`(handle >> 8) & 0xFFFF` against the halfword at `slot+0x04`, so the handle
packs slot index in bits 0..7 and channel allocation in bits 8..23, returning
`-1` on mismatch. This was not previously stated.

**Silent truncation.** `psndSFX_vol` and `psndSFX_dist` are `stb`/`sth` with no
clamp, and `psndSFX_pit` splits between a local `slot+0x14` store (when the low
5 bits of `slot+0x06` are clear) and a `SoundEfxSetPitch` call (when they are
set) — the same gate `psndSFXOff` uses to decide whether to stop anything.

**Correction to my earlier reply in this request.** I published three data bases
computed without the sign extension that `lis` performs, so they were wrong by
`0x3B0000`. Corrected values:

| Published earlier | Correct | Symbol |
| --- | --- | --- |
| `0x8002F988` | **`0x803DF988`** | `pssfx` |
| `0x80016B28` | **`0x80316B28`** | `pssfxlist` |
| `0x8004CF40` | **`0x8036CF40`** | `monosiriMessageTable` |

The same error affected seven field-collision contracts from the previous
session, where `lfs` constants were given as `0x800494xx`. Those are now in
their authoritative `N(r2)` form taken from the instruction bytes, and the
`lis`-derived globals are corrected to `0x80343B78` / `0x80343D88`, both of
which resolve into `.data`. A correction note is at the top of each affected
document.

One thing I initially "corrected" in the wrong direction and have restored: the
three values I first read as debug label **strings** (`0x80091434`, `0x80091504`,
`0x800915e4`) are code addresses — they are the symbol entry points of
`chkfilterVecVivian`, `chkfilterVecRoll` and `chkfilterVec`, and are passed to
`hitCheckVecFilter` as filter labels. Two of them were already correct in the
original text and are now documented as function pointers rather than strings.

### ID 3, 2026-10-12 — `__psndSFXOn` provisioning decoded

[Slot provisioning](../behavior/audio/POSITIONAL_SFX_SLOT_PROVISIONING.md).
`__psndSFXOn` 800d9934 is now decoded **end to end**, which closes the last
structural gap in the positional-SFX family: it is the only writer of the
`pssfx` fields the accessors read.

Recovered behaviour, all class B:

- **Name resolution is a `strcmp` scan.** The `key` argument is passed to
  `strcmp` against `pssfxlist[i].word0`, stride 20, up to **4242** entries.
  A key of `0` skips the scan and resolves to index 0; a scan that runs off
  the end also resolves to index 0. There is no not-found failure.
- **Three rejection gates before any slot is touched:** global `+0x56` bit
  29 as a mute that applies **only when `arg7 != 0`**; a signed-byte
  **cooldown of 6** at global `+0x5c` that rejects while nonzero and is set
  to 6 on every start; and, for pssfxlist index 869 only, an event flag at
  `0xF5DE0180` that must equal 2. Indices outside `<91`, `>=2250` and not
  445/447 bypass the event path; index 517 diverts to it.
- **Two-stage slot search.** First 20 slots for a free slot with the same
  `(key >> 19)`, bumping a preference cursor at global `+0x54` by **8** per
  reuse. If none, **40** slots are scanned and an occupied compatible slot
  is *reclaimed*: the body inlines a full copy of `psndSFXOff`, including
  the `slot+0x06 & 0x1F` gate, the `key >> 19` table index and the two-bit
  SoundSS/Song/Efx stop, then clears the record. Reclaim also requires
  `slot+0x0c == pssfxlist[+0x10]`. **Exhaustion returns `-1`.**
- **`slot+0x06` channel bits are a function of the pssfxlist index** —
  base `0x101`, then bits 5, 13, 14, 15 of the index plus per-index and
  per-field overrides. This is how similar sounds are routed to different
  hardware channels.
- **Handle packing confirmed:** `channel = ++global[0x50]`,
  `slot[+0x04] = (u16)channel`, return `slot_index | (channel << 8)`. This
  independently confirms the layout `psndSFXChk` validates. Note the
  asymmetry: `slot+0x04` gets the full 16-bit counter but only the value
  shifted by 8 reaches the handle, so the two diverge once the 16-bit
  counter wraps.
- **Two dead clamps.** At `0x800d9c60` and `0x800d9c70` both `clrlwi
  ...,24` then `cmplwi ...,255` can never be greater, so `li 20,127` and
  the `lbz 21,13(28)` default are **unreachable**. Recorded, not repaired.
- **Nothing here decrements the cooldown.** If `psndSFXMain` is not ported
  with its decrement, `global[0x5c]` stays nonzero and all further starts
  fail permanently. This is now the practical dependency.

**One item left deliberately unresolved, and it is worth flagging upstream.**
`__psndSFXOn` writes `slot+0x00` as the **bare pssfxlist index** (0..4241)
via `stw 18, 0(29)`. But `psndSFXOff` and `psndSFX_pit` both derive their
pssfxlist row from `(slot+0x00) >> 19`, which for 0..4241 is always **0**.
Taken literally the accessors would always resolve `pssfxlist[0]`. The
instruction bytes are unambiguous, so either the accessors are wrong, or
`slot+0x00` is meant to hold a packed key and `__psndSFXOn` is not the only
writer — but no other writer is visible. I have **not** guessed a
reconciliation. This should be settled before either side is relied on.

**Still open after this:** `psndSFXMain` (2964 bytes), which is now the
sole remaining item in the positional-SFX chain and owns the cooldown
decrement, the preference-cursor expiry and the reader of volume, distance
and the published position. Also open: the two distance constants at
`0x800E3C60`/`0x800E3C68` and `lfs -23412(r2)`, `pssfxlist` row fields,
Monosiri record layout and writers, `BtlUnit_CheckStatus`/`GetStatus`,
`SoundEfxSetPitch`, `irand(41)` domain. No original execution traces. No ISO
bytes were copied into the repository and no fabricated audio mapping is
claimed.

Validation: body re-disassembled from the read-only ISO with the ELF in
`/tmp` only; callees resolved against `us_symbols.csv`; every `lis`/`addi`
data base re-derived and cross-checked to land in `.bss`/`.data` against the
`pssfx` and `pssfxlist` symbols; `git diff --check` and link audit clean.
Docs-only, so `make test` / `make all` were not rerun.

**Still open:** `__psndSFXOn` 800d9934 (1344 bytes) remains the largest gap and
now blocks only the slot writers; `psndSFXMain` (2964 bytes) is the reader of
volume, distance and the published position; Monosiri record layout and table
writers; `BtlUnit_CheckStatus`/`GetStatus`; `SoundEfxSetPitch`; `irand(41)`
domain. No original execution traces. No ISO bytes were copied into the
repository and no fabricated audio mapping is claimed.

Validation: bodies re-disassembled per function with ELFs in `/tmp` only;
callees resolved against `us_symbols.csv`; `lis`/`addi`/`addis` arithmetic
re-derived in Python and cross-checked against named symbols in `us_symbols.csv`;
`git diff --check` clean; link audits clean. Docs-only, so `make test` /
`make all` were not rerun.

The append-only delivery notes, implementation breadcrumbs and host-gate history for this request live in [EVT-01_HISTORY.md](EVT-01_HISTORY.md). Validated deliveries are also registered in the [delivery log](DELIVERY_LOG.md).

### 2026-10-13 — Priority0 service consolidation and capture-order reconciliation

The [consolidated answer](../behavior/event/EVT_01_SERVICE_CONTRACT_AND_REMAINING_BINDINGS.md)
connects complete manager/interpreter/constructor/continuation contracts and
conditional NormalJump registration. Split broad todos into delivered static
portions and remaining source/runtime portions; request text remains untouched.
Corrected the unit-query shared-pipeline claim from independent read-only ISO
inspection of all six leaves, including neighboring CheckStatus/CheckData order.
Menu writers and Input cursor reuse are delivered; query-specific unsupported
stack domains remain open, with no invented fallback.

Validation: nine existing source verifiers pass (manager, root, descendants,
dispatch tail, run/control, menu domains, ActMenu, interruptions, Jump registry).
Inspection ELFs remain `/tmp/inbox-*.elf`. No native code or original assets
changed; no original execution trace was collected. Parent request remains OPEN.

### 2026-10-13 — Priority0 positional-SFX source corrections and update ownership

[Correction/update contract](../behavior/audio/POSITIONAL_SFX_UPDATE_OWNERS_AND_SOURCE_CORRECTIONS.md)
supersedes conflicting2026-10-12 audio delivery statements above. Rewrote the
complete provisioning, stop and accessor contracts against fresh US listings.
The previously claimed index-versus-packed-key conflict was a clrlwi decoding
error: readers use key&0x1fff. Off queued flag1 SET clears without engine stop;
flag1 clear dispatches through row+4 high descriptor bits. Name pointers are
signed-negative keys; nonnegative keys are raw numeric IDs. Default255 volume/
pan paths are live, mute override direction is corrected, reclaim uses strict
priority greater-than, generation wrap stays coherent. Cooldown decrements in
psndMain, not psndSFXMain; the latter resets preference at its tail.

Validation: nine independently regenerated body listings under `/tmp`;
13 body/table/numeric spans and78 source anchors pass the existing generic
DOL verifier with `tools/positional_sfx_corrections_evidence.json`. No new tool
code or native/asset change. Full spatial/backend/macro/resource/original
execution remains open, with no fabricated playback or sample mapping.

### 2026-10-13 — Monosiri table/source/census and message-key leaf

[Original table/consumers](../behavior/battle/MONOSIRI_TABLE_SOURCE_AND_CONSUMERS.md)
answers initialized231-row source provisioning, six used pointer fields,
complete get_monosiri_msg_no leaf and17 direct DOL calls plus2 module0 REL24
imports across29 shipped RELs. It is Tattle/message/model data, not spatial
sound. Corrected lookup arithmetic and invalid-domain null behavior: no guarded
null failure, but wrapping arbitrary kind can compute0. The actual sampling
pointer test and second fresh kind/lookup are preserved, not optimized away.
The event helper resolves a second operand even though its result is unused,
then captures raw output before fresh battle/unit lookup; no actor translation.
Raw identifiers are read from ISO strings rather than truncated symbol labels.

Validation:13 body/table fingerprints and40 source anchors pass generic DOL
verification; existing sampling verifier7 bodies/48 anchors/14 constants passes.
Independent direct-reference scan and US usage audit agree17 linked/0 nonlink/
0 data-word candidates plus2 REL imports. Independent table scan checks all231
records and column nonnull counts230/164/143/229/230/230; actual example IDs
and wrapped-zero derivation were verified. Inspection-only files remain `/tmp`.
No native tool/code or original asset/dialogue payload change; full table writer/
source-domain/Tattle/log/AC/persistence/runtime integration remains OPEN.

### 2026-10-13 — Tattle flag group/write/check and swdrv prerequisites

[Flag owners](../behavior/battle/MONOSIRI_FLAG_GROUPS_AND_SWITCH_STORAGE.md)
recovers full setter/checker with48 groups/247 source member occurrences,
persistent4474+kind writes, separate retained-BattleWork bits, policy bypass
and asymmetric original-kind checks. [swdrv storage](../behavior/memory/SW_SET.md)
closes12 local access/init/reset bodies and corrects the remainder encoding.
These flag writes do not populate the six-pointer Monosiri table.

Validation: generic read-only source verifier63 spans/205 anchors passes;
independent signed arithmetic8951 cases and source list scan pass. All original
inspection outputs remain `/tmp`. Table mutation/aliases/source domains, other
flag/policy writers, full Tattle/log/AC/scene/card lifetime and original runtime
remain OPEN. Request text remains unchanged; no native or original save claim.

### 2026-10-13 — Complete pending startup and six local effect backend bodies

[Pending region](../behavior/audio/POSITIONAL_SFX_PENDING_STARTUP_REQUESTS.md)
closes queued delay decrement/start order, exact stream/song/effect requests,
volume division/clamping, spatial packet byte extraction, aux and same-pass
live processing. [PlayEx](../behavior/audio/SOUND_EFFECT_PLAY_EX_ALLOCATION_AND_START.md)
closes separate40-record first-free/priority<=steal, post-KeyOff emitter flag,
interrupt nesting, raw controller arguments versus stored narrowed values,
new-voice failure and fresh-pool/final-liveness behavior. [Five controls](../behavior/audio/SOUND_EFFECT_RECORD_CONTROL_LEAVES.md)
close Check/Volume/Pitch/Aux1/Stop, including repeated versus single sndFXCheck,
raw controller values, fresh global zero-volume gate and flags-only stop clear.
Do not collapse public slot, effect record ordinal and underlying voice identity.

Validation: read-only generic verifier25 spans/143 anchors; independent census50
aligned direct call sites in six bodies and caller region resolves all targets.
Division127 arithmetic checks65026 byte-products plus195078 clamp cases.
Fresh listings remain `/tmp/inbox-fx*.elf` and source/current disassembly files.
Linked backend identities are explicitly separate from complete bodies. Full
calc3D/live/spatial/fade/MusyX/resources/hardware/source/original runtime still
OPEN. No native code/tool implementation, original asset export or playback claim.

### 2026-10-13 — Complete MusyX effect API, voice lookup and IRQ wrappers

[Eight local bodies](../behavior/audio/MUSYX_EFFECT_API_WRAPPERS_AND_VOICE_LOOKUP.md)
close retained raw arguments/core returns, default-table byte selection,
external-versus-internal voice identity, sorted-list lookup/null/miss behavior
and separate16-bit MusyX IRQ nesting. sndFXCheck preserves the INPUT external
ID when mapping exists; repeated source checks still perform fresh lookups
with an Enable/Disable interval. These bodies do not close synthFXStart,
controller/KeyOff core, resource loading or actual audible behavior.

Validation: source manifest12 body fingerprints (8 complete/4 linked-only)
and77 original anchors passes read-only generic DOL verification. Eight full
listings remain `/tmp/inbox-snd*.elf`, vidinternal and MusyX IRQ ELFs. Derived
IRQ boundary metadata distinguishes counts0/1/2/65534/65535 and wrap; no
original code executes. Parent remains OPEN for the core/source/runtime chain.

### 2026-10-13 — Synth FX resource/start and chained controller/KeyOff cores

[Complete local core request graph](../behavior/audio/MUSYX_SYNTH_EFFECT_RESOURCE_AND_CONTROL_CORES.md)
closes loaded-group FX lookup/shared scratch write, binary search/comparator,
low-byte255 defaults and all16 synthStartSound operands. It records both
controller cores' stale-generation early exit versus KeyOff's skip-and-continue,
raw MIDI argument widths, fresh synthVoice reload after requests and processed
return versus actual controller acceptance. No guessed macro/sample launch,
loaded-resource guarantee or hardware result is supplied.

Validation: read-only source manifest12 spans (8 complete/4 linked-only),192
original anchors and15 resolved aligned direct calls passes generic DOL verifier.
All new complete body listings were independently regenerated under `/tmp`;
vid lookup reuses its revalidated whole-body identity and complete prior contract.
Core synthStartSound/MIDI/keyoff mutations, resource registry producers, valid
voice/chain/cycle/source domains and original audio/runtime remain OPEN.

### 2026-10-13 — Complete FX registry insert/remove source producers

[Registry contract](../behavior/audio/MUSYX_FX_GROUP_REGISTRY_INSERT_AND_REMOVE.md)
closes low16 identity search, duplicate refcount (return0), initial128 cap versus
post-IRQ fresh publication count, retained entry pointers, live byte9=31 patch,
reference decrement/wrap and raw12-byte compaction. Removal does not clear tail,
free entry resources, restore byte9 or stop voices. The synthStartSound additional
operand is read from this live-mutated row, not unchanged original .proj metadata.

Validation: generic read-only verifier5 spans/87 anchors passes; both full
listings regenerated under `/tmp`. Refcount boundary vectors retain0/1/2/65535
wrap semantics. No native code, resource conversion or ISO mutation. Complete
sndPushGroup/sndPopGroup/residency/sorting/unload/voice/IRQ/source/runtime chain
remains OPEN; source request text remains unchanged.

### 2026-10-13 — Group lifecycle, pool lists and game loader bridge

[Complete group lifecycle](../behavior/audio/MUSYX_GROUP_PUSH_POP_AND_RESOURCE_LISTS.md)
closes early stack publication, five typed resource lists/ranges, conditional
sample registration, FX insertion and reverse-sample pop/removal order. Pop
requests emitter/sequence/macro/sample voice stops before resource removal;
child results/residency are not inferred. InsertData preserves exact pool offsets,
key tags, missing-record delegate calls and raw widths. [Game loader](../behavior/audio/GAME_SOUND_GROUP_WARMUP_AND_LOADER_REQUESTS.md)
closes busy/index/ready warm-up, bank captures, explicit-list versus sequential
count behavior, raw sample-address0 and temporary-buffer-only free.

Validation:29 body spans (6 complete/23 linked),476 anchors and56 direct call
sites pass generic verifier; original project resource hash also passes in its
resource-span slot (0 map nodes). Independent original pmario.proj parse finds
12 groups0..11,2998 list occurrences and matching forward/reverse samples in
all12. Group6 includes775/776 samples and994/995 macros. Fresh full listings/
derived reports stay `/tmp`; no original asset export or native change. Child
SDir/pool/sample/stop, actual residency/sorting/voice/source/hardware/original
execution remains OPEN. Source request text is unchanged.

### 2026-10-13 — Sample directory/reference lifetime and storage bridge

[Four complete directory/reference bodies](../behavior/audio/MUSYX_SAMPLE_DIRECTORY_REGISTRATION_AND_REFERENCES.md)
close preferred-pointer publication even on refusal, duplicate/capacity behavior,
low16 ref wrap, first-acquire address publication and hardware rewrite, zero-ref
release and ordered registry compaction. Missing preferred inactive candidate
has no guarded failure return; valid sample/directory identity is a prerequisite.
Removing a directory does not reset the preferred pointer or free sample data.
[Four hardware bridge bodies](../behavior/audio/MUSYX_SAMPLE_STORAGE_HARDWARE_BRIDGE.md)
close raw save/remove/cache requests, identity translation and queue-sync request.
Encoded counts are literal format routes; format0/1/4/5 byte sizing is proved
ceil(N/14)*8 throughout the24-bit source domain, separate from audible duration.

Validation: read-only manifest13 spans (8 complete/5 linked-only),256 instruction
anchors,10 independently resolved direct calls and original .sdir resource hash
passes. Independent directory check1247 records, all original refs0/formats0;
Hammer775/776 transfer sizes3016/872 bytes.54 derived format/count vectors and
whole-domain arithmetic proof do not execute original code. Fresh inspection
ELFs/listings stay `/tmp`; no native code or original asset export. ARAM/cache/
DMA/voice/macro/loading/valid-source domains and original traces remain OPEN.

### 2026-10-13 — ARAM sample store, cursor rewind and queue completion

[Six complete store/config/address bodies](../behavior/audio/MUSYX_ARAM_SAMPLE_STORE_AND_CURSOR_REWIND.md)
close wrapping32-byte size alignment, cache-dependent capacity limits, retained
return versus fresh destination/cursor, direct and callback chunk uploads,
max-based chunk configuration and raw address/INPUT-byte leaves. Removal ignores
the address and only rewinds cache.word8; it does not free/erase/cancel/stop.
[Two complete queue bodies](../behavior/audio/MUSYX_ARAM_QUEUE_COMPLETION_AND_SAMPLE_REQUESTS.md)
close selected-queue full-slot callback scan/unconditional count decrement and
low-only busy synchronization. Source sample posts precede count/index updates;
invalid pointer/count and callback/chunk/index domains remain prerequisites.

Validation: read-only manifest16 spans (8 complete/8 linked SDK/helper),207
anchors and16 resolved direct calls passes generic verifier. Derived checks:
8 alignment/rewind boundary vectors including wrap, all256 possible byte index
updates,5 completion-count boundaries. These arithmetic cases are not original
execution. Fresh full ELFs/listings remain `/tmp/inbox-aram*`; no native change or
original bank/code-body export. Init/stream/other queue producers/cache providers,
SDK flush/interrupt/ARQ/DMA and original voice/audio traces remain OPEN.

### 2026-10-13 — ARAM initialization, hook and stream-buffer source owners

[Initialization and hooks](../behavior/audio/MUSYX_ARAM_INITIALIZATION_AND_ALLOCATOR_HOOKS.md)
close zero allocation/fill/flush, queue reset/post/count order, low-only completion
wait before free, two-read size clamp and stream initialization request. Hook leaves
invoke live salHooks with no guard or added allocation semantics.
[Stream-list contract](../behavior/audio/MUSYX_ARAM_STREAM_BUFFER_LISTS_AND_LIFETIME.md)
closes64-node idle initialization, cached-capacity search, downward allocation,
low-byte getter/default pointer and free traversal. Larger-candidate predecessor
is NOT retained alongside the candidate; tail cleanup resets the global cached
head per qualifying node. Exact writes remain distinct from assumed valid list
invariants or a corrected allocator. Miss/double-free does not stop mutation.

Validation: read-only manifest16 spans (8 complete/8 linked),251 anchors and14
resolved direct calls passes. Derived64-link chain,8 alignment/7 index/7 search
vectors checked; a larger-candidate predecessor example is static arithmetic,
not original execution. Complete ELFs/listings remain `/tmp`; no native change
or original code-body/bank export. Default-cache/hook providers, source/list/
cycle/order/membership domains, stream upload/SDK/DMA/voice/original traces OPEN.

### 2026-10-13 — Generic ARAM upload and hardware stream request bridge

[Five complete local bodies](../behavior/audio/MUSYX_ARAM_UPLOAD_AND_HARDWARE_STREAM_BRIDGE.md)
close raw upload words, full-word nonzero→high/priority1, user callback/argument,
post-before-count/index and no completion wait. hwFlushStream requests the ordinal
address first, ignores the returned descriptor size, aligns offset/length with
word wrap, requests DCStoreRange and posts high. Init/exit/play-address wrappers
preserve exact child arguments/returns without extra lifetime or status policy.
The play-buffer wrapper does not query hardware play position.

Validation: read-only manifest16 spans (5 complete/11 linked),114 anchors and14
resolved direct calls passes. Whole-DOL aligned direct census25 sites:17 Flush/1
Get in streamHandle,1 Init in sndStreamAllocEx,5 Exit in sndStreamFree,1 upload in
Flush. Eight priority and42 offset/count boundary vectors are derived evidence;
no original function executes. Fresh full ELFs/listings stay `/tmp`; no native
change/code-body/bank export. Caller/indirect/REL/dynamic/source/extent/lifetime/
SDK/cache/ARQ/DMA/original audio traces remain OPEN.

### 2026-10-13 — Default cache, resource reset and hook installation

[Seven complete producer/wrapper bodies](../behavior/audio/MUSYX_DEFAULT_CACHE_RESOURCE_RESET_AND_HOOK_INSTALLATION.md)
close ordered gsDefault/root/current/cache base/end/cursor publication, six U16
registry resets and800hex-byte macro main-table clear. hwInitSampleMem forwards
only original r4 as aramInit's input. sndSetHooks reads two input words and writes
allocator twice before reading/publishing free; aliasing is order-sensitive.
Ordinary salMalloc and public/hardware upload-configuration forwarding preserve
raw inputs/child returns, without independent success/lifetime policy.

Validation: read-only manifest9 spans (7 complete/2 linked documented children),
87 anchors/four resolved direct calls passes; four derived cache/wrap vectors,
exact macro-clear extent and three hook alias vectors checked. Full inspection
ELFs/listings remain `/tmp`; no original code execution/native change/bank export.
Caller/provider/alias/reset/concurrency/residency/per-kind registry/SDK/voice and
original setup/audio traces remain OPEN. Local portions checked, parent open.

### 2026-10-13 — Game allocator providers and MusyX bootstrap callers

[Four complete bodies plus scoped game prefix](../behavior/audio/GAME_AUDIO_ALLOCATOR_AND_MUSYX_BOOTSTRAP.md)
close heap0 allocator/free requests, actual two-word hook table installation,
fixed game sndInit arguments, low-byte configuration/caps, hardware failure
returns and successful resource/sequence/synth/stream/virtual-sample/3D order.
Default-cache base/size and dataInit raw inputs now have a complete local caller.
Hardware failures do not unwind earlier child mutations; game startup prefix
ignores sndInit result. Active1 is local publication, not residency/playback proof.

Validation: read-only manifest29 spans (4 complete/1 scoped prefix/1 original
hook table/23 linked),134 anchors and25 resolved direct calls passes. Original
8-byte table independently equals sndMalloc/sndFree; nine derived width/clamp
vectors checked. Full listings/ELFs remain `/tmp`; no original code executes or
body/bank asset is exported. Whole SoundInit/child OS/ARAM/AI/DSP/IRQ/synth/stream/
source/provider/reset/concurrency/failure/original startup/audio remain OPEN.

### 2026-10-13 — AI buffer, callback and IRQ initialization source

[Six complete local bodies](../behavior/audio/MUSYX_AI_BUFFERS_CALLBACK_AND_IRQ_INITIALIZATION.md)
close initial OS saved state/nesting1, null allocation publication, four-slot clear/
flush and AI state/registration/physical DMA/rate writes. Callback rotates DMA and
publishes OS tick BEFORE DSP-done/active gates; dispatch enables OS interrupts,
loads fresh callback, then disables and clears active without restoring saved state.
CPU destination is two slots ahead; compressor only publishes a flag. No measured
frame/DMA timing or original audible output is inferred.

Validation: read-only manifest15 spans (6 complete/9 linked),83 anchors and11
resolved direct calls passes. All256 byte-index arithmetic cases, exact4*280=a00
extent and9 logic-gate vectors checked; source functions not executed. Fresh
ELFs/listings remain `/tmp`. SDK/DMA/DSP/frame providers, shared-state writers,
null/alias/reinit/concurrency and original callback/audio traces remain OPEN.

### 2026-10-13 — DSP task, ready callbacks and command-mail source

[Six complete bodies](../behavior/audio/MUSYX_DSP_TASK_CALLBACKS_AND_COMMAND_MAIL.md)
close task fields/request order, init-flag clear AFTER task addition and nesting
wait; Init writes ready/init flags, Resume consumes waiting before active gate,
Done only wakes a thread queue. Control retains command pointer before ready-clear/
sync, captures first size afterward, then sends header/pointer with separate mail
polls. Raw delay retains word-wrap/high-product/divisor-zero domain distinctions.
No DSP decoding/command execution/actual rate or audio timing is inferred.

Validation: read-only manifest21 spans (6 complete/13 linked/2 original program
and length),98 anchors/16 resolved direct calls passes. Original length1ec0
matches program span;9 gate/5 mail/6 clock-domain vectors are derived only.
Fresh full ELFs/listings remain `/tmp`; no original function executes or DSP/bank/
code body is exported. SDK/task/mail/builder/state/thread/clock/concurrency and
original startup/resume/audio remain OPEN.

### 2026-10-13 — Complete audio frame and auxiliary callback request graph

[Seven complete local bodies](../behavior/audio/MUSYX_AUDIO_FRAME_AND_AUX_CALLBACK_ORDER.md)
close one-time active gate, DSP-before-updates graph, distinct direct OS critical
pairs, frame byte advance/live per-field voice-pointer reload, five ascending
seq/synth requests and final3D/stream/virtual-sample order. Aux processing retains
callbacks/buffers but reads B gate/arguments after A, flushes retained buffers and
rechecks live studio count. No inferred physical frames/action ticks/input samples.

Validation: read-only manifest17 spans (7 complete/10 linked),127 anchors/29
resolved direct calls passes. All256 byte arithmetic cases, five-slice order,
three-channel extent and6 aux gates checked as static deductions. Full listings/
ELFs remain `/tmp`; no original code executes or body/bank asset is exported.
Child seq/synth/stream/3D/virtual-sample/builder, shared-state/providers/format/
IRQ/reentrancy and original frame/aux/audio traces remain OPEN.

### 2026-10-13 — Complete calc3D spatial result and angle table

[Two complete local bodies](../behavior/audio/GAME_CALC3D_SPATIAL_RESULT_AND_ANGLE_TABLE.md)
close relative-then-replace point flags, retained heading/fresh listener reads,
91-row table angle branches, literal196 pan fold, reciprocal sqrt refinement,
cos/sin surround transform, offset200/range100*distance falloff and byte-first
volume clamp. Shared result writes only bytes0..2 and returns retained byte3
with the packed word. No host atan2/sqrt replacement or audible mapping invented.

Validation: read-only manifest9 spans (2 complete/2 linked libm/table/4 literal
spans),161 anchors/four resolved direct calls passes. Original91 table rows and
literal bytes independently checked;15 signed pan-fold boundary vectors remain
static deductions. Full ELFs/listings stay `/tmp`; no original execution/body/
bank export. Listener/shared source writers, finite/exception/PPC/libm numeric
invariants, live SFX/backend and original spatial/audio traces remain OPEN.

### 2026-10-13 — Listener setter and scoped sequence camera bindings

[Listener source contract](../behavior/audio/GAME_LISTENER_SETTER_AND_SEQUENCE_CAMERA_BINDINGS.md)
closes X/Y capture then stores before Z read, heading stfs and raw pointer return;
camGetPtr is unchecked shifted table lookup. Field caller copies first camera4
position then second camera4 heading; battle uses fixed(0,0,400), bit100→Z600,
then fresh camera4 heading. Whole sequences/gates/cadence and camera providers
remain separate; no coherent listener snapshot or unsupported normalization.

Validation: read-only manifest6 spans (2 complete/2 scoped/1 linked/1 literal),
46 anchors/8 resolved direct calls passes. Whole-DOL aligned setter census2 sites;
original literals0/400/200 independently decoded. Alias vector is static deduction,
not original execution. Fresh ELFs/listings remain `/tmp`; no native/body/asset
export. Camera/heading/battle flag/shared writers, sequence gates, indirect/REL/
dynamic/source domains and original spatial/audio traces remain OPEN.

### 2026-10-13 — Complete remaining SFXMain live/fade/mute request regions

[Live contract](../behavior/audio/POSITIONAL_SFX_LIVE_SPATIAL_FADE_AND_MUTE.md)
closes global fade decrement/raw factor and signed percentage producer, spatial
calc3D versus nonspatial controls, backend-specific scaling, first mute, bit40
fade and zero-result fresh slot/descriptor stop/clear, then final mute even after
clear. Composition with previous sweep/startup/loop/tail closes the local entire
psndSFXMain graph. No reordered suppression or backend success inference.

Validation: read-only manifest18 spans (2 scoped/15 linked/whole fingerprint),
446 anchors/35 resolved direct calls passes.65536 division127 byte products and
65536 division100 unsigned×signed byte products checked; six fade wrap/decrement
vectors are derived source cases, not original execution. Full ELF/listing stays
`/tmp`. SS/song/effect child services/writers/flags/fade/denominator/source/alias/
concurrency/lifetime/original audio remain OPEN; source request text unchanged.

### 2026-10-13 — Effect spatial controllers and emitter query leaves

[Five complete local bodies](../behavior/audio/SOUND_EFFECT_SPATIAL_CONTROLS_AND_EMITTER_LOOKUP.md)
close pan/surround stores before fresh pool/voice lookup, retained raw controller
arguments, surround131 then132 mapping and low-byte stored-volume read. Emitter
Check returns active-gated bit10000; VoiceID reads retained word38 under MusyX
IRQ without independently checking voice validity. Stored state differs from
actual playback; first controller failure does not suppress the second request.

Validation: read-only manifest10 spans (5 complete/5 linked),75 anchors/11
resolved direct calls passes.256 surround/nine raw-width/15 emitter flag vectors
checked as static deductions; full fresh listings/ELFs remain `/tmp`. No native
change/original execution/body/bank export. Pool/emitter writers/ordinal/null/
lifetime/concurrency/controller/MIDI/voice/original spatial/audio remain OPEN.

2026-10-13 setup4 intro/phase/player-source follow-up:
[Setup4 intro/phase/stage/player bindings](../behavior/battle/UNDERGROUND_SETUP4_INTRO_PHASE_STAGE_AND_PLAYER_COMMANDS.md)
pins kindcount/table/init offsets andfourbodypart records, conditionalfresh
registry IDs0system/1Mario/2Goombella/3..6sourcegroup, concurrent trackedentry
barriersbeforeselector0Franklyphase(channel4exact04000001/turn1) beforemenu.
Actualsource bti_01/stg_00_1 m/d,t resources/hash evidence andidentical8x8
b/tpl stubs; exactMovePosition45 is callbackcountwithsoundmode-1, notspeed.
Jump/Hammer andHeadbonkpower/action sourcebindings supplied; Headbonkpouch+0xa
attacklevel differsfromtechmenu+0xc, its227commandgraph identified.
NEW boundedproducer requests: executed tik4 startingfirstAttackType/phase/turn
writers andentry/curtain/runtime barriersequence; sourcecandidategeometry/
weaponmask/commandconfirmation domains; compositebattle-stageplacement/object
providers; actualstartingpouchrank/loadout; fullHeadbonk/Hammer/partyinit service
composition,pad/impactclocks andbattleloss/escape resultproducer routing.
Keep these pendingwhileimplementing explicitlyscoped best-effortproviders.
No nativeedit/commit ororiginalexecution claim.

### 2026-10-13 — Emitter individual/all/FX-array lifecycle bodies

[Three complete local bodies](../behavior/audio/MUSYX_EMITTER_REMOVE_ALL_AND_FX_LIFETIME.md)
close active-system admission versus removal return, conditional inlined unlink,
all-high16 flag clear, retained links/voice and key-off request. Bulk paths retain
next before mutation and follow it even when inactive; FX-array first-match
breaks even when mutation is skipped. No node free/instant voice-kill invented.

Validation: read-only manifest6 spans (3 complete/3 linked),117 anchors/9
resolved direct calls passes. Whole-DOL aligned caller census9 sites;6 flag-clear/
12 active-gate vectors derived only. Full ELFs/listings stay `/tmp`; no original
execution/body/asset export. Emitter construction/list/FX/member/cycle/IRQ/source,
full caller unload/silence and original key-off/audio/lifetime remain OPEN.

### 2026-10-13 — Emitter vector/volume update and 3D root reset

[Two complete local bodies](../behavior/audio/MUSYX_EMITTER_VECTOR_VOLUME_UPDATE_AND_3D_RESET.md)
close active-gated two-vector publication, low-byte volume/127 target and ordered
current>target clamp, independent of emitter membership. 3D reset zeros roots/
used-studio word, assigns base1/max3/count0 and flags bits1/2 without clearing
old nodes, voices or active flags. No smoothing/cadence/release rule invented.

Validation: read-only manifest6 spans (2 complete/2 IRQ/2 literals),47 anchors/
two resolved direct calls passes; whole-DOL direct census2 sites.8 volume/7
configuration/5 finite clamp vectors are derived only, not PPC execution.
Full ELFs/listings remain `/tmp`; no native change/body/asset export. Constructors/
consumers/caller-provider/alias/null/finite/reset/concurrency and original audio
remain OPEN; local portions marked, parent open.

### 2026-10-13 — Game effect main and outer sound-update services

[Three complete local bodies](../behavior/audio/GAME_EFFECT_MAIN_LISTENER_EMITTER_AND_COMPLETION.md)
close listener displacement/sin-cos basis arguments, first40 emitter updates,
second40 captured traversal versus fresh pool voice checks/cleanup, double
sndFXCheck and countdown continuation after initial cleanup. SoundMain checks
exact sequence-valid1, flags/stops invalid records then orders effect/DVD/SS;
interrupt wrapper only requests SSMainInt. No measured velocity/cadence invented.

Validation: read-only manifest21 spans (3 complete/14 linked/4 literals),135
anchors/25 resolved direct calls passes.40 offsets/four countdown boundaries/
double-query metadata derived only; full fresh ELFs/listings remain `/tmp`.
No native changes/original execution/body/bank export. Listener/providers/pool/
sequence metadata/writers/alias/concurrency/SS/DVD/controller/voice and original
completion/displacement/audio traces remain OPEN.

### 2026-10-13 — Listener update, cross product and matrix inverse

[Three complete local bodies](../behavior/audio/MUSYX_LISTENER_UPDATE_CROSS_AND_MATRIX_INVERSE.md)
close active-gated four-vector ordered copies, cross-product row basis/position
stack matrix, inverse cofactor/translation store order and low-byte volume/127.
No normalization/singular fallback/status rejection or in-place alias safety
invented. Math uses separate single instructions; child result is not checked.

Validation: read-only manifest10 spans (3 complete/4 linked/3 literal),229
anchors/six resolved direct calls passes. Derived stack/store maps/two admitted
matrix examples/one cross example are static deductions, not original numeric
execution. Full fresh ELFs/listings stay `/tmp`; no native/body/asset export.
Listener/basis/providers/alias/null/finite/invertible/PPC/CalcEmitter consumers
and original geometry/audio remain OPEN.

### 2026-10-13 — Sequence validity/private-ID and stop lifecycle

[Four complete local bodies](../behavior/audio/MUSYX_SEQUENCE_VALIDITY_PRIVATE_ID_AND_STOP.md)
close active-before-paused low31 key lookup/tagged private-byte return, boolean
validity, IRQ stop wrapper, high-bit efc-clear versus untagged state-specific
unlink/note-kill/recycle paths and shared instance-free insertion. No private
index guard, unknown-state refusal or audible validity guarantee invented.

Validation: read-only manifest9 spans (4 complete/5 linked),157 anchors/8
resolved direct calls passes. Nine validity/five width/five state vectors and
original8-instance extent checked as source deductions. Full listings/ELFs stay
`/tmp`; no original execution/native/body/asset export. ID/state/list/note/efc/
index/cycle/alias/concurrency/voice/resource and original stop/audio remain OPEN.

### 2026-10-13 — Sequence initialization and free-chain source producer

[Complete seqInit](../behavior/audio/MUSYX_SEQUENCE_INITIALIZATION_AND_FREE_CHAINS.md)
closes empty active/paused roots, eight-instance chain/private IDs0..7/state0,
128 MIDI-priority halfwordsffff,256-node note chain and final next-ID0. Note
+18 stores target the next20-byte record's previous link; other payloads survive.
No voice/resource release or whole-instance zero inferred from root/state reset.

Validation: read-only manifestone whole span/88 anchors/no calls passes. Exact
8-instance/256-note/128-halfword extents and note block simulation512 link
assertions/in-bounds writes checked as deductions. Full ELF/listing stays `/tmp`;
no original code executes/body/asset exports. Constructors/ID/list/note/payload/
priority consumers/voice/resources/reset/concurrency/original audio remain OPEN.

### 2026-10-13 — Sound-chain kill and internal allocator recycling

[Two complete local bodies](../behavior/audio/MUSYX_VOICE_SOUND_KILL_AND_ALLOCATOR_RECYCLE.md)
close ordered external-map lookup, prefetched successor across identity/kill,
stale skip/continue and request-based return0. Internal kill retains record across
map/macro children, unlinks priority chains, conditionally appends free voice/
decrements byte counts, clears identity, then requests stream/hardware even
when macro-active word was0. No CPU free/audible completion inferred.

Validation: read-only manifest6 spans (2 complete/4 linked),124 anchors/five
resolved direct calls passes.5 mask/4 counter/6 index/4 chain-route vectors are
source deductions, not execution. Full fresh ELFs/listings stay `/tmp`; no native/
body/asset export. Map/macro/stream/hardware/voice-priority-free providers and
index/member/chain/cycle/IRQ/concurrency/original audio remain OPEN.

### 2026-10-13 — Stream kill/reset and hardware break/active record source

[Four complete local bodies](../behavior/audio/MUSYX_STREAM_KILL_INITIALIZATION_AND_HW_BREAK.md)
close64-record first state1/2 voice match, optional Unblock then state3/fresh
callback(0,0,0,0,user), configured-count-only reset with fresh tail count, and
hardware f2/event-word publication. Active query reports any nonzero f0 while
Break's f2 requires exact1/offset0. No actual hardware completion inferred.

Validation: read-only manifest5 spans (4 complete/1 linked),78 anchors/one call
passes.10 initialization/10 kill-route/16 break vectors are derived only; full
fresh ELFs/listings stay `/tmp`. No original execution/native/body/asset export.
Unblock/callback/state/count/index/DSP provider-consumer/reset/concurrency and
original stream/voice shutdown/audio remain OPEN.

### 2026-10-13 — Macro active/inactive/wait/init local lifecycle

[Four complete bodies](../behavior/audio/MUSYX_MACRO_ACTIVE_INACTIVE_WAIT_AND_INIT.md)
close equality early return, active/time unlink, zero/all-ones wait distinction,
low-then-high clear/time publication, literalfffbfffb flags, forced two-job request
and live-count/per-field pointer macro init. No member/unknown-state/alias repair
or inferred macro scheduling/voice/resource release.

Validation: read-only manifest5 spans (4 complete/1 linked),125 anchors/three
resolved calls passes;6 wait/5 mask/7 state vectors/job order are static deductions.
Full fresh ELFs/listings stay `/tmp`; no original execution/native/body/asset
export. Job/interpreter/wait/state/list/voice/provider/IRQ/concurrency and original
macro/activation/audio remain OPEN.

### 2026-10-13 — Synth job insertion and initial timestamp requests

[Two complete local bodies](../behavior/audio/MUSYX_SYNTH_JOB_INSERTION_AND_START_TIMESTAMPS.md)
close32-slot selection, three embedded categories, category0/1 same-slot return/
reschedule versus category2 any-queued return, exact unlink/prepend writes and
two independently captured real-time pairs before0/1 requests. Unsupported raw
categories lack initialized pointers; no safe default/no-op invented.

Validation: read-only manifesttwo spans/75 anchors/two resolved calls passes.
21 slot/3 category/9 duplicate/two time mappings are source deductions, not
execution. Fresh full ELFs/listings stay `/tmp`; no native/body/asset export.
Queue initialization/markers/index/time/consumer/type/member/alias/IRQ/concurrency
and original scheduling/macro/voice/audio remain OPEN.

2026-10-13 trio target/turn/result follow-up:
[Trio targets, turns and victory](../behavior/battle/UNDERGROUND_TRIO_TARGETS_TURNS_AND_VICTORY.md)
usesexistingcompletecandidate/UI/countcontracts withactualdefaultweaponmasks
andfreshtrio sourcecandidates: Jump/Headbonkall3includingselectablespike;
HammerfrontGoombaonlyuntilremoved,airborneexcluded. Exactbodygeometry/bias
importoffsets, precheck1(finalaccuracydraw) vslaterunresistedtopspikecounter
1damage/return-1, firstturncounts/phaseIDs, victoryresult1producerandflag31
completionpublication recovered. Boundedremaining requests: actualsource
HP→status27death/hit-event/barrier producer (CheckStatus27doesnotreadHP),
usedmenu/turn/orderchange/headbonk/guard/servicecomposition, completeend
reward/override/loss/escape/fieldreturn andoriginaltrace. No nativeedit/commit.

### 2026-10-13 — Scoped synthesis queue consumer, voice controls and clock tail

[Complete local regions](../behavior/audio/MUSYX_SYNTH_JOB_DRAIN_VOICE_CONTROLS_AND_CLOCK.md)
close active sample-info gate/mac request, category0→2→1 captured-next/markerff/
root-clear order, stream skip, pedal/start/key-off/ADSR/dirty flags, fresh slot
advance and64-bit raw-input time addition. FrameDone is a no-op. Remaining
synthHandle master-fade/aux section is explicitly NOT closed by its fingerprint.

Validation: read-only manifest13 spans (2 regions/1 complete leaf/9 linked/whole
fingerprint),163 anchors/10 resolved calls passes.3 queue/5 index/5 clock vectors
and65536 pedal cases derived only. Full fresh ELF/listing stays `/tmp`; no native/
body/asset export. Remaining middle/child/provider/member/alias/IRQ/concurrency
and original scheduling/voice/audio remain OPEN.

### 2026-10-13 — Remaining synthHandle fade/aux region and local closure

[Complete final local region](../behavior/audio/MUSYX_SYNTH_MASTER_PAUSE_FADES_AND_AUX_CALLBACKS.md)
closes zero-offset admission, live master versus captured pause bitmap clear,
ordered single coefficient updates/snap/stop-pause-mute requests, early empty
scan exit and8-studio A→B four-param/tag1 callbacks. Prior queue/voice/clock plus
this region covers local entire synthHandle, separate from child execution.

Validation: read-only manifest9 spans (region/6 linked/whole fingerprint/zero),
150 anchors/six resolved calls passes.6 modes/5 bitmaps/16 studio-side rows are
source deductions, not execution. Full listing/ELF stays `/tmp`; no native/body/
asset export. Fade/mode/providers/numeric/IRQ/concurrency/seqPause-Mute/aux and
other children/callback installation/original scheduling/audio remain OPEN.

### 2026-10-13 — Sequence pause/mute and public mute wrapper

[Three complete local bodies](../behavior/audio/MUSYX_SEQUENCE_PAUSE_MUTE_AND_DEFERRED_FIELDS.md)
close inline active/paused lookup, untagged state1 active→paused publication
before note voice-kill/recycle, tagged nonzero-state pause bit8, unconditional
mute raw words/tagged bit10+deferred words and raw public IRQ forwarding.
No invented audible silence timing, private-index repair or extra state gate.

Validation: read-only manifest6 spans (3 complete/3 linked),160 anchors/five
resolved calls passes.10 pause/6 mute/5 flag vectors/three note-head order are
source deductions; full fresh ELFs/listings stay `/tmp`. No native change/original
execution/body/asset export. Private/state/list/note/deferred consumers/resume/
voice/source/index/alias/IRQ/concurrency/original audio remain OPEN.

### 2026-10-13 — Sequence continue and missing-ID domain

[Two complete local bodies](../behavior/audio/MUSYX_SEQUENCE_CONTINUE_AND_MISSING_ID_DOMAIN.md)
close state2 paused→active publication, tagged bit8 clearing without a state
gate and raw public IRQ forwarding. Continue lacks the lookup-miss guard found
in pause/mute/stop: miss ffffffff takes the tagged route and derives byte
803eb432 before seqInstance. This is static address evidence, not an observed
corruption or an invented graceful no-op. Caller lookup success remains a
required source/domain contract.

Validation: read-only manifest four spans (two complete/two linked), 53 anchors
and three resolved calls passes. Seven ID/tag/address, five flag-clear and five
state-route vectors are deductions; fresh full ELFs/listings stay `/tmp`.
No native change/original execution/body/asset export. Caller private-ID/state/
list/track/note/deferred/alias/IRQ/concurrency and original resume/missing-ID/
audio traces remain OPEN.

### 2026-10-13 — Sequence speed/volume and group forwarding

[Four complete local bodies](../behavior/audio/MUSYX_SEQUENCE_SPEED_VOLUME_AND_GROUP_FORWARDING.md)
close16 track speed halfword stores, tagged speed flag/payload order, immediate
volume primary request then64 fresh group-byte comparisons/requests, four tagged
volume modes and public raw IRQ forwarding. Speed shares continue's unguarded
lookup miss: OR20 at byte803eb432 then halfword803eb430 store. Volume instead
returns on miss. Repeated secondary groups are not deduplicated; primary group
is freshly read per comparison. No audible tempo/fade timing is inferred.

Validation: read-only manifest nine spans (four complete/five linked),137
anchors/ten calls passes. Eight halfword/seven address/16 mode/15 flag vectors,
16 speed offsets and three64-byte group-order examples are deductions. Fresh
ELFs/listings stay `/tmp`; no native change/original execution/body/asset export.
synthVolume/deferred/track consumers, ID/group/mode/time providers, lookup-success/
index/bounds/alias/IRQ/concurrency and original tempo/volume/miss/audio remain OPEN.

### 2026-10-13 — Synthesis volume selectors and fader publication

[Complete synthVolume and duration converter](../behavior/audio/MUSYX_SYNTH_VOLUME_SELECTORS_AND_FADE_PUBLICATION.md)
close six row-type selector scans/unchecked direct indexing, exact target and
coefficient literals, low16 duration<<8, mode/ID publication, zero-duration
immediate sequence dispatch and final fresh bitmap OR. Zero duration still
sets active while preserving earlier start/coefficient fields. Special scans
store IDffffffff; no invented direct bounds repair, volume clamp, coefficient
reset or original physical fade measurement.

Validation: read-only manifest11 spans (two complete/five linked/four literals),
192 anchors/15 calls passes.256 selector/eight duration/eight target/ten mode/
six row-type vectors checked as deductions. Fresh full ELFs/listings stay `/tmp`;
no native change/original execution/body/asset export. Fader initialization/other
writers, caller/group/ID/numeric/alias/IRQ/concurrency domains, deferred consumers
and original zero/nonzero-duration/tempo/fade/audio remain OPEN.

### 2026-10-13 — Synthesis initialization, row types, jobs and callback

[Four complete local bodies](../behavior/audio/MUSYX_SYNTH_INITIALIZATION_TYPES_JOBS_AND_CALLBACK.md)
close raw allocation/count/voice field order, partial fader initialization and
row21/22 current overrides/row23..30 type0/row31 type1, bitmap clear, aux domains,
child ordering, global/job resets and callback installation. The initializer
does not clear whole fader records or activate sndActive. Public volume forwards
mode0/missing ID; row-type setter gates on sndActive and directly indexes the
low8 group without a32-row bound. No invented full-reset/reinit repair.

Validation: read-only manifest19 spans (four complete/13 linked/two literals),
279 anchors/13 calls passes.32 fader rows/96 job words/34 constant+three ordinal
voice fields/seven count/six setter/six reset-domain vectors are source-derived.
Fresh full ELFs/listings stay `/tmp`; no native change/original execution/body/
asset export. Child/controller/allocation/voice/callback services, other writers,
caller bounds/null/overflow/alias/IRQ/concurrency/reinit and original init/audio
remain OPEN.

### 2026-10-13 — Installed hardware messages and sample notifications

[Five complete local bodies](../behavior/audio/MUSYX_HW_MESSAGE_AND_SAMPLE_NOTIFICATION_ORDER.md)
close exact four-message dispatch, hardware virtual-ID lookup, virtual end
map/generation/callback/fresh cleanup, macro state/event/flag wakeup and virtual
start reclaim/first-free reservation/map publication/candidate generation/
payload/callback-zero acceptance versus buffer+slot cleanup. Virtual end runs
before message0's fresh full-ID macro check; message3 has no identity/gate/macro
path. Only message2 propagates a child result. No invented callback success
convention, reservation rollback or post-callback identity repair.

Validation: read-only manifest16 spans (five complete/11 linked),201 anchors/
20 calls passes. Nine message/eight ID/15 generation/eight wakeup/four callback/
five stable-slot vectors are deductions; full fresh ELFs/listings stay `/tmp`.
No native change/original execution/body/asset export. Table/map/count/counter/
callback/voice/event/flag providers, hardware/VID/sequence-instance children,
actual callback mutations, bounds/alias/IRQ/reentrancy/concurrency and original
start/end/refusal/reuse/message/audio traces remain OPEN.
