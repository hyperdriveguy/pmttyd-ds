# Original Trouble Center board selection and acceptance

US G8ME01 disc0 revision0; read-only recovery2026-10-14 for
[OW-INTERACT-01](../../requests/OW-INTERACT-01.md) and
[ITEM-01](../../requests/ITEM-01.md). Original rel/gor.rel static event,
complete selection/init/check/end callback bodies and relocation evidence.
Addresses and offsets hexadecimal; quest IDs, amounts and states decimal.
No original executable is linked or run. This closes finite producer/control
semantics; native menu drawing, map case lifetime and original observations
remain distinct requirements.

## Correction to earlier acceptance description

The earlier [writer contract](TROUBLE_CENTER_QUEST2_13_WRITERS_AND_ACQUISITION.md)
incorrectly describes LW11 as a zero-based index and opcode115 as an increment.
LW11 is the **one-based quest ID** published from the menu's original list.
The event subtracts1 into temporary LW0 for pointer-table access, preserving
LW11. At file66248 it sets GSW1420=LW11. The following opcode115 is the
[debug-register instruction](../event/EVENT_DISPATCH_TAIL_AND_DEBUG_FORMATTING.md), not an
increment. Accepting original quest2 therefore writes1420=2, never3.

## Actual map installer and first lecture

`gor_02_init_evt` file4d554/87c requests `gor_irai_init` through opcode92 at
file4dc40. This request follows the preceding closed story switch; there is
no adjacent Trouble Center quest/completion guard around this installer.
`gor_irai_init` file65fb8/48 registers, in order:

- `evt_run_case_evt(9,1,A_i04_keizi,0,gor_irai_main,0)`.
- `evt_run_case_evt(9,1,A_i04_kunter,0,gor_irai_tenshu_main,0)`.

These are authored map collision objects for board/counter interaction,
not an NPC talk event or the plaza Goomba3. Cases are installed in original
map gor_02; a native interior mask or invented NPC placement does not follow
from this registration. Full case9 admission/raycast/key/object visibility
and door ownership must be preserved by their respective providers.

In original `m/gor_02/d`, collision hierarchy paths are:
`world_root/A/A_house/A_ie_04/A_ie_04C/A_i04_kmnC/A_i04_keiziban/A_i04_keizi`
and `.../A_i04_asen/A_i04_kunter`. Nodes at file12d538 and12d400 each have
one mesh part. Composed static authored bbox midpoints, in **map asset units**,
are approximately(24.12630917,8.40001160,-9.13685310) for board and
(16.00000014,7.99833102,-8.13685076) for counter. These are transform inspection
results, not native interaction anchors or source runtime hitObjGetPos proof.
Authored map-to-engine scaling, animation, collision and cutaway/door state
remain required. Do not assign the professor's existing native interior mask.

`gor_irai_main` file660a0/1f8 first tests GSF5374==0. On that arm it launches
three inline requests: Mario direction(270,200,0), party0 direction(270,200),
party1 direction(270,200). Then opcode94 calls `gor_irai_lecture` and returns.
Thus first successful lecture does not immediately open the list; board must
be interacted with again.

Lecture file66000/a0: KeyOff; `evt_msg_toge(2,150,12,-156)`;
`evt_msg_print(0,irai_00,0,0)`; `evt_msg_select(0,irai_01)`.
LW0==0 appends `evt_msg_print_add(0,irai_03)` then sets5374=1.
Otherwise append `(0,irai_02)`, preserving5374. KeyOn; return/end.
These are original IDs/operand order, not permission to rewrite US dialogue.

## Init callback: ordered original list, no completion filtering

`irai_init_func` file106cc/2b0 allocates416c bytes through mapalloc_base_ptr,
zeroes the entire work, sets state0 and reads global story `swByteGet(0)`.
Work+8 holds30 signed32 one-based quest IDs. Fixed slots are populated by
these signed story comparisons, in this actual order:

| Story | Slots, zero-based | Quest IDs written |
| --- | --- | --- |
| All | 0..5 | 1,2,3,4,5,6 |
| >=56 | 6..8 | 7,8,9 |
| >=112 | 9..11 | 10,11,12 |
| >=164 | 12..14 | 13,14,15 |
| >=215 | 15..17 | 16,17,18 |
| >=261 | 18..19 | 20,21 |
| >=281 | 20 | 19 |
| >=339 | 21..23 | 22,23,24 |
| >=373 | 24..26 | 25,26,27 |
| >=401 | 27..29 | 28,29,30 |

Then count nonzero entries across all30 into work+4. **No compaction, sorting
or removal of completed tasks occurs.** Quest19 follows20/21 after its later
story unlock. In particular story8 includes quest2 at cursor1.
Two initial loops over encoded5405..5421 and1469..1474 contain only bdnz;
there are NO swClear/swByteSet calls. This init must not reset progress.
Actual acceptance/cancellation reset callbacks are separate.

For every original quest1..30, msgSearch/client-table and strcpy cache the
client at work+9c+21*(ID-1); then contents at work+312+31*(ID-1).
Source copy calls provide no explicit destination length check. Work fields:
state+0, count+4, list+8, cursor+80, previous+84, top+88, iconX+8c,
iconY+90, counter field+94, smooth scroll float+98. Init sets cursor,
previous,top,counter,scroll to0 and publishes work pointer to eventLW10(+c4),
returns2. Allocation failure, US string bounds and complete draw behavior
remain providers; do not confuse original requested416c with a measured size.

## Main callback states, original input and UI requests

`irai_main_func` fileffa8/724 reads eventLW10 work. State0 requests, in order:
Entry2D(iraiYubi,504),FlagOn(Yubi,4); Entry2D(iraiArrowUp,524),FlagOn(Up,8);
Entry2D(iraiArrowDown,524); Entry2D(iraiLButton,134),SetScale(L,.8);
Entry2D(iraiRButton,136),SetScale(R,.8). Then FlagOn(mask2) for
Yubi,Up,Down,L,R in that order. Set state1, SFX42. No state1 input this call.
Register `dispEntry(8,0,irai_window_disp,work,300)` and return0.

State1 checks A trigger100 first. Read swByteGet1420; only when0 read
swGet(5374+list[cursor]). If that is0, SFX18, state100 and normal display/return0
without navigation tail. Otherwise continue remaining input. Thus completed
quests remain listed but cannot be selected, and an active quest blocks A.
Quest2 checks source5376. Neither item counts nor5405/5406 authorize acceptance.
B trigger200 next: SFX19,state101, normal display/return0 without navigation.

Navigation operates sequentially on live cursor/top; direction masks are
2000 down and1000 up, button masks40 L and20 R. SFX5 accompanies each admitted
change. Capture old cursor before navigation:

- Down repeat increments when cursor<count-1. At last row only fresh down
  direction trigger permits increment; a repeated hold alone does not wrap.
- Up repeat decrements when cursor>0. At first row only fresh up direction
  trigger permits decrement.
- If cursor>=count: cursor=0,top=0. If cursor<0: cursor=count-1 and set
  top=count-10 only when count-10>0.
- If newcursor>old and cursor-top>7 and top<count-10, increment top.
  If newcursor<old and cursor-top<3 and top>0, decrement top.
- If top>0 and L trigger, subtract min(top,10) from both top and cursor.
  Then if top<count-10 and R trigger, add min(count-10-top,10) to both.

If cursor differs from work.previous, set counter94=0; set previous=cursor.
Update smooth scroll98 with single operations: delta=float(top*30)-scroll;
scroll=scroll+delta/6. Set iconX=-129,iconY=120-(cursor-top)*30.
SetPos(Yubi,iconX,iconY,0),FlagOff(Yubi,2), then positions
Up(231,178,0),Down(231,-190,0),L(261,146,0),R(261,-190,0).
If top==0 FlagOn(Up,2),FlagOn(L,2); otherwise FlagOff in same order.
If top>=count-10 FlagOn(Down,2),FlagOn(R,2); otherwise FlagOff same order.
Register same dispEntry and return0. Main does not increment counter94;
the complete original display does not use it; no increment owner is established.

State100 executes on the next callback invocation: publish list[cursor] to
LW0(+9c), state0, delete Yubi,Up,Down,L,R in order, SFX43,return2.
State101 similarly publishesLW0=-1,state0, same deletes/SFX43,return2.
These exit visits do not register display. Other work states only register
the common display and return0; the source callback does not add validation,
firstCall reset, cursor clamp or native resource-failure handling.

## Check callback and blocking acceptance event

`irai_check_func` fileff60/48: ifLW0!=-1, publish kakunin[LW0-1] toLW12(+cc),
copy LW0 toLW11(+c8); otherwise LW12=0,LW11=-1. Return2. Therefore LW11 is
an original **quest ID**, while cursor is a different zero-based list position.
For quest2, original tables at65ab8/65b30/65ba8/65c20 bind client_02,
contents_02,help_02,irai_44. For quest13 bind client_13,contents_13,help_13,
irai_55. No story prose is copied into this contract.

Normal gor_irai_main path after lecture flag5374: KeyOff, init, label10,
blocking main callback, check. LW0==-1 goes to label100 cleanup. Otherwise
set read pointer exposition table, LW0=LW11, add-1, read pointer intoLW1;
`toge(1,0,0,0)`,print(0,LW1,0,0); same toge and print(0,LW12,0,0);
select(0,irai_01).

Yes LW0==0: append(0,irai_73),call irai_start_func; read
start_event[LW11-1] intoLW1, execute opcode92 RUN_EVT(LW1),then SET1420=LW11,
debug115(1420). This RUN_EVT is **not** the blocking opcode94 child call.
Quest2 and13 both resolve start_event_null65dd4/8(return/end), so no missing
child work can justify granting inventory directly.

Other selection result: msg_continue, goto10, which re-enters state0 menu
with preserved list/cursor/work rather than reallocating or resetting switches.
Label100 calls irai_end_func(fileff2c/34), mapFree(mapalloc_base_ptr,eventLW10),
KeyOn,return/end. End callback does not reset quest/flags or zero freed pointer.
Full source render/menu/error/interruption and case ownership are unclosed.

## Evidence and scope

REL SHA256 d4965f21254ecf1a78e5d66ea12a0b000eed3f123032af6ab3cb0f703d3f185a.
Fresh rel_event_inventory report `/tmp/acceptance-events.json` records event
bindings and spans. Complete C-body hashes:
init20905c95c1c3171f69b88cafd938bd97da3f154e68712aeca2960406168e073f;
maina453b07cd172c53d9d5947a8ce40bdf6ba059275b3c57ed733c7f6e171bfb6b0;
checkce2970ef448df7ef1ce858d9d165470227925cfc174c7a23af7ded6cbe99e9cc;
end799bc3770592f2447535e68c50607af1db0cd3327e509ebcea0937a18f47d87a.
Original m/gor_02/d ISOoffset991830984,size1513698,SHA256
 d738bff28f92ed2a2d0658fb836b0a6e91b915a32553897b47032febe2f62353.
REL24 and ADDR16_HA/LO decoding pins calls, constants and table identities;
unrelocated zero immediates are not null pointers. Inspection ELF/disassembly,
raw module and event reports remain temporary/ignored, no payload committed.

Remaining: irai_window_disp full frame/text/completion styling/counter owner;
case9 interaction geometry/door/cutaway/lifetime; full original global switch
banks and cache-string copy services; original US message/menu resources;
matched original execution and native hardware/audio/performance validation.
This delivery changes only this source contract, not native code or shared
indexes/progress/inbox files. Existing selected-switch native checkpoint does
not become the complete original Trouble Center save system.
