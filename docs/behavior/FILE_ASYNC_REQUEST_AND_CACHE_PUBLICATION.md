# Complete async file request and cache publication

US G8ME01 revision0; ID2,2026-10-12. Complete fileAsync80189d44/3a4.
Verify with `python3 tools/verify_file_async.py`. Read
[acquisition/reference](FILE_ACQUISITION_AND_REFERENCE_REUSE.md),
[archive dispatch](FILE_ARCHIVE_AND_DVD_ACQUISITION_REQUESTS.md),
[completion](FILE_ASYNC_COMPLETION_AND_DVD_REQUEST_STATE.md) and
[effect driver](EFFECT_DRIVER_UPDATE_AND_TEXTURE_CACHE.md).
Static evidence only;no original async timing,worker or scene trace is claimed.

## Arguments and cached result are raw addresses/statuses

Retain path P(r3),type T(r4),optional completion pointer C(r5). Read current
afp80418498 manager's used head08;walk E.a4. State0 skips string comparison.
Other states call strcmp(E+20,P);first equal name wins independently of T,C,
archive mode,context or size. On a match,read pending E.ac. Nonzero returns0
immediately,without yielding or changing fields. No later duplicate is examined.

When pending0,reload state. Exact3 returns E immediately. Exact2 stores state3,
then returns E. Every other state returns E without mutation. No count increment,
count reset,new C assignment,relocation or holder check occurs on cache hits.
A reactivated state2 can retain a nonzero or negative count;normal count domains
require their producers. This API is a readiness/preload probe,not acquisition
of an additional reference under its local contract.

| Path | Local raw return |
| --- | --- |
| Matching used entry with pending!=0 | 0 |
| Matching used entry with pending0 | Entry address E |
| New archive-backed entry | Entry address E |
| First DVDMgrOpen fails | ffffffff (-1),not0 |
| DVD rounded length becomes0 | 0 after deferred close |
| New DVD request after submission | 0 regardless of request-leaf return |

Do not collapse nonzero to guaranteed resource success or treat every0 as the
same failure. In particular the effect driver's nonzero branch admits -1 before
making a separate fileAllocf call;it does not dereference the async return.

## Available node and archive mode match acquisition until initialization

On miss,reload manager,capture E=M10. If null,call _fileGarbage(1),reload head;
if still null,call _fileGarbage(0),reload head. Reload manager,capture NEXT=E.a4
and signed archive mode M04. No final-null guard before E.a4 exists.
Archive calls are exactly those in the linked dispatcher:mode0 one probe,
mode1/2 up to three identical arcOpen(P,0,0) probes;other signed modes skip.
Retain nonzero archive result R;call fileGarbageDataAdrSet(R,T),ignore return.

Initialize archive E in order:state3;holder E.a0=E+4;reread holder/store R at its
word0;count0;low8T;E.a4=0;strcpy(E+20,P);E.ac=0. No E.a8 callback store occurs.
Thus a recycled entry's prior callback word is preserved on this path,but not
invoked locally. There is no local archive-size or payload copy.

## Archive free-head publication rereads a live link after clearing E.a4

The archive path does NOT publish captured NEXT. After string copy and pending
clear,reload manager M;read CURRENT head H=M10;read H.a4;store that word to the
SAME M10. Reload manager;when its head is0,clear that manager's tail14. Then
reload manager and append retained E to used head08/tail0c in the same live
head/tail sequence as synchronous acquisition. Return E.

This ordering is directly pinned at80189f38..80189f68. In the ordinary case
where no provider changes head H=E,the earlier E.a4=0 means the subsequently
read link is0,even if captured NEXT was nonzero. A port must not silently use
NEXT on this archive branch because the DVD/synchronous paths use it. Whether
source provisioning avoids a multi-node available chain here remains open.
A swapped head means that OTHER current node supplies the popped link;retained
E is still the node appended. No null guard protects that live H dereference.

## DVD path closes length probe before allocation,then opens again

Call DVDMgrOpen(P,2,0). Null returns-1 with no local entry/list mutation.
Otherwise get WORD D74 length,round up to32 using low5 remainder/add with32-bit
wrap. Rounded0 calls DVDMgrClose(D),returns0. Nonzero also calls
DVDMgrClose(D) BEFORE smart allocation;the first handle is only a length probe.
Repeat low5 rounding test/add,then smartAlloc(roundedLength,0). No null guard.
Store returned header H08=E;E.a0=H,state3,count0,low8T,E.a4=0,E.a8=C;
strcpy(E+20,P). This branch does not explicitly clear E.ac before publication.

Publish available head using retained NEXT,clear tail when current head0,and
append retained E through the live used-list head/tail sequence. Only THEN call
DVDMgrOpen(P,2,0) again and store RAW result to E.ac. No check rejects a null
second open or rolls back the already published entry.

Reload E.ac for D,read retained H00 as destination,and request
DVDMgrReadAsync(D,destination,roundedLength,0,dvdReadDoneCallBack8018a1a0).
Ignore its return;return0. The complete request leaf dereferences D,so a null
second open is an unchecked memory boundary. No relocation occurs yet;the
completion callback supplies it. Pending clear/read worker/close scheduling
are not determined by a native simulation tick or an event callback's polling.

## Implementation and acceptance requirements

Keep original32-bit pointer/status returns,shared manager/list bytes,pending
handle,holder indirection and preserved E.a8 on archive reuse. A ready archive
state3/count0 becomes state1/count1 only when a separate _fileAlloc acquires it.
DVD submission publishes state3/count0 before reopening and queueing I/O;no
implicit reference is added by preload or by its completion.

Fixtures need duplicate-name first match,pending ready/not-ready,state2→3 with
count preservation,ignored new T/C on hit,raw-1 first-open failure,rounded0,
archive live-head link versus retained NEXT,manager/head swaps,archive callback
word preservation,DVD callback assignment,list publication before second open,
null second-open boundary and ignored request return. Keep deferred-close probe
handle distinct from final pending handle even if a backend later reuses slots.

Full _fileGarbage/type-dependent relocation/smart allocator/DVD worker/platform,
source counts/archive mounts/valid type and name domains,thread timing,TPL and
scene execution remain EVT-01/JUMP-01/DMG-02 work. No original async success or
complete-game resource engine is inferred from these local operations.
