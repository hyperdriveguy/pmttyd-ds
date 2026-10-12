# Event restart: complete local reset and preserved ownership

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Complete evtRestart80036abc/148. Read
[constructors](EVENT_CHILD_BROTHER_AND_INLINE_CONSTRUCTION.md),
[label/run-check](EVENT_LABEL_SCAN_AND_ENTRY_RUN_CHECK.md),
[manager](EVENT_MANAGER_COMPLETE_LOOP.md) and
[dispatcher continuations](EVENT_DISPATCH_AND_WAIT_CONTINUATIONS.md).
Verify: `python3 tools/verify_event_descendant_constructors.py`.
No original restart/pause execution trace claim.

## Exact stores,scan and return

Input raw entry E;there is no null/active/relationship guard and no bulk clear.
Read script=E.word1a0,then perform:

1. E.word14=script;E.word1a8=script;E.byte a=0.
2. E.float164=constant1.0;E.float168=constant+0.0.
3. E.bytes e and f=0xff (loop/switch stack indices).
4. Read current global evtSpd804151ec;store that to E.float164;
   store+0.0 AGAIN to E.float168. The earlier1.0 is an intermediate write,
   not the final speed policy.
5. E.lifetime low word4=0,then high word0=0.
6. Reinitialize sixteen label IDs at1c..2b to0xff and sixteen label pointers
   at2c..68 to0 in the same two8-record iterations used by constructors.
7. Scan script via next14 with unconditional first-operand reads,signed high16
   argument count advance,opcode1 terminal,opcode3 low8 ID/after-command target
   writes,no effective16-label cap. Same valid-script invariants apply.
8. Call evtEntryRunCheck(E),then return the SAME captured entry pointer.

The script/next field14 remains its initial restart address after label scanning;
the scan uses a separate pointer. No evtmgrCmd/script execution,current-table
append,priority sort,ID increment or allocation counter change occurs locally.
The all-event run-check can alter stopped flags through other services.

## Complete preservation boundary

Apart from the listed stores and called run-check effects,every entry field is
preserved. In particular:local work9c..d8,local flagsdc..e4,user pointers154/158,
actor160,event ID15c,priorityb,typec,ownership links6c/70/74,flags8,retained150,
message metadata178..190,1a0 restart address,1a4,16c/170,callback98,argument
pointer18/count9,markerd,timers78..80 and old loop/switch stack arrays beyond
their index bytes. Reinitializing label pointers does not clear those stacks.

Marker/callback/argument leftovers do not mean a restarted USER_FUNC keeps
polling:opcodea=0 causes the dispatcher to FETCH first,which clears markerd,
loads new arguments/count,next pointer and current-command address. Restart
itself leaves those bytes;the fetch supplies their later reset. Preserve those
two stages when exposing entry snapshots to host callbacks.

Restart does not delete an existing child/brother,detach ownership,clear a
parent waiting bit,resume all stopped entries,restore locals from a parent or
allocate a new event ID. A waiting-child flag0x10 remains unless another owner
changes it;manager eligibility can therefore postpone execution after restart.
An existing scheduled ID still matches because identity is retained. A restart
during a dispatcher callback resets credit0 and lifetime0;the manager's later
inner credit test observes current credit rather than automatically redispatching
or applying another lifetime delta. No implicit transaction/rollback is proven.

Acceptance cases:local/ownership/ID preservation,all reset stores and intermediate
speed1.0→globalSpeed;preserved old first-call state until fetch;waiting/stopped
entry remains inhibited;label rebuild without stack clear;restart with live
child;current-table identity/credit behavior;run-check affects other entries.
Caller/restart opcode ownership,deletion/control closure,valid script/capacity
and original execution traces remain open. This is a local restart contract,
not a complete pause,resume or event cancellation policy.
