# House05 incoming locals and installer frontier

US G8ME01 disc0 revision0; read-only static source recovery2026-10-14 for
[EVT-01](../../requests/EVT-01.md). Complements
[house05 installer/GW11](HOUSE05_ORIGINAL_INSTALLER_GW11_AND_NPC_LIST_CONSUMERS.md),
[child construction](../event/EVENT_CHILD_BROTHER_AND_INLINE_CONSTRUCTION.md)
and [child deletion/copy-back](../event/EVENT_DELETE_AND_COPYBACK.md).
Addresses, offsets and masks are hexadecimal; local indices/counts decimal.
This is a source execution frontier, not an original-runtime trace or a claim
that a native initial16-word local bank is known.

## Incoming bank is not established by the house05 prefix

gor_02_init_evt file4d554/87c reaches house05 copy/list selection at4d8e8
after earlier entry work. At4d5ec it sets LW0=bero_entry_data;
4d5f8 requests evt_bero_get_info;4d600 RUN_CHILDs evt_bero_info_run.
The attached child's construction copies all16 parent local words, and its
deletion copies all16 words back, including local changes made by its own
attached descendants. The local bank at4d8e8 therefore cannot be inferred
solely from the root constructor's cleared initial storage.

evt_bero_info_run DOL8030ec7c/3e4 first requests
evt_bero_get_into_info8007a214. That callback selects actual incoming-entry
information and writes event offsets a0..bc (LW1..8). Explicit stores include
8007a2d4/a2dc/a2e4/a2ec and the later branch's a3ac/a3b4/a3c8/a3d4,
plus a3dc/a3ec/a3f8/a400 and a4c0/a4c8/a4dc/a4e8. Values depend on the
selected entry and its original fields; they are not all constant zeros.

The original child then reads actual system/Bero flags, has a save-load branch
which RUN_CHILDs bero_case_entry at8030ed64 and returns at8030ed6c,
and on the ordinary branch RUN_CHILDs evt_bero_initial_pos at8030eefc.
It launches evt_bero_mario_enter independently at8030f050 before returning.
Those distinct child and independent-event lifetimes must be preserved;
independent RUN_EVT changes do not automatically copy locals back.
The complete transitive producer/read closure for every incoming LW4..14
has not been recovered here. In particular, absence of a direct assignment
in the outer gor_02_init body is not evidence that these words stayed zero.
This contract does not assert that every one of LW4..14 is dynamic; it asserts
that their complete values have not been established and must be retained.

Later gor_02_init requests NPC setup at4d630 and RUN_CHILD
evt_gazigazi_entry at4d6dc. The inspected gazigazi child803646e8/64 contains
NPC entry/position/pera/yaw/flag requests, without explicit LW destinations.
It is not a blanket local-bank reset. Subsequent attached door children are:

| Outer call | Original child | Explicit local outputs in inspected body |
| --- | --- | --- |
| 4d844 | evt_door_02_entry,file45d10/100 | selector3 LW1/2;selector1 LW1/2/3;LW15 assignments8 then0 |
| 4d858 | evt_door_03_entry,file462ac/100 | selector3 LW1/2;selector1 LW1/2/3;LW15=0 |
| 4d86c | evt_door_03_roll_entry,file467b8/50 | selector1 LW1/2/3;LW15=0 |
| 4d8dc,only story>=23 | generic evt_door_entry,DOL8033156c/134 | selector3 LW1/2;selector1 LW1/2/3;LW15=0;selector9 LW1 |

The source branch for story<23 instead registers the locked Trouble door.
These inspected later bodies do not establish a reset of LW4..14. Their
attached-child copy-back preserves untouched words rather than synthesizing
fresh zero locals. Incoming LW2/3 residuals may consequently depend on the
preceding branch, but house05 explicitly overwrites them before its cases.

## Exact selector outputs and known house05 locals

evt_door_param800e81f4/3a0 resolves its first two arguments with
evtGetValue in order: record address, then selector. Its output destinations
are raw event operands passed to evtSetValue, not resolved values to overwrite
as host pointers. The selector dispatch table is803316a0:

| Selector/target | Ordered outputs |
| --- | --- |
| 1→800e829c | raw operand2←record+0c;operand3←record+10;operand4←record+14 |
| 3→800e82f4 | raw operand2←record+20;operand3←record+24 |

Each source read/publication is sequential, using evtSetValue80030cfc.
Selector1 stores occur through800e82a8,800e82b8,800e82c8;
selector3 through800e8300 and800e8310. This local callback returns2;
it does not register cases or hide maps/NPCs by itself.

After4d8e8..4d948 selects the copied record's NPC-list pointer,4d94c requests
selector3(LW0,3,LW1,LW2). It publishes S_ie_05_naka intoLW1 and A_ie_05C
intoLW2. The conditional requests at4d970 and4d998 respectively hide the
actual named map hierarchy with evt_mapobj_flag_onoff(1,1,LW1,1) and disable
the actual collision hierarchy with evt_hitobj_onoff(LW2,1,0).
These requests can be implemented as a bounded prefix independently of later
case registration, provided their source names/hierarchies and mutable flags
are actually bound. A synthetic interior mask does not establish either
named hierarchy service.

At4d9b0 selector1 overwrites LW1/2/3. Before either evt_run_case_evt:

| Local | Established value |
| --- | --- |
| LW0 | actual mutable house05 copy address, also published GW11 |
| LW1 | S_ie_05_doa |
| LW2 | A_ie_05_doa02,close collision |
| LW3 | A_ie_05_doa01,open collision |
| LW4..14 | incoming words,unmodified by this inspected house05 prefix |
| LW15 | close0 at4d9d8;open4 at4da14 |

evt_run_case_evt80070110/d8 snapshots all16 caller words in each actual
registration at4d9e4 and4da20. The final argument0 publishes no case index.
A finite descriptor may retain the proven names/requests and the need for
an incoming bank, but must not label invented zero LW4..14 as original saved
locals. Real snapshots remain unbound until that bank exists. A later proof
that an event overwrites an input before reading it could support a narrower
execution adapter; it would still not turn fabricated words into exact source
case snapshots.

## Earliest generic open/close services

The registered scripts are generic evt_door_open8032f670/7f0 and
evt_door_close80330724/6fc. They begin:

| Open | Close | Request |
| --- | --- | --- |
| 8032f670 | 80330724 | evt_cam_letter_box_disable(1) |
| 8032f67c | 80330730 | evt_mario_key_onoff(0) |
| 8032f688 | 8033073c | SET DoorFLAG=1 |
| 8032f694 | 80330748 | evt_door_param(LW0,7,LW1,LW2) |

Selector7 returns record+40/+44 into raw operands2/3. Open tests LW1!=0
at8032f6ac, sets LF0=0 at8032f6b8 and RUN_EVTs that callback at8032f6c4.
Close tests LW2!=0 at80330760,sets LF0=1 at8033076c and RUN_EVTs it at80330778.
These are independent launched events, not blocking child calls.
House05 callbacks46c08/46c40 already launch original map/world children
and issue original BGM/environment requests. They cannot be acknowledged
as no-ops while claiming door behavior. Generic open then changes actual
exterior map flags and starts independent camera work; close starts its own
camera/animation sequence. Missing services must hold at their actual call
frontier; preflight resource policy may prevent admission before controls
are taken, without inventing successful source requests.

The native reusable part is fieldDoorAdmitted's case9 contact/A/common-gate
predicate and the host-testable EventNativeRuntime construction/scheduling
machinery. FieldDoorState remains a movement/wait/interior-mask bridge, not
an executor of these original bodies. PMDOOR2 only admits existing room0mask1
and room2masks2/4 records and at most3 entries; no house05 mask is proven by
that native format. The current board/counter contact roots are not a general
64-slot case manager. door-data.pmevt includes local REL data closure; imported
DOL open/close addresses do not themselves include the DOL script words.
Actual scripts, callbacks, saved-local cases and area-owned record lifetime
must be bound before extending this prefix into house05 gameplay.

## Evidence and limits

Original rel/gor.rel SHA256
d4965f21254ecf1a78e5d66ea12a0b000eed3f123032af6ab3cb0f703d3f185a.
Ignored reports/listings: artifacts/trouble-child-installer-events.json,
house05-preceding-door-entries.json,house05-preceding-dol-events.json,
house05-bero-get-info.txt,house05-bero-into-info.txt,house05-doorparam.txt,
house05-generic-door-events.json. Bero info event SHA256
bb92a499be0384713a44616f99f5e3f725f2cf507b3132126bb1e71b521f4a59.
Full incoming bank/transitive Bero producer closure, original-runtime timing
and raster behavior remain open. No code/shared documentation/index/build
changes were made for this contract.
