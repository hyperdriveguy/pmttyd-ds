# Complete formatted file-request forwarding and shared filename storage

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Complete fileAsyncf8018a0e8/b8,fileAllocf8018a61c/c8 and fileAlloc8018a5f8/24.
Verify:`python3 tools/verify_file_request_wrappers.py`:seven complete/linked
identities,122 instructions and effect-path format span. Formatter/async/load
identities are linked,not newly complete resource/cache/platform engines.
Read [handle release](FILE_HANDLE_RELEASE_AND_EFFECT_CACHE_OWNERSHIP.md),
[effect cache caller](../effect/EFFECT_DRIVER_UPDATE_AND_TEXTURE_CACHE.md) and
[original formatter runtime](../format/FORMAT_OUTPUT_RUNTIME.md).
Static source behavior;no original file/format/driver/runtime execution trace.

## Original named arguments and exact downstream order

Use raw word A/B for named parameters rather than inventing resource/backend
semantics from their values. The complete forwarding contracts are:

| Public wrapper input | Formatting request | Downstream request / raw return |
| --- | --- | --- |
| fileAsyncf(A,B,format,...) | vsprintf(sharedFilename,format,&originalVa) | fileAsync(sharedFilename,A,B);propagate its r3 return |
| fileAllocf(A,format,...) | vsprintf(sharedFilename,format,&originalVa) | _fileAlloc(sharedFilename,A,0);propagate its r3 return |
| fileAlloc(path,A) | None | _fileAlloc(path,A,0);propagate its r3 return |

fileAllocf calls _fileAlloc DIRECTLY,not fileAlloc. fileAlloc overwrites third
argument with0 irrespective of incoming r5;it has no pointer/size/type/null
validation,formatting,cache check or allocation-success policy. The underlying
_fileAlloc8018a2a4/354 and fileAsync80189d44/3a4 remain complete body identities
without their request/cache/read semantics recovered here. Native adapters must
bind actual backend behavior,not replace a request with fabricated success.

Both formatted wrappers retain A (and B for async) before formatting. Formatter
return is ignored;there is no negative/truncation/error branch before the backend
call. No callback/user event result interpretation,first-call gate,source-domain
normalization,path canonicalization or implicit0/1 return conversion. Buffer
content supplied by formatter is what downstream sees,including unsupported
format-domain outcomes. Backend r3 returns survive ordinary register restoration.

## Exact shared filename buffer

Both wrappers use global BSS `_filename` at803e6e78,size400 hex=1024 bytes.
They pass the SAME address as vsprintf destination and downstream filename.
No per-call buffer,heap allocation,whole-buffer clear,capacity argument,lock,
thread-local view,copy-on-return or retained owned filename object. Subsequent
formatted requests overwrite that storage. A shorter result need not clear
bytes after its terminating null. Actual backend copying/retaining a filename
is a fileAsync/_fileAlloc obligation,not supplied by these wrappers.

vsprintf's own output/NUL rule is already in the formatter contract. These
wrappers pass no1024-byte bound even though the declared buffer extent is1024;
they do not choose snprintf. A native allowed-domain check can be explicit,
but bounded truncation is not original local file-wrapper behavior. Source
strings/formats/varargs,raw memory alias and platform failure policy remain
required;the ordinary effect path is a short controlled source format.

## PowerPC varargs record,register capture and bit6

Both bodies allocate0x90 bytes of stack and save original r3..r10 WORDS at
newSP+08,+0c,+10,+14,+18,+1c,+20,+24. On entry test condition-register BIT6
(`bf6`),not CR FIELD6. Only when that bit is SET,save f1..f8 as eight DOUBLES
at newSP+28,+30,+38,+40,+48,+50,+58,+60. When bit6 is clear,those FP save
slots are not initialized by this body. Do not infer or regenerate an FP count
from the format string before supplying the original record.

Build a12-byte varargs record at newSP+68,with:

| Word / offset | Asyncf | Allocf |
| --- | --- | --- |
| +0 | 03000000 (initial general index3,float index0) | 02000000 (general index2,float index0) |
| +4 | newSP+98,caller overflow arguments | Same |
| +8 | newSP+08,register-save base | Same |

All pointers are original32-bit address words and fields big-endian. General
index3 skips async's three named GPR parameters A/B/format;index2 skips alloc's
A/format. Initial variable GPR is r6 for async,r5 for alloc. FP index starts0
independently of named GPR count. Original varargs descriptor is not a native
host va_list layout;host formatting must preserve types/order/address semantics
through an explicit adapter. Named parameters are snapshotted before registers
are repurposed for the formatter.

Pass format as r4 and descriptor address as r5 to vsprintf. Afterwards set
filename pointer as r3,retained A as r4,and B or0 as r5. Thus the API's
named-prefix order changes at the downstream call boundary. Retaining only a
host format result while dropping original address/argument-order observations
is a source fidelity limitation,not proved by ordinary string-format fixtures.

## Concrete effect-cache use

The delivered driver calls Asyncf(4,0,format,root,prefix,prefix) with integer-only
varargs and condition bit6 cleared. It later calls Allocf(4,format,root,prefix,
prefix) with a separate set of source reads. Format802c1f20 is
`%s/e/%s/effect_%s.tpl`;stable root`.`/prefix`us` yields `./e/us/effect_us.tpl`.
These wrappers do not strip `./`,change case,substitute locale or map the path
to a host/RomFS location. The source provider/backend owns that adaptation.

Driver checks Asyncf's raw return for exact0;on nonzero it invokes Allocf and
stores its raw handle,even0,before selector tag publication. Forwarding wrappers
supply no stronger async/load success guarantee. The effect cache's captured/live
manager/GP/root reads remain the linked driver contract.

Acceptance:both argument reorderings,forced third0 for direct/format allocation,
retained A/B through formatter writes,identical global buffer address,bit6 FP
save versus no-save and all eight GPR slots,03/02 descriptor prefixes/overflow/
register pointers,ignored format result and raw backend result propagation.
Underlying file state/cache/I/O/ownership,format/address/platform domains,TPL
consumers and original resource/runtime traces remain EVT-01/JUMP-01/DMG-02 work.

[Complete acquisition/cache reuse](FILE_ACQUISITION_AND_REFERENCE_REUSE.md) and
[archive/DVD request bodies](FILE_ARCHIVE_AND_DVD_ACQUISITION_REQUESTS.md)
now supply the previously identity-only _fileAlloc contract. fileAsync remains
open;collection/relocation/platform/source and original execution remain separate.

[Complete async request/cache](FILE_ASYNC_REQUEST_AND_CACHE_PUBLICATION.md)
and [completion/request leaf](FILE_ASYNC_COMPLETION_AND_DVD_REQUEST_STATE.md)
now close fileAsync's local graph. Raw-1 on first DVD-open failure is forwarded
unchanged;worker/collection/relocation/source/runtime remain separate.
