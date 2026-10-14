# Original key-item95 receive insertion

US G8ME01 disc0 revision0; read-only DOL recovery2026-10-13.
This explicitly **supersedes** [the legacy pouchGetItem stub](POUCH_GET_ITEM.md)
for address, return type and behavior. That stub's800d5520 address is inside
this routine's backwards-shift tail, not an entry point; its item-pointer
lookup/quantity-structure description is contradicted by source bytes.
Original receive entry is800d50cc/4a4,returns integer0/1.

Complements [receive dispatcher](../battle/POUCH_GET_ITEM_DISPATCH.md) and
[field deferred grant](../field/GOOMBA3_ITEM95_GET_SEQUENCE_AND_DEFERRED_GRANT.md).
For item95, source800d5344..5368 admits1<=ID<121, sets base=pouch+a0,
capacity121, parallelbaseNULL. It does not query strange-sack held-item capacity,
search itemDataTable, return an item record, or allocate inventory storage.

Set index120 and cursorbase+240. Scan **descending120..0** using signed16
loads; first encountered value exactly0 is the insertion gap. Negative values,
duplicate95 and every other nonzero value are occupied. Highest-index zero
wins even if lower gaps exist. If no zero,return0 with no pouch writes.

For gap indexk,copy slots k-1..0 one slot right, in descending order:
slot[j+1]=slot[j] for j=k-1 down0. Source unrolls eight halfword moves,
then its remaining count; it preserves the same overlap-safe store order.
Do not compact gaps,sort IDs,remove duplicate95 or shift slots abovek.
After shifts store95 into slot0 and return1. Parallel equipment stores are
skipped because key-item parallelbaseNULL. Other pouch bytes are untouched.

Example first four slots[11,0,22,0],slots4..120occupied:
highest zero k3 ->[95,11,0,22]; the old lower zero persists atindex2.
With zeroat120 and lowergaps, source shifts all120 preceding slots, retaining
all intermediate zero gaps. With zerok0,source stores95 directly.

No parameter revision or sound/UI/event write occurs in this receive leaf.
No introduced quantity cache,count clamp or existing-item suppression applies.
The get-item caller interprets0 as full-storage presentation, not an unknown
pointer or successful grant. Retain raw signed16 source storage semantics.

Evidence: fresh inspect_dol_function.py pouchGetItem reread original ISO;
ignored artifacts/reference/goomba3-services/receive.txt contains full4a4 span,
with key routing5354..5368,descending scan53a8..53c8,backwards shifts54ac..5524,
finalstore552c and returns553c/5550. Documentation only; no native code,
legacy-file rewrite,sharedindex/progress,emulator or saves edited.
