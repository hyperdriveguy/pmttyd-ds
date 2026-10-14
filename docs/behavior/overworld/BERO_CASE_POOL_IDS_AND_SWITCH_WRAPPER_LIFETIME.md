# Bero case pool, IDs and switch-wrapper lifetime

US G8ME01 disc0 revision0; read-only static recovery2026-10-14 for
[EVT-01](../../requests/EVT-01.md). Extends
[indexed query/state](BERO_INDEXED_INFO_LENGTH_AND_REGISTRATION_STATE.md),
[house05 incoming bank](../field/HOUSE05_INCOMING_LOCALS_AND_INSTALLER_FRONTIER.md)
and [case9 admission/lifetime](../field/TROUBLE_CENTER_DOOR_CASE9_GEOMETRY_AND_LIFETIME.md).
Addresses, structure offsets and masks are hexadecimal; counts, indices and
event priorities are decimal. This records source allocation and scheduling,
not observed runtime case numbers or a complete native case implementation.

## Shared allocation owner and actual slot construction

Case work is an8-byte object at8041e930, referenced by wp80415fc8. It owns
count+0 and record-array pointer+4. caseInit8006ed60/5c writes count64,
requests __memAlloc800300f0(heap0,64*160), stores the returned array and
clears that entire allocation. This is not a per-door or per-NPC list.

caseEntry8006ec28/100 captures count and array, scans slots in ascending
order for first U16flags+0 with active1 clear, then clears160 bytes at that
slot and constructs it. It has no name deduplication and no full-pool refusal:
if all configured records are active, the shown loop advances beyond them
before clearing/constructing. A native capacity check is necessary policy,
not a source successful no-op. The returned index is the actual first-free
slot, not the Bero table index or an authored source-node offset.

Descriptor→case publications include:

| Case offset | Constructed value |
| --- | --- |
| 00 | U16active1;also4 if descriptor type had8000 |
| 04 | descriptor U16type with8000 removed |
| 08.. | strcpy original name |
| 48 | actual allocated index |
| 4c | descriptor+08 argument bits |
| 50 | descriptor+0c callback pointer |
| 54..93 | memcpy16 descriptor local words |
| 94 | zero eventID |
| 98 | descriptor+54 original event script |
| 9c | descriptor+58 priority |

Unlisted bytes remain cleared by construction. The source strcpy and allocation
do not establish arbitrary-name/full-pool safety. Preserve source pool order
and actual incoming local words; translating geometry or selecting a table
does not establish the same allocation result.

Descriptor span is5c bytes: U16type+0,name+4,argbits+8,callback+c,
LW16+10..4f,unused+50,script+54,priority+58. Wrappers do not initialize
the unused+50 word; caseEntry does not read it. Destination name storage
physically occupies64 bytes atcase+8..47, but strcpy8026d5fc has no bound.
Native bounded-name validation must be explicit policy,not a claimed source
strncpy. Clearing precedes index48 publication; flags+0 are cleared again,
type/active flags/name/remaining fields are set,then eventID94=0,then
memcpy16locals is the final payload publication before returning the index.

## Bero registration wrapper differs from ordinary registration

evt_run_case_evt_bero80070034/dc captures args and calls:
evtGetValue on raw operands0,1,2 in order;
**evtGetNumber800310fc on raw operand3**;
evtGetValue on raw operand4. Nonzero resolved operand1 ORs8000 into type.
It builds descriptor name, argument bits, callbackNULL, original script and
**priority20**, copies16 callerLW words at800700d4, then calls caseEntry.
After allocation it freshly reads raw operand5; if nonzero it publishes
the actual returned index with evtSetValue. Return2.

evtGetNumber is raw namespace-number conversion, not evtGetFloat80030840
and not an ordinary dereference of an event variable. All scoped Bero and
house05 calls use literal operand3=0, so this distinction leaves that finite
value0. It nevertheless corrects the earlier case9 contract's general prose
which called this argument's service evtGetFloat.

Ordinary evt_run_case_evt80070110/d8 uses the same descriptor shape but
priority0. Consequently house05 inline case9 registrations and board/counter
cases differ from Bero registrations even when both use a collision name.
This is a real scheduling/ownership distinction, not a display preference.

## Original Bero registration producers

bero_case_entry8030e608/190 READs actual BeroNUM intoLW2, setsLW1=0 and
loops every source table entry in order. Each iteration calls indexed info,
which publishes LW3..14 and reads actual BeroSW intoLW9. Only LW9==-1 enters
the registration switch. Base kind=LW4&fff chooses:

| Kind | Script passed to evt_run_case_evt_bero |
| --- | --- |
| 0xf00 | bero_case_exec_none8030da14 |
| 0 | bero_case_exec_road8030dac8 |
| 1 | bero_case_exec_door8030dbf4 |
| 2 | bero_case_exec_pipe8030e28c |
| 3 | bero_case_exec_gate8030e554 |

Every shown request is (LW10,1,LW3,0,script,LW15), where LW10 is the actual
resolved case type. ROAD kind0 first requests
evt_hitobj_attr_onoff(0,1,LW3,00800000) at8030e690. Allocation snapshots the
whole bank **before** its output writes the new slot into callerLW15; the
saved case's LW15 therefore contains the caller's previous word at that point.
Do not substitute the newly returned slot into an already captured snapshot.

Next evt_bero_case_id_save(LW1,LW15) at8030e744 stores that slot in
BeroID803dc090. An independent inline then calls raw evt_bero_switch_on(LW1)
at8030e758; its local changes do not copy back. The main loop incrementsLW1
untilLW1==LW2. It does not locally assign invented handles for missing case
providers. Unsupported kinds have no registration arm; their downstream
state is not a generic successful registration.

bero_case_entry_sub8030e798/11c implements the same kind→script allocation
and ROAD attribute-on sequence for a single already-published indexed entry,
then saves actual LW15 in BeroID at8030e89c. It does not perform the parent's
loop or inline switch-on itself.

## Raw switch functions versus higher-level event wrappers

Raw evt_bero_switch_on800794b0 and evt_bero_switch_off800793ac only update
BeroSW and publish LW0/LW1; their exact effects are in the indexed-state
contract. Actual higher-level **bero_case_switch_on/off** events add lifecycle:

bero_case_switch_on8030e8b4/4c:

1. raw switch_on(LW0) resolves numeric/name input, publishes previous-state
   Boolean inLW0 and selected indexLW1, then setsSW[index]=1.
2. indexed info(LW1), preserving that LW0 while publishing entry outputs.
3. Only ifLW0==0, RUN_CHILD bero_case_entry_sub and await actual registration.
   Otherwise skip allocation to prevent a duplicate.

bero_case_switch_off8030e900/7c:

1. raw switch_off(LW0),then indexed info(LW1).
2. Only ifLW0==0 (previousSW was1), case_id_load(LW1,LW15),
   evt_del_case_evt(0,LW15),then
   evt_hitobj_attr_onoff(0,0,LW3,00800000).
3. Otherwise skip those requests. Return/end.

Thus raw callbacks do not allocate/delete cases; these event wrappers do.
Source writes SW before the later allocation/deletion provider. A native
execution owner must preserve that scheduling/pending lifetime and not
rollback or falsely acknowledge unbound subsequent operations.

evt_bero_case_id_save8007928c/60 resolves operand0 index, then operand1 ID,
wraps index<<2 and writes BeroID[index] at800792cc. case_id_load80079230/5c
resolves index, then reads raw operand1 destination, fetches BeroID[index]
and calls evtSetValue. Both return2 without source index bounds checks.
get_info does not initialize BeroID. Do not equate BeroSW -1/0/1 with case
handles, or reconstruct BeroID from an immutable original table.

## Deletion, pool reuse and event retirement

evt_del_case_evt8006ff04/84 resolves mode then caseID. If mode==0, it calls
caseIdToPtr8006e440, reads case+94 eventID, calls evtCheckID, and if alive
calls evtDeleteID80036664. Then it calls caseDelete for the resolved index.
Nonzero mode skips the event check/deletion and still deletes the case.
There is no shown NULL guard after caseIdToPtr; mode0 requires an actual
active valid case, not a stale/missing fabricated ID.

caseIdToPtr returns array+wrap(index*160) when active1 is set, otherwiseNULL,
without source bounds checking. caseDelete8006ec0c/1c clears only active1
in the indexed record. It does not clear its data, erase BeroID or kill its
event independently. caseReInit8006ed28/38 likewise clears active1 in every
configured slot without zeroing all fields or freeing the pool. A subsequent
caseEntry may reuse the slot and replaces all160 bytes. BeroID old values
alone do not identify a still-live registration after deletion/reinitialization.

Map _unload80007ab4 calls evtmgrReInit at80007b3c,then caseReInit at80007b40,
before later primary/secondary heap resets80007bf4/7bfc. The ordering is
important: the case clear itself does not supply event deletion. See
[reset callers](../event/EVENT_BOOTSTRAP_HEAP_AND_RESET_CALLERS.md).
Native area transactions must retain/restore pool, BeroID/SW and pending
events consistently; a failed map staging must not leak collision attributes
or named lookup flags into another area.

## CaseMain execution and remaining native frontier

caseMain8006e5ec/620 first captures current Mario contacts/input, then applies
system/window/global/field-battle/Mario validity gates. Its mark pass uses
actual per-type contact and input admission; a later separate pool-order pass
visits active1+marked2 records. It checks actual saved eventID94 and clears
that field if dead. Only zero eventID creates
evtEntry(script98,priority9c,flags20), sets event+16c=case index, saves the
actual returned eventID and copies16 case locals into event+9c.
**Only priority20 invokes npcStopForEvent80043cd8** at8006ebd0.
Transient case mark2 is then cleared, including when a live event prevented
reentry. Event root typeef comes from the original root constructor;
flags20 is not an event type. Cases remain active after their event ends.

Exact second-scan order at8006eb50..8006ebf4: capture array from currentwp+4,
index0; test fresh U16active1 then marked2; read94 and call evtCheckID;
ifdead store94=0; reread94. Zero creates with script98 read before priority9c.
After constructor returns,read case48 and store event16c,read event15c and
store case94,copy locals,then reread casepriority9c for the20 NPC-stop test.
Finally reread caseflags and clear2. Advance record160/index1; read current
wp and configured count afresh on every loop test,while array remains the
captured base. Constructor append/run-check happens before these caller-side
case/local publications;do not move the copy before evtEntry or dispatch a
new event synchronously inside the wrapper. There is no source NULL-return
guard after evtEntry;native event-capacity refusal is separate policy.

No source actual slot numbers can be supplied from static tables: prior
active cases, deletions, area/NPC callbacks and timing determine first-free
allocation. Source cases need actual caller snapshots and all shared pool
ordering. Native fieldDoorAdmitted can supply a bounded contact predicate;
it does not construct this pool or establish BeroID. Native EventNativeRuntime
can supply already-recovered construction/manager mechanics after actual
script/callback/context bindings exist. Full original NPC stop/start, contact,
attr mutation and event retirement providers remain separate.

Ordinary evt_bero_info_run launches evt_bero_mario_enter independently at
8030f050. That event waits actual sequence state at8030f2d8, executes actual
entry childLW7, Bero/arrival-kind/Mario work, resumes NPC state and finally
RUN_CHILDs bero_case_entry at8030f424 before finishing BeroEXEC requests.
Save-load domain instead RUN_CHILDs bero_case_entry at8030ed64 directly.
The ordinary gor_02 init root continues after the independent launch; do not
assume all Bero cases allocate before house05 or all house05 cases allocate
before them without the real sequence/event schedule.

## House05 and inspected area switch callers

House05 original inline installer calls ordinary priority0 registration twice
after its copied GW11/list/map prefix; it does not call a Bero switch wrapper.
Its incoming bank still depends on the source producers established in the
house05 contract. Local house05 open/close callbacks launch gor.rel3ec20/
3ec8c children, whose shown work toggles named hit groups, with later source
story/flag/GW guards on closing; neither calls a Bero switch wrapper.

Fresh gor.rel ADDR32 import census found no direct raw switch_on/off imports
and no wrapper imports in gor_02.o. Actual higher-level wrapper consumers are:

| Wrapper | gor.rel import operand sites / source owners |
| --- | --- |
| on8030e8b4 | 31144:nancy_talk@gor_01.o;52a98/52ad8/52b18/52b58:bteresa minigame@gor_03.o;605d0/605fc:gor_04_init |
| off8030e900 | 30c6c:nancy_init@gor_01.o;52a74/52ab4/52af4/52b34:bteresa minigame@gor_03.o;60634:gor_04_init |

For example Nancy init setsLW0=w_bero at30c5c and RUN_CHILDs off at30c68
inside its original saved-flag branch; station init setsLW0=n_bero then
RUN_CHILDs on/off at605cc/605f8/60630 under original train/entry branches.
These are scoped source import facts, not permission to bypass those quests,
NPCs or train scenes. Full-game REL/indirect callback closure remains open.

## Evidence

Ignored listings artifacts/bero-case-evt_run_case_evt_bero.txt,
bero-case-caseEntry.txt,bero-case-caseInit.txt,bero-case-caseMain.txt,
bero-case-caseDelete.txt,bero-case-caseReInit.txt,bero-case-caseIdToPtr.txt,
bero-case-evt_del_case_evt.txt,bero-case-evt_bero_case_id_load.txt,
bero-case-evtGetNumber.txt and bero-case-unload.txt.
Original event reports bero-case-callers.json,bero-case-init-check.json,
bero-case-area-callers.json and prior house05-bero-subchildren.json.
Wrapper on SHA2569b673def2593b67664535c1ccde47e97e041f532aa97fa942e69f78d59d8596e;
off79dfe181460c2ca3667846bd497ee7bed0bba3ceb7837169a0fe74fe33c10d94.
Original REL SHA256d4965f21254ecf1a78e5d66ea12a0b000eed3f123032af6ab3cb0f703d3f185a.
No shared documentation, native code, builds or git operations changed here.
