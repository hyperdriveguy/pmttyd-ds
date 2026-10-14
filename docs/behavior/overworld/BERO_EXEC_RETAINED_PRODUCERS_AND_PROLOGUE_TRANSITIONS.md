# BeroEXEC retained producers and prologue transitions

US G8ME01 revision0, read-only recovery2026-10-14. Complements
[road contracts](OVERWORLD_ROGUEPORT_ROADS.md),
[independent departure ordering](OVERWORLD_ROAD_LANDING_WAIT.md) and
[letterbox consumer](../camera/CAMERA_LETTERBOX_AND_DUMMY_DRAW_SERVICES.md).
Static source evidence, not observed original timing or complete scene execution.

## Actual word lifetime and direct writers

BeroEXEC is word8041ea14, .sbss evt_bero.o, addressed by r13+1af4 with original
r13=8041cf20. Startup __init_data's actual clear table800055b0 includes
range8041e580:sizeea4, covering this word and initializing it0. It has no
initialized file payload; an ISO read at a guessed BSS offset is not evidence.

A scan of all seven loaded DOL executable sections for direct r13+1af4 memory
instructions finds exactly:

| Instruction | Owner | Operation |
| --- | --- | --- |
| 8007a5ec | evt_bero_get_info8007a50c/100 | Store zero after table loading |
| 8007a9d8 | evt_bero_exec_wait8007a9c0/44 | Read |
| 8007aa14 | evt_bero_exec_get8007aa04/30 | Read |
| 8007aa68 | evt_bero_exec_onoff8007aa34/68 | Read |
| 8007aa7c | evt_bero_exec_onoff | Store resolved OR/AND-clear result |
| 8007ac40 | bero_get_BeroEXEC8007ac40/8 | Read/return |

These are complete direct displacement references, not a proof excluding all
indirect stores, imported REL code or injected runtime changes.

evt_bero_get_info clears16 table slots, validates hit names through hitGetPtr,
loads actual records from EventEntry.LW0+9c into BeroINFOARR, initializes the
used BeroSW entries-1, writes countBeroNUM, then resets BeroEXEC0 at8007a5ec.
The reset is after these providers. Do not clear it on every frame, battle
entry or Scene change. Each Rogueport room initializer supplies its actual Bero
table and calls this wrapper: gor_00 REL2f240, gor_01 REL3c3bc,
gor_02 REL4d5f8, gor_03 REL5a3f0,gor_04 REL603fc (command addresses; relocated
function operands are four bytes later). Fresh gor.rel import inspection finds
these five get_info bindings and no import of exec_onoff/get_BeroEXEC.
Rogueport transition mutations therefore use the shared DOL event scripts.

evt_bero_exec_onoff resolves arg0 with evtGetValue80031260, then arg1 with
the same service, then freshly loads the retained word. If arg0==0, OR arg1;
otherwise AND-clear arg1. Store result at8007aa7c, return USER_FUNC2.
It does not branch on firstCall, clamp mask, reset other bits or wait.
Argument0 means set; argument1 means clear. Preserve all32 mask bits, including
sign-bit masks. No authored Scene-to-busy conversion belongs here.

## Lowbit1 producers relevant to letterbox

bero_case_init_check starts by clearing10000 at8030d888, gets the word and
ANDs1. Existing busy1 returns LW0=1 immediately, without a new set1 request.
Otherwise it performs actual NPC/position/current-Bero providers, sets1 at
8030d994, then invokes the custom out child. Custom handled resultLW0==1
resumes NPC services, sets10000 at8030d9d8, clears1 at8030d9e8 and returns1.
The unhandled generic departure retains1 until its type-specific completion.

evt_bero_info_run ordinary path checks existing busy1 and returns if set;
otherwise sets1 at8030ed94 before key-off/initial placement and launches
evt_bero_mario_enter. The separate systemflag1 load path sets1 at8030ecb8,
then has its own wait/NPC completion sequence; it clears1 at8030ed44 and
sets10000 at8030ed54 before case registration.

evt_bero_mario_enter waits for actual sequence2 and executes custom entry.
Handled customLW0==1 enables keys and jumps common label100. Generic paths
perform type-specific travel. At the common completion, resume NPC state,
register cases via child bero_case_entry, clear1 at8030f42c, then set10000 at
8030f43c. These stores follow actual child completion, not an arbitrary cooldown.

## Complete shared-DOL event writer request census

The scan matches actual USER_FUNC header0003005b followed by8007aa34 and
its two literal operands, across named loaded DOL data spans. Set/clear below
are decoded using the actual wrapper polarity. Every recovered operand is
literal; no RW/LW substitution is needed for this bounded shared graph.

| Script | Command addresses and mutations |
| --- | --- |
| bero_case_init_check | d888 clear10000; d994 set1; d9d8 set10000; d9e8 clear1 |
| bero_case_exec_none | da54 set10000; da64 clear1; da74 set8 |
| bero_case_exec_road | db74 set10000; db84 clear1; dba0 set8 |
| bero_case_exec_door | dcd0 set10000; dce0 clear1; dd04 set8 |
| bero_case_exec_pipe_sub | e0cc set2; e194 clear2; e264 set10000; e274 clear1 |
| bero_case_exec_pipe | e500 set8 |
| bero_case_exec_gate | e594 set10000; e5a4 clear1; e5b4 set8 |
| evt_bero_info_run | ecb8 set1; ece8 set2; ed1c clear2; ed44 clear1; ed54 set10000; ed94 set1 |
| evt_bero_mario_enter | f33c set2; f3f4 clear2; f42c clear1; f43c set10000 |

All abbreviated addresses have prefix8030. Thirty requests total. Bit2 and8
have distinct pipe/pre-mapchange consumers and must not be treated as bit1.
In generic ROAD, db74/db84 occur in the independent landing/travel inline;
parent WAIT150ms then dba0 set8 happens independently, followed by WAIT2frames
and mapchange. Parent mapchange is not gated by inline completion. For DOOR,
the actual door script owns its separate choreography; native interior mask
changes are not themselves BeroEXEC stores.

## Bindable native operations and outstanding ownership

Current Game's road contact handler identifies an actual admitted original
road and callback policy. A future retained BeroEXEC owner can bind the shared
init-check set1 after the actual request providers and custom callback admission,
then the independent inline's set10000/clear1 when RoadMotion genuinely ends.
The native parent timer's requestMap boundary corresponds to the separate set8;
do not couple clear1 to that timer. Arrival begin/completion are candidate native
adapter boundaries for source info-run set1 and common-tail clear1/set10000,
but complete original initial-position/case-registration/NPC/child scheduling
must remain explicit rather than claiming whole event execution from these hooks.

Game.finishWarp invokes restore(next), which is also used by checkpoint/battle
flows. Therefore adding an unconditional BeroEXEC reset to restore would give
unrelated flows a false get_info completion. Reset belongs to actual room Bero
table construction after providers, not a general restore operation. Native
cancelRoad rollback has no recovered counterpart clearing busy flags; do not
invent that store without a declared cancellation adapter and retained state.

Native FieldDoor begins inline interior travel on admitted door contact and
changes collision/mask locally. There is currently no shared init-check child
executing its actual set1 provider there; attaching busy1 merely while the mask
is moving would be an inferred scene-state substitution. Bind typed original
writer requests at the authored event milestones, or retain unavailability.

Startup0 is a real producer and is safe to retain for an owner whose observed
history has not processed room/event writers. It is not evidence of current0
after unbound prologue room transition history. Once history is known incomplete,
letterbox should expose missing producer/history instead of inventing0.

## Validation

Fresh complete body SHA256:
evt_bero_get_info8007a50c:100 =
bcdf80ce9404618a80e5e550dbc53dff42c732a89f27d791e729d4819db5936e;
evt_bero_exec_onoff8007aa34:68 =
fa1a47efc98af3d6e052762a0021f85bd64df5b09117589eee47fd1af6ea5b9b.
Fresh read-only direct-reference scan and literal event scan reproduced the
six references/thirty requests above. Fresh gor.rel relocation bindings checked
the five room reset requests. Existing verify_overworld_roads.py passed25 DOL
spans,14 REL spans,83 bindings,40 anchors,five maps/ten nodes;
verify_overworld_road_landing_wait.py passed19 DOL spans/46 anchors.
No make/build/emulator/hardware claim; no original payload added to source
history. Inspection outputs remain ignored artifacts/reference/camera-manager/.
