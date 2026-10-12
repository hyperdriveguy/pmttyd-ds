# Child,brother and inline construction: full inherited/reset state

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Complete evtChildEntry80036eec/334,evtBrotherEntry80036c04/2e8,
evt_brother_evt8003325c/94 and evt_inline_evt8003346c/15c.
Read [root construction](EVENT_ROOT_CONSTRUCTORS_AND_APPEND.md),
[label/run-check](EVENT_LABEL_SCAN_AND_ENTRY_RUN_CHECK.md),
[manager](EVENT_MANAGER_COMPLETE_LOOP.md),[lifetime](EVENT_RUNTIME.md) and
[restart](EVENT_RESTART_PRESERVATION.md). Verify:
`python3 tools/verify_event_descendant_constructors.py`.
Static local contracts;original execution traces remain open.

## Allocation and ownership links

ChildEntry(parent,script,flags) and BrotherEntry(owner,script,flags) capture GP,
select field/battle context and scan ascending slots for first inactive bit1,
exactly as root constructors. There is no capacity-exhaustion/null result path.
Increment allocation counter8041e860 before clearing new entry's1b0 bytes.

CHILD ONLY: before that memset,store new pointer to parent.word70,read parent
flags8,OR0x10,store byte8. Existing parent.child is overwritten without deleting
or testing it. After clear set child.word6c=parent,word70=0,word74=0.
BROTHER: no owner writes;after clear set word6c=0,word74=owner,word70=0.
Pointers are raw entry pointers,not event IDs. No parent/owner null/active test.
Valid free capacity and relationship domain are preconditions. If allocation
aliases invalid source storage,the original ordered writes do not supply rollback.

## Complete post-clear sequence and inheritance

Both initialize the following in order,with differences shown:

| Group / destination | Child | Brother |
| --- | --- | --- |
| flags byte8 | low8(supplied flags OR1) | Same |
| next14,restart1a0,current1a8 | Supplied script,each in that order | Same |
| opcode bytea | 0 | 0 |
| relationship links | 6c=parent,70=0,74=0 | 6c=0,74=owner,70=0 |
| priority byteb | low8(U8(parent+b)+1) | U8(owner+b) |
| globalID / entry15c | Increment global evtID;store PRE-increment ID | Same |
| owner word160 | Copy parent160 | Copy owner160 |
| stack index bytese/f | Both0xff | Same |
| type bytec | Copy parent type byte | Copy owner type byte |
| word1a4 | 0 | 0 |
| user pointers154,158 | Copy parent154,then158 | Copy owner154,then158 |
| speed164 / credit168 | Current GLOBAL evtSpd,then binary32+0.0 | Same |
| word16c | -1 | -1 |
| word170 | Copy parent170 | Copy owner170 |
| lifetime low4,thenhigh0 | Both0 | Same |
| metadata before locals | Copy source17c,178,180,paired184/188,18c,190 | No copy;remain clear0 |
| local work9c..d8 | Copy16 source words ascending | Same |
| local flagsdc,e0,e4 | Copy three source words ascending | Same |
| label bytes1c..2b / pointers2c..68 | Initialize0xff/0 in two8-record iterations | Same |

All unlisted destination bytes remain memset0,including callback98,argument
pointer18,argument count9,first-call markerd,timers78..80 and retained150.
Copied local values are WORDS,including encoded floats;no resolution/coercion.
User pointers154/158 are shared pointers,while locals/flags are value copies.
Speed is not inherited from source164;credit is not copied from source168.
Child priority255 wraps to0;Brother retains255. Source flags8 are not copied:
new stopped/active/inhibition bits come from supplied flags and later run-check.
Parent/owner lifetime,timer,callback,stack and opcode do not become descendants'.

Then scan labels from new.next14 using the complete root scanner:read first
operand unconditionally,signed high16 count advance,terminal opcode1 only,
low8 label IDs,after-label pointers,no effective16-label bound. Child/brother
have distinct embedded copies of that same algorithm,all fingerprinted here.

## Live append and run-check order

After labels,read manager-active8041e858. For BOTH constructors,nonzero alone
appends chosen slot at803d8930[count],reread new ID15c into803d8530[count],then
store count+1 to8041e85c. No flags0x20 gate,no sort,no deduplication or size test.
Outside a manager pass,neither appends. This differs from root/typed constructors.

Child then calls evtEntryRunCheck(new),resets global evtID to1 if currently0,
and returns new. Brother first performs that zero-ID reset,THEN calls run-check
and returns new. Preserve order if adapters mutate globals. Run-check applies
system-level masks to all current-context events,not just the new argument.
A newly appended event can be stopped before its scheduled visit. Construction
does not dispatch it immediately;the live-count manager supplies later visits.

## Attached brother opcode110 versus independent inline107

The complete opcode110 helper starts script at owner's next14,scans to FIRST
opcode112 with signed-count word advances (no nesting count),stores owner.next14
to after that end marker,then calls BrotherEntry(owner,start,0x60). After return
it REREADS and copies owner fields160,typec,154,158,170,150 to new in that order.
Some copies repeat constructor writes AFTER append/run-check;150 is new here.
It returns handler2. Constructor already copied locals/flags;the wrapper does
not copy message metadata178..190. End112 deletes that entry (handler0);because
6c=0,it does not copy local changes back to the owner on deletion.

The complete opcode107 helper likewise scans next14 to FIRST109,no nesting,
stores owner.next14 after109,then calls EntryType(start,U8(owner+b),0x60,
U8(owner+c)). Root construction runs label scan/conditional append/run-check
BEFORE the wrapper's inherited writes. Root flags become0x61,which include0x20,
so it appends when manager-active. Post-return copy owner160,154,158,170,
metadata17c,178,180,paired184/188,18c,190,then locals9c..d8 and flagsdc..e4.
No copy150 here. Return handler2. Root constructor relationships remain all0,
so this inline is independent of owner deletion rather than an attached brother.
End109 deletes it with terminal handler255. Both wrapper scan paths have no
missing-end/invalid stream fallback or allocation null check.

This closes these exact107/110 paths;ID-output opcode variants108/111 and
other event control wrappers require separate contracts. Child-call opcode94
and deletion copy-back remain linked existing ownership/request evidence,
not a newly complete interpreter specification here.

Acceptance cases: dirty free slot/clear,all copied/reset fields,global speed
versus parent speed,child255→0/brother255,shared pointers versus local copies;
parent wait-link before clear,existing link overwrite;child/brother unconditional
active-manager append versus root flag gate;zero-ID/run-check order;post-run-
check wrapper recopy;brother150 versus inline metadata;owner cancellation and
no local copy-back from brother/inline. Full deletion/control opcode closure,
capacity/domain/source initialization and original traces remain EVT-01 work.
