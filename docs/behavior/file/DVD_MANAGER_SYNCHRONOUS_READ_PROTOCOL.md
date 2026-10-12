# Complete DVD manager synchronous-read request and wait protocol

US G8ME01 revision0; ID2,2026-10-12. Complete DVDMgrRead80144e18/70.
Verify `python3 tools/verify_dvd_manager_requests.py`;read
[slot/global callback](DVD_MANAGER_SLOT_OPEN_AND_GLOBAL_CALLBACK.md),
[async leaf](FILE_ASYNC_COMPLETION_AND_DVD_REQUEST_STATE.md),
[file acquisition](FILE_ARCHIVE_AND_DVD_ACQUISITION_REQUESTS.md).
This local function publishes request state and yields;it does not implement
original worker/platform I/O or establish measured read timing.

## Ordered request words and two flag read/modify/store pairs

Arguments D(r3),destination(r4),length(r5),offset(r6). Retain D;no null,occupied,
existing-request,alignment,length/offset/source or callback ownership guard.
Perform these operations exactly:

1. Store destination to D7c,length to D80,offset to D84.
2. Read U16 D90,OR0001,store halfword D90.
3. REREAD U16 D90,clear0002,store halfword D90.
4. Store WORD D8c=0,then WORD D88=0.

Other flag bits,including8000 occupied and0008 deferred close,are preserved by
these two pairs. Each reread is observable;do not combine them into one snapshot
mask. No DVD read call/allocation/service occurs before the wait. It overwrites
any prior per-request callback with0;global _callback8041ebf4 is untouched.
D88's worker meaning remains separately open;this body only initializes it0.

## Wait solely for current flag0002;return current length word

Read U16 D90 and test0002. If set,exit without a yield. If clear,call
OSYieldThread(),ignore its return,then reread the same retained slot's flag word.
Repeat until0002 set. No thread identity/worker exception,pending-pointer test,
result/short-read/error branch,timeout,cancellation check or frame credit exists.
The source body has no main-thread versus worker fast path. Any claimed such
policy requires separately recovered caller/worker evidence.

Once admitted by0002,read CURRENT WORD D74 and return it. This is the same
field returned by DVDMgrGetLength,not D88 or a read-result counter. The wrapper
therefore does not return the requested length or independent measured number
of successfully read bytes. No callback/close is invoked on exit;flags/request
words remain whatever the worker/provider last supplied.

Even a length0 request writes state and waits. A retained close bit or a provider
setting0002 can end the local wait without a local success check. Reused/mutated
slot identities are not generation-checked. Original valid domains and worker
error behavior must be recovered before claiming successful synchronous loading.

## Composition with file and message loaders

_fileAlloc requests rounded file bytes through this wrapper,ignores its return,
requests deferred close,then initializes/publishes the file entry and performs
address setup. This return's length-field meaning does not add a successful-read
gate to that caller. Message resource loaders likewise retain their recovered
raw-buffer/read ordering;their original source/parser/state policy is separate.

DVDMgrReadAsync publishes similar request words/flags but stores a supplied
callback and returns immediately. Neither leaf itself issues platform I/O.
File completion callback has its own -3-only branch and pending clear;do not
substitute that callback for this synchronous wait or invent a generic promise
whose result changes source-visible fields/order. Worker drives actual progress.

## Native service requirements and acceptance

Separate portable request-slot memory from platform I/O/thread scheduling. Keep
all numeric flags/request/length/callback words coherent with raw32-bit original
addresses. A yield is a thread service request,not proof of one simulation tick
or rendered frame. Preserve action-command/event simulation cadence separately;
this wrapper provides no fixed read latency or battle timing unit.

Acceptance covers store order/flag reread,retained extra flags,callback overwrite,
D88 clear,immediate-ready after publication,multiple yields/provider mutations,
request0/error raw states,slot74 changes before return,no thread exception and
no close/callback/deallocator calls. Verify invalid-address/provisioning policy
explicitly;do not label authored backend fixtures original traces.

DVDMgrMain/initialization/deletion,SDK path/open/read/worker/error/cancellation,
callback consumers,source domains/cache/scene and original runtime remain open.
Complete Open/Read/global setter bodies and existing async/close leaves supply
local interfaces;they do not establish a complete DVD engine or full-game
resource loading/performance/hardware requirements.
