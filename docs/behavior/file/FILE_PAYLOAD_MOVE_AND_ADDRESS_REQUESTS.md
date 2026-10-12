# Complete file-payload move and address-clear/setup requests

US G8ME01 revision0; ID2,2026-10-12. Complete fileGarbageMoveMem8018a8d0/c8.
Verify `python3 tools/verify_smart_compaction.py`;read
[compactor](../memory/SMART_HEAP_COMPACTION_AND_PENDING_FILES.md),
[file holder/acquisition](FILE_ACQUISITION_AND_REFERENCE_REUSE.md),
[async request state](FILE_ASYNC_COMPLETION_AND_DVD_REQUEST_STATE.md).
Clear/setup relocation engines are linked identities,not decoded type schemas.
No original payload-move/I/O/texture execution trace is claimed.

## Arguments and initial address-clear admission

Retain destination D(r3) and raw file entry E(r4). Read BYTE E00 with no entry
null guard. If exact3,read WORD E.ac. Nonzero skips address clearing;zero calls
fileGarbageDataAdrClear(E). Every other state calls the same clear helper.
Ignore its return. The clear argument is the FILE ENTRY,not its holder/payload.
This function does not check header tags,reference count,type or movement bounds.

A pending state3 file therefore skips clear but still moves below. It is the
caller smartGarbage that supplies the nonworker movement barrier;this helper
itself never queries current thread,waits,defers or rejects pending data.

## Copy through the current holder,then reread holder for pointer publication

Read CURRENT E.a0 as H. Read H00 as source and H04 as length. Request
memmove(retained D,source,length),ignore return. Reread CURRENT E.a0 as H2;
store retained D to H2.word0. No retained initial-holder substitute is used for
this pointer publication. Header/payload address domains and physical aliases
must remain coherent through the clear/copy operations.

This path assumes a holder supporting the length word at+4,such as the smart
header supplied by DVD allocations. There is no local embedded-holder exclusion
or graceful type mismatch. Valid file/holder producers need separate source
evidence;do not invent an archive-copy length based on the borrowed payload.

Only the holder's payload word is directly published by this helper. It does
not move/reallocate the file entry or header,resize the region,clear source
bytes,change count/state/type/callback/list links,or mark the request complete.
memmove may change other bytes through overlap/aliases;do not claim a general
nonaliasing domain or an independently recovered SDK copy engine.

## Fresh state/pending branch chooses request retarget or address setup

After pointer publication,reload BYTE E00. If exact3,reload WORD E.ac.
When nonzero DREQ,store retained destination D to WORD DREQ7c,then return with
no address-setup call. The request's length,offset,callback,flags/status remain
unchanged locally. This retargets its existing request destination;it does not
open a new handle,requeue I/O or prove the worker has not already captured a
previous destination. Actual DVD worker/interrupt policy remains open.

When state3/pending0,or any other state,read CURRENT E.a0 as holder,read BYTE
E01 type,read holder.word0 payload,sign-extend type byte,and request
fileGarbageDataAdrSet(payload,signedType),ignore return. There is no old/new
state comparison or fixed snapshot tying this branch to the earlier clear gate.
A provider can change E.state/pending/holder/type during clear/copy;later live
reads determine setup or retarget independently.

| Initial branch | Final branch | Local service sequence |
| --- | --- | --- |
| state3,pending nonzero | state3,pending nonzero | copy,publish,retarget current pending request |
| state3,pending nonzero | ready/other state | copy,publish,address setup |
| ready/other state | state3,pending nonzero | address clear,copy,publish,retarget |
| ready/other state | ready/other state | address clear,copy,publish,address setup |

These are memory-provider cases,not measured original source transitions. There
is no user-completion callback invocation,DVD close or smartFree in this body.
Raw function return has no specified scalar success contract.

## Resource implementation and acceptance requirements

Implement two pointer levels:entry.a0→header/holder→payload. Moving bytes must
update the live holder rather than replacing entry identity. Preserve original
clear ENTRY argument versus setup PAYLOAD argument,signed-byte type,retained
D/E but live holder/state/pending reads,and request7c retarget after publication.
Asset contents remain supplied original data outside source history;movement
is not a texture conversion or geometry adaptation policy.

Check state3/pending admission combinations,all other bytes,clear/copy mutation,
H versus H2 alias differences,overlap/raw length,pointer publication before
pending retarget,type80/ff sign extension and no invented completion/requeue/
heap cleanup. Direct callers require explicit valid holder/arena provisioning;
invalid addresses may be surfaced by a host provider without claiming original
safe failure behavior.

Type-specific clear/setup/TPL/animation relocation,smart arena/allocation,
DVD worker captured requests/cache/draw platform/source/scene and original
traces remain open under EVT-01/JUMP-01/DMG-02. This local helper is implementable
while its required backend interfaces and source domains remain explicit.
