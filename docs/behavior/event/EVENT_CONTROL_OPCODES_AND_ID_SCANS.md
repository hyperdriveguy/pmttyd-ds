# Event controls92..106: child yield,setters,stop/start and ID scans

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Scoped complete dispatcher regions92..106 in evtmgrCmd800316b8/13b8;
complete evtSetPri800365fc/8,evtSetSpeed800365ec/10,evtSetType800365e4/8,
evtStopOther80035b4c/88,evtStartOther80035ac4/88,
evtStopID80035d9c/a0,evtStartID80035cfc/a0 and evtCheckID80036604/60.
Read [run helpers](EVENT_RUN_AND_ID_OUTPUT_VARIANTS.md),
[descendants](EVENT_CHILD_BROTHER_AND_INLINE_CONSTRUCTION.md),
[restart](EVENT_RESTART_PRESERVATION.md),
[dispatch results](EVENT_DISPATCH_AND_WAIT_CONTINUATIONS.md) and
[stop/lifetime](EVENT_RUNTIME.md). Verify:
`python3 tools/verify_event_run_and_controls.py`.
Containing dispatcher fingerprint does not recover other opcode bodies.

## Exact dispatch bindings

E denotes current entry. Operands are resolved via evtGetValue except SPEED,
which uses evtGetFloat. Each block below has no first-call gate. Scalar service
returns are ignored except CheckID. Handler2 continues within this dispatcher
invocation;255 returns dispatcher-1 and ends only E's current credit loop.

| Opcode / target | Whole local request sequence |
| --- | --- |
| 92 /80032770 | evt_run_evt(E);forward its handler2 |
| 93 /80032780 | evt_run_evt_id(E);forward its handler2 |
| 94 /80032790 | Resolve script0;ChildEntry(E,script,flags0);clear E.opcodea=0;handler255 |
| 95 /8003244c | Resolve ID0;evtDeleteID(ID);handler2 |
| 96 /80032828 | Resolve script0;store E.restart1a0=script;evtRestart(E);handler2 |
| 97 /800327c0 | Resolve priority0;evtSetPri(E,priority);handler2 |
| 98 /800327e4 | Resolve speed0 through evtGetFloat;evtSetSpeed(E,speed);handler2 |
| 99 /80032804 | Resolve type0;evtSetType(E,type);handler2 |
| 100 /8003284c | Resolve mask0;evtStopAll(mask);handler2 |
| 101 /80032868 | Resolve mask0;evtStartAll(mask);handler2 |
| 102 /80032884 | Resolve mask0;evtStopOther(E,mask);handler2 |
| 103 /800328a8 | Resolve mask0;evtStartOther(E,mask);handler2 |
| 104 /800328cc | Resolve ID0;evtStopID(ID);handler2 |
| 105 /800328e8 | Resolve ID0;evtStartID(ID);handler2 |
| 106 /80032904 | Resolve ID0;capture RAW output1;evtCheckID(ID);evtSetValue(E,rawOutput,result);handler2 |

There is no use of ID destination as an input value or exact ignored-output
sentinel in106. Scalar setter/deletion/stop returns do not become event results.
Opcode96 replaces restart script before resetting;it does not restart from old
current next14 or allocate an independent root.

Opcode94's handler255 does NOT mean this entry was deleted. ChildEntry installs
E.child70 and flags8|0x10;the dispatcher clears opcode before returning-1. The
manager stops E's remaining-credit loop and later eligibility skips waiting E.
Deletion of the child clears parent wait/copies locals under the existing lifetime
contract;parent later fetches its NEXT command. Do not implement94 as recursive
immediate execution,another RUN_EVT,owner deletion or handler2 continuation.
Child flags0 still append under its constructor's active-manager policy.

Opcode95 returns handler2 even if its ID deletion service changes current entry
storage. There is no wrapper-side self-ID check,special terminal handling or
restart rollback. The complete evtDeleteID/evtDelete lifetime algorithms and
valid control domains remain separate obligations;do not invent safe recovery.

## Complete setter leaves

SetPri stores low8(priority) to E+b,without resorting the current table.
SetType stores low8(type) to E+c,without calling run-check or automatically
changing stopped flags. SetSpeed loads current global evtSpd804151ec,multiplies
supplied binary32 speed by it with single fmuls,stores E+164. Credit168 remains
unchanged;there is no min/max/sign/finite validation. A setter inside a manager
visit does not retroactively change the credit increment already performed at
visit entry. Global scale is applied here rather than retaining the raw factor.

## Other-event scans

StopOther(excludedPointer,mask) and StartOther capture current field/battle context
through GP14,read its entry-array pointer and start slot0. At each iteration,
check active bit1. For active entries whose POINTER differs from excludedPointer,
call evtStop(entry,mask) or evtStart(entry,mask),respectively. Advance stride1b0;
reload signed context.capacity0 for every loop test. No saved-ID validation,
priority sort,mask narrowing or direct type filter in this wrapper.

The exclusion is pointer identity,not actor/event ID or all related entries.
Called relationship traversal can still affect the excluded entry indirectly;
this local direct exclusion does not establish caller immunity. Existing stop/
start bodies apply the supplied mask/type policy and relationship effects.
Context is captured once by this wrapper;callee services may reload globals.
Both return no specified scalar value and provide no per-match output.

## Stop/start by ID and CheckID

StopID(ID)/StartID(ID) capture context,array and scan ascending slots,checking
active bit1 then exact word15c equality. EVERY matching active entry is passed
to evtStop/evtStart with HARDCODED mask0xef. They do not break after the first
match,clamp/normalize ID or select by array slot. Capacity is reread at every
loop condition. A type whose only bit is0x10 has no direct mask intersection;
ID targeting does not bypass this filter in the called service. No match gives
no service requests. There is no returned success/failure contract.

CheckID(ID) instead captures capacity ONCE and uses a counted loop over its
array. First active entry with exact15c match returns integer1 immediately;
otherwise return0 (also for nonpositive captured capacity). Stopped/waiting/
0x80 entries still count as live while active. It does not test eligibility,
completion of an event action,current-context schedule inclusion or script
execution. No reserved0/negative ID exclusion occurs locally. Duplicate-ID states
therefore differ:Check reports first match;Stop/Start processes all matches.

Acceptance cases:child255 yield while owner remains active and waiting;restart
new-script before reset;low8 priority/type without immediate resort/run-check;
speed preserves credit;all/other/ID distinctions;pointer exclusion with transitive
service effects;live versus captured capacity;duplicate-ID all/first policies;
stopped active liveness;hardcoded0xef mask and raw output alias. Complete
stop/start/delete traversal,remaining opcode/namespace/domain initialization and
original pause/creation/deletion/multicredit traces remain open under EVT-01.
