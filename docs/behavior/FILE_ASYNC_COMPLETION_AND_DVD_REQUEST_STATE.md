# Complete async completion routing and DVD request-state publication

US G8ME01 revision0; ID2,2026-10-12. Complete dvdReadDoneCallBack8018a1a0/c0
and DVDMgrReadAsync80144de4/34. Verify:
`python3 tools/verify_file_async.py`;read
[request/cache graph](FILE_ASYNC_REQUEST_AND_CACHE_PUBLICATION.md),
[holder/acquisition](FILE_ACQUISITION_AND_REFERENCE_REUSE.md) and
[deferred-close leaf](FILE_ARCHIVE_AND_DVD_ACQUISITION_REQUESTS.md).
These are memory/request/callback operations;worker execution and platform
result semantics beyond local numeric branches remain separately unproven.

## Request leaf writes words and flags in literal order

DVDMgrReadAsync(D,destination,length,offset,callback) has no local null guard,
platform call,loop or allocation. It performs:

1. Store destination at D7c,length at D80,offset at D84.
2. Read U16 D90;OR0001;store halfword D90.
3. REREAD U16 D90;clear bit0002;store halfword D90.
4. Store callback at D8c;store word0 at D88.
5. Return without a specified new scalar status value.

All other bytes are preserved. D90's two read/modify/write pairs are separate
observable operations;do not replace them with a single flags snapshot if
memory providers can change the flag word between accesses. Clearing0002 does
not clear0008 (deferred close) or other retained flags. D88's full semantic role
requires worker evidence;locally it is only initialized0.

The file request passes dvdReadDoneCallBack8018a1a0 here. Callback scheduling,
D+40 platform info provisioning and flag consumers are absent from this leaf.
No guarantee of synchronous completion,a given latency or event tick exists.

## Completion lookup uses pending+40 equality without a state gate

Inputs:raw result R(r3),raw callback fileInfo pointer I(r4). Read current manager
used head08. For each node E,read E.ac,compute WORD(E.ac+40hex) with32-bit wrap,
compare to I. First equality wins;otherwise advance live E.a4 until null.
No state,count,name,type or nonnull-pending test filters nodes. A pending0 node
therefore compares computed address40. This is literal address arithmetic,
not a native object identity/generation-checked callback map.

After match OR exhausted scan,read E.ac into retained D. There is NO not-found
return or null guard:exhaustion still dereferences E0+ac. Different nodes with
the same pending handle resolve first in used-chain order. No request-provided
entry context or smart-header backpointer is used by this lookup.

## Exact -3 branch clears pending,marks state/type and requests close

Compare full signed R to-3 (fffffffd). Only exact equality takes this branch:
store E.ac=0;store BYTE state2;store BYTE type0b;call DVDMgrClose(retained D).
Do not change count,holder,payload,name,next or callback word. Do not call
fileGarbageDataAdrSet or user callback. No heap free/read rollback/retry occurs.
Close only ORs flag0008 in D90 through the separately recovered leaf.

No other result is tested for success,negative sign,read length or short read.
All other values,including0,-1,-2 and positive values,take the following branch.
The meaning and actual set of platform result values must be sourced separately.

## Every other result relocates data,closes,then reads live user callback

Store E.ac=0 FIRST. Read E.a0 as holder H;read E01 byte;read WORD H00 as payload;
sign-extend type byte to a word;call fileGarbageDataAdrSet(payload,signedType).
Ignore its return. Then DVDMgrClose(retained D),ignore return. Only afterward
read CURRENT E.a8 as C. If C!=0,call C(E);ignore its return. Otherwise skip.
State/count are not reset or acquired on this branch. E can remain state3/count0
until a separate acquisition changes it.

Signed type matters:byteff passes-1 here,where a direct newly requested type
may originally be full word255 before its truncating E01 store. Do not silently
use an unsigned enum or merge request/completion relocation arguments.

The pending clear precedes relocation and close. If a service allows another
request/acquisition/reentrant observer at those boundaries,it sees pending0
before the later operations have completed. E/D are retained but holder/type/C
are read at their displayed times;C may be changed by relocation or close and
the new callback is then used. No captured initial callback or state validity
check is performed before invocation. The callback return does not set this
leaf's state or result contract.

## Composition with acquisition,release and effect cache

The local-3 path leaves a state2 entry with its holder intact and callback
suppressed. A later fileAsync match with pending0 sets state3 and returns E;
_fileAlloc on state3 resets count0 then adds1. Neither checks whether the bytes
were successfully read or the type was changed to0b. This is the recovered
composition,not proof that normal source scenes admit that failure recovery.
Collection/type0b producers/consumers and backing allocation validity remain open.

The effect driver treats async -1 as nonzero,then separately calls fileAllocf.
A null synchronous allocation can still be stored and the selector tag advanced
under its existing contract. Consequently bool-ready/retry-until-loaded and
free-on-pending-clear adapters would impose additional policy. Preserve raw
results and close/relocation/list/holder ownership until those consumers are
recovered. Asset reuse remains original data outside source history.

## Implementation acceptance and remaining evidence

Check exact request store order,flag reread/bit retention,all callback lookup
matches/misses/duplicate handles,zero-pending computed40,wrapped addresses,
-3-only branching,count/holder/callback preservation on failure,all other raw
results,sign extension of types80/ff,pending-clear before relocation,retained D
versus live holder/type/C,and callback mutation by backend calls. Invalid memory
may be surfaced explicitly by a host provider;do not label it an original
not-found success or benign ignore.

DVD worker/direct platform close/read/interrupt/thread callback cadence,
smart allocator/relocation/type0b collection and actual source failure domains,
TPL/animation/scene integration and original traces remain unimplemented
requirements. These complete local bodies do not prove that original assets
loaded or that a full async backend exists.
