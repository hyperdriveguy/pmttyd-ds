# EVT-01 — Native event scheduler and dispatch closure

**Status: OPEN (partially answered).** Part of the [request inbox](../DOCUMENTATION_REQUESTS.md); validated deliveries and locks are registered in the [delivery log](DELIVERY_LOG.md).
Request text below is preserved verbatim from the inbox; answers and implementation breadcrumbs are append-only history. Updated 2026-10-12.

## Open todos

- [ ] Complete US `evtmgrMain`/`evtmgrCmd` service contract: eligibility
  masks, 64-bit lifetime increment source/rounding, credit
  precision/subtraction order, interpreter 0/-1/1 handling, current-table
  appends and live-ID checks; initial credit/state/type/run-check policy for
  root/child/brother/inline entries; `WAIT_FRAME`/`WAIT_MSEC`/`USER_FUNC`
  continuation semantics with exact first-call markers. Keep mathematical
  event invocations separate from physical pad samples. *(Partial deliveries:
  manager loop, constructors, dispatch/wait continuations, controls and
  opcode coverage are registered; original pause/multicredit/creation/
  deletion traces remain.)*
- [ ] Complete `BtlUnit_snd_se_pos` 80126088/114, `psndSFX_pos` 800d96f8/4c
  and `psndSFXOff` 800d95f8/c8 bodies with their `__psndSFXOn`/macro/voice/
  resource chain (raw sentinel/key/unit/belong/position capture, returned/
  invalid handle behavior, updates/stops, source lifetime, original lookup
  inputs, resource provisioning). The Monosiri/spatial playback request
  remains open alongside it.
- [ ] Complete `battleGetUnitMonosiriPtr` 8012ef20 with its original
  table/source provisioning (bounds/negative kind/capture/null/returned
  record semantics, data pointers/lifetime).
- [ ] `CommandGetWeaponAddress`/`SelectWeapon` caller and menu-type writers
  that establish valid cursor modes (9/11..13/>14 leave the original stack
  cursor unwritten); reachable type/input invariants or evidence of original
  stack/cursor reuse.
- [ ] Reconcile `BATTLE_UNIT_QUERY_EVENT_LEAVES` prose with the corrected
  capture-order instruction sites (`BATTLE_UNIT_HP_WORK_CAPTURE_ORDER`).
- [ ] Remaining NormalJump entry/root registration and
  owner/priority/type/context/ID/speed writer paths needed to bind 80355cf0,
  plus the callback address/caller census for the stream's native service
  registry. *(Partial: source/root registration and the 156-site registry
  census were delivered; menu/global writer paths, dynamic data7/transitive
  sources and original runtime remain open.)*
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

The append-only delivery notes, implementation breadcrumbs and host-gate history for this request live in [EVT-01_HISTORY.md](EVT-01_HISTORY.md). Validated deliveries are also registered in the [delivery log](DELIVERY_LOG.md).
