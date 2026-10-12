# Complete archive request dispatcher and synchronous file-load request path

US G8ME01 revision0; documentation agent ID2,2026-10-12. Complete local
_fileAlloc8018a2a4/354 and arcOpen800782e4/d4,plus ARCGetStartAddrInMem,
ARCGetLength,ARCClose,DVDMgrGetLength and DVDMgrClose leaves. Verify with
`python3 tools/verify_file_acquisition.py`;read
[cache/reference/publication](FILE_ACQUISITION_AND_REFERENCE_REUSE.md).
ARCOpen,DVDMgrOpen/Read,smartAlloc,fileGarbageDataAdrSet,_fileGarbage and
thread/string engines are linked identities,not full backend implementations.
No original I/O/asset conversion/runtime success claim.

## Signed archive-mode dispatch on a new entry

After capturing available E and NEXT,the allocator reads signed M04 once.
Initialize resource R=0. Mode0 calls arcOpen(P,0,0) once. Mode1 and mode2 BOTH
perform the identical sequence:arcOpen(P,0,0),retry the same call if0,and retry
it a third time if still0. Stop on the first nonzero result. No local mode
change,different archive index,different filename or intervening yield occurs.
Every other signed mode skips archive calls entirely. This includes negatives
and values>=3;they proceed with R=0 to the DVD request path.

If R!=0,call fileGarbageDataAdrSet(R,T) BEFORE initializing E. Its return is
ignored. Set E.state=1;store E.a0=E+4;reload that holder word,store R into its
word0;store E.count=1;store low8T to E01;clear E.a4;strcpy(E+20,P);clear E.ac.
Then publish the used/available chains as described in the companion contract.
This is borrowed archive data via an embedded holder,not a copied heap payload.
No local size field,payload copy,archive owner reference or result check exists.

## arcOpen scans four active archive descriptors in fixed order

Capture name P and optional output pointers OUTADDR,OUTLEN. Starting at
803ebff0,scan four descriptors,each40 bytes. Read U16 descriptor00 and require
bit01;other bits do not affect admission. Call ARCOpen(descriptor+4,P,&local
fileInfo). Zero result advances;any nonzero is success. No selected-mode/index
argument reaches this dispatcher;the allocator's three retries each restart
this SAME four-descriptor scan.

On success call ARCGetStartAddrInMem(&fileInfo),retain ADDRESS. If OUTADDR!=0,
store ADDRESS. If OUTLEN!=0,call ARCGetLength(&fileInfo),store LENGTH. Then
call ARCClose(&fileInfo),ignore its return,and return retained ADDRESS. A
successful ARCOpen with ADDRESS0 still returns0,even if outputs were written.
On exhaustion,clear each nonnull output pointer in order OUTADDR then OUTLEN,
and return0. No output stores occur on individual failed descriptor probes.

The three complete SDK leaves establish only these operations:

| Leaf / address / size | Exact local behavior |
| --- | --- |
| ARCGetStartAddrInMem80077c1c/14 | Read info.word0 as handle H,then info.word4 as offset;read H.word0 as base;return base+offset with word wrap |
| ARCGetLength80077c14/8 | Return info.word8 |
| ARCClose80077c0c/8 | Return1;no memory or service operation |

The descriptor array's actual mount contents/ownership and ARCOpen path parser
remain backend/source requirements. Closing this local fileInfo does not prove
archive deallocation;the recovered ARCClose leaf performs none.

## DVD path,rounded allocation and unchecked read

When R==0,call DVDMgrOpen(P,2,0),retain handle D. Null returns0 immediately,
without initializing E or locally consuming its available-chain node.
Otherwise DVDMgrGetLength(D) returns WORD D74. Compute unsigned low5 remainder;
if nonzero add32-remainder with32-bit wrap. If rounded result==0,call
DVDMgrClose(D),ignore return,and return0. Consequently length0 and overflowing
round-up can reach this failure path. There is no signed-positive length guard.

The source repeats the low5 alignment test/add before allocation;preserve its
word arithmetic even though the first rounding normally clears the low5 bits.
Call smartAlloc(roundedLength,0),retain returned header H. No null-result guard
exists. Write H08=E BEFORE reading H00 as destination. Request
DVDMgrRead(D,WORD H00,roundedLength,0),ignore its return,then DVDMgrClose(D),
ignore its return. The request length can exceed the reported file length by
up to31;short-read/padding/allocator policy is delegated to the backend.

Store E.a0=H,state1,count1,low8T,E.a4=0;strcpy(E+20,P);clear E.ac. Then reread
CURRENT H00 and call fileGarbageDataAdrSet(thatPayload,T),ignore its return.
Finally publish E using retained NEXT and current manager reloads. The archive
path calls garbage-address setup BEFORE entry writes;the DVD path calls it
AFTER string copying and pending clear. This order affects aliases/providers.
No read-success test separates entry publication from DVD read failure.

## DVD close is a flag request,not a direct platform close

Complete DVDMgrClose80144dd4/10 reads U16 D90,ORs0008,stores the low16 back.
No platform DVDClose,heap free,pointer clear,status result or null guard occurs.
DVDMgrGetLength80144dcc/8 only returns WORD D74. Actual deferred close/read/
open/worker/slot ownership and all DVD platform outcomes remain backend work.
A host close that immediately destroys the handle needs a separate equivalence
argument;this recovered leaf only marks a bit in retained manager memory.

## Asset reuse and implementation acceptance

Reuse original archive/file contents and keep converted packs/extracted data
outside source history. The allocator does not transform texture/model/message
bytes locally;fileGarbageDataAdrSet is a separate type-dependent relocation/
address contract. Do not infer a type enum,complete TPL decoder or modified
geometry policy from the byte T copied here. Effect-cache type4 and its US TPL
source are separately recorded in the linked driver and asset documentation.

Acceptance should exercise every signed mode,all1/2 retry exits,four-descriptor
order/active-bit filtering,optional output aliases,successful parse with null
address,modular start-address arithmetic,length0/31/32/overflow,unchecked
smartAlloc boundary,read failure with continued publication,H00 changes during
read/close/copy,and different archive/DVD relocation-call order. Preserve raw
handles until separately recovered collection consumers allow reclamation.
Original platform,source provisioning,relocation,async,eviction/scene and full
resource rendering remain open;these local request contracts are implementable
without inventing their missing backend behavior.
