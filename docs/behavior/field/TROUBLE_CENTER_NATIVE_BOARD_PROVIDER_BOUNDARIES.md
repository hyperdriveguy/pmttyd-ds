# Trouble Center board provider boundaries and all30 start children

US G8ME01 disc0 revision0; read-only source recovery2026-10-14. Complements
[board producer](TROUBLE_CENTER_BOARD_SELECTION_AND_ACCEPTANCE.md),
[door/case9](TROUBLE_CENTER_DOOR_CASE9_GEOMETRY_AND_LIFETIME.md), and
[quest reset/cancellation](TROUBLE_CENTER_QUEST2_13_WRITERS_AND_ACQUISITION.md).
This is a source contract, not a claim that the native board is implemented.

## Retained print and separate selection ownership

The existing [Continue/Add contract](../message/SAVE_EVENT_RETAINED_WINDOW_CONTINUE_AND_APPEND.md)
and [selection contract](../message/MESSAGE_SELECTION.md) apply directly.
Original print stores the window index at event+178; selection stores its
independent window at event+17c. A retained print stop clears pollbit2 while
keeping allocationbit1 and the window/data alive. Thus completion of print,
Add, or Continue is the original window poll condition, not deletion or text
visibility. Selection publishes its signed result into LW0 after its closing
poll condition, then deletes its own window. Do not replace the print with the
selection or carry the input that confirmed the selection into a new key wait.

`gor_irai_lecture` file66000/a0 requests KeyOff, toge(2,150,12,-156),
print(0,irai_00,0,0), then select(0,irai_01). Choice0 appends(0,irai_03),
awaits that poll, then setsGSF5374=1. Otherwise append(0,irai_02), preserving
5374. KeyOn follows append completion. `gor_irai_main` first-lecture branch
launches three inline direction providers before its blocking opcode94 lecture
child: Mario(270,200,0), party0(270,200), party1(270,200). After that child
it returns; this contact does not fall through to the task list.

The exact US `irai_00` has wait250, key/page controls, and a terminal `<o>`.
`irai_01` starts `<select 0 1 0 40>`: initial choice0, B cancellation result1,
auto width, Y40. `irai_02` starts `<p>`, contains wait250 and ends `<k>`;
`irai_03` starts `<p>` and ends `<k>`, with all original intermediate keys,
pages and wait250 controls retained. Append reparses/extends the retained
print timeline; it must not open a replacement print window.

Normal board path file66110..66294: KeyOff, init, label10, blocking main,
check. Cancellation LW0=-1 skips to label100/end/KeyOn. A selected quest uses
exposition[Q-1], toge(1,0,0,0), print(0,exposition,0,0), then the same toge
and print(0,LW12,0,0), then select(0,irai_01). Both confirmation messages
forQ2/Q13 end `<o>`; exposition help02/help13 starts `<diary>`, selecting
window type7. A native original type7 display provider is therefore required;
supporting the parser token alone does not establish source presentation.

Yes appends(0,irai_73), awaits completion, resets progress through
irai_start_func, reads start_event[Q-1], launches opcode92 RUN_EVT, then sets
GSW1420=Q and requests debug115(1420). This is a nonblocking child request,
not opcode94; the parent does not await child completion before1420. `irai_73`
starts `<p>` and ends `<k>` before a final newline. Other result calls
evt_msg_continue and awaits the retained print's poll, then goes to label10
with the same menu work/list/cursor. Continue does not force-close or release
a retained key wait. End cleanup occurs only at label100.

Current native MessagePlayback has `pending()` for pollbit2, `append()` for
retained accumulated text, and `resume()` for Continue's first-call mutation.
The event bridge must latch first-call actions and poll rather than append or
resume every simulation tick. MessageSelection retains its independent
message and reaches state4 after closing; its result must survive until read.
These interfaces are implementation observations, not original timing proof.

## All30 table bindings and source child effects

Actual local ADDR32 relocations establish these quest-order formulas:

| Table | File span | QuestQ=1..30 binding |
| --- | --- | --- |
| irai_msg_exposition | 65ba8/78 | irai_help_Q, two-digit suffix; strings1eca8+10hex*(Q-1) |
| irai_msg_kakunin | 65c20/78 | irai_(Q+42), two-digit suffix; strings1ee88+8*(Q-1) |
| irai_start_event | 65e30/78 | Listed below; pointer slot65e30+4*(Q-1) |

Q1 binds irai_start_event01 at65ddc/34. Q4 binds irai_start_event04 at65e10/20.
Every other Q (2,3,5..30) binds irai_start_event_null at65dd4/8, exactly
RETURN then END. There are28 null entries, not onlyQ2/Q13.

Q1 child requests evt_npc_set_position(盗賊団２,445,0,120), then
evt_door_set_param(GW11,0,door_data_05_npc_01), then returns. The first
door argument rawfd050f8b decodes to GW11 under the
[original integer namespaces](../event/EVENT_INTEGER_OPERAND_NAMESPACES.md).
Q4 child requests evt_npc_set_position(ボロドー２,-475,0,-180), then returns.
These need actual source actor/door providers; a null substitute forQ1/Q4
changes behavior. The finite caller may require readiness for the selected
child rather than admitting an unsupported child and dropping its requests.

## Counter interaction is a separate source event

The installer separately binds A_i04_kunter to gor_irai_tenshu_main
file66298/418. It is not board interaction and not the door NPC's talk event.
First it sumsGSF5375..5404. Sum30 takes KeyOff, the same fixed toge,
print(0,irai_74,0,0), KeyOn, return. Otherwise5374==0 calls the blocking
lecture child and returns. Remaining path KeyOff: activeQuest0 printsirai_06;
nonzero printsirai_07, selectsirai_08. That menu has
`<select 0 -2 0 60>` and three choices; B publishes-1.

Choice-1 resumes the retained print before the debug notice. Choice0 appends
irai_msg_hikiukechu[activeQuest-1]. Choice1 appendsirai_09, then selectsirai_01:
Yes reads coins; >=30 appendsirai_10, subtracts30, saves oldquestLW11,
callscancel(activeQuest), writes1420=0, then nonblocking RUN_EVT of indexed
end_event. Fewer coins appendsirai_11; No appendsirai_12. Choice1 finally
setsLW0=1. Choice2 appendsirai_04. KeyOn follows the selected path. Cancellation
inventory/reset/end-child services remain separate from board acceptance.

## Evidence

Fresh local event inventory: artifacts/trouble-board-provider-events.json.
REL SHA256d4965f21254ecf1a78e5d66ea12a0b000eed3f123032af6ab3cb0f703d3f185a.
Main49commands SHA2026305ead23f8bfe55a2cc8278b1e311f68817f4be31f99817d844c243c8b55;
lecture13commands SHAa9f8188d957403358fee2c3406327e37ae580df76e80dd0c487c8c35d2261601;
counter92commands SHA9943f53e28869dc9e368cfde01d9359fcb1e81d8d734dcd14b7d1a7d9e40c596.
Exact US bytes independently reread from ISOmsg/US/gor_02.txt,
offset1177698108,size128972,
SHA1958e3d22bb27b01332e08b67f279a33f21e0e8a3aba2b947ad8c2a446144f16;
current ignored romfs/messages/gor_02.txt matches exactly. No dialogue prose,
assets, code, shared indexes or progress files changed for this recovery.
