# EVT-01 — delivery notes and implementation breadcrumbs

Append-only history for [EVT-01](EVT-01.md): documentation-agent replies, native implementation breadcrumbs, validation/host-gate notes and corrections, in the order they were recorded in the request inbox. Moved verbatim on 2026-10-12; nothing was edited or removed.

ID2 pinch status/warning port-function reply (2026-10-12):
[Threshold transitions](../behavior/battle/BATTLE_PINCH_THRESHOLD_FLAGS_AND_TRANSITIONS.md),
[all-unit scan/countdown](../behavior/battle/BATTLE_PINCH_ALL_UNIT_SCAN_AND_WARNING_CADENCE.md),
and [audience/source bindings](../behavior/battle/BATTLE_PINCH_AUDIENCE_REQUESTS_AND_SOURCE_THRESHOLDS.md)
close four complete bodies. All-64 updates, first-prequalified shadowing,
independent live threshold phases, callback suppression, positive-HP warning
filter, shared wrapping timer and exact sound/float source data supplied.
12 hashes (4 complete/8 linked), 225 anchors, 14 spans pass twice identically.
ID2 lock released; no native implementation changes. Current `make test`
gate is 86/87: newly added battle_packed_mario_jump aborts at
`tests/battle_packed_mario_jump_tests.cpp:53`, assertion
`f.run(1)==2 && f.entries[0].initialized==2 && f.playCalls==0 && f.sinCalls==0`.
Exact log /tmp/id2-pinch-test.log. Python suite separately passes 90;
`make all` passes. Native owner should resolve the packed jump fixture/behavior.
Status/audio/audience-core/cadence/runtime remain open. Next needed port
function documentation: first-act/turn/end dispatch and audience target core.

ID2 parent BattleMain/manager/hook reply (2026-10-12):
[Main service order/final state](../behavior/battle/BATTLE_MAIN_SERVICE_ORDER_AND_FINAL_STATE_CHECK.md)
and [manager banks/outer hook](../behavior/battle/BATTLE_SEQUENCE_MANAGER_BANKS_AND_OUTER_HOOK.md)
close three complete bodies.Main retained B versus manager/hook recaptures,
11 ordered services before bank4→0,raw global-word masks/fresh bank query,
end bank/flags,one captured manager bank/init→pinch despite mutation,and
outer null guard/retained completion flag supplied.20 hashes(3 complete/
17 linked),117 anchors/1 span/2 exact DOL calls/repeated reports,
86 CTest/90 Python/make all pass.ID2 lock released;no native edits.
Prior init-dispatch host gate RESOLVED:Jump fixture now passed/current86 suite;
first failures remain historical logs.Next port evidence:outer-hook scheduling,
end reimport/handler source domains and query-event reachability.

Native implementation breadcrumb (2026-10-12): MarioJumpParam80109c68 now
uses native event/unit/work fields with raw Y/output prefetch,zero-only speed
lookup,original60/0.15 correction/word-wrap/lower-bound and resulting-count
sentinel policy. 86 CTest/90 Python,Jump-flow verifier and final make all pass.
Transient unrelated host link/renderer build failures resolved on final rechecks.
See behavior/battle/BATTLE_NATIVE_MARIO_JUMP_PARAMETER_ORDER.md. Local6=42
propagates to existing native brother creation in authored fixture;airborne
MarioJumpPosition/stylish/source/numeric/audio/transitive graph/Game execution
remain pending. Existing Monosiri/spatial playback requests remain open.

ID2 complete init dispatch/registry/readiness reply (2026-10-12):
[Init dispatch/resource polls](../behavior/battle/BATTLE_INIT_SEQUENCE_DISPATCH_AND_RESOURCE_READINESS.md),
[reset/readiness scan](../behavior/battle/BATTLE_UNIT_REGISTRY_RESET_AND_INIT_EVENT_SCAN.md),
[sequence fields/source](../behavior/battle/BATTLE_SEQUENCE_FIELDS_AND_INIT_SOURCE_BINDINGS.md)
close six local bodies.State0 fall-through/carried SetSeq arguments,setup
invocation/state2 full event scan,ordered three effects/Nice readiness,state5
window/sequence writes,64 registry clears,signed setter upper-only bound,
8-field get/set table supplied.17 hashes(6 complete/11 linked),209 anchors/
5 spans/2 direct calls/repeated reports/own links pass.90 Python/make all pass.
Host retry85/86:effect_deletion now passed;new battle_mario_jump_parameter_event
still fails line46(/tmp/id2-initdispatch-test-retry.log).No native edits by ID2.
ID2 local-body lock released;parent/host gate open.Native owner notified.
Next needed port evidence:parent scheduler/end reimport/source/query domains.

ID2 host gate breadcrumb (2026-10-12):init dispatch evidence passes
(6 complete/11 linked,209 anchors/5 spans/2 DOL calls).First make test86-suite
failed effect_deletion(Not Run) and new battle_mario_jump_parameter_event:
tests/battle_mario_jump_parameter_event_tests.cpp:46 assertion registry.invoke
80109c68/result2/locals6==42/initialized0 failed.Exact log
/tmp/id2-initdispatch-test.log.No native fixes by ID2;native owner please inspect
current executable/fixture/binding.Next attempt retries current tree;Python
stage run separately because CTest stopped make.

ID2 complete Btl_UnitSetup reply (2026-10-12):
[Startup actors/repeated imports](../behavior/battle/BATTLE_UNIT_SETUP_ACTORS_AND_REPEATED_IMPORTS.md)
and [opponent/equipment tail](../behavior/battle/BATTLE_UNIT_SETUP_OPPONENT_LOOP_AND_EQUIPMENT_MASK.md)
close full800f8aac/2f8.Both previously scoped import callers now complete.
Captured D/G/P versus fresh initial-party root,partial startup writes,two fixed
setup entries,additional post-equipment imports,nonzero-only party admission,
original-140/5 coordinates,live opponent count/signed-byte override versus full
ordinal fallback,HP/FP copies,and200-slot exact mask mapping supplied.
8 hashes(1 complete/7 linked),190 anchors/5 spans/repeat reports,
85 CTest/90 Python/make all pass.ID2 lock released;no native edits.Next needed
port evidence:setup invocation/end reimport caller paths and source/query-event
reachability.Entry/services/global readiness remain separate.

Native implementation breadcrumb (2026-10-12): MovePosition8010ca74 now
operates on native event/unit/work/sound records with original horizontal helper,
first-call count/speed policy and terminal snap/yield. 85 CTest/90 Python/make all
and both Jump source verifiers pass. See
behavior/battle/BATTLE_NATIVE_MOVE_POSITION_CAPTURE_ORDER.md for operand prefetch,
live field rereads,trig/X/cos-round/alias order and actual USER_FUNC resume. Next:
native MarioJumpParam/MarioJumpPosition plus source/math/audio/transitive event/
outer Game composition. Existing Monosiri/spatial playback requests remain open;
no original trajectory/numeric/hardware or full scene claim.

ID2 snapshot caller host gate RESOLVED (2026-10-12):native fixture arrived;
make test retry passed85 CTest/90 Python(/tmp/id2-switch-test-retry.log).
Prior missing-file configuration failure is historical,not current blocking.
Local docs commit ad1a934 and make all/binary checks remain validated.
No native fixes by ID2;source/lifetime/query/full-game boundaries remain open.

ID2 party switch/field export caller reply (2026-10-12):
[Complete switch/home/export order](../behavior/battle/PARTY_SWITCH_SPAWN_HOME_AND_SNAPSHOT_EXPORT.md)
and [field wrapper/seven call domains](../behavior/battle/PARTY_FIELD_EXPORT_AND_SNAPSHOT_CALL_DOMAINS.md)
close four local bodies.Selected kind→seven48-byte setup→spawn source pinned.
OLD lookup before admission,NEW spawn before OLD export,64-ID-list exchange,
old X/Z+new Y/global-floor request,mode/acting-ID copies,post-export old flag8,
Mario unconditional versus party guarded export supplied.14 hashes(4 complete/
10 linked),127 anchors/2 spans/7 exact direct calls/repeat reports/own links pass.
90 Python/make all pass.Host make test currently stops at configuration:missing
new tests/battle_packed_move_position_tests.cpp(CMake394,/tmp/id2-switch-test.log).
No native edits by ID2;full host gate remains pending native fixture completion.
ID2 local-body lock released;parent request open.Next evidence:Btl_UnitSetup
import paths/end caller domains/source mutations/query-event reachability.

ID2 validation breadcrumb (2026-10-12):party-switch/export evidence passes
(4 complete/10 linked,127 anchors/2 spans/7 direct calls).make test configuration
currently fails because CMakeLists.txt:394 references missing
 tests/battle_packed_move_position_tests.cpp.Exact /tmp/id2-switch-test.log.
No native source/test edits by ID2.Native owner:please finish your target's
fixture;ID2 will rerun once the source is present.Documentation work continues.

ID2 seven-party snapshot initializer reply (2026-10-12):
[Complete reset/raw map](../behavior/battle/PARTY_BATTLE_SNAPSHOT_INITIALIZATION_AND_RAW_MAP.md)
and [import/export/lifetime](../behavior/battle/PARTY_BATTLE_SNAPSHOT_INIT_IMPORT_AND_LIFETIME.md)
close BattlePartyInfoWorkInit800f7c20/84.Seven44-byte records,float1.0,
word24 BEFORE fill30,half28 then kind increment then half2a,gaps22/23 preserved.
One aligned direct DOL caller at800f9018 BattleInit verified.Import/export
same-record linkage and already recovered kind.c0→unit18 initial publication
supplied;remaining runtime provisioning concerns source selection/mutations/
readiness,not a wholly unknown initial table copy.3 hashes(1 complete/2 linked),
34 anchors/2 spans/1 direct call/repeated reports,84 CTest/90 Python/make all
pass.ID2 lock released;no native edits.Next port evidence:snapshot export
caller domains and source/table mutations,or query-event closure.Save/Game/
platform/original execution remain open.

ID2 remaining party weapon callback closure (2026-10-12):
[Complete shared protocol/Koops exception](../behavior/battle/PARTY_WEAPON_CALLBACKS_COMPLETE_SHARED_PROTOCOL.md)
and [exact28-weapon source map](../behavior/battle/PARTY_WEAPON_CALLBACKS_WEAPON_SOURCE_MAP.md)
close all six formerly open local key1 bodies.Five full183-instruction programs
match Christine after source/call normalization;Koops181 uses literal344/345
for first two icons,then current weapon icons.Original order/gaps/Ms.Mowz
nonmonotonic pointers pinned.All seven callbacks plus rank helper now complete:
12 hashes(8 complete/4 linked),1296 anchors/71 spans,5 full normalized
comparisons/repeat reports,83 CTest/90 Python/make all pass.ID2 lock released;
no native edits.Next needed port evidence:runtime unit table provisioning and
returned-event/query callback reachability,not more unsupported cursor defaults.

Native implementation breadcrumb (2026-10-12): GetMoveFrame8010c8c0 now
operates on native event/unit/work bytes with required numeric/distance providers.
83 CTest/90 Python and make all pass;13 bodies/68 anchors/5 spans/6 Jump callers
match ISO preparation verifier. See
behavior/battle/BATTLE_NATIVE_MOVE_FRAME_CAPTURE_ORDER.md for raw Y/speed
prefetches,zero capture/local fallback and query versus motion count distinction.
Next:live native MovePosition/sound/Mario jump motion plus original source/math/
execution and outer Game binding. Existing Monosiri source request remains open.

ID2 Goombella weapon callback/source reply (2026-10-12):
[Complete callback/tech query](../behavior/battle/GOOMBELLA_WEAPON_TABLE_CALLBACK_AND_TECH_LEVEL.md)
and [seven key1 chains](../behavior/battle/PARTY_WEAPON_CALLBACK_KEY1_SOURCE_BINDINGS.md)
recover Christine __makeTechMenuFunc80189634/2dc and partyGetTechLv80140e60/44.
Current-global Party→kind translation→fresh pouch level,captured signed rank,
four fixed source rows,repeated mutable-count reads,reason preservation,direct
weapon-key lookup,append/raw return distinctions supplied.Seven original
unit+0xc0/table/key1 chains pinned;other six bodies identities only,NOT complete
semantics.12 hashes(2 complete/10 linked),200 anchors/23 spans/repeat reports,
82 CTest/90 Python/make all retry pass.ID2 lock released;no native edits.Next priority:
remaining six party callbacks and explicit source/order differences,then query
callback/event reachability.Unit provisioning/Game/runtime remain open.

ID2 build breadcrumb (2026-10-12):Goombella callback/source verifier and
82 CTest/90 Python pass.First make all failed in source/display/render.cpp:343,
C3D_TexEnvSrc argument5 int0→GPU_TEVSRC invalid conversion;log
/tmp/id2-partytech-all.log.No native edits by ID2;renderer owner please
check current Citro3D enum argument binding.Retrying current build state.

ID2 complete multi-item menu/cancellation reply (2026-10-12):
[Mode producer/sources](../behavior/battle/MULTI_ITEM_TABLE_MODE_FIELDS_AND_SOURCE_BINDINGS.md),
[Main selection/navigation](../behavior/battle/MULTI_ITEM_MENU_MAIN_SELECTION_AND_NAVIGATION.md),
[setup/end/Input](../behavior/battle/MULTI_ITEM_MENU_SETUP_END_AND_INPUT_BINDINGS.md),
[cancel-check controls/display](../behavior/battle/MULTI_ITEM_CANCEL_CHECK_CONTROL_AND_DISPLAY_REQUEST.md)
close seven local bodies.Mode1 preserves old reason;independent2f3/2f4,
raw enable/low8 WORD cost,actor-specific ItemData,post-sound M550/state5,
reason17,cancel1/audience order,and separate cancel cursor/setup2/affirmative
M551+state29/negative6/display-after-decision supplied.21 hashes(7 complete/
14 linked),477 anchors/18 spans/repeat reports,82 CTest/90 Python/make all pass.
First packed-sampling address assertion(/tmp/id2-multi-test.log) RESOLVED on
retry(/tmp/id2-multi-test-retry.log);no native edits by ID2.ID2 lock released.
Next port-function priority:party key1 weapon callback/source bindings and
query-specific event reachability;UI/cancel display internals remain separate.

ID2 host-validation breadcrumb (2026-10-12):multi-item evidence passes
(7 complete/14 linked,477 anchors/18 spans).make test currently81/82:
new battle_packed_sampling aborts at source/event/event_memory_binding.cpp:7,
EventMemoryBinding::services read_word lambda assertion ok failed.Log
/tmp/id2-multi-test.log.Python stage skipped by make's CTest failure;ID2
will run it separately.No native source/tests modified by ID2.Native owner:
please inspect packed sampling source-address fixture/bindings;not repaired
or inferred as original behavior here.

ID2 party/protect menu lifecycle reply (2026-10-12):
[Party selection/pending](../behavior/battle/PARTY_MENU_MAIN_SELECTION_AND_PENDING_BEHAVIOR.md),
[navigation/cancel/audience](../behavior/battle/PARTY_MENU_NAVIGATION_CANCEL_AND_AUDIENCE.md),
[party/protect setup/end/Input](../behavior/battle/PARTY_PROTECT_MENU_SETUP_END_AND_INPUT_BINDINGS.md)
close six complete local bodies.Party confirmation rereads selected kind after
sound/light before M54c/state29;disabled confirm clears pending then continues
navigation.Cancellation goes20;audience pending BEFORE End/result0.Setup
forwards F to UI without direct producer call.Protect queries other unit but
publishes no party ID;two flag stores and Input-owned controls supplied.
17 hashes(6 complete/11 linked),193 anchors/2 keys/repeat reports,
81 CTest/90 Python/make all pass.ID2 lock released;no native edits.
Next port-function priority:remaining multi-item menu writers and party key1
weapon callback/source bindings.Query-event/Game/runtime remain open.

Native implementation breadcrumb (2026-10-12): complete chooser now operates
on live native unit/kind/status/weapon/item/FP and raw/private candidate bytes;
Takeoff backend defaults to this chooser with explicit sampling still required.
81 CTest/90 Python and make all pass. See
behavior/battle/BATTLE_PACKED_CHOICE_SOURCE_ORDER.md. Next:full native sampling,
original source geometry/monosiri ownership and numeric/RNG/executed-position
coverage under EVT-01/JUMP-01. No full Jump/Game or invalid-domain fidelity claim.

Documentation-agent correction breadcrumb (2026-10-12): original chooser
800febc8 complete disassembly and verifier constants show mode4 uses0.1+0.4*
(N-rank),mode8 uses0.5+0.4*rank,and item142 calls CanActStatus8012724c.
Corrected behavior/battle/BATTLE_ENEMY_CANDIDATE_CHOICE.md and semantic chooser;
FP deficit independently retains0.1. Please follow these corrected source roles
in future sampling/menu binding documents. Original inspection /tmp/pm-native-choice.*;
existing verifier pins7 full hashes/48 anchors/14 spans. Bonus permutation is
also reread after initial output stores (800fec58),now retained in both choosers.

ID2 complete OperationMenuMain/setup/end reply (2026-10-12):
[Opcode/state publication](../behavior/battle/OPERATION_MENU_MAIN_OPCODE_AND_STATE_PUBLICATION.md),
[navigation/cancel/audience](../behavior/battle/OPERATION_MENU_NAVIGATION_CANCEL_AND_AUDIENCE.md),
and [setup/end/live flags](../behavior/battle/OPERATION_MENU_SETUP_END_AND_FLAG_OWNERSHIP.md)
close four complete local bodies.Main clears pending before key gate,fresh
opcode after sound,operation0→state24/4→27/1..3,5..8→29,unknown enabled
opcode still End/result1,disabled fresh reason/state22,edge Trigger versus
interior Repeat,cancel state after End,audience pending before End/result0.
Setup producer→UI→OR01000100;End UI→clear100;UI Main no-op distinct Init/End.
14 hashes(4 complete/10 linked),213 anchors/2 keys/repeat reports,
81 CTest/90 Python/make all pass.ID2 lock released;no native code/tests changed.
Next port-function priority:party selection/protect-partner menu writers,then
party key1 weapon callback/source binding.Query-event/Game/runtime remain open.

ID2 operation/party producer reply (2026-10-12):
[Entry/optional/publication](../behavior/battle/OPERATION_TABLE_ENTRY_PARTIAL_ROWS_AND_PUBLICATION.md),
[charge/Defend/Escape/keys](../behavior/battle/OPERATION_TABLE_CHARGE_DEFEND_ESCAPE_AND_KEYS.md),
[party producer](../behavior/battle/PARTY_SELECTION_TABLE_PRODUCTION_AND_CAPTURE.md),
[icons/CanActStatus](../behavior/battle/PARTY_ICON_SOURCE_AND_CAN_ACT_STATUS.md)
close four complete bodies:operation table,party table,party icon,action-status
predicate.Party admitted count versus disabled rows,optional/mandatory operation
order,retained/fresh setup,cost/enable rereads,ignored versus consumed CanAct,
current/retained other-unit HP,signed cursor count/selection and distinct source
scan bounds supplied.16 hashes(4 complete/12 linked),472 anchors/32 spans,
repeat reports,80 CTest/90 Python/make all pass.ID2 lock released;no native edits.
Next port-function priority:OperationMenuMain and party-selection menu writers,
then party key1 weapon callbacks/source.Query-event/Game/runtime remain open.

ID2 complete confusion selector reply (2026-10-12):
[Class/type publication](../behavior/battle/CONFUSION_CLASS_SELECTION_AND_MENU_TYPE_WRITE.md),
[weapon/target policy](../behavior/battle/CONFUSION_WEAPON_SELECTION_AND_TARGET_POLICY.md),
[operations/party/event outputs](../behavior/battle/CONFUSION_OPERATIONS_PARTY_AND_EVENT_OUTPUTS.md)
close full BattleSetConfuseAct8011f62c/610:retained actor/entry mutations,four
captured-count/low-byte halfword lists,original irand calls,fresh type/row reads,
weapon rebuild/rechecks,target byte draw,nine operation cases,existing party
rows,and script-address/null results.Fixed fallback is803532b0 flustered,
NOT Wait;empty weapon/party candidate lists return0.Type13 can be published
then returns flustered locally.9 hashes(1 complete/8 linked),388 anchors/5
spans/repeat reports,80 CTest/90 Python/make all pass.ID2 lock released.
No native code/tests changed.Operation producer/party key1 source and callback,
returned-event/query reachability remain open.Next priority:those producer and
source bindings,plus remaining menu writers;do not infer global query safety.

Native implementation breadcrumb (2026-10-12): GetTakeoffPosition80109054
now operates on native records with private hit-output aliases and required
sampling/choice providers. 80 CTest/90 Python and make all pass. See
behavior/battle/BATTLE_PACKED_TAKEOFF_CAPTURE_ORDER.md. Next implementation
priority is live native sampling/choice unit/part/status/source adapters;local
core behavior contracts already exist. Documentation follow-up:leave original
kind/part/monosiri source provisioning and numeric/FPSCR/executed-position
coverage here or under JUMP-01 as recovered. No fixed candidate fallback or
full motion/Jump/Game claim is introduced.

ID2 complete audience/gift interruption reply (2026-10-12):
[Audience type11](../behavior/battle/AUDIENCE_INTERRUPTION_MENU_TYPE_WRITER.md),
[gift recovery/type12](../behavior/battle/GIFT_ITEM_RECOVERY_AND_MENU_TYPE_WRITER.md),
and [restore/present fields](../behavior/battle/MENU_INTERRUPTION_RESTORE_AND_AUDIENCE_FIELDS.md)
close ten local bodies:AudienceCheck,present item,restore,AllEnd,three present
get/set wrappers,audience base,height/position leaves.Captured B before services,
previous type after shutdown,typed asymmetric bounds/unchecked-1,gift recovery
versus generic completion,FMA request order,pending1/2,equality restore,and
separate saved Input/previous type fields supplied.20 hashes(10 complete/10
linked),243 anchors/3 spans/repeat reports,79 CTest/90 Python/make all pass.
ID2 lock released;no native edits.Local11/12 writer question answered;audience
core/present producer/event/query reachability remains open.Next port-function
priority:complete confusion writer and party key1 table callback/source binding.

Native implementation breadcrumb (2026-10-12): hit-position layering and
GetHitPos801120a8 now bind to native unit/part/work records with explicit
multiply/FMA providers. See behavior/battle/BATTLE_PACKED_HIT_POSITION_CAPTURE_ORDER.md
for six live scale reads,independent facing/side signs,once-loaded Y coefficient,
ordered aliases and first-unit retention versus fresh part lookup. 79 CTest/90
Python and make all pass. Leave original field114/part-offset writer,PowerPC
numeric/exception,backend and executed-trajectory evidence under EVT-01/JUMP-01;
full takeoff/motion/Jump/Game composition remains open.

ID2 shared-link breadcrumb (2026-10-12):own four weapon-table documents'
file links pass.Broader docs scan currently finds one missing display link:
behavior/display/MAP_MATERIAL_LAST_STAGE_COLOR_AND_FOG.md links
ANIMATION_INNER_POSE_STATE_AND_PASSES.md,which does not exist in that folder.
ID1 owns that current material slice;left its file unchanged.Please correct
against actual inner-draw contract before reporting whole-doc link validation.

ID2 complete weapon-table producer reply (2026-10-12):
[Entry/layout/publication](../behavior/battle/WEAPON_TABLE_BUILDER_ENTRY_LAYOUT_AND_PUBLICATION.md),
[Jump/Hammer/badges](../behavior/battle/WEAPON_TABLE_JUMP_HAMMER_AND_BADGE_ROWS.md),
[inventory/Special/party](../behavior/battle/WEAPON_TABLE_ITEM_SPECIAL_AND_PARTY_ROUTES.md),
and [sources/icons](../behavior/battle/WEAPON_TABLE_SOURCE_BINDINGS_AND_ICON_LEAVES.md)
close complete MakeSelectWeaponTable80123ec0/d50 and two icon leaves.
All branches,partial28-byte descriptors,independent level admissions,late
first-row weapon override,badge arrays,20 inventory slots/explicit substitute/
dummy row,captured Special mask/live pointers,unchecked party delegate,count
publication/empty preservation/signed scroll and late eligibility supplied.
10 hashes(3 complete/7 linked),894 anchors/52 spans,repeated reports,
78 CTest/90 Python/make all pass.No native code/tests changed.ID2 lock released.
Party key1 callback/source binding remains open;other source provisioning/menu
writers/query-event reachability/Game/runtime remain open.Next port-function
priority:party table callback and remaining audience/gift/confusion type writers.

Native implementation breadcrumb (2026-10-12): part getter/registry/world
position and GetPartsPos80112d98 now compose on actual native byte storage.
78 CTest/90 Python and make all pass. See
behavior/battle/BATTLE_PACKED_PART_POSITION_CAPTURE_ORDER.md for single-part/
negative-ID asymmetry,unused initial lookup/fresh second lookup,absolute aliases,
relative preloads/signed owner189 and explicit numeric providers. Hit scale/
offset layer,source unit/part writers,full motion/Jump/Game and original runtime
remain open;leave further source/numeric/backend evidence under EVT-01/JUMP-01.

ID2 weapon-menu Main/target eligibility reply (2026-10-12):
[Confirmation/light requests](../behavior/battle/WEAPON_MENU_MAIN_CONFIRMATION_AND_LIGHT_REQUESTS.md),
[navigation/cancel/End](../behavior/battle/WEAPON_MENU_MAIN_NAVIGATION_CANCELLATION_AND_END.md),
and [count/update](../behavior/battle/WEAPON_MENU_TARGET_COUNT_AND_ELIGIBILITY_UPDATE.md)
close seven complete local bodies:Main/End/target count/update/UI Main/two
light stubs.Confirmation priority,interior Repeat/boundary Trigger,post-service
cursor rereads,retainedT,disabled reason/result,typed negated sampling direction,
captured updater count,and reason overwrite supplied.20 hashes(7 complete/
13 linked),344 anchors/2 keys/repeat reports,78 CTest/90 Python and make all
pass.First shared-host link failure(/tmp/id2-wmain-test.log) RESOLVED on retry
(/tmp/id2-wmain-test-retry.log);root cause unproven,no native edits by ID2.
Next native documentation priority:full MakeSelectWeaponTable row/source
production,remaining menu writers,and query-specific event domains.No safe
unsupported-type fallback is established by these separate private stack slots.

ID2 validation breadcrumb (2026-10-12):weapon-menu Main/target evidence
verifier passes(7 complete/13 linked,344 anchors/2 keys);make all passes.
First make test attempt failed before CTest at battle_packed_position_tests:
/usr/bin/ld reports Relocations in generic ELF(EM:62),file in wrong format
for tests/battle_packed_position_tests.cpp.o.Host cache/link command both name
/usr/bin/g++;root cause unproven.No native source/test changes by ID2.
Exact log /tmp/id2-wmain-test.log;retrying current host gate.

ID2 complete BattleCommandInput reply (2026-10-12):
[Entry/re-entry/class states](../behavior/battle/COMMAND_INPUT_ENTRY_REENTRY_AND_CLASS_STATES.md),
[target remap/confirmation](../behavior/battle/COMMAND_INPUT_TARGET_REMAP_AND_CONFIRMATION.md),
and [all other states/attack dispatch](../behavior/battle/COMMAND_INPUT_AUXILIARY_STATES_AND_ATTACK_DISPATCH.md)
cover COMPLETE801229d8/838,all30 states/default,sharedSP08,captured actor/menu,
pending re-entry without progress reset,fall-through versus next-call waits,
first-pair target remap,both repeat gates,cancel/confirm/audience order,and
state29 live flag clears/direct1.39 hashes(1 complete/38 linked),526 anchors/
3 spans,repeated reports,77 CTest/90 Python and make all pass.Local Input
request answered;type9/13→state29 dispatch is now linked,but downstream
query-event safety remains unproven.Next port-function priority:complete
WeaponSelectMenuMain and MakeSelectWeaponTable/target filtering,then remaining
menu writers and query event/callback source domains.No default weapon added.

Native implementation acknowledgement (2026-10-12): consumed ID2's complete
first-attack pair/getter/event consolidation. GetFirstAttackTarget8010b418 now
uses actual captured-battle scan/Mario/Party/kind predicate/body/output composition;
77 CTest/86 Python and make all pass. Paired roles/raw return,last-slot miss,
ignored attacker,body-before-target-id and retained tokens/rereads are preserved.
Native source registry/setup/part/input validity and full first-strike/Game/runtime
remain open;local body request is answered. Continue existing EVT-01/JUMP-01
source/menu/ownership closure and leave further backend evidence here.

ID2 ActMenuMain writer reply (2026-10-12),partial source-domain answer:
[Selection/type publication](../behavior/battle/ACT_MENU_MAIN_SELECTION_AND_TYPE_PUBLICATION.md),
[navigation/service order](../behavior/battle/ACT_MENU_MAIN_NAVIGATION_AND_SERVICE_ORDER.md),
and [setup/end/position gate](../behavior/battle/ACT_MENU_SETUP_END_AND_POSITION_GATE.md)
close six complete local bodies for porting.Three type stores:two explicit9/
state29 routes;enabled current row copied at8012254c then14-way switch(type13
state29).Fresh cursor reads after sound/End,disabled reasons,live class-count
scan,independent result0/1,typed position admission,and nine partial cursor
resets supplied.24 hashes(6 complete/18 linked),472 anchors/3 spans,repeated
reports,77 CTest/86 Python/make all pass.Class/ActMenuMain writer request is
answered locally;complete Input/confusion/query-event reachability remains open.
Next native source priority:Input states5/14/19/29 and selected-weapon query
callback/event bindings.Do not promote known type9/13 writers into proof that
weapon queries execute with those types,or reuse another frame's cursor slot.

ID2 shared-index breadcrumb (2026-10-12):commit5709c2d contains the validated
action-class producer delivery plus concurrently staged reference census,
inspect_dol_function updates,and three overworld contracts.Another writer
staged those seven additional paths/PROGRESS section between ID2's exact
seven-path index check and commit.No history rewrite or other-owner revert
performed.Reference-census owner:your staged work is already in5709c2d.

ID2 action-class producer reply (2026-10-12),partial menu-source answer:
[Admission/publication](../behavior/battle/ACTION_CLASS_TABLE_ADMISSION_AND_PUBLICATION.md)
and [eligibility/keys](../behavior/battle/ACTION_CLASS_TABLE_ELIGIBILITY_AND_MENU_KEYS.md)
close complete _btlcmd_MakeActClassTable80124dc8/5b8 for native porting.
Types0/6/1/2/4/5/13,partial20-byte descriptors,disabled counted rows,retained
class cursor and signed index correction supplied. Operations and Lecture use
independent fresh flag reads;message-visible changes can emit both/neither.
Seven original keys/icons pinned;no invented text.4 hashes(1 complete/3 linked),
366 anchors/7 spans,repeat reports,76 CTest/86 Python and make all pass.
This is a producer contract,not proof of safe weapon-query reachability.
Next native priority:complete ActMenuMain type publication and query domains.

Native implementation / capture refinement (2026-10-12): SetPos80112fcc
and GetBodyId8010ead8 now bind native original-address memory.76 CTest/86 Python
and make all pass. See behavior/battle/BATTLE_POSITION_WRITE_AND_BODY_ID_ORDER.md:
SetPos uses GetValue→integer→float and RAW sentinel preservation after old-position
read;body query retains earlier battle/output and returns first flagged part's
DEFINITION word0. No hardcoded body1 or global pipeline snapshot. Source part/
unit writers,numeric/full motion/render/Game and original traces remain open;
first-attack/menu-domain requests still pending. Leave further evidence here.

ID2 menu-domain documentation reply (2026-10-12),partial source answer:
[Complete setup/decision](../behavior/battle/WEAPON_MENU_SETUP_AND_DECISION.md) and
[type/cursor source evidence](../behavior/battle/WEAPON_MENU_TYPE_DOMAINS_AND_CURSOR_REUSE.md)
supply complete WeaponSelectMenuSetup/SelectWeaponDecide for porting. Source
contains explicit type12 write/saved previous type;Input state0 conditionally
restores11/12. Same Input invocation initializes SP08 with type14 cursor before
later state11 query/read of that SAME word. This is concrete unchanged-output
reuse,not a fallback for GetSelectWeapon's DIFFERENT uninitialized stack slot.
Six direct selected-weapon query callbacks/16 total DOL sites pinned. Full class/
input/gift/confusion/query-event reachability is still REQUIRED before Game
binding;global supported-type and safe query-specific invariants are unproven.
Next source priority:ActMenuMain/MakeActClassTable writers and query call domains.

Native implementation acknowledgement (2026-10-12): consumed ID2's complete
movement setter/source-token delivery. SetMoveSpeed8010d860/SetFallAccel8010d770
now bind actual native GetValue/GetFloat/translation/late B/registry and field-only
writes.75 CTest/78 Python and make all pass. Native pack test invokes seven real
ordinary Jump requests with exact header/function/actor/token/bits;Majinai+0.5
is a delivered-token fixture while nested stream remains unpacked. Retained A/ID/
float and unchecked-1 behavior are preserved. Local wrapper request is answered;
source/unit/numeric/platform/full-motion/Game remain open. First-attack-target/
menu-domain requests remain pending;leave further source/backend evidence here.

Native implementation breadcrumb (2026-10-12): GetHomePos801124f8 and
GetPos80113620 now use actual packed coordinates/captured event outputs and
platform conversion.74 CTest/78 Python and make all pass;earlier concurrent
field failures resolved. See behavior/battle/BATTLE_POSITION_EVENT_CAPTURE_ORDER.md:
visible raw-sentinel skip differs from home's unconditional conversion/output;
leaf alias copies differ from event's private preloaded vector. Numeric/FPSCR,
unit writers/motion/full Jump/Game and original trajectories remain open.
Movement-wrapper lock is claimed by ID2;first-attack-target/menu-domain requests
remain pending. Leave delivered source/backend evidence here.

ID2 native movement-wrapper documentation reply (2026-10-12):
[Complete event/leaf order](../behavior/battle/BATTLE_MOVEMENT_SETTER_EVENT_ORDER.md)
and [source tokens](../behavior/battle/BATTLE_MOVEMENT_WRAPPER_SOURCE_TOKENS.md)
answer the requested COMPLETE SetMoveSpeed8010d860/SetFallAccel8010d770 bodies.
Retained E/A,raw actor load→GetValue→BattleTransID→raw A04→GetFloat→fresh global
B→GetUnitPtr→retained float leaf store→2. No firstCall,-1/null shortcut or scalar
clamp. Leaves stfs only unit170/174. Eight original requests and exact binary32
outputs pinned;Majinai f24a7c80 is+0.5,not the prior prose's-0.5. Private commit
ef07139. Native registration/storage/Game/source-domain/execution remain open;
this closes the requested local wrapper protocol for implementation.

Native implementation / source-domain request (2026-10-12): native target
and command-weapon callbacks now read original selection/menu/unit bytes with
ordered outputs and captured/current owners. See
behavior/battle/BATTLE_PACKED_SELECTION_CAPTURE_ORDER.md for instruction-order
refinements. Focused selection/registry tests and78 Python pass;make all passes.
Full suite currently72/73 with concurrent field_movement fixture failure;own
selection cases pass. Source menu/ItemData/unit provisioning is not claimed.

ID2 native first-attack documentation reply (2026-10-12):
[Pair/getters](../behavior/battle/FIRST_ATTACK_PAIR_SELECTION_AND_GETTERS.md) and
[event/body output](../behavior/battle/FIRST_ATTACK_TARGET_EVENT_AND_BODY_PART.md)
answer the requested COMPLETE local GetFirstAttackTarget/BattleGetFirstAttackUnit/
800f86f4/GetBodyPartsId consolidation. All seven called query bodies,captured
battle/raw output order,setup mode ranges/paired stores/last-slot miss,helper raw
return,Mario/Party null/kind policies and body-before-id/two-setter order supplied.
Crump/Mario initial part1 source witness is reused;setup/registry/current source
invariants are explicitly unproven,not invented safe fallback. Short overview
marked superseded. Movement-wrapper request remains the next priority per user.

Native implementation breadcrumb (2026-10-12): Start/StopWaitEvent and
WaitGuardMove now bind actual root/DeleteID and global bit01 polling.72 CTest/78
Python and make all pass. See behavior/battle/BATTLE_WAIT_EVENT_NATIVE_ORDER.md
for retained unit,outer script gate,script reload,actor-before-ID stores,flags0
and seven-instruction guard contract. Actual dispatcher polling/resumption passes;
unit wait-script/flag writers,animation/outer timing/full Jump/Game remain open.
First-attack-target and movement-wrapper requests remain pending;leave further
source/backend evidence under existing EVT-01/JUMP-01 closure here.

Native implementation / refinement breadcrumb (2026-10-12): packed status
getter/presence and registered CheckStatus80105f0c/CheckData80109f88 now share
native event/unit RAM.71 CTest/78 Python and make all pass. Evidence in
behavior/battle/BATTLE_UNIT_PRESENCE_CAPTURE_ORDER.md refines wrapper output/
lookup/key order and strength-first gated status reads. Membership accepts keys
with zero values and treats key0 as terminator;it is not GetData!=0. Complete
BATTLE_STATUS_STORAGE closes used GetStatus/CheckStatus locals despite the newer
unit-query summary calling that dependency open. Source writers/lifetimes/full
Game and original traces remain open;first-attack-target and movement-wrapper
requests remain pending. Leave further source/callback evidence here.

Native implementation breadcrumb (2026-10-12): local turn-rule selector
and two-phase helper are implemented with exact counter load orders and separate
global recapture. Native turn-start defaults to actual rule judgment;69 CTest/78
Python and make all pass. Integration proves prior record count is judged before
increment. Ignored rules,threshold equality/255,raw status/mask02,phase rereads
and owner changes are covered. Outer controller/counter/descriptor source/Game
ownership and original runtime remain open;movement-wrapper request is pending.
Continue STATUS-01/DMG-02/EVT-01 closure and leave documentation replies here.

Native implementation breadcrumb (2026-10-12): complete hard/soft/raw Maya
effect-release leaves now preserve record flags/payload ordering and heap3 family.
Native fog adapter defaults to actual soft deletion and SDK-backed free;
68 CTest/78 Python and make all pass. Deferred02→OR04 records retain allocation;
hard/immediate paths release payload then clear only flags. Native fog/deferred/
hard free/reuse path is integrated. Effect update/resource/render/record-payload
source ownership,rule/outer turn/Game and original traces remain open;movement-
wrapper request remains pending. Continue EVT-01/DMG-02 closure via docs.

Native implementation breadcrumb (2026-10-12): native turn-first-process
now defaults to recovered original-byte ResetMoveStatus rather than requiring
an authored reset callback. Fast/Slow duration storage,byte narrowing,signed Slow
rules,preserved21/24 and23 reread follow documented/inspected bodies.67 CTest/78
Python and make all pass;native registry unit/Slow counter increment/reset/
priority/status-clear path is integrated. Other status helper cases,rule/effect
providers,outer turn/Game storage/source cadence remain separate. Movement-wrapper
request is still pending;continue existing STATUS-01/DMG-02/EVT-01 closure.

Native implementation breadcrumb (2026-10-12): complete local turn-first-
process and fog-expiry composition now execute on shared original-address memory.
Native adapter uses actual fortune/registry/RNG services;judgment,move-reset and
soft-effect deletion retain explicit required backend providers.66 CTest/78 Python
and make all pass. Halfword gate,record clears/cap,64-slot unit loop,retained kind/
post-RNG priority reads,status mask and global fog expiry semantics are covered.
Outer turn cadence/controller and rule/move/effect source ownership are not
claimed;no render-loop call was introduced. Movement-wrapper request remains
pending;existing STATUS-01/DMG-02/EVT-01 source/runtime requirements stay open.

Native implementation breadcrumb (2026-10-12): complete fortune producer
and count initializer now use shared byte storage with explicit irand provider;
initializer event800f08ac is registered. Native initializer→producer→power-query
consumption passes;65 CTest/78 Python and make all pass. Tests preserve negative
countdown precedence,sticky raw mode,intermediate/final rereads,captured setup
mask20 thresholds,draw budgets and independent initializer pouch reloads.
Actual turn caller/seed/interleaving/field assignment/save ownership and active
presentation remain open;no render-based fortune cadence was introduced.
Movement-wrapper documentation request remains pending;continue existing
EVT-01/JUMP-01/STATUS-01 source/runtime closure and leave replies here.

Native implementation breadcrumb / read-order refinement (2026-10-12):
flag query8011384c and frame conversion80103dc0 are now registered. Static
read-only-ISO inspection shows flag mask/flag reads BEFORE raw output read;
see behavior/battle/BATTLE_FLAG_AND_FRAME_UTILITY_READ_ORDER.md for hashes/addresses.
Actual packed Jump mask2 request passes. ftomsec uses wrapped signed product
and fixed60 with late output token.64 CTest/78 Python and make all pass after
concurrent camera updates settled. Nested sound stream/playback and full graph
remain unbound. SetMoveSpeed/SetFallAccel request remains pending;continue
existing EVT-01/JUMP-01 source/domain/platform/runtime closure independently.

Native implementation breadcrumb (2026-10-12): registered Majinai query
801146c4 now executes recovered output/flag/Done/EndCheck order with separate
battle/pouch reads and byte wrap/reread/reset behavior. Original packed power-
check80363388 inactive branch runs through the actual native VM/registry and
deletes.63 CTest/78 Python and make all pass. Active query storage is tested;
active sound/effect/jump/announcement graph remains unbound. Provider records
are authored inputs;source/runtime/full activation claims remain open.
SetMoveSpeed/SetFallAccel complete-wrapper request above remains pending;other
closed callback work can continue independently. Leave evidence/replies here.

Native implementation breadcrumb / documentation request (2026-10-12):
JumpSetting8010d6cc is now registered and writes actual packed unit16c/170/174
through native operands/actor/registry services. Original first Jump request
verifies20/0/.69921875;62 CTest/78 Python and make all pass. Full motion/Game/
original traces remain open.

Native implementation breadcrumb (2026-10-12): BattleTransID,registry get/
set and Mario/party/system queries are now implemented with original selectors,
capture/read order and distinct missing-query/setter policies. Native data-child
services can use these recovered helpers;the real packed key7 test now does so
instead of an authored selector/fixed-unit backend.62 CTest/78 Python and make
all pass. Registry/unit/table data remain authored provider bytes;original unit
entry/writer/lifetime and full Jump execution are not claimed. Next:additional
callback families plus packed unit/state ownership. Existing EVT-01/JUMP-01
source/domain/concurrency/original traces remain open.

Native implementation breadcrumb (2026-10-12): original-address callback
registry now binds the recovered RunDataEventChild8010a010 to native runtime
operands/memory/creation/schedule. Real packed key7 child and parent continuation
pass through registry;zero data continues without child.61 CTest/78 Python and
make all pass. Actor/current-battle/unit/table/cache fixtures remain authored
backend inputs;this is not a full72-callback binding or original Jump execution.
Next:complete actor/registry-query providers and additional callback families.
Please leave missing callback/backend source evidence under the existing EVT-01/
JUMP-01 requests;source/domain/concurrency/original traces remain open.

Native implementation breadcrumb (2026-10-12): `EventNativeRuntime` now
owns both contexts and all shared event memory/services. Init/ReInit,root creation
and original key7 child execution pass through actual SDK-backed arrays. Queries
reload GP/context and explicit platform providers own clock,numeric,USER_FUNC,
diagnostics and cache boundaries.60 CTest/78 Python and make all pass. Native
address/arena/provider policies remain explicit;Game/full Jump unbound. Next:
original-address native callback registry plus battle/unit/motion/action/effect/
presentation services. Existing source/domain/FPSCR/concurrency/original execution
requests remain open;please leave additional callback evidence here.

Native implementation breadcrumb (2026-10-12): `EventEntryStorage` now
promotes SDK-allocated entry payloads into live mappings with adjacent headers/
RAM preserved. Actual context Init/ReInit and interpreter/root deletion run on
that storage. Promotion preserves post-cache bytes;retirement restores raw bytes
and retains original pointer tokens without SDK free/event deletion. Repeat Init
retains old arrays,as recovered.59 CTest/78 Python and make all pass. Native span
splitting/owner lifetime/assertions remain provider policy. Next:complete service
owner,platform cache and actual native callback bindings. Existing source/domain/
concurrency/original-runtime requests stay open.

Native implementation breadcrumb (2026-10-12): `EventMemoryBinding` routes
all allocator memory services to shared original-address RAM and forwards cache/
system-call boundaries to explicit platform callbacks. Integration now executes
original five-pool configuration,SDK context-sized allocation/reuse,map/battle
subpool allocation/free and reset together on actual owned byte storage.
58 CTest/78 Python checks and make all pass. Test arena/address/cache callbacks
are authored native-provider inputs;source runtime/arena/platform evidence is
not claimed. Next:entry-span promotion with SDK headers preserved and actual
context-init/native callback ownership. Existing EVT-01 requests remain open.

Native implementation breadcrumb (2026-10-12): `EventAddressSpace` combines
entry/context/runtime/GP bytes with owned RAM. Imported original script segments
are mutable runtime copies;extracted packs remain unchanged. Halfword adapters
now share live storage. Real pack byte comparison and actual child execution
pass,alongside57 CTest/78 Python and make all. Registration order,unsupported
span rejection and RAM lifetime are explicit native policies. Next:native arena/
heap ownership and callback service bindings. Existing source/domain/concurrency/
original-execution requests remain open;no additional local-body closure claimed.

Native implementation breadcrumb (2026-10-12): `EventGpAddresses` now maps
80415fd0 and registered0x13d8-byte GP backing with live04/14/38/3c aliases;
original saved-switch helpers operate on that backing. Other offsets remain raw
bytes. Exact registered-base native resolution, fresh zero backing and explicit
retirement snapshots are port storage policies, not original allocator claims.
All56 CTest/78 Python checks and make all pass. Next implementation dependency:
address-space/arena ownership and callback bindings. Please leave additional
original pointer/allocation/initialization evidence here; existing EVT-01 source,
concurrency and original-runtime requests remain open.

Shared-runtime breadcrumb (2026-10-12): runtime mapper now shares the actual
ID/slot banks,counters,speed,requested-level and active WORD with interpreter/
constructor/manager/lifecycle. Active is uint32,retaining raw nonzero and byte
aliases. Full validation before GP step:55 suites/78 Python tests;make all passes.
Next:GP/backing arena/initialization/native ownership and actual callback registry.
No source/execution closure was inferred from storage coherence.

Data-child breadcrumb (2026-10-12): `battle_data_child.cpp` ports complete
RunDataEventChild/BtlUnit_GetData with delayed key/table lookup and actual child
255 continuation. Default pack now includes80416bd0+four ORIGINAL trailing
bytes;four streams/4564 bytes. Real extracted tiny child executes through manager,
interpreter,lifecycle and copy-back in host integration. Final make test passes54
suites/78 Python tests;make all passes;ISO verifier passes10 bodies/109 anchors.
Kind/table/unit ownership fixture is authored,not source import or complete
Jump execution.Next:native ownership/registry and remaining original callback
services,with no immediate-completion substitute for queued children.

Action-registration breadcrumb (2026-10-12): `battle_attack_registration.cpp`
ports FULL state6 local gates/root10/flags0/post-ID→actor→wait-delete/channel6
ordering. Real constructor/lifecycle fixtures pin global context/speed/no live
append and actor recapture;status/RNG routes remain original. Final make test
passes53 suites/78 Python tests;make all passes;source verifier passes72 callback
identities/156 sites/127 anchors. Other action states/native unit/menu/GP/arena/
registry and original execution remain open.Next:delivered RunDataEventChild and
Mario key7 binding,missing80416bd0+original trailing word in pack,then actual
native owners/callback implementations. Earlier constructor-level binding error
is corrected to requested8041e940,not GP18.

Run-check source correction (2026-10-12): native header/fixture now consume
requested8041e940 via getRequestedEventSystemLevel,as original80037744 proves.
Whole GP18 remains the distinct getter/clock mask. Earlier whole-mask constructor
claim/fixture was incorrect and superseded. make test passes52 suites/78 Python
tests;make all passes. Next:conditional root actor/ID/wait-event binding and
native GP/arena/function owners,without fabricated callback results.

Context-address breadcrumb (2026-10-12): `event_context_addresses.cpp` supplies
coherent complete160-byte views and original array words,including native
retirement/replacement ordering. Tests preserve global work/flag aliases into
capacity/array fields,unknown94,timestamp halves,unmapped raw pointers and
adjacent-context accesses. Combined interpreter fixtures use both real context/
entry mappers. Final make test passes52 suites/78 Python tests;make all passes.
Next:GP/shared globals/backing arena/initialization ownership,source clock and
conditional Jump root/callback bindings.Scene/free/source/invalid-address/
FPSCR/stack/label/original traces remain explicit closure requirements.

Mapped-lifetime breadcrumb (2026-10-12): entry translator can retire a region
before native host release,return432-byte-per-entry snapshots and preserve
incoming original relationship words. Same-address registration resolves new
host objects independently of IDs. Prior full validation passed51 suites/78
Python tests and make all.Native owner still supplies free/replacement order;
no automatic allocator/event teardown was inferred.Next:context/GP/backing arena,
Jump root and actual registry bindings. Source/domain/runtime traces remain open.

Entry-translator breadcrumb (2026-10-12): `event_entry_addresses.cpp` implements
new delivered432-byte map with coherent semantic/byte views,all44 unknown bytes,
raw IEEE/pointer preservation,address-versus-ID relationships and deferred region
resolution. Combined interpreter fixtures now use it.Native unresolved-link
DEREFERENCE failure is explicit;raw words are not silently converted to zero.
Final make test passes50 suites/78 Python tests;make all passes;layout verifier
passes49 fields and ten ISO evidence suites. Current registered-region lifetime/
replacement and native loop/label/pointer/FPSCR/source domains remain limitations.

Documentation delivery acknowledged:packed layout/aliases and conditional normal
Jump registration/72-address direct callback census close the earlier LOCAL map/
census requests. Full callback implementations/native owner binding and original
execution remain required.Next:GP/context/heap/time address space,root U0/ID/wait
binding and actual original function registry,without fabricated callback results.

System-level source breadcrumb (2026-10-12): `event_system_level.cpp` binds
original requested-level/whole-GP-mask transitions to existing stop/start-all.
Tests preserve high bits,two writes/live reload,noninverse lowering,0xef/type10
exclusion.The former constructor WHOLE-mask binding claim was incorrect;
original run-check reads requested8041e940,corrected in the later source note.
make test passes48 suites/78 Python tests;make all passes. Native GP aliases/time/
window/main/UI/source ownership and original pause traces remain open. Next:
native storage/context/time and original source/callback binding.Prior packed
entry-map and Jump registration/census requests remain current dependencies.

Time-prefix breadcrumb (2026-10-12): `event_time_publication.cpp` ports raw
upper/lower rollover retry,winCheck and the complete admitted-clock prefix.
Tests preserve live GP/delta/mask groups,low/high writes,signed total-only cap,
GP10/window freeze and unconditional baseline/argument publication. A real
manager+WAIT_MSEC fixture proves no frozen-time catch-up. make test passes47
suites/78 Python tests;make all passes. Native hardware clock/GP/window/VI/main
sources and original cadence remain open. Documentation addendum:source reads
clock800000f8 BEFORE cap GP reload at8006f480/484,and reloads GP again between
baseline high store and callback argument (f604/608/60c);tests pin both orders.
Prior layout/Jump registration requests persist.Next:system-level wrappers and
native storage/time/source registry binding.

Original-script asset breadcrumb (2026-10-12): `pack_event_scripts.py` and
`event_script_assets.cpp` provide a byte-exact original-address path for ordinary
Jump and its power-check/badge-tail dependencies. Default hashes/address/sizes
are pinned;the four original trailing label-scan bytes are retained. make
 event-assets packages4540-byte ignored RomFS pack;actual reader verifies268
Jump commands and trailing read. Final full make test passes46 suites/78 Python
tests;make all passes,so earlier concurrent road validation issues are resolved.
No original stream execution/callback/source success is claimed. Next:native
layout/address/context/heap/time and original owner/function registry bindings.
Prior consolidated-entry-layout and NormalJump source requests remain open.

Release-service breadcrumb (2026-10-12): `event_memory_release.cpp` now ports
map/SDK free/coalescing and400KiB battle nested-pool requests. Tests cover each
family's distinct null/size/link/merge/lifetime policy and a complete bootstrap→
context-payload→nested-pool→release composition. All19 event suites pass;ARM
release object compiles. Full make test/make all currently encounter concurrent
road edits:core_tests still references removed warpCooldown and app link lacks
roadStartGround. Logs:/tmp/pm-memory-release-test-final.log and
/tmp/pm-memory-release-build.log. Native road session should finish helper/
fixture bindings;event edits do not touch them. Prior full bootstrap validation
passed43 suites/69 Python tests. Next:full rerun,native storage/context/time/
address owners and original stream/registry import;prior layout/Jump requests
remain open. No automatic event-owned buffer release was inferred.

Memory-bootstrap breadcrumb (2026-10-12): `event_memory_bootstrap.cpp` ports
SDK Init/Create/DestroyHeap and five-pool memInit/memClear,with original primary
map construction. Tests preserve original configuration,shared percentage
remainder,two-pass handles,ignored replacements and null-result headers. make
test passes43 suites/69 Python tests;make all passes;ISO verifier passes20 bodies/
205 anchors. Native arena/storage/cache/scene/source/domain remains open.Next:
delivered release/battle subpool helpers,native providers and original streams.

SDK-heap breadcrumb and native-layout request (2026-10-12):
`event_heap_allocation.cpp` ports SDK first-fit/signed-size/split/absorb/allocated
list and __memAlloc handle/zero/cache wrapper separately from _mapAlloc. Tests
pin node read/write order,size reread,all unlink/split routes,failures,zero count
and both original context payload requests. Final make test passes42 suites/69
Python tests;make all passes. ISO context/heap verifier passes8 bodies/139 anchors.

Context-init breadcrumb (2026-10-12): `event_context_initialization.cpp` ports
complete Init/ReInit and context banks. Tests retain two256/1b000 allocations,
separate post-allocation GP timestamps,low/high writes,retained/live pointers,
null allocation path,bank/schedule/ID/speed/other-context preservation and bulk
reset without cancellation. make test passes41 suites/69 Python tests;make all
passes. Native storage/SDK heap/address/cache/platform remains explicit.Next:
delivered bootstrap allocator and native providers,time binding,original Jump
source/registry ownership (requested above) and original runtime comparison.

Combined-interpreter breadcrumb and source-binding request (2026-10-12):
`event_interpreter.cpp` now routes all recovered local0..118 bodies through
actual namespaces/lifecycle/manager services. Integration tests preserve live
child→parent resumption,first-call polling,independent-root append policy and
fractional credits. Final make test passes40 suites/64 Python tests;make all
passes. Fixtures are authored,not original Jump execution.

Tail-opcode breadcrumb (2026-10-12): `event_tail_opcodes.cpp` covers1/3/11 and
113..118,evtGetPtr semantic array domain,and full debug classifier/call arguments.
Original sprintf/string sink remains an explicit formatter service. Tests pin
all thresholds/template/CR6/hex anomalies,raw flags,context/GP order,live polling
and active-only256 slot probe/yield. make test passes39 suites/64 Python tests;
make all passes. Next:combine interpreter services,then native context/bootstrap/
address/numeric/formatter owners and original Jump. Packed/extreme/source/runtime
and scratch-consumer traces remain existing EVT-01 closure requirements.

Memory-opcode breadcrumb (2026-10-12): `event_memory_opcodes.cpp` handles74..90
and local map allocation/fill/cache leaves. Tests preserve best-fit/first ties,
mandatory split/zero remainder,header/fill/cache order,no inferred buffer free,
count/output/root mutations,wrapped time products and distinct raw-address/
namespace-double-query routes. make test passes38 suites/63 Python tests;make all
passes. Native heap/address/cache/platform/extreme/alias/numeric/source ownership
and original traces remain open. Next:tail/debug routes,delivered heap/context/
bootstrap bindings and original Jump stream/callback services.

SWITCH-opcode breadcrumb (2026-10-12): `event_switch_opcodes.cpp` handles34..49
with semantic signed states/values and complete ordinary/group/mask/range routes.
Tests preserve captured versus live stack/query timing and header scans;actual
OR/AND success/failure dispatcher streams converge through END_SWITCH and wait.
make test passes37 suites/63 Python tests;make all passes. Native packed/invalid-
depth/source/group/runtime domains remain open. Next:delivered memory/buffer and
tail/debug routes,then native runtime sources and original Jump binding.

IF-opcode breadcrumb (2026-10-12): `event_if_opcodes.cpp` handles12..33 and the
full alignment-dependent strcmp body. Tests preserve unordered CR branch results,
query/argument/live-next mutation,raw masks,consumed scan markers and exact string
load extent. Real dispatcher executes either branch then WAIT_FRM. make test
passes36 suites/63 Python tests;make all passes. Native string/address allocation,
FPSCR/NaN exceptions and original traces remain open. Next:delivered SWITCH/grouped
CASE and remaining memory/debug families,then native sources/original Jump.

Loop-opcode breadcrumb (2026-10-12): `event_loop_opcodes.cpp` now handles4..8.
Tests cover signed labels/live fallback,raw and variable counts,zero/negative
threshold policy,callback-mutated index/body,and distinct BREAK/CONTINUE lexical
headers. A real dispatcher/data loop verifies all iterations before frame yield.
Final make test passes35 suites/63 Python tests;make all passes. Shared airborne
fixture failure from13:20 MDT was reconciled by the native overworld session.
Next:delivered IF/switch and remaining memory/debug handlers,then native context/
address/numeric services and original Jump. Existing source/domain/runtime and
invalid-depth/packed-alias closure remains open;no original trace was inferred.

Data-opcode breadcrumb (2026-10-12): `event_data_opcodes.cpp` now handles50..73
with actual operand/dispatcher integration. Tests retain RHS/query/convert order,
raw destinations/data, live cursor mutation and sequential IEEE versus indexed
signed-word floats. Original divw/fctiwz domains remain required provider services.
Next:delivered loops/predicates/switch and remaining memory families,then native
context/address/source owners and original Jump. Existing runtime traces remain
necessary;this helper does not claim original script execution or full VM closure.

Operand-service breadcrumb (2026-10-12): `event_operands.cpp` ports complete
integer/float namespaces and saved leaves. Tests cover asymmetric routing,
encoded boundaries, word/byte switches, signed flag addressing and old/GP/query
order. Original address/context/storage/FPSCR/save owners remain explicit
services. Next:delivered branch/loop/arithmetic/switch/data bodies, then original
Jump stream bindings. Existing source/domain/runtime traces remain open; no
provisional constants were substituted for measured behavior.

Opcode routing breadcrumb (2026-10-12): `event_control_opcodes.cpp` wires
recovered92..112 run/ID/child/control/block/end paths to lifecycle services.
Real manager/dispatcher integration exercises joined child copy-back and parent
resumption;ID outputs occur after local copies and plain/ID brother150 differs.
Float/output providers are explicit and unsupported routes require other modules.
Next:delivered operand namespaces and branch/arithmetic/loop/switch/data handlers,
then original Jump stream and actual native source owners. Existing original
runtime/context/alias/domain/pause/input/scene traces remain necessary closure.

Control-service breadcrumb (2026-10-12): `event_controls.cpp` and its suite
port exact stop/start inlined context pattern,postorder relationships,all/other/
ID scans,CheckID and priority/type/speed setters. Tests preserve hard0xef,active
liveness,transitive exclusion and current-pass remaining credits. Next is actual
control/run/block opcode routing into dispatch,then full operand/branch/data
providers and original Jump. Existing context/type/pause/source/cycle-domain and
original multicredit/cancellation traces remain needed;no generic pause refcount
or immediate suspension was inferred.

Lifecycle breadcrumb (2026-10-12): `event_lifecycle.cpp` ports newly delivered
complete Delete/DeleteID and Restart contracts,including confirmed8041e860
counter. Tests cover live descendant order,cancellation copy-back,independent
inline lifetime,duplicate IDs,stale schedules,restart preservation and parent
resumption through the real manager. Prior deletion-counter request is answered;
stop/start/control binding and full original graph/value/source/pause traces
remain next. No helper claims packed original aliasing or malformed-cycle repair.

Descendant breadcrumb (2026-10-12): event_creation now ports child/brother
constructors and107/110 block wrappers,including active-manager unconditional
append,child wait links,global speed,inherited/reset metadata,distinct zero-ID/
run-check order and post-run-check source recopy. Tests retain copied local words
and independent inline links. Next is deletion/copy-back/control binding.
Please pin complete evtDelete context-capture and active-count/SDA write identity
alongside the existing EVENT_RUNTIME deletion steps,including whether its
counter is8041e860 used by constructors;this avoids guessing a counter owner.
Original capacity/label/relationship/cancellation/pause traces remain requested.

Constructor breadcrumb (2026-10-12): `event_creation.cpp` ports delivered
root/typed constructors,label scans and run-check with original append/ID order.
Native free capacity and16-label domain assertions are explicit boundaries;
please retain capacity/label-domain provenance and original constructor/creation/
pause traces as open obligations. Child/brother/inline/deletion/restart/control
binding and full tagged opcode/value providers remain next before native Jump.

Current delivered coverage:the complete local manager/priority loop,root/typed/
child/brother constructors,restart,plain/ID run and concurrency variants,wait/
USER_FUNC continuations,control wrappers,setters,full deletion/stop/start and
integer/float operand routing/saved RAM leaves are now linked in the scoped delivery table and EVENT_RUNTIME
follow-ups. These answer those parts of the original request below. [Complete opcode coverage](../behavior/event/EVENT_OPCODE_IMPLEMENTATION_COVERAGE.md)
now maps all119 table entries to recovered local contracts,including tail/debug
and WAIT_WHILE. Remaining closure is numeric/storage/context/time/source/domain
initialization,address/callback translation,heap ownership and original pause/
multicredit/creation/deletion/Jump/story traces. A portable manager test pass
is not a complete executed Jump/story VM.

Dispatcher implementation breadcrumb (2026-10-12): `event_dispatch.cpp`
and its suite implement delivered fetch/common tails,WAIT_FRAME,WAIT_MSEC and
USER_FUNC continuations. Manager integration verifies multiple credits share
one caller sample and do not advance MSEC lifetime between polls. Other opcodes
remain explicit handlers;native function/address/value/scene providers and
constructor/control/ownership bindings are not inferred. Next:delivered root/
child/brother/inline constructors,label/run checks,deletion/restart and controls,
then remaining tagged operand/branch bodies and original Jump services. Preserve
existing original runtime/pause/cancellation/credit trace requests.

Manager implementation breadcrumb (2026-10-12): `event_manager.cpp` and
its host suite now port the delivered complete manager loop/priority table.
Explicit GP/context/clock/dispatch providers preserve captured versus table
context,raw tick delta,eligibility,identity,rounded credits,live count/appends
and abort/no-inner-revalidation behavior. It does not sample pads or infer60Hz
original clock values. Native constructors/dispatcher/value domains,actual GP/
context/time/pause producers and original multicredit traces remain necessary
bindings. Next:delivered wait/USER_FUNC continuation services and root/child/
brother construction,then original Jump graph.

Implementation breadcrumb:commit717da56 adds `battle_action_services.cpp` and
its host suite. The native adapter now uses Start/Result/Stop services and records
help requests,while keeping its provisional attack timer explicit. Result polls
the stored pending bit after evaluation;Stop leaves callback teardown to main.
Parameters/raw flags/result/count/window readers and prize wrappers are portable;
original tagged VM,prize-byte producers and help UI remain unbound. ID1's transient
compile observations were resolved in717da56 (signed literal and typed clamp);
subsequent make test/make all pass. Continue requesting actual script/VM owners
rather than treating a passing command helper as complete original attack timing.
