# Complete event context initialization and reset/capacity provenance

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Complete evtmgrInit8003781c/118,evtmgrReInit800377b0/6c and called
 evt_msg_init800d2db0/4. [Bootstrap heap/callers](EVENT_BOOTSTRAP_HEAP_AND_RESET_CALLERS.md)
closes called local allocation and scoped source bindings.
`python3 tools/verify_event_context_initialization.py` checks eight whole/linked
hashes,139 anchors,two startup/static spans and four direct DOL call sites.
Static original DOL evidence;no original initialization/reset execution claim.
Read [manager](EVENT_MANAGER_COMPLETE_LOOP.md),
[constructors](EVENT_ROOT_CONSTRUCTORS_AND_APPEND.md),
[deletion](EVENT_DELETE_AND_COPYBACK.md) and
[all119 opcode coverage](EVENT_OPCODE_IMPLEMENTATION_COVERAGE.md).

## Concrete context storage and capacity

Field C0=803d8d30,battle C1=803d8dd0 (C0+a0). Both use:

| Offset | Original layout / initialization meaning |
| --- | --- |
| 0 | Signed capacity word;Init explicitly sets256 in EACH context |
| 4..83 | 32 global work WORDS (128 bytes) |
| 84..8f | Three global flag words (12 bytes) |
| 90 | Entry-array pointer,allocated separately for each context |
| 94 | Unwritten by these init/reset bodies;no invented reset meaning |
| 98 /9c | Previous timestamp high/low |

Entry stride1b0=432 bytes. Each initial allocation requests256*1b0=1b000
bytes (110592). Two payload arrays total36000 hex=221184 bytes (216KiB),
excluding SDK heap header/rounding. This is direct original capacity/allocation
provenance,not a value measured from emulator behavior or chosen for the port.
The manager's256-word shared slot/ID schedule banks and opcode118's256 probes
now have a concrete initial array extent;later capacity writers/aliases and
source valid-free-slot invariants are still distinct requirements.

## evtmgrInit complete write/call sequence

No context-selector test:Init always initializes BOTH contexts in field-then-
battle order. For field C0,perform:

1. Store C0.capacity=256. Compute byte count256*1b0;call __memAlloc(heapIndex0,
   byteCount1b000). The allocation service can return0;Init adds no failure test.
2. Store returned pointer to C0.entryArray90. Reload GP pointer80415fd0;read
   GP.word38 high,then GP.word3c low. Store low to C0.9c THEN high to C0.98.
3. Reload C0.capacity;memset(the returned allocation pointer,0,capacity*1b0).
   The pointer is the allocation result retained in the argument register,not
   another read of C0.entryArray. Count multiplication wraps32. There is no
   per-entry construction/deletion/callback or free-list population.
4. memset(C0+4,0,0x80),then memset(C0+84,0,0x0c).

Repeat equivalent steps for C1:store capacity256,request heap0/1b000,store
entry pointer,AFTER that allocation reload GP,read high/low and store low/high,
clear returned array with reloaded capacity,then work bank and flag bank.
Timestamp snapshots for field and battle can therefore differ;Init does not
capture a single GP timestamp before both allocations. GP clock source/refresh
frequency and heap platform ownership are outside this local body.

Finally store global allocation count8041e860=0,then live schedule count
8041e85c=0,then manager-active8041e858=0. Call evt_msg_init and return with no
specified scalar result. That message-init body is ONLY `blr`:no message bank,
text lookup,window cleanup or callback state reset occurs in this local leaf.

__memAlloc already zeroes/flushes successful allocation payload;Init's array
memset is an additional clear. On allocation failure,Init still publishes0
and invokes memset with positive size at0. No recovered “stop initialization”
or error return is established. Native allocation failure checks can be stated
as platform/domain policy,not original local recovery.

## evtmgrReInit complete selected-context reset

Read GP once from80415fd0. Select C0 if GP.word14==0;C1 for every nonzero word.
Using this SAME captured GP,read timestamp high38 then low3c. Store selected
context.previous LOW9c then HIGH98. Load selected capacity0 and array90;
memset(array,0,wrap32(capacity*1b0)). Then store allocation count8041e860=0,
manager-active8041e858=0;call the no-op evt_msg_init;return without scalar contract.
There is NO allocation,context-capacity write,array-pointer replacement,work/
flag bank clear or per-entry evtDelete invocation in ReInit.

The preserved/changed state is materially different from Init and deletion:

| State | Init | ReInit |
| --- | --- | --- |
| Entry arrays | Allocate/clear both | Clear selected existing array only |
| Capacity and pointer | Set both capacity256 and pointers | Preserve both |
| Context previous timestamp | Snapshot separately for both | Update selected only |
| Global work/flag banks | Clear both | Preserve both |
| Shared allocation count8041e860 | Set0 | Set0 even though other array is untouched |
| Manager-active8041e858 | Set0 | Set0 |
| Live schedule count8041e85c | Set0 | Preserve |
| Shared ID/slot schedule contents | No explicit clear | No explicit clear |
| Global event ID804151e8 and speed804151ec | No writes | No writes |
| User buffer allocations/parent copy-back | No local free/copy-back | No local free/copy-back |

The initialized DOL words at804151e8/ec are ID1 and float1.0,independently pinned.
These static initial values are NOT assignments performed by Init/ReInit.
Repeat Init allocates new arrays and replaces pointers without a local release
of old arrays;it does not reset the global ID sequence/speed or bulk-clear the
whole context record. GP system-level mask18 and saved pouch/switch storage are
not written by either body. Initial BSS/GP/heap/global initialization is a
separate bootstrap boundary;unwritten fields must not be inferred reset here.

## Reset versus cancellation and manager implications

Selected entries'flags,IDs,locals,relationship pointers,stacks,timers,credits
and user pointers become zero through bulk clear. No child locals copy back,
parent wait callback,audio stop,map-buffer free or semantic destructor runs.
The other array and BOTH context work/flag banks remain untouched on ReInit.
Shared count0 therefore need not equal the aggregate active-entry count across
both contexts immediately afterward. It is not proof the other array was empty.
The manager's separate active scheduling rebuild/reuse rules still apply.

ReInit leaves shared schedule count/tails untouched but clears manager-active;
no reentrant/current-manager invocation guard exists. Script/source/scene domain
must establish when reset can occur. A native reset implemented as repeated
DeleteID or StopAll introduces different copy-back/lifetime/ownership behavior.
A reset implemented as full bank/context memset erases state that this body
preserves. Parent graph/buffer allocations need separately evidenced teardown.

Acceptance:both256 capacities/two allocations,field/battle timestamp load and
low-before-high stores,allocator failure path,duplicate payload clears,exact
work/flag extents;ReInit0 versus nonzero GP14,captured GP/timestamp,selected array
clear,other array/banks preservation,global reset order and preserved schedule
count/ID/speed;no per-entry destructor or heap free. Capacity mutation,original
valid-free-slot/label/stack/source domains,scene/cancellation/reset traces and
platform heap/callback/clock bindings remain EVT-01 work. Complete local init
provenance does not establish a complete executed story/Jump VM.
