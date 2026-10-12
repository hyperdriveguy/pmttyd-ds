# Complete event deletion: descendants,parent copy-back and clear order

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Complete evtDelete80036700/1d0 and evtDeleteID80036664/9c.
Read [constructors](EVENT_CHILD_BROTHER_AND_INLINE_CONSTRUCTION.md),
[stop/start](EVENT_STOP_START_RELATIONSHIP_TRAVERSAL.md),
[manager](EVENT_MANAGER_COMPLETE_LOOP.md) and
[controls](EVENT_CONTROL_OPCODES_AND_ID_SCANS.md).
Verify: `python3 tools/verify_event_lifetime_services.py`.
Static local contracts;no original cancellation or runtime trace claim.

## evtDelete(E) complete sequence

Capture current context through GP.word14:field803d8d30 if0,battle803d8dd0
otherwise. Read E.flags8. If active bit1 is absent,return without ANY child scan,
copy-back,entry clear or allocation-counter decrement. No raw E-null guard.
For active E,perform in order:

1. Read waiting-child pointer E+70. If nonnull,call evtDelete(child) recursively.
   Child activity/descendants/context are checked by that invocation. Parent
   pointer may be changed by its child's deletion before later steps.
2. Read captured context.entryArray90. Scan slots ascending,entry stride1b0,
   with signed context.capacity0 reread for each loop condition. For each
   currently active entry B,read B.owner74. If pointer equals E,evtDelete(B).
   Outer array pointer stays captured;each recursive deletion captures context
   independently. No event-ID,priority,type,stopped/inhibition filter here.
3. AFTER those recursive deletions,reload parent pointer E+6c. If nonnull,
   read parent.flags8,clear bit0x10 only,store byte8;store parent.child70=0.
   No parent activity,ID or parent.child==E validation. Then copy back fields
   in the exact order below,even on ancestor cancellation rather than normal
   successful child completion.
4. Read E.flags8,clear active bit1 only and store;memset(E,0,0x1b0).
5. Read global allocation counter8041e860,subtract1 with word wrap,store.
   Return has no specified value. No script callback,heap free,display/audio
   stop,pouch save or action-command cancellation occurs in this local body.

Parent copy-back is individual WORD reads/writes in this order:

| Source and same-offset destination | Sequence |
| --- | --- |
| 9c..d8 | Sixteen local work words,ascending |
| dc,e0,e4 | Three local flag words |
| 17c,178,180 | These metadata words,in that order |
| 184/188 | Read both,then store184,then188 |
| 18c,190 | Two final metadata words |

Shared user pointers154/158,owner160,event ID15c,priority/type,speed/credit,
callback,timers,stacks and parent relationships are NOT copied back. Bulk clear
later resets all child fields,including its ID. Clearing active before memset
is a distinct write;the count decrement follows clear. Inactive entries do not
normalize their stale bytes or count. Recursive cycles/null/invalid ownership
are not locally detected;valid relationship domain is required.

## Copy-back and cancellation implications

Child's deletion can copy its locals into E,then E's own parent copy-back uses
THOSE newly copied values. Attached brothers have6c=0,so their modified locals
do not copy back. Independent inline entries have74=0,so owner deletion's
brother scan does not discover them merely because they were created by E.
These are actual pointer-field rules,not event names or a host parent hierarchy.

The scan reads active/owner fields live. Recursive deletion of one brother can
remove descendants that occur later in slot order;later scans skip the newly
inactive entries. There is no precomputed descendant set or reverse-order
stack destruction. If a child pointer is nonnull but inactive,its invocation
returns without clearing E.child;E's later deletion still clears E wholesale.
No rollback of copied locals on cancellation is established.

Clearing parent's wait does not execute it immediately. Manager eligibility,
remaining schedule position,credits and stored/live event IDs control when it
can resume. A slot reused after deletion is still subject to the manager's ID
comparison;deletion does not edit or remove schedule-array entries/count.

## evtDeleteID(ID) complete scan

Capture current context and array once,scan ascending slots,checking active bit1
then exact word15c equality to supplied ID. Call evtDelete for EVERY matching
active entry;no break after first. Increment slot/pointer and reread signed
context.capacity0 for every test. No stopped/type/eligibility filter,no ID0/
negative special case and no ID-to-slot conversion. No match makes no deletion
request. The leaf has no specified scalar success result.

Called evtDelete can clear additional later entries;the outer loop's next active
read observes that. Context selection inside each deletion is independent of
DeleteID's captured context. Opcode95 ignores this service's return and returns
handler2 even if deletion changed its own storage;see linked control contract.

Acceptance cases:inactive no-op,child-before-brother order,ascending live scans,
parent copy-back after descendant mutations,paired metadata read/store order,
ancestor cancellation copy-back,brother no-copy-back,inline independent lifetime,
parent no-ID/link validation,flag-clear/memset/count order,duplicate-ID/live-match
scan and stale schedule identity. Full event opcodes/namespace/source lifecycle,
valid context/ownership/capacity and original execution traces remain EVT-01 work.
