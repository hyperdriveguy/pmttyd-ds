# Root event constructors: complete initialization and live-table append

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Complete evtEntry800374ac/28c and evtEntryType80037220/28c;read
[label/run-check policy](EVENT_LABEL_SCAN_AND_ENTRY_RUN_CHECK.md),
[complete manager](EVENT_MANAGER_COMPLETE_LOOP.md),
[system-level/speed ownership](EVENT_SYSTEM_LEVEL_AND_SPEED.md) and
[existing lifetime](EVENT_RUNTIME.md).
Verify: `python3 tools/verify_event_root_constructors.py`.
Static local behavior;original creation/deletion/pause traces remain open.

## ABI,context and allocation

Arguments: evtEntry(script,priority,flags);evtEntryType(script,priority,flags,type).
The bodies share the same algorithm,except evtEntry supplies type0xef and
EntryType stores low8 of its fourth argument. Priority and flags also narrow8.
There is no automatic inherited parent/type/priority/run-check policy here.

Capture current GP and choose context803d8d30 when GP.word14==0,otherwise
803d8dd0. Read signed context.word0 capacity and context.word90 entry-array.
Scan ascending slot indices from0,entry stride1b0,for first flags8 lacking1.
Other entry flags do not make an active entry available. Retain chosen slot and
pointer. There is NO exhausted-capacity return/null guard: if every slot is
active,the pointer ends one entry beyond the array and initialization still
proceeds;nonpositive capacity skips scanning and uses the array pointer/slot0.
Valid free capacity is a constructor precondition,not a recovered null fallback.

Increment shared allocation counter8041e860 BEFORE memset(entry,0,0x1b0).
This body uses no heap allocator and does not test memset's return. The clear
sets all unspecified fields to zero;subsequent explicit writes are below.
Both constructors return the initialized entry pointer unconditionally.
Neither executes its script or calls evtmgrCmd directly.

## Complete post-clear write sequence

In exact groups shown,write:

| Entry field | Value/order |
| --- | --- |
| byte8 | low8(flags OR1) |
| word14 | script (next command) |
| word1a0,then1a8 | script (restart/current-command retained addresses) |
| bytea | 0 (current opcode) |
| word6c,70,74 | 0 (waiting-parent,waiting-child,brother owner) |
| byteb | low8(priority) |
| global evtID804151e8 | Increment current ID word with wrap32 |
| word15c | PRE-INCREMENT global ID |
| word160 | -1 (owner/actor selector storage) |
| bytee,thenf | 0xff (loop/switch stack indices) |
| bytec | 0xef for evtEntry;low8(type) for EntryType |
| word1a4 | 0 |
| float164 | Current global evtSpd804151ec,read NOW |
| float168 | Binary32 +0.0 (constant8041fa34) |
| word16c | -1 |
| word170 | 0 |
| word4,then0 | 0 (64-bit lifetime low then high) |
| words9c..d8 | Sixteen local work words0 in ascending order |
| wordsdc,e0,e4 | Three local flag words0 |
| bytes1c..2b / words2c..68 | Sixteen label IDs0xff and pointers0;two8-record unrolled iterations |

Arg-count9,first-call markerd,argument pointer18,user-work154,user-flag158,
callback98,timer78..80 and all other bytes remain the bulk-clear zeros unless
subsequent label/run-check behavior changes them. Global speed is copied,not
an unconditional1.0,nor derived from priority/type. Credit begins0,so a
manager visit adds speed before deciding whether to dispatch. This determines
first-call opportunities under fractional/multiple-credit rates.

Next perform the complete label scan described in the companion document.
Only AFTER that scan read manager-active8041e858. If it is nonzero AND the
NOW-read entry flags8 has0x20,append chosen slot to schedule-index array
803d8930[count],then reread entryID15c and append it to ID array803d8530[count],
then store count+1 to global live schedule count8041e85c. Otherwise no append.
No priority sort,capacity validation,deduplication or constructor-immediate
execution. Even inactive/stopped flags can participate when the append gate
is met;the manager later checks actual flags and scheduled ID.

Call evtEntryRunCheck(entry),which applies current global system level to ALL
selected-context events via evtStopAll. Finally reread global evtID; if exact0,
store1. Return the captured entry pointer. Thus wrapping next ID from0xffffffff
becomes1 only after run-check. The current event retained its pre-increment ID;
entry with prior globalID0 can retain0. Do not implement reserved-zero avoidance
before assignment or a native generation policy as original byte behavior.

## Same-pass creation implications and acceptance

A root created outside a manager pass waits for the next priority-table rebuild.
Inside a pass,flags0x20 adds it at the live table end without sorting;flag0 does
not append even though the new entry is active. A later manager loop test reads
live count and can visit the appended entry;run-check might stop it first.
Priority255 does not move an appended entry ahead of existing entries this pass.
Both constructor flags and global manager-active state are required evidence.

Acceptance cases: ascending first-free selection,nonactive dirty slot clear;
flags/type/priority narrowing;global speed capture and initial credit0;actor-1,
stack/label sentinel initialization;label scan before append;both append gates,
slot/ID/count write order,unsorted current-table tail;ID wrap/reset ordering;
run-check affects existing events. Child/brother/inline constructors,restart,
capacity/domain policy,ID/global/context initialization and original traces remain
open. The complete root bodies do not prove those other ownership paths.
