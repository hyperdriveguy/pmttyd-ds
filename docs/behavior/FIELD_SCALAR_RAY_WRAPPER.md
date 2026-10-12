# Scalar ray interface to the shared hit backend

US G8ME01 disc0 revision0,ID1,recovered2026-10-12. Evidence B:complete
hitCheckFilter0x8001577c,size0x128,used by
[partner ceiling probes](FIELD_PARTNER_CEILING_QUERY.md). Backend
hitCheckVecFilter0x800158a4,size0xd04 is fingerprinted but only selected
filter/result-layout regions are specified here;full traversal remains open.
The name hitCheckFilter does not make it a dedicated roof-only service.

## Register/pointer ABI

The wrapper takes a callback pointer in r3,point-X/Y/Z output pointers in
r4/r5/r6,a length input/output pointer in r7,and normal-X/Y/Z output pointers
in r8/r9/r10. Floating arguments f1/f2/f3 are origin X/Y/Z;f4/f5/f6 are
direction X/Y/Z. The return value r3 is a hit pointer or null. Caller output
vectors need not be contiguous;the wrapper receives seven independent pointers.
It performs no output-pointer null checks.

Build local query Q at stack+20. Origin is Q+c,direction Q+18,max length
Q+3c. Original three-float templates at0x802bf7b8/0x802bf7c4 initialize
temporary origin/direction buffers,then all components are overwritten from
f1..f6. Copy those float bits into Q and load length from *r7. Call
hitCheckVecFilter(&Q,callback),not a partner-local filter policy.

This wrapper does not explicitly initialize every header/context field of Q.
Full backend ownership/cull/header behavior must establish how they are used;
do not invent a default cull selector solely from this local caller. All
ceiling probes pass callback0. At selected backend callback gates,a null
callback skips invocation;it does not mean a named block/Yoshi filter was run.
Other backend geometry/cull/attribute decisions still apply.

## Null versus success publication

If backend returns null,return null immediately WITHOUT touching any of the
seven caller outputs. The caller's input length therefore remains unchanged,
and output point/normal floats may retain prior or uninitialized values.

On nonnull H,publish these fields in this exact order:

| Store order | Caller destination | Query source | Wrapper store |
| --- | --- | --- | --- |
| 0 | Length pointer r7 | Q+3c |0x8001585c |
| 1 | Point-X pointer r4 | Q+24 |0x80015864 |
| 2 | Point-Y pointer r5 | Q+28 |0x8001586c |
| 3 | Point-Z pointer r6 | Q+2c |0x80015874 |
| 4 | Normal-X pointer r8 | Q+30 |0x8001587c |
| 5 | Normal-Y pointer r9 | Q+34 |0x80015884 |
| 6 | Normal-Z pointer r10 | Q+38 |0x8001588c |

Return H unchanged;there is no boolean conversion or point→position mutation
outside these pointer stores. Alias destinations observe this ordered publication,
not a simultaneous struct replacement. Backend-selected output anchors and the
[front-wall ABI correction](FIELD_PARTNER_FRONT_WALL_QUERY.md) establish
point+24,normal+30;these must not be swapped. The backend derives the point
from origin plus scaled direction and normalizes the retained normal before
its final publication. Exact hit/normal/distance domains and traversal ordering
remain part of the unclosed backend.

## Ceiling binding and validation boundary

Each partySearchHead probe passes null callback,six distinct local point/normal
floats and its own local length pointer. The wrapper can return an intersection
point,but partySearchHead discards it and publishes its OWN sampled coordinates.
Do not use wrapper output semantics to infer a different caller output.

tools/verify_field_partner_head.py checks the complete wrapper and its sole
backend call,argument construction,null skip and every success output store.
The containing backend hash plus selected callback/point/normal anchors support
only those statements. Original execution,scene traversal,all filter policies,
triangle math,Gekko numeric environment and cross-system resource lifetimes
remain open. Native semantic ray adapters may have a different struct ABI,but
must preserve the documented caller-visible outputs and null behavior.
