# Trouble Center quest2/13 writers and acquisition

US G8ME01 disc0 revision0; recovered2026-10-13 from read-only rel/gor.rel
and rel/tou.rel bytes and relocations. Narrow caller/writer graph for
[OW-INTERACT-01](../../requests/OW-INTERACT-01.md) and
[ITEM-01](../../requests/ITEM-01.md). Complements
[Goomba4 hand-in](ROGUEPORT_GOOMBA4_QUEST_AND_ITEM_SELECTION.md).
Static source evidence does not establish native execution or full Trouble
Center UI/NPC setup. No dialogue prose or extracted assets are added.

## Acceptance establishes the working switches

`gor_irai_main` gor_irai.o,file660a0/1f8 contains the actual acceptance owner.
After original selection machinery publishes LW11 as a zero-based table index,
it prints the indexed exposition and LW12 description, requests
`evt_msg_select(0,irai_01)`. In selection-result LW0==0 (Yes), it appends
`irai_73`, calls `irai_start_func`, obtains `irai_start_event[LW11]` and runs
that child, then sets GSW1420=LW11 and increments GSW1420. The resulting quest
number is one-based. Never set the quest merely from reading its consumer guard.
The menu's availability/filtering and LW11/LW12 producer remain separate C
services; this contract does not admit arbitrary quest acceptance in every story.

`irai_start_func` file10a48/7c has no operands/firstCall behavior. It clears
GSF5405..5421 inclusive (17 flags), then sets GSW1469..1474 inclusive (six
bytes) to0 through swByteSet, then returns event2. It does not itself set
GSW1420. Thus original accepted quests2/13 establish5405/5406/5407 false through
this explicit caller, independently of swInit. They do not preserve prior
quest-progress flags. Relocated start-event table entries for quests2/13 both
point to irai_start_event_null,file65dd4/8: RETURN then END.

`gor_irai_tenshu_main` file66298/418 provides the cancellation route. With
GSW1420 nonzero, original menu choice1 prompts `irai_09` and Yes/No `irai_01`.
Yes reads coins; at least30 prints `irai_10`, subtracts30 coins, retains the
old quest in LW11, calls `irai_cancel_func(GSW1420)`, sets GSW1420=0, then runs
the old quest's indexed end event. Too few coins prints `irai_11`; declining
prints `irai_12`. Choice0 appends indexed current-task description; choice2
appends `irai_04`. Original key locks, message continuation/menu-end callbacks
and common lecture/completed-all-tasks branches remain required separate owners.
Quest2/13 end-event table entries both point to RETURN/END null,file65ea8/8.

## Cancellation clears progress and removes every task item

`irai_cancel_func` file1097c/cc resolves argument0, subtracts1 for table index,
then clears the same17 flags and six bytes as start. Read the retained pointer
from irai_item_table,file65d5c +4*(quest-1). Iterate signed32 IDs until-1.
For each ID, repeatedly call pouchCheckItem; while count>0 call pouchRemoveItem,
then recheck. This removes **all copies**, including copies behind zero gaps.
It calls the low-level removal directly, with no Mario/party revision wrapper,
no per-item event-result destination and no count cached across removals.
No bounds check guards the original quest index or task-table pointer here;
native admission must establish its finite original table domain separately.

Relocated item-table bindings:

| Quest | Pointer slot | Source signed32 table |
| --- | --- | --- |
| 2 | gor file65d60 | file65d18:95,-1 |
| 13 | gor file65d8c | file65d20:107,-1 |

The cancellation callback's HA/LO relocations at file109f2/109fa both resolve
self module9 section5 addend45cd4, establishing irai_item_table identity; an
unrelocated zero immediate is not a null table.

## Quest2 acquisition and reward belong to Goomba3

Original gor_01.o `kuribo3_talk` file32f7c/240:

When GSW1420==2:

- If GSF5406==0 and GSF5405==0, print `(0,irai_02_00,0,me)`, then
  `evt_item_entry(irai_item,95,0,-1000,0,16,-1,0)`,
  `evt_item_flag_on(irai_item,2)`, `evt_item_get_item(irai_item)`, then
  set GSF5405=1. Item-entry/get-item services and actor placement are distinct
  dependencies; direct native inventory insertion is not this complete graph.
- If GSF5406==0 but5405!=0, print `(0,irai_02_01,0,me)`.
- Otherwise print `(0,irai_02_02,0,me)`, call `evt_sub_get_coin(20)`, set
  GSW1420=0, set GSF5376=1, print `(0,sys_request_finish,0,0)`.

Return after that quest arm. Only when quest!=2 does the original ordinary
story switch choose gor_01_028_00..08. This acquisition/reward NPC is distinct
from east Goomba4: sharing c_kuribo_n art does not transfer its event ownership.
The documented east hand-in's5406 write is therefore an intermediate state;
it does not itself grant coins, mark5376 complete or clear the active quest.

## Quest13 acquisition and reward span Glitzville

`tou_06_init_evt` file32a08/880 tests GSW1420==13 at file3314c and runs child
`iri_13_init` file326d8/1f0. That child returns immediately when GSF5406==1.
Otherwise check count107 into LW0, set LW1=20 then subtract count. Allocate
position-table work through `make_tbl`; loop until LW1==0, obtaining position
and indexed name through `get_pos` and `iri_13_make_name`. Enter original
item107 with that name/position, flags16, secondary-1. Entry index0 receives
callback `iri_13_pants_00_evt` only while GSF5408==0; other entries have no
callback. Each entry enables map-object gor_hae flag1. Increment index and
decrement remaining count, then free position-table work. If any entries were
created, launch the original randomized3000..3999ms looping fly sound worker.
Exact position/name/table generator and item pickup callback bodies remain open;
this is an acquisition caller graph, not a faithful native item-spawn implementation.
The loop's raw source count arithmetic has no introduced min/max clamp.

`tou_05.o talk_kinoshikowa` file2e31c/608, GSW1420==13:

1. Check count107 into LW0.
2. When5406==0: if count==20, print `irai_13_03_b` first time5405==0,
   set5405=1, otherwise print `irai_13_04`; then set5406=1. If count!=20,
   print `irai_13_00` and set5405=1 first time, otherwise `irai_13_02`.
3. When5406!=0: if count==0, print `irai_13_03`, grant30 coins through
   evt_sub_get_coin, set own NPC animation M_1, wait200ms, repeat M_1/wait200,
   print `irai_13_05`, save playtime into GSW1437, setGSW1420=0,
   setGSF5387=1, print sys_request_finish. If count!=0 print irai_13_04.
4. Return. All these prints use flags0/callback0/numeric speaker0, which in an
   ordinary type6 talk event still resolves that NPC owner.

Thus Goomba4's quest13 gate5406 indicates the Glitzville quantity checkpoint;
the east hand-in removes the107 copies, but completion/reward belongs to the
separate original Glitzville actor after the source count becomes0.5407 is
Goomba4's first/continuing hand-in latch, not overall task completion.

## Evidence and remaining integration

Original gor.rel SHA256d4965f21254ecf1a78e5d66ea12a0b000eed3f123032af6ab3cb0f703d3f185a.
Unmodified callback-span hashes: start34c62f3d0b29037ee7e030e70f47704bbb0737d29fd60a3f4cf8231c567fa3b7;
cancelc4357c4391d7a246f4c4708be3332c2c3c01d63b04420a0234ad4db789e53996.
Fresh rel_event_inventory reports pin gor main/tenshu and Goomba3 spans;
raw relocation-stream decoding resolves REL24 imported calls and HA/LO table
identity for the C callbacks. Reports, inspection ELF/disassembly and full
raw-operand census remain /tmp/quest-* outside source history.

Open: full Trouble Center menu filtering/input/draw/C callback state ownership,
source object/NPC lifetime/installers, full item-entry/get-item pickup/reward
services, quest13 position-table and pickup callbacks, GSW1437 playtime and
other task switch banks, matching original execution and native area/hardware
validation. Existing native checkpoint v8 preserves selected known switches
and full pouch but is not the original card format or a complete task-system
save. No shared implementation,index,PROGRESS,emulator profile or save was edited.
