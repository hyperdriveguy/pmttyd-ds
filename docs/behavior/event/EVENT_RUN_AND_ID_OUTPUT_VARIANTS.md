# Event run and ID-output variants: complete helper contracts

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Complete evt_run_evt800336e8/104,evt_run_evt_id800335c8/120,
evt_inline_evt_id800332f0/17c,evt_brother_evt_id800331ac/b0.
Read [root](EVENT_ROOT_CONSTRUCTORS_AND_APPEND.md),
[descendants/plain variants](EVENT_CHILD_BROTHER_AND_INLINE_CONSTRUCTION.md),
[manager](EVENT_MANAGER_COMPLETE_LOOP.md) and
[controls](EVENT_CONTROL_OPCODES_AND_ID_SCANS.md).
Verify: `python3 tools/verify_event_run_and_controls.py`.
No original execution trace claim.

## Independent RUN_EVT92 and RUN_EVT_ID93

Both resolve script operand0 through evtGetValue,then request
EntryType(script,U8(owner+b),flags0,U8(owner+c)). The resulting root has active
flags1,all ownership links0,global speed and credit0. Thus even during an active
manager pass,it DOES NOT append (root append requires0x20). It is available at
the next table rebuild;there is no immediate evtmgrCmd call or parent wait.

After constructor/run-check return,copy owner.word160,then170,then16 local
work words9c..d8 and three flag wordsdc/e0/e4 in ascending order,then user
pointers154 and158. All other root fields retain constructor state,including
message metadata178..190 and150 as0. No copy of source credit,speed,lifetime,
callback,stacks,timers or ownership links. Shared user pointers and copied local
words remain distinct storage policies. Priority/type arguments were captured
before construction;copied fields are reread after it.

RUN_EVT returns handler2. RUN_EVT_ID captures RAW output operand1 AFTER
script resolution,but BEFORE construction. Only after all field copies read
new.word15c and evtSetValue(owner,rawOutput,ID),then return2. No sentinel/null
skip or output resolution. If the output aliases an owner local word,the new
local retains the value copied before ID publication;only owner output changes.
These helpers are independent roots,not waiting ChildEntry semantics.

## INLINE_EVT_ID108

Capture raw output operand0 before scanning. Start script=owner.next14. Scan
command words by signed high16 counts until FIRST opcode109;advance owner.next14
to after that end. No nesting counter/missing-end/invalid-script guard. Then
call EntryType(start,owner unsigned priority,0x60,owner unsigned type),as plain
inline107. Flags become0x61 and permit same-pass root append when manager-active.

After constructor/run-check copy fields in EXACT plain107 order:160,154,158,170,
metadata17c,178,180,paired184/188,18c,190,local work9c..d8,flagsdc/e0/e4.
Do not copy150. Read new15c and write it through evtSetValue to the captured raw
output,then handler2. The output is published after local copies. Relationships
remain root zeros:owner deletion does not attach/cancel this entry. End109
uses terminal deletion handler255 through the existing dispatcher contract.

## BROTHER_EVT_ID111 and the difference from plain110

Capture raw output operand0 before scanning owner's next14 to FIRST112,using
signed-count advance,no nesting. Store owner.next14 after that end,then request
BrotherEntry(owner,start,0x60). Constructor copies locals/flags,user pointers,
actor/type/170 and installs new74=owner,6c/70=0; it appends whenever manager-active.

After constructor/run-check reread/copy owner160,type bytec,154,158,170,
then read new15c and evtSetValue(owner,rawOutput,ID),handler2.
Unlike plain BROTHER_EVT110,this wrapper DOES NOT copy owner150. BrotherEntry
bulk-clear leaves new150=0. Calling plain110 and adding ID output would therefore
change this variant's state. Message metadata178..190 also remains clear0 in
both brother variants. Output-local alias affects owner after the constructor's
local copy. There is no local allocation failure or ignored-output sentinel.

The attached brother ends with112 deletion and no local copy-back (new6c=0),
while the inline remains independent. Full deletion/control services and original
traces remain separate dependencies. ID publication alone does not prove actual
same-pass execution,liveness at a later check or save persistence.

Acceptance cases:root flags0 versus inline/brother0x60 append policy;run copied
locals without message metadata;ID capture/read/write order and local alias;
inline/brother first-end scanner;ID-brother150=0 versus plain110 copy;independent
versus attached ownership;constructor run-check before post-return copies.
These close92/93/108/111 helpers,not all opcode/variable namespaces or runtime.
