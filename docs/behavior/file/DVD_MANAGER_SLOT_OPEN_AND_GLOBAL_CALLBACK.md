# Complete DVD manager slot opening and global callback setup

US G8ME01 revision0; ID2,2026-10-12. Complete DVDMgrOpen80144e88/19c and
DVDMgrSetupCallback80144dc4/8. Verify `python3 tools/verify_dvd_manager_requests.py`;
read [synchronous protocol](DVD_MANAGER_SYNCHRONOUS_READ_PROTOCOL.md),
[file requests](FILE_ASYNC_REQUEST_AND_CACHE_PUBLICATION.md),
[deferred close](FILE_ARCHIVE_AND_DVD_ACQUISITION_REQUESTS.md).
Platform path/open engines and worker are linked identities,not full contracts.
No original disc/thread/source/runtime execution trace.

## Path admission precedes reading the queue pointer

Retain filename P(r3),request parameter A(r4),parameter C(r5). Call
DVDConvertPathToEntrynum(P). Only exact full-word -1 returns0 immediately;
all other raw returns continue. The returned entry number is not retained or
passed to the subsequent DVDOpen. No filename-null/local syntax guard or signed
negative-general rejection occurs. Path conversion failure semantics beyond
this numeric branch belong to the SDK provider.

## First of256 slots without occupied bit8000 wins

Read queue base WORD dvdq8041ebf8 once. Traverse256 records of stride0x98=152
bytes,in ascending index order. The loop has32 groups,each checking eight slots
before advancing to the next group. Each admission reads U16 slot90 and tests
only bit8000;clear wins immediately. Other flags,including read/complete/close,
name,type,callback and SDK state do not affect this local predicate.

No global base reread,thread query,yield,lock,worker service or deferred-close
reclaim occurs during the scan. If all256 occupied,return0. No slot guard or
pool-bound check exists beyond the fixed index threshold;queue provisioning is
separate. Preserve the unrolled ordered reads under address aliases rather than
choosing a free native descriptor by a different predicate/order.

## Clear selected slot,copy name,set parameters,then mark occupied/open

Retain selected D. Call memset(D,0,152),then strcpy(D,P),ignoring returns.
There is no local copy bound;do not infer a safe64-byte maximum from D+40 SDK
info placement. Source filename domains and host rejection policy must be explicit.

Then store HALFWORD D90=0;store HALFWORD D92=zero-extended LOW8 A;store HALFWORD
D94=LOW16 C. Store WORD D7c=0,D80=0,D84=0,D88=0,in that order. Other fields were
cleared by memset,although an overflowing/aliased strcpy can change them before
these later stores. Read U16 D90,OR8000,store halfword back.

Call DVDOpen(D,D+40hex),passing copied filename at D and embedded SDK fileInfo.
Zero result requests a SECOND memset(D,0,152),then returns0. Any nonzero result
returns retained D. No direct platform close,free,callback or rollback service
beyond that zeroing occurs. No second path-conversion call or entry-number
fast-open is used. The occupied bit is published BEFORE the SDK call;source
thread/interrupt/slot-sharing policy remains worker/platform evidence.

## Callback setup writes one global pointer

DVDMgrSetupCallback(CALLBACK) stores raw r3 to WORD _callback8041ebf4 and returns.
No slot argument,null/type validation,invocation or request callback field store.
Per-request callback D8c is assigned by DVDMgrReadAsync and cleared by DVDMgrRead;
it is a distinct address/domain from this global word. The global consumer and
invocation/return policy belong to DVDMgrMain/worker recovery,not this setter.

## Implementation and acceptance requirements

Provide original32-bit queue/slot/global addresses and byte coherence. Preserve
path admission -1 only,first-slot8000 test,256*152 domain,name copy/service order,
LOW8 A into halfword versus LOW16 C,occupied-before-open and failed-open full
slot reset. Native host descriptors/payload storage and SDK fileInfo need
explicit mapping;open0 is not an independent generic allocation exception.

Check path result-1/-2/0/positive,all occupied/first/last slots,flags other than
8000,base capture,slot aliases/name overflow provider policy,parameter truncation,
clear→copy→stores→SDK call,open0/nonzero and global callback0/nonzero. Verify
request-local versus global callback ownership separately.

Worker/slot provisioning/SDK open/path/parser/thread failures,source filename/
parameter domains,DVD platform/cache/scene and original traces remain
EVT-01/JUMP-01/DMG-02 work. Reused original resources stay outside source history;
this slot API converts no asset bytes and proves no full-game loading fidelity.
